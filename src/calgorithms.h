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

// the CAlgorithm class -------------------------------------------------------+
class CAlgorithm {
public:
	CAlgorithm(): m_bRunning(false) { }
	virtual ~CAlgorithm() { }

	virtual void Init(CViData *viData) = 0;
	virtual void Step() = 0;
	virtual void Stop() = 0;

	void Pause() { m_bRunning = false; }
	void Resume() { m_bRunning = true; }

	const char *GetName() { return m_strName; }
	bool IsRunning() { return m_bRunning; }

	template <class T> static void Exchange(T &a, T &b) { T t = a; a = b; b = t; ++m_iExchanges; }
	template <class T> static void CompExch(T &a, T &b) { if (IsBigger(a, b)) Exchange(a, b); }
	template <class T> static void CompExch2(T &a, T &b) { if (IsLittler(a, b)) Exchange(a, b); }
	template <class T> static bool IsBigger(T &a, T &b) { ++m_iComparisions; return (a > b); }
	template <class T> static bool IsLittler(T &a, T &b) { ++m_iComparisions; return (a < b); }
	static void NextIteration() { ++m_iIterations; }
	static void NextChange() { ++m_iExchanges; }
	
	static int GetNumOfComparisions() { return m_iComparisions; }
	static int GetNumOfExchanges() { return m_iExchanges; }
	static int GetNumOfIterations() { return m_iIterations; }
	static void ZeroStats() { m_iComparisions = 0; m_iExchanges = 0; m_iIterations = 0; }
private:
	static int m_iComparisions;
	static int m_iExchanges;
	static int m_iIterations;

protected:
	bool m_bRunning;
	char m_strName[64];		// it must be set in constructor
};

// the CBubbleSortAlgorithm class ---------------------------------------------+
class CBubbleSortAlgorithm : public CAlgorithm {
public:
	CBubbleSortAlgorithm();
	~CBubbleSortAlgorithm();

	void Init(CViData *viData);
	void Step();
	void Stop();
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;    // loop iterators
};

// the CShakerSortAlgorithm class ---------------------------------------------+
class CShakerSortAlgorithm : public CAlgorithm {
public:
	CShakerSortAlgorithm();
	~CShakerSortAlgorithm();

	void Init(CViData *viData);
	void Step();
	void Stop();
private:
	CViArray<float> *m_viArray;
	int m_i, m_j, m_j2;    // loop iterators
};

// the CSelectionSortAlgorithm class ------------------------------------------+
class CSelectionSortAlgorithm : public CAlgorithm {
public:
	CSelectionSortAlgorithm();
	~CSelectionSortAlgorithm();

	void Init(CViData *viData);
	void Step();
	void Stop();
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;    // loop iterators
	int m_iMin;
};

// the CInsertionSortAlgorithm class ------------------------------------------+
class CInsertionSortAlgorithm : public CAlgorithm {
public:
	CInsertionSortAlgorithm();
	~CInsertionSortAlgorithm();

	void Init(CViData *viData);
	void Step();
	void Stop();
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;       // loop iterators
	float m_fValue;
};

// the CShellSortAlgorithm class ------------------------------------------+
class CShellSortAlgorithm : public CAlgorithm {
public:
	CShellSortAlgorithm();
	~CShellSortAlgorithm();

	void Init(CViData *viData);
	void Step();
	void Stop();
private:
	CViArray<float> *m_viArray;
	int m_i, m_j;       // loop iterators
	int m_h;
	float m_fValue;
};

// the CShellSortAlgorithm class ------------------------------------------+
class CQuickSortAlgorithm : public CAlgorithm {
public:
	CQuickSortAlgorithm();
	~CQuickSortAlgorithm();

	void Init(CViData* viData);
	void Step();
	void Stop();
private:
	CViArray<float>* m_viArray;
	int m_i, m_j;       // loop iterators
	int m_h;
	float m_fValue;
};

// the CAlgManager class ------------------------------------------------------+
class CAlgManager {
public:
	CAlgManager();
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
private:
	CBeat m_bBeat;
	CAlgorithm *m_alg;
	bool m_bPause;
	DataOrder m_dOrder;
	CViArray<float> m_viArray;
	CViArray<float> m_viArray2;
};

#endif // CALGORITHMS_H

// end of file ----------------------------------------------------------------+
