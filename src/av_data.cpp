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

std::string ToString(DataOrder d)
{
	static const std::string names[] { "Sorted", "Reversed", "Randomized", "Special Randomized" };
	return names[static_cast<int>(d)];
}

void DataRenderer::Reset()
{
	m_vCurrPos.clear();
}

void DataRenderer::Render(const CViArray<float>& numbers, CAVSystem* avSystem)
{
	// reload data?
	if (numbers.GetSize() != m_vCurrPos.size())
	{
		m_vCurrPos.resize(numbers.GetSize());
		for (size_t i = 0; i < m_vCurrPos.size(); ++i)
			m_vCurrPos[i] = numbers[i];
	}

	ColorType ct;
	avSystem->BeginDrawing(1.0, (int)numbers.GetSize());
	for (int i = 0; i < (int)numbers.GetSize(); ++i) {
		ct = ctNormal;

		m_vCurrPos[i] += (numbers[i] - m_vCurrPos[i]) * s_AnimBlendFactor;

		if (i >= numbers.GetRangeLeft() && i <= numbers.GetRangeRight()) ct = ctHighlighted;
		if (i == numbers.GetLastPos() || i == numbers.GetLastPosAdditional()) ct = ctMarked;
		avSystem->DrawDiagramBlock((double)m_vCurrPos[i], ct);
	}
	avSystem->EndDrawing();
}
