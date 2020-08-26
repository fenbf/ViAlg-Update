/*-----------------------------------------------------------------------------+
| File name: calgorithms.cpp   					   						       |
| Date:		 15 March 2006													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorithm classes thah will be used in Algorithm Visualization system...     |
| Implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "calgorithms.h"
#include <random>
#include <algorithm>
#include <numeric>
#include "resource.h"

/*-----------------------------------------------------------------------------+
|               Implementation of the AlgorithmFactory class               |
+-----------------------------------------------------------------------------*/

std::unique_ptr<IAlgorithm> AlgorithmFactory::Create(WORD algID)
{
	switch (algID)
	{
	case ID_METHOD_BUBBLESORT: return std::make_unique<CBubbleSortAlgorithm>();
	case ID_METHOD_SHAKERSORT: return std::make_unique<CShakerSortAlgorithm>();
	case ID_METHOD_SELECTIONSORT: return std::make_unique<CSelectionSortAlgorithm>();
	case ID_METHOD_INSERTIONSORT: return std::make_unique<CInsertionSortAlgorithm>();
	case ID_METHOD_SHELLSORT: return std::make_unique<CShellSortAlgorithm>();
	case ID_METHOD_QUICKSORT: return std::make_unique<CQuickSortAlgorithm>();
	case ID_METHOD_SHUFFLE: return std::make_unique<CShuffleElementsAlgorithm>();
	};

	throw std::exception("unrecognized algorithm class ID!");
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CBubbleSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CBubbleSortAlgorithm::Init(CViData *viData) {
	m_viArray = dynamic_cast < CViArray<float> *>(viData);
	
	m_i = 0;
	m_j = m_viArray->GetSize()-1;
	m_viArray->SetSection(m_i, m_j);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CBubbleSortAlgorithm::Step() {
	if (!m_isDone) return;

	// normal code of the bubble sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         for (j = m_viArray.GetSize()-1; j > i; --j)
	//             CAlgorithm::CompExch(m_viArray[j-1], m_viArray[j]);
	//     }

	m_stats.NextIteration();

	if (m_j > m_i) {
		m_stats.CompExch((*m_viArray)[m_j-1],(*m_viArray)[m_j]);
		--m_j;
	}
	else {
		++m_i;

		if (m_i >= m_viArray->GetSize()-1) m_isDone = false;

		m_j = m_viArray->GetSize()-1;

		m_viArray->SetSection(m_i, m_j);
	}
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CShakerSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CShakerSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 0;
	m_j = m_viArray->GetSize()-1;    // backward
	m_j2 = 0;                       // forward
	m_viArray->SetSection(m_i, m_j);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CShakerSortAlgorithm::Step() {
	if (!m_isDone) return;

	// normal code of the bubble sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         for (j = m_viArray.GetSize()-1, j2 = i; j > i; --j, --j2) {
	//             CAlgorithm::CompExch(m_viArray[j-1], m_viArray[j]);
	//             CAlgorithm::CompExch2(m_viArray[j], m_viArray[j+1]);
	//         }
	//     }

	m_stats.NextIteration();

	if (m_j > m_i) {
		m_stats.CompExch((*m_viArray)[m_j-1], (*m_viArray)[m_j]);
		m_stats.CompExch((*m_viArray)[m_j2], (*m_viArray)[m_j2 + 1]);
		m_viArray->SetAdditionalMark(m_j-1);
		--m_j;
		++m_j2;
		return;
	}

	++m_i;

	m_viArray->SetAdditionalMark(-1);
	if (m_i >= m_viArray->GetSize()-1) m_isDone = false;

	m_j = m_viArray->GetSize()-1-m_i;
	m_j2 = m_i;

	m_viArray->SetSection(m_i, m_j);
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CSelectionSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CSelectionSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 0;
	m_j = 1;
	m_iMin = m_i;
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CSelectionSortAlgorithm::Step() {
	if (!m_isDone) return;

	// normal code of the selection sort algorith is:
	//     for (i = 0; i < m_viArray.GetSize()-1; ++i) {
	//         m_iMin = i;
	//         for (j = i+1; j < m_viArray.GetSize(); ++j)
	//             if (m_viArray[j] < m_viArray[m_iMin]) m_iMin = j;
	//         Exchange(m_viArray[i], m_viArray[m_iMin]);
	//     }

	m_stats.NextIteration();

	if (m_j < m_viArray->GetSize()) {
		if (m_stats.IsSmaller((*m_viArray)[m_j], (*m_viArray)[m_iMin])) 
			m_iMin = m_j;
		
		++m_j;
		return;
	}
	
	m_stats.Exchange((*m_viArray)[m_i], (*m_viArray)[m_iMin]);
	++m_i;
	m_j = m_i+1;
	m_iMin = m_i;
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);

	if (m_i >= m_viArray->GetSize()-1) 
		m_isDone = false;
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CInsertionSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CInsertionSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	m_i = 1;
	m_j = 1;
	m_fValue = (*m_viArray)[1];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CInsertionSortAlgorithm::Step() {
	if (!m_isDone) return;

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

	m_stats.NextIteration();

	if (m_j > 0 && m_stats.IsBigger((*m_viArray)[m_j-1], m_fValue)) {
		(*m_viArray)[m_j] = (*m_viArray)[m_j-1];
		m_stats.NextChange();
		
		--m_j;
		return;
	}
	
	(*m_viArray)[m_j] = m_fValue;
	m_stats.NextChange();
	++m_i;

	if (m_i >= m_viArray->GetSize()) { 
		m_isDone = false; 
		return; 
	}

	m_j = m_i;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CShellSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CShellSortAlgorithm::Init(CViData *vData) {
	m_viArray = dynamic_cast < CViArray<float> *>(vData);
	
	for (m_h = 1; m_h < m_viArray->GetSize()/9; m_h = 3*m_h+1);
	m_i = m_h;
	m_j = m_h;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CShellSortAlgorithm::Step() {
	if (!m_isDone) return;

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
	m_stats.NextIteration();

	if (m_j >= m_h && m_stats.IsBigger((*m_viArray)[m_j-m_h], m_fValue)) {
		(*m_viArray)[m_j] = (*m_viArray)[m_j-m_h];
		m_stats.NextChange();
		
		m_j-=m_h;
		return;
	}
	
	(*m_viArray)[m_j] = m_fValue;
	m_stats.NextChange();
	++m_i;

	if (m_i >= m_viArray->GetSize()) {
		m_h /= 3;
		if (m_h < 1) { 
			m_isDone = false; 
			return; 
		}
		m_i = m_h;
	}

	m_j = m_i;
	m_fValue = (*m_viArray)[m_i];
	m_viArray->SetSection(m_i, m_viArray->GetSize()-1);

	m_name = "Shell Sort " + std::to_string(m_h) + "-sorting";
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CQuickSortAlgorithm class               |
+-----------------------------------------------------------------------------*/

// from https://www.geeksforgeeks.org/iterative-quick-sort/
///* This function is same in both iterative and recursive*/
//int partition(int arr[], int l, int h)
//{
//	int x = arr[h];
//	int i = (l - 1);
//
//	for (int j = l; j <= h - 1; j++) {
//		if (arr[j] <= x) {
//			i++;
//			swap(&arr[i], &arr[j]);
//		}
//	}
//	swap(&arr[i + 1], &arr[h]);
//	return (i + 1);
//}
//
///* A[] --> Array to be sorted,
//l --> Starting index,
//h --> Ending index */
//void quickSortIterative(int arr[], int l, int h)
//{
//	// Create an auxiliary stack 
//	int stack[h - l + 1];
//
//	// initialize top of stack 
//	int top = -1;
//
//	// push initial values of l and h to stack 
//	stack[++top] = l;
//	stack[++top] = h;
//
//	// Keep popping from stack while is not empty 
//	while (top >= 0) {
//		// Pop h and l 
//		h = stack[top--];
//		l = stack[top--];
//
//		// Set pivot element at its correct position 
//		// in sorted array 
//		int p = partition(arr, l, h);
//
//		// If there are elements on left side of pivot, 
//		// then push left side to stack 
//		if (p - 1 > l) {
//			stack[++top] = l;
//			stack[++top] = p - 1;
//		}
//
//		// If there are elements on right side of pivot, 
//		// then push right side to stack 
//		if (p + 1 < h) {
//			stack[++top] = p + 1;
//			stack[++top] = h;
//		}
//	}
//}

// the Init method ------------------------------------------------------------+
void CQuickSortAlgorithm::Init(CViData* vData) {
	m_viArray = dynamic_cast <CViArray<float>*>(vData);

	m_l = 0;
	m_h = m_viArray->GetSize() - 1;

	m_valPartition = (*m_viArray)[m_h];
	m_iter = 0;
	m_indexPartition = -1;

	m_viArray->SetSection(0, m_h);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CQuickSortAlgorithm::Step() {
	if (!m_isDone) return;

	m_stats.NextIteration();

	if (m_iter < m_h)
	{
		if (m_stats.IsBigger(m_valPartition, (*m_viArray)[m_iter]))
		{
			m_indexPartition++;
			m_stats.Exchange((*m_viArray)[m_indexPartition], (*m_viArray)[m_iter]);
		}
		m_iter++;
		return;
	}
	
	// (m_iterPartition == m_h) // finish partition...
	m_indexPartition++;
	m_stats.Exchange((*m_viArray)[m_indexPartition], (*m_viArray)[m_h]);

	if (m_indexPartition - 1 > m_l)
	{
		m_stack.push(m_l);
		m_stack.push(m_indexPartition - 1);
	}

	if (m_indexPartition + 1 < m_h)
	{
		m_stack.push(m_indexPartition + 1);
		m_stack.push(m_h);
	}

	if (m_stack.empty())
		m_isDone = false;
	else
	{
		m_h = m_stack.top();
		m_stack.pop();
		m_l = m_stack.top();
		m_stack.pop();

		m_iter = m_l;
		m_indexPartition = m_iter - 1;
		m_valPartition = (*m_viArray)[m_h];
		m_viArray->SetSection(m_l, m_h);
	}
}

/*-----------------------------------------------------------------------------+
|               Implementation of the CShuffleElementsAlgorithm class               |
+-----------------------------------------------------------------------------*/

// the Init method ------------------------------------------------------------+
void CShuffleElementsAlgorithm::Init(CViData* vData) {
	m_viArray = dynamic_cast <CViArray<float>*>(vData);

	m_i = 0;
	auto rng = std::default_random_engine{};
	m_randomOrder.resize(m_viArray->GetSize());
	std::iota(std::begin(m_randomOrder), std::end(m_randomOrder), 0);
	std::shuffle(std::begin(m_randomOrder), std::end(m_randomOrder), rng);

	m_viArray->SetSection(0, m_viArray->GetSize() - 1);
	m_stats.ZeroStats();
	m_isDone = true;
}

// the Step method ------------------------------------------------------------+
void CShuffleElementsAlgorithm::Step() {
	if (!m_isDone) return;

	m_stats.NextIteration();

	if (m_i < m_randomOrder.size())
	{
		// swap:
		auto a = m_randomOrder[m_i];
		m_stats.Exchange((*m_viArray)[m_i], (*m_viArray)[a]);
		++m_i;
	}
	else
		m_isDone = false;
}

/*-----------------------------------------------------------------------------+
|                  Implementation of the CAlgManager class                     |
+-----------------------------------------------------------------------------*/

// constructor:
CAlgManager::CAlgManager(const CLog& logger):
    m_bBeat(1.0),
	m_bPause(false),
	m_dOrder(DataOrder::doSpecialRandomized),
	m_viArray(),
	m_logger(logger)
{
	m_logger.AddMsg(LogMode::Info, "%s - initialised", typeid(*this).name());
}

// destructor: #simplification: do we really need to log this here?
CAlgManager::~CAlgManager() {
	m_logger.AddMsg(LogMode::Info, "%s clean up", typeid(*this).name());
}

// the Update method ----------------------------------------------------------+
void CAlgManager::Update() {
	if (m_bPause == true) return;
	
	if (m_pCurrentAlg && m_bBeat.Check(g_Timer.GetTime())) {
		m_pCurrentAlg->Step();
	}
}

// the Render method ----------------------------------------------------------+
void CAlgManager::Render(CAVSystem *avSystem) {
	m_viArray.Render(avSystem);
}

// RunAgain method ------------------------------------------------------------+
void CAlgManager::RunAgain() {
	if (m_pCurrentAlg) 
		m_pCurrentAlg->Init(&m_viArray);
}

// the GenerateData method ----------------------------------------------------+
void CAlgManager::GenerateData(DataOrder dOrder) {
	m_logger.AddMsg(LogMode::Info, "%s - Data was generated: type - %s", typeid(*this).name(), GetDataOrderName().c_str());
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
void CAlgManager::SetAlgorithm(WORD algID) {
	m_viArray.SetAdditionalMark(-1); 
	m_pCurrentAlg = AlgorithmFactory::Create(algID);
	m_pCurrentAlg->Init(&m_viArray);
	m_logger.AddMsg(LogMode::Info, "%s - %s was assigned the manager", typeid(*this).name(), GetAlgorithmName());
}

// the SetNumOfElements method ------------------------------------------------+
void CAlgManager::SetNumOfElements(int iElems) {
	if(iElems == m_viArray2.GetSize()) return; 
	m_logger.AddMsg(LogMode::Info, "%s - new number of data element was set to: %d", typeid(*this).name(), iElems);
	
	m_viArray2.Resize(iElems); 
	GenerateData(m_dOrder);
}

// end of file ----------------------------------------------------------------+


