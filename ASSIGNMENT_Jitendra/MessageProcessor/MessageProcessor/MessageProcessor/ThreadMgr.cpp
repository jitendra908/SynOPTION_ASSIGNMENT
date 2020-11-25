#include "ThreadMgr.h"
#include <sstream>

ThreadMgr::ThreadMgr(int NumberOfthread, MessageProcessor& messageProcessor):messageProcessor(messageProcessor)
{
    stopProcessing = false;
    for (int i = 0; i< NumberOfthread; i++)
        vecThread.push_back(thread(ThreadMgr::proccess, this));

    timeTread = thread(ThreadMgr::timer, this);
}

void ThreadMgr::join()
{
    for (auto it = vecThread.begin(); it != vecThread.end(); it++)
        it->join();

    end = true;
    timeTread.join();
}

void ThreadMgr::timer(void* ptr)
{
    ThreadMgr& threadMgr = *((ThreadMgr*)ptr);
    MessageProcessor& messageProcessor = threadMgr.messageProcessor;
    unsigned long& totalMessageProccessed = messageProcessor.totalMessageProccessed;
    mutex& CounterMutex = threadMgr.CounterMutex;
    condition_variable& m_condVar = threadMgr.m_condVar;
    bool& stopProcessing = threadMgr.stopProcessing;
    bool& end = threadMgr.end;
    
    string msg = "\n\nStart Timer Thread \n\n";
    cout << msg;
    while (1)
    {
        this_thread::sleep_for(1s);
        msg = "\n\nProcessed Total message in last one min : \t" + to_string(totalMessageProccessed) + "\n\n";
        cout << msg;
    
        CounterMutex.lock();
        totalMessageProccessed = 0;
        stopProcessing = true;
        CounterMutex.unlock();
        m_condVar.notify_all();
        if (end)
            break;
        
    }
    msg = "\n\nEnd Timer Thread \n\n";
    cout << msg;
}
void ThreadMgr::proccess(void * ptr)
{
    static int count = 0;
    string tCount = to_string(++count); 
    ThreadMgr& threadMgr = *((ThreadMgr*)ptr);
    MessageProcessor& messageProcessor = threadMgr.messageProcessor;
    unsigned long& totalMessageProccessed = messageProcessor.totalMessageProccessed;
    const unsigned long& max = messageProcessor.max;
    Directory & directory = messageProcessor.directory;
    SortFile & sortFile = messageProcessor.sortFile;
    mutex & QueueMutex = threadMgr.QueueMutex;
    mutex& CounterMutex = threadMgr.CounterMutex;
    condition_variable& m_condVar = threadMgr.m_condVar;
    bool & stopProcessing = threadMgr.stopProcessing;

    std::ostringstream ss;
    ss << std::this_thread::get_id();
    std::string idstr = ss.str();
    string ID = "Thead " + idstr + " : ";
    
    string msg = "\n\nStart \t" + ID + "\n\n";
	cout << msg;
    while (1)
    {
        fs::path file;
        QueueMutex.lock();
        if (!directory.isQueueEmpaty())
        {
            file = directory.getFile();
            QueueMutex.unlock();
            msg = "\n\n" + ID  + " Start processing -> \t" + file.string() + "\n\n";
            cout << msg;
            sortFile.proccess(file, totalMessageProccessed, max, CounterMutex, m_condVar, stopProcessing);
            msg = "\n\n" + ID + " End processing -> \t" + file.string() + "\n\n";   
        }
        else
        {
            QueueMutex.unlock();
            break;
        }
    }
    msg = "\n\nEnd \t" + ID + "\n\n";
    cout << msg;
}