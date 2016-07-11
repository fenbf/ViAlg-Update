/*-----------------------------------------------------------------------------+
| File name: av_data.cpp												       |
| Date:		 14th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorith Visualization Data                                                  |
| Privides Vi(sual)Data classes that behave like standard Abstract Data Types  |
| but also have methods for Rendering them on the screen.                      |
| Implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "av_data.h"

//using namespace std;

/*-----------------------------------------------------------------------------+
|                    Implementation of the CViArray class                      |
+-----------------------------------------------------------------------------*/

// constructor:
template <class T>
CViArray<T>::CViArray(int iSize) {
	m_vArray.resize(iSize);
	m_iLast = 0;
}

// destructor:
template <class T>
CViArray<T>::~CViArray() {

}

// Render method --------------------------------------------------------------+
template <class T>
void CViArray<T>::Render(const CAVSystem *avSystem) const {
	avSystem->DrawDiagram(m_vArray.begin(), m_vArray.end());
}

// the [] operator ------------------------------------------------------------+
template <class T>
const T& CViArray<T>::operator [] (int iId) const {
	m_iLast = iId;
	return m_vArray[iId];
}

// the [] operator ------------------------------------------------------------+
template <class T>
T& CViArray<T>::operator [] (int iId) {
	m_iLast = iId;
	return m_vArray[iId];
}

// end of file ----------------------------------------------------------------+