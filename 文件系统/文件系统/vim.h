#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>  
#include <string.h>
#include <string>
#include <algorithm>
#include <cstdio>
#include <Windows.h>

typedef struct fileLine
{
	std::string data;
	struct fileLine* next = nullptr;
	struct fileLine* prev = nullptr;
}Line;
class Vim
{
private:
	std::string filePath;//�ļ�·��
	int lineNum = 0;
	int BypeNum = 0;
	int atNum = 1;
	bool save;
	Line* head = nullptr;//ͷָ��
	Line* tail = nullptr;//βָ��
	Line* atLine = nullptr;//��ǰ��ָ��
public:
	Vim(std::string filePath);
	~Vim();
	int start();//��ʼ�����������
	void help();//������ʾ
	void display();//չʾ����
	void show();//��ʾ��������
	std::string getFilePath();//��ȡ�ļ�·��
	int getLineNum();//��ȡ��ǰ����
	bool getSave();//��ȡ�Ƿ��޸���Ϣ
	std::string getAtLineData();//��ȡ��ǰ��
	void toBegin();//��ͷһ��
	void toEnd();//��βһ��
	void toPrev();//����һ��
	void toNext();//����һ��
	void quit();//�������
	void jump(int line);//�ƶ���ָ����
	void Save();//��������
	void insert(int line, std::string data);//�������ݵ�ָ����
	void remove(int line);//ɾ����
	void replace(std::string data);//�滻����
	//////////////////////////�����Ǹ��²�����ʽ�ĳ���/////////////////////
	/**
	* @brief ��ɫ����
	*
	*/
	void setColor(WORD color);
	int atCol = 1;//��ǰ��
	bool change = 0;//��
	void controlByShow();//��ʾ���

};
