//#include "Util.h"
//std::string homePath = "C:\\Users\\myh\\Desktop\\linux\\home";//�ļ�ϵͳ��Ŀ¼
//std::string CountPath = "C:\\Users\\myh\\Desktop\\linux\\home\\root\\account.txt";//�û���Ϣ�ļ�·��
//char* Blocks = (char*)malloc(1024 * 1024 * sizeof(char));//���̿��СΪ1024��1kB��
//User UFD[10];//�û���Ϣ����Ӷ����ļ�ϵͳ
//bool Map[1024] = { false };//��ķֲ����
//DIR *MFD = new DIR();//һ���ļ�ϵͳ,Ҳ����homeĿ¼�Ĵ�ŵ�
//std::string userPath[10];//�û�Ŀ¼·��
//
////////////////////////////////////////////////////////////////////////����ļ��Ǳ���ԭ��,������vscode/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////��������////////////////////////////////////////////////////////////////////////////////
///**
//* @brief ��root�е�account.txt�ж�ȡ�û���Ϣ
//*/
//int readUsers(std::string path)
//{
//	std::ifstream file(path);
//	std::string line;
//	int i = 0;
//	if (file)
//	{
//		while (getline(file, line))
//		{
//			std::string userMess = line;
//			std::string name = userMess.substr(0, userMess.find_first_of(","));//��ȡ�û���
//			std::string password = userMess.substr(userMess.find_first_of(",") + 1, userMess.length());
//			UFD[i].userName = name;
//			UFD[i].password = password;
//			i++;
//			//���ԣ�std::cout << "�û�����" << name << " ���룺" << password << std::endl;
//		}
//	}
//	else
//	{
//		std::cout << "�û���Ϣ������" << std::endl;
//	}
//	return i;
//}
///**
//* @brief �����ļ���С
//* @param filePath �ļ�·��
//*/
//double calcFileSize(std::string filePath)
//{
//	double sizeKB = 0;
//	std::ifstream file(filePath);//���ļ�
//	if (file.is_open())
//	{
//		file.seekg(0, std::ios::end);//�ƶ�ָ�뵽ĩβ
//		double size = file.tellg();//�����ļ�ָ��λ��
//		if (size == 0)
//		{
//			sizeKB = 0;
//		}
//		else
//		{
//			sizeKB = size / 1024.0;
//		}
//	}
//	return sizeKB;
//}
///**
//* @brief �����ļ��д�С
//*/
//double calcDirSize(std::string dirPath)
//{
//	intptr_t handleFile = 0;//�ļ����
//	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
//	std::string temp;//��ʱ�ַ���
//	double fileSize = 0;
//	if ((handleFile = _findfirst(temp.assign(dirPath).append("\\*").c_str(), &fileInfo)) != -1)//����Ŀ¼�µ������ļ�
//	{
//		do
//		{
//			if ((fileInfo.attrib & _A_SUBDIR))//��Ŀ¼
//			{
//				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//�ų�.��..Ŀ¼
//				{
//					fileSize += calcDirSize(dirPath + "\\" + fileInfo.name);//�ݹ������Ŀ¼��С���������ܵĴ�С��Ŀ¼��С��ռһ����
//				}
//			}
//			else//���ļ�
//			{
//				fileSize += calcFileSize(dirPath + "\\" + fileInfo.name);//�����ļ���С
//			}
//		} while (_findnext(handleFile, &fileInfo) == 0);//������һ��
//		_findclose(handleFile);//�رվ��
//	}
//	return fileSize;
//}
//
///**
//* @brief һ���ļ�ϵͳ��home����ʼ��
//*/
//void initFirstLevelSystem(std::string path)
//{
//	////////////////////////��ʼ���û���Ϣ//////////////////////////
//	int userNum = readUsers(CountPath);
//	//����std::printf("�û�������%d\n", userNum);
//	///////////////////////////��ʼ��һ��Ŀ¼//////////////////////////////////
//	intptr_t handleFile = 0;//�ļ����
//	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
//	std::string temp;//��ʱ�ַ���
//	//////////////////////////////////���²����û�Ŀ¼��Сδ����///////////////////////
//	if ((handleFile = _findfirst(temp.assign(path).append("\\*").c_str(), &fileInfo)) != -1)
//	{
//		do//��MFDĿ¼�µ������ļ����б���(Ҳ�����û�Ŀ¼)
//		{
//			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//�ų�.��..Ŀ¼,�����û�Ŀ¼
//			{
//				for (int i = 0; i < userNum; i++)
//				{
//					if (UFD[i].userName == fileInfo.name)//�ҵ��˶�Ӧ���û�Ŀ¼
//					{
//						int flag = 0;//��׼δ������
//						MFD->fcb[i] = new FCB;//����һ��FCB
//						//Ϊ�û�Ŀ¼�����
//						for (int j = 0; j < 1024; j++)//����������Ϊ1024��
//						{
//							if (Map[j] == false)
//							{
//								Map[j] = true;
//								MFD->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
//								MFD->fcb[i]->pbNUmber = j;//�û�Ŀ¼���
//								MFD->fcb[i]->firstBlock->blockIndexList[MFD->fcb[i]->firstBlock->index++] = j;//���
//
//
//								MFD->fcb[i]->fileName = fileInfo.name;//�û���
//								MFD->fcb[i]->fileType = "dir";
//								MFD->fcb[i]->access[0].read = true;
//								MFD->fcb[i]->access[0].write = true;
//								MFD->fcb[i]->access[1].read = true;
//								MFD->fcb[i]->access[1].write = true;
//								MFD->fcb[i]->access[2].read = true;
//								MFD->fcb[i]->owner = &UFD[i];//ָ������
//								MFD->fcb[i]->groupOwner = &UFD[i];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
//								MFD->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��
//								MFD->fcb[i]->filePath = path + "\\" + fileInfo.name;//�ļ�·��
//
//								//�����û���ӦĿ¼
//								MFD->child[i] = new DIR;//����һ���û�Ŀ¼
//								for (int k = 0; k < 10; k++)
//								{
//									MFD->child[i]->fcb[k] = NULL;
//									MFD->child[i]->child[k] = NULL;
//								}
//								MFD->child[i]->pbNUmber = j;//�û�Ŀ¼���
//								MFD->child[i]->parent = MFD;//������Ϊ�գ�Ϊ�˽������û������ж���׼��
//								MFD->child[i]->fileName = fileInfo.name;//�û���
//								MFD->child[i]->filePath = path + "\\" + fileInfo.name;//�û�Ŀ¼�µ�·��
//								MFD->child[i]->owner = &UFD[i];//ָ���û�
//								MFD->child[i]->groupOwner = &UFD[i];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
//								MFD->child[i]->dirFcb = MFD->fcb[i];
//								//��ʼ���û���ָ���Լ��û�Ŀ¼
//								UFD[i].userDir = MFD->child[i];
//
//								flag = 1;
//								break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
//							}
//						}
//						if (flag == 1)
//						{
//							break;
//						}
//					}
//				}
//			}
//		} while (_findnext(handleFile, &fileInfo) == 0);
//		_findclose(handleFile);//�رվ��
//	}
//
//	////////////////////////////��ʼ��һ��Ŀ¼����//////////////////////////////
//	intptr_t handleFile2 = 0;//�ļ����
//	struct _finddata_t fileInfo2;//�ļ���Ϣ�ṹ��
//	std::string temp2;//��ʱ�ַ���
//
//	if ((handleFile2 = _findfirst(temp2.assign(path).append("\\*").c_str(), &fileInfo2)) != -1)
//	{
//		do
//		{
//			if (strcmp(fileInfo2.name, ".") != 0 && strcmp(fileInfo2.name, "..") != 0)
//			{
//				for (int i = 0; i < userNum; i++)
//				{
//					if (UFD[i].userName == fileInfo2.name)//�ҵ��˶�Ӧ���û�Ŀ¼
//					{
//						MFD->fcb[i]->fileSize = calcDirSize(path + "\\" + fileInfo2.name);//�����û�Ŀ¼��С
//						//std::cout << "�û���" << fileInfo2.name << " ��С��" << MFD->fcb[i]->fileSize << "KB" << std::endl;
//					}
//				}
//			}
//		} while (_findnext(handleFile2, &fileInfo2) == 0);
//		_findclose(handleFile2);//�رվ��
//	}
//}
//
///**
//* @brief �ݹ��ʼ�������ļ�ϵͳ�����ļ�
//* @param userDir �û�Ŀ¼
//* @param index ��Ӧ�û�����
//*/
//void initUserLevelSystemTree(DIR *dir, int index)//�û���Ӧ������
//{
//	//////////////////////////////////////////��ʼ�������ļ���Ϣ//////////////////////////////////
//	intptr_t handHandle = 0;//�ļ����
//	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
//	std::string temp;//��ʱ�ַ���
//	int count = 0;//�ļ�����
//	if ((handHandle = _findfirst(temp.assign(dir->filePath).append("\\*").c_str(), &fileInfo)) != -1)
//	{
//
//		do
//		{
//			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
//			{
//				//std::cout << fileInfo.name << std::endl;
//
//				if (count == 10)
//				{
//					break;
//				}
//				for (int i = 0; i < 10; i++)//���10��
//				{
//					//std::cout << dir->fileName << std::endl;
//
//					if (dir->fcb[i] == NULL)//ָ�û��ļ�Ϊ��
//					{
//						//std::cout << dir->filePath << std::endl;
//						count++;
//						if (fileInfo.attrib & _A_SUBDIR)//��Ŀ¼
//						{
//							dir->fcb[i] = new FCB;//����һ��FCB
//							dir->child[i] = new DIR;//����һ����Ŀ¼
//							for (int k = 0; k < 10; k++)
//							{
//								dir->child[i]->fcb[k] = NULL;
//								dir->child[i]->child[k] = NULL;
//							}
//							dir->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
//							for (int j = 0; j < 1024; j++)//����������Ϊ1024��
//							{
//								if (Map[j] == false)
//								{
//									Map[j] = true;
//									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//���
//									dir->fcb[i]->pbNUmber = j;//Ŀ¼���
//									dir->fcb[i]->access[0].read = true;
//									dir->fcb[i]->access[0].write = true;
//									dir->fcb[i]->access[1].read = true;
//									dir->fcb[i]->access[1].write = true;
//									dir->fcb[i]->access[2].read = true;
//									dir->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��
//
//									//void showDate(struct tm * p);
//									//showDate(localtime(&fileInfo.time_create));//����
//									//std::cout << std::endl;
//
//									dir->fcb[i]->fileName = fileInfo.name;//�ļ���
//									dir->fcb[i]->fileType = "dir";
//									dir->fcb[i]->owner = &UFD[index];//ָ���û�
//									dir->fcb[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
//									dir->fcb[i]->fileSize = calcDirSize(dir->filePath + "\\" + fileInfo.name);//����Ŀ¼��С
//									dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��
//
//									dir->child[i]->fileName = fileInfo.name;//�ļ���
//									dir->child[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��
//									dir->child[i]->owner = &UFD[index];//ָ���û�
//									dir->child[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
//									dir->child[i]->parent = dir;//ָ��Ŀ¼
//									dir->child[i]->pbNUmber = j;//Ŀ¼���
//									dir->child[i]->dirFcb = dir->fcb[i];//ָ��Ŀ¼�Լ���FCB
//
//									break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
//								}
//							}
//
//							//std::cout << dir->fcb[i]->fileName << std::endl;
//							///////////////////////////////////////////�ݹ鴦����Ŀ¼/////////////////////////////
//							initUserLevelSystemTree(dir->child[i], index);
//						}
//						else//���ļ�
//						{
//							dir->fcb[i] = new FCB;//����һ��FCB
//							dir->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
//							//Ȩ��664
//							dir->fcb[i]->access[0].read = true;
//							dir->fcb[i]->access[0].write = true;
//							dir->fcb[i]->access[1].read = true;
//							dir->fcb[i]->access[1].write = true;
//							dir->fcb[i]->access[2].read = true;
//							dir->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��
//							dir->fcb[i]->fileName = fileInfo.name;//�ļ���
//							dir->fcb[i]->fileType = "-";
//							dir->fcb[i]->owner = &UFD[index];//ָ���û�
//							dir->fcb[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
//							double sizeKB = calcFileSize(dir->filePath + "\\" + fileInfo.name);//�����ļ���С
//							dir->fcb[i]->fileSize = sizeKB;
//							dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��
//
//							//�����
//
//							int count = 1;
//							if (sizeKB > count)//�������������Ҫ�����һ����
//							{
//								count++;
//							}
//							int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
//							for (int j = 0; j < 1024; j++)//����������Ϊ1024��
//							{
//								if (Map[j] == false)
//								{
//									Map[j] = true;
//									if (flag == 0)
//									{
//										dir->fcb[i]->pbNUmber = j;//�ļ����
//									}
//									flag = 1;
//									count--;
//									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//���
//									if (count == 0)
//									{
//										break;
//									}
//								}
//							}
//						}
//
//
//						break;
//					}
//				}
//			}
//		} while (_findnext(handHandle, &fileInfo) == 0);
//		_findclose(handHandle);//�رվ��
//	}
//
//
//}
//
///**
//* @brief �����ļ�ϵͳ���û�����ʼ��
//*/
//void initUserLevelSystem(std::string path)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (MFD->child[i] != NULL)//Ҳ���Ǵ����û�Ŀ¼
//		{
//			// std::cout << "�û���" << MFD->child[i]->fileName << " ��С��" << MFD->fcb[i]->fileSize << "KB" << std::endl;
//			// std::cout <<  MFD->child[i]->fileName << std::endl;
//			// std::cout <<  MFD->child[i]->filePath << std::endl;
//			// std::cout << MFD->child[i]->parent->fileName << std::endl;
//			// std::cout << MFD->child[i]->pbNUmber << std::endl;
//			// std::cout << MFD->child[i]->owner->userDir->fileName << std::endl;
//			//std::cout << std::endl;
//
//			//����û������
//			initUserLevelSystemTree(MFD->child[i], i);//�ݹ鴦���û�Ŀ¼�µ�������Ϣ
//		}
//	}
//}
//
///**
//* @brief �˵�
//*/
//void help()
//{
//	std::cout << std::endl;
//	std::cout << "*******************��ӭʹ��linux�ļ�ϵͳ*******************" << std::endl;
//	std::cout << std::endl;
//	std::cout << "        ����                      ˵��                      " << std::endl;
//	std::cout << "        login                     ��¼                      " << std::endl;
//	std::cout << "        su �û���                 �л��û�                  " << std::endl;
//	std::cout << "        cd ..                     �л�����һ��·��              " << std::endl;
//	std::cout << "        cd Ŀ¼                   �л�����һ��·��              " << std::endl;
//	std::cout << "        cd ~                      �л����û���Ŀ¼·��               " << std::endl;
//	std::cout << "        pwd                       ��ʾ��ǰ·��              " << std::endl;
//	std::cout << "        ls                        չʾ�����ļ���              " << std::endl;
//	std::cout << "        ls Ŀ¼��                  ��ʾĿ¼·���µ������ļ���              " << std::endl;
//	std::cout << "        ls -l                     ��ʾ�����ļ���Ϣ              " << std::endl;
//	std::cout << "        ls -l �ļ���               ��ʾ�ļ���Ϣ              " << std::endl;
//	std::cout << "        ls -l Ŀ¼��               չʾĿ¼�������ļ���Ϣ              " << std::endl;
//	std::cout << "        ll                        ��ʾ��ǰ·���������ļ���Ϣ              " << std::endl;
//	std::cout << "        ll �ļ���                  ��ʾ��ǰ�ļ���Ϣ              " << std::endl;
//	std::cout << "        ll Ŀ¼��                  ��ʾĿ¼�������ļ���Ϣ              " << std::endl;
//	std::cout << "        touch �ļ���              �����ļ�                  " << std::endl;
//	std::cout << "        mkdir Ŀ¼��              ����Ŀ¼                  " << std::endl;
//	std::cout << "        find �ļ���               �ݹ�ز����ļ�            " << std::endl;
//	std::cout << "        rm �ļ���                 ɾ���ļ�                  " << std::endl;
//	std::cout << "        rm -r Ŀ¼��              �ݹ��ɾ��Ŀ¼�����ļ�    " << std::endl;
//	std::cout << "        cp �ļ��� ���ļ���         �����ļ�                  " << std::endl;
//	std::cout << "        cp -r Ŀ¼�� ��Ŀ¼��      �ݹ鸴���ļ���                  " << std::endl;
//	///////////////////////////////mv�ļ��д�ʵ��/////////////////////////////////////
//	std::cout << "        mv �ļ��� Ŀ¼��           �ƶ��ļ���Ŀ¼��        " << std::endl;
//	std::cout << "        mv �ļ��� ..              �ƶ��ļ�����һ��Ŀ¼��         " << std::endl;
//	std::cout << "        mv Ŀ¼�� Ŀ¼��           �ƶ�Ŀ¼����һ��Ŀ¼           " << std::endl;
//	std::cout << "        mv Ŀ¼�� ..              �ƶ�Ŀ¼����һ��Ŀ¼��            " << std::endl;
//	std::cout << "        chmod (644) �ļ���/Ŀ¼��  �л��ļ�Ȩ��              " << std::endl;
//
//	std::cout << std::endl;
//	std::cout << "*******************��ӭʹ��linux�ļ�ϵͳ*******************" << std::endl;
//	std::cout << std::endl;
//}
//
///**
//* @brief ��¼
//*/
//int Login()
//{
//	int num = -1;
//	int flag = 0;
//	while (num == -1)
//	{
//		std::cout << "localhost: ";
//		std::string input;
//		getline(std::cin, input);
//		if (input == "login")
//		{
//			std::cout << "user: ";
//			std::string username;
//			getline(std::cin, username);
//
//			std::cout << "password: ";
//			std::string password;
//			getline(std::cin, password);
//
//			for (int i = 0; i < 10; i++)
//			{
//				if (UFD[i].userDir != NULL && UFD[i].userName == username && UFD[i].password == password)
//				{
//					num = i;//�ڼ����û�
//					flag = 1;
//					break;
//				}
//			}
//
//		}
//		else if (input == "help")
//		{
//			help();
//		}
//		else if (input == "clear")
//		{
//			system("cls");
//		}
//		else
//		{
//			std::cout << "operation error�� please login!" << std::endl;
//		}
//	}
//	if (flag == 0)
//	{
//		std::cout << "no such user!" << std::endl;
//	}
//	else
//	{
//		std::cout << "login success!" << std::endl;
//	}
//
//	return num;
//}
//
///**
//* @brief ��ʼ���ļ�ϵͳ
//* @param initFirstLevelSystem ��ʼ��һ���ļ�ϵͳ
//*/
//void init(std::string path)
//{
//	//һ�³�ʼ���������ǶԼ����ļ�ϵͳ�Ƕ�Ӧ·�����е��ļ���Ϣ��ʼ��
//	initFirstLevelSystem(path);
//	initUserLevelSystem(path);
//}
///**
//* @brief ��ɫ����
//*
//*/
//void setColor(WORD color)
//{
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hConsole, color);
//}
///**
//* @brief ��ʾ�ļ���ʼ����
//*/
//void showDate(long long createTime)
//{
//	struct tm * p = localtime(&createTime);
//	printf("%d/%d/%d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
//}
///**
//* @brief ��ʾ�ļ�����
//*/
//void showInfo(FCB* fcb = nullptr)
//{
//	if (fcb->fileType == "dir")
//	{
//		std::cout << "d";
//	}
//	else
//	{
//		std::cout << "-";
//	}
//	std::cout << (fcb->access[0].read ? "r" : "-") << (fcb->access[0].write ? "w" : "-") << (fcb->access[0].execute ? "x" : "-");
//	std::cout << (fcb->access[1].read ? "r" : "-") << (fcb->access[1].write ? "w" : "-") << (fcb->access[1].execute ? "x" : "-");
//	std::cout << (fcb->access[2].read ? "r" : "-") << (fcb->access[2].write ? "w" : "-") << (fcb->access[2].execute ? "x" : "-");
//	std::cout << "  ";
//
//	std::cout << fcb->pbNUmber << "  ";
//	std::cout << fcb->owner->userName << "  ";
//	std::cout << fcb->groupOwner->userName << "  ";
//	if (fcb->fileSize < 512 && fcb->fileSize > 0.1)
//	{
//		std::cout << std::setprecision(3) << fcb->fileSize << "KB" << "\t";
//	}
//	else if (fcb->fileSize > 512)
//	{
//		std::cout << std::setprecision(3) << fcb->fileSize / 1024 << "MB" << "\t";
//	}
//	else if (fcb->fileSize < 0.1)
//	{
//		if (fcb->fileSize == 0)
//		{
//			std::cout << "0B" << "\t\t";
//		}
//		else
//		{
//			std::cout << std::setprecision(2) << fcb->fileSize * 1024 << "B" << "\t\t";
//		}
//	}
//
//	showDate(fcb->createTime);
//	std::cout << "\t";
//	if (fcb->fileType == "dir")
//	{
//		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
//		std::cout << fcb->fileName << "\t";
//		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
//	}
//	else
//	{
//		std::cout << fcb->fileName << "\t";
//	}
//	std::cout << fcb->filePath << std::endl;
//}
//
///**
//* @brief ��ʾ�����ļ���Ϣ
//*/
//void lsAll(DIR* dir)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->fcb[i] != NULL)
//		{
//			showInfo(dir->fcb[i]);
//		}
//	}
//}
///**
//* @brief ��ʾ�ļ���
//*/
//void lsName(DIR* dir)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->fcb[i] != NULL)
//		{
//			if (dir->fcb[i]->fileType == "dir")
//			{
//				setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
//				std::cout << dir->fcb[i]->fileName << "   ";
//				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//			}
//			else
//			{
//				std::cout << dir->fcb[i]->fileName << "   ";
//			}
//		}
//	}
//	std::cout << std::endl;
//}
//
///**
//* @brief �ų�window��ǰ׺��Ϊת��Ϊlinux·����׼��
//*/
//std::string excludePathPrefix(const std::string& fullPath, const std::string& prefixToExclude)
//{
//	// ���ǰ׺�Ƿ����������·����
//	if (fullPath.find(prefixToExclude) == 0)
//	{
//		// �����ų�ǰ׺���ʣ�ಿ��
//		size_t prefixLength = prefixToExclude.length();
//		return fullPath.substr(prefixLength);
//	}
//	else
//	{
//		// ���ǰ׺������������·���У��򷵻�ԭʼ·��
//		return fullPath;
//	}
//}
//
///**
//* @brief ��window·��ת��Ϊ��ǰ�û���linux·��
//*/
//std::string convertWinToLinux(std::string winPath, std::string userName)
//{
//	if (userName == "root")
//	{
//		// Ҫ�ų���ǰ׺
//		std::string prefixToExclude = homePath.substr(0, homePath.find_last_of("\\"));
//		// ���ú����ų�ǰ׺
//		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);
//
//		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//��·���еķ�б���滻Ϊб��
//		return linuxPath;
//
//	}
//	else
//	{
//		// Ҫ�ų���ǰ׺
//		std::string prefixToExclude = homePath + "\\" + userName;
//
//		// ���ú����ų�ǰ׺
//		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);//�����û��ֶ����·��
//
//		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//��·���еķ�б���滻Ϊб��
//		return linuxPath;
//	}
//
//}
///**
//* @brief ����·���¶�ӦĿ¼
//*/
//DIR* findDir(DIR* dir, std::string dirPath)
//{
//	DIR* temp = nullptr;
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->child[i] != NULL)
//		{
//			if (dir->child[i]->filePath == dirPath)
//			{
//				temp = dir->child[i];
//			}
//			else
//			{
//				temp = findDir(dir->child[i], dirPath);
//			}
//		}
//	}
//	return temp;
//}
//
///**
//* @brief �ݹ�ɾ���ļ����µ��ļ�
//*/
//void rm_r(DIR* dir)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->fcb[i] != NULL)
//		{
//			if (dir->fcb[i]->fileType == "dir")
//			{
//				rm_r(dir->child[i]);//�ݹ�ɾ����Ŀ¼���ȵݹ鵽��ɾ���ļ����ٿ���ɾ��Ŀ¼
//
//				//ɾ��Ŀ¼
//				////////////////////////////ɾ��Ŀ¼��¼�Ŀ�////////////////
//				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
//				{
//					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
//				}
//				//ɾ��Ŀ¼
//				if (!RemoveDirectoryA((dir->fcb[i]->filePath).c_str()))//ɾ��Ŀ¼
//				{
//					std::cout << "ɾ��ʧ��!" << std::endl;
//				}
//				//ɾ��Ŀ¼�ļ�¼
//				delete dir->fcb[i];
//				delete dir->child[i];
//				dir->fcb[i] = NULL;
//				dir->child[i] = NULL;
//			}
//			else
//			{
//				//ɾ���ļ�
//				////////////////////////////ɾ���ļ���¼�Ŀ�////////////////
//				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
//				{
//					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
//				}
//				//ɾ����ʵ�ļ�
//				if (!DeleteFileA((dir->fcb[i]->filePath).c_str()))//ɾ���ļ�
//				{
//					std::cout << "ɾ��ʧ��!" << std::endl;
//				}
//				//ɾ���ļ�ϵͳ�ļ�¼
//				delete dir->fcb[i];
//				dir->fcb[i] = NULL;
//			}
//		}
//
//	}
//}
//
//
///**
//* @brief ���Ҷ�Ӧ�ļ�����·����Ϣ
//*/
//void find(DIR* dir, std::string fileName)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->fcb[i] != NULL)
//		{
//			if (dir->fcb[i]->fileName == fileName)
//			{
//				showInfo(dir->fcb[i]);
//			}
//			else if (dir->fcb[i]->fileType == "dir")
//			{
//				find(dir->child[i], fileName);
//			}
//		}
//	}
//}
//
//
///**
//* @brief �������ô�С�����������¼����ļ���С�������¼����Ŀ¼��С
//* @param dir Ŀ¼
//* @param index ��һ��Ŀ¼��Ӧ����Ŀ¼������
//*/
//double resetSize(DIR *dir, int index)
//{
//	dir->parent->fcb[index]->fileSize = 0;//Ŀ¼��Ӧ��fcb�Ĵ�С��������,��ʼ�����С
//	for (int i = 0; i < 10; i++)
//	{
//		if (dir->fcb[i] != NULL)
//		{
//			if (dir->fcb[i]->fileType == "dir")
//			{
//				dir->parent->fcb[index]->fileSize += resetSize(dir->child[i], i);//Ŀ¼��Ӧ��fcb�Ĵ�С��������
//			}
//			else
//			{
//				dir->parent->fcb[index]->fileSize += dir->fcb[i]->fileSize;
//			}
//		}
//	}
//	return dir->parent->fcb[index]->fileSize;
//}
//
///**
//* brief �ݹ鸴��Ŀ¼�������ļ�
//*/
//void cp_r(DIR* src, DIR* dst)
//{
//	for (int i = 0; i < 10; i++)
//	{
//		if (src->fcb[i] != NULL)//���ļ���Ŀ¼
//		{
//			if (src->fcb[i]->fileType == "dir")
//			{
//				///////////////////��ʼ��Ŀ¼��Ϣ////////////////////////
//				dst->child[i] = new DIR;
//				for (int k = 0; k < 10; k++)
//				{
//					dst->child[i]->fcb[k] = NULL;
//					dst->child[i]->child[k] = NULL;
//				}
//				dst->fcb[i] = new FCB;
//				dst->fcb[i]->firstBlock = new PBC;
//
//				dst->fcb[i]->access[0].read = src->fcb[i]->access[0].read;
//				dst->fcb[i]->access[0].write = src->fcb[i]->access[0].write;
//				dst->fcb[i]->access[0].execute = src->fcb[i]->access[0].execute;
//				dst->fcb[i]->access[1].read = src->fcb[i]->access[1].read;
//				dst->fcb[i]->access[1].write = src->fcb[i]->access[1].write;
//				dst->fcb[i]->access[1].execute = src->fcb[i]->access[1].execute;
//				dst->fcb[i]->access[2].read = src->fcb[i]->access[2].read;
//				dst->fcb[i]->access[2].write = src->fcb[i]->access[2].write;
//				dst->fcb[i]->access[2].execute = src->fcb[i]->access[2].execute;
//				dst->fcb[i]->createTime = time(0);
//				dst->fcb[i]->fileName = src->fcb[i]->fileName;
//				dst->fcb[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
//				dst->fcb[i]->fileSize = src->fcb[i]->fileSize;
//				dst->fcb[i]->fileType = "dir";
//				dst->fcb[i]->groupOwner = src->fcb[i]->groupOwner;
//				dst->fcb[i]->owner = src->fcb[i]->owner;
//
//				dst->child[i]->parent = dst;
//				dst->child[i]->dirFcb = dst->fcb[i];
//				dst->child[i]->fileName = src->fcb[i]->fileName;
//				dst->child[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
//				dst->child[i]->groupOwner = src->fcb[i]->groupOwner;
//				dst->child[i]->owner = src->fcb[i]->owner;
//				dst->child[i]->groupOwner = src->fcb[i]->groupOwner;
//
//				///////////////////�ļ�ϵͳ�з���� ////////////////////////
//				for (int j = 0; j < 1024; j++)
//				{
//					if (Map[j] == false)
//					{
//						Map[j] = true;
//						dst->fcb[i]->firstBlock->blockIndexList[dst->fcb[i]->firstBlock->index++] = j;
//						dst->child[i]->pbNUmber = j;
//						dst->fcb[i]->pbNUmber = j;
//						break;
//					}
//				}
//				///////////////////������ʵ�е��ļ���/////////////////////
//				if (!CreateDirectory((dst->fcb[i]->filePath).c_str(), NULL))
//				{
//					std::cout << "����ʧ�ܣ�" << std::endl;
//					continue;
//				}
//
//				///////////////////�ݹ鸴����Ŀ¼////////////////
//				cp_r(src->child[i], dst->child[i]);
//
//			}
//			else
//			{
//				////////////////////////////////////////////////////��ʼ���ļ���Ϣ//////////////////////////////////////////
//
//				///////////////////////////////////////////////////////////////cp�������Ż�������������ʱ����к�����װ�Ż�///////////////////////////////
//				dst->fcb[i] = new FCB;
//				dst->fcb[i]->firstBlock = new PBC;
//
//				dst->fcb[i]->access[0].read = src->fcb[i]->access[0].read;
//				dst->fcb[i]->access[0].write = src->fcb[i]->access[0].write;
//				dst->fcb[i]->access[0].execute = src->fcb[i]->access[0].execute;
//				dst->fcb[i]->access[1].read = src->fcb[i]->access[1].read;
//				dst->fcb[i]->access[1].write = src->fcb[i]->access[1].write;
//				dst->fcb[i]->access[1].execute = src->fcb[i]->access[1].execute;
//				dst->fcb[i]->access[2].read = src->fcb[i]->access[2].read;
//				dst->fcb[i]->access[2].write = src->fcb[i]->access[2].write;
//				dst->fcb[i]->access[2].execute = src->fcb[i]->access[2].execute;
//
//				dst->fcb[i]->createTime = time(0);
//				dst->fcb[i]->fileName = src->fcb[i]->fileName;
//				dst->fcb[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
//				dst->fcb[i]->fileSize = src->fcb[i]->fileSize;
//				dst->fcb[i]->fileType = "-";
//				dst->fcb[i]->groupOwner = src->fcb[i]->groupOwner;
//				dst->fcb[i]->owner = src->fcb[i]->owner;
//				/////////////////////////////////////�ļ�ϵͳ�з����///////////////////////////////////
//				int count = src->fcb[i]->firstBlock->index;
//				int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
//				for (int j = 0; j < 1024; j++)
//				{
//					if (Map[j] == false)
//					{
//						Map[j] = true;
//						if (flag == 0)
//						{
//							dst->fcb[i]->pbNUmber = j;//��һ�����
//							flag = 1;
//						}
//						count--;
//						dst->fcb[i]->firstBlock->blockIndexList[dst->fcb[i]->firstBlock->index++] = j;//���
//					}
//					if (count == 0)
//					{
//						break;
//					}
//				}
//				//////////////////////////////////////////������и����ļ�////////////////////////////////
//				CopyFile((src->fcb[i]->filePath).c_str(), (dst->fcb[i]->filePath).c_str(), false);//�����ļ�
//			}
//		}
//
//	}
//}
///////////////////////////////////////////////////////////////���ܺ���:���Ż�///////////////////////////////////////////////////////////////////////////////////
//
///**
//* @brief �û��������
//* @param userDir �û�Ŀ¼
//* @param ifChangeUser ������л��û��������л��û�ʱ���������룬�����������·����Ϣ��������һ�����,��һ�����·����Ϣsystem("cls")
//* @return �л��û���־
//*/
//int userControl(DIR* userDir, bool ifChangeUser)
//{
//	//·�����û���
//	std::string userName = userDir->owner->userName;//�û���
//	std::string nowPath = userDir->filePath;//��ǰ·��
//	//Ŀ¼ָ��
//	DIR *now = userDir;//��ǰĿ¼
//	int flag = -1;//�л��û���־
//	system("cls");
//	while (1)
//	{
//		if (userDir->fileName == "root")//����л�Ϊroot�û���root@localhost:/home/user1# ·������ɫ
//		{
//			//std::cout<< "root" << "@localhost";
//			setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//������ɫ����
//			std::cout << "root@localhost";
//			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
//			std::cout << ":";
//			setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
//			std::cout << convertWinToLinux(nowPath, userName);
//			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//			std::cout << "# ";
//		}
//		else
//		{
//			//user1@localhost: ~$ ·������ɫ
//			setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//������ɫ����
//			std::cout << userName << "@localhost";
//			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
//			std::cout << ": ";
//			setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
//			std::cout << "~";
//			std::cout << convertWinToLinux(nowPath, userName);
//			setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
//			std::cout << "$ ";
//		}
//		////////////////////////////////////////ѡ��//////////////////8///////////
//		std::string input;
//		getline(std::cin, input);
//		size_t pos = input.find_first_of(' ');
//		std::string first = input.substr(0, pos);
//		std::string second = input.substr(pos + 1);
//		// std::cout << first << " " << second << std::endl;
//		if (first == "help")
//		{
//			help();
//		}
//		else if (first == "exit")
//		{
//			flag = -1;
//			break;
//		}
//		else if (first == "clear")
//		{
//			system("cls");
//		}
//		else if (first == "ls" && second != first)
//		{
//			size_t pos = second.find_first_of(' ');
//			std::string second_1 = second.substr(0, pos);
//			std::string second_2 = second.substr(pos + 1);
//			if (second_1 == second_2 && second_1 != "-l")//��ʾĿ¼���ļ����ڵ��ļ���
//			{
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->child[i] != NULL && now->child[i]->fileName == second_2)
//					{
//						lsName(now->child[i]);
//						break;
//					}
//				}
//			}
//			else if (second_1 == "-l" && second_2 != "-l")
//			{
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
//					{
//						if (now->fcb[i]->fileType == "dir")
//						{
//							lsAll(now->child[i]);
//						}
//						else
//						{
//							showInfo(now->fcb[i]);
//						}
//					}
//				}
//			}
//			else if (second_1 != "-l")
//			{
//
//			}
//			else
//			{
//				lsAll(now);
//			}
//		}
//		else if (first == "ls" && second == "ls")
//		{
//			lsName(now);
//		}
//		else if (first == "ll" && second != first)
//		{
//			for (int i = 0; i < 10; i++)
//			{
//				if (now->fcb[i] != NULL && now->fcb[i]->fileName == second)
//				{
//					if (now->fcb[i]->fileType == "dir")
//					{
//						lsAll(now->child[i]);
//					}
//					else
//					{
//						showInfo(now->fcb[i]);
//					}
//				}
//			}
//		}
//		else if (first == "ll")
//		{
//			lsAll(now);
//		}
//		else if (first == "pwd")
//		{
//			std::cout << nowPath << std::endl;
//		}
//		else if (first == "cd" && !second.empty())
//		{
//			if (second == "..")
//			{
//				if (now->parent->fileName == "home")
//				{
//
//				}
//				else
//				{
//					now = now->parent;
//					nowPath = now->filePath;
//				}
//			}
//			else if (second == "~")
//			{
//				now = userDir;
//				nowPath = userDir->filePath;
//			}
//			else
//			{
//				int find = -1;
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->child[i] != NULL)
//					{
//						if (now->child[i]->fileName == second)
//						{
//							now = now->child[i];
//							nowPath = now->filePath;
//							find = 1;
//							break;
//						}
//					}
//				}
//				if (find == -1)
//				{
//					std::cout << "no such directory!" << std::endl;
//				}
//			}
//		}
//		else if (first == "su" && !second.empty())
//		{
//			for (int i = 0; i < 10; i++)
//			{
//				if (UFD[i].userDir != NULL && UFD[i].userName == second)
//				{
//					std::cout << "����: ";
//					std::string password;
//					std::cin >> password;
//					if (UFD[i].password == password)
//					{
//						flag = i;
//					}
//					else
//					{
//						std::cout << "�������" << std::endl;
//					}
//					break;
//				}
//			}
//			if (flag != -1)
//			{
//				break;
//			}
//		}
//		else if (first == "touch" && !second.empty())
//		{
//			int exis = -1;
//			////////////////////////�ж��ļ��Ƿ����/////////////////////
//			for (int i = 0; i < 10; i++)
//			{
//				if (now->fcb[i] != NULL && now->fcb[i]->fileName == second)
//				{
//					exis = 1;
//					std::cout << "�ļ��Ѵ��ڣ�" << std::endl;
//					break;
//				}
//			}
//			if (exis == 1)//�ļ��Ѵ���
//			{
//				break;
//			}
//
//			////////////////////////�ж��Ƿ��п���λ��///////////////////
//			int empty = -1;
//			for (int i = 0; i < 10; i++)
//			{
//				if (now->fcb[i] == NULL)
//				{
//					empty = i;
//					break;
//				}
//			}
//			if (empty == -1)
//			{
//				std::cout << "�ļ�������" << std::endl;
//			}
//
//			//������д����ļ�
//			std::ofstream File;
//			File.open(now->filePath + "\\" + second);
//			File.close();
//
//
//			/////////////////////////////////////һ���ļ��������ظ����룬�����Ż��ȷ���/////////////////////////////////////
//			//���ļ�ϵͳ����
//			now->fcb[empty] = new FCB;//����һ��FCB
//			now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
//
//			now->fcb[empty]->access[0].read = true;
//			now->fcb[empty]->access[0].write = true;
//			now->fcb[empty]->access[1].read = true;
//			now->fcb[empty]->access[1].write = true;
//			now->fcb[empty]->access[2].read = true;
//			now->fcb[empty]->createTime = time(0);//����ʱ��
//			now->fcb[empty]->fileName = second;//�ļ���
//			now->fcb[empty]->fileType = "-";
//			now->fcb[empty]->owner = userDir->owner;//ָ���û�
//			now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
//			double sizeKB = 0;//�����ļ���С
//			now->fcb[empty]->fileSize = sizeKB;
//			now->fcb[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��
//			//�����
//			int count = 1;
//			if (sizeKB > count)//�������������Ҫ�����һ����
//			{
//				count++;
//			}
//			int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
//			for (int j = 0; j < 1024; j++)//����������Ϊ1024��
//			{
//				if (Map[j] == false)
//				{
//					Map[j] = true;
//					if (flag == 0)
//					{
//						now->fcb[empty]->pbNUmber = j;//�ļ����
//					}
//					flag = 1;
//					count--;
//					now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
//					if (count == 0)
//					{
//						break;
//					}
//				}
//			}
//		}
//		else if (first == "mkdir" && !second.empty())
//		{
//			int exis = -1;
//			////////////////////////�ж�Ŀ¼�Ƿ����/////////////////////
//			for (int i = 0; i < 10; i++)
//			{
//				if (now->child[i] != NULL && now->child[i]->fileName == second)
//				{
//					exis = 1;
//					std::cout << "�ļ��Ѵ��ڣ�" << std::endl;
//					break;
//				}
//			}
//			if (exis == 1)//�ļ��Ѵ���
//			{
//				break;
//			}
//			////////////////////////�ж��Ƿ��п���λ��///////////////
//			int empty = -1;
//			for (int i = 0; i < 10; i++)
//			{
//				if (now->fcb[i] == NULL)
//				{
//					empty = i;
//					break;
//				}
//			}
//			if (empty == -1)
//			{
//				std::cout << "�ļ�������" << std::endl;
//			}
//			//�����ļ���
//			if (!CreateDirectory((now->filePath + "\\" + second).c_str(), NULL))
//			{
//				std::cout << "����ʧ�ܣ�" << std::endl;
//				continue;
//			}
//			//////////////////////////���³�ʼ��Ŀ¼���ظ����룬�����Ż��ȷ���///////////////////////////
//			now->fcb[empty] = new FCB;//����һ��FCB
//			now->child[empty] = new DIR;//����һ����Ŀ¼
//			for (int j = 0; j < 10; j++)
//			{
//				now->child[empty]->fcb[j] = NULL;
//				now->child[empty]->child[j] = NULL;
//			}
//			now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
//			for (int j = 0; j < 1024; j++)//����������Ϊ1024��
//			{
//				if (Map[j] == false)
//				{
//					Map[j] = true;
//					now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
//					now->fcb[empty]->pbNUmber = j;//Ŀ¼���
//					now->fcb[empty]->access[0].read = true;
//					now->fcb[empty]->access[0].write = true;
//					now->fcb[empty]->access[1].read = true;
//					now->fcb[empty]->access[1].write = true;
//					now->fcb[empty]->access[2].read = true;
//					now->fcb[empty]->createTime = time(0);//����ʱ���ʼ��
//					now->fcb[empty]->fileName = second;//�ļ���
//					now->fcb[empty]->fileType = "dir";
//					now->fcb[empty]->owner = userDir->owner;//ָ���û�
//					now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
//					now->fcb[empty]->fileSize = 0;//Ŀ¼��С
//					now->fcb[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��
//
//					now->child[empty]->fileName = second;//�ļ���
//					now->child[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��
//					now->child[empty]->owner = userDir->owner;//ָ���û�
//					now->child[empty]->groupOwner = userDir->owner;//ָ���û�
//					now->child[empty]->parent = now;//ָ��Ŀ¼
//					now->child[empty]->pbNUmber = j;//Ŀ¼���
//					now->child[empty]->dirFcb = now->fcb[empty];//ָ��Ŀ¼FCB
//					break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
//				}
//			}
//
//		}
//		else if (first == "rm" && !second.empty())
//		{
//			size_t pos = second.find_first_of(' ');
//			std::string second_1 = second.substr(0, pos);
//			std::string second_2 = second.substr(pos + 1);
//			//std::cout << "first:" << first << " second:" << second_1 << " third:" << second_2 << std::endl;
//			if (second_1 == second_2)//�����rm <filename>
//			{
//				//std::cout << "file" << std::endl;
//				////////////////////////�ж��ļ��Ƿ����//////////////////////////////////////////////////////
//				int exist = -1;
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
//					{
//						exist = i;
//						break;
//					}
//				}
//				if (exist == -1)//�ļ�������
//				{
//					continue;
//				}
//
//				////////////////////////ɾ���ļ���/////////////////////////////////////////////////////////////
//				if (!DeleteFileA((now->fcb[exist]->filePath).c_str()))//ɾ���ļ�
//				{
//					std::cout << "ɾ��ʧ��!" << std::endl;
//				}
//				//��ȥ�ļ���ռ�õĿ�
//				for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
//				{
//					Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//������Ϊδ����
//				}
//				////////////////////////ɾ���ļ�ϵͳ��///////////////
//				delete (now->fcb[exist]);//�ͷ�FCB
//				now->fcb[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL
//				/////////////////////////////////////////////�µĵݹ����¼����С////////////////////////////////////////////////////////////
//				for (int i = 0; i < 10; i++)
//				{
//					if (userDir->child[i] != NULL)
//					{
//						resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//					}
//				}
//			}
//			else if (second_1 == "-r" && second_2 != "-r")
//			{
//				//std::cout << "-r file" << std::endl;
//				////////////////////////�ж�Ŀ¼�Ƿ����/////////////////
//				int exist = -1;
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->child[i] != NULL && now->child[i]->fileName == second_2)
//					{
//						exist = i;
//						break;
//					}
//				}
//				if (exist == -1)//Ŀ¼������
//				{
//					continue;
//				}
//				// ////////////////////////����ɾ��Ŀ¼����һ��Ŀ¼�Ĵ�С:���ݹ��Ż�//////////
//				// if(now->fileName != userDir->fileName)//���û���Ŀ¼���¼����С
//				// {
//				//     //�ݹ��ȥ�ļ��д�С�����¼����ϲ��ļ����ܴ�С
//				//     for(int i = 0; i < 10; i++)
//				//     {
//				//         if(now->parent->child[i] != NULL && now->parent->child[i]->fileName == now->fileName)
//				//         {
//				//             now->parent->fcb[i]->fileSize -= now->fcb[exist]->fileSize;//�����ϲ��ļ��д�С,��ȥ��ǰ�ļ��д�С
//				//             break;
//				//         }
//				//     }
//				// }
//
//				//��ȥ�ļ�����ռ�õĿ�
//				for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
//				{
//					Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//������Ϊδ����
//				}
//				///////////////////�ݹ�ɾ��Ŀ¼����Ŀ¼���µ��ļ�///////////////
//				rm_r(now->child[exist]);
//				////////////////////////ɾ��Ŀ¼/////////////////
//				if (!RemoveDirectoryA((now->fcb[exist]->filePath).c_str()))//ɾ��Ŀ¼
//				{
//					std::cout << "ɾ��ʧ��!" << std::endl;
//				}
//
//				////////////////////////ɾ���������Ŀ¼////////////  
//				delete now->fcb[exist];//�ͷŵ�ǰĿ¼��FCB
//				delete now->child[exist];//�ͷŵ�ǰĿ¼��Ŀ¼
//				now->fcb[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL
//				now->child[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL
//
//				///////////////////////////////////////////�µĵݹ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
//				for (int i = 0; i < 10; i++)
//				{
//					if (userDir->child[i] != NULL)
//					{
//						resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//					}
//				}
//			}
//		}
//		else if (first == "find" && !second.empty())
//		{
//			find(userDir, second);
//		}
//		else if (first == "cp" && !second.empty())
//		{
//			size_t pos = second.find_first_of(' ');
//			std::string second_1 = second.substr(0, pos);
//			std::string second_2 = second.substr(pos + 1);
//			//std::cout << "first:" << first << " second:" << second_1 << " third:" << second_2 << std::endl;
//			if (second_1 != second_2)
//			{
//				if (second_1 != "-r")
//				{
//					/////////////////////////�ж��ļ��Ƿ����////////////////////////////
//					int cpSource = -1;
//					for (int i = 0; i < 10; i++)
//					{
//						if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_1)
//						{
//							cpSource = i;
//							break;
//						}
//					}
//					if (cpSource == -1)
//					{
//						std::cout << "Դ�ļ������ڣ�" << std::endl;
//						continue;
//					}
//
//					////////////////////////�ҵ��Ƿ��п���λ��//////////////////////
//					int empty = -1;
//					for (int i = 0; i < 10; i++)
//					{
//						if (now->fcb[i] == NULL)
//						{
//							empty = i;
//							break;
//						}
//					}
//					if (empty == -1)
//					{
//						std::cout << "�ļ�������" << std::endl;
//						continue;
//					}
//
//					/////////////////////////////////////��ʼ�������ļ�����Ϣ//////////////////////////////
//					now->fcb[empty] = new FCB;//����һ��FCB
//					now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
//					///////////////////////////////////////���´�����Ż�///////////////////////////////////////
//					now->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
//					now->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
//					now->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;
//
//					now->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
//					now->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
//					now->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;
//
//					now->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
//					now->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
//					now->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;
//
//					now->fcb[empty]->createTime = time(0);//����ʱ��
//					now->fcb[empty]->fileName = second_2;//�ļ���
//					now->fcb[empty]->filePath = now->filePath + "\\" + second_2;//�ļ�·��
//					now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//�ļ���С
//					now->fcb[empty]->fileType = now->fcb[cpSource]->fileType;//�ļ�����
//					now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
//					now->fcb[empty]->owner = userDir->owner;//ָ���û�
//
//					/////////////////////////////////�����///////////////////////////////////////
//					int count = now->fcb[cpSource]->firstBlock->index;
//					int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
//					for (int j = 0; j < 1024; j++)
//					{
//						if (Map[j] == false)
//						{
//							Map[j] = true;
//							if (flag == 0)
//							{
//								now->fcb[empty]->pbNUmber = j;//��һ�����
//								flag = 1;
//							}
//							count--;
//							now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
//						}
//						if (count == 0)
//						{
//							break;
//						}
//					}
//					/////////////////////////////////�����ļ�////////////////////////////////////////
//					CopyFile((now->fcb[cpSource]->filePath).c_str(), (now->fcb[empty]->filePath).c_str(), false);//�����ļ�
//
//					///////////////////////////////////////////�µ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
//					for (int i = 0; i < 10; i++)
//					{
//						if (userDir->child[i] != NULL)
//						{
//							resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//						}
//					}
//				}
//				else
//				{
//					size_t pos_2 = second_2.find_first_of(' ');
//					std::string second_2_1 = second_2.substr(0, pos_2);
//					std::string second_2_2 = second_2.substr(pos_2 + 1);
//					if (second_2_1 != second_2_2)
//					{
//						/////////////////////////�ж�Ŀ¼�Ƿ����////////////////////////////
//						int cpSource = -1;
//						for (int i = 0; i < 10; i++)
//						{
//							if (now->child[i] != NULL && now->child[i]->fileName == second_2_1)
//							{
//								cpSource = i;
//								break;
//							}
//						}
//						if (cpSource == -1)
//						{
//							std::cout << "ԴĿ¼�����ڣ�" << std::endl;
//							continue;
//						}
//
//						////////////////////////�ҵ��Ƿ��п���λ��/////////////////////////
//						int empty = -1;
//						for (int i = 0; i < 10; i++)
//						{
//							if (now->fcb[i] == NULL)
//							{
//								empty = i;
//								break;
//							}
//						}
//						if (empty == -1)
//						{
//							std::cout << "�ļ�������" << std::endl;
//							continue;
//						}
//						////////////////////////////////�����ļ���:Ŀ��second_2_1 ����second_2_2////////////////////////////////////////
//						now->fcb[empty] = new FCB;//����һ��FCB
//						now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
//						now->child[empty] = new DIR;//����һ����Ŀ¼
//						for (int k = 0; k < 10; k++)
//						{
//							now->child[empty]->fcb[k] = NULL;
//							now->child[empty]->child[k] = NULL;
//						}
//						//////////////////////////////////////////////////////////
//						now->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
//						now->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
//						now->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;
//						now->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
//						now->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
//						now->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;
//						now->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
//						now->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
//						now->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;
//
//						now->fcb[empty]->createTime = time(0);//����ʱ��
//						now->fcb[empty]->fileName = second_2_2;//�ļ���
//						now->fcb[empty]->filePath = now->filePath + "\\" + second_2_2;//�ļ�·��
//						now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//�ļ���С
//						now->fcb[empty]->fileType = "dir";//�ļ�����
//						now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
//						now->fcb[empty]->owner = userDir->owner;//ָ���û�
//
//						now->child[empty]->fileName = second_2_2;//�ļ���
//						now->child[empty]->filePath = now->filePath + "\\" + second_2_2;//�ļ�·��
//						now->child[empty]->groupOwner = userDir->owner;//ָ���û�
//						now->child[empty]->owner = userDir->owner;//ָ���û�
//						now->child[empty]->parent = now;//ָ��Ŀ¼
//						now->child[empty]->dirFcb = now->fcb[empty];//ָ��Ŀ¼FCB
//						//////////////////////////////////////��Ŀ¼�����////////////////////////////////
//						for (int j = 0; j < 1024; j++)
//						{
//							if (Map[j] == false)
//							{
//								Map[j] = true;
//								now->fcb[empty]->pbNUmber = j;//��һ�����
//								now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
//								now->child[empty]->pbNUmber = j;//��һ�����
//								break;
//							}
//						}
//						///////////////////////////////////�½�Ŀ¼/////////////////////////////////////////
//						if (!CreateDirectory((now->fcb[empty]->filePath).c_str(), NULL))
//						{
//							std::cout << "Ŀ¼����ʧ�ܣ�" << std::endl;
//							continue;
//						}
//						//////////////////////////////////////��ʼ�ݹ鸴��Ŀ¼��������Ϣ���ļ�//////////////////////////////
//						cp_r(now->child[cpSource], now->child[empty]);
//					}
//					///////////////////////////////////////////�µ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
//					for (int i = 0; i < 10; i++)
//					{
//						if (userDir->child[i] != NULL)
//						{
//							resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//						}
//					}
//				}
//
//
//			}
//		}
//		else if (first == "mv" && !second.empty())
//		{
//			size_t pos = second.find_first_of(' ');
//			std::string second_1 = second.substr(0, pos);
//			std::string second_2 = second.substr(pos + 1);
//			if (second_1 != second_2)
//			{
//				int mvSource = -1;
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_1)
//					{
//						mvSource = i;
//						break;
//					}
//				}
//				if (mvSource == -1)
//				{
//					std::cout << "Դ�ļ������ڣ�" << std::endl;
//					continue;
//				}
//				if (second_2 == "..")//�ƶ����ϼ�Ŀ¼
//				{
//					if (now->parent->fileName != "home")
//					{
//						/////////////////////////////�ж��ļ������Ƿ��ÿռ���Դ��/////////////////////////////
//						int empty = -1;
//						for (int i = 0; i < 10; i++)
//						{
//							if (now->parent->fcb[i] == NULL)
//							{
//								empty = i;
//								break;
//							}
//						}
//						if (empty == -1)
//						{
//							continue;
//						}
//						if (now->fcb[mvSource]->fileType == "dir")//������ļ���
//						{
//
//						}
//						else//������ļ�
//						{
//							////////////////////////////////////////////��������ƶ��ļ�////////////////////////////////////////
//							MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->parent->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//�ƶ���
//
//							now->parent->fcb[empty] = now->fcb[mvSource];//���ļ��ƶ���Ŀ���ļ���
//							now->fcb[mvSource] = NULL;//��Դ�ļ�����ΪNULL
//							/////////////////////////////////////////����Ŀ¼·����Ϣ/////////////////////////////////
//							now->parent->fcb[empty]->filePath = now->parent->filePath + "\\" + now->parent->fcb[empty]->fileName;//�����ļ�·��
//						}
//						///////////////////////////////////////////�µ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
//						for (int i = 0; i < 10; i++)
//						{
//							if (userDir->child[i] != NULL)
//							{
//								resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//							}
//						}
//
//					}
//
//				}
//				else
//				{
//					////////////////////////�ж��ļ����Ƿ����////////////////////////////
//					int exist = -1;
//					for (int i = 0; i < 10; i++)
//					{
//						if (now->child[i] != NULL && now->child[i]->fileName == second_2)
//						{
//							exist = i;
//							break;
//						}
//					}
//					if (exist == -1)
//					{
//						continue;
//					}
//
//					/////////////////////////////�ж��ļ������Ƿ��ÿռ���Դ��/////////////////////////////
//					int empty = -1;
//					for (int i = 0; i < 10; i++)
//					{
//						if (now->child[exist]->fcb[i] == NULL)
//						{
//							empty = i;
//							break;
//						}
//					}
//					if (empty == -1)
//					{
//						continue;
//					}
//					//////////////////////////////�����ļ���ɾ���ļ��ﵽĿ��//////////////////////
//					if (now->fcb[mvSource]->fileType == "dir")//������ļ���
//					{
//						/////////////////////////////////////////��������ƶ�Ŀ¼//////////////////////////////////////
//
//
//						//////////////////////////////////////�ƶ��ļ��нṹ����һ���ļ���//////////////////////////////////////////
//						now->child[exist]->fcb[empty] = now->fcb[mvSource];//���ļ��ƶ���Ŀ���ļ���
//						now->fcb[mvSource] = NULL;//��Դ�ļ�����ΪNULL
//						now->child[exist]->child[empty] = now->child[mvSource];//����Ŀ¼�ƶ���Ŀ���ļ���
//						now->child[mvSource] = NULL;//��ԴĿ¼����ΪNULL
//
//						/////////////////////////////////////�����ƶ����ļ���Ŀ¼�Ĵ�С/////////////////////////
//						//now->fcb[exist]->fileSize += now->child[exist]->fcb[empty]->fileSize;//����Ŀ���ļ��д�С
//
//						////////////////////////////////////////����Ŀ¼·����Ϣ////////////////////////////////
//						now->child[exist]->fcb[empty]->filePath = now->child[exist]->filePath + "\\" + now->child[exist]->fcb[empty]->fileName;//�����ļ�·��  
//					}
//					else//������ļ�
//					{
//						////////////////////////////////////////////��������ƶ��ļ�////////////////////////////////////////
//						MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->child[exist]->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//�ƶ���
//
//						now->child[exist]->fcb[empty] = now->fcb[mvSource];//���ļ��ƶ���Ŀ���ļ���
//						now->fcb[mvSource] = NULL;//��Դ�ļ�����ΪNULL
//						/////////////////////////////////////////����Ŀ¼·����Ϣ/////////////////////////////////
//						now->child[exist]->fcb[empty]->filePath = now->child[exist]->filePath + "\\" + now->child[exist]->fcb[empty]->fileName;//�����ļ�·��
//
//
//					}
//					///////////////////////////////////////////�µĵݹ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
//					for (int i = 0; i < 10; i++)
//					{
//						if (userDir->child[i] != NULL)
//						{
//							resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
//						}
//					}
//				}
//
//			}
//		}
//		else if (first == "chmod" && !second.empty())//�޸�Ȩ��
//		{
//			size_t pos = second.find_first_of(' ');
//			std::string second_1 = second.substr(0, pos);
//			std::string second_2 = second.substr(pos + 1);
//			//std::cout <<second_1.length() << " " << second_2 << std::endl;
//
//
//			if (second_1 != second_2)
//			{
//				if (second_1.length() != 3)
//				{
//					continue;
//				}
//				//std::cout << second_1[0] << " " << second_1[1] << " " << second_1[2] << std::endl;
//				bool ifSuit = true;
//				for (int i = 0; i < 3; i++)
//				{
//					//std::cout << second_1[i] << " ";
//					if (second_1[i] > '7' || second_1[i] < '0')
//					{
//						ifSuit = false;
//						break;;
//					}
//				}
//				if (!ifSuit)
//				{
//					continue;
//				}
//
//				//auto num = std::stoi(second_1);
//				//std::cout << num << " " << second_2 << std::endl;
//				int exist = -1;
//				for (int i = 0; i < 10; i++)
//				{
//					if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
//					{
//						exist = i;
//						break;
//					}
//				}
//				if (exist == -1)
//				{
//					continue;
//				}
//				//�޸�Ȩ��
//				for (int i = 0; i < 3; i++)
//				{
//					if (second_1[i] == '0')
//					{
//						now->fcb[exist]->access[i].read = false;
//						now->fcb[exist]->access[i].write = false;
//						now->fcb[exist]->access[i].execute = false;
//					}
//					else if (second_1[i] == '1')
//					{
//						now->fcb[exist]->access[i].read = false;
//						now->fcb[exist]->access[i].write = false;
//						now->fcb[exist]->access[i].execute = true;
//
//					}
//					else if (second_1[i] == '2')
//					{
//						now->fcb[exist]->access[i].read = false;
//						now->fcb[exist]->access[i].write = true;
//						now->fcb[exist]->access[i].execute = false;
//					}
//					else if (second_1[i] == '3')
//					{
//						now->fcb[exist]->access[i].read = false;
//						now->fcb[exist]->access[i].write = true;
//						now->fcb[exist]->access[i].execute = true;
//					}
//					else if (second_1[i] == '4')
//					{
//						now->fcb[exist]->access[i].read = true;
//						now->fcb[exist]->access[i].write = false;
//						now->fcb[exist]->access[i].execute = false;
//					}
//					else if (second_1[i] == '5')
//					{
//						now->fcb[exist]->access[i].read = true;
//						now->fcb[exist]->access[i].write = false;
//						now->fcb[exist]->access[i].execute = true;
//					}
//					else if (second_1[i] == '6')
//					{
//						now->fcb[exist]->access[i].read = true;
//						now->fcb[exist]->access[i].write = true;
//						now->fcb[exist]->access[i].execute = false;
//					}
//					else if (second_1[i] == '7')
//					{
//						now->fcb[exist]->access[i].read = true;
//						now->fcb[exist]->access[i].write = true;
//						now->fcb[exist]->access[i].execute = true;
//					}
//				}
//			}
//
//		}
//
//
//
//		if (ifChangeUser)
//		{
//			system("cls");
//			ifChangeUser = false;
//		}
//
//	}
//
//
//
//	return flag;//�л��û���־
//}
//
///**
//* @brief �������
//*/
//void operation()
//{
//	std::cout << "��ʼ����..." << std::endl;
//	MFD->fileName = "home";//���ø�Ŀ¼����,������ǵ�����
//	for (int k = 0; k < 10; k++)
//	{
//		MFD->fcb[k] = NULL;
//		MFD->child[k] = NULL;
//	}
//	init(homePath);//��ʼ���ļ�ϵͳ
//	system("cls");
//	int login = -1;
//	while (login == -1)
//	{
//		login = Login();
//		if (login != -1)
//		{
//			break;
//		}
//	}
//	int flag = userControl(MFD->child[login], false);
//	while (flag != -1)//�л��û�,ֱ���˳�
//	{
//		flag = userControl(UFD[flag].userDir, true);
//	}
//}
//int main()
//{
//	operation();
//	return 0;
//}