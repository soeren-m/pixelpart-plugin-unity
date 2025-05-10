#ifndef PIXELPART_UTIL_H
#define PIXELPART_UTIL_H

#include "PixelpartUnityTypes.h"
#include "pixelpart-runtime/common/Types.h"
#include "pixelpart-runtime/common/Math.h"
#include "pixelpart-runtime/common/ThreadPool.h"
#include "pixelpart-runtime/effect/Effect.h"
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

namespace internal {
UnityInt toUnity(pixelpart::int_t v);
UnityFloat toUnity(pixelpart::float_t v);
UnityVector2 toUnity(const pixelpart::float2_t& v);
UnityVector3 toUnity(const pixelpart::float3_t& v);
UnityVector4 toUnity(const pixelpart::float3_t& v, pixelpart::float_t w);
UnityVector4 toUnity(const pixelpart::float4_t& v);
UnityColor toUnityColor(const pixelpart::float4_t& v);

pixelpart::int_t fromUnity(UnityInt v);
pixelpart::float_t fromUnity(UnityFloat v);
pixelpart::float2_t fromUnity(const UnityVector2& v);
pixelpart::float3_t fromUnity(const UnityVector3& v);
pixelpart::float4_t fromUnity(const UnityVector4& v);
pixelpart::float4_t fromUnity(const UnityColor& v);

pixelpart::float3_t rotate2d(const pixelpart::float3_t& p, const pixelpart::float3_t& o, pixelpart::float_t a);
pixelpart::mat3_t rotation3d(const pixelpart::float3_t& angle);
pixelpart::mat3_t lookAt(const pixelpart::float3_t& direction);

template <typename T>
std::vector<typename pixelpart::Curve<T>::Point> orderedPoints(const pixelpart::float_t* positionList, const T* valueList, const uint32_t* order, uint32_t count) {
	std::vector<typename pixelpart::Curve<T>::Point> result;
	result.resize(count);
	for(uint32_t index = 0; index < count; index++) {
		result[index] = typename pixelpart::Curve<T>::Point{ positionList[index], valueList[order[index]] };
	}

	return result;
}

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

#ifdef PIXELPART_RUNTIME_MULTITHREADING
template <typename It, typename Comp = std::less<typename std::iterator_traits<It>::value_type>>
void parallelInsertionSort(pixelpart::ThreadPool& threadPool, std::size_t minWorkSize, It begin, It end, Comp comp = Comp()) {
	std::size_t length = std::distance(begin, end);
	if(length < 2) {
		return;
	}

	std::size_t threadCount = std::min(length / std::max(minWorkSize, static_cast<std::size_t>(1)), threadPool.threadCount());
	if(threadCount <= 1) {
		insertionSort(begin, end, comp);
		return;
	}

	std::vector<std::pair<std::size_t, std::size_t>> workgroups;
	std::size_t threadCountWithLargerLoad = length % threadCount;
	std::size_t workSize = length / threadCount;
	std::size_t workgroupStart = 0;

	for(std::size_t t = 0; t < threadCount; t++) {
		std::size_t workgroupSize = (t < threadCount - threadCountWithLargerLoad) ? workSize : workSize + 1;
		std::size_t workgroupEnd = workgroupStart + workgroupSize;

		threadPool.enqueue(static_cast<uint32_t>(t),
			insertionSort<It, Comp>,
			std::next(begin, workgroupStart),
			std::next(begin, workgroupEnd),
			comp);

		workgroups.emplace_back(std::make_pair(workgroupStart, workgroupEnd));
		workgroupStart += workgroupSize;
	}

	for(std::size_t t = 0; t < threadCount; t++) {
		threadPool.wait(static_cast<uint32_t>(t));
	}

	for(std::size_t t = 1; t < threadCount; t++) {
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

#endif