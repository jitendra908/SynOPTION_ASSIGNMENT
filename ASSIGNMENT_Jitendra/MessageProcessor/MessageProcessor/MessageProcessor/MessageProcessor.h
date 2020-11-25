#ifndef H_MESSAGEPROCESSOR_
#define H_MESSAGEPROCESSOR_

#include "Directory.h"
#include "SortFile.h"
#include "ThreadMgr.h"
#include <iostream>
#include <string>

using namespace std;

class MessageProcessor
{
public:
	MessageProcessor(string inputFolder, string outputFolder, int, const unsigned long);
	void Proccess();
private:
	bool end;
	int NumberOfthread;
	unsigned long totalMessageProccessed;
	const unsigned long max;
	string inputFolder;
	string outputFolder;
	Directory directory;
	SortFile sortFile;
	friend class ThreadMgr;
};

#endif