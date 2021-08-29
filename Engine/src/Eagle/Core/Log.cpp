#include <EaglePCH.h>

namespace Egl {
	LogR::Logger Log::engineLogger;
	LogR::Logger Log::clientLogger;
	
	void Log::Init() {
		engineLogger.SetPrefix("[Eagle]");
		clientLogger.SetPrefix("[App]");
	}

}