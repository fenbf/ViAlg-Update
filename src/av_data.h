/*-----------------------------------------------------------------------------+
| File name: av_data.h													       |
| Date:		 14th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorithm Visualization Data                                                  |
| Provides Vi(sual)Data classes that behave like standard Abstract Data Types  |
| but also have methods for Rendering them on the screen.                      |
+-----------------------------------------------------------------------------*/

#ifndef AV_DATA_H
	#define AV_DATA_H

#include <vector>
#include <random>
#include <limits>

class IRenderSystem;

enum class DataOrder { doSorted = 0, doReversed, doRandomized, doSpecialRandomized };
std::string ToString(DataOrder d);

template<typename T>
void GenerateData(std::vector<T>& outVec, DataOrder dOrder) {
	const T tAsp = (T)(1.0 / static_cast<double>(outVec.size()));

	static std::random_device dev;
	static std::mt19937 rng(dev());

	switch (dOrder) {
		case DataOrder::doSorted: {
			for (size_t i = 0; i < outVec.size(); ++i)
				outVec[i] = (T)(tAsp * (double)(i + 1));
			break;
		}
		case DataOrder::doReversed: {
			for (size_t i = 0; i < outVec.size(); ++i)
				outVec[i] = (T)(1.0 - tAsp * (double)(i));
			break;
		}
		case DataOrder::doRandomized: {
			std::uniform_real_distribution<> dist;
			for (size_t i = 0; i < outVec.size(); ++i)
				outVec[i] = (T)(dist(rng));
			break;
		}
		case DataOrder::doSpecialRandomized: {
			for (size_t i = 0; i < outVec.size(); ++i)
				outVec[i] = (T)(tAsp * (double)(i + 1));
			std::shuffle(outVec.begin(), outVec.end(), rng);
			break;
		}
	}
}

// it beheaves like std::vector but it can also track extra states like "last post",
// current active "window", etc... handy for drawing
template <class T>
class CViArray {
	static constexpr size_t INVALID_ID{ std::numeric_limits<size_t>::max() };
public:
	CViArray(size_t iSize);
	CViArray() { }

	void Resize(size_t iSize) { m_vArray.resize(iSize); m_iLast = INVALID_ID; m_iLast2 = INVALID_ID; }
	void SetSection(size_t iLeft, size_t iRight) { m_iL = iLeft; m_iR = iRight; }
	void SetAdditionalMark(size_t iId) { m_iLast2 = iId; }
	size_t GetSize() const { return m_vArray.size(); }

	size_t GetLastPos() const { return m_iLast; }
	size_t GetLastPosAdditional() const { return m_iLast2; }
	size_t GetRangeLeft() const { return m_iL; }
	size_t GetRangeRight() const { return m_iR; }

	const T& operator [] (size_t iId) const;
	T& operator [] (size_t iId);

private:
	std::vector<T> m_vArray;
	size_t m_iLast{ INVALID_ID };			// last accessed element
	size_t m_iLast2{ INVALID_ID };			// additional accesed element
	size_t m_iL, m_iR{ INVALID_ID };         // highlighted section - left and right
};

// decouples the storage from the rendering
class DataRenderer {
public:
	void Reset();
	void Render(const CViArray<float>& numbers,  IRenderSystem* avSystem);

private:
	std::vector<float> m_vCurrPos; // values displayed, might be interpolated - creates nice animation

	static constexpr float s_AnimBlendFactor = 0.1f;
};

/*-----------------------------------------------------------------------------+
|                    Implementation of the CViArray class                      |
+-----------------------------------------------------------------------------*/

// constructor:
template <class T>
CViArray<T>::CViArray(size_t iSize) {
	m_vArray.resize(iSize);
	m_iLast = -1;
	m_iLast2 = -1;
	m_iR = -1;
	m_iL = -1;
}

// the [] operator ------------------------------------------------------------+
template <class T>
const T& CViArray<T>::operator [] (size_t iId) const {
	if (iId >= m_vArray.size()) return m_vArray[m_vArray.size()-1];
	//m_iLast = iId; // cannot change it, unless it's mutable... but not needed...
	return m_vArray[iId];
}

// the [] operator ------------------------------------------------------------+
template <class T>
T& CViArray<T>::operator [] (size_t iId) {
	if (iId >= m_vArray.size()) return m_vArray[m_vArray.size()-1];
	m_iLast = iId;
	return m_vArray[iId];
}

#endif // AV_DATA_H

// end of file ----------------------------------------------------------------+