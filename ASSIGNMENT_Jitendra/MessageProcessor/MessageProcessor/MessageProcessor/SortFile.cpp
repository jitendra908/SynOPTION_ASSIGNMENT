#include "SortFile.h"
#include <thread>
#include <sstream>


void SortFile::proccess(fs::path &file, unsigned long & totalMessageProccessed, const unsigned long & max, mutex &CounterMutex, condition_variable& m_condVar, bool & waitCon)
{
	char buf[UDP_HEADER_LEN + 10];
	ifstream inFile(file, ios::binary);
	unsigned long off = 0;
	std::ostringstream ss;
	ss << std::this_thread::get_id();
	std::string idstr = ss.str();
	string ID = "Thead " + idstr + " : ";
	while (1)
	{
		//string msg  = ID + " totalMessageProccessed "+ to_string(totalMessageProccessed) + " MAX " + to_string(max) + "\n";
		//cout << msg;
		CounterMutex.lock();
		if (totalMessageProccessed >= max)
		{
			CounterMutex.unlock();
			mutex m;
			unique_lock<std::mutex> lck(m);
			m_condVar.wait(lck, [&] {return waitCon; });
			waitCon = false;

		}
		else
		{
			CounterMutex.unlock();
		}
		

		inFile.read(buf, UDP_HEADER_LEN);
		if (inFile.eof())
		{
			inFile.close();
			break;
		}
		unsigned long* pSeqNumber = (unsigned long*)(buf + 1);
		unsigned long* pSize = (unsigned long*)(buf + 6);
		CounterMutex.lock();
		sequenceMap.insert({ *pSeqNumber, UDPHeaderInfo(*pSize, off, file) });
		++totalMessageProccessed;
		CounterMutex.unlock();
		off += *pSize;
		inFile.seekg(off);
		//this_thread::sleep_for(1s);
	}
}

void  SortFile::CreateSortedFile(string File)
{
	
	cout << "\n\nStart CreateSortedFile\n\n";
	ofstream outfile;
	outfile.open(File, ios::binary);
	outfile.close();
	for (auto it = sequenceMap.begin(); it != sequenceMap.end(); it++)
	{
		UDPHeaderInfo obj = (UDPHeaderInfo)it->second;
		write(obj, File);
	}
	cout << "\n\nEnd CreateSortedFile\n\n";
}

void  SortFile::write(UDPHeaderInfo & HeaderInfo, string File)
{
	unsigned long size = HeaderInfo.getSize();
	unsigned long file_off = HeaderInfo.getOffSet();
	fs::path in = HeaderInfo.getfile();

	//cout << " sorting file " << in << " " << file_off << "  " << size << endl;
	//string out = "out/Outfile.txt";
	ofstream outfile;
	outfile.open(File, ios::binary | ios::app);

	ifstream inFile(in, ios::binary);
	inFile.seekg(file_off);

	char* pBuf = new char[size];
	inFile.read(pBuf, size);

	outfile.write(pBuf, size);

	outfile.close();
	inFile.close();
}