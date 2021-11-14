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
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll


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

void Print(char *s) {//������������Ĵ��������
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (s[i] == '#') cout << endl;
		else cout << s[i];
	}
}

void playerOutput(SOCKET & clientSocket) {
	char recvBuf[4096] = {};
	int reLen;
	cout << "==================�û���Ϣ==================" << endl;

	Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 4096, 0);
	Print(recvBuf);

	cout << "=============================================" << endl;
}

void Login(SOCKET & clientSocket) {
	//�����˺�ID
	cout << "�����������˺�: " ;
	string ID; getline(cin, ID);
	send(clientSocket, (char*)ID.c_str(), (int)ID.length(), 0);//���͵������

	//�����˺�����
	cout << "��������������: " ;
	string KEY; getline(cin, KEY);
	send(clientSocket, (char*)KEY.c_str(), (int)KEY.length(), 0);//���͵������

	char recvBuf[4096] = {};
	int reLen = recv(clientSocket, recvBuf, 4096, 0);//���մӷ���˵�������Ϣ��0Ϊʧ�ܣ�1Ϊ�ɹ�

	if (strcmp(recvBuf, "0") == 0) {
		cout << "\n�˺Ż��������,�����²���!" << endl << endl;
		return;
	} 

	cout << "\n\n========��½�ɹ�!========\n" << endl;

	playerOutput(clientSocket);

	while (true) {
		system("cls");
		cout << "\n\t\t|---------------�ͻ���---------------|\n\n";

		cout << "�𾴵���ң���ӭ��������С������Ϸ!" << endl;
		cout << "��ѡ�����ģʽ" << endl;
		cout << "\t\t-------------------------------\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         1.��ѯ������Ϣ       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         2.��ѯ�����û�       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         3.��ѯ�����û�����   |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         4.�˳���¼           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         0.����������         |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t-------------------------------\n";

		cout << "����������ѡ��: ";
		int Type; cin >> Type;
		send(clientSocket, (char*)to_string(Type).c_str(), to_string(Type).length(), 0);
		getchar();
		if (Type == 1) {//��ѯ������Ϣ
			playerOutput(clientSocket);
		}
		else if (Type == 2) {//��ѯ�����û�
			cout << "\n���������û�:" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 4096, 0);
			Print(recvBuf);
		}
		else if (Type == 3) {//��ѯ�����û�����
			cout << "\n�����û��ľ���" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 4096, 0);
			Print(recvBuf);
		}
		else if (Type == 4) {//�˳���¼
			cout << "\n�����˳�����С������Ϸ......" << endl;
			return;
		}
		else if (Type == 0) {//����������
			cout << "\n�����������������......" << endl;
			return;
		}
		else {
			cout << "����ָ���������������!" << endl;
		}
		system("pause");
	}

}

void Register(SOCKET & clientSocket) {
	cout << "������Ҫע����û���: " ;
	string NAME; getline(cin, NAME);
	send(clientSocket, (char*)NAME.c_str(), (int)NAME.length(), 0);

	cout << "������Ҫע����˺�: " ;
	string ID; getline(cin, ID);
	send(clientSocket, (char*)ID.c_str(), (int)ID.length(), 0);

	cout << "����������: " ;
	string KEY; getline(cin, KEY);
	send(clientSocket, (char*)KEY.c_str(), (int)KEY.length(), 0);

	cout << "���ظ���������: " ;
	string REPKEY; getline(cin, REPKEY);
	send(clientSocket, (char*)REPKEY.c_str(), (int)REPKEY.length(), 0);

	if (REPKEY != KEY) {
		cout << "\n������������벻ͬ��ע��ʧ�ܣ������²���!" << endl << endl;
		return;
	}

	char recvBuf[1024] = {};
	int reLen = recv(clientSocket, recvBuf, 1024, 0);
	if (strcmp(recvBuf, "1") == 0) {
		cout << "\n�û��Ѿ����ڣ�ע��ʧ�ܣ������²���!" << endl << endl;
		return;
	}

	cout << "\n\n========ע��ɹ�!========\n" << endl;

	playerOutput(clientSocket);


}

int main()
{
	cout << "\t\t-------------------------------\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t|            �ͻ���            |\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t-------------------------------\n";

	//	1	��ʼ��
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	//	2	�����׽���
	SOCKET clientSocket = {};
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == clientSocket) {
		cout << "�׽��ִ���ʧ��....." << endl;
		cout << "�����˳��ͻ��ˣ�����������!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "�׽��ִ����ɹ�....." << endl;
	}


	//	3	���׽���	ָ���󶨵�IP��ַ�Ͷ˿ں�
	sockaddr_in socketAddr;
	socketAddr.sin_family = PF_INET;
	socketAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(1234);
	int cRes = connect(clientSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == cRes) {
		cout << "�����������ʧ��....." << endl;
		cout << "�����˳��ͻ��ˣ�����������!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "����������ӳɹ�....." << endl;
	}

	Sleep(1000);
	
	//ѭ�����ͽ�������+�ṹ��
	while (true) {
		system("cls");
		cout << "\n\t\t|---------------�ͻ���---------------|\n\n";

		cout << "��ӭ��������С������Ϸ" << endl;
		cout << "��ѡ�����ģʽ" << endl;
		cout << "\t\t-------------------------------\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         1.�û���½           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         2.�û�ע��           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         0.�˳�               |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t-------------------------------\n";

		cout << "����������ѡ��: "; 
		int Type; cin >> Type;
		send(clientSocket, (char*)to_string(Type).c_str(), to_string(Type).length(), 0);
		getchar();
		if (Type == 1) {//�û���¼
			Login(clientSocket);
		}
		else if (Type == 2) {//�û�ע��
			Register(clientSocket);
		}
		else if (Type == 0) {
			cout << "\n�����˳�����С������Ϸ......" << endl;
			Sleep(1000);
			return 0;
		}
		else {
			cout << "����ָ���������������!" << endl;
		}
		system("pause");
		/*string s;
		cout << "���뷢������: ";
		getline(cin, s);
		send(clientSocket, (char*)s.c_str(), (int)s.length(), 0);

		//��Ϊrecv���պ������������������Լ����ж�
		//������ȷ�Ž�������
		if (strcmp("��ȡ�汾��Ϣ", s.c_str()) == 0) {
			char recvBuf[1024] = {};
			int reLen = recv(clientSocket, recvBuf, 1024, 0);//��������,�ȴ���������
			cout << recvBuf << endl << endl;
		}
		else if (strcmp("exit", s.c_str()) == 0) {
			cout << "�˳��ͻ���" << endl;
			break;
		}*/
	}
	/*
		//ѭ�����ͽ�������
		while(true) {
			string s;
			cout << "���뷢������: ";
			getline(cin, s);
			send(clientSocket, (char*)s.c_str(), (int)s.length(), 0);

			//��Ϊrecv���պ������������������Լ����ж�
			//������ȷ�Ž�������
			if (strcmp("��ȡ�汾��Ϣ", s.c_str()) ==0 ) {
				char recvBuf[1024] = {};
				int reLen = recv(clientSocket, recvBuf, 1024, 0);//��������,�ȴ���������
				cout << recvBuf << endl << endl;
			}
			else if(strcmp("exit", s.c_str()) == 0) {
				cout << "�˳��ͻ���" << endl;
				break;
			}
		}
	*/
	/*//ѭ�����ͽ�������
	while(true) {
		// 4 ��������
		string s;
		cout << "���뷢�����ݣ�" ;
		getline(cin, s);//������ո�Ĭ���Ի��з���������
		send(clientSocket, (char*)s.c_str(), s.length(), 0);

		//5 ��������
		char recvBuf[2048] = {};
		recv(clientSocket, recvBuf, 2048, 0);
		cout << "�ͻ��˽������ݣ�" << recvBuf <<endl << endl;
	} */
	/*
	//	4	��������
	char sendBuf[1024] = "from Client:   hello service.";
	send(clientSocket, sendBuf, strlen(sendBuf), 0);


	//	5	����/���� ����
	char recvBuf[1024] = {};
	recv(clientSocket, recvBuf, 1024, 0);
	cout << "�ͻ��˽�������:  " << recvBuf << endl << endl;
	*/

	//	6	�ر�socket
	closesocket(clientSocket);


	//	7	��ֹ
	WSACleanup();

	cout << "�ͻ����˳�" << endl;
	cin.get();
	return 0;
}


