#ifndef H_UDPMESSAGE_
#define H_UDPMESSAGE_
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

const int UDP_HEADER_LEN = 10;
const int SEQNUMBER_OFF_SET = 1;
const int SIZE_OFF_SET = 6;

class UDPHeaderInfo
{
public:
	UDPHeaderInfo(unsigned long size, unsigned long offSet, fs::path file) :size(size), offSet(offSet), file(file){}
	unsigned long getSize() { return size; }
	unsigned long getOffSet() { return offSet; }
	fs::path getfile() { return file; }
private:
	unsigned long size;
	unsigned long offSet;
	fs::path file;
};

#endif#pragma once
