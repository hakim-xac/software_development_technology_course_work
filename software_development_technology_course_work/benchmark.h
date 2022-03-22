#pragma once
#include <chrono>

namespace BENCHMARK {
	class BenchMark {

		std::chrono::steady_clock::time_point now_;
	public:

		BenchMark();
		BenchMark(const BenchMark& lhs);
		std::chrono::milliseconds diffirence(const BenchMark& time);
	};
}

