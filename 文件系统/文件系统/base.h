#ifndef BASE_H
#define BASE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>  
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
#include <malloc.h>
#include <io.h>
#include <iomanip>
#include <string>
#include <windows.h>
#include <algorithm>
#include <cstring>
#include <cstdio>
typedef struct AcessControlList
{
	bool read = 0;
	bool write = 0;
	bool execute = 0;
} fileACL;
typedef struct User
{
	std::string userName;//用户名
	std::string password;//密码
	struct Directory *userDir = NULL;//用户目录
} User;
typedef struct PhysicalBlockControl
{
	int blockSize = 1024;//每一个物理块大小
	int index = 0;//目前入的数量
	int blockIndexList[16];//分配的索引块好
} PBC;
typedef struct FileControlBlock
{
	int pbNUmber;//物理块号
	std::string fileName;//文件名
	std::string fileType;//文件类型
	std::string filePath;//文件路径
	//struct tm *createTime;//创建时间
	long long createTime;//创建时间
	double fileSize;//文件大小
	struct AcessControlList access[3];//文件执行权限,0用户，1组，2其他
	struct User *owner;//指向用户user的指针
	struct User *groupOwner;//指向用户组user的指针
	struct PhysicalBlockControl *firstBlock;//指向第一个物理块控制
}FCB;

typedef struct Directory
{
	int pbNUmber;//物理块号
	std::string fileName;//文件名
	std::string filePath;//文件路径
	struct User* owner;//文件所有者（组）
	struct User* groupOwner;//文件所属组
	struct FileControlBlock *fcb[10];//目录最多存放10个文件
	struct Directory *child[10];//指向二级文件结构,与文件控制块fcb相对应
	struct Directory *parent;//指向父目录
	struct FileControlBlock *dirFcb = NULL;//指向该目录的fcb
}DIR;

#endif