#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <string>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll


using namespace std;

int string_to_int(char *s) {
	int len = strlen(s);
	int val = 0;
	for (int i = 0; i < len; i++) {
		val = val * 10 + s[i] - '0';
	}
	return val;
}

#define Mem(s) memset(s, 0, sizeof(s))

void Print(char *s) {//输出，将连续的串逐条输出
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (s[i] == '#') cout << endl;
		else cout << s[i];
	}
}

void playerOutput(SOCKET & clientSocket) {
	char recvBuf[4096] = {};
	int reLen;
	cout << "==================用户信息==================" << endl;

	Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 4096, 0);
	Print(recvBuf);

	cout << "=============================================" << endl;
}

void Login(SOCKET & clientSocket) {
	//输入账号ID
	cout << "请输入您的账号: " ;
	string ID; getline(cin, ID);
	send(clientSocket, (char*)ID.c_str(), (int)ID.length(), 0);//发送到服务端

	//输入账号密码
	cout << "请输入您的密码: " ;
	string KEY; getline(cin, KEY);
	send(clientSocket, (char*)KEY.c_str(), (int)KEY.length(), 0);//发送到服务端

	char recvBuf[4096] = {};
	int reLen = recv(clientSocket, recvBuf, 4096, 0);//接收从服务端得来的信息，0为失败，1为成功

	if (strcmp(recvBuf, "0") == 0) {
		cout << "\n账号或密码错误,请重新操作!" << endl << endl;
		return;
	} 

	cout << "\n\n========登陆成功!========\n" << endl;

	playerOutput(clientSocket);

	while (true) {
		system("cls");
		cout << "\n\t\t|---------------客户端---------------|\n\n";

		cout << "尊敬的玩家，欢迎来到宠物小精灵游戏!" << endl;
		cout << "请选择操作模式" << endl;
		cout << "\t\t-------------------------------\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         1.查询个人信息       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         2.查询在线用户       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         3.查询所有用户精灵   |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         4.退出登录           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         0.返回主界面         |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t-------------------------------\n";

		cout << "请输入您的选择: ";
		int Type; cin >> Type;
		send(clientSocket, (char*)to_string(Type).c_str(), to_string(Type).length(), 0);
		getchar();
		if (Type == 1) {//查询个人信息
			playerOutput(clientSocket);
		}
		else if (Type == 2) {//查询在线用户
			cout << "\n所有在线用户:" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 4096, 0);
			Print(recvBuf);
		}
		else if (Type == 3) {//查询所有用户精灵
			cout << "\n所有用户的精灵" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 4096, 0);
			Print(recvBuf);
		}
		else if (Type == 4) {//退出登录
			cout << "\n正在退出宠物小精灵游戏......" << endl;
			return;
		}
		else if (Type == 0) {//返回主界面
			cout << "\n按任意键返回主界面......" << endl;
			return;
		}
		else {
			cout << "操作指令错误，请重新输入!" << endl;
		}
		system("pause");
	}

}

void Register(SOCKET & clientSocket) {
	cout << "请输入要注册的用户名: " ;
	string NAME; getline(cin, NAME);
	send(clientSocket, (char*)NAME.c_str(), (int)NAME.length(), 0);

	cout << "请输入要注册的账号: " ;
	string ID; getline(cin, ID);
	send(clientSocket, (char*)ID.c_str(), (int)ID.length(), 0);

	cout << "请输入密码: " ;
	string KEY; getline(cin, KEY);
	send(clientSocket, (char*)KEY.c_str(), (int)KEY.length(), 0);

	cout << "请重复输入密码: " ;
	string REPKEY; getline(cin, REPKEY);
	send(clientSocket, (char*)REPKEY.c_str(), (int)REPKEY.length(), 0);

	if (REPKEY != KEY) {
		cout << "\n两次输入的密码不同，注册失败，请重新操作!" << endl << endl;
		return;
	}

	char recvBuf[1024] = {};
	int reLen = recv(clientSocket, recvBuf, 1024, 0);
	if (strcmp(recvBuf, "1") == 0) {
		cout << "\n用户已经存在，注册失败，请重新操作!" << endl << endl;
		return;
	}

	cout << "\n\n========注册成功!========\n" << endl;

	playerOutput(clientSocket);


}

int main()
{
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t|            客户端            |\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t-------------------------------\n";

	//	1	初始化
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	//	2	创建套接字
	SOCKET clientSocket = {};
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == clientSocket) {
		cout << "套接字创建失败....." << endl;
		cout << "正在退出客户端，请重新启动!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "套接字创建成功....." << endl;
	}


	//	3	绑定套接字	指定绑定的IP地址和端口号
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(1234);
	int cRes = connect(clientSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == cRes) {
		cout << "与服务器连接失败....." << endl;
		cout << "正在退出客户端，请重新启动!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "与服务器连接成功....." << endl;
	}

	Sleep(1000);
	
	//循环发送接收数据+结构体
	while (true) {
		system("cls");
		cout << "\n\t\t|---------------客户端---------------|\n\n";

		cout << "欢迎来到宠物小精灵游戏" << endl;
		cout << "请选择操作模式" << endl;
		cout << "\t\t-------------------------------\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         1.用户登陆           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         2.用户注册           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         0.退出               |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t-------------------------------\n";

		cout << "请输入您的选择: "; 
		int Type; cin >> Type;
		send(clientSocket, (char*)to_string(Type).c_str(), to_string(Type).length(), 0);
		getchar();
		if (Type == 1) {//用户登录
			Login(clientSocket);
		}
		else if (Type == 2) {//用户注册
			Register(clientSocket);
		}
		else if (Type == 0) {
			cout << "\n正在退出宠物小精灵游戏......" << endl;
			Sleep(1000);
			return 0;
		}
		else {
			cout << "操作指令错误，请重新输入!" << endl;
		}
		system("pause");
		/*string s;
		cout << "输入发送数据: ";
		getline(cin, s);
		send(clientSocket, (char*)s.c_str(), (int)s.length(), 0);

		//因为recv接收函数是阻塞函数，所以加以判断
		//请求正确才接收数据
		if (strcmp("获取版本信息", s.c_str()) == 0) {
			char recvBuf[1024] = {};
			int reLen = recv(clientSocket, recvBuf, 1024, 0);//阻塞函数,等待接受数据
			cout << recvBuf << endl << endl;
		}
		else if (strcmp("exit", s.c_str()) == 0) {
			cout << "退出客户端" << endl;
			break;
		}*/
	}
	/*
		//循环发送接收数据
		while(true) {
			string s;
			cout << "输入发送数据: ";
			getline(cin, s);
			send(clientSocket, (char*)s.c_str(), (int)s.length(), 0);

			//因为recv接收函数是阻塞函数，所以加以判断
			//请求正确才接收数据
			if (strcmp("获取版本信息", s.c_str()) ==0 ) {
				char recvBuf[1024] = {};
				int reLen = recv(clientSocket, recvBuf, 1024, 0);//阻塞函数,等待接受数据
				cout << recvBuf << endl << endl;
			}
			else if(strcmp("exit", s.c_str()) == 0) {
				cout << "退出客户端" << endl;
				break;
			}
		}
	*/
	/*//循环发送接收数据
	while(true) {
		// 4 发送数据
		string s;
		cout << "输入发送数据：" ;
		getline(cin, s);//可输入空格，默认以换行符结束输入
		send(clientSocket, (char*)s.c_str(), s.length(), 0);

		//5 接收数据
		char recvBuf[2048] = {};
		recv(clientSocket, recvBuf, 2048, 0);
		cout << "客户端接收数据：" << recvBuf <<endl << endl;
	} */
	/*
	//	4	发送请求
	char sendBuf[1024] = "from Client:   hello service.";
	send(clientSocket, sendBuf, strlen(sendBuf), 0);


	//	5	发送/接受 数据
	char recvBuf[1024] = {};
	recv(clientSocket, recvBuf, 1024, 0);
	cout << "客户端接收数据:  " << recvBuf << endl << endl;
	*/

	//	6	关闭socket
	closesocket(clientSocket);


	//	7	终止
	WSACleanup();

	cout << "客户端退出" << endl;
	cin.get();
	return 0;
}


