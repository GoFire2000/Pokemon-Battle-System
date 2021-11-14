#define _WINSOCK_DEPRECATED_NO_WARNINGS 	
#include <ctime>
#include <string>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>						//一般情况下,这个头文件位于windows.h之前,避免发生某些错误
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib") 			//显示加载 ws2_32.dll	ws2_32.dll就是最新socket版本

#include "Player.h"
using namespace std;


Player player[2048];
int playerNum;


void Print(string s) {//输出，将改造的串恢复正常输出
	int len = s.length();
	for (int i = 0; i < len; i++) {
		if (s[i] == '#') cout << endl;
		else cout << s[i];
	}
}

string playerInformation(Player p) {//将用户信息+介绍化成一个字符串
	string verStr = "";
	verStr += "用户名: " + p.name + "#";
	verStr += "用户账号: " + p.id + "#";
	verStr += "拥有精灵数量: " + to_string(p.number) + "#";
	for (int i = 1; i <= p.number; i++) {
		Pokemon pok = p.pok[i];
		verStr += "#第" + to_string(i) + "个精灵:" + "#";
		verStr += "精灵名称: " + pok._name + "#";
		verStr += "种族名称: " + pok.getRaceName() + "#";
		verStr += "精灵等级: " + to_string(pok._level) + "#";
		verStr += "生命值: " + to_string(pok._hp) + " 攻击力: " + to_string(pok._atk) + " 防御力: " + to_string(pok._def) + " 速度: " + to_string(pok._sp) + "#";
		verStr += "技能: ";
		for (int j = 0; j < 5; j++) {
			if (pok._level >= (j - 1) * 5) verStr += pok.getSkillName(j) + "  ";
			else verStr += pok.getSkillName(j) + "(未学习)  ";
		}
		verStr += "#";
	}
	return verStr;
}

string playerInformationStore(Player p) {//将纯用户信息化成一个字符串
	string verStr = "";
	verStr += p.name + "#";//用户名
	verStr += p.id + "#";//账号
	verStr += p.key + "#";//密码
	verStr += to_string(p.number) + "#";//精灵数量
	for (int i = 1; i <= p.number; i++) {
		Pokemon pok = p.pok[i];
		verStr += to_string(pok._raceId) + "#";//种类编号
		verStr += pok._name + "#";//精灵名称
		verStr += to_string(pok._level) + "#";//等级
		verStr += to_string(pok._exp) + "#";//经验值
		verStr += to_string(pok._atk) + "#";//攻击力
		verStr += to_string(pok._def) + "#";//防御力
		verStr += to_string(pok._sp) + "#";//攻击速度
		verStr += to_string(pok._hp) + "#";//血量
	}
	return verStr;
}

void GetInformationFromFile() {//从文档中读取用户信息
	ifstream ifs;
	ifs.open(PLAYERSTORE_FILE, ios::in);
	
	//判断文件是否存在
	if (!ifs.is_open()) {
		cout << "没有历史记录,直接进入游戏阶段!" << endl;
		ifs.close();
		return;
	}
	else {
		cout << "正在载入数据....." << endl;
		Sleep(500);
	}
	string strVer = "";
	playerNum = 0;

	ifs >> playerNum;

	for (int i = 1; i <= playerNum; i++) {
		ifs >> player[i].name;
		ifs >> player[i].id;
		ifs >> player[i].key;
		ifs >> player[i].number;
		for (int j = 1; j <= player[i].number; j++) {
			ifs >> player[i].pok[j]._raceId;//种类编号
			ifs >> player[i].pok[j]._name;//精灵名称
			ifs >> player[i].pok[j]._level;//等级
			ifs >> player[i].pok[j]._exp;//经验值
			ifs >> player[i].pok[j]._atk;//攻击力
			ifs >> player[i].pok[j]._def;//防御力
			ifs >> player[i].pok[j]._sp;//速度
			ifs >> player[i].pok[j]._hp;//血量
		}
	}

	ifs.close();
}


void playerOutputToFile() {//把用户信息输入到文档
	//有详细介绍的用户信息，用于开发人员查看
	ofstream ofs;
	ofs.open(PLAYER_FILE, ios::out);

	string verStr = "";
	for (int i = 1; i <= playerNum; i++) {
		verStr += "==========第" + to_string(i) + "个用户==========";
		verStr += "#";
		verStr += playerInformation(player[i]);
		verStr += "####";
	}
	int len = verStr.length();
	for (int i = 0; i < len; i++) {
		if (verStr[i] == '#') ofs << endl;
		else ofs << verStr[i];
	}

	ofs.close();

	//纯信息，用于记录所有用户信息，在下次开启服务端时读取
	ofs.open(PLAYERSTORE_FILE, ios::out);
	verStr = to_string(playerNum) + "#";//用户数量

	for (int i = 1; i <= playerNum; i++) {
		verStr += playerInformationStore(player[i]);//每个用户的信息
	}
	len = verStr.length();
	for (int i = 0; i < len; i++) {
		if (verStr[i] == '#') ofs << endl;
		else ofs << verStr[i];
	}

	ofs.close();
}

#define Mem(s) memset(s, 0 ,sizeof(s))
void playerOutput(SOCKET & recvClientSocket, Player p) {
	string verStr = "";
	int sLen;

	cout << "=================用户信息==================" << endl;

	verStr = playerInformation(p);
	Print(verStr);
	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	cout << "=============================================" <<  endl;
}

void Login(SOCKET & recvClientSocket) {
	cout << endl << "用户登录" << endl;
	char recvBuf[1024] = {};
	int AccExist = 0, KeyRight = 0;//分别为账号是否存在，密码是否正确
	int reLen;
	//从客户端读取账号id
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);
	cout << "用户账号: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	//判断该账号是否存在
	for (int i = 1; i <= playerNum; i++) {
		if (strcmp(recvBuf, player[i].id.c_str()) == 0) {
			AccExist = i;
			break;
		}
	}

	//从客户端读取账号密码
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);
	cout << "用户密码: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	//如果账号存在并且对应密码正确，使KeyRight设为true
	if (AccExist && strcmp(recvBuf, player[AccExist].key.c_str()) == 0) {
		KeyRight = 1;
	}

	//将登陆成功与否返回到客户端，0为失败，1为成功
	int sLen = send(recvClientSocket, (char*)to_string(KeyRight).c_str(), to_string(KeyRight).length(), 0);

	if (KeyRight == 0) {
		cout << "\n登陆失败！" << endl;
		return;
	}

	cout << "\n\n===登陆成功!===\n" << endl;
	playerOutput(recvClientSocket, player[AccExist]);

	player[AccExist].loginStatus = true;

	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
		//阻塞函数，等待接收数据
		cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

		string verStr = "";
		if (strcmp(recvBuf, "1") == 0) {//查询个人信息
			playerOutput(recvClientSocket, player[AccExist]);
		}
		else if (strcmp(recvBuf, "2") == 0) {//查询在线用户
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) if (player[i].loginStatus) {
				verStr += "第" + to_string(++tot) + "个用户:    ||   " + "用户名: " + player[i].name + "  用户账号: " + player[i].id + "  精灵数量: " + to_string(player[i].number);
				verStr += "#";
			}
			verStr += "共" + to_string(tot) + "个在线用户";
			verStr += "##";
			
			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "3") == 0) {

			int tot = 0;
			for (int i = 1; i <= playerNum; i++) {
				verStr += "用户" + player[i].name + "共" + to_string(player[i].number) + "个精灵:";
				verStr += "#";
				for (int j = 1; j <= player[i].number; j++) {
					verStr += "\t"; 
					verStr += "第" + to_string(j) + "个精灵:    ||   " + "精灵名称: " + player[i].pok[j]._name + "    精灵种族: " +player[i].pok[j].getRaceName() + "    精灵等级: " + to_string(player[i].pok[j]._level);
					verStr += "#";
				}
				verStr += "#";
			}
			verStr += "##";

			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
		else if (strcmp(recvBuf, "4") == 0) {
			player[AccExist].loginStatus = false;
			cout << "退出登录" << endl;
			break;
		}
		else if (strcmp(recvBuf, "0") == 0) {
			cout << "返回主界面" << endl;
			break;
		}
		else {
			cout << "操作指令错误，请重新输入!" << endl;
		}

	}
}


void Register(SOCKET & recvClientSocket) {
	cout << endl << "用户注册" << endl;
	char recvBuf[1024] = {};
	string NAME, ID, KEY;
	int reLen;

	++playerNum;

	//从客户端读取用户名
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "用户名: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	NAME = recvBuf;

	//从客户端读取用户账号
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "用户账号: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	ID = recvBuf;
	
	//从客户端读取用户密码
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "用户密码: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	KEY = recvBuf;

	//从客户端再次读取用户密码
	Mem(recvBuf); reLen =  recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	if (strcmp(recvBuf, KEY.c_str()) != 0) {
		playerNum--;
		cout << "\n两次密码不同，注册失败，信息撤销!" << endl;
		return;
	}

	//判断用户ID是否已经存在，true表示已经存在
	int tag = 0;
	for (int i = 1; i <= playerNum-1; i++) {
		if (ID == player[i].id) {
			tag = 1;
			break;
		}
	}
	int sLen = send(recvClientSocket, (char*)to_string(tag).c_str(), to_string(tag).length(), 0);
	if (tag == 1) {
		playerNum--;
		cout << "\n用户已经存在，注册失败，信息撤销!" << endl;
		return;
	}

	player[playerNum] = Player(NAME, ID, KEY);

	cout << "\n\n===注册成功!===\n" << endl;
	playerOutput(recvClientSocket, player[playerNum]);

	playerOutputToFile();

}

//服务线程
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET *recvClientSocket = (SOCKET*)lpParameter;
	//循环接收+判断
	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(*recvClientSocket, recvBuf, 1024, 0);

		cout << "接收到消息: " << recvBuf << "               来自客户端:" << *recvClientSocket << endl;
		//阻塞函数，等待接收数据

		if (strcmp(recvBuf, "1") == 0) {//用户登录
			Login(*recvClientSocket);
		}
		else if (strcmp(recvBuf, "2") == 0) {//用户注册
			Register(*recvClientSocket);
		}
		else if (strcmp(recvBuf, "0") == 0) {//退出
			cout << "退出" << endl;

			break;
		}
		else {
			cout << "操作指令错误，请重新输入!" << endl;
		}
	}
	closesocket(*recvClientSocket);
	free(recvClientSocket);
	return 0;
}

int main()
{
	srand(time(NULL));
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t|            服务端            |\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t-------------------------------\n";
	//	1	初始化
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);	//make word,你把鼠标移到WSAStartup看看参数列表,是不是就是一个word啊


	//	2	创建服务器的套接字
	SOCKET serviceSocket;
	serviceSocket = socket(AF_INET, SOCK_STREAM, 0);	//socket(协议族,socket数据传输方式,某个协议)	我们默认为0,其实就是一个宏
	if (SOCKET_ERROR == serviceSocket) {
		cout << "套接字创建失败....." << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "套接字创建成功....." << endl;
	}


	//	3	绑定套接字	指定绑定的IP地址和端口号
	sockaddr_in socketAddr;								//一个绑定地址:有IP地址,有端口号,有协议族
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	socketAddr.sin_port = htons(1234);
	int bRes = bind(serviceSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));	//绑定注意的一点就是记得强制类型转换
	if (SOCKET_ERROR == bRes) {
		cout << "绑定失败....." << endl;
		cout << "正在退出服务端，请重新启动!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "绑定成功....." << endl;
	}

	//	4	服务器监听	
	int lLen = listen(serviceSocket, 20);	//监听的第二个参数就是:能存放多少个客户端请求,到并发编程的时候很有用哦
	if (SOCKET_ERROR == lLen) {
		cout << "监听失败....." << endl;
		cout << "正在退出服务端，请重新启动!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "监听成功....." << endl;
	}

	//从playerInformationStore.txt里面读取信息，获取历史记录
	GetInformationFromFile();


	while (true) {//多线程，循环接收客户端连接请求并创建服务线程
		SOCKET *ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		//接收客户端连接请求
		int SockAddrlen = sizeof(sockaddr);
		*ClientSocket = accept(serviceSocket, 0, 0);
		cout << "一个客户端已连接到服务器，socket是：" << *ClientSocket << endl;
		CreateThread(NULL, 0, &ServerThread, ClientSocket, 0, NULL);
	}
	/*
	//	5	接受请求
	sockaddr_in revClientAddr;
	SOCKET recvClientSocket = INVALID_SOCKET;	//初始化一个接受的客户端socket
	int _revSize = sizeof(sockaddr_in);
	recvClientSocket = accept(serviceSocket, (SOCKADDR*)&revClientAddr, &_revSize);
	if (INVALID_SOCKET == recvClientSocket) {
		cout << "服务端接受请求失败....." << endl;
		cout << "正在退出服务端，请重新启动!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "服务端接受请求成功....." << endl;
	}

	Sleep(1000);

	//6 发送/接收数据
	//循环接收+判断
	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
		//阻塞函数，等待接收数据

		if (strcmp(recvBuf, "1") == 0) {//用户登录
			Login(recvClientSocket);
			
		}
		else if (strcmp(recvBuf, "2") == 0) {//用户注册
			Register(recvClientSocket);
		}
		else if (strcmp(recvBuf, "0") == 0) {//退出
			cout << "退出" << endl;
			
			break;
		}
		else {
			cout << "操作指令错误，请重新输入!" << endl;
		}

	}*/

	//	7	关闭socket
	//closesocket(recvClientSocket);
	closesocket(serviceSocket);

	//	8	终止
	WSACleanup();

	cout << "服务器停止" << endl;
	cin.get();
	return 0;
}
