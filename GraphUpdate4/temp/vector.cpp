#include <iostream>
#include <vector>
using namespace std;

void show(vector<int> container)
{
	for(int i = 0; i < container.size(); i++)
		cout<<container[i]<<" ";

	cout<<endl;
}

int main(int argc, char *argv[])
{
	vector<int> container;

	for(int i = 0; i < 5; i++)
		container.push_back(i);

	container.pop_back();

	container.erase(container.begin());

	show(container);

	return 0;
}
