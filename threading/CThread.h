class CThread
{
public:
    CThread(void);
    ~CThread(void);

private:   
    DWORD    m_ThreadID;        // ID of thread
    HANDLE    m_hThread;        // Handle to thread
    BOOL    m_bIsFree;            // Flag indicates which is free thread
    string    m_strMessage;    // Message or Task 
    HANDLE  m_hWorkEvent[2]; // m_hWorkEvent[0] Work Event. m_hWorkEvent[1] ShutDown event


public:
    void    Run(void); // Function which will get executed by thread
    static    DWORD WINAPI ThreadProc(LPVOID Param); // Thread procedure
    BOOL    IsFree(); // Determine if thread is free or not
    void    CreateWorkerThread();
    HANDLE    GetThreadHandle();
    DWORD    GetThreadID();
    void    SetMessage(string strMessage);
    void    SignalWorkEvent();
    void    SignalShutDownEvent();
    void    SetThreadBusy();
    void    ReleaseHandles();
}; 
