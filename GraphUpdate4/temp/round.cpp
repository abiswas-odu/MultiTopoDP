#include <iostream>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[])
{
	double number = 1.35679;

	cout<<number<<endl;
	cout<<setw(3)<<fixed<<setprecision(4)<<number<<endl;
	return 0;
}
