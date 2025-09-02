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
	std::string userName;//�û���
	std::string password;//����
	struct Directory *userDir = NULL;//�û�Ŀ¼
} User;
typedef struct PhysicalBlockControl
{
	int blockSize = 1024;//ÿһ��������С
	int index = 0;//Ŀǰ�������
	int blockIndexList[16];//������������
} PBC;
typedef struct FileControlBlock
{
	int pbNUmber;//������
	std::string fileName;//�ļ���
	std::string fileType;//�ļ�����
	std::string filePath;//�ļ�·��
	//struct tm *createTime;//����ʱ��
	long long createTime;//����ʱ��
	double fileSize;//�ļ���С
	struct AcessControlList access[3];//�ļ�ִ��Ȩ��,0�û���1�飬2����
	struct User *owner;//ָ���û�user��ָ��
	struct User *groupOwner;//ָ���û���user��ָ��
	struct PhysicalBlockControl *firstBlock;//ָ���һ����������
}FCB;

typedef struct Directory
{
	int pbNUmber;//������
	std::string fileName;//�ļ���
	std::string filePath;//�ļ�·��
	struct User* owner;//�ļ������ߣ��飩
	struct User* groupOwner;//�ļ�������
	struct FileControlBlock *fcb[10];//Ŀ¼�����10���ļ�
	struct Directory *child[10];//ָ������ļ��ṹ,���ļ����ƿ�fcb���Ӧ
	struct Directory *parent;//ָ��Ŀ¼
	struct FileControlBlock *dirFcb = NULL;//ָ���Ŀ¼��fcb
}DIR;

#endif