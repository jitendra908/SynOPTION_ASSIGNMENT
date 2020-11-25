#include <iostream>
#include "Directory.h"


Directory::Directory(string strDirectoryName):strDirectoryName(strDirectoryName)
{
    //cout << "Directry Name is : " << strDirectoryName << endl;
}

int Directory::Read()
{
    for (const auto& entry : fs::directory_iterator(strDirectoryName))
    {
        //std::cout<< "Directory::Read " << entry.path() << std::endl;
        queueFiles.push(entry.path());
    }

}

bool Directory::isQueueEmpaty()
{
    return queueFiles.empty();
}

fs::path Directory::getFile()
{
    fs::path filePath = queueFiles.front();
    queueFiles.pop();
    return filePath;
}

