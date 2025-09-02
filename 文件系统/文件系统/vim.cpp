#include "vim.h"
/**
* @brief ��ʼ������
*/
Vim::Vim(std::string filePath) :filePath(filePath), save(true), head(new Line()), atLine(head), tail(head)
{
	//SetConsoleOutputCP(65001);//�����������ΪUTF-8
	////////////////////////////////��ʼ����������//////////////////////////////////
	std::ifstream in(filePath);
	std::string line;//��ʱ��
	lineNum = 0;//������
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
	if (lineNum == 0)//����ļ�Ϊ�գ�Ϊ�˱���ɲ����ԣ�����һ�пս��в���
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
	//SetConsoleOutputCP(936);//���ñ���ΪGBK
}
/**
* @brief �ͷ���Դ
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
* @brief �ı��������,�����ز�����������ַ���С
*/
int Vim::start()
{
	std::string choice;
	while (true)
	{
		//this->controlByShow();
		std::cout << "## ���ڱ༭�ļ�" << this->getFilePath() << std::endl;
		std::cout << "## ͨ������h����ȡ�������..." << std::endl;
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
		else if (first == "v")//��ʾ��������
		{
			system("cls");
			this->show();
			system("pause");
		}
		else if (first == "gg")//����ͷ��
		{
			this->toBegin();
		}
		else if (first == "ctrl" && second == "g")//����β��
		{
			this->toEnd();
		}
		else if (first == "k")//����
		{
			this->toPrev();
		}
		else if (first == "j")//����
		{
			this->toNext();
		}
		else if (second == "G" && second != first)//��ת��ָ����
		{
			int flag = -1;//�ж�Ҫ��ת�����Ƿ����
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
				std::cout << "�����������������!" << std::endl;
				Sleep(360);
				system("cls");
				continue;
			}
			int num = std::stoi(first.c_str());//ת��Ϊ����

			if (num <= 0 || num > lineNum)
			{
				std::cout << "��������!" << std::endl;
				Sleep(360);
				system("cls");
				continue;
			}

			this->jump(num);//��ת
		}
		else if (first == "i" && second != first)//����ָ����
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
				int num = std::stoi(second_1.c_str());//���ַ�ת����
				//std::cout << num << std::endl;
				this->insert(num, second_2);
			}
		}
		else if (first == "dd")//ɾ������
		{
			if (second != first)
			{
				int flag = -1;
				for (int i = 0; i < second.length(); i++)//�ж�second���ַ��Ƿ�Ϸ����Ƿ����ת��Ϊ����
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
				int num = std::stoi(second.c_str());//���ַ�ת����
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
		else if (first == "w")//����
		{
			this->Save();
		}
		else if (first == "q")//�˳�
		{
			if (!this->save)
			{
				std::cout << "����δ���б��棬�Ƿ񱣴��޸Ľ��?(yes/no)" << std::endl;
				std::string temp;
				std::cout << "��������Ĳ���: ";
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
					std::cout << "������Ч!" << std::endl;
					continue;
				}
			}
			//this->quit();
			break;
		}
		else if (first == "wq")//���沢�˳�
		{
			this->Save();
			//this->quit();
			break;
		}

		system("cls");//����
	}
	return this->BypeNum;
}
/**
* @brief �����˵�
*/
void Vim::help()
{
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "           h                     ��ʾ�˵�" << std::endl;
	std::cout << "           clear                 ����" << std::endl;
	std::cout << "           v                     ��ʾ��������" << std::endl;
	std::cout << "           gg                    ����ͷ��" << std::endl;
	std::cout << "           ctrl g                ����β��" << std::endl;
	//std::cout << "           h                     �����ƶ�" << std::endl;
	std::cout << "           k                     ����һ��" << std::endl;
	std::cout << "           j                     ����һ��" << std::endl;
	//std::cout << "           l                     �����ƶ�" << std::endl;
	std::cout << "           [����] G              ��ת��ָ����" << std::endl;
	std::cout << "           i [����] [����]       �������ݵ�ָ����" << std::endl;
	std::cout << "           r [����]              �滻��ǰ�е�����" << std::endl;
	std::cout << "           dd                    ɾ����ǰ��      " << std::endl;
	std::cout << "           dd [����]             ɾ��ָ����      " << std::endl;
	std::cout << "           w                     ����        " << std::endl;
	std::cout << "           q                     �˳�        " << std::endl;
	std::cout << "           wq                    ���沢�˳�      " << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	//std::cout << "" << std::endl;
	//std::cout << "" << std::endl;
		
}
/**
* @brief չʾ����,���ŵ�ǰָ�������ݶ�̬��ʾ�ı�����,ÿ��ֻ��ʾ15��
*/
void Vim::display()
{
	Line* p = head;
	int height = 1;

	if (lineNum <= 15)//��begin��end��ȫ����С��15,ֱ�����ȫ��
	{
		std::cout << "---------------------------------  BEGIN  ---------------------------------" << std::endl;
		while (p != nullptr)
		{
			std::cout << height++ << ": " << p->data << std::endl;
			p = p->next;
		}
		std::cout << "---------------------------------   END   ---------------------------------" << std::endl;
	}
	else//ȫ������15����Ŀǰ�ڵڼ��н������
	{
		if (atNum <= 7)//��begin�� 15��
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
		else if ((lineNum - 7) <= atNum)//��end - 15, end��
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
		else//��atNum-7�� atNum+7��
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
* @brief ��ʾ��������
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
@ brief ��ȡ��ǰ������
*/
int Vim::getLineNum()
{
	return this->atNum;
}
/**
* @brief ���ر������
*/
bool Vim::getSave()
{
	return this->save;
}
/**
* @brief ���ص�ǰ������
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
* @brief �ƶ���ͷ
*/
void Vim::toBegin()
{
	this->atLine = this->head;
	this->atNum = 1;
}
/**
* @brief �ƶ���β
*/
void Vim::toEnd()
{
	this->atLine = this->tail;
	this->atNum = this->lineNum;
}
/**
* @brief �ƶ�����һ��
*/
void Vim::toPrev()
{
	if (this->atLine->prev == nullptr)
	{
		std::cout << "�Ѿ���ͷ��" << std::endl;
		return;
	}
	this->atLine = this->atLine->prev;
	this->atNum--;
}

/**
* @brief �ƶ�����һ��
*/
void Vim::toNext()
{
	if (this->atLine->next == nullptr)
	{
		std::cout << "�Ѿ�������" << std::endl;
		return;
	}
	this->atLine = this->atLine->next;
	this->atNum++;
}
/**
* @brief �ƶ���ָ����
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
* @brief ��������
*/
void Vim::Save()
{
	std::ofstream  ofs; //���������� 
	ofs.open(this->getFilePath(), std::ios::out);//���ļ���outģʽΪ���ǣ� 
	Line* temp = this->head;
	while (temp!=nullptr)
	{
		ofs << temp->data << "\t\n";
		temp = temp->next;
	}
	ofs.close();//�ر��ļ�	
}

/**
* @brief �˳�
*/
void Vim::quit()
{
	this->~Vim();
}

/**
* @brief ����һ��
*/
void Vim::insert(int line, std::string data)
{
	if (line > this->lineNum + 1)
	{
		std::cout << "��������" << std::endl;
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
			if (this->lineNum == 1 && this->atLine->data == "")//�����һ��Ϊ�գ���ô�ڶ��оͲ�Ҫ������
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
			//////////////////////////////////���Ҷ�Ӧλ��///////////////////
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
		////////////////���±�д��ǰ����////////////////////
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
		this->save = false;//�и��±䶯����ʾδ������
	}
}

/**
* @brief ɾ��ָ����
*/
void Vim::remove(int line)
{
	if (line < 0 | line > this->lineNum)
	{
		std::cout << "��������" << std::endl;
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
			////////////////���±�д��ǰ����////////////////////
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
			this->save = false;//�и��±䶯����ʾδ������
			this->lineNum--;
		}


	}
	else if (line == this->lineNum)
	{
		//�ı�����������2�в��ܽ������if�������ܽ���˵��line�Ϸ���Ҳ���Ǵ���2�������пɲ�����β��ɾ��
		temp = tail;
		tail = tail->prev;
		tail->next = nullptr;

		this->BypeNum -= temp->data.size();

		delete temp;
		////////////////���±�д��ǰ����////////////////////
		if (this->atNum == line)
		{
			atLine = tail;
			this->atLine--;
		}

		this->save = false;//�и��±䶯����ʾδ������
		this->lineNum--;
	}
	else//����elseif����,ֻ����head��tail֮��
	{
		//////////////////////////////////���Ҷ�Ӧλ��///////////////////
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

		////////////////���±�д��ǰ����////////////////////
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
		this->save = false;//�и��±䶯����ʾδ������
	}

}

/**
* @brief �滻����
*/
/////////////////
void Vim::replace(std::string data)
{
	atLine->data = data;
	this->save = false;
}
/////////�����Ǹ��²�����ʽ�ĳ���/////////////////////
/**
* @brief ��ɫ����
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
		setColor(FOREGROUND_RED | FOREGROUND_GREEN);//���û�ɫ����
		std::cout << height++;
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
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


