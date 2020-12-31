#pragma once

namespace Egl {
	class Time {
	public:
		static float GetFrameDelta() { return Get().mFrameDelta; }
		static float GetFrameDeltaMilliseconds() { return Get().mFrameDelta * 1000; }
		static float GetSeconds() { return Get().mTime; }
		static float GetMilliseconds() { return Get().mTime * 1000; }

	protected:
		static void SetTime(float time, float frameDelta) {
			Get().mTime = time;
			Get().mFrameDelta = frameDelta;
		}
		friend class Application;

	private:
		Time() : mTime(0), mFrameDelta(0) {}
		static Time& Get() {
			static Time time;
			return time;
		}

		float mTime;
		float mFrameDelta;
	};
}