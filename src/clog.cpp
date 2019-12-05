/*-----------------------------------------------------------------------------+
| File name: clog.hpp							   						       |
| Date:		 5 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Log system that is represented by CLog class.   							   |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

/*
system logów bêdzie bardzo prosty, oparty w³aœciwie na jednej czy kilku funkcjach
inicjujemy obiekt podaj¹c nazwê pliku do wpisów, a póxniej wywo³ujemy metodê Write
lub coœ w tym stylu aby dokonaæ wpisu.
Bardzo ciekawa metoda polega na zapisie w HTMLu dziêki czemu mo¿na stosowaæ ró¿ne
kolory, podbieram ten pomys³ z kodu do wczeœniej wspomnianej ksi¹¿ki.

format pliku HTML
Log Started
Date and Time: ....

 - wiadomoœæ 1	 (w danym kolorze)
 - wiadomoœæ 2
 - ...
*/

#include "clog.h"

/*-----------------------------------------------------------------------------+
|                      Implementation of the CLog class                        |
+-----------------------------------------------------------------------------*/

// constructor:
CLog::CLog():
	m_bEnabled(false)
{
	m_szFileName[0] = '\0';
}

// constructor 2:
CLog::CLog(const char *szFileName)
{
	Init(szFileName);
}

// destructor:
CLog::~CLog()
{

}

/*-----------------------------------------------------------------------------+
|                                    Init                                      |
+------------------------------------------------------------------------------+
| Description:																   |
|    Initializes the Log system, opens the specific file and writes basic info |
|    to it such as date, and time...			                               |
| Arguments:																   |
|  - const char *szFileName - file name                    					   |
| Return value:																   |
|    true if everything goes ok, unless false								   |
+-----------------------------------------------------------------------------*/
bool CLog::Init(const char *szFileName) {
	strcpy_s(m_szFileName, szFileName);

	// open clean file and write some basic info:
	FILE *f;
	fopen_s(&f, m_szFileName, "w+");

	if (f == NULL)
		return false;

	m_bEnabled = true;

	// date and time:
	char timestring[280];
	__time64_t ltime;

    _time64( &ltime );
	_ctime64_s(timestring, &ltime);
    //sprintf_s(timestring, "%s",  );

    // module name:
	char szModuleName[128];
	GetModuleFileName(NULL, szModuleName, 128);
	fprintf(f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
			   "<HTML>\n"
			   "<HEAD>\n"
			   "<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-2\">"
			   "<TITLE>Log File</TITLE>\n"
			   "</HEAD>"
			   "<BODY bgcolor=\"FFFFFF\">\n");

	fprintf(f, "<font style=\"font-size: medium; font-family: Courier; color: 4444ee\"> Log for the \"%s\" started!</font><br>\n", szModuleName);
	fprintf(f, "<font style=\"font-size: medium; font-family: Courier; color: 4444ee\"> Date and time: %s </font><br>\n", timestring);
	fprintf(f, "</BODY>\n"
		       "</HTML>\n");

	fclose(f);
	return true;
}

/*-----------------------------------------------------------------------------+
|                                   AddMsg                                     |
+------------------------------------------------------------------------------+
| Description:																   |
|    Writes MSG into the log file.											   |
|    			                              							       |
| Arguments:																   |
|  - LOG_MODE lmMode - type of the message (Error, Normal, or Success)         |
|  - char *szMsg - message string with argumets ( ... )                        |
| Return value:																   |
|    none								                                       |
+-----------------------------------------------------------------------------*/
void CLog::AddMsg(LOG_MODE lmMode, const char *szMsg, ...) {
	if (m_bEnabled == false)
		return;
	
	FILE *f;
	fopen_s(&f, m_szFileName, "r+");
	if (f == NULL)
		return;

	// parse argument to the string:
	char buf[512];

	va_list arglist;

	if (!szMsg)
   		return;

	va_start(arglist, szMsg);
	vsprintf_s(buf, szMsg, arglist);
	va_end(arglist);

	// overwrite last 16 characters - that is "</BODY>\n"..."
	fseek(f, -18, SEEK_END);
	fprintf(f, "<font style=\"font-size: small; font-family: Courier; color: %s\"> %s  </font><br> \n", GetColorName(lmMode), buf);
	fprintf(f, "</BODY>\n"
		       "</HTML>\n");


	fclose(f);
}

/*-----------------------------------------------------------------------------+
|                                GetColorName                                  |
+------------------------------------------------------------------------------+
| Description:																   |
|    Decodes LOG_MODE into the color used in HTML file          			   |
|    			                              							       |
| Arguments:																   |
|  - LOG_MODE lmMode - type of the message (Error, Normal, or Success)         |
| Return value:																   |
|    array of char that represent color fe. "ffeeff"						   |
+-----------------------------------------------------------------------------*/
const char *CLog::GetColorName(LOG_MODE lmMode) {
	return LogColors[(int)lmMode];
}

// end of file ----------------------------------------------------------------+

