#include"TestMessageProcessor.h"
#include "MessageProcessor.h"
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>


int main()
{
    freopen("test_output.txt", "w", stdout);
    string inputFile = "intput";
    string outputFolder = "out/Outfile.txt";
    fs::create_directory("out");
    fs::create_directory("intput");
    int NumberOfthread = 5;
    int processMaxMsgInSec = 1024;
    int fileCount = 20;
    int SeqNumbersCount = 100;
    MessageProcessor messageProcessor(inputFile, outputFolder, NumberOfthread, processMaxMsgInSec);
    createFlie(fileCount, SeqNumbersCount);
    messageProcessor.Proccess();
    ReadOutFile(outputFolder);
}
