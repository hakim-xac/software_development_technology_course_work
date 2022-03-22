#pragma once
#include "benchmark.h"

namespace BENCHMARK {


	BenchMark::BenchMark() : now_(std::chrono::steady_clock::now()) {}
	BenchMark::BenchMark(const BenchMark& lhs) : now_(lhs.now_) {}


	std::chrono::milliseconds BenchMark::diffirence(const BenchMark& time) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(now_ - time.now_);
	}
}