#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <limits>
#include <vector>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
using namespace std;
namespace fs = std::filesystem;



typedef std::vector<char> char_array;


char arr[1024] = { 0 };

char_array charset()
{
    //Change this to suit
    return char_array(
        {
        'A','B','C','D','E','F',
        'G','H','I','J','K',
        'L','M','N','O','P',
        'Q','R','S','T','U',
        'V','W','X','Y','Z',
        'a','b','c','d','e','f',
        'g','h','i','j','k',
        'l','m','n','o','p',
        'q','r','s','t','u',
        'v','w','x','y','z'
        });
}

std::string random_string()
{

    const auto ch_set = charset();

    //1) create a non-deterministic random number generator      
    std::default_random_engine rng(std::random_device{}());

    //2) create a random number "shaper" that will give
    //   us uniformly distributed indices into the character set
    std::uniform_int_distribution<> dist(0, ch_set.size() - 1);

    //3) create a function that ties them together, to get:
    //   a non-deterministic uniform distribution from the 
    //   character set of your choice.
    auto randchar = [ch_set, &dist, &rng]() {return ch_set[dist(rng)]; };

    //4) set the length of the string you want and profit!        
    auto length = dist(rng) % 10;
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void createFlie(const string &file, vector<unsigned long> & seqNums)
{
    std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                               //and seed it with entropy.
    //Define the distribution, by default it goes from 0 to MAX(unsigned long long)
    //or what have you.
    std::uniform_int_distribution<unsigned long long> distr;

    unsigned long baseSize = 2 * sizeof(char) + 2 * sizeof(unsigned long) + sizeof(double);


    ofstream myfile;
    myfile.open(file, ios::binary);

    cout << "Creating File : \t" << file << "\n";
    for (auto it = seqNums.begin(); it != seqNums.end(); it++)
    {
        char type = 'A' + distr(eng) % 26;
        unsigned long seqNumber = *it;
        char side = distr(eng) % 26;
        string Ticker = random_string();
        double price = (double)distr(eng);
        unsigned long size = baseSize + Ticker.length();


        myfile.write(&type, sizeof(char));
        myfile.write((char*)&seqNumber, sizeof(unsigned long));
        myfile.write(&side, sizeof(char));
        myfile.write((char*)&size, sizeof(unsigned long));
        myfile.write(Ticker.c_str(), Ticker.length());
        myfile.write((char*)&price, sizeof(double));

        //cout << "seqNumber -> " << seqNumber << "\tUDP Size -> " << size << endl;

    }

    myfile.close();


}

class FileInfo
{
public:
    static void createFileInfo(int fileCount, int SeqNumbersCount)
    {
        std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
        std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                                   //and seed it with entropy.
        //Define the distribution, by default it goes from 0 to MAX(unsigned long long)
        //or what have you.
        std::uniform_int_distribution<unsigned long long> distr;
        unsigned long long div = ((SeqNumbersCount * fileCount) - (SeqNumbersCount * fileCount) / 10 );

        FileInfo obj;
        for (auto i = 0; i < fileCount ; i++)
        {
            obj.file = "intput/input_" + to_string((i + 1));
            //cout << obj.file << endl ;
            for (auto seq = 0; seq < SeqNumbersCount; seq++)
            {
                unsigned long long randNum = distr(eng) % div;
                obj.seqNums.push_back(randNum);
            }
            vecFileInfo.push_back(obj);
        }
    }
    static vector<FileInfo> vecFileInfo;
    string file;
    vector<unsigned long> seqNums;
    
};

vector<FileInfo> FileInfo::vecFileInfo;

void createFlie(int fileCount, int SeqNumbersCount)
{
    /*string files[] = { "intput/Infile1.txt", "intput/Infile2.txt" };
    unsigned long seqNumbers[][7] = { {3,4,12,9,1,2,2}, {2,5,6,8,7,10,11} };
    for (auto i = 0; i < 2; i++)
    {
        createFlie(files[i], seqNumbers[i], 7);
    }*/
    cout << "\n\n***************** Start file Creation *************************\n\n";
    FileInfo::createFileInfo(fileCount, SeqNumbersCount);
    for (auto it = FileInfo::vecFileInfo.begin(); it != FileInfo::vecFileInfo.end(); it++)
    {
        createFlie(it->file, it->seqNums);
    }
    cout << "\n\n***************** End file Creation *************************\n\n";
}


void ReadOutFile(string file)
{
    cout << "\n\n******************* Start Reading output File " << file << " **************\n";

    ifstream inFile(file, ios::binary);
    int off = 0;
    while (1)
    {

        char buf[22];
        char type;
        unsigned long seqNumber;
        unsigned long size = 0;

        inFile.read(buf, 10);
        if (inFile.eof())
        {
            inFile.close();
            break;
        }

        char* p = buf;
        type = *p;
        unsigned long* unp = (unsigned long*)(buf + 1);
        seqNumber = *unp;
        unp = (unsigned long*)(buf + 6);
        size = *unp;

        off += size;
        inFile.seekg(off);
        cout << "Seq Number -> \t" << seqNumber << "\tUDP Size -> \t" << size << endl;

    }
    cout << "\n\n******************* End Reading output File " << file << " **************\n";
}

