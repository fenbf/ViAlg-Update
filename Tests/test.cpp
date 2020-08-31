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
	auto alg = AlgorithmFactory::Create(ID_METHOD_BUBBLESORT);
	EXPECT_EQ(0, alg.index());
}

class TestRenderer : public IRenderSystem {
public:
	void SetDiagramBlockInfo(BlockType bType, const VECTOR3D& vCol, const VECTOR3D& vColMarked, const VECTOR3D& vColHighlighted) override 	{	
	}

	void BeginDrawing(double fMaxData, int nCount) override {
	}

	void DrawDiagramBlock(double fValue, ColorType cType) override	{
		++numDrawCalls;
	}

	void EndDrawing() override	{
	}

	void SetMaxSize(float fWidth, float fHeight, float fDepth) override	{
	}

	void SetOutlook(const VECTOR3D& vFrameCol, unsigned int iFrameTex) override	{
	}

	void SetBlockType(BlockType bType) override	{
	}

	size_t numDrawCalls{ 0 };
};

class TestLogger : public ILogger {

public:
	bool Init(std::string filename) noexcept override {
		return true;
	}

	void AddMsg(LogMode lmMode, const char* szMsg, ...) const noexcept override {
	}
};

TEST(Decoupling, Rendering) {
	TestLogger testLogger;
	CAlgManager mgr(testLogger);
	TestRenderer testRenderer;

	constexpr size_t NumElements = 100;

	mgr.SetTempo(3000.0);
	mgr.SetNumOfElements(NumElements);
	mgr.GenerateData(DataOrder::doSpecialRandomized);
	mgr.SetAlgorithm(ID_METHOD_QUICKSORT);
	mgr.Render(&testRenderer);

	EXPECT_EQ(testRenderer.numDrawCalls, NumElements);
}