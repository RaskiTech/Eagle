#pragma once
#include <memory>

namespace Egl {
	// Handy class that gives an unique id for evey frame, though the specific ID can change. 
	// Mostly used for ImGui.
	class UniqueID {
	public:
		static inline uint32_t GetUniqueFrameID() { return mCurrentFrameID++; }

	private:
		static void ResetFrameIDs() { mCurrentFrameID = 0; }
		friend class Application;

		static uint32_t mCurrentFrameID;
	};
}