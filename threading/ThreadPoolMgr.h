class CThreadPoolMgr
{
public:
    CThreadPoolMgr(void);
public:
    ~CThreadPoolMgr(void);

private:
    CThread*    m_ptrCThread[5];
    HANDLE        m_hThreadPool[5]; // Handle will be used in the end of Pool MGR for waiting on all thread to end
    int            m_nThreadCount;
public:

    void    Initialize(int nThread);    
    void    AssignTask( string strTask );
    void    ShutDown(void);
    int        GetFreeTherad(void);
    wstring    GetTaskMessage(void);
    int        GetThreadCount(void);
    HANDLE    GetMutex(void);
};
