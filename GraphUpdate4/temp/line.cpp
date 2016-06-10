#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
	ifstream traceFile("./tracePath.txt");

	string str;

	while(getline(traceFile, str))
	{
		char *strTemp;
		strTemp = new char [str.size()+1];
		strcpy(strTemp, str.c_str());

		char *pch;
		pch = strtok(strTemp, " ");

		while(pch != NULL)
		{
			cout<<atoi(pch)<<" ";
			pch = strtok(NULL, " ");
		}

		cout<<"\n=============="<<endl;

		delete [] strTemp;
	}

	return 0;
}
