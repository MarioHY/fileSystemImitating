#include "Util.h"
#include "vim.h"
std::string homePath = ".\\home";//文件系统根目录
std::string passwdPath = ".\\etc\\passwd.txt";//用户信息文件路径
std::string groupPath = ".\\etc\\group.txt";//用户信息文件路径
//char* Blocks = (char*)malloc(1024 * 1024 * sizeof(char));//磁盘块大小为1024个1kB块
User UFD[10];//用户信息数组加二级文件系统
bool Map[1024] = { false };//块的分布情况
//DIR *ROOT = new DIR();//根目录/,有空升级
DIR *MFD = new DIR();//一级文件系统,也就是home目录的存放的


////////////////////////////////////////////////////////////////////////////////初始化函数////////////////////////////////////////////////////////////////////////////
/**
* @brief 初始化文件系统
* @param initFirstLevelSystem 初始化一级文件系统
*/
void init(std::string path)
{
	//一下初始化操作都是对加载文件系统是对应路径已有的文件信息初始化
	initFirstLevelSystem(path, UFD, MFD, passwdPath, Map);
	initUserLevelSystem(path, MFD, UFD, Map);
}


/////////////////////////////////////////////////////////////////////////////功能函数封装//////////////////////////////////////////////////////////////
/**
* @brief 所有的ls功能封装
*/
void LS(DIR* now,std::string first, std::string second)
{
	if (second != first)
	{
		size_t pos = second.find_first_of(' ');
		std::string second_1 = second.substr(0, pos);
		std::string second_2 = second.substr(pos + 1);
		if (second_1 == second_2 && second_1 != "-l")
		{
			lsLocalNameF(now, second_2);//显示指定目录下的所有名字,ls 目录名
		}
		else if (second_1 == "-l" && second_2 != "-l")
		{
			ls_lAnyTypeF(now, second_2);//显示指定目录内文件信息或单个文件信息,ls -l 目录名/文件名
		}
		else if (second_1 != "-l")
		{
			//无效操作
		}
		else
		{
			lsAll(now);//显示当前目录下所有详细信息, ls -l
		}
	}
	else
	{
		lsName(now);//显示当前目录下的所有名字
	}
}

/**
* @brief 所有的ll功能封装
*/
void LL(DIR* now, std::string first, std::string second)
{
	if (second != first)
	{
		llAnyTypeF(now, second);//显示对应知道目录或文件的所有详细信息
	}
	else
	{
		lsAll(now);//显示当前目录下的所有信息
	}
}

/**
* @brief 所有的cd功能封装
*/
void CD(DIR*& now, DIR* userDir, std::string& nowPath, std::string second)
{
	if (second == "..")
	{
		if (now->parent->fileName == "home")
		{

		}
		else
		{
			now = now->parent;
			nowPath = now->filePath;
		}
	}
	else if (second == "~")
	{
		now = userDir;
		nowPath = userDir->filePath;
	}
	else
	{
		int find = -1;
		for (int i = 0; i < 10; i++)
		{
			if (now->child[i] != NULL)
			{
				if (now->child[i]->fileName == second)
				{
					now = now->child[i];
					nowPath = now->filePath;
					find = 1;
					break;
				}
			}
		}
		if (find == -1)
		{
			std::cout << "no such directory!" << std::endl;
		}
	}
}
/**
* @brief 返回用户的对应的Index
*/
int SU(User UFD[], std::string second)
{
	int flag = -1;
	for (int i = 0; i < 10; i++)
	{
		if (UFD[i].userDir != NULL && UFD[i].userName == second)
		{
			std::cout << "密码: ";
			std::string password;
			std::cin >> password;
			if (UFD[i].password == password)
			{
				flag = i;
			}
			else
			{
				std::cout << "密码错误！" << std::endl;
			}
			break;
		}
	}
	return flag;
}

/**
* @brief 所有rm功能封装
*/
void RM(DIR* now, DIR* userDir, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	//std::cout << "first:" << first << " second:" << second_1 << " third:" << second_2 << std::endl;
	if (second_1 == second_2)//如果是rm <filename>
	{
		rmF(now, userDir, Map, second_2);//删除文件
	}
	else if (second_1 == "-r" && second_2 != "-r")
	{
		rm_rF(now, userDir, Map, second_2);//删除文件夹
	}
}

/**
* @brief 所有cp函数封装
*/
void CP(DIR* now, DIR* userDir, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	//std::cout << "first:" << first << " second:" << second_1 << " third:" << second_2 << std::endl;
	if (second_1 != second_2)
	{
		if (second_1 != "-r")
		{
			cpF(now, userDir, Map, second_1, second_2);//复制文件
		}
		else
		{
			size_t pos_2 = second_2.find_first_of(' ');
			std::string second_2_1 = second_2.substr(0, pos_2);
			std::string second_2_2 = second_2.substr(pos_2 + 1);
			if (second_2_1 != second_2_2)
			{
				cp_rF(now, userDir, Map, second_2_1, second_2_2);//复制文件夹
			}
		}
	}
}
/**
* @brief 所mv功能封装
*/
void MV(DIR* now, DIR* userDir, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	if (second_1 != second_2)
	{
		int mvSource = -1;
		for (int i = 0; i < 10; i++)
		{
			if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_1)
			{
				mvSource = i;
				break;
			}
		}
		if (mvSource == -1)
		{
			std::cout << "源文件不存在！" << std::endl;
			return;
		}
		if (second_2 == "..")//移动到上级目录
		{
			if (now->parent->fileName != "home")
			{

				if (now->fcb[mvSource]->fileType == "dir")//如果是文件夹
				{
					//复制文件夹
					cp_rAnyDir(now, now->parent, userDir, Map, second_1, second_1);//复制文件夹
					//删除文件夹
					rm_rF(now, userDir, Map, second_1);
				}
				else//如果是文件
				{
					/////////////////////////////判断文件夹内是否还用空间可以存放/////////////////////////////
					int empty = -1;
					for (int i = 0; i < 10; i++)
					{
						if (now->parent->fcb[i] == NULL)
						{
							empty = i;
							break;
						}
					}
					if (empty == -1)
					{
						return;
					}
					mv_F(now, mvSource, empty);//移动文件到上一级目录
				}
				for (int i = 0; i < 10; i++)
				{
					if (userDir->child[i] != NULL)
					{
						resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
					}
				}
			}
		}
		else
		{
			////////////////////////判断文件夹是否存在////////////////////////////
			int exist = -1;
			for (int i = 0; i < 10; i++)
			{
				if (now->child[i] != NULL && now->child[i]->fileName == second_2)
				{
					exist = i;
					break;
				}
			}
			if (exist == -1)
			{
				return;
			}
			if (now->fcb[mvSource]->fileType == "dir")//如果是文件夹
			{
				//复制文件夹
				cp_rAnyDir(now, now->child[exist], userDir, Map, second_1, second_1);//复制文件夹
				//删除文件夹
				rm_rF(now, userDir, Map, second_1);
			}
			else//如果是文件
			{
				/////////////////////////////判断文件夹内是否还用空间可以存放/////////////////////////////
				int empty = -1;
				for (int i = 0; i < 10; i++)
				{
					if (now->child[exist]->fcb[i] == NULL)
					{
						empty = i;
						break;
					}
				}
				if (empty == -1)
				{
					return;
				}
				mvF(now, mvSource, exist, empty);//移动文件到文件夹
			}

			for (int i = 0; i < 10; i++)
			{
				if (userDir->child[i] != NULL)
				{
					resetSize(userDir->child[i], i);//重新计算目录大小，也就目录下所有文件大小和
				}
			}
		}
	}
}

/**
* @brief chmod
*/
void CHMOD(DIR* now, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);

	if (second_1 != second_2)
	{
		chmodF(now, second_1, second_2);
	}
}

/**
* @brief cat
*/
void CAT(DIR* now, std::string second)
{
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second && now->fcb[i]->fileType == "-")
		{
			cat(now->fcb[i]->filePath);//显示文件信息
			break;
		}
	}
}
/**
* @brief head
*/
void HEAD(DIR* now, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	if (second_1 == second_2)//head filename
	{
		for (int i = 0; i < 10; i++)
		{
			if (now->fcb[i] != NULL && now->fcb[i]->fileName == second && now->fcb[i]->fileType == "-")
			{
				head(now->fcb[i]->filePath, 5);//显示文件信息
				break;
			}
		}
	}
	else//判断是否 -num filename
	{
		if (second_1.length() < 1)
		{
			return;
		}
		std::string strNum = second_1.substr(1, second_1.length());
		//std::cout << second_1 << " " << second_2 << " " << num << std::endl;
		int flag = -1;
		for (int i = 0; i < strNum.length(); i++)
		{
			if (strNum[i] < '0' || strNum[i] > '9')
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			return;
		}
		int num = std::stoi(strNum.c_str());//将字符转数字
		//std::cout << num << std::endl;
		for (int i = 0; i < 10; i++)
		{
			if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2 && now->fcb[i]->fileType == "-")
			{
				head(now->fcb[i]->filePath, num);//显示文件信息
				break;
			}
		}
	}
}
/**
* @brief tail
*/
void TAIL(DIR* now, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	if (second_1 == second_2)//head filename
	{
		for (int i = 0; i < 10; i++)
		{
			if (now->fcb[i] != NULL && now->fcb[i]->fileName == second && now->fcb[i]->fileType == "-")
			{
				tail(now->fcb[i]->filePath, 5);//显示文件信息
				break;
			}
		}
	}
	else//判断是否 -num filename
	{
		if (second_1.length() < 1)
		{
			return;
		}
		std::string strNum = second_1.substr(1, second_1.length());
		//std::cout << second_1 << " " << second_2 << " " << num << std::endl;
		int flag = -1;
		for (int i = 0; i < strNum.length(); i++)
		{
			if (strNum[i] < '0' || strNum[i] > '9')
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			return;
		}
		int num = std::stoi(strNum.c_str());//将字符转数字
		//std::cout << num << std::endl;
		for (int i = 0; i < 10; i++)
		{
			if (now->fcb[i] != NULL && now->fcb[i]->fileName == second_2 && now->fcb[i]->fileType == "-")
			{
				tail(now->fcb[i]->filePath, num);//显示文件信息
				break;
			}
		}
	}
}
/**
* @brief 仿vim对文件操作
*/
void VIM(DIR* now, std::string second)
{
	int find = -1;
	for (int i = 0; i < 10; i++)
	{
		if (now->fcb[i] != NULL && now->fcb[i]->fileName == second && now->fcb[i]->fileType == "-")
		{
			find = i;
			break;
		}
	}
	if (find == -1)
	{
		return;
	}
	system("cls");
	Vim* vim = new Vim(now->fcb[find]->filePath);//进行仿vim编辑操作
	vim->start();
	double sizeKB = calcFileSize(now->fcb[find]->filePath);
	delete vim;
	now->fcb[find]->fileSize = sizeKB;
	//std::cout << byteNum << std::endl;
	//std::cout << sizeKB << std::endl;
	system("cls");
}
/**
* @brief useradd添加用户
*/
void USERADD(std::string second)
{
	if (second != "useradd")
	{
		size_t pos = second.find_first_of(' ');
		std::string second_1 = second.substr(0, pos);
		std::string second_2 = second.substr(pos + 1);
		int flag = -5;

		flag = addUser(homePath, passwdPath, MFD, UFD, Map, second_1, second_2);

		if (flag < 0)
		{
			std::cout << "Failed to add user!" << std::endl;
		}
		else
		{
			std::cout << "Add user successfully!" << std::endl;
		}
	}
}
/**
* @brief 修改密码
* @return -1 不存在该用户,-2 无法打开文件
*/
int PASSWD(DIR* userDir, std::string second)
{
	std::string name;
	std::string newPassword;
	std::cout << "请输入新的密码:";
	std::getline(std::cin, newPassword);

	std::ifstream file(passwdPath);
	std::string line;
	std::string userAll[10];
	std::string temp;
	if (second == "passwd")
	{
		name = userDir->owner->userName;
		userDir->owner->password = newPassword;
	}
	else
	{
		name = second;

		int findName = -1;
		for (int i = 0; i < 10; i++)
		{
			if (UFD[i].userDir != NULL && UFD[i].userName == name)
			{
				findName = i;
				break;
			}
		}
		if (findName == -1)
		{
			std::cout << "不存在该用户!" << std::endl;
			return -1;
		}
		UFD[findName].password = newPassword;

	}

	int n = 0;
	int find = 0;
	if (file)
	{
		while (std::getline(file, line))
		{
			userAll[n++] = line;
			temp = line;
			if (temp.substr(0, temp.find_first_of(":")) == name)
			{
				find = n - 1;
			}
		}
	}
	else
	{
		std::cout << "open file error！" << std::endl;
		return -2;
	}

	std::ofstream ofs;//创建流对象
	ofs.open(passwdPath, std::ios::out);//覆盖out
	//修改文本中的密码
	userAll[find].replace(userAll[find].find_first_of(":") + 1, userAll[find].size() - userAll[find].find_first_of(":") - 1, newPassword);
	for (int i = 0; i < n; i++)
	{
		ofs << userAll[i] << "\n";
	}

	ofs.close();//关闭文件
	return 1;
}
/**
* @brief 删除用户
* @return -1 不存在该用户,-2 取消删除用户， -3 删除目录失败,-4 打开文件失败
*/
int USERDEL(DIR* userDir, std::string second)
{
	size_t pos = second.find_first_of(' ');
	std::string second_1 = second.substr(0, pos);
	std::string second_2 = second.substr(pos + 1);
	int flag = 1;//返回问题的标志
	if (second_1 == "-r" && second_2 != second_1)
	{
		int findUser = -1;
		for (int i = 0; i < 10; i++)
		{
			if (UFD[i].userDir != NULL && UFD[i].userName == second_2)
			{
				findUser = i;
				break;
			}
		}
		if (findUser == -1)
		{
			std::cout << "不存在该用户!" << std::endl;
			flag = -1;
			return flag;
		}
		else
		{
			std::cout << "用户：" << second_2 << "的数据将全部丢失," << "确定要删除该用户？(yes/no)" << std::endl;
			std::string judge;
			std::getline(std::cin, judge);
			if (judge != "yes")
			{
				std::cout << "取消删除用户成功!" << std::endl;
				flag = -2;
				return flag;
			}
		}
		//////判断是否删除的是自身的用户///

		if (userDir->owner->userName == second_2)
		{
			flag = 0;
		}


		int findFcb = -1;
		for (int i = 0; i < 10; i++)
		{
			if (MFD->child[i] != NULL && MFD->child[i]->owner->userName == second_2)
			{
				findFcb = i;
				break;
			}
		}
		//除去文件夹占用的块
		for (int i = 0; i < MFD->fcb[findFcb]->firstBlock->index; i++)
		{
			Map[MFD->fcb[findFcb]->firstBlock->blockIndexList[i]] = false;//置为未分配
		}

		//递归删除该用户的目录及其子文件夹，到这里就只有自身处理
		rm_r(UFD[findUser].userDir, Map);
		//删除目录
		if (!RemoveDirectoryA((MFD->fcb[findFcb]->filePath).c_str()))
		{
			std::cout << "删除目录失败!" << std::endl;
			flag = -3;
			return flag;
		}

		//删除文件系统中的信息
		delete MFD->fcb[findFcb];
		delete MFD->child[findFcb];
		MFD->fcb[findFcb] = NULL;
		MFD->child[findFcb] = NULL;

		//删除User信息
		UFD[findUser].userName = "";
		UFD[findUser].password = "";
		UFD[findUser].userDir = NULL;
		
		//在文本中删除信息
		std::string name = second_2;
		std::ifstream file(passwdPath);
		std::string line;
		std::string userAll[10];
		std::string temp;

		int n = 0;
		int find = 0;
		if (file)
		{
			while (std::getline(file, line))
			{
				userAll[n++] = line;
				temp = line;
				if (temp.substr(0, temp.find_first_of(":")) == name)
				{
					find = n - 1;
				}
			}
		}
		else
		{
			std::cout << "open file error！" << std::endl;
			flag = -4;
			return flag;
		}

		std::ofstream ofs;//创建流对象
		ofs.open(passwdPath, std::ios::out);//覆盖out
		for (int i = 0; i < n; i++)
		{
			if (i == find)
			{
				continue;
			}
			ofs << userAll[i] << "\n";
		}

		ofs.close();//关闭文件

		return flag;
	}
}

/**
* @brief 用户控制操作
*/
int userControl(DIR* userDir, bool ifChangeUser)
{
	//路径与用户名
	std::string userName = userDir->owner->userName;//用户名
	std::string nowPath = userDir->filePath;//当前路径
	//目录指针
	DIR *now = userDir;//当前目录
	int flag = -1;//切换用户标志
	system("cls");
	while (1)
	{
		/////////////////////////////////////显示命令行头///////////////////////////
		showCommandLine(userDir, nowPath, userName, homePath);
		////////////////////////////////////////选择////////////////////////////////
		std::string input;
		getline(std::cin, input);
		size_t pos = input.find_first_of(' ');
		std::string first = input.substr(0, pos);//如果只输入了一个字符串，first与second是一样的，例cd--->first:cd, second:cd
		std::string second = input.substr(pos + 1);

		//std::cout << first << " " << second << std::endl;

		if (first == "help")
		{
			help();//显示菜单
		}
		else if (first == "exit")
		{
			flag = -1;//退出
			break;
		}
		else if (first == "clear")
		{
			system("cls");//清屏
		}
		else if (first == "ls")
		{
			LS(now, first, second);
		}
		else if (first == "ll")
		{
			LL(now, first, second);
		}
		else if (first == "pwd")
		{
			std::cout << nowPath << std::endl;
		}
		else if (first == "cd" && !second.empty())
		{
			CD(now, userDir, nowPath, second);
		}
		else if (first == "su" && !second.empty())
		{
			if ((flag = SU(UFD, second)) != -1)
			{
				break;
			}
		}
		else if (first == "whoami" || first == "id")
		{
			std::cout << userDir->owner->userName << std::endl;
		}
		else if (first == "touch" && !second.empty())
		{
			touchF(now, userDir, Map, second);//创建文件
		}
		else if (first == "mkdir" && !second.empty())
		{
			mkdirF(now, userDir, Map, second);//创建文件夹
		}
		else if (first == "rm" && !second.empty())
		{
			RM(now, userDir, second);
		}
		else if (first == "find" && !second.empty())
		{
			find(userDir, second);
		}
		else if (first == "cp" && !second.empty())
		{
			CP(now, userDir, second);
		}
		else if (first == "mv" && !second.empty())
		{
			MV(now, userDir, second);
		}
		else if (first == "chmod" && !second.empty())//修改权限
		{
			CHMOD(now, second);
		}
		else if (first == "cat" && !second.empty())
		{
			CAT(now, second);
		}
		else if (first == "head" && !second.empty())
		{
			HEAD(now, second);
		}
		else if (first == "tail" && !second.empty())
		{
			TAIL(now, second);
		}
		else if (first == "vim" && !second.empty())
		{
			VIM(now, second);
		}
		else if (first == "useradd" && !second.empty())
		{
			USERADD(second);
		}
		else if (first == "passwd" && !second.empty())
		{
			if (PASSWD(userDir, second) < 0)
			{
				std::cout << "修改失败!" << std::endl;
			}
			else
			{
				std::cout << "修改成功!" << std::endl;
			}
		}
		else if (first == "userdel" && !second.empty())
		{
			int judge = USERDEL(userDir, second);
			if (judge < 0)
			{
				std::cout << "删除用户失败!" << std::endl;
			}
			else 
			{
				std::cout << "删除用户成功!" << std::endl;
				if (judge == 0)
				{
					flag = 10;//10代表删除的是自身用户
					break;
				}
			}
		}
		if (ifChangeUser)
		{
			system("cls");
			ifChangeUser = false;
		}
	}

	return flag;//切换用户标志
}

/**
* @brief 操作入口
*/
void operation()
{
	std::cout << "初始化中..." << std::endl;
	/////////////////////////////////初始化home及以下用户目录等等目录//////////////////////////////////
	MFD->fileName = "home";//设置home目录名称,用来标记到用户底了
	for (int k = 0; k < 10; k++)
	{
		MFD->fcb[k] = NULL;
		MFD->child[k] = NULL;
	}
	init(homePath);//初始化文件系统
	////////////////////////////////////////登录账户//////////////////////////////////////
	system("cls");
	int login = -1;
	while (login == -1)
	{
		login = Login(UFD);
		//std::cout << login << std::endl;
		//system("pause");
		if (login != -1)
		{
			break;
		}
	}
	//int flag = userControl(MFD->child[login], false);
	int flag = userControl(UFD[login].userDir, false);
	//////////////当删除了自身用户，退出，重新登录//////////
	if (flag == 10)
	{
		system("cls");
		flag = -1;
		while (flag == -1)
		{
			flag = Login(UFD);
			//std::cout << login << std::endl;
			//system("pause");
			if (flag != -1)
			{
				break;
			}
		}
	}


	while (flag != -1)//切换用户,直到退出
	{

		//////////////当删除了自身用户，退出，重新登录//////////
		if (flag == 10)
		{
			system("cls");
			flag = -1;
			while (flag == -1)
			{
				flag = Login(UFD);
				//std::cout << login << std::endl;
				//system("pause");
				if (flag != -1)
				{
					break;
				}
			}
		}

		flag = userControl(UFD[flag].userDir, true);
	}
}
int main()
{
	operation();
	return 0;

}
