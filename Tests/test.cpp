#include "gtest/gtest.h"

#include "../src/av_data.h"
#include "../src/calgorithms.h"
#include "../src/resource.h" // for ID_METHOD_BUBBLESORT... and others...

TEST(GenerateData, NoValues) {
	std::vector<float> vec;
	GenerateData(vec, DataOrder::doRandomized);
	EXPECT_EQ(vec.size(), 0U);
}

TEST(GenerateData, SortedOrder) {
	std::vector<float> vec;
	vec.resize(100);
	GenerateData(vec, DataOrder::doSorted);
	EXPECT_EQ(vec.size(), 100U);
	EXPECT_EQ(std::is_sorted(std::begin(vec), std::end(vec)), true);
}

TEST(Algorithms, BasicCreation) {
	auto pAlg = AlgorithmFactory::Create(ID_METHOD_BUBBLESORT);
	EXPECT_NE(pAlg, nullptr);
}