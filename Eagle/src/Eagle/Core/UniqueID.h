#pragma once
#include <memory>

namespace Egl {
	class UniqueID {
	public:
		static inline uint32_t GetUniqueFrameID() { return mCurrentFrameID++; }

	private:
		static void ResetFrameIDs() { mCurrentFrameID = 0; }
		friend class Application;

		static uint32_t mCurrentFrameID;
	};
}