#include "Util.h"
////////////////////////////////////////////////////////��������///////////////////////////////////////////////////////////
/**
* @brief ��root�е�account.txt�ж�ȡ�û���Ϣ
*/
int readUsers(std::string path, User UFD[])
{
	std::ifstream file(path);
	std::string line;
	int i = 0;
	if (file)
	{
		while (getline(file, line))
		{
			std::string userMess = line;
			std::string name = userMess.substr(0, userMess.find_first_of(":"));//��ȡ�û���
			std::string password = userMess.substr(userMess.find_first_of(":") + 1, userMess.length());
			//std::cout << name << password << std::endl;
			UFD[i].userName = name;
			UFD[i].password = password;
			i++;
			//���ԣ�std::cout << "�û�����" << name << " ���룺" << password << std::endl;
		}
	}
	else
	{
		std::cout << "�û���Ϣ������" << std::endl;
	}
	//system("pause");
	return i;
}

/**
* @brief �����ļ���С
* @param filePath �ļ�·��
*/
double calcFileSize(std::string filePath)
{
	double sizeKB = 0;
	std::ifstream file(filePath);//���ļ�
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);//�ƶ�ָ�뵽ĩβ
		double size = file.tellg();//�����ļ�ָ��λ��
		if (size == 0)
		{
			sizeKB = 0;
		}
		else
		{
			sizeKB = size / 1024.0;
		}
	}
	return sizeKB;
}
/**
* @brief �����ļ��д�С
*/
double calcDirSize(std::string dirPath)
{
	intptr_t handleFile = 0;//�ļ����
	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
	std::string temp;//��ʱ�ַ���
	double fileSize = 0;
	if ((handleFile = _findfirst(temp.assign(dirPath).append("\\*").c_str(), &fileInfo)) != -1)//����Ŀ¼�µ������ļ�
	{
		do
		{
			if ((fileInfo.attrib & _A_SUBDIR))//��Ŀ¼
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//�ų�.��..Ŀ¼
				{
					fileSize += calcDirSize(dirPath + "\\" + fileInfo.name);//�ݹ������Ŀ¼��С���������ܵĴ�С��Ŀ¼��С��ռһ����
				}
			}
			else//���ļ�
			{
				fileSize += calcFileSize(dirPath + "\\" + fileInfo.name);//�����ļ���С
			}
		} while (_findnext(handleFile, &fileInfo) == 0);//������һ��
		_findclose(handleFile);//�رվ��
	}
	return fileSize;
}

/**
* @brief һ���ļ�ϵͳ��home����ʼ��
*/
void initFirstLevelSystem(std::string path, User UFD[], DIR* MFD, std::string passwdPath, bool Map[])
{
	////////////////////////��ʼ���û���Ϣ//////////////////////////
	int userNum = readUsers(passwdPath, UFD);
	//����std::printf("�û�������%d\n", userNum);
	///////////////////////////��ʼ��һ��Ŀ¼//////////////////////////////////
	intptr_t handleFile = 0;//�ļ����
	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
	std::string temp;//��ʱ�ַ���
	//////////////////////////////////���²����û�Ŀ¼��Сδ����///////////////////////
	if ((handleFile = _findfirst(temp.assign(path).append("\\*").c_str(), &fileInfo)) != -1)
	{
		do//��MFDĿ¼�µ������ļ����б���(Ҳ�����û�Ŀ¼)
		{
			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//�ų�.��..Ŀ¼,�����û�Ŀ¼
			{
				for (int i = 0; i < userNum; i++)
				{
					if (UFD[i].userName == fileInfo.name)//�ҵ��˶�Ӧ���û�Ŀ¼
					{
						//std::cout << fileInfo.name << std::endl;

						int flag = 0;//��׼δ������
						MFD->fcb[i] = new FCB;//����һ��FCB
						//Ϊ�û�Ŀ¼�����
						for (int j = 0; j < 1024; j++)//����������Ϊ1024��
						{
							if (Map[j] == false)
							{
								Map[j] = true;
								MFD->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
								MFD->fcb[i]->pbNUmber = j;//�û�Ŀ¼���
								MFD->fcb[i]->firstBlock->blockIndexList[MFD->fcb[i]->firstBlock->index++] = j;//���


								MFD->fcb[i]->fileName = fileInfo.name;//�û���
								MFD->fcb[i]->fileType = "dir";
								MFD->fcb[i]->access[0].read = true;
								MFD->fcb[i]->access[0].write = true;
								MFD->fcb[i]->access[1].read = true;
								MFD->fcb[i]->access[1].write = true;
								MFD->fcb[i]->access[2].read = true;
								MFD->fcb[i]->owner = &UFD[i];//ָ������
								MFD->fcb[i]->groupOwner = &UFD[i];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
								MFD->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��
								MFD->fcb[i]->filePath = path + "\\" + fileInfo.name;//�ļ�·��

								//�����û���ӦĿ¼
								MFD->child[i] = new DIR;//����һ���û�Ŀ¼
								for (int k = 0; k < 10; k++)
								{
									MFD->child[i]->fcb[k] = NULL;
									MFD->child[i]->child[k] = NULL;
								}
								MFD->child[i]->pbNUmber = j;//�û�Ŀ¼���
								MFD->child[i]->parent = MFD;//������Ϊ�գ�Ϊ�˽������û������ж���׼��
								MFD->child[i]->fileName = fileInfo.name;//�û���
								MFD->child[i]->filePath = path + "\\" + fileInfo.name;//�û�Ŀ¼�µ�·��
								MFD->child[i]->owner = &UFD[i];//ָ���û�
								MFD->child[i]->groupOwner = &UFD[i];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
								MFD->child[i]->dirFcb = MFD->fcb[i];
								//��ʼ���û���ָ���Լ��û�Ŀ¼
								UFD[i].userDir = MFD->child[i];

								flag = 1;
								break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
							}
						}
						if (flag == 1)
						{
							break;
						}
					}
				}
			}
		} while (_findnext(handleFile, &fileInfo) == 0);
		_findclose(handleFile);//�رվ��
	}
	//system("pause");

	////////////////////////////��ʼ��һ��Ŀ¼����//////////////////////////////
	intptr_t handleFile2 = 0;//�ļ����
	struct _finddata_t fileInfo2;//�ļ���Ϣ�ṹ��
	std::string temp2;//��ʱ�ַ���

	if ((handleFile2 = _findfirst(temp2.assign(path).append("\\*").c_str(), &fileInfo2)) != -1)
	{
		do
		{
			if (strcmp(fileInfo2.name, ".") != 0 && strcmp(fileInfo2.name, "..") != 0)
			{
				for (int i = 0; i < userNum; i++)
				{
					if (UFD[i].userName == fileInfo2.name)//�ҵ��˶�Ӧ���û�Ŀ¼
					{
						MFD->fcb[i]->fileSize = calcDirSize(path + "\\" + fileInfo2.name);//�����û�Ŀ¼��С
						//std::cout << "�û���" << fileInfo2.name << " ��С��" << MFD->fcb[i]->fileSize << "KB" << std::endl;
					}
				}
			}
		} while (_findnext(handleFile2, &fileInfo2) == 0);
		_findclose(handleFile2);//�رվ��
	}
}

/**
* @brief �ݹ��ʼ�������ļ�ϵͳ�����ļ�
* @param userDir �û�Ŀ¼
* @param index ��Ӧ�û�����
*/
void initUserLevelSystemTree(DIR *dir, int index, User UFD[], bool Map[])//�û���Ӧ������
{
	//////////////////////////////////////////��ʼ�������ļ���Ϣ//////////////////////////////////
	intptr_t handHandle = 0;//�ļ����
	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
	std::string temp;//��ʱ�ַ���
	int count = 0;//�ļ�����
	if ((handHandle = _findfirst(temp.assign(dir->filePath).append("\\*").c_str(), &fileInfo)) != -1)
	{

		do
		{
			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
			{
				//std::cout << fileInfo.name << std::endl;

				if (count == 10)
				{
					break;
				}
				for (int i = 0; i < 10; i++)//���10��
				{
					//std::cout << dir->fileName << std::endl;

					if (dir->fcb[i] == NULL)//ָ�û��ļ�Ϊ��
					{
						//std::cout << dir->filePath << std::endl;
						count++;
						if (fileInfo.attrib & _A_SUBDIR)//��Ŀ¼
						{
							dir->fcb[i] = new FCB;//����һ��FCB
							dir->child[i] = new DIR;//����һ����Ŀ¼
							for (int k = 0; k < 10; k++)
							{
								dir->child[i]->fcb[k] = NULL;
								dir->child[i]->child[k] = NULL;
							}
							dir->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
							for (int j = 0; j < 1024; j++)//����������Ϊ1024��
							{
								if (Map[j] == false)
								{
									Map[j] = true;
									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//���
									dir->fcb[i]->pbNUmber = j;//Ŀ¼���
									dir->fcb[i]->access[0].read = true;
									dir->fcb[i]->access[0].write = true;
									dir->fcb[i]->access[1].read = true;
									dir->fcb[i]->access[1].write = true;
									dir->fcb[i]->access[2].read = true;
									dir->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��

									//void showDate(struct tm * p);
									//showDate(localtime(&fileInfo.time_create));//����
									//std::cout << std::endl;

									dir->fcb[i]->fileName = fileInfo.name;//�ļ���
									dir->fcb[i]->fileType = "dir";
									dir->fcb[i]->owner = &UFD[index];//ָ���û�
									dir->fcb[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
									dir->fcb[i]->fileSize = calcDirSize(dir->filePath + "\\" + fileInfo.name);//����Ŀ¼��С
									dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��

									dir->child[i]->fileName = fileInfo.name;//�ļ���
									dir->child[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��
									dir->child[i]->owner = &UFD[index];//ָ���û�
									dir->child[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
									dir->child[i]->parent = dir;//ָ��Ŀ¼
									dir->child[i]->pbNUmber = j;//Ŀ¼���
									dir->child[i]->dirFcb = dir->fcb[i];//ָ��Ŀ¼�Լ���FCB

									break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
								}
							}

							//std::cout << dir->fcb[i]->fileName << std::endl;
							///////////////////////////////////////////�ݹ鴦����Ŀ¼/////////////////////////////
							initUserLevelSystemTree(dir->child[i], index, UFD, Map);
						}
						else//���ļ�
						{
							dir->fcb[i] = new FCB;//����һ��FCB
							dir->fcb[i]->firstBlock = new PBC;//�������ƽṹ��
							//Ȩ��664
							dir->fcb[i]->access[0].read = true;
							dir->fcb[i]->access[0].write = true;
							dir->fcb[i]->access[1].read = true;
							dir->fcb[i]->access[1].write = true;
							dir->fcb[i]->access[2].read = true;
							dir->fcb[i]->createTime = fileInfo.time_create;//����ʱ���ʼ��
							dir->fcb[i]->fileName = fileInfo.name;//�ļ���
							dir->fcb[i]->fileType = "-";
							dir->fcb[i]->owner = &UFD[index];//ָ���û�
							dir->fcb[i]->groupOwner = &UFD[index];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
							double sizeKB = calcFileSize(dir->filePath + "\\" + fileInfo.name);//�����ļ���С
							dir->fcb[i]->fileSize = sizeKB;
							dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//�ļ�·��

							//�����

							int count = 1;
							if (sizeKB > count)//�������������Ҫ�����һ����
							{
								count++;
							}
							int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
							for (int j = 0; j < 1024; j++)//����������Ϊ1024��
							{
								if (Map[j] == false)
								{
									Map[j] = true;
									if (flag == 0)
									{
										dir->fcb[i]->pbNUmber = j;//�ļ����
									}
									flag = 1;
									count--;
									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//���
									if (count == 0)
									{
										break;
									}
								}
							}
						}


						break;
					}
				}
			}
		} while (_findnext(handHandle, &fileInfo) == 0);
		_findclose(handHandle);//�رվ��
	}


}
/**
* @brief �˵�
*/
void help()
{
	std::cout << std::endl;
	std::cout << "*******************��ӭʹ��linux�ļ�ϵͳ*******************" << std::endl;
	std::cout << std::endl;
	std::cout << "        ����                      ˵��                      " << std::endl;
	std::cout << "        login                     ��¼                      " << std::endl;
	std::cout << "        su �û���                 �л��û�                  " << std::endl;
	std::cout << "        whoami/id                 ��ǰ�û���                  " << std::endl;
	std::cout << "        cd ..                     �л�����һ��·��              " << std::endl;
	std::cout << "        cd Ŀ¼                   �л�����һ��·��              " << std::endl;
	std::cout << "        cd ~                      �л����û���Ŀ¼·��               " << std::endl;
	std::cout << "        pwd                       ��ʾ��ǰ·��              " << std::endl;
	std::cout << "        ls                        չʾ�����ļ���              " << std::endl;
	std::cout << "        ls Ŀ¼��                  ��ʾĿ¼·���µ������ļ���              " << std::endl;
	std::cout << "        ls -l                     ��ʾ�����ļ���Ϣ              " << std::endl;
	std::cout << "        ls -l �ļ���               ��ʾ�ļ���Ϣ              " << std::endl;
	std::cout << "        ls -l Ŀ¼��               չʾĿ¼�������ļ���Ϣ              " << std::endl;
	std::cout << "        ll                        ��ʾ��ǰ·���������ļ���Ϣ              " << std::endl;
	std::cout << "        ll �ļ���                  ��ʾ��ǰ�ļ���Ϣ              " << std::endl;
	std::cout << "        ll Ŀ¼��                  ��ʾĿ¼�������ļ���Ϣ              " << std::endl;
	std::cout << "        touch �ļ���              �����ļ�                  " << std::endl;
	std::cout << "        mkdir Ŀ¼��              ����Ŀ¼                  " << std::endl;
	std::cout << "        find �ļ���               �ݹ�ز����ļ�            " << std::endl;
	std::cout << "        rm �ļ���                 ɾ���ļ�                  " << std::endl;
	std::cout << "        rm -r Ŀ¼��              �ݹ��ɾ��Ŀ¼�����ļ�    " << std::endl;
	std::cout << "        cp �ļ��� ���ļ���         �����ļ�                  " << std::endl;
	std::cout << "        cp -r Ŀ¼�� ��Ŀ¼��      �ݹ鸴���ļ���                  " << std::endl;
	std::cout << "        mv �ļ��� Ŀ¼��           �ƶ��ļ���Ŀ¼��        " << std::endl;
	std::cout << "        mv �ļ��� ..              �ƶ��ļ�����һ��Ŀ¼��         " << std::endl;
	std::cout << "        mv Ŀ¼�� Ŀ¼��           �ƶ�Ŀ¼����һ��Ŀ¼         " << std::endl;
	std::cout << "        mv Ŀ¼�� ..              �ƶ�Ŀ¼����һ��Ŀ¼��      " << std::endl;
	std::cout << "        cat �ļ���                ��ʾ�ļ��е���������      " << std::endl;
	std::cout << "        head �ļ���               ��ʾ�ļ���ͷ���е�����      " << std::endl;
	std::cout << "        head -num �ļ���          ��ʾ�ļ���ͷnum�е�����      " << std::endl;
	std::cout << "        tail �ļ���               ��ʾ�ļ���β��������      " << std::endl;
	std::cout << "        tail -num �ļ���          ��ʾ�ļ���βnum������      " << std::endl;
	std::cout << "        chmod (644) �ļ���/Ŀ¼�� �л��ļ�Ȩ��              " << std::endl;
	std::cout << "        useradd �û���            ����û�[Ĭ������Ϊ�û���]              " << std::endl;
	std::cout << "        passwd                    �޸ĵ�ǰ�û�������              " << std::endl;
	std::cout << "        passwd �û���             �޸�ָ���û�������              " << std::endl;
	std::cout << "        userdel -r �û���         ɾ���û���������              " << std::endl;
	std::cout << "        vim �ļ���                �����vim�༭�������ı��༭              " << std::endl;

	std::cout << std::endl;
	std::cout << "*******************��ӭʹ��linux�ļ�ϵͳ*******************" << std::endl;
	std::cout << std::endl;
}

/**
* @brief �����ļ�ϵͳ���û�����ʼ��
*/
void initUserLevelSystem(std::string path, DIR* MFD, User UFD[], bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (MFD->child[i] != NULL)//Ҳ���Ǵ����û�Ŀ¼
		{
			// std::cout << "�û���" << MFD->child[i]->fileName << " ��С��" << MFD->fcb[i]->fileSize << "KB" << std::endl;
			// std::cout <<  MFD->child[i]->fileName << std::endl;
			// std::cout <<  MFD->child[i]->filePath << std::endl;
			// std::cout << MFD->child[i]->parent->fileName << std::endl;
			// std::cout << MFD->child[i]->pbNUmber << std::endl;
			// std::cout << MFD->child[i]->owner->userDir->fileName << std::endl;
			//std::cout << std::endl;

			initUserLevelSystemTree(MFD->child[i], i, UFD, Map);//�ݹ鴦���û�Ŀ¼�µ�������Ϣ
		}
	}
}

/**
* @brief ��ɫ����
*
*/
void setColor(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
/**
* @brief ��ʾ�ļ���ʼ����
*/
void showDate(long long createTime)
{
	struct tm * p = localtime(&createTime);
	printf("%d/%d/%d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

/**
* @brief ��ʾ�ļ�����
*/
void showInfo(FCB* fcb)
{
	if (fcb->fileType == "dir")
	{
		std::cout << "d";
	}
	else
	{
		std::cout << "-";
	}
	std::cout << (fcb->access[0].read ? "r" : "-") << (fcb->access[0].write ? "w" : "-") << (fcb->access[0].execute ? "x" : "-");
	std::cout << (fcb->access[1].read ? "r" : "-") << (fcb->access[1].write ? "w" : "-") << (fcb->access[1].execute ? "x" : "-");
	std::cout << (fcb->access[2].read ? "r" : "-") << (fcb->access[2].write ? "w" : "-") << (fcb->access[2].execute ? "x" : "-");
	std::cout << "  ";

	std::cout << fcb->pbNUmber << "  ";
	std::cout << fcb->owner->userName << "  ";
	std::cout << fcb->groupOwner->userName << "  ";
	if (fcb->fileSize < 512 && fcb->fileSize > 0.05)
	{
		std::cout << std::setprecision(3) << fcb->fileSize << "KB" << "\t";
	}
	else if (fcb->fileSize > 512)
	{
		std::cout << std::setprecision(3) << fcb->fileSize / 1024 << "MB" << "\t";
	}
	else if (fcb->fileSize < 0.05)
	{
		if (fcb->fileSize == 0)
		{
			std::cout << "0B" << "\t\t";
		}
		else
		{
			std::cout << std::setprecision(2) << fcb->fileSize * 1024 << "B" << "\t\t";
		}
	}

	showDate(fcb->createTime);
	std::cout << "\t";
	if (fcb->fileType == "dir")
	{
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
		std::cout << fcb->fileName << "\t";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
	}
	else
	{
		std::cout << fcb->fileName << "\t";
	}
	std::cout << fcb->filePath << std::endl;
}

/**
* @brief ��ʾ�����ļ���Ϣ
*/
void lsAll(DIR* dir)
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			showInfo(dir->fcb[i]);
		}
	}
}

/**
* @brief ��ʾ�ļ���
*/
void lsName(DIR* dir)
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
				std::cout << dir->fcb[i]->fileName << "   ";
				setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				std::cout << dir->fcb[i]->fileName << "   ";
			}
		}
	}
	std::cout << std::endl;
}

/**
* @brief �ų�window��ǰ׺��Ϊת��Ϊlinux·����׼��
*/
std::string excludePathPrefix(const std::string& fullPath, const std::string& prefixToExclude)
{
	// ���ǰ׺�Ƿ����������·����
	if (fullPath.find(prefixToExclude) == 0)
	{
		// �����ų�ǰ׺���ʣ�ಿ��
		size_t prefixLength = prefixToExclude.length();
		return fullPath.substr(prefixLength);
	}
	else
	{
		// ���ǰ׺������������·���У��򷵻�ԭʼ·��
		return fullPath;
	}
}

/**
* @brief ��window·��ת��Ϊ��ǰ�û���linux·��
*/
std::string convertWinToLinux(std::string winPath, std::string userName, std::string homePath)
{
	if (userName == "root")
	{
		// Ҫ�ų���ǰ׺
		std::string prefixToExclude = homePath.substr(0, homePath.find_last_of("\\"));
		// ���ú����ų�ǰ׺
		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);

		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//��·���еķ�б���滻Ϊб��
		return linuxPath;

	}
	else
	{
		// Ҫ�ų���ǰ׺
		std::string prefixToExclude = homePath + "\\" + userName;

		// ���ú����ų�ǰ׺
		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);//�����û��ֶ����·��

		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//��·���еķ�б���滻Ϊб��
		return linuxPath;
	}

}

/**
* @brief ��¼
*/
int Login(User UFD[])
{
	int num = -1;
	int flag = 0;
	while (num == -1)
	{
		std::cout << "localhost: ";
		std::string input;
		getline(std::cin, input);
		if (input == "login")
		{

			std::cout << "user: ";
			std::string username;
			getline(std::cin, username);

			std::cout << "password: ";
			std::string password;
			getline(std::cin, password);
			///////////////////////////////////�����ı�,��ȡ����" "Ҳ����Ӱ��////////////////
			//std::cout << UFD[4].userName << UFD[4].password << std::endl;
			//std::cout << UFD[4].userName.size() << UFD[4].password.size() << std::endl;
			//std::cout << std::endl;
			//std::cout << username << password << std::endl;
			//std::cout << username.size() << password.size() << std::endl;
			//system("pause");

			//if (UFD[4].userName == username && UFD[4].password == password)
			//{
			//	std::cout << "true" << std::endl;
			//}
			////////////////////////////////////////////
			for (int i = 0; i < 10; i++)
			{
				//std::cout << UFD[i].userName << UFD[i].password << std::endl;
				//std::cout << username << password << std::endl;
				if (UFD[i].userDir != NULL && UFD[i].userName == username && UFD[i].password == password)
				{

					num = i;//�ڼ����û�
					flag = 1;
					break;
				}
			}
			//system("pause");

		}
		else if (input == "help")
		{
			help();
		}
		else if (input == "clear")
		{
			system("cls");
		}
		else
		{
			std::cout << "operation error�� please login!" << std::endl;
		}
	}
	if (flag == 0)
	{
		std::cout << "no such user!" << std::endl;
	}
	else
	{
		std::cout << "login success!" << std::endl;
	}

	return num;
}

/**
* @brief ����·���¶�ӦĿ¼
*/
DIR* findDir(DIR* dir, std::string dirPath)
{
	DIR* temp = nullptr;
	for (int i = 0; i < 10; i++)
	{
		if (dir->child[i] != NULL)
		{
			if (dir->child[i]->filePath == dirPath)
			{
				temp = dir->child[i];
			}
			else
			{
				temp = findDir(dir->child[i], dirPath);
			}
		}
	}
	return temp;
}

/**
* @brief �ݹ�ɾ���ļ����µ��ļ�
*/
void rm_r(DIR* dir, bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				rm_r(dir->child[i], Map);//�ݹ�ɾ����Ŀ¼���ȵݹ鵽��ɾ���ļ����ٿ���ɾ��Ŀ¼

				//ɾ��Ŀ¼
				////////////////////////////ɾ��Ŀ¼��¼�Ŀ�////////////////
				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
				{
					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
				}
				//ɾ��Ŀ¼
				if (!RemoveDirectoryA((dir->fcb[i]->filePath).c_str()))//ɾ��Ŀ¼
				{
					std::cout << "ɾ��ʧ��!" << std::endl;
				}
				//ɾ��Ŀ¼�ļ�¼
				delete dir->fcb[i];
				delete dir->child[i];
				dir->fcb[i] = NULL;
				dir->child[i] = NULL;
			}
			else
			{
				//ɾ���ļ�
				////////////////////////////ɾ���ļ���¼�Ŀ�////////////////
				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
				{
					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
				}
				//ɾ����ʵ�ļ�
				if (!DeleteFileA((dir->fcb[i]->filePath).c_str()))//ɾ���ļ�
				{
					std::cout << "ɾ��ʧ��!" << std::endl;
				}
				//ɾ���ļ�ϵͳ�ļ�¼
				delete dir->fcb[i];
				dir->fcb[i] = NULL;
			}
		}

	}
}

/**
* @brief �������ô�С�����������¼����ļ���С�������¼����Ŀ¼��С
* @param dir Ŀ¼
* @param index ��һ��Ŀ¼��Ӧ����Ŀ¼������
*/
double resetSize(DIR *dir, int index)
{
	dir->parent->fcb[index]->fileSize = 0;//Ŀ¼��Ӧ��fcb�Ĵ�С��������,��ʼ�����С
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				dir->parent->fcb[index]->fileSize += resetSize(dir->child[i], i);//Ŀ¼��Ӧ��fcb�Ĵ�С��������
			}
			else
			{
				dir->parent->fcb[index]->fileSize += dir->fcb[i]->fileSize;
			}
		}
	}
	return dir->parent->fcb[index]->fileSize;
}

/**
* brief �ݹ鸴��Ŀ¼�������ļ�
*/
void cp_r(DIR* src, DIR* dst, bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (src->fcb[i] != NULL)//���ļ���Ŀ¼
		{
			if (src->fcb[i]->fileType == "dir")
			{
				///////////////////��ʼ��Ŀ¼��Ϣ////////////////////////
				dst->child[i] = new DIR;
				for (int k = 0; k < 10; k++)
				{
					dst->child[i]->fcb[k] = NULL;
					dst->child[i]->child[k] = NULL;
				}
				dst->fcb[i] = new FCB;
				dst->fcb[i]->firstBlock = new PBC;
				dst->child[i]->dirFcb = dst->fcb[i];
				dst->fcb[i]->access[0].read = src->fcb[i]->access[0].read;
				dst->fcb[i]->access[0].write = src->fcb[i]->access[0].write;
				dst->fcb[i]->access[0].execute = src->fcb[i]->access[0].execute;
				dst->fcb[i]->access[1].read = src->fcb[i]->access[1].read;
				dst->fcb[i]->access[1].write = src->fcb[i]->access[1].write;
				dst->fcb[i]->access[1].execute = src->fcb[i]->access[1].execute;
				dst->fcb[i]->access[2].read = src->fcb[i]->access[2].read;
				dst->fcb[i]->access[2].write = src->fcb[i]->access[2].write;
				dst->fcb[i]->access[2].execute = src->fcb[i]->access[2].execute;
				dst->fcb[i]->createTime = time(0);
				dst->fcb[i]->fileName = src->fcb[i]->fileName;
				dst->fcb[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
				dst->fcb[i]->fileSize = src->fcb[i]->fileSize;
				dst->fcb[i]->fileType = "dir";
				dst->fcb[i]->groupOwner = src->fcb[i]->groupOwner;
				dst->fcb[i]->owner = src->fcb[i]->owner;

				dst->child[i]->parent = dst;
				dst->child[i]->dirFcb = dst->fcb[i];
				dst->child[i]->fileName = src->fcb[i]->fileName;
				dst->child[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
				dst->child[i]->groupOwner = src->fcb[i]->groupOwner;
				dst->child[i]->owner = src->fcb[i]->owner;
				dst->child[i]->groupOwner = src->fcb[i]->groupOwner;

				///////////////////�ļ�ϵͳ�з���� ////////////////////////
				for (int j = 0; j < 1024; j++)
				{
					if (Map[j] == false)
					{
						Map[j] = true;
						dst->fcb[i]->firstBlock->blockIndexList[dst->fcb[i]->firstBlock->index++] = j;
						dst->child[i]->pbNUmber = j;
						dst->fcb[i]->pbNUmber = j;
						break;
					}
				}
				///////////////////������ʵ�е��ļ���/////////////////////
				if (!CreateDirectory((dst->fcb[i]->filePath).c_str(), NULL))
				{
					std::cout << "����ʧ�ܣ�" << std::endl;
					continue;
				}

				///////////////////�ݹ鸴����Ŀ¼////////////////
				cp_r(src->child[i], dst->child[i], Map);

			}
			else
			{
				////////////////////////////////////////////////////��ʼ���ļ���Ϣ//////////////////////////////////////////

				///////////////////////////////////////////////////////////////cp�������Ż�������������ʱ����к�����װ�Ż�///////////////////////////////
				dst->fcb[i] = new FCB;
				dst->fcb[i]->firstBlock = new PBC;

				dst->fcb[i]->access[0].read = src->fcb[i]->access[0].read;
				dst->fcb[i]->access[0].write = src->fcb[i]->access[0].write;
				dst->fcb[i]->access[0].execute = src->fcb[i]->access[0].execute;
				dst->fcb[i]->access[1].read = src->fcb[i]->access[1].read;
				dst->fcb[i]->access[1].write = src->fcb[i]->access[1].write;
				dst->fcb[i]->access[1].execute = src->fcb[i]->access[1].execute;
				dst->fcb[i]->access[2].read = src->fcb[i]->access[2].read;
				dst->fcb[i]->access[2].write = src->fcb[i]->access[2].write;
				dst->fcb[i]->access[2].execute = src->fcb[i]->access[2].execute;

				dst->fcb[i]->createTime = time(0);
				dst->fcb[i]->fileName = src->fcb[i]->fileName;
				dst->fcb[i]->filePath = dst->filePath + "\\" + src->fcb[i]->fileName;
				dst->fcb[i]->fileSize = src->fcb[i]->fileSize;
				dst->fcb[i]->fileType = "-";
				dst->fcb[i]->groupOwner = src->fcb[i]->groupOwner;
				dst->fcb[i]->owner = src->fcb[i]->owner;
				/////////////////////////////////////�ļ�ϵͳ�з����///////////////////////////////////
				int count = src->fcb[i]->firstBlock->index;
				int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
				for (int j = 0; j < 1024; j++)
				{
					if (Map[j] == false)
					{
						Map[j] = true;
						if (flag == 0)
						{
							dst->fcb[i]->pbNUmber = j;//��һ�����
							flag = 1;
						}
						count--;
						dst->fcb[i]->firstBlock->blockIndexList[dst->fcb[i]->firstBlock->index++] = j;//���
					}
					if (count == 0)
					{
						break;
					}
				}
				//////////////////////////////////////////������и����ļ�////////////////////////////////
				CopyFile((src->fcb[i]->filePath).c_str(), (dst->fcb[i]->filePath).c_str(), false);//�����ļ�
			}
		}

	}
}

/**
* @brief ���ݲ�ͬ�û���ʾ��ͬ·��,��ʾ��
*/
void showCommandLine(DIR* userDir, std::string nowPath, std::string userName, std::string homePath)
{
	if (userDir->fileName == "root")//����л�Ϊroot�û���root@localhost:/home/user1# ·������ɫ
	{
		//std::cout<< "root" << "@localhost";
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//������ɫ����
		std::cout << "root@localhost";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
		std::cout << ":";
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
		std::cout << convertWinToLinux(nowPath, userName, homePath);
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "# ";
	}
	else
	{
		//user1@localhost: ~$ ·������ɫ
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//������ɫ����
		std::cout << userName << "@localhost";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
		std::cout << ": ";
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//������ɫ����
		std::cout << "~";
		std::cout << convertWinToLinux(nowPath, userName, homePath);
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//����Ĭ����ɫ
		std::cout << "$ ";
	}
}

/**
* @brief �����ı�����
*/
int getfileLineNum(std::string filePath)
{
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in)
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	else
	{
		std::cout << "no such file��" << std::endl;
	}
	in.close();
	return count;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////��λ�������ܺ���///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////ls//////////////////////////////////////////////////////////////////////
/**
* @brief ��ʾָ��Ŀ¼������
*/
void lsLocalNameF(DIR* now, std::string second_2)
{
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second_2)
		{
			lsName(now->child[i]);
			break;
		}
	}
}
/**
* @brief ��ʾָ��Ŀ¼���ļ���Ϣ�򵥸��ļ���Ϣ
*/
void ls_lAnyTypeF(DIR* now, std::string second_2)
{
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
		{
			if (now->fcb[i]->fileType == "dir")
			{
				lsAll(now->child[i]);
			}
			else
			{
				showInfo(now->fcb[i]);
			}
		}
	}
}

/**
* @brief ��ʾ��Ӧ֪��Ŀ¼���ļ���������ϸ��Ϣ
*/
void llAnyTypeF(DIR* now, std::string second)
{
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second)
		{
			if (now->fcb[i]->fileType == "dir")
			{
				lsAll(now->child[i]);
			}
			else
			{
				showInfo(now->fcb[i]);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////touch/////////////////////////////////////////////////////////////////////////

/**
* @brief �����ļ�����
*/
bool touchF(DIR* now, DIR* userDir, bool Map[], std::string second)
{
	int exis = -1;
	////////////////////////�ж��ļ��Ƿ����/////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second)
		{
			exis = 1;
			std::cout << "�ļ��Ѵ��ڣ�" << std::endl;
			break;
		}
	}
	if (exis == 1)//�ļ��Ѵ���
	{
		return 0;
	}

	////////////////////////�ж��Ƿ��п���λ��///////////////////
	int empty = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] == NULL)
		{
			empty = i;
			break;
		}
	}
	if (empty == -1)
	{
		std::cout << "�ļ�������" << std::endl;
		return 0;
	}

	//������д����ļ�
	std::ofstream File;
	File.open(now->filePath + "\\" + second);
	File.close();


	/////////////////////////////////////һ���ļ��������ظ����룬�����Ż��ȷ���/////////////////////////////////////
	//���ļ�ϵͳ����
	now->fcb[empty] = new FCB;//����һ��FCB
	now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��

	now->fcb[empty]->access[0].read = true;
	now->fcb[empty]->access[0].write = true;
	now->fcb[empty]->access[1].read = true;
	now->fcb[empty]->access[1].write = true;
	now->fcb[empty]->access[2].read = true;
	now->fcb[empty]->createTime = time(0);//����ʱ��
	now->fcb[empty]->fileName = second;//�ļ���
	now->fcb[empty]->fileType = "-";
	now->fcb[empty]->owner = userDir->owner;//ָ���û�
	now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
	double sizeKB = 0;//�����ļ���С
	now->fcb[empty]->fileSize = sizeKB;
	now->fcb[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��
	//�����
	int count = 1;
	if (sizeKB > count)//�������������Ҫ�����һ����
	{
		count++;
	}
	int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
	for (int j = 0; j < 1024; j++)//����������Ϊ1024��
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			if (flag == 0)
			{
				now->fcb[empty]->pbNUmber = j;//�ļ����
			}
			flag = 1;
			count--;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
			if (count == 0)
			{
				break;
			}
		}
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////mkdir////////////////////////////////////////////////////////////////////////
/**
* @brief �����ļ��й���
*/
bool mkdirF(DIR* now, DIR* userDir, bool Map[], std::string second)
{
	int exis = -1;
	////////////////////////�ж�Ŀ¼�Ƿ����/////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second)
		{
			exis = 1;
			std::cout << "�ļ��Ѵ��ڣ�" << std::endl;
			break;
		}
	}
	if (exis == 1)//�ļ��Ѵ���
	{
		return 0;
	}
	////////////////////////�ж��Ƿ��п���λ��///////////////
	int empty = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] == NULL)
		{
			empty = i;
			break;
		}
	}
	if (empty == -1)
	{
		std::cout << "�ļ�������" << std::endl;
		return 0;
	}
	//�����ļ���
	if (!CreateDirectory((now->filePath + "\\" + second).c_str(), NULL))
	{
		std::cout << "����ʧ�ܣ�" << std::endl;
		return 0;
	}
	//////////////////////////���³�ʼ��Ŀ¼���ظ����룬�����Ż��ȷ���///////////////////////////
	now->fcb[empty] = new FCB;//����һ��FCB
	now->child[empty] = new DIR;//����һ����Ŀ¼
	for (int j = 0; j < 10; j++)
	{
		now->child[empty]->fcb[j] = NULL;
		now->child[empty]->child[j] = NULL;
	}
	now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
	for (int j = 0; j < 1024; j++)//����������Ϊ1024��
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
			now->fcb[empty]->pbNUmber = j;//Ŀ¼���
			now->fcb[empty]->access[0].read = true;
			now->fcb[empty]->access[0].write = true;
			now->fcb[empty]->access[1].read = true;
			now->fcb[empty]->access[1].write = true;
			now->fcb[empty]->access[2].read = true;
			now->fcb[empty]->createTime = time(0);//����ʱ���ʼ��
			now->fcb[empty]->fileName = second;//�ļ���
			now->fcb[empty]->fileType = "dir";
			now->fcb[empty]->owner = userDir->owner;//ָ���û�
			now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
			now->fcb[empty]->fileSize = 0;//Ŀ¼��С
			now->fcb[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��

			now->child[empty]->fileName = second;//�ļ���
			now->child[empty]->filePath = now->filePath + "\\" + second;//�ļ�·��
			now->child[empty]->owner = userDir->owner;//ָ���û�
			now->child[empty]->groupOwner = userDir->owner;//ָ���û�
			now->child[empty]->parent = now;//ָ��Ŀ¼
			now->child[empty]->pbNUmber = j;//Ŀ¼���
			now->child[empty]->dirFcb = now->fcb[empty];//ָ��Ŀ¼FCB
			break;//��ΪĿ¼ֻ��Ҫһ���飬ֱ���˳��Ϳ�
		}
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////rm///////////////////////////////////////////////////////////////////////////////
/**
* @brief ɾ���ļ�
*/
bool rmF(DIR* now, DIR* userDir, bool Map[], std::string second_2)
{
	//std::cout << "file" << std::endl;
	////////////////////////�ж��ļ��Ƿ����//////////////////////////////////////////////////////
	int exist = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
		{
			exist = i;
			break;
		}
	}
	if (exist == -1)//�ļ�������
	{
		return 0;
	}
	if (now->fcb[exist]->fileType == "dir")
	{
		return 0;
	}
	////////////////////////ɾ���ļ���/////////////////////////////////////////////////////////////
	if (!DeleteFileA((now->fcb[exist]->filePath).c_str()))//ɾ���ļ�
	{
		std::cout << "ɾ��ʧ��!" << std::endl;
		return 0;
	}
	//��ȥ�ļ���ռ�õĿ�
	for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
	{
		Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//������Ϊδ����
	}
	////////////////////////ɾ���ļ�ϵͳ��///////////////
	delete (now->fcb[exist]);//�ͷ�FCB
	now->fcb[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL
	/////////////////////////////////////////////�µĵݹ����¼����С////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
		}
	}
	return 1;
}

/**
* @brief ɾ���ļ���
*/
bool rm_rF(DIR* now, DIR* userDir, bool Map[], std::string second_2)
{
	//std::cout << "-r file" << std::endl;
	////////////////////////�ж�Ŀ¼�Ƿ����/////////////////
	int exist = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second_2)
		{
			exist = i;
			break;
		}
	}
	if (exist == -1)//Ŀ¼������
	{
		return 0;
	}

	//��ȥ�ļ�����ռ�õĿ�
	for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
	{
		Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//������Ϊδ����
	}
	///////////////////�ݹ�ɾ��Ŀ¼����Ŀ¼���µ��ļ�///////////////
	rm_r(now->child[exist], Map);
	////////////////////////ɾ��Ŀ¼/////////////////
	if (!RemoveDirectoryA((now->fcb[exist]->filePath).c_str()))//ɾ��Ŀ¼
	{
		std::cout << "ɾ��ʧ��!" << std::endl;
		return 0;
	}

	////////////////////////ɾ���ļ�ϵͳ�е���Ϣ////////////  
	delete now->fcb[exist];//�ͷŵ�ǰĿ¼��FCB
	delete now->child[exist];//�ͷŵ�ǰĿ¼��Ŀ¼
	now->fcb[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL
	now->child[exist] = NULL;//����ǰĿ¼������ļ�����ΪNULL

	///////////////////////////////////////////�µĵݹ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////find//////////////////////////////////////////////////////////////////////////////
/**
* @brief ���Ҷ�Ӧ�ļ�����·����Ϣ
*/
void find(DIR* dir, std::string fileName)
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileName == fileName)
			{
				showInfo(dir->fcb[i]);
			}
			else if (dir->fcb[i]->fileType == "dir")
			{
				find(dir->child[i], fileName);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////cp//////////////////////////////////////////////////////////////////
/**
* @brief �����ļ�
*/
bool cpF(DIR* now, DIR* userDir, bool Map[], std::string second_1, std::string second_2)
{
	/////////////////////////�ж��ļ��Ƿ����////////////////////////////
	int cpSource = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_1 && now->fcb[i]->fileType == "-")
		{
			cpSource = i;
			break;
		}
	}
	if (cpSource == -1)
	{
		std::cout << "Դ�ļ������ڣ�" << std::endl;
		return 0;
	}
	
	////////////////////////�ҵ��Ƿ��п���λ��//////////////////////
	int empty = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] == NULL)
		{
			empty = i;
			break;
		}
	}
	if (empty == -1)
	{
		std::cout << "�ļ�������" << std::endl;
		return 0;
	}

	/////////////////////////////////////��ʼ�������ļ�����Ϣ//////////////////////////////
	now->fcb[empty] = new FCB;//����һ��FCB
	now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
	///////////////////////////////////////���´�����Ż�///////////////////////////////////////
	now->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
	now->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
	now->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;

	now->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
	now->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
	now->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;

	now->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
	now->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
	now->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;

	now->fcb[empty]->createTime = time(0);//����ʱ��
	now->fcb[empty]->fileName = second_2;//�ļ���
	now->fcb[empty]->filePath = now->filePath + "\\" + second_2;//�ļ�·��
	now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//�ļ���С
	now->fcb[empty]->fileType = now->fcb[cpSource]->fileType;//�ļ�����
	now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
	now->fcb[empty]->owner = userDir->owner;//ָ���û�

	/////////////////////////////////�����///////////////////////////////////////
	int count = now->fcb[cpSource]->firstBlock->index;
	int flag = 0;//�ж��Ƿ��Ƿ���ĵ�һ����
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			if (flag == 0)
			{
				now->fcb[empty]->pbNUmber = j;//��һ�����
				flag = 1;
			}
			count--;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
		}
		if (count == 0)
		{
			break;
		}
	}
	/////////////////////////////////�����ļ�////////////////////////////////////////
	CopyFile((now->fcb[cpSource]->filePath).c_str(), (now->fcb[empty]->filePath).c_str(), false);//�����ļ�

	///////////////////////////////////////////�µ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
		}
	}
	return 1;
}

/**
* @brief �����ļ���
*/
bool cp_rF(DIR* now, DIR* userDir, bool Map[], std::string second_2_1, std::string second_2_2)
{
	/////////////////////////�ж�Ŀ¼�Ƿ����////////////////////////////
	int cpSource = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second_2_1)
		{
			cpSource = i;
			break;
		}
	}
	if (cpSource == -1)
	{
		std::cout << "ԴĿ¼�����ڣ�" << std::endl;
		return 0;
	}

	////////////////////////�ҵ��Ƿ��п���λ��/////////////////////////
	int empty = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] == NULL)
		{
			empty = i;
			break;
		}
	}
	if (empty == -1)
	{
		std::cout << "�ļ�������" << std::endl;
		return 0;
	}
	////////////////////////////////�����ļ���:Ŀ��second_2_1 ����second_2_2////////////////////////////////////////
	now->fcb[empty] = new FCB;//����һ��FCB
	now->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
	now->child[empty] = new DIR;//����һ����Ŀ¼
	now->child[empty]->dirFcb = now->fcb[empty];
	for (int k = 0; k < 10; k++)
	{
		now->child[empty]->fcb[k] = NULL;
		now->child[empty]->child[k] = NULL;
	}
	//////////////////////////////////////////////////////////
	now->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
	now->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
	now->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;
	now->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
	now->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
	now->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;
	now->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
	now->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
	now->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;

	now->fcb[empty]->createTime = now->fcb[cpSource]->createTime;//����ʱ��
	now->fcb[empty]->fileName = second_2_2;//�ļ���
	now->fcb[empty]->filePath = now->filePath + "\\" + second_2_2;//�ļ�·��
	now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//�ļ���С
	now->fcb[empty]->fileType = "dir";//�ļ�����
	now->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
	now->fcb[empty]->owner = userDir->owner;//ָ���û�

	now->child[empty]->fileName = second_2_2;//�ļ���
	now->child[empty]->filePath = now->filePath + "\\" + second_2_2;//�ļ�·��
	now->child[empty]->groupOwner = userDir->owner;//ָ���û�
	now->child[empty]->owner = userDir->owner;//ָ���û�
	now->child[empty]->parent = now;//ָ��Ŀ¼
	now->child[empty]->dirFcb = now->fcb[empty];//ָ��Ŀ¼FCB
	//////////////////////////////////////��Ŀ¼�����////////////////////////////////
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			now->fcb[empty]->pbNUmber = j;//��һ�����
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//���
			now->child[empty]->pbNUmber = j;//��һ�����
			break;
		}
	}
	///////////////////////////////////�½�Ŀ¼/////////////////////////////////////////
	if (!CreateDirectory((now->fcb[empty]->filePath).c_str(), NULL))
	{
		std::cout << "Ŀ¼����ʧ�ܣ�" << std::endl;
		return 0;
	}
	//////////////////////////////////////��ʼ�ݹ鸴��Ŀ¼��������Ϣ���ļ�//////////////////////////////
	cp_r(now->child[cpSource], now->child[empty], Map);

	///////////////////////////////////////////�µ����¼����С:�ɿ��ǵ�/��Ŀ¼�µĲ���������������////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//���¼���Ŀ¼��С��Ҳ��Ŀ¼�������ļ���С��
		}
	}
	return 1;
}

/**
* @brief �����ļ��е�����λ��
*/
bool cp_rAnyDir(DIR* now, DIR* aim,DIR* userDir, bool Map[], std::string second_2_1, std::string second_2_2)
{
	/////////////////////////�ж�Ŀ¼�Ƿ����////////////////////////////
	int cpSource = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second_2_1)
		{
			cpSource = i;
			break;
		}
	}
	if (cpSource == -1)
	{
		std::cout << "ԴĿ¼�����ڣ�" << std::endl;
		return 0;
	}

	////////////////////////�ҵ��Ƿ��п���λ��/////////////////////////
	int empty = -1;
	for (int i = 0; i < 10; i++)
	{
		if (aim->fcb[i] == NULL)
		{
			empty = i;
			break;
		}
	}
	if (empty == -1)
	{
		std::cout << "�ļ�������" << std::endl;
		return 0;
	}
	////////////////////////////////�����ļ���:Ŀ��second_2_1 ����second_2_2////////////////////////////////////////
	aim->fcb[empty] = new FCB;//����һ��FCB
	aim->fcb[empty]->firstBlock = new PBC;//�������ƽṹ��
	aim->child[empty] = new DIR;//����һ����Ŀ¼
	for (int k = 0; k < 10; k++)
	{
		aim->child[empty]->fcb[k] = NULL;
		aim->child[empty]->child[k] = NULL;
	}
	//////////////////////////////////////////////////////////
	aim->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
	aim->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
	aim->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;
	aim->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
	aim->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
	aim->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;
	aim->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
	aim->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
	aim->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;

	aim->fcb[empty]->createTime = now->fcb[cpSource]->createTime;//����ʱ��
	aim->fcb[empty]->fileName = second_2_2;//�ļ���
	aim->fcb[empty]->filePath = aim->filePath + "\\" + second_2_2;//�ļ�·��
	aim->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//�ļ���С
	aim->fcb[empty]->fileType = "dir";//�ļ�����
	aim->fcb[empty]->groupOwner = userDir->owner;//ָ���û�
	aim->fcb[empty]->owner = userDir->owner;//ָ���û�

	aim->child[empty]->fileName = second_2_2;//�ļ���
	aim->child[empty]->filePath = aim->filePath + "\\" + second_2_2;//�ļ�·��
	aim->child[empty]->groupOwner = userDir->owner;//ָ���û�
	aim->child[empty]->owner = userDir->owner;//ָ���û�
	aim->child[empty]->parent = aim;//ָ��Ŀ¼
	aim->child[empty]->dirFcb = now->fcb[empty];//ָ��Ŀ¼FCB
	//////////////////////////////////////��Ŀ¼�����////////////////////////////////
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			aim->fcb[empty]->pbNUmber = j;//��һ�����
			aim->fcb[empty]->firstBlock->blockIndexList[aim->fcb[empty]->firstBlock->index++] = j;//���
			aim->child[empty]->pbNUmber = j;//��һ�����
			break;
		}
	}
	///////////////////////////////////�½�Ŀ¼/////////////////////////////////////////
	if (!CreateDirectory((aim->fcb[empty]->filePath).c_str(), NULL))
	{
		std::cout << "Ŀ¼����ʧ�ܣ�" << std::endl;
		return 0;
	}
	//////////////////////////////////////��ʼ�ݹ鸴��Ŀ¼��������Ϣ���ļ�//////////////////////////////
	cp_r(now->child[cpSource], aim->child[empty], Map);

	return 1;
}
////////////////////////////////////////////////////////////////////////////////////mv////////////////////////////////////////////////////////////////
/**
* @brief �ƶ��ļ����ļ���
*/
void mvF(DIR* now, int mvSource, int exist, int empty)
{
	////////////////////////////////////////////��������ƶ��ļ�////////////////////////////////////////
	MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->child[exist]->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//�ƶ���

	now->child[exist]->fcb[empty] = now->fcb[mvSource];//���ļ��ƶ���Ŀ���ļ���
	now->fcb[mvSource] = NULL;//��Դ�ļ�����ΪNULL
	/////////////////////////////////////////����Ŀ¼·����Ϣ/////////////////////////////////
	now->child[exist]->fcb[empty]->filePath = now->child[exist]->filePath + "\\" + now->child[exist]->fcb[empty]->fileName;//�����ļ�·��
}

/**
* @brief �ƶ��ļ�����һ��Ŀ¼
*/
void mv_F(DIR* now, int mvSource, int empty)
{
	////////////////////////////////////////////��������ƶ��ļ�////////////////////////////////////////
	MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->parent->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//�ƶ���

	now->parent->fcb[empty] = now->fcb[mvSource];//���ļ��ƶ���Ŀ���ļ���
	now->fcb[mvSource] = NULL;//��Դ�ļ�����ΪNULL
	/////////////////////////////////////////����Ŀ¼·����Ϣ/////////////////////////////////
	now->parent->fcb[empty]->filePath = now->parent->filePath + "\\" + now->parent->fcb[empty]->fileName;//�����ļ�·��
}
////////////////////////////////////////////////////////////////////////////////chmod//////////////////////////////////////////////////////////////
/**
* @brief �л��ļ�Ȩ��
*/
void chmodF(DIR* now, std::string second_1, std::string second_2)
{
	if (second_1.length() != 3)
	{
		return;
	}
	//std::cout << second_1[0] << " " << second_1[1] << " " << second_1[2] << std::endl;
	bool ifSuit = true;
	for (int i = 0; i < 3; i++)
	{
		//std::cout << second_1[i] << " ";
		if (second_1[i] > '7' || second_1[i] < '0')
		{
			ifSuit = false;
			break;;
		}
	}
	if (!ifSuit)
	{
		return;
	}

	//auto num = std::stoi(second_1);
	//std::cout << num << " " << second_2 << std::endl;
	int exist = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
		{
			exist = i;
			break;
		}
	}
	if (exist == -1)
	{
		return;
	}
	//�޸�Ȩ��
	for (int i = 0; i < 3; i++)
	{
		if (second_1[i] == '0')
		{
			now->fcb[exist]->access[i].read = false;
			now->fcb[exist]->access[i].write = false;
			now->fcb[exist]->access[i].execute = false;
		}
		else if (second_1[i] == '1')
		{
			now->fcb[exist]->access[i].read = false;
			now->fcb[exist]->access[i].write = false;
			now->fcb[exist]->access[i].execute = true;

		}
		else if (second_1[i] == '2')
		{
			now->fcb[exist]->access[i].read = false;
			now->fcb[exist]->access[i].write = true;
			now->fcb[exist]->access[i].execute = false;
		}
		else if (second_1[i] == '3')
		{
			now->fcb[exist]->access[i].read = false;
			now->fcb[exist]->access[i].write = true;
			now->fcb[exist]->access[i].execute = true;
		}
		else if (second_1[i] == '4')
		{
			now->fcb[exist]->access[i].read = true;
			now->fcb[exist]->access[i].write = false;
			now->fcb[exist]->access[i].execute = false;
		}
		else if (second_1[i] == '5')
		{
			now->fcb[exist]->access[i].read = true;
			now->fcb[exist]->access[i].write = false;
			now->fcb[exist]->access[i].execute = true;
		}
		else if (second_1[i] == '6')
		{
			now->fcb[exist]->access[i].read = true;
			now->fcb[exist]->access[i].write = true;
			now->fcb[exist]->access[i].execute = false;
		}
		else if (second_1[i] == '7')
		{
			now->fcb[exist]->access[i].read = true;
			now->fcb[exist]->access[i].write = true;
			now->fcb[exist]->access[i].execute = true;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////cat////////////////////////////////////////////////////////////////
/**
* @brief cat���ļ�
*/
int cat(std::string filePath)
{
	SetConsoleOutputCP(65001);//�����������ΪUTF-8
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in)
	{
		while (std::getline(in, line))
		{
			count++;
			std::cout << line << std::endl;
		}
	}
	else
	{
		std::cout << "no such file!" << std::endl;
	}
	in.close();
	SetConsoleOutputCP(936);//���ñ���ΪGBK
	return count;
}
/**
* @brief head���ļ�ͷnum��
*/
int head(std::string filePath, int num)
{
	SetConsoleOutputCP(65001);//���ñ���ΪUTF-8
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in) // �и��ļ�
	{
		while (std::getline(in, line))
		{
			count++;
			std::cout << line << std::endl;
			if (count >= num)
			{
				break;
			}
		}
	}
	else // û�и��ļ�
	{
		std::cout << "no such file" << std::endl;
	}

	in.close();
	SetConsoleOutputCP(936);//���ñ���ΪGBK

	return count;
}
/**
* @brief tail��ȡ�ļ�βnum��
*/
int tail(std::string filePath, int num)
{
	SetConsoleOutputCP(65001);//���ñ���ΪUTF-8
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in) // �и��ļ�
	{
		int sumNum = getfileLineNum(filePath);//��ȡ����
		int showNum = sumNum - num;
		if (showNum < 0)
		{
			showNum = 0;
		}
		while (std::getline(in, line))
		{
			count++;
			if (count > showNum)
			{
				std::cout << line << std::endl;
			}
		}
	}
	else // û�и��ļ�
	{
		std::cout << "no such file" << std::endl;
	}

	in.close();
	SetConsoleOutputCP(936);//���ñ���ΪGBK

	return count;
}
///////////////////////////////////////////////////////////////////////////////useradd////////////////////////////////////////////////////////////////
/**
* @brief ����û�
* @return -1 û��λ�� -2 �Ѵ��ڣ�-3 ����ʧ��.-4 �ļ�ϵͳû��λ��һ�㲻���ܷ���
*/
int addUser(std::string homePath, std::string passwdPath, DIR* MFD,User UFD[], bool Map[], std::string name, std::string password)
{
	for (int i = 0; i < 10; i++)//�����û��Ƿ��Ѵ���
	{
		if (UFD[i].userDir != NULL && UFD[i].userName == name)
		{
			return -2;
		}
	}
	int emptyUser = -1;
	for (int i = 0; i < 10; i++)//����Ƿ��п�λ����û�
	{
		if (UFD[i].userDir == NULL)
		{
			emptyUser = i;
			break;
		}
	}
	if (emptyUser == -1)
	{
		return -1;
	}

	UFD[emptyUser].userName = name;
	UFD[emptyUser].password = password;
	//�����ļ���
	if (!CreateDirectory((homePath + "\\" + name).c_str(), NULL))
	{
		std::cout << "����ʧ�ܣ�" << std::endl;
		return -3;
	}

	int emptyFile = -4;
	for (int i = 0; i < 10; i++)
	{
		if (MFD->fcb[i] == NULL)
		{
			emptyFile = i;
			break;
		}
	}
	if (emptyFile == -4)
	{
		return -4;
	}

	intptr_t handleFile = 0;//�ļ����
	struct _finddata_t fileInfo;//�ļ���Ϣ�ṹ��
	if ((handleFile = _findfirst((homePath + "\\" + name).c_str(), &fileInfo)) != -1)
	{
		MFD->fcb[emptyFile] = new FCB;//����һ��fcb
		//Ϊ�û�Ŀ¼�����ͻ�����Ϣ����ΪĿ¼��ֻ��һ��
		for (int j = 0; j < 1024; j++)
		{
			if (Map[j] == false)
			{

				Map[j] = true;
				MFD->fcb[emptyFile]->firstBlock = new PBC;//�������ƽṹ��
				MFD->fcb[emptyFile]->pbNUmber = j;//�û�Ŀ¼���
				MFD->fcb[emptyFile]->firstBlock->blockIndexList[MFD->fcb[emptyFile]->firstBlock->index++] = j;//���

				MFD->fcb[emptyFile]->fileName = fileInfo.name;//�û���
				MFD->fcb[emptyFile]->fileType = "dir";
				MFD->fcb[emptyFile]->access[0].read = true;
				MFD->fcb[emptyFile]->access[0].write = true;
				MFD->fcb[emptyFile]->access[1].read = true;
				MFD->fcb[emptyFile]->access[1].write = true;
				MFD->fcb[emptyFile]->access[2].read = true;
				MFD->fcb[emptyFile]->owner = &UFD[emptyUser];//ָ������
				MFD->fcb[emptyFile]->groupOwner = &UFD[emptyUser];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
				MFD->fcb[emptyFile]->createTime = fileInfo.time_create;//����ʱ���ʼ��
				MFD->fcb[emptyFile]->filePath = homePath + "\\" + fileInfo.name;//�ļ�·��

				//�����û���ӦĿ¼
				MFD->child[emptyFile] = new DIR;//����һ���û�Ŀ¼
				for (int k = 0; k < 10; k++)
				{
					MFD->child[emptyFile]->fcb[k] = NULL;
					MFD->child[emptyFile]->child[k] = NULL;
				}
				MFD->child[emptyFile]->pbNUmber = j;//�û�Ŀ¼���
				MFD->child[emptyFile]->parent = MFD;//������Ϊ�գ�Ϊ�˽������û������ж���׼��
				MFD->child[emptyFile]->fileName = fileInfo.name;//�û���
				MFD->child[emptyFile]->filePath = homePath + "\\" + fileInfo.name;//�û�Ŀ¼�µ�·��
				MFD->child[emptyFile]->owner = &UFD[emptyUser];//ָ���û�
				MFD->child[emptyFile]->groupOwner = &UFD[emptyUser];//ռʱ�û���ֻ��������chmod�л��û���Ȩ��
				MFD->child[emptyFile]->dirFcb = MFD->fcb[emptyFile];

				UFD[emptyUser].userDir = MFD->child[emptyFile];////��ʼ���û���ָ���Լ��û�Ŀ¼

				break;
			}
		}
		_findclose(handleFile);//�رվ��
	}
	else
	{
		std::cout << "cannot find user file��" << std::endl;
	}
	MFD->fcb[emptyFile]->fileSize = 0;//�մ����û�
	///////////////////////////passwd�ı���Ϣ�������/////////////////
	std::ofstream ofs;//����������
	ofs.open(passwdPath, std::ios::app);//׷��app
	std::string line = name + ":" + password;
	ofs << line << "\n";
	ofs.close();//�ر��ļ�

	return emptyUser;
}
