#include <iostream>
using namespace std;

void modify(int *(&array))
{
	array = new int [10];

	for(int i = 0; i < 10; i++)
		array[i] = i;
}

int main(int argc, char *argv[])
{
	int *array;

	modify(array);

	for(int i = 0; i < 10; i++)
		cout<<array[i]<<endl;

	return 0;
}
