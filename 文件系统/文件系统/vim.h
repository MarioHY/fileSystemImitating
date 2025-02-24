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
	std::string filePath;//文件路径
	int lineNum = 0;
	int BypeNum = 0;
	int atNum = 1;
	bool save;
	Line* head = nullptr;//头指针
	Line* tail = nullptr;//尾指针
	Line* atLine = nullptr;//当前行指针
public:
	Vim(std::string filePath);
	~Vim();
	int start();//开始操作界面入口
	void help();//功能显示
	void display();//展示界面
	void show();//显示所有数据
	std::string getFilePath();//获取文件路径
	int getLineNum();//获取当前行数
	bool getSave();//获取是否修改信息
	std::string getAtLineData();//获取当前行
	void toBegin();//到头一行
	void toEnd();//到尾一行
	void toPrev();//到上一行
	void toNext();//到下一行
	void quit();//清除数据
	void jump(int line);//移动到指定行
	void Save();//保存数据
	void insert(int line, std::string data);//插入数据到指定行
	void remove(int line);//删除行
	void replace(std::string data);//替换该行
	//////////////////////////以下是更新操作方式的尝试/////////////////////
	/**
	* @brief 颜色设置
	*
	*/
	void setColor(WORD color);
	int atCol = 1;//当前列
	bool change = 0;//闪
	void controlByShow();//显示光标

};
