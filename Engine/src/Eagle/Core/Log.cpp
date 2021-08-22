#include <EaglePCH.h>

namespace Egl {
	LogR::Logger Log::engineLogger;
	LogR::Logger Log::clientLogger;
	
	void Log::Init() {
		engineLogger.SetPrefix("[Eagle]");
		clientLogger.SetPrefix("[App]");

		engineLogger.Log("This is logging and", 4 - 1, "is a number."); // This is logging and 3 is a number.

		const char* amount = "many";
		engineLogger.Warning("There are", amount, "warnings."); // There are many warnings.
		
		engineLogger.Error( LogR::Format("%.2f", 3.14159), "is pi with 2 decimals." ); // 3.14 is pi with 2 decimals.
	}

}