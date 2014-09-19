#include "cbaseinterp.h"
#include "debug.h"
#include <QtConcurrent/QtConcurrent>

CBasedInterpreter::CBasedInterpreter() : QObject(NULL), m_status(_STATUS_NORMAL) {

}

CBasedInterpreter::~CBasedInterpreter() {
	TInterpThreadMap::iterator pFind;

	for (pFind=m_mapThread.begin(); pFind != m_mapThread.end(); pFind++) {
		pFind.value().cancel();
		pFind.value().waitForFinished();
	}

	m_mapThread.clear();
}

int CBasedInterpreter::runThread(CBasedInterpreter *interp, QString szFile) {
	if (NULL == interp || szFile.isEmpty()) {
		return -1;
	}

	interp->m_status = CBasedInterpreter::_STATUS_RUNNING;
	if (0 != interp->run(szFile)) {
		emit interp->sigThreadError(szFile);
		interp->m_status = CBasedInterpreter::_STATUS_STOP;
	}
	else {
		interp->m_status = CBasedInterpreter::_STATUS_STOP;
	}
	return interp->finishRun(szFile);
}

int CBasedInterpreter::run(QString szFile) {
	return 0;
}

int CBasedInterpreter::finishRun(QString szFile) {
	emit sigThreadFinished(szFile);
	// remove the thread from map
	m_mapThread.remove(szFile);
	return 0;
}
int CBasedInterpreter::slotRun(QString szFile) {
	if (szFile.isEmpty()) {
		return -1;
	}

	// is the file already running?
	TInterpThreadMap::iterator pFind=m_mapThread.find(szFile);
	if (m_mapThread.end() != pFind) {
		return -2;
	}
	QFuture<int> thread=QtConcurrent::run(
			CBasedInterpreter::runThread, this, szFile);

	m_mapThread[szFile]=thread;
	return 0;
}
