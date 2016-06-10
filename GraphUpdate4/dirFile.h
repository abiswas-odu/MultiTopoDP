#ifndef DIRFILE_H
#define DIRFILE_H
#include <string>
#include <sys/stat.h>
using namespace std;

bool DFIsExist(string);//check if file exists
bool DFIsFile(string);//check if file is regular file
bool DFIsDirectory(string);//check if file is directory
off_t DFSize(string);//return file size by byte
string DFType(string);//return type of file
time_t DFLAccess(string);//return time of last access
time_t DFLModify(string);//return time of last modify
time_t DFLChange(string);//return time of last change
uid_t DFUID(string);//return user id
gid_t DFGID(string);//return group id
void DFInfo(string);//return file information
bool DFCDirectory(string, mode_t = 0755);//create folder
mode_t DFMode(string);//get folder or file permission

#endif
