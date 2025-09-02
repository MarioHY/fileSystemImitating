#include "Util.h"
////////////////////////////////////////////////////////基础函数///////////////////////////////////////////////////////////
/**
* @brief 从root中的account.txt中读取用户信息
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
			std::string name = userMess.substr(0, userMess.find_first_of(":"));//获取用户名
			std::string password = userMess.substr(userMess.find_first_of(":") + 1, userMess.length());
			//std::cout << name << password << std::endl;
			UFD[i].userName = name;
			UFD[i].password = password;
			i++;
			//测试：std::cout << "用户名：" << name << " 密码：" << password << std::endl;
		}
	}
	else
	{
		std::cout << "用户信息不存在" << std::endl;
	}
	//system("pause");
	return i;
}

/**
* @brief 计算文件大小
* @param filePath 文件路径
*/
double calcFileSize(std::string filePath)
{
	double sizeKB = 0;
	std::ifstream file(filePath);//打开文件
	if (file.is_open())
	{
		file.seekg(0, std::ios::end);//移动指针到末尾
		double size = file.tellg();//返回文件指针位置
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
* @brief 计算文件夹大小
*/
double calcDirSize(std::string dirPath)
{
	intptr_t handleFile = 0;//文件句柄
	struct _finddata_t fileInfo;//文件信息结构体
	std::string temp;//临时字符串
	double fileSize = 0;
	if ((handleFile = _findfirst(temp.assign(dirPath).append("\\*").c_str(), &fileInfo)) != -1)//查找目录下的所有文件
	{
		do
		{
			if ((fileInfo.attrib & _A_SUBDIR))//是目录
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//排除.和..目录
				{
					fileSize += calcDirSize(dirPath + "\\" + fileInfo.name);//递归计算子目录大小，这是是总的大小，目录大小就占一个块
				}
			}
			else//是文件
			{
				fileSize += calcFileSize(dirPath + "\\" + fileInfo.name);//计算文件大小
			}
		} while (_findnext(handleFile, &fileInfo) == 0);//查找下一个
		_findclose(handleFile);//关闭句柄
	}
	return fileSize;
}

/**
* @brief 一级文件系统（home）初始化
*/
void initFirstLevelSystem(std::string path, User UFD[], DIR* MFD, std::string passwdPath, bool Map[])
{
	////////////////////////初始化用户信息//////////////////////////
	int userNum = readUsers(passwdPath, UFD);
	//测试std::printf("用户数量：%d\n", userNum);
	///////////////////////////初始化一级目录//////////////////////////////////
	intptr_t handleFile = 0;//文件句柄
	struct _finddata_t fileInfo;//文件信息结构体
	std::string temp;//临时字符串
	//////////////////////////////////以下操作用户目录大小未计算///////////////////////
	if ((handleFile = _findfirst(temp.assign(path).append("\\*").c_str(), &fileInfo)) != -1)
	{
		do//对MFD目录下的所有文件进行遍历(也就是用户目录)
		{
			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)//排除.和..目录,搜索用户目录
			{
				for (int i = 0; i < userNum; i++)
				{
					if (UFD[i].userName == fileInfo.name)//找到了对应的用户目录
					{
						//std::cout << fileInfo.name << std::endl;

						int flag = 0;//标准未处理完
						MFD->fcb[i] = new FCB;//分配一个FCB
						//为用户目录分配块
						for (int j = 0; j < 1024; j++)//块的最大限制为1024个
						{
							if (Map[j] == false)
							{
								Map[j] = true;
								MFD->fcb[i]->firstBlock = new PBC;//物理块控制结构体
								MFD->fcb[i]->pbNUmber = j;//用户目录块号
								MFD->fcb[i]->firstBlock->blockIndexList[MFD->fcb[i]->firstBlock->index++] = j;//块号


								MFD->fcb[i]->fileName = fileInfo.name;//用户名
								MFD->fcb[i]->fileType = "dir";
								MFD->fcb[i]->access[0].read = true;
								MFD->fcb[i]->access[0].write = true;
								MFD->fcb[i]->access[1].read = true;
								MFD->fcb[i]->access[1].write = true;
								MFD->fcb[i]->access[2].read = true;
								MFD->fcb[i]->owner = &UFD[i];//指向自身
								MFD->fcb[i]->groupOwner = &UFD[i];//占时用户组只是自身，待chmod切换用户组权限
								MFD->fcb[i]->createTime = fileInfo.time_create;//创建时间初始化
								MFD->fcb[i]->filePath = path + "\\" + fileInfo.name;//文件路径

								//创建用户对应目录
								MFD->child[i] = new DIR;//分配一个用户目录
								for (int k = 0; k < 10; k++)
								{
									MFD->child[i]->fcb[k] = NULL;
									MFD->child[i]->child[k] = NULL;
								}
								MFD->child[i]->pbNUmber = j;//用户目录块号
								MFD->child[i]->parent = MFD;//将其设为空，为了接下来用户根的判断做准备
								MFD->child[i]->fileName = fileInfo.name;//用户名
								MFD->child[i]->filePath = path + "\\" + fileInfo.name;//用户目录下的路径
								MFD->child[i]->owner = &UFD[i];//指向用户
								MFD->child[i]->groupOwner = &UFD[i];//占时用户组只是自身，待chmod切换用户组权限
								MFD->child[i]->dirFcb = MFD->fcb[i];
								//初始化用户的指向自己用户目录
								UFD[i].userDir = MFD->child[i];

								flag = 1;
								break;//因为目录只需要一个块，直接退出就可
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
		_findclose(handleFile);//关闭句柄
	}
	//system("pause");

	////////////////////////////初始化一级目录长度//////////////////////////////
	intptr_t handleFile2 = 0;//文件句柄
	struct _finddata_t fileInfo2;//文件信息结构体
	std::string temp2;//临时字符串

	if ((handleFile2 = _findfirst(temp2.assign(path).append("\\*").c_str(), &fileInfo2)) != -1)
	{
		do
		{
			if (strcmp(fileInfo2.name, ".") != 0 && strcmp(fileInfo2.name, "..") != 0)
			{
				for (int i = 0; i < userNum; i++)
				{
					if (UFD[i].userName == fileInfo2.name)//找到了对应的用户目录
					{
						MFD->fcb[i]->fileSize = calcDirSize(path + "\\" + fileInfo2.name);//计算用户目录大小
						//std::cout << "用户：" << fileInfo2.name << " 大小：" << MFD->fcb[i]->fileSize << "KB" << std::endl;
					}
				}
			}
		} while (_findnext(handleFile2, &fileInfo2) == 0);
		_findclose(handleFile2);//关闭句柄
	}
}

/**
* @brief 递归初始化二级文件系统以下文件
* @param userDir 用户目录
* @param index 对应用户索引
*/
void initUserLevelSystemTree(DIR *dir, int index, User UFD[], bool Map[])//用户对应的索引
{
	//////////////////////////////////////////初始化各层文件信息//////////////////////////////////
	intptr_t handHandle = 0;//文件句柄
	struct _finddata_t fileInfo;//文件信息结构体
	std::string temp;//临时字符串
	int count = 0;//文件数量
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
				for (int i = 0; i < 10; i++)//最多10个
				{
					//std::cout << dir->fileName << std::endl;

					if (dir->fcb[i] == NULL)//指用户文件为空
					{
						//std::cout << dir->filePath << std::endl;
						count++;
						if (fileInfo.attrib & _A_SUBDIR)//是目录
						{
							dir->fcb[i] = new FCB;//分配一个FCB
							dir->child[i] = new DIR;//分配一个子目录
							for (int k = 0; k < 10; k++)
							{
								dir->child[i]->fcb[k] = NULL;
								dir->child[i]->child[k] = NULL;
							}
							dir->fcb[i]->firstBlock = new PBC;//物理块控制结构体
							for (int j = 0; j < 1024; j++)//块的最大限制为1024个
							{
								if (Map[j] == false)
								{
									Map[j] = true;
									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//块号
									dir->fcb[i]->pbNUmber = j;//目录块号
									dir->fcb[i]->access[0].read = true;
									dir->fcb[i]->access[0].write = true;
									dir->fcb[i]->access[1].read = true;
									dir->fcb[i]->access[1].write = true;
									dir->fcb[i]->access[2].read = true;
									dir->fcb[i]->createTime = fileInfo.time_create;//创建时间初始化

									//void showDate(struct tm * p);
									//showDate(localtime(&fileInfo.time_create));//测试
									//std::cout << std::endl;

									dir->fcb[i]->fileName = fileInfo.name;//文件名
									dir->fcb[i]->fileType = "dir";
									dir->fcb[i]->owner = &UFD[index];//指向用户
									dir->fcb[i]->groupOwner = &UFD[index];//占时用户组只是自身，待chmod切换用户组权限
									dir->fcb[i]->fileSize = calcDirSize(dir->filePath + "\\" + fileInfo.name);//计算目录大小
									dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//文件路径

									dir->child[i]->fileName = fileInfo.name;//文件名
									dir->child[i]->filePath = dir->filePath + "\\" + fileInfo.name;//文件路径
									dir->child[i]->owner = &UFD[index];//指向用户
									dir->child[i]->groupOwner = &UFD[index];//占时用户组只是自身，待chmod切换用户组权限
									dir->child[i]->parent = dir;//指向父目录
									dir->child[i]->pbNUmber = j;//目录块号
									dir->child[i]->dirFcb = dir->fcb[i];//指向目录自己的FCB

									break;//因为目录只需要一个块，直接退出就可
								}
							}

							//std::cout << dir->fcb[i]->fileName << std::endl;
							///////////////////////////////////////////递归处理子目录/////////////////////////////
							initUserLevelSystemTree(dir->child[i], index, UFD, Map);
						}
						else//是文件
						{
							dir->fcb[i] = new FCB;//分配一个FCB
							dir->fcb[i]->firstBlock = new PBC;//物理块控制结构体
							//权限664
							dir->fcb[i]->access[0].read = true;
							dir->fcb[i]->access[0].write = true;
							dir->fcb[i]->access[1].read = true;
							dir->fcb[i]->access[1].write = true;
							dir->fcb[i]->access[2].read = true;
							dir->fcb[i]->createTime = fileInfo.time_create;//创建时间初始化
							dir->fcb[i]->fileName = fileInfo.name;//文件名
							dir->fcb[i]->fileType = "-";
							dir->fcb[i]->owner = &UFD[index];//指向用户
							dir->fcb[i]->groupOwner = &UFD[index];//占时用户组只是自身，待chmod切换用户组权限
							double sizeKB = calcFileSize(dir->filePath + "\\" + fileInfo.name);//计算文件大小
							dir->fcb[i]->fileSize = sizeKB;
							dir->fcb[i]->filePath = dir->filePath + "\\" + fileInfo.name;//文件路径

							//分配块

							int count = 1;
							if (sizeKB > count)//如果有余数，需要多分配一个块
							{
								count++;
							}
							int flag = 0;//判断是否是分配的第一个块
							for (int j = 0; j < 1024; j++)//块的最大限制为1024个
							{
								if (Map[j] == false)
								{
									Map[j] = true;
									if (flag == 0)
									{
										dir->fcb[i]->pbNUmber = j;//文件块号
									}
									flag = 1;
									count--;
									dir->fcb[i]->firstBlock->blockIndexList[dir->fcb[i]->firstBlock->index++] = j;//块号
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
		_findclose(handHandle);//关闭句柄
	}


}
/**
* @brief 菜单
*/
void help()
{
	std::cout << std::endl;
	std::cout << "*******************欢迎使用linux文件系统*******************" << std::endl;
	std::cout << std::endl;
	std::cout << "        命令                      说明                      " << std::endl;
	std::cout << "        login                     登录                      " << std::endl;
	std::cout << "        su 用户名                 切换用户                  " << std::endl;
	std::cout << "        whoami/id                 当前用户名                  " << std::endl;
	std::cout << "        cd ..                     切换到上一个路径              " << std::endl;
	std::cout << "        cd 目录                   切换到上一个路径              " << std::endl;
	std::cout << "        cd ~                      切换到用户根目录路径               " << std::endl;
	std::cout << "        pwd                       显示当前路径              " << std::endl;
	std::cout << "        ls                        展示所有文件名              " << std::endl;
	std::cout << "        ls 目录名                  显示目录路径下的所有文件名              " << std::endl;
	std::cout << "        ls -l                     显示所有文件信息              " << std::endl;
	std::cout << "        ls -l 文件名               显示文件信息              " << std::endl;
	std::cout << "        ls -l 目录名               展示目录下所有文件信息              " << std::endl;
	std::cout << "        ll                        显示当前路径下所有文件信息              " << std::endl;
	std::cout << "        ll 文件名                  显示当前文件信息              " << std::endl;
	std::cout << "        ll 目录名                  显示目录下所有文件信息              " << std::endl;
	std::cout << "        touch 文件名              创建文件                  " << std::endl;
	std::cout << "        mkdir 目录名              创建目录                  " << std::endl;
	std::cout << "        find 文件名               递归地查找文件            " << std::endl;
	std::cout << "        rm 文件名                 删除文件                  " << std::endl;
	std::cout << "        rm -r 目录名              递归地删除目录及其文件    " << std::endl;
	std::cout << "        cp 文件名 新文件名         复制文件                  " << std::endl;
	std::cout << "        cp -r 目录名 新目录名      递归复制文件夹                  " << std::endl;
	std::cout << "        mv 文件名 目录名           移动文件到目录中        " << std::endl;
	std::cout << "        mv 文件名 ..              移动文件到上一级目录下         " << std::endl;
	std::cout << "        mv 目录名 目录名           移动目录到另一个目录         " << std::endl;
	std::cout << "        mv 目录名 ..              移动目录到上一级目录下      " << std::endl;
	std::cout << "        cat 文件名                显示文件中的所有内容      " << std::endl;
	std::cout << "        head 文件名               显示文件中头几行的内容      " << std::endl;
	std::cout << "        head -num 文件名          显示文件中头num行的内容      " << std::endl;
	std::cout << "        tail 文件名               显示文件中尾几行内容      " << std::endl;
	std::cout << "        tail -num 文件名          显示文件中尾num行内容      " << std::endl;
	std::cout << "        chmod (644) 文件名/目录名 切换文件权限              " << std::endl;
	std::cout << "        useradd 用户名            添加用户[默认密码为用户名]              " << std::endl;
	std::cout << "        passwd                    修改当前用户的密码              " << std::endl;
	std::cout << "        passwd 用户名             修改指定用户的密码              " << std::endl;
	std::cout << "        userdel -r 用户名         删除用户即其数据              " << std::endl;
	std::cout << "        vim 文件名                进入仿vim编辑器进行文本编辑              " << std::endl;

	std::cout << std::endl;
	std::cout << "*******************欢迎使用linux文件系统*******************" << std::endl;
	std::cout << std::endl;
}

/**
* @brief 二级文件系统（用户）初始化
*/
void initUserLevelSystem(std::string path, DIR* MFD, User UFD[], bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (MFD->child[i] != NULL)//也就是存在用户目录
		{
			// std::cout << "用户：" << MFD->child[i]->fileName << " 大小：" << MFD->fcb[i]->fileSize << "KB" << std::endl;
			// std::cout <<  MFD->child[i]->fileName << std::endl;
			// std::cout <<  MFD->child[i]->filePath << std::endl;
			// std::cout << MFD->child[i]->parent->fileName << std::endl;
			// std::cout << MFD->child[i]->pbNUmber << std::endl;
			// std::cout << MFD->child[i]->owner->userDir->fileName << std::endl;
			//std::cout << std::endl;

			initUserLevelSystemTree(MFD->child[i], i, UFD, Map);//递归处理用户目录下的所有信息
		}
	}
}

/**
* @brief 颜色设置
*
*/
void setColor(WORD color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
/**
* @brief 显示文件起始日期
*/
void showDate(long long createTime)
{
	struct tm * p = localtime(&createTime);
	printf("%d/%d/%d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

/**
* @brief 显示文件内容
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
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//设置蓝色字体
		std::cout << fcb->fileName << "\t";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置默认颜色
	}
	else
	{
		std::cout << fcb->fileName << "\t";
	}
	std::cout << fcb->filePath << std::endl;
}

/**
* @brief 显示所用文件信息
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
* @brief 显示文件名
*/
void lsName(DIR* dir)
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//设置蓝色字体
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
* @brief 排除window的前缀，为转换为linux路径做准备
*/
std::string excludePathPrefix(const std::string& fullPath, const std::string& prefixToExclude)
{
	// 检查前缀是否存在于完整路径中
	if (fullPath.find(prefixToExclude) == 0)
	{
		// 计算排除前缀后的剩余部分
		size_t prefixLength = prefixToExclude.length();
		return fullPath.substr(prefixLength);
	}
	else
	{
		// 如果前缀不存在于完整路径中，则返回原始路径
		return fullPath;
	}
}

/**
* @brief 将window路径转换为当前用户的linux路径
*/
std::string convertWinToLinux(std::string winPath, std::string userName, std::string homePath)
{
	if (userName == "root")
	{
		// 要排除的前缀
		std::string prefixToExclude = homePath.substr(0, homePath.find_last_of("\\"));
		// 调用函数排除前缀
		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);

		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//将路径中的反斜杠替换为斜杠
		return linuxPath;

	}
	else
	{
		// 要排除的前缀
		std::string prefixToExclude = homePath + "\\" + userName;

		// 调用函数排除前缀
		std::string linuxPath = excludePathPrefix(winPath, prefixToExclude);//其他用户现对相对路径

		std::replace(linuxPath.begin(), linuxPath.end(), '\\', '/');//将路径中的反斜杠替换为斜杠
		return linuxPath;
	}

}

/**
* @brief 登录
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
			///////////////////////////////////保存文本,读取多了" "也会有影响////////////////
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

					num = i;//第几个用户
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
			std::cout << "operation error， please login!" << std::endl;
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
* @brief 查找路径下对应目录
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
* @brief 递归删除文件夹下的文件
*/
void rm_r(DIR* dir, bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				rm_r(dir->child[i], Map);//递归删除子目录，先递归到底删除文件，再考虑删除目录

				//删除目录
				////////////////////////////删除目录记录的块////////////////
				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
				{
					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
				}
				//删除目录
				if (!RemoveDirectoryA((dir->fcb[i]->filePath).c_str()))//删除目录
				{
					std::cout << "删除失败!" << std::endl;
				}
				//删除目录的记录
				delete dir->fcb[i];
				delete dir->child[i];
				dir->fcb[i] = NULL;
				dir->child[i] = NULL;
			}
			else
			{
				//删除文件
				////////////////////////////删除文件记录的块////////////////
				for (int j = 0; j < dir->fcb[i]->firstBlock->index; j++)
				{
					Map[dir->fcb[i]->firstBlock->blockIndexList[j]] = false;
				}
				//删除真实文件
				if (!DeleteFileA((dir->fcb[i]->filePath).c_str()))//删除文件
				{
					std::cout << "删除失败!" << std::endl;
				}
				//删除文件系统的记录
				delete dir->fcb[i];
				dir->fcb[i] = NULL;
			}
		}

	}
}

/**
* @brief 重新设置大小，不打算重新计算文件大小，就重新计算机目录大小
* @param dir 目录
* @param index 上一层目录对应当下目录的索引
*/
double resetSize(DIR *dir, int index)
{
	dir->parent->fcb[index]->fileSize = 0;//目录对应的fcb的大小进行重置,开始计算大小
	for (int i = 0; i < 10; i++)
	{
		if (dir->fcb[i] != NULL)
		{
			if (dir->fcb[i]->fileType == "dir")
			{
				dir->parent->fcb[index]->fileSize += resetSize(dir->child[i], i);//目录对应的fcb的大小进行重置
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
* brief 递归复制目录及以下文件
*/
void cp_r(DIR* src, DIR* dst, bool Map[])
{
	for (int i = 0; i < 10; i++)
	{
		if (src->fcb[i] != NULL)//有文件或目录
		{
			if (src->fcb[i]->fileType == "dir")
			{
				///////////////////初始化目录信息////////////////////////
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

				///////////////////文件系统中分配块 ////////////////////////
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
				///////////////////创建现实中的文件夹/////////////////////
				if (!CreateDirectory((dst->fcb[i]->filePath).c_str(), NULL))
				{
					std::cout << "创建失败！" << std::endl;
					continue;
				}

				///////////////////递归复制子目录////////////////
				cp_r(src->child[i], dst->child[i], Map);

			}
			else
			{
				////////////////////////////////////////////////////初始化文件信息//////////////////////////////////////////

				///////////////////////////////////////////////////////////////cp函数可优化处，待后续有时间进行函数封装优化///////////////////////////////
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
				/////////////////////////////////////文件系统中分配块///////////////////////////////////
				int count = src->fcb[i]->firstBlock->index;
				int flag = 0;//判断是否是分配的第一个块
				for (int j = 0; j < 1024; j++)
				{
					if (Map[j] == false)
					{
						Map[j] = true;
						if (flag == 0)
						{
							dst->fcb[i]->pbNUmber = j;//第一个块号
							flag = 1;
						}
						count--;
						dst->fcb[i]->firstBlock->blockIndexList[dst->fcb[i]->firstBlock->index++] = j;//块号
					}
					if (count == 0)
					{
						break;
					}
				}
				//////////////////////////////////////////计算机中复制文件////////////////////////////////
				CopyFile((src->fcb[i]->filePath).c_str(), (dst->fcb[i]->filePath).c_str(), false);//复制文件
			}
		}

	}
}

/**
* @brief 根据不同用户显示不同路径,提示符
*/
void showCommandLine(DIR* userDir, std::string nowPath, std::string userName, std::string homePath)
{
	if (userDir->fileName == "root")//如果切换为root用户，root@localhost:/home/user1# 路径是蓝色
	{
		//std::cout<< "root" << "@localhost";
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//设置绿色字体
		std::cout << "root@localhost";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置默认颜色
		std::cout << ":";
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//设置蓝色字体
		std::cout << convertWinToLinux(nowPath, userName, homePath);
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "# ";
	}
	else
	{
		//user1@localhost: ~$ 路径是蓝色
		setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);//设置绿色字体
		std::cout << userName << "@localhost";
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置默认颜色
		std::cout << ": ";
		setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);//设置蓝色字体
		std::cout << "~";
		std::cout << convertWinToLinux(nowPath, userName, homePath);
		setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置默认颜色
		std::cout << "$ ";
	}
}

/**
* @brief 计算文本行数
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
		std::cout << "no such file！" << std::endl;
	}
	in.close();
	return count;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////单位基础功能函数///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////ls//////////////////////////////////////////////////////////////////////
/**
* @brief 显示指定目录的名字
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
* @brief 显示指定目录内文件信息或单个文件信息
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
* @brief 显示对应知道目录或文件的所有详细信息
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
* @brief 创建文件功能
*/
bool touchF(DIR* now, DIR* userDir, bool Map[], std::string second)
{
	int exis = -1;
	////////////////////////判断文件是否存在/////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second)
		{
			exis = 1;
			std::cout << "文件已存在！" << std::endl;
			break;
		}
	}
	if (exis == 1)//文件已存在
	{
		return 0;
	}

	////////////////////////判断是否还有空闲位置///////////////////
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
		std::cout << "文件已满！" << std::endl;
		return 0;
	}

	//计算机中创建文件
	std::ofstream File;
	File.open(now->filePath + "\\" + second);
	File.close();


	/////////////////////////////////////一下文件分配有重复代码，可以优化先放着/////////////////////////////////////
	//在文件系统声明
	now->fcb[empty] = new FCB;//分配一个FCB
	now->fcb[empty]->firstBlock = new PBC;//物理块控制结构体

	now->fcb[empty]->access[0].read = true;
	now->fcb[empty]->access[0].write = true;
	now->fcb[empty]->access[1].read = true;
	now->fcb[empty]->access[1].write = true;
	now->fcb[empty]->access[2].read = true;
	now->fcb[empty]->createTime = time(0);//创建时间
	now->fcb[empty]->fileName = second;//文件名
	now->fcb[empty]->fileType = "-";
	now->fcb[empty]->owner = userDir->owner;//指向用户
	now->fcb[empty]->groupOwner = userDir->owner;//指向用户
	double sizeKB = 0;//计算文件大小
	now->fcb[empty]->fileSize = sizeKB;
	now->fcb[empty]->filePath = now->filePath + "\\" + second;//文件路径
	//分配块
	int count = 1;
	if (sizeKB > count)//如果有余数，需要多分配一个块
	{
		count++;
	}
	int flag = 0;//判断是否是分配的第一个块
	for (int j = 0; j < 1024; j++)//块的最大限制为1024个
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			if (flag == 0)
			{
				now->fcb[empty]->pbNUmber = j;//文件块号
			}
			flag = 1;
			count--;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//块号
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
* @brief 创建文件夹功能
*/
bool mkdirF(DIR* now, DIR* userDir, bool Map[], std::string second)
{
	int exis = -1;
	////////////////////////判断目录是否存在/////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second)
		{
			exis = 1;
			std::cout << "文件已存在！" << std::endl;
			break;
		}
	}
	if (exis == 1)//文件已存在
	{
		return 0;
	}
	////////////////////////判断是否还有空闲位置///////////////
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
		std::cout << "文件已满！" << std::endl;
		return 0;
	}
	//创建文件夹
	if (!CreateDirectory((now->filePath + "\\" + second).c_str(), NULL))
	{
		std::cout << "创建失败！" << std::endl;
		return 0;
	}
	//////////////////////////以下初始化目录有重复代码，可以优化先放着///////////////////////////
	now->fcb[empty] = new FCB;//分配一个FCB
	now->child[empty] = new DIR;//分配一个子目录
	for (int j = 0; j < 10; j++)
	{
		now->child[empty]->fcb[j] = NULL;
		now->child[empty]->child[j] = NULL;
	}
	now->fcb[empty]->firstBlock = new PBC;//物理块控制结构体
	for (int j = 0; j < 1024; j++)//块的最大限制为1024个
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//块号
			now->fcb[empty]->pbNUmber = j;//目录块号
			now->fcb[empty]->access[0].read = true;
			now->fcb[empty]->access[0].write = true;
			now->fcb[empty]->access[1].read = true;
			now->fcb[empty]->access[1].write = true;
			now->fcb[empty]->access[2].read = true;
			now->fcb[empty]->createTime = time(0);//创建时间初始化
			now->fcb[empty]->fileName = second;//文件名
			now->fcb[empty]->fileType = "dir";
			now->fcb[empty]->owner = userDir->owner;//指向用户
			now->fcb[empty]->groupOwner = userDir->owner;//指向用户
			now->fcb[empty]->fileSize = 0;//目录大小
			now->fcb[empty]->filePath = now->filePath + "\\" + second;//文件路径

			now->child[empty]->fileName = second;//文件名
			now->child[empty]->filePath = now->filePath + "\\" + second;//文件路径
			now->child[empty]->owner = userDir->owner;//指向用户
			now->child[empty]->groupOwner = userDir->owner;//指向用户
			now->child[empty]->parent = now;//指向父目录
			now->child[empty]->pbNUmber = j;//目录块号
			now->child[empty]->dirFcb = now->fcb[empty];//指向目录FCB
			break;//因为目录只需要一个块，直接退出就可
		}
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////rm///////////////////////////////////////////////////////////////////////////////
/**
* @brief 删除文件
*/
bool rmF(DIR* now, DIR* userDir, bool Map[], std::string second_2)
{
	//std::cout << "file" << std::endl;
	////////////////////////判断文件是否存在//////////////////////////////////////////////////////
	int exist = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2)
		{
			exist = i;
			break;
		}
	}
	if (exist == -1)//文件不存在
	{
		return 0;
	}
	if (now->fcb[exist]->fileType == "dir")
	{
		return 0;
	}
	////////////////////////删除文件块/////////////////////////////////////////////////////////////
	if (!DeleteFileA((now->fcb[exist]->filePath).c_str()))//删除文件
	{
		std::cout << "删除失败!" << std::endl;
		return 0;
	}
	//除去文件所占用的块
	for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
	{
		Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//将块置为未分配
	}
	////////////////////////删除文件系统的///////////////
	delete (now->fcb[exist]);//释放FCB
	now->fcb[exist] = NULL;//将当前目录下这个文件设置为NULL
	/////////////////////////////////////////////新的递归重新计算大小////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
		}
	}
	return 1;
}

/**
* @brief 删除文件夹
*/
bool rm_rF(DIR* now, DIR* userDir, bool Map[], std::string second_2)
{
	//std::cout << "-r file" << std::endl;
	////////////////////////判断目录是否存在/////////////////
	int exist = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->child[i] != NULL && now->child[i]->fileName == second_2)
		{
			exist = i;
			break;
		}
	}
	if (exist == -1)//目录不存在
	{
		return 0;
	}

	//除去文件夹所占用的块
	for (int i = 0; i < now->fcb[exist]->firstBlock->index; i++)
	{
		Map[now->fcb[exist]->firstBlock->blockIndexList[i]] = false;//将块置为未分配
	}
	///////////////////递归删除目录即子目录以下的文件///////////////
	rm_r(now->child[exist], Map);
	////////////////////////删除目录/////////////////
	if (!RemoveDirectoryA((now->fcb[exist]->filePath).c_str()))//删除目录
	{
		std::cout << "删除失败!" << std::endl;
		return 0;
	}

	////////////////////////删除文件系统中的信息////////////  
	delete now->fcb[exist];//释放当前目录的FCB
	delete now->child[exist];//释放当前目录的目录
	now->fcb[exist] = NULL;//将当前目录下这个文件设置为NULL
	now->child[exist] = NULL;//将当前目录下这个文件设置为NULL

	///////////////////////////////////////////新的递归重新计算大小:可考虑到/根目录下的操作，还可以升级////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////find//////////////////////////////////////////////////////////////////////////////
/**
* @brief 查找对应文件名的路径信息
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
* @brief 复制文件
*/
bool cpF(DIR* now, DIR* userDir, bool Map[], std::string second_1, std::string second_2)
{
	/////////////////////////判断文件是否存在////////////////////////////
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
		std::cout << "源文件不存在！" << std::endl;
		return 0;
	}
	
	////////////////////////找到是否有空闲位置//////////////////////
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
		std::cout << "文件已满！" << std::endl;
		return 0;
	}

	/////////////////////////////////////初始化复制文件的信息//////////////////////////////
	now->fcb[empty] = new FCB;//分配一个FCB
	now->fcb[empty]->firstBlock = new PBC;//物理块控制结构体
	///////////////////////////////////////以下代码可优化///////////////////////////////////////
	now->fcb[empty]->access[0].read = now->fcb[cpSource]->access[0].read;
	now->fcb[empty]->access[0].write = now->fcb[cpSource]->access[0].write;
	now->fcb[empty]->access[0].execute = now->fcb[cpSource]->access[0].execute;

	now->fcb[empty]->access[1].read = now->fcb[cpSource]->access[1].read;
	now->fcb[empty]->access[1].write = now->fcb[cpSource]->access[1].write;
	now->fcb[empty]->access[1].execute = now->fcb[cpSource]->access[1].execute;

	now->fcb[empty]->access[2].read = now->fcb[cpSource]->access[2].read;
	now->fcb[empty]->access[2].write = now->fcb[cpSource]->access[2].write;
	now->fcb[empty]->access[2].execute = now->fcb[cpSource]->access[2].execute;

	now->fcb[empty]->createTime = time(0);//创建时间
	now->fcb[empty]->fileName = second_2;//文件名
	now->fcb[empty]->filePath = now->filePath + "\\" + second_2;//文件路径
	now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//文件大小
	now->fcb[empty]->fileType = now->fcb[cpSource]->fileType;//文件类型
	now->fcb[empty]->groupOwner = userDir->owner;//指向用户
	now->fcb[empty]->owner = userDir->owner;//指向用户

	/////////////////////////////////分配块///////////////////////////////////////
	int count = now->fcb[cpSource]->firstBlock->index;
	int flag = 0;//判断是否是分配的第一个块
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			if (flag == 0)
			{
				now->fcb[empty]->pbNUmber = j;//第一个块号
				flag = 1;
			}
			count--;
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//块号
		}
		if (count == 0)
		{
			break;
		}
	}
	/////////////////////////////////复制文件////////////////////////////////////////
	CopyFile((now->fcb[cpSource]->filePath).c_str(), (now->fcb[empty]->filePath).c_str(), false);//复制文件

	///////////////////////////////////////////新的重新计算大小:可考虑到/根目录下的操作，还可以升级////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
		}
	}
	return 1;
}

/**
* @brief 复制文件夹
*/
bool cp_rF(DIR* now, DIR* userDir, bool Map[], std::string second_2_1, std::string second_2_2)
{
	/////////////////////////判断目录是否存在////////////////////////////
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
		std::cout << "源目录不存在！" << std::endl;
		return 0;
	}

	////////////////////////找到是否有空闲位置/////////////////////////
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
		std::cout << "文件已满！" << std::endl;
		return 0;
	}
	////////////////////////////////复制文件夹:目标second_2_1 生成second_2_2////////////////////////////////////////
	now->fcb[empty] = new FCB;//分配一个FCB
	now->fcb[empty]->firstBlock = new PBC;//物理块控制结构体
	now->child[empty] = new DIR;//分配一个子目录
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

	now->fcb[empty]->createTime = now->fcb[cpSource]->createTime;//创建时间
	now->fcb[empty]->fileName = second_2_2;//文件名
	now->fcb[empty]->filePath = now->filePath + "\\" + second_2_2;//文件路径
	now->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//文件大小
	now->fcb[empty]->fileType = "dir";//文件类型
	now->fcb[empty]->groupOwner = userDir->owner;//指向用户
	now->fcb[empty]->owner = userDir->owner;//指向用户

	now->child[empty]->fileName = second_2_2;//文件名
	now->child[empty]->filePath = now->filePath + "\\" + second_2_2;//文件路径
	now->child[empty]->groupOwner = userDir->owner;//指向用户
	now->child[empty]->owner = userDir->owner;//指向用户
	now->child[empty]->parent = now;//指向父目录
	now->child[empty]->dirFcb = now->fcb[empty];//指向目录FCB
	//////////////////////////////////////给目录分配块////////////////////////////////
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			now->fcb[empty]->pbNUmber = j;//第一个块号
			now->fcb[empty]->firstBlock->blockIndexList[now->fcb[empty]->firstBlock->index++] = j;//块号
			now->child[empty]->pbNUmber = j;//第一个块号
			break;
		}
	}
	///////////////////////////////////新建目录/////////////////////////////////////////
	if (!CreateDirectory((now->fcb[empty]->filePath).c_str(), NULL))
	{
		std::cout << "目录创建失败！" << std::endl;
		return 0;
	}
	//////////////////////////////////////开始递归复制目录下所有信息和文件//////////////////////////////
	cp_r(now->child[cpSource], now->child[empty], Map);

	///////////////////////////////////////////新的重新计算大小:可考虑到/根目录下的操作，还可以升级////////////////////////////////////////////////////////////
	for (int i = 0; i < 10; i++)
	{
		if (userDir->child[i] != NULL)
		{
			resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
		}
	}
	return 1;
}

/**
* @brief 复制文件夹到任意位置
*/
bool cp_rAnyDir(DIR* now, DIR* aim,DIR* userDir, bool Map[], std::string second_2_1, std::string second_2_2)
{
	/////////////////////////判断目录是否存在////////////////////////////
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
		std::cout << "源目录不存在！" << std::endl;
		return 0;
	}

	////////////////////////找到是否有空闲位置/////////////////////////
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
		std::cout << "文件已满！" << std::endl;
		return 0;
	}
	////////////////////////////////复制文件夹:目标second_2_1 生成second_2_2////////////////////////////////////////
	aim->fcb[empty] = new FCB;//分配一个FCB
	aim->fcb[empty]->firstBlock = new PBC;//物理块控制结构体
	aim->child[empty] = new DIR;//分配一个子目录
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

	aim->fcb[empty]->createTime = now->fcb[cpSource]->createTime;//创建时间
	aim->fcb[empty]->fileName = second_2_2;//文件名
	aim->fcb[empty]->filePath = aim->filePath + "\\" + second_2_2;//文件路径
	aim->fcb[empty]->fileSize = now->fcb[cpSource]->fileSize;//文件大小
	aim->fcb[empty]->fileType = "dir";//文件类型
	aim->fcb[empty]->groupOwner = userDir->owner;//指向用户
	aim->fcb[empty]->owner = userDir->owner;//指向用户

	aim->child[empty]->fileName = second_2_2;//文件名
	aim->child[empty]->filePath = aim->filePath + "\\" + second_2_2;//文件路径
	aim->child[empty]->groupOwner = userDir->owner;//指向用户
	aim->child[empty]->owner = userDir->owner;//指向用户
	aim->child[empty]->parent = aim;//指向父目录
	aim->child[empty]->dirFcb = now->fcb[empty];//指向目录FCB
	//////////////////////////////////////给目录分配块////////////////////////////////
	for (int j = 0; j < 1024; j++)
	{
		if (Map[j] == false)
		{
			Map[j] = true;
			aim->fcb[empty]->pbNUmber = j;//第一个块号
			aim->fcb[empty]->firstBlock->blockIndexList[aim->fcb[empty]->firstBlock->index++] = j;//块号
			aim->child[empty]->pbNUmber = j;//第一个块号
			break;
		}
	}
	///////////////////////////////////新建目录/////////////////////////////////////////
	if (!CreateDirectory((aim->fcb[empty]->filePath).c_str(), NULL))
	{
		std::cout << "目录创建失败！" << std::endl;
		return 0;
	}
	//////////////////////////////////////开始递归复制目录下所有信息和文件//////////////////////////////
	cp_r(now->child[cpSource], aim->child[empty], Map);

	return 1;
}
////////////////////////////////////////////////////////////////////////////////////mv////////////////////////////////////////////////////////////////
/**
* @brief 移动文件到文件夹
*/
void mvF(DIR* now, int mvSource, int exist, int empty)
{
	////////////////////////////////////////////计算机中移动文件////////////////////////////////////////
	MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->child[exist]->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//移动夹

	now->child[exist]->fcb[empty] = now->fcb[mvSource];//将文件移动到目标文件夹
	now->fcb[mvSource] = NULL;//将源文件设置为NULL
	/////////////////////////////////////////更新目录路径信息/////////////////////////////////
	now->child[exist]->fcb[empty]->filePath = now->child[exist]->filePath + "\\" + now->child[exist]->fcb[empty]->fileName;//更新文件路径
}

/**
* @brief 移动文件到上一级目录
*/
void mv_F(DIR* now, int mvSource, int empty)
{
	////////////////////////////////////////////计算机中移动文件////////////////////////////////////////
	MoveFile(now->fcb[mvSource]->filePath.c_str(), (now->parent->filePath + "\\" + now->fcb[mvSource]->fileName).c_str());//移动夹

	now->parent->fcb[empty] = now->fcb[mvSource];//将文件移动到目标文件夹
	now->fcb[mvSource] = NULL;//将源文件设置为NULL
	/////////////////////////////////////////更新目录路径信息/////////////////////////////////
	now->parent->fcb[empty]->filePath = now->parent->filePath + "\\" + now->parent->fcb[empty]->fileName;//更新文件路径
}
////////////////////////////////////////////////////////////////////////////////chmod//////////////////////////////////////////////////////////////
/**
* @brief 切换文件权限
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
	//修改权限
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
* @brief cat读文件
*/
int cat(std::string filePath)
{
	SetConsoleOutputCP(65001);//设置输出编码为UTF-8
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
	SetConsoleOutputCP(936);//设置编码为GBK
	return count;
}
/**
* @brief head读文件头num行
*/
int head(std::string filePath, int num)
{
	SetConsoleOutputCP(65001);//设置编码为UTF-8
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in) // 有该文件
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
	else // 没有该文件
	{
		std::cout << "no such file" << std::endl;
	}

	in.close();
	SetConsoleOutputCP(936);//设置编码为GBK

	return count;
}
/**
* @brief tail读取文件尾num行
*/
int tail(std::string filePath, int num)
{
	SetConsoleOutputCP(65001);//设置编码为UTF-8
	std::ifstream in(filePath);
	std::string line;
	int count = 0;
	if (in) // 有该文件
	{
		int sumNum = getfileLineNum(filePath);//获取行数
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
	else // 没有该文件
	{
		std::cout << "no such file" << std::endl;
	}

	in.close();
	SetConsoleOutputCP(936);//设置编码为GBK

	return count;
}
///////////////////////////////////////////////////////////////////////////////useradd////////////////////////////////////////////////////////////////
/**
* @brief 添加用户
* @return -1 没空位， -2 已存在，-3 创建失败.-4 文件系统没空位，一般不可能发生
*/
int addUser(std::string homePath, std::string passwdPath, DIR* MFD,User UFD[], bool Map[], std::string name, std::string password)
{
	for (int i = 0; i < 10; i++)//查找用户是否已存在
	{
		if (UFD[i].userDir != NULL && UFD[i].userName == name)
		{
			return -2;
		}
	}
	int emptyUser = -1;
	for (int i = 0; i < 10; i++)//检查是否还有空位存放用户
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
	//创建文件夹
	if (!CreateDirectory((homePath + "\\" + name).c_str(), NULL))
	{
		std::cout << "创建失败！" << std::endl;
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

	intptr_t handleFile = 0;//文件句柄
	struct _finddata_t fileInfo;//文件信息结构体
	if ((handleFile = _findfirst((homePath + "\\" + name).c_str(), &fileInfo)) != -1)
	{
		MFD->fcb[emptyFile] = new FCB;//分配一个fcb
		//为用户目录分配块和基本信息，因为目录块只有一个
		for (int j = 0; j < 1024; j++)
		{
			if (Map[j] == false)
			{

				Map[j] = true;
				MFD->fcb[emptyFile]->firstBlock = new PBC;//物理块控制结构体
				MFD->fcb[emptyFile]->pbNUmber = j;//用户目录块号
				MFD->fcb[emptyFile]->firstBlock->blockIndexList[MFD->fcb[emptyFile]->firstBlock->index++] = j;//块号

				MFD->fcb[emptyFile]->fileName = fileInfo.name;//用户名
				MFD->fcb[emptyFile]->fileType = "dir";
				MFD->fcb[emptyFile]->access[0].read = true;
				MFD->fcb[emptyFile]->access[0].write = true;
				MFD->fcb[emptyFile]->access[1].read = true;
				MFD->fcb[emptyFile]->access[1].write = true;
				MFD->fcb[emptyFile]->access[2].read = true;
				MFD->fcb[emptyFile]->owner = &UFD[emptyUser];//指向自身
				MFD->fcb[emptyFile]->groupOwner = &UFD[emptyUser];//占时用户组只是自身，待chmod切换用户组权限
				MFD->fcb[emptyFile]->createTime = fileInfo.time_create;//创建时间初始化
				MFD->fcb[emptyFile]->filePath = homePath + "\\" + fileInfo.name;//文件路径

				//创建用户对应目录
				MFD->child[emptyFile] = new DIR;//分配一个用户目录
				for (int k = 0; k < 10; k++)
				{
					MFD->child[emptyFile]->fcb[k] = NULL;
					MFD->child[emptyFile]->child[k] = NULL;
				}
				MFD->child[emptyFile]->pbNUmber = j;//用户目录块号
				MFD->child[emptyFile]->parent = MFD;//将其设为空，为了接下来用户根的判断做准备
				MFD->child[emptyFile]->fileName = fileInfo.name;//用户名
				MFD->child[emptyFile]->filePath = homePath + "\\" + fileInfo.name;//用户目录下的路径
				MFD->child[emptyFile]->owner = &UFD[emptyUser];//指向用户
				MFD->child[emptyFile]->groupOwner = &UFD[emptyUser];//占时用户组只是自身，待chmod切换用户组权限
				MFD->child[emptyFile]->dirFcb = MFD->fcb[emptyFile];

				UFD[emptyUser].userDir = MFD->child[emptyFile];////初始化用户的指向自己用户目录

				break;
			}
		}
		_findclose(handleFile);//关闭句柄
	}
	else
	{
		std::cout << "cannot find user file！" << std::endl;
	}
	MFD->fcb[emptyFile]->fileSize = 0;//刚创建用户
	///////////////////////////passwd文本信息进行添加/////////////////
	std::ofstream ofs;//创建流对象
	ofs.open(passwdPath, std::ios::app);//追加app
	std::string line = name + ":" + password;
	ofs << line << "\n";
	ofs.close();//关闭文件

	return emptyUser;
}
