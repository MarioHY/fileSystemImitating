#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

struct Buffer 
{
	char data[1024];//一行的最大字符数是1024
	Buffer* next;//指向下一行
	Buffer* prev;//指向上一行
};

class TextController 
{
private:
	const char* originText;
	std::string filepath;//存入文件的路径
	std::string outputpath;//保存的文件路径
	std::ifstream inputStream;//读出流对象
	std::ofstream outputStream;//写入流对象
	int charCount;//字符长度
	int lineCount;//行数
	int atLine;//当前所在行数
	bool saved;//是否有改变
	Buffer* head;//头一行的指针
	Buffer* tail;//尾一行的指针
	Buffer* at;//当前行的指针

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