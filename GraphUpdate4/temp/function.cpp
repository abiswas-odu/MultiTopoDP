#include <iostream>
using namespace std;

int getNumber(int &n)
{
	n++;

	return n*2;
}

int main(int argc, char *argv[])
{
	int n = 1;

	getNumber(n);

	cout<<n<<endl;

	return 0;
}
