#include "ThreadPoolMgr.h"

/****************************************************************************
CThreadPoolMgr Initialize 
Description : Creates threads. Thread limit is 5
****************************************************************************/
void CThreadPoolMgr::Initialize(int nThread)
{
    m_nThreadCount = nThread;

    int nCounter = 0;
    int nThreadCount = m_nThreadCount - 1;
    
    while( nCounter <= nThreadCount )
    {
        // Create objects in heap
        m_ptrCThread[nCounter] = new CThread();

        m_ptrCThread[nCounter]->CreateWorkerThread();
        m_hThreadPool[nCounter] = m_ptrCThread[nCounter]->GetThreadHandle();
        // Increment the counter
        nCounter++;
    }    
}



/****************************************************************************
CThreadPoolMgr ShutDown 
Description : Mark shutdown siganl and wait for each thread to end 
****************************************************************************/
void CThreadPoolMgr::ShutDown()
{
    int Count = 0;

    while(Count <= (m_nThreadCount - 1))
    {
        m_ptrCThread[Count]->SignalShutDownEvent();
        
        Count++;
    }

    // Check if all threads ended successfully
    DWORD dwWaitResult = WaitForMultipleObjects( GetThreadCount(), m_hThreadPool, TRUE, INFINITE);
    
    switch(dwWaitResult)
    {
    case WAIT_OBJECT_0:
        {
            cout << "All threads are ended.\n";
            // Close all handles 
            Count = 0;
            while( Count <= (m_nThreadCount - 1))
            {
                m_ptrCThread[Count]->ReleaseHandles();
                delete m_ptrCThread[Count];
                Count++;
            }
            
            break;
        }

    default:
        cout << "Wait Error: " << GetLastError() << "\n";
    }
    
}

/****************************************************************************
CThreadPoolMgr GetFreeTherad 
Description : Return no. of free thread. 
****************************************************************************/
int CThreadPoolMgr::GetFreeTherad()
{
    // Search which thread is free
    int count = 0;
    bool bIsAllBusy = true;
    while(count <= (m_nThreadCount - 1))
    {
        if(m_ptrCThread[count]->IsFree() == TRUE)
        {
            return count;
        }
        else
        {
            cout << "Thread " << count << ": " << m_ptrCThread[count]->GetThreadID() << " is busy!!!\n";
        }
        
        count++;
    }
    if( bIsAllBusy )
    {
        cout << "All thread are busy. Wait for thread to be free!!!\n" ;
        return -1;
    }
}

/****************************************************************************
CThreadPoolMgr AssignTask 
Description : Assign task to free thread. 
              1. Seacrch which is free thread
              2. Assign task to free thread
****************************************************************************/
void CThreadPoolMgr::AssignTask( string strTask )
{
    int Count = GetFreeTherad();
    if( Count != -1)
    {
        m_ptrCThread[Count]->SetThreadBusy();
        
        // Set information to thread member so that thread can use it
        m_ptrCThread[Count]->SetMessage(strTask);
                
        // Signal Work event
        m_ptrCThread[Count]->SignalWorkEvent();
    }
}

int CThreadPoolMgr::GetThreadCount()
{
    return m_nThreadCount;
}


5.  Main program

 #include "ThreadPoolMgr.h"


int _tmain(int argc, _TCHAR* argv[])
{
        
    CThreadPoolMgr MGR;

    int Count = 0;
    
    cout << "************************************************************\n\n\nWelcome to ThreadPool!!!\n\nEnter number of thread that u want to start(U can only create 5 threads):)\n";
    cin >> Count;
    

    cout << "Initializing " << Count <<"threads...\n";
    MGR.Initialize(Count);

    MGR.AssignTask( "1 task" );
    MGR.AssignTask( "2 task" );

    MGR.AssignTask( "3 task" );

    MGR.AssignTask( "4 task" );   

    Sleep(5000);
    MGR.ShutDown();

    return 0;
}
