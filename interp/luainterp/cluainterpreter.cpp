#include "cluainterpreter.h"

#include "debug.h"
#include <iostream>
#include <sstream>

#ifdef Q_OS_WIN
#include <windows.h>

static std::streambuf *g_oriStdout=NULL;
static std::streambuf *g_oriStderr=NULL;
static std::stringstream g_coutBuf;
static std::stringstream g_cerrBuf;


extern "C" BOOL WINAPI DllMain(
  HANDLE hinstDLL,
  DWORD fdwReason,
  LPVOID lpvReserved
)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			_DMSG("dll attached");

			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			_DMSG("dll deattached");

			break;
	}
	return true;
}

#endif


CLuaInterpreter::CLuaInterpreter() : CBasedInterpreter(), mLua()
{
}

CLuaInterpreter::~CLuaInterpreter() {
	mLua.close();
}

int CLuaInterpreter::run(QString szFile) {
	if (szFile.isEmpty()) {
		return -3;
	}

	mLua.runFile(szFile);

	return 0;
}

QString CLuaInterpreter::getStdout() {

	return "";
}
