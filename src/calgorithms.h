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
#include "ilogger.h"
#include "beat.h"
#include <stack>
#include <memory>
#include <variant>
#include "irendersystem.h"

// class that wraps operations used by algorithms and calculates stats that can be presented later
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
class IAlgorithm {
public:
	struct InitFn {
		CViArray<float>* viData;
		template<typename T>
		inline void operator()(T& alg) const { alg.Init(viData); }
	};

	struct StepFn {
		template<typename T>
		inline void operator()(T& alg) const { alg.Step(); }
	};

	struct GetNameFn {
		template<typename T>
		inline const std::string& operator()(const T& alg) const { return alg.GetName(); }
	};

	struct IsDoneFn {
		template<typename T>
		inline bool operator()(const T& alg) const { return alg.IsDone(); }
	};

	struct GetStatsFn {
		template<typename T>
		inline const AlgOpsWrapper& operator()(const T& alg) const { return alg.GetStats(); }
	};
public:
	explicit IAlgorithm(const std::string& name): m_isDone(false), m_name(name) { }

	void Init(CViArray<float> *viData) { }
	void Step() { }

	const std::string& GetName() const { return m_name; }
	bool IsDone() { return m_isDone; }

	const AlgOpsWrapper& GetStats() const noexcept { return m_stats; }

protected:
	AlgOpsWrapper m_stats;
	std::string m_name;
	CViArray<float>* m_viArray{ nullptr }; // for observing only, we work on that data
	bool m_isDone;
};

// bubble sort technique
class CBubbleSortAlgorithm : public IAlgorithm {
public:
	explicit CBubbleSortAlgorithm() : IAlgorithm("Bubble Sort") { }

	void Init(CViArray<float>* viData);
	void Step();

private:
	size_t m_i{ 0 };
	size_t m_j{ 0 };    // loop iterators
};

// shaker sort, bubble sort but from two ends, left and right at the same time
class CShakerSortAlgorithm : public IAlgorithm {
public:
	explicit CShakerSortAlgorithm() : IAlgorithm("Shaker Sort") { }

	void Init(CViArray<float> *viData);
	void Step();

private:
	size_t m_i{ 0 };
	size_t m_j{ 0 };
	size_t m_j2{ 0 };
};

// a regular selection sort algorithm
class CSelectionSortAlgorithm : public IAlgorithm {
public:
	explicit CSelectionSortAlgorithm() : IAlgorithm("Selection Sort") { }

	void Init(CViArray<float> *viData);
	void Step();

private:
	size_t m_i{ 0 };
	size_t m_j{ 0 };
	size_t m_iMin{ 0 };
};

// a regular insertion sort algorithm
class CInsertionSortAlgorithm : public IAlgorithm {
public:
	explicit CInsertionSortAlgorithm() : IAlgorithm("Insertion Sort") { }

	void Init(CViArray<float> *viData);
	void Step();

private:
	size_t m_i{ 0 };
	size_t m_j{ 0 };
	float m_fValue{ 0.0f };
};

// shell sort
class CShellSortAlgorithm : public IAlgorithm {
public:
	explicit CShellSortAlgorithm() : IAlgorithm("Shell Sort") { }

	void Init(CViArray<float> *viData);
	void Step();

private:
	size_t m_i{ 0 };
	size_t m_j{ 0 };
	size_t m_h{ 0 };
	float m_fValue{ 0.0f };
};

// quick sort
class CQuickSortAlgorithm : public IAlgorithm {
public:
	explicit CQuickSortAlgorithm() : IAlgorithm("Quick Sort") { }

	void Init(CViArray<float> * viData);
	void Step();

private:
	std::stack<size_t> m_stack;
	size_t m_l{ 0 };
	size_t m_h{ 0 };
	size_t m_indexPartition{ 0 };
	size_t m_iter{ 0 };
	float m_valPartition{ 0.0f };
};

// shuffle, demo code, testing
class CShuffleElementsAlgorithm : public IAlgorithm {
public:
	explicit CShuffleElementsAlgorithm() : IAlgorithm("Shuffle Elements") { }

	void Init(CViArray<float> * viData);
	void Step();

private:
	std::vector<int> m_randomOrder;
	size_t m_i{ 0 };
};

using AlgorithmsVariant = std::variant<
	CBubbleSortAlgorithm,
	CShakerSortAlgorithm,
	CSelectionSortAlgorithm,
	CInsertionSortAlgorithm,
	CShellSortAlgorithm,
	CQuickSortAlgorithm,
	CShuffleElementsAlgorithm
>;

// class that has a role of creating new algorithm classes, based on the ID we pass
// for now we use ID that are UI menu options
class AlgorithmFactory
{
public:
	static AlgorithmsVariant Create(uint16_t algID);
};

// binds and manages the currently selected algorithm with the array that we work with
class CAlgManager {
public:
	explicit CAlgManager(const ILogger& logger);
	//~CAlgManager();

	void Update(double fTime);
	void Render(IRenderSystem* avSystem);
	void RunAgain();
	void GenerateData(DataOrder dOrder);
	void RegenerateData();
	void SetAlgorithm(uint16_t algID);
	void SetNumOfElements(size_t iElems);

	void SetTempo(double fTempo) { if (fTempo > 0.0) m_bBeat.SetTempoBPM(fTempo); }
	double GetTempo() { return m_bBeat.GetTempoBPM(); }
	void Pause(bool bPause) { m_bPause = bPause; }
	void SwapPause() { m_bPause = ( m_bPause == true ? false : true ); }
	const std::string& GetAlgorithmName() const;
	size_t GetNumOfElements() { return m_viArrayInitial.size(); }
	std::string GetDataOrderName() { return ToString(m_dOrder); }

	const AlgOpsWrapper& GetCurrentStats() const;

private:
	CBeat m_bBeat;
	AlgorithmsVariant m_CurrentAlg;
	bool m_bPause{ false };
	DataOrder m_dOrder{ DataOrder::doSpecialRandomized };
	CViArray<float> m_viArrayCurrent; // the current array that we operate on
	std::vector<float> m_viArrayInitial; // initial data, we can then run the algorithm again using the same data...
	DataRenderer m_dataRenderer;

	const ILogger& m_logger;
};

#endif // CALGORITHMS_H

// end of file ----------------------------------------------------------------+
