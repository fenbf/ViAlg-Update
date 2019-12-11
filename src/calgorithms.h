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

// class that wraps the operations used by algorithms and cals stats that can be presented later
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
class CAlgorithm {
public:
	explicit CAlgorithm(const CLog& logger): m_bRunning(false), m_logger(logger) { }
	virtual ~CAlgorithm() { }

	virtual void Init(CViData *viData) = 0;
	virtual void Step() = 0;
	virtual void Stop() = 0;

	void Pause() { m_bRunning = false; }
	void Resume() { m_bRunning = true; }

	const char *GetName() { return m_strName; }
	bool IsRunning() { return m_bRunning; }

	const AlgOpsWrapper& GetStats() const noexcept { return m_stats; }

protected:
	bool m_bRunning;
	char m_strName[64]{ 0 };		// it must be set in constructor
	const CLog& m_logger;
	AlgOpsWrapper m_stats;
};

// the CBubbleSortAlgorithm class ---------------------------------------------+
class CBubbleSortAlgorithm : public CAlgorithm {
public:
	explicit CBubbleSortAlgorithm(const CLog& logger);
	~CBubbleSortAlgorithm();

	void Init(CViData* viData) override;
	void Step() override;
	void Stop() override;
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;    // loop iterators
};

// the CShakerSortAlgorithm class ---------------------------------------------+
class CShakerSortAlgorithm : public CAlgorithm {
public:
	explicit CShakerSortAlgorithm(const CLog& logger);
	~CShakerSortAlgorithm();

	void Init(CViData *viData) override;
	void Step() override;
	void Stop() override;

private:
	CViArray<float> *m_viArray;
	int m_i, m_j, m_j2;    // loop iterators
};

// the CSelectionSortAlgorithm class ------------------------------------------+
class CSelectionSortAlgorithm : public CAlgorithm {
public:
	explicit CSelectionSortAlgorithm(const CLog& logger);
	~CSelectionSortAlgorithm();

	void Init(CViData *viData) override;
	void Step() override;
	void Stop() override;

private:
	CViArray<float> *m_viArray;
	int m_i, m_j;    // loop iterators
	int m_iMin;
};

// the CInsertionSortAlgorithm class ------------------------------------------+
class CInsertionSortAlgorithm : public CAlgorithm {
public:
	explicit CInsertionSortAlgorithm(const CLog& logger);
	~CInsertionSortAlgorithm();

	void Init(CViData *viData) override;
	void Step() override;
	void Stop() override;

private:
	CViArray<float> *m_viArray;
	int m_i, m_j;       // loop iterators
	float m_fValue;
};

// the CShellSortAlgorithm class ------------------------------------------+
class CShellSortAlgorithm : public CAlgorithm {
public:
	explicit CShellSortAlgorithm(const CLog& logger);
	~CShellSortAlgorithm();

	void Init(CViData *viData) override;
	void Step() override;
	void Stop() override;
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;       // loop iterators
	int m_h;
	float m_fValue;
};

// the CShellSortAlgorithm class ------------------------------------------+
class CQuickSortAlgorithm : public CAlgorithm {
public:
	explicit CQuickSortAlgorithm(const CLog& logger);
	~CQuickSortAlgorithm();

	void Init(CViData* viData) override;
	void Step() override;
	void Stop() override;

private:
	CViArray<float>* m_viArray;
	int m_l;
	int m_h;
	std::stack<int> m_stack;
	float m_valPartition;
	int m_indexPartition;
	int m_iter;
};

// the CShellSortAlgorithm class ------------------------------------------+
class CShuffleElementsAlgorithm : public CAlgorithm {
public:
	explicit CShuffleElementsAlgorithm(const CLog& logger);
	~CShuffleElementsAlgorithm();

	void Init(CViData* viData) override;
	void Step() override;
	void Stop() override;

private:
	CViArray<float>* m_viArray;
	int m_i;
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
	void SetAlgorithm(CAlgorithm *cAlg);
	void SetNumOfElements(int iElems);

	void SetTempo(double fTempo) { if (fTempo > 0.0) m_bBeat.SetTempoBPM(fTempo); }
	double GetTempo() { return m_bBeat.GetTempoBPM(); }
	void Pause(bool bPause) { m_bPause = bPause; }
	void SwapPause() { m_bPause = ( m_bPause == true ? false : true ); }
	const char *GetAlgorithmName() { return m_alg->GetName(); }
	int GetNumOfElements() { return m_viArray2.GetSize(); }
	const char* GetDataOrderName() { return strDataOrderNames[(int)m_dOrder]; }

	const AlgOpsWrapper& GetCurrentStats() const { return m_alg->GetStats(); }

private:
	CBeat m_bBeat;
	CAlgorithm *m_alg;
	bool m_bPause;
	DataOrder m_dOrder;
	CViArray<float> m_viArray;
	CViArray<float> m_viArray2;
	std::vector<float> m_array; // the array that all algorithms operate on

	const CLog& m_logger;
};

#endif // CALGORITHMS_H

// end of file ----------------------------------------------------------------+
