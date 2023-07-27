#pragma once

#include "ThreadPool.h"
#include <algorithm>

namespace util {
template<typename It, typename Comp>
std::vector<typename It::value_type> merge(It begin, It mid, It end, Comp comp) {
	std::vector<typename It::value_type> result;
	result.reserve(std::distance(begin, end));

	It left = begin;
	It right = mid;

	while(left != mid && right != end) {
		result.push_back(comp(*left, *right)
			? *left++
			: *right++);
	}

	result.insert(result.end(), left, mid);
	result.insert(result.end(), right, end);

	return result;
}

template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void insertionSort(It begin, It end, Comp comp = Comp()) {
	std::iter_swap(begin, std::min_element(begin, end, comp));

	for(It i = begin; ++i < end; begin = i) {
		for(It j = i; comp(*j, *begin); --j, --begin) {
			std::iter_swap(begin, j);
		}
	}
}

#ifndef __EMSCRIPTEN__
template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void parallelInsertionSort(pixelpart::ThreadPool& threadPool, std::size_t minWorkSize, It begin, It end, Comp comp = Comp()) {
	std::size_t length = std::distance(begin, end);
	if(length < 2u) {
		return;
	}

	std::size_t numThreads = std::min(length / std::max(minWorkSize, (std::size_t)1u), threadPool.getNumThreads());
	if(numThreads <= 1) {
		insertionSort(begin, end, comp);
		return;
	}

	std::vector<std::pair<std::size_t, std::size_t>> workgroups;
	std::size_t numThreadsWithLargerLoad = length % numThreads;
	std::size_t workSize = length / numThreads;
	std::size_t workgroupStart = 0u;

	for(std::size_t t = 0; t < numThreads; t++) {
		std::size_t workgroupSize = (t < numThreads - numThreadsWithLargerLoad) ? workSize : workSize + 1u;
		std::size_t workgroupEnd = workgroupStart + workgroupSize;

		threadPool.enqueue(static_cast<uint32_t>(t),
			insertionSort<It, Comp>,
			std::next(begin, workgroupStart),
			std::next(begin, workgroupEnd),
			comp);

		workgroups.emplace_back(std::make_pair(workgroupStart, workgroupEnd));
		workgroupStart += workgroupSize;
	}

	for(std::size_t t = 0; t < numThreads; t++) {
		threadPool.wait(static_cast<uint32_t>(t));
	}

	for(std::size_t t = 1; t < numThreads; t++) {
		auto&& result = merge(
			begin,
			std::next(begin, workgroups[t].first),
			std::next(begin, workgroups[t].second),
			comp);

		std::move(result.cbegin(), result.cend(), begin);
	}
}
#endif
}