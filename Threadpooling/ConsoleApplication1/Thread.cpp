#include "stdafx.h"
#include "Thread.h"
#include <fstream>

/****************************************************************************
CThread Constructer
****************************************************************************/
CThread::CThread(void)
{
	// Initialize members
	m_ThreadID = 0;
	m_hThread = NULL;
	m_bIsFree = TRUE;
	m_strMessage = "";

	m_hWorkEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hWorkEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CThread::~CThread(void)
{

}

/****************************************************************************
CThread ThreadProc
Description : Thread Procedure.
****************************************************************************/
DWORD WINAPI CThread::ThreadProc(LPVOID Param)
{
	// Create object of ThreadPoolMgr
	CThread* ptrThread = (CThread*)Param;

	BOOL bShutDown = FALSE;

	while (!bShutDown)
	{
		DWORD dwWaitResult = WaitForMultipleObjects(2, ptrThread->m_hWorkEvent, FALSE, INFINITE);
		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			// Work event signaled Call the Run function    
			ptrThread->Run();
			break;

		case WAIT_OBJECT_0 + 1:
			bShutDown = TRUE;
			break;
		}
	}

	return 0;
}
void CThread::Run(void)
{
	ofstream myfile;
	myfile.open(m_strMessage.c_str());

	myfile << "Thread " << GetCurrentThreadId() << " is Running.\n";
	myfile << "Thread Message: " << m_strMessage.c_str() << "\n";
	cout << "Thread Message: " << m_strMessage.c_str() << "\n";
	myfile.close();
	/*cout << "Thread " << GetCurrentThreadId() <<" is Running.\n";
	cout << "Thread Message: "<< m_strMessage.c_str() << "\n";*/

	Sleep(3000);

	// work is done. Now 
	// 1. m_bIsFree flag to true
	// 2. make work event to nonsignaled and 
	// 3. set message string to empty
	m_bIsFree = TRUE;
	m_strMessage.clear();
	ResetEvent(m_hWorkEvent[0]);
}

/****************************************************************************
CThread IsFree
Description : returns state of thread.
****************************************************************************/
BOOL CThread::IsFree()
{
	return m_bIsFree;
}

void CThread::CreateWorkerThread()
{
	m_hThread = CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &m_ThreadID);

	if (m_hThread == NULL)
		cout << "Thread could not be created: " << GetLastError() << "\n";
	else
		cout << "Successfully created thread: " << m_ThreadID << "\n";
}


HANDLE CThread::GetThreadHandle()
{
	return m_hThread;
}

DWORD CThread::GetThreadID()
{
	return m_ThreadID;
}

void CThread::SetMessage(std::string strMessage)
{
	m_strMessage = strMessage;
}

void CThread::SignalWorkEvent()
{
	SetEvent(m_hWorkEvent[0]);
}

void CThread::SignalShutDownEvent()
{
	SetEvent(m_hWorkEvent[1]);
}

void CThread::SetThreadBusy()
{
	m_bIsFree = FALSE;
}

void CThread::ReleaseHandles()
{
	// Close all handles 
	CloseHandle(m_hThread);
	CloseHandle(m_hWorkEvent[0]);
	CloseHandle(m_hWorkEvent[1]);
}