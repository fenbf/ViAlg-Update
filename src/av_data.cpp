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
