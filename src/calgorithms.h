/*-----------------------------------------------------------------------------+
| File name: calgorithms.h    					   						       |
| Date:		 15 March 2006													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorithm classes thah will be used in Algorithm Visualization system...     |
+-----------------------------------------------------------------------------*/

#ifndef CALGORITHMS_H
	#define CALGORITHMS_H

#include "av_data.h"
#include "ctimer.h"
#include "clog.h"
#include <stack>
#include <memory>


// class that wraps the operations used by algorithms and calculates stats that can be presented later
class AlgOpsWrapper {
public:
	template <class T> void Exchange(T& a, T& b) noexcept { T t = a; a = b; b = t; ++m_iExchanges; }
	template <class T> void CompExch(T& a, T& b) noexcept { if (IsBigger(a, b)) Exchange(a, b); }
	template <class T> bool IsBigger(const T& a, const T& b) noexcept { ++m_iComparisions; return (a > b); }
	template <class T> bool IsSmaller(const T& a, const T& b) noexcept { ++m_iComparisions; return (a < b); }

	void NextIteration() noexcept { ++m_iIterations; }
	void NextChange() noexcept { ++m_iExchanges; }

	uint32_t GetNumOfComparisions() const noexcept { return m_iComparisions; }
	uint32_t GetNumOfExchanges() const noexcept { return m_iExchanges; }
	uint32_t GetNumOfIterations() const noexcept { return m_iIterations; }
	void ZeroStats() noexcept { m_iComparisions = 0; m_iExchanges = 0; m_iIterations = 0; }

private:
	uint32_t m_iComparisions{ 0 };
	uint32_t m_iExchanges{ 0 };
	uint32_t m_iIterations{ 0 };
};

// defines a basic interface for all algorithms
// #refactor: extract AlgStats class
class IAlgorithm {
public:
	explicit IAlgorithm(const std::string& name): m_isDone(false), m_name(name) { }
	virtual ~IAlgorithm() noexcept { }

	virtual void Init(CViData *viData) = 0;
	virtual void Step() = 0;

	const std::string& GetName() { return m_name; }
	bool IsDone() { return m_isDone; }

	const AlgOpsWrapper& GetStats() const noexcept { return m_stats; }

protected:
	bool m_isDone;
	std::string m_name;
	AlgOpsWrapper m_stats;
};

// class that has a role of creating new algorithm classes, based on the ID we pass
// for now we use ID that are UI menu options
class AlgorithmFactory
{
public:
	static std::unique_ptr<IAlgorithm> Create(WORD algID);
};

// the CBubbleSortAlgorithm class ---------------------------------------------+
class CBubbleSortAlgorithm : public IAlgorithm {
public:
	explicit CBubbleSortAlgorithm() : IAlgorithm("Bubble Sort") { }

	void Init(CViData* viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	int m_j{ 0 };    // loop iterators
};

// the CShakerSortAlgorithm class ---------------------------------------------+
class CShakerSortAlgorithm : public IAlgorithm {
public:
	explicit CShakerSortAlgorithm() : IAlgorithm("Shaker Sort") { }

	void Init(CViData *viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	int m_j{ 0 };
	int m_j2{ 0 };
};

// the CSelectionSortAlgorithm class ------------------------------------------+
class CSelectionSortAlgorithm : public IAlgorithm {
public:
	explicit CSelectionSortAlgorithm() : IAlgorithm("Selection Sort") { }

	void Init(CViData *viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	int m_j{ 0 };
	int m_iMin{ 0 };
};

// the CInsertionSortAlgorithm class ------------------------------------------+
class CInsertionSortAlgorithm : public IAlgorithm {
public:
	explicit CInsertionSortAlgorithm() : IAlgorithm("Insertion Sort") { }

	void Init(CViData *viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	int m_j{ 0 };
	float m_fValue{ 0.0f };
};

// the CShellSortAlgorithm class ------------------------------------------+
class CShellSortAlgorithm : public IAlgorithm {
public:
	explicit CShellSortAlgorithm() : IAlgorithm("Shell Sort") { }

	void Init(CViData *viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	int m_j{ 0 };
	int m_h{ 0 };
	float m_fValue{ 0.0f };
};

// the CShellSortAlgorithm class ------------------------------------------+
class CQuickSortAlgorithm : public IAlgorithm {
public:
	explicit CQuickSortAlgorithm() : IAlgorithm("Quick Sort") { }

	void Init(CViData* viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_l{ 0 };
	int m_h{ 0 };
	std::stack<int> m_stack;
	float m_valPartition{ 0.0f };
	int m_indexPartition{ 0 };
	int m_iter{ 0 };
};

// the CShellSortAlgorithm class ------------------------------------------+
class CShuffleElementsAlgorithm : public IAlgorithm {
public:
	explicit CShuffleElementsAlgorithm() : IAlgorithm("Shuffle Elements") { }

	void Init(CViData* viData) override;
	void Step() override;

private:
	CViArray<float>* m_viArray{ nullptr };
	int m_i{ 0 };
	std::vector<int> m_randomOrder;
};

// binds and manages the currently selected algorithm with the array that we work with
class CAlgManager {
public:
	explicit CAlgManager(const CLog& logger);
	~CAlgManager();

	void Update();
	void Render(CAVSystem *avSystem);
	void RunAgain();
	void GenerateData(DataOrder dOrder);
	void RegenerateData();
	void SetAlgorithm(WORD algID);
	void SetNumOfElements(int iElems);

	void SetTempo(double fTempo) { if (fTempo > 0.0) m_bBeat.SetTempoBPM(fTempo); }
	double GetTempo() { return m_bBeat.GetTempoBPM(); }
	void Pause(bool bPause) { m_bPause = bPause; }
	void SwapPause() { m_bPause = ( m_bPause == true ? false : true ); }
	const std::string& GetAlgorithmName() { return m_pCurrentAlg->GetName(); }
	int GetNumOfElements() { return m_viArray2.GetSize(); }
	std::string GetDataOrderName() { return ToString(m_dOrder); }

	const AlgOpsWrapper& GetCurrentStats() const { return m_pCurrentAlg->GetStats(); }

private:
	CBeat m_bBeat;
	std::unique_ptr<IAlgorithm> m_pCurrentAlg;
	bool m_bPause{ false };
	DataOrder m_dOrder{ DataOrder::doSpecialRandomized };
	CViArray<float> m_viArray;
	CViArray<float> m_viArray2;
	std::vector<float> m_array; // the array that all algorithms operate on

	const CLog& m_logger;
};

#endif // CALGORITHMS_H

// end of file ----------------------------------------------------------------+
