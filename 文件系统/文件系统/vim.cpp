#include "vim.h"
/**
* @brief 初始化数据
*/
Vim::Vim(std::string filePath) :filePath(filePath), save(true), head(new Line()), atLine(head), tail(head)
{
	//SetConsoleOutputCP(65001);//设置输出编码为UTF-8
	////////////////////////////////初始化基本数据//////////////////////////////////
	std::ifstream in(filePath);
	std::string line;//临时行
	lineNum = 0;//总行数
	if (in)
	{
		while (std::getline(in, line))
		{
			lineNum++;
			Line* newLine = new Line();
			//std::cout << line << std::endl;
			newLine->data = line;
			newLine->prev = tail;
			tail->next = newLine;
			tail = newLine;
			this->BypeNum += line.size();
		}

	}
	else
	{
		std::cout << "no such file!" << std::endl;
		return;
	}
	if (lineNum == 0)//如果文件为空，为了保存可操作性，输入一行空进行操作
	{
		lineNum++;
		Line* newLine = new Line();
		newLine->data = "";
		newLine->prev = tail;
		tail->next = newLine;
		tail = newLine;
	}

	atLine = head;
	head->next->prev = nullptr;
	head = head->next;
	delete atLine;
	atLine = head;
	in.close();
	//SetConsoleOutputCP(936);//设置编码为GBK
}
/**
* @brief 释放资源
*/
Vim::~Vim()
{
	Line* p = nullptr;
	while (head!=nullptr)
	{
		p = head;
		head = p->next;
		delete p;
	}
}
/**
* @brief 文本操作入口,并返回操作结束后的字符大小
*/
int Vim::start()
{
	std::string choice;
	while (true)
	{
		//this->controlByShow();
		std::cout << "## 正在编辑文件" << this->getFilePath() << std::endl;
		std::cout << "## 通过输入h来获取更多帮助..." << std::endl;
		std::cout << std::endl;
		this->display();
		std::cout << std::endl;


		std::cout << "~" << std::endl;
		std::cout << this->getLineNum() << ": " << this->getAtLineData() << std::endl;
		std::cout << "# ";
		std::getline(std::cin, choice);

		

		size_t pos = choice.find_first_of(' ');
		std::string first = choice.substr(0, pos);
		std::string second = choice.substr(pos + 1);
		if (first == "h")
		{
			system("cls");
			this->help();
			system("pause");
		}
		else if (first == "clear")
		{
			system("cls");
			continue;
		}
		else if (first == "v")//显示所有数据
		{
			system("cls");
			this->show();
			system("pause");
		}
		else if (first == "gg")//返回头部
		{
			this->toBegin();
		}
		else if (first == "ctrl" && second == "g")//返回尾部
		{
			this->toEnd();
		}
		else if (first == "k")//向上
		{
			this->toPrev();
		}
		else if (first == "j")//向下
		{
			this->toNext();
		}
		else if (second == "G" && second != first)//跳转到指定行
		{
			int flag = -1;//判断要跳转的行是否合理
			for (int i = 0; i < first.length(); i++)
			{
				if (first[i] < '0' || first[i] > '9')
				{
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				std::cout << "输入的行数不是数字!" << std::endl;
				Sleep(360);
				system("cls");
				continue;
			}
			int num = std::stoi(first.c_str());//转换为整数

			if (num <= 0 || num > lineNum)
			{
				std::cout << "行数错误!" << std::endl;
				Sleep(360);
				system("cls");
				continue;
			}

			this->jump(num);//跳转
		}
		else if (first == "i" && second != first)//插入指定行
		{
			size_t pos2 = second.find_first_of(' ');
			std::string second_1 = second.substr(0, pos2);
			std::string second_2 = second.substr(pos2 + 1);
			if (second_2 != second_1)
			{
				//std::cout << second_1 << std::endl;
				int flag = -1;
				for (int i = 0; i < second_1.length(); i++)
				{
					if (second_1[i] < '0' || second_1[i] > '9')
					{
						flag = 1;
						break;
					}
				}
				if (flag == 1)
				{
					continue;
				}
				int num = std::stoi(second_1.c_str());//将字符转数字
				//std::cout << num << std::endl;
				this->insert(num, second_2);
			}
		}
		else if (first == "dd")//删除改行
		{
			if (second != first)
			{
				int flag = -1;
				for (int i = 0; i < second.length(); i++)//判断second中字符是否合法，是否可以转换为数字
				{
					if (second[i] < '0' || second[i] > '9')
					{
						flag = 1;
						break;
					}
				}
				if (flag == 1)
				{
					continue;
				}
				int num = std::stoi(second.c_str());//将字符转数字
				this->remove(num);
			}
			else
			{
				this->remove(this->atNum);
			}
		}
		else if (first == "r")
		{
			this->replace(second);
		}
		else if (first == "w")//保存
		{
			this->Save();
		}
		else if (first == "q")//退出
		{
			if (!this->save)
			{
				std::cout << "您还未进行保存，是否保存修改结果?(yes/no)" << std::endl;
				std::string temp;
				std::cout << "请输入你的操作: ";
				std::getline(std::cin, temp);
				if (temp == "yes")
				{
					this->Save();
				}
				else if (temp == "no")
				{
					
				}
				else
				{
					std::cout << "操作无效!" << std::endl;
					continue;
				}
			}
			//this->quit();
			break;
		}
		else if (first == "wq")//保存并退出
		{
			this->Save();
			//this->quit();
			break;
		}

		system("cls");//清屏
	}
	return this->BypeNum;
}
/**
* @brief 帮助菜单
*/
void Vim::help()
{
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "           h                     显示菜单" << std::endl;
	std::cout << "           clear                 清屏" << std::endl;
	std::cout << "           v                     显示所有内容" << std::endl;
	std::cout << "           gg                    返回头部" << std::endl;
	std::cout << "           ctrl g                返回尾部" << std::endl;
	//std::cout << "           h                     向左移动" << std::endl;
	std::cout << "           k                     向上一行" << std::endl;
	std::cout << "           j                     向下一行" << std::endl;
	//std::cout << "           l                     向右移动" << std::endl;
	std::cout << "           [行数] G              跳转到指定行" << std::endl;
	std::cout << "           i [行数] [数据]       插入数据到指定行" << std::endl;
	std::cout << "           r [数据]              替换当前行的数据" << std::endl;
	std::cout << "           dd                    删除当前行      " << std::endl;
	std::cout << "           dd [行数]             删除指定行      " << std::endl;
	std::cout << "           w                     保存        " << std::endl;
	std::cout << "           q                     退出        " << std::endl;
	std::cout << "           wq                    保存并退出      " << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	//std::cout << "" << std::endl;
	//std::cout << "" << std::endl;
		
}
/**
* @brief 展示界面,随着当前指定行内容动态显示文本数据,每次只显示15行
*/
void Vim::display()
{
	Line* p = head;
	int height = 1;

	if (lineNum <= 15)//【begin，end】全部都小于15,直接输出全部
	{
		std::cout << "---------------------------------  BEGIN  ---------------------------------" << std::endl;
		while (p != nullptr)
		{
			std::cout << height++ << ": " << p->data << std::endl;
			p = p->next;
		}
		std::cout << "---------------------------------   END   ---------------------------------" << std::endl;
	}
	else//全部大于15，看目前在第几行进行输出
	{
		if (atNum <= 7)//【begin， 15】
		{
			while (p != nullptr)
			{
				if (height == 1)
				{
					std::cout << "---------------------------------  BEGIN  ---------------------------------" << std::endl;
				}
				std::cout << height++ << ": " << p->data << std::endl;
				p = p->next;
				if (height == 16)
				{
					std::cout << "---------------------------------   ...  ----------------------------------" << std::endl;
					break;
				}
			}
		}
		else if ((lineNum - 7) <= atNum)//【end - 15, end】
		{
			while (p != nullptr)
			{
				if (height == (lineNum - 15))
				{
					std::cout << "---------------------------------   ...  ----------------------------------" << std::endl;
				}
				if (height >= (lineNum - 15))
				{
					std::cout << height++ << ": " << p->data << std::endl;
					p = p->next;
				}
				else
				{
					height++;
					p = p->next;
					continue;
				}

				if (p == nullptr)
				{
					std::cout << "---------------------------------   END   ---------------------------------" << std::endl;
				}
			}
		}
		else//【atNum-7， atNum+7】
		{

			while (p != nullptr)
			{
				if (height == (atNum - 7))
				{
					std::cout << "---------------------------------   ...  ----------------------------------" << std::endl;
				}
				if (height < (atNum - 7))
				{
					height++;
					p = p->next;
					continue;
				}
				else
				{
					std::cout << height++ << ": " << p->data << std::endl;
					p = p->next;
				}

				if (height == (atNum + 7) + 1)
				{
					std::cout << "---------------------------------   ...  ----------------------------------" << std::endl;
					break;
				}
			}

		}

	}
}
/**
* @brief 显示所有数据
*/
void Vim::show()
{
	Line* p = head;
	int height = 1;
	std::cout << "---------------------------------  BEGIN  ---------------------------------" << std::endl;
	while (p != nullptr)
	{
		std::cout << height++ << ": " << p->data << std::endl;
		p = p->next;
	}
	std::cout << "---------------------------------   END   ---------------------------------" << std::endl;
}
/**
@ brief 获取当前的行数
*/
int Vim::getLineNum()
{
	return this->atNum;
}
/**
* @brief 返回保存情况
*/
bool Vim::getSave()
{
	return this->save;
}
/**
* @brief 返回当前行数据
*/
std::string Vim::getAtLineData()
{
	return this->atLine->data;
}
/**
* @brief
*/
std::string Vim::getFilePath()
{
	return this->filePath;
}
/**
* @brief 移动到头
*/
void Vim::toBegin()
{
	this->atLine = this->head;
	this->atNum = 1;
}
/**
* @brief 移动到尾
*/
void Vim::toEnd()
{
	this->atLine = this->tail;
	this->atNum = this->lineNum;
}
/**
* @brief 移动到上一行
*/
void Vim::toPrev()
{
	if (this->atLine->prev == nullptr)
	{
		std::cout << "已经到头了" << std::endl;
		return;
	}
	this->atLine = this->atLine->prev;
	this->atNum--;
}

/**
* @brief 移动到下一行
*/
void Vim::toNext()
{
	if (this->atLine->next == nullptr)
	{
		std::cout << "已经到底了" << std::endl;
		return;
	}
	this->atLine = this->atLine->next;
	this->atNum++;
}
/**
* @brief 移动到指定行
*/
void Vim::jump(int line)
{
	int n = 1;
	Line* temp = head;
	while (temp != nullptr)
	{
		if (line == n)
		{
			this->atNum = n;
			this->atLine = temp;
			break;
		}
		n++;
		temp = temp->next;
	}
}

/**
* @brief 保存数据
*/
void Vim::Save()
{
	std::ofstream  ofs; //创建流对象 
	ofs.open(this->getFilePath(), std::ios::out);//打开文件（out模式为覆盖） 
	Line* temp = this->head;
	while (temp!=nullptr)
	{
		ofs << temp->data << "\t\n";
		temp = temp->next;
	}
	ofs.close();//关闭文件	
}

/**
* @brief 退出
*/
void Vim::quit()
{
	this->~Vim();
}

/**
* @brief 插入一行
*/
void Vim::insert(int line, std::string data)
{
	if (line > this->lineNum + 1)
	{
		std::cout << "行数错误！" << std::endl;
		return;
	}
	else
	{
		Line* newLine = new Line();
		newLine->data = data;
		if (line == (this->lineNum + 1))
		{
			//std::cout << lineNum;
			tail->next = newLine;
			newLine->prev = tail;
			tail = newLine;

			this->lineNum++;
		}
		else if (line == 1)
		{
			newLine->next = head;
			head->prev = newLine;
			head = newLine;
			if (this->lineNum == 1 && this->atLine->data == "")//如果第一行为空，那么第二行就不要保存了
			{
				Line* temp = head->next;
				head->next = nullptr;
				delete temp;
				tail = head;
			}
			else
			{
				this->lineNum++;
			}
		}
		else
		{
			//////////////////////////////////查找对应位置///////////////////
			int tempLine = 1;
			Line* temp = head;
			while (temp != nullptr)
			{
				if (tempLine == line)
				{
					break;
				}
				temp = temp->next;
				tempLine++;
			}

			Line* tempPrev = nullptr;
			tempPrev = temp->prev;
			tempPrev->next = newLine;
			newLine->prev = tempPrev;
			newLine->next = temp;
			temp->prev = newLine;

			this->lineNum++;
		}
		this->BypeNum += data.size();
		////////////////重新编写当前行数////////////////////
		if (this->atNum == line)
		{
			atLine = newLine;
		}
		else if (this->atNum > line)
		{
			atNum++;
		}
		else
		{

		}
		this->save = false;//有更新变动，表示未保存标记
	}
}

/**
* @brief 删除指定行
*/
void Vim::remove(int line)
{
	if (line < 0 | line > this->lineNum)
	{
		std::cout << "行数错误！" << std::endl;
		return;
	}
	Line* temp = nullptr;
	if (line == 1)
	{
		if (this->lineNum == 1)
		{
			this->BypeNum -= head->data.size();

			head->data = "";

			this->save = false;
		}
		else
		{
			temp = head;
			head = head->next;
			head->prev = nullptr;

			this->BypeNum -= temp->data.size();
			delete temp;
			////////////////重新编写当前行数////////////////////
			if (this->atNum == line)
			{
				atLine = head;
			}
			else if (this->atNum > line)
			{
				atNum--;
			}
			else
			{

			}
			this->save = false;//有更新变动，表示未保存标记
			this->lineNum--;
		}


	}
	else if (line == this->lineNum)
	{
		//文本必须至少有2行才能进入这个if，而且能进入说明line合法，也就是存在2个以上行可操作的尾部删除
		temp = tail;
		tail = tail->prev;
		tail->next = nullptr;

		this->BypeNum -= temp->data.size();

		delete temp;
		////////////////重新编写当前行数////////////////////
		if (this->atNum == line)
		{
			atLine = tail;
			this->atLine--;
		}

		this->save = false;//有更新变动，表示未保存标记
		this->lineNum--;
	}
	else//经过elseif过滤,只能是head与tail之间
	{
		//////////////////////////////////查找对应位置///////////////////
		int tempLine = 1;
		temp = head;
		while (temp != nullptr)
		{
			if (tempLine == line)
			{
				break;
			}
			temp = temp->next;
			tempLine++;
		}

		Line* tempPrev = nullptr;
		tempPrev = temp->prev;
		temp->next->prev = tempPrev;
		tempPrev->next = temp->next;

		this->BypeNum -= temp->data.size();
		delete temp;

		////////////////重新编写当前行数////////////////////
		if (this->atNum == line)
		{
			atLine = head;
			atNum = 1;
		}
		else if (this->atNum > line)
		{
			atNum--;
		}
		else
		{

		}
		this->lineNum--;
		this->save = false;//有更新变动，表示未保存标记
	}

}

/**
* @brief 替换该行
*/
/////////////////
void Vim::replace(std::string data)
{
	atLine->data = data;
	this->save = false;
}
/////////以下是更新操作方式的尝试/////////////////////
/**
* @brief 颜色设置
*
*/
void Vim::setColor(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void Vim::controlByShow()
{
	Line* p = head;
	int height = 1;
	while (p != nullptr)
	{
		setColor(FOREGROUND_RED | FOREGROUND_GREEN);//设置黄色字体
		std::cout << height++;
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置默认颜色
		if (height > 10)
		{
			std::cout << " ";
		}
		else
		{
			std::cout << "  ";
		}
		if (height - 1 == atNum)
		{
			if (!change)
			{
				for (int i = 0; i < p->data.size(); i++)
				{
					if (i == atCol - 1)
					{
						std::cout << "_";
						change = !change;
					}
				}
			}
			else
			{
				change = !change;
			}
		}
		else
		{
			std::cout << p->data << std::endl;
		}

		p = p->next;
	}
	Sleep(0.5);

}


