#pragma once
#include <chrono>

namespace BENCHMARK {
	class BenchMark {

		std::chrono::steady_clock::time_point now_;
	public:

		BenchMark();
		std::chrono::milliseconds diffirence(const BenchMark& time);
	};
}

