#pragma once

#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <cmath>
#include <thread>
#include <std::vector>
#include <fstream>
#include <string>
#include <atomic>
#include <condition_variable>
#include <Windows.h>

const double PI = 2.0 * acos(0.0);

template<class T>
class NoiseMaker
{
public:
	NoiseMaker(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512)
	{
		Create(sOutputDevice, nSampleRate, nChannels, nBlocks, nBlockSamples);
	}

	~NoiseMaker()
	{
		Destroy();
	}

	bool Create(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512)
	{
		mReady = false;
		mSampleRate = nSampleRate;
		mChannels = nChannels;
		mBlockCount = nBlocks;
		mBlockSamples = nBlockSamples;
		mBlockFree = mBlockCount;
		mBlockCurrent = 0;
		mBlockMemory = nullptr;
		mWaveHeaders = nullptr;

		mUserFunction = nullptr;

		// Validate device
		std::vector<wstring> devices = Enumerate();
		auto d = std::find(devices.begin(), devices.end(), sOutputDevice);
		if (d != devices.end())
		{
			// Device is available
			int nDeviceID = distance(devices.begin(), d);
			WAVEFORMATEX waveFormat;
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nSamplesPerSec = mSampleRate;
			waveFormat.wBitsPerSample = sizeof(T) * 8;
			waveFormat.nChannels = mChannels;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
			waveFormat.cbSize = 0;

			// Open Device if valid
			if (waveOutOpen(&m_hwDevice, nDeviceID, &waveFormat, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)this, CALLBACK_FUNCTION) != S_OK)
				return Destroy();
		}

		// Allocate Wave|Block Memory
		mBlockMemory = new T[mBlockCount * mBlockSamples];
		if (mBlockMemory == nullptr)
			return Destroy();
		ZeroMemory(mBlockMemory, sizeof(T) * mBlockCount * mBlockSamples);

		mWaveHeaders = new WAVEHDR[mBlockCount];
		if (mWaveHeaders == nullptr)
			return Destroy();
		ZeroMemory(mWaveHeaders, sizeof(WAVEHDR) * mBlockCount);

		// Link headers to block memory
		for (unsigned int n = 0; n < mBlockCount; n++)
		{
			mWaveHeaders[n].dwBufferLength = mBlockSamples * sizeof(T);
			mWaveHeaders[n].lpData = (LPSTR)(mBlockMemory + (n * mBlockSamples));
		}

		mReady = true;

		m_thread = thread(&NoiseMaker::MainThread, this);

		// Start the ball rolling
		unique_lock<mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();

		return true;
	}

	bool Destroy()
	{
		return false;
	}

	void Stop()
	{
		mReady = false;
		m_thread.join();
	}

	// Override to process current sample
	virtual double UserProcess(double dTime)
	{
		return 0.0;
	}

	double GetTime()
	{
		return m_dGlobalTime;
	}



public:
	static std::vector<wstring> Enumerate()
	{
		int nDeviceCount = waveOutGetNumDevs();
		std::vector<wstring> sDevices;
		WAVEOUTCAPS woc;
		for (int n = 0; n < nDeviceCount; n++)
			if (waveOutGetDevCaps(n, &woc, sizeof(WAVEOUTCAPS)) == S_OK)
				sDevices.push_back(woc.szPname);
		return sDevices;
	}

	void SetUserFunction(double(*func)(double))
	{
		mUserFunction = func;
	}

	double clip(double dSample, double dMax)
	{
		if (dSample >= 0.0)
			return fmin(dSample, dMax);
		else
			return fmax(dSample, -dMax);
	}


private:
	double(*mUserFunction)(double);

	unsigned int mSampleRate;
	unsigned int mChannels;
	unsigned int mBlockCount;
	unsigned int mBlockSamples;
	unsigned int mBlockCurrent;

	T* mBlockMemory;
	WAVEHDR* mWaveHeaders;
	HWAVEOUT m_hwDevice;

	thread m_thread;
	atomic<bool> mReady;
	atomic<unsigned int> mBlockFree;
	condition_variable m_cvBlockNotZero;
	mutex m_muxBlockNotZero;

	atomic<double> m_dGlobalTime;

	// Handler for soundcard request for more data
	void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2)
	{
		if (uMsg != WOM_DONE) return;

		mBlockFree++;
		unique_lock<mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();
	}

	// Static wrapper for sound card handler
	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		((NoiseMaker*)dwInstance)->waveOutProc(hWaveOut, uMsg, dwParam1, dwParam2);
	}

	// Main thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void MainThread()
	{
		m_dGlobalTime = 0.0;
		double dTimeStep = 1.0 / (double)mSampleRate;

		// Goofy hack to get maximum integer for a type at run-time
		T nMaxSample = (T)pow(2, (sizeof(T) * 8) - 1) - 1;
		double dMaxSample = (double)nMaxSample;
		T nPreviousSample = 0;

		while (mReady)
		{
			// Wait for block to become available
			if (mBlockFree == 0)
			{
				unique_lock<mutex> lm(m_muxBlockNotZero);
				m_cvBlockNotZero.wait(lm);
			}

			// Block is here, so use it
			mBlockFree--;

			// Prepare block for processing
			if (mWaveHeaders[mBlockCurrent].dwFlags & WHDR_PREPARED)
				waveOutUnprepareHeader(m_hwDevice, &mWaveHeaders[mBlockCurrent], sizeof(WAVEHDR));

			T nNewSample = 0;
			int nCurrentBlock = mBlockCurrent * mBlockSamples;

			for (unsigned int n = 0; n < mBlockSamples; n++)
			{
				// User Process
				if (mUserFunction == nullptr)
					nNewSample = (T)(clip(UserProcess(m_dGlobalTime), 1.0) * dMaxSample);
				else
					nNewSample = (T)(clip(mUserFunction(m_dGlobalTime), 1.0) * dMaxSample);

				mBlockMemory[nCurrentBlock + n] = nNewSample;
				nPreviousSample = nNewSample;
				m_dGlobalTime = m_dGlobalTime + dTimeStep;
			}

			// Send block to sound device
			waveOutPrepareHeader(m_hwDevice, &mWaveHeaders[mBlockCurrent], sizeof(WAVEHDR));
			waveOutWrite(m_hwDevice, &mWaveHeaders[mBlockCurrent], sizeof(WAVEHDR));
			mBlockCurrent++;
			mBlockCurrent %= mBlockCount;
		}
	}
};