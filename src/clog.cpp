/*-----------------------------------------------------------------------------+
| File name: clog.hpp							   						       |
| Date:		 5 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Log system that is represented by CLog class.   							   |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

#include "clog.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

/*-----------------------------------------------------------------------------+
|                      Implementation of the CLog class                        |
+-----------------------------------------------------------------------------*/


// constructor 2:
CLog::CLog(std::string filename) noexcept
{
	Init(filename);
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
bool CLog::Init(std::string filename) noexcept {
	m_filename = filename;

	// open clean file and write some basic info:
	FILE *f;
	fopen_s(&f, m_filename.c_str(), "w+");

	if (f == nullptr)
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
			   "<style> p { font-family: Courier; margin:0;} .info { color: black; } .err {color: #aa6666;} .success {color:#66aa66;}</style>\n"
			   "</HEAD>"
			   "<BODY>\n");

	fprintf(f, "<p class=\"info\"> Log for the \"%s\" started!</p>\n", szModuleName);
	fprintf(f, "<p class=\"info\"> Date and time: %s </p>\n", timestring);
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
void CLog::AddMsg(LogMode lmMode, const char *szMsg, ...) const noexcept {
	if (m_bEnabled == false)
		return;
	
	FILE *f;
	fopen_s(&f, m_filename.c_str(), "r+");
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
	fprintf(f, "<p class=\"%s\"> %s  </p>\n", GetLogClassName(lmMode), buf);
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
const char *CLog::GetLogClassName(LogMode lmMode) const noexcept {
	static constexpr std::string_view LogClasses[] = { "info", 
												 		"err",  
														"success" };

	return LogClasses[(int)lmMode].data();
}

// end of file ----------------------------------------------------------------+

