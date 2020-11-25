#ifndef H_SORTFILE_
#define H_SORTFILE_

#include "UDPMessage.h"
#include <string>
#include <map>
#include <iostream>
#include <mutex>
#include <condition_variable>

class SortFile
{
public :
	void proccess(fs::path &, unsigned long & totalMessageProccessed,  const unsigned long &max, mutex &CounterMutex, condition_variable &m_condVar, bool& waitCon);
	void write(UDPHeaderInfo&, string outFile);
	void CreateSortedFile(string outFile);

private:
	//fs::path file;
	map <unsigned long, UDPHeaderInfo > sequenceMap;
	//unordered_set<ifstream> unorderedSetInfiles;
};

#endif
