#include <iostream>
#include <iomanip>
#include <string>
#include <sys/stat.h>
#include "dirFile.h"
using namespace std;

mode_t DFMode(string str)
{
	if(!DFIsExist(str))
		return 0;

	struct stat temp;

	lstat(str.c_str(), &temp);

	return temp.st_mode;
}

bool DFCDirectory(string str, mode_t mode)
{
	int succ;

	succ = mkdir(str.c_str(), mode);

	if(!succ)
		return true;
	else
	{
		cerr<<str<<" had been created or can not create direcotry "<<str<<", permission is denied ..."<<endl;
		return false;
	}
}

off_t DFSize(string str)
{
	if(!DFIsExist(str))
	{
		cerr<<str<<" does not exist ..."<<endl;
		return 0;
	}

	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	return temp.st_size;
}

bool DFIsDirectory(string str)
{
	if(DFType(str) == "Directory")
		return true;

	return false;
}

bool DFIsFile(string str)
{
	if(DFType(str) == "File")
		return true;

	return false;
}

gid_t DFGID(string str)
{
	if(!DFIsExist(str))
	{
		cerr<<str<<" does not exist ..."<<endl;
		return -1;
	}

	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	return temp.st_gid;
}

uid_t DFUID(string str)
{
	if(!DFIsExist(str))
	{
		cerr<<str<<" does not exist ..."<<endl;
		return -1;
	}

	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	return temp.st_uid;
}

bool DFIsExist(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	return !succ;
}

void DFInfo(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	if(DFIsExist(str))
	{
		cout<<setw(10)<<"Name: "<<str<<endl;
		cout<<setw(10)<<"Type: "<<setw(10)<<DFType(str)<<endl;
		cout<<setw(10)<<"Size: "<<setw(10)<<DFSize(str)/1024.0<<endl;
		cout<<setw(10)<<"Links: "<<setw(10)<<temp.st_nlink<<endl;
		cout<<setw(10)<<"User ID: "<<setw(10)<<DFUID(str)<<" Group ID: "<<setw(10)<<DFGID(str)<<" Device ID: "<<setw(10)<<temp.st_rdev<<endl;
		time_t timeTemp;
		timeTemp = DFLAccess(str);
		cout<<"Last Access: "<<setw(10)<<ctime(&timeTemp);
		timeTemp = DFLModify(str);
		cout<<"Last Modify: "<<setw(10)<<ctime(&timeTemp);
		timeTemp = DFLChange(str);
		cout<<"Last Change: "<<setw(10)<<ctime(&timeTemp);
	}
	else
	{
		cerr<<str<<" does not exist ..."<<endl;
	}
}

time_t DFLAccess(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	if(!succ)
		return temp.st_atime;
	else
	{
		cerr<<str<<" does not exist ..."<<endl;
		return 0;
	}
}

time_t DFLModify(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	if(!succ)
		return temp.st_mtime;
	else
	{
		cerr<<str<<" does not exist ..."<<endl;
		return 0;
	}
}

time_t DFLChange(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	if(!succ)
		return temp.st_ctime;
	else
	{
		cerr<<str<<" does not exist ..."<<endl;
		return 0;
	}
}

string DFType(string str)
{
	struct stat temp;

	int succ;

	succ = lstat(str.c_str(), &temp);

	if(S_ISREG(temp.st_mode))
		return "File";
	else if(S_ISDIR(temp.st_mode))
		return "Directory";
	else if(S_ISCHR(temp.st_mode))
		return "Character device";
	else if(S_ISBLK(temp.st_mode))
		return "Block devide";
	else if(S_ISFIFO(temp.st_mode))
		return "FIFO";
	else if(S_ISLNK(temp.st_mode))
		return "Link";
	else if(S_ISSOCK(temp.st_mode))
		return "Socket";
	else
		return "Non-determined type";
}
