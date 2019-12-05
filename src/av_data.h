/*-----------------------------------------------------------------------------+
| File name: av_data.h													       |
| Date:		 14th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorith Visualization Data                                                  |
| Privides Vi(sual)Data classes that behave like standard Abstract Data Types  |
| but also have methods for Rendering them on the screen.                      |
+-----------------------------------------------------------------------------*/

#ifndef AV_DATA_H
	#define AV_DATA_H

#include "av_system.h"
#include <vector>

enum DataOrder { doSorted = 0, doReversed, doRandomized, doSpecialRandomized };
const char strDataOrderNames[4][24] = { "Sorted", "Reversed", "Randomized", "Special Randomized" };

// the CViData base abstract class --------------------------------------------+
class CViData {
public:
	CViData() { }
	virtual ~CViData() { }

	virtual void Render(CAVSystem *avSystem) = 0;
};

// the CViArray class ---------------------------------------------------------+
// it beheaves like the std::vector class but it has ability to be drawn by
// CAVSystem class
template <class T>
class CViArray : public CViData {
public:
	CViArray(int iSize);
	CViArray(): m_iLast(-1), m_iLast2(-1), m_iL(-1), m_iR(-1) { }
	~CViArray();

	void Render(CAVSystem *avSystem);

	void Generate(DataOrder dOrder);
	void Resize(int iSize) { m_vArray.resize(iSize); m_iLast = -1; m_iLast2 = -1; m_vCurrPos = m_vArray; }
	void SetSection(int iLeft, int iRight) { m_iL = iLeft; m_iR = iRight; }
	void SetAdditionalMark(int iId) { m_iLast2 = iId; }
	int GetSize() { return (int)m_vArray.size(); }

	const T& operator [] (int iId) const;
	T& operator [] (int iId);
	void Func(T obj);
private:
	std::vector<T> m_vArray;
	std::vector<T> m_vCurrPos;
	int m_iLast;			// last accessed element
	int m_iLast2;			// additional accesed element
	int m_iL, m_iR;         // highlighted section - left and right
};

/*-----------------------------------------------------------------------------+
|                    Implementation of the CViArray class                      |
+-----------------------------------------------------------------------------*/

// constructor:
template <class T>
CViArray<T>::CViArray(int iSize) {
	m_vArray.resize(iSize);
	m_vCurrPos = m_vArray;
	m_iLast = -1;
	m_iLast2 = -1;
	m_iR = -1;
	m_iL = -1;
}

// destructor:
template <class T>
CViArray<T>::~CViArray() {

}

// Render method --------------------------------------------------------------+
template <class T>
void CViArray<T>::Render(CAVSystem *avSystem) {
	ColorType ct;
	avSystem->BeginDrawing(1.0, (int)m_vArray.size());
	for (int i = 0; i < (int)m_vArray.size(); ++i) {
        ct = ctNormal;

		m_vCurrPos[i] += (m_vArray[i] - m_vCurrPos[i]) * 0.1f;

		if (i >= m_iL && i <= m_iR) ct = ctHighlighted;
		if (i == m_iLast || i == m_iLast2) ct = ctMarked;
		avSystem->DrawDiagramBlock((double)m_vCurrPos[i], ct);
	}
	avSystem->EndDrawing();
}

// Ganerate method ------------------------------------------------------------+
template <class T>
void CViArray<T>::Generate(DataOrder dOrder) {
	T tAsp = (T)(1.0/(double)m_vArray.size());
	switch ( dOrder ) {
		case doSorted: {
			for (size_t i = 0; i < m_vArray.size(); ++i)
				m_vArray[i] = (T)(tAsp*(double)(i+1));
			break;
			}
	   case doReversed: {
			for (size_t i = 0; i < m_vArray.size(); ++i)
				m_vArray[i] = (T)(1.0 - tAsp*(double)(i));
			break;
			}
	   case doRandomized: {
			for (size_t i = 0; i < m_vArray.size(); ++i)
				m_vArray[i] = (T)(((double)rand()/RAND_MAX)*(double)i*tAsp);
			break;
			}
	   case doSpecialRandomized: {
			for (size_t i = 0; i < m_vArray.size(); ++i)
				m_vArray[i] = (T)(tAsp*(double)(i+1));
			for (size_t i = 0; i < 10*m_vArray.size(); ++i) {
				int a = rand()%(int)m_vArray.size();
				int b = rand()%(int)m_vArray.size();
				Swap(m_vArray[a], m_vArray[b]);
			}
			break;
		}
	}
	m_iLast2 = -1;
	m_vCurrPos = m_vArray;
}

// the [] operator ------------------------------------------------------------+
template <class T>
const T& CViArray<T>::operator [] (int iId) const {
	if (iId >= m_vArray.size()) return m_vArray[m_vArray.size()-1];
	m_iLast = iId;
	return m_vArray[iId];
}

// the [] operator ------------------------------------------------------------+
template <class T>
T& CViArray<T>::operator [] (int iId) {
	if (iId >= (int)m_vArray.size()) return m_vArray[m_vArray.size()-1];
	m_iLast = iId;
	return m_vArray[iId];
}

#endif // AV_DATA_H

// end of file ----------------------------------------------------------------+