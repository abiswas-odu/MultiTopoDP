#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char *argv[])
{
	string temp = "1 2 3 4 ID";
	stringstream stream;
	stream<<temp;

	string str;
	while(!stream.eof())
	{
		stream>>str;
		cout<<str<<endl;
	}

	cout<<"Output the stream done ..."<<endl;

	return 0;
}
