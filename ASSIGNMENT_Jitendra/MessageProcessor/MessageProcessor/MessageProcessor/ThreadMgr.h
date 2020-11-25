#ifndef H_THREADMGR_
#define H_THREADMGR_

#include "MessageProcessor.h"
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
using namespace std;

class ThreadMgr
{
public:
	ThreadMgr(int NumberOfthread, class MessageProcessor &);
	void join();
	bool isStopProcessing() { return stopProcessing; }
private:
	static void proccess(void* ptr);
	static void timer(void* ptr);
	MessageProcessor & messageProcessor;
	vector<thread > vecThread;
	mutex QueueMutex;
	mutex CounterMutex;
	condition_variable m_condVar;
	bool stopProcessing;
	bool end;
	thread timeTread;

};
#endif
