#pragma once

namespace Egl {
	class Time {
	public:
		static float GetDeltaTime() { return Get().mDeltaTime; }
		static float GetDeltaTimeMilliseconds() { return Get().mDeltaTime; }
		static float GetSeconds() { return Get().mTime; }
		static float GetMilliseconds() { return Get().mTime * 1000; }

	protected:
		static void SetTime(float time, float deltaTime) {
			Get().mTime = time;
			Get().mDeltaTime = deltaTime;
		}
		friend class Application;

	private:
		Time() : mTime(0), mDeltaTime(0) {}
		static Time& Get() {
			static Time time;
			return time;
		}

		float mTime;
		float mDeltaTime;
	};
}