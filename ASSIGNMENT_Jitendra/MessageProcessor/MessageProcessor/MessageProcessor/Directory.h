#ifndef H_DIRECTORY_
#define H_DIRECTORY_
#include<string>
#include<queue>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;
class Directory
{
public:
	Directory(string);
	int Read();
	fs::path getFile();
	bool isQueueEmpaty();
private:
	string strDirectoryName;
	queue<fs::path> queueFiles;
};

#endif