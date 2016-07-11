/*-----------------------------------------------------------------------------+
| File name: calgorithms.cpp   					   						       |
| Date:		 15 March 2006													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorithm classes thah will be used in Algorithm Visualization system...     |
| Implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "calgorithms.h"

/*-----------------------------------------------------------------------------+
|                  Implementation of the CAlgorithm class                      |
+-----------------------------------------------------------------------------*/

int CAlgorithm::m_iComparisions = 0;
int CAlgorithm::m_iExchanges = 0;
int CAlgorithm::m_iIterations = 0;

/*-----------------------------------------------------------------------------+
|               Implementation of the CBubbleSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

CBubbleSortAlgorithm::CBubbleSortAlgorithm():
    m_i(0),
	m_j(0)
{
	m_viArray = NULL;
	sprintf_s(m_strName, "Bubble Sort");

	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

CBubbleSortAlgorithm::~CBubbleSortAlgorithm() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Init method ------------------------------------------------------------+
void CBubbleSortAlgorithm::Init(CViData *viData) {
	m_viArray = dynamic_cast < CViArray<float> *>(viData);
	
	m_i = 0;
	m_j = m_viArray->GetSize()-1;
	m_viArray->SetSection(m_i, m_j);
	CAlgorithm::ZeroStats();
	m_bRunning = true;
}

// the Step method ------------------------------------------------------------+
void CBubbleSortAlgorithm::Step() {
	if (!m_bRunning) return;

	// normal code of the bubble sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         for (j = m_viArray.GetSize()-1; j > i; --j)
	//             CAlgorithm::CompExch(m_viArray[j-1], m_viArray[j]);
	//     }

	if (m_j > m_i) {
		CAlgorithm::NextIteration();
		CAlgorithm::CompExch((*m_viArray)[m_j-1],(*m_viArray)[m_j]);
		--m_j;
	}
	else {
		++m_i;

		if (m_i >= m_viArray->GetSize()-1) m_bRunning = false;

		m_j = m_viArray->GetSize()-1;

		m_viArray->SetSection(m_i, m_j);
	}
}

// the stop method ------------------------------------------------------------+
void CBubbleSortAlgorithm::Stop() {
	m_i = 0;
	m_j = m_viArray->GetSize()-1;
	m_viArray->SetSection(-1, -1);
	m_bRunning = false;
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CShakerSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

CShakerSortAlgorithm::CShakerSortAlgorithm():
    m_i(0),
	m_j(0),
	m_j2(0)
{
	m_viArray = NULL;
	sprintf_s(m_strName, "Shaker Sort");

	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

CShakerSortAlgorithm::~CShakerSortAlgorithm() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Init method ------------------------------------------------------------+
void CShakerSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 0;
	m_j = m_viArray->GetSize()-1;    // backward
	m_j2 = 0;                       // forward
	m_viArray->SetSection(m_i, m_j);
	CAlgorithm::ZeroStats();
	m_bRunning = true;
}

// the Step method ------------------------------------------------------------+
void CShakerSortAlgorithm::Step() {
	if (!m_bRunning) return;

	// normal code of the bubble sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         for (j = m_viArray.GetSize()-1, j2 = i; j > i; --j, --j2) {
	//             CAlgorithm::CompExch(m_viArray[j-1], m_viArray[j]);
	//             CAlgorithm::CompExch2(m_viArray[j], m_viArray[j+1]);
	//         }
	//     }

	if (m_j > m_i) {
		CAlgorithm::NextIteration();
		CAlgorithm::CompExch((*m_viArray)[m_j-1], (*m_viArray)[m_j]);
		CAlgorithm::CompExch2((*m_viArray)[m_j2+1], (*m_viArray)[m_j2]);
		m_viArray->SetAdditionalMark(m_j-1);
		--m_j;
		++m_j2;
		return;
	}

	++m_i;

	m_viArray->SetAdditionalMark(-1);
	if (m_i >= m_viArray->GetSize()-1) m_bRunning = false;

	m_j = m_viArray->GetSize()-1-m_i;
	m_j2 = m_i;

	m_viArray->SetSection(m_i, m_j);
}

// the stop method ------------------------------------------------------------+
void CShakerSortAlgorithm::Stop() {
	m_i = 0;
	m_j = m_viArray->GetSize()-1;
	m_viArray->SetSection(-1, -1);
	m_bRunning = false;
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CSelectionSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

CSelectionSortAlgorithm::CSelectionSortAlgorithm():
    m_i(0),
	m_j(0),
	m_iMin(0)
{
	m_viArray = NULL;
	sprintf_s(m_strName, "Selection Sort");

	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

CSelectionSortAlgorithm::~CSelectionSortAlgorithm() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Init method ------------------------------------------------------------+
void CSelectionSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 0;
	m_j = 1;
	m_iMin = m_i;
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	CAlgorithm::ZeroStats();
	m_bRunning = true;
}

// the Step method ------------------------------------------------------------+
void CSelectionSortAlgorithm::Step() {
	if (!m_bRunning) return;

	// normal code of the selection sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         m_iMin = i;
	//         for (j = i+1; j < m_viArray.GetSize(); ++j)
	//             if (m_viArray[j] < m_viArray[m_iMin]) m_iMin = j;
	//         Exchange(m_viArray[i], m_viArray[m_iMin]);
	//     }

	if (m_j < m_viArray->GetSize()) {
		CAlgorithm::NextIteration();
		if (CAlgorithm::IsLittler((*m_viArray)[m_j], (*m_viArray)[m_iMin])) m_iMin = m_j;
		
		++m_j;
		return;
	}
	
	Exchange((*m_viArray)[m_i], (*m_viArray)[m_iMin]);
	++m_i;
	m_j = m_i+1;
	m_iMin = m_i;
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);

	if (m_i >= m_viArray->GetSize()-1) m_bRunning = false;
}

// the stop method ------------------------------------------------------------+
void CSelectionSortAlgorithm::Stop() {
	m_i = 0;
	m_j = 1;
	m_viArray->SetSection(-1, -1);
	m_bRunning = false;
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CInsertionSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

CInsertionSortAlgorithm::CInsertionSortAlgorithm():
    m_i(0),
	m_j(0),
	m_fValue(0.0f)
{
	m_viArray = NULL;
	sprintf_s(m_strName, "Insertion Sort");

	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

CInsertionSortAlgorithm::~CInsertionSortAlgorithm() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Init method ------------------------------------------------------------+
void CInsertionSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 1;
	m_j = 1;
	m_fValue = (*m_viArray)[1];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	CAlgorithm::ZeroStats();
	m_bRunning = true;
}

// the Step method ------------------------------------------------------------+
void CInsertionSortAlgorithm::Step() {
	if (!m_bRunning) return;

	// normal code of the insertion sort algorith is:
	//     for (i = 1; i < m_viArray.GetSize(); ++i) {
	//         m_fValue = m_viArray[i];
	//         j = i;
	//         while (j > 0 && m_viArray[j-1] > m_fValue) {
	//             m_viarray[j] = m_viArray[j-1];
	//             --j;
	//         }
	//         m_viArray[j] = m_fValue;
	//     }

	if (m_j > 0 && CAlgorithm::IsBigger((*m_viArray)[m_j-1], m_fValue)) {
		CAlgorithm::NextIteration();
		(*m_viArray)[m_j] = (*m_viArray)[m_j-1];
		CAlgorithm::NexChange();
		
		--m_j;
		return;
	}
	
	(*m_viArray)[m_j] = m_fValue;
	CAlgorithm::NexChange();
	++m_i;

	if (m_i >= m_viArray->GetSize()) { m_bRunning = false; return; }

	m_j = m_i;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
}

// the stop method ------------------------------------------------------------+
void CInsertionSortAlgorithm::Stop() {
	m_i = 0;
	m_j = 1;
	m_viArray->SetSection(-1, -1);
	m_bRunning = false;
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CShellSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

CShellSortAlgorithm::CShellSortAlgorithm():
    m_i(0),
	m_j(0),
	m_h(0),
	m_fValue(0.0f)
{
	m_viArray = NULL;
	sprintf_s(m_strName, "Shell Sort");

	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

CShellSortAlgorithm::~CShellSortAlgorithm() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Init method ------------------------------------------------------------+
void CShellSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	for (m_h = 1; m_h < m_viArray->GetSize()/9; m_h = 3*m_h+1);
	m_i = m_h;
	m_j = m_h;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	CAlgorithm::ZeroStats();
	m_bRunning = true;
}

// the Step method ------------------------------------------------------------+
void CShellSortAlgorithm::Step() {
	if (!m_bRunning) return;

	// normal code of the insertion sort algorith is:
	//    for (h = 1; h < m_viArray.GetSize()/9; h = 3*h+1); 
	//    for ( ; h > 0; h =/ 3) {
	//        for (i = h; i < m_viArray.GetSize(); ++i) {
	//            m_fValue = m_viArray[i];
	//            j = i;
	//            while (j > 0 && m_viArray[j-h] > m_fValue) {
	//                m_viarray[j] = m_viArray[j-h];
	//                --j;
	//            }
	//            m_viArray[j] = m_fValue;
	//        }   
	//    }    
	CAlgorithm::NextIteration();

	if (m_j >= m_h && CAlgorithm::IsBigger((*m_viArray)[m_j-m_h], m_fValue)) {
		(*m_viArray)[m_j] = (*m_viArray)[m_j-m_h];
		CAlgorithm::NexChange();
		
		m_j-=m_h;
		return;
	}
	
	(*m_viArray)[m_j] = m_fValue;
	CAlgorithm::NexChange();
	++m_i;

	if (m_i >= m_viArray->GetSize()) {
		m_h /= 3;
		if (m_h < 1) { m_bRunning = false; return; }
		m_i = m_h;
	}

	m_j = m_i;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);

	sprintf_s(m_strName, "Shell Sort - %d sorting", m_h);
}

// the stop method ------------------------------------------------------------+
void CShellSortAlgorithm::Stop() {
	m_i = 0;
	m_j = 1;
	m_viArray->SetSection(-1, -1);
	m_bRunning = false;
}

/*-----------------------------------------------------------------------------+
|                  Implementation of the CAlgManager class                     |
+-----------------------------------------------------------------------------*/

// constructor:
CAlgManager::CAlgManager():
    m_bBeat(1.0),
	m_bPause(false),
	m_dOrder(doSpecialRandomized),
	m_viArray()
{
	m_alg = NULL;
	g_Log.AddMsg(lmNormal, "%s - initialised", typeid(*this).name());
}

// destructor:
CAlgManager::~CAlgManager() {
	g_Log.AddMsg(lmNormal, "%s clean up", typeid(*this).name());
}

// the Update method ----------------------------------------------------------+
void CAlgManager::Update() {
	if (m_bPause == true) return;
	
	if (m_bBeat.Check(g_Timer.GetTime())) {
		m_alg->Step();
	}
}

// the Render method ----------------------------------------------------------+
void CAlgManager::Render(CAVSystem *avSystem) {
	m_viArray.Render(avSystem);
}

// RunAgain method ------------------------------------------------------------+
void CAlgManager::RunAgain() {
	if (m_alg != NULL) m_alg->Init(&m_viArray);
}

// the GenerateData method ----------------------------------------------------+
void CAlgManager::GenerateData(DataOrder dOrder) {
	g_Log.AddMsg(lmNormal, "%s - Data was generated: type - %s", typeid(*this).name(), GetDataOrderName());
	m_viArray2.Generate(dOrder);
	m_dOrder = dOrder;
	RegenerateData();
}

// the Regenerate method ------------------------------------------------------+
void CAlgManager::RegenerateData() {
	m_viArray.Resize(m_viArray2.GetSize());
	for (int i = 0; i < m_viArray2.GetSize(); ++i)
		m_viArray[i] = m_viArray2[i];
	m_viArray.SetAdditionalMark(-1);
}

// the SetAlgorithm method ----------------------------------------------------+
void CAlgManager::SetAlgorithm(CAlgorithm *cAlg) {
	m_viArray.SetAdditionalMark(-1); 
	m_alg = cAlg; 
	m_alg->Init(&m_viArray);
	g_Log.AddMsg(lmNormal, "%s - %s was assigned the manager", typeid(*this).name(), GetAlgorithmName());
}

// the SetNumOfElements method ------------------------------------------------+
void CAlgManager::SetNumOfElements(int iElems) {
	if(iElems == m_viArray2.GetSize()) return; 
	g_Log.AddMsg(lmNormal, "%s - new number of data element was set to: %d", typeid(*this).name(), iElems);
	if (m_alg != NULL) m_alg->Stop(); 
	m_viArray2.Resize(iElems); 
	GenerateData(m_dOrder);
}

// end of file ----------------------------------------------------------------+