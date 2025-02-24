#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

struct Buffer 
{
	char data[1024];//һ�е�����ַ�����1024
	Buffer* next;//ָ����һ��
	Buffer* prev;//ָ����һ��
};

class TextController 
{
private:
	const char* originText;
	std::string filepath;//�����ļ���·��
	std::string outputpath;//������ļ�·��
	std::ifstream inputStream;//����������
	std::ofstream outputStream;//д��������
	int charCount;//�ַ�����
	int lineCount;//����
	int atLine;//��ǰ��������
	bool saved;//�Ƿ��иı�
	Buffer* head;//ͷһ�е�ָ��
	Buffer* tail;//βһ�е�ָ��
	Buffer* at;//��ǰ�е�ָ��

public:
	TextController(std::string inputFile, std::string outputFile);
	~TextController();

	bool isSaved();
	int getLineNum();
	char* getLine();
	int getLineCount();
	std::string getFilePath();
	void showInfo();

	void goPrev();
	void goNext();
	void goHead();
	void goTail();
	void find(const char* str, bool regex);
	void insert(int line, const char* data);
	void replaceLine(const char* data);
	void replaceStr(const char* data, const char* str, bool a);
	void jump(int line);
	void remove(int line);
	void print();
	void reload();
	void save();
	void quit();
};