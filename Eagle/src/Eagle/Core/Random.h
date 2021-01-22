#pragma once
#include <random>

namespace Egl {
	class Random {
	public:
		/// <summary>The engine calls this, you don't have to.</summary>
		static float Float01() { return (float)sDistribution(sRandomizer) / (float)std::numeric_limits<uint32_t>::max(); }

		/// <summary>Returns a float between 0 and the max value.</summary>
		static float Float(float max) { return Float01() * max; }
		/// <summary>Returns a float between two values.</summary>
		static float Float(float min, float max) { return Float01() * (max-min) + min; }
	protected:
		friend class Application;
		static void Init() { sRandomizer.seed(std::random_device()()); }
	private:
		static std::mt19937 sRandomizer;
		static std::uniform_int_distribution<std::mt19937::result_type> sDistribution;
	};
}