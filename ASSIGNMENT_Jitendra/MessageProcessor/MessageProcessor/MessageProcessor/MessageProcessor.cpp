#include "MessageProcessor.h"

MessageProcessor::MessageProcessor(string inputFolder, string outputFolder, int NumberOfthread, const unsigned long max):
    inputFolder(inputFolder), outputFolder(outputFolder), directory(inputFolder), NumberOfthread(NumberOfthread), max(max), end(false){}

void MessageProcessor::Proccess()
{
   
    directory.Read();

    cout << "************ Start reading from Queue ****************************\n";
    ThreadMgr threadMgr(NumberOfthread, *this);
    this_thread::sleep_for(5s);
    threadMgr.join();
    sortFile.CreateSortedFile(outputFolder);
}