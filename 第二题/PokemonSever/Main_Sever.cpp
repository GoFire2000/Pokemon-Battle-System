#define _WINSOCK_DEPRECATED_NO_WARNINGS 	
#include <ctime>
#include <string>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>						//һ�������,���ͷ�ļ�λ��windows.h֮ǰ,���ⷢ��ĳЩ����
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib") 			//��ʾ���� ws2_32.dll	ws2_32.dll��������socket�汾

#include "Player.h"
using namespace std;


Player player[2048];
int playerNum;


void Print(string s) {//�����������Ĵ��ָ��������
	int len = s.length();
	for (int i = 0; i < len; i++) {
		if (s[i] == '#') cout << endl;
		else cout << s[i];
	}
}

string playerInformation(Player p) {//���û���Ϣ+���ܻ���һ���ַ���
	string verStr = "";
	verStr += "�û���: " + p.name + "#";
	verStr += "�û��˺�: " + p.id + "#";
	verStr += "ӵ�о�������: " + to_string(p.number) + "#";
	for (int i = 1; i <= p.number; i++) {
		Pokemon pok = p.pok[i];
		verStr += "#��" + to_string(i) + "������:" + "#";
		verStr += "��������: " + pok._name + "#";
		verStr += "��������: " + pok.getRaceName() + "#";
		verStr += "����ȼ�: " + to_string(pok._level) + "#";
		verStr += "����ֵ: " + to_string(pok._hp) + " ������: " + to_string(pok._atk) + " ������: " + to_string(pok._def) + " �ٶ�: " + to_string(pok._sp) + "#";
		verStr += "����: ";
		for (int j = 0; j < 5; j++) {
			if (pok._level >= (j - 1) * 5) verStr += pok.getSkillName(j) + "  ";
			else verStr += pok.getSkillName(j) + "(δѧϰ)  ";
		}
		verStr += "#";
	}
	return verStr;
}

string playerInformationStore(Player p) {//�����û���Ϣ����һ���ַ���
	string verStr = "";
	verStr += p.name + "#";//�û���
	verStr += p.id + "#";//�˺�
	verStr += p.key + "#";//����
	verStr += to_string(p.number) + "#";//��������
	for (int i = 1; i <= p.number; i++) {
		Pokemon pok = p.pok[i];
		verStr += to_string(pok._raceId) + "#";//������
		verStr += pok._name + "#";//��������
		verStr += to_string(pok._level) + "#";//�ȼ�
		verStr += to_string(pok._exp) + "#";//����ֵ
		verStr += to_string(pok._atk) + "#";//������
		verStr += to_string(pok._def) + "#";//������
		verStr += to_string(pok._sp) + "#";//�����ٶ�
		verStr += to_string(pok._hp) + "#";//Ѫ��
	}
	return verStr;
}

void GetInformationFromFile() {//���ĵ��ж�ȡ�û���Ϣ
	ifstream ifs;
	ifs.open(PLAYERSTORE_FILE, ios::in);
	
	//�ж��ļ��Ƿ����
	if (!ifs.is_open()) {
		cout << "û����ʷ��¼,ֱ�ӽ�����Ϸ�׶�!" << endl;
		ifs.close();
		return;
	}
	else {
		cout << "������������....." << endl;
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
			ifs >> player[i].pok[j]._raceId;//������
			ifs >> player[i].pok[j]._name;//��������
			ifs >> player[i].pok[j]._level;//�ȼ�
			ifs >> player[i].pok[j]._exp;//����ֵ
			ifs >> player[i].pok[j]._atk;//������
			ifs >> player[i].pok[j]._def;//������
			ifs >> player[i].pok[j]._sp;//�ٶ�
			ifs >> player[i].pok[j]._hp;//Ѫ��
		}
	}

	ifs.close();
}


void playerOutputToFile() {//���û���Ϣ���뵽�ĵ�
	//����ϸ���ܵ��û���Ϣ�����ڿ�����Ա�鿴
	ofstream ofs;
	ofs.open(PLAYER_FILE, ios::out);

	string verStr = "";
	for (int i = 1; i <= playerNum; i++) {
		verStr += "==========��" + to_string(i) + "���û�==========";
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

	//����Ϣ�����ڼ�¼�����û���Ϣ�����´ο��������ʱ��ȡ
	ofs.open(PLAYERSTORE_FILE, ios::out);
	verStr = to_string(playerNum) + "#";//�û�����

	for (int i = 1; i <= playerNum; i++) {
		verStr += playerInformationStore(player[i]);//ÿ���û�����Ϣ
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

	cout << "=================�û���Ϣ==================" << endl;

	verStr = playerInformation(p);
	Print(verStr);
	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	cout << "=============================================" <<  endl;
}

void Login(SOCKET & recvClientSocket) {
	cout << endl << "�û���¼" << endl;
	char recvBuf[1024] = {};
	int AccExist = 0, KeyRight = 0;//�ֱ�Ϊ�˺��Ƿ���ڣ������Ƿ���ȷ
	int reLen;
	//�ӿͻ��˶�ȡ�˺�id
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);
	cout << "�û��˺�: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	//�жϸ��˺��Ƿ����
	for (int i = 1; i <= playerNum; i++) {
		if (strcmp(recvBuf, player[i].id.c_str()) == 0) {
			AccExist = i;
			break;
		}
	}

	//�ӿͻ��˶�ȡ�˺�����
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);
	cout << "�û�����: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	//����˺Ŵ��ڲ��Ҷ�Ӧ������ȷ��ʹKeyRight��Ϊtrue
	if (AccExist && strcmp(recvBuf, player[AccExist].key.c_str()) == 0) {
		KeyRight = 1;
	}

	//����½�ɹ���񷵻ص��ͻ��ˣ�0Ϊʧ�ܣ�1Ϊ�ɹ�
	int sLen = send(recvClientSocket, (char*)to_string(KeyRight).c_str(), to_string(KeyRight).length(), 0);

	if (KeyRight == 0) {
		cout << "\n��½ʧ�ܣ�" << endl;
		return;
	}

	cout << "\n\n===��½�ɹ�!===\n" << endl;
	playerOutput(recvClientSocket, player[AccExist]);

	player[AccExist].loginStatus = true;

	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
		//�����������ȴ���������
		cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

		string verStr = "";
		if (strcmp(recvBuf, "1") == 0) {//��ѯ������Ϣ
			playerOutput(recvClientSocket, player[AccExist]);
		}
		else if (strcmp(recvBuf, "2") == 0) {//��ѯ�����û�
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) if (player[i].loginStatus) {
				verStr += "��" + to_string(++tot) + "���û�:    ||   " + "�û���: " + player[i].name + "  �û��˺�: " + player[i].id + "  ��������: " + to_string(player[i].number);
				verStr += "#";
			}
			verStr += "��" + to_string(tot) + "�������û�";
			verStr += "##";
			
			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "3") == 0) {

			int tot = 0;
			for (int i = 1; i <= playerNum; i++) {
				verStr += "�û�" + player[i].name + "��" + to_string(player[i].number) + "������:";
				verStr += "#";
				for (int j = 1; j <= player[i].number; j++) {
					verStr += "\t"; 
					verStr += "��" + to_string(j) + "������:    ||   " + "��������: " + player[i].pok[j]._name + "    ��������: " +player[i].pok[j].getRaceName() + "    ����ȼ�: " + to_string(player[i].pok[j]._level);
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
			cout << "�˳���¼" << endl;
			break;
		}
		else if (strcmp(recvBuf, "0") == 0) {
			cout << "����������" << endl;
			break;
		}
		else {
			cout << "����ָ���������������!" << endl;
		}

	}
}


void Register(SOCKET & recvClientSocket) {
	cout << endl << "�û�ע��" << endl;
	char recvBuf[1024] = {};
	string NAME, ID, KEY;
	int reLen;

	++playerNum;

	//�ӿͻ��˶�ȡ�û���
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "�û���: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	NAME = recvBuf;

	//�ӿͻ��˶�ȡ�û��˺�
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "�û��˺�: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	ID = recvBuf;
	
	//�ӿͻ��˶�ȡ�û�����
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "�û�����: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	KEY = recvBuf;

	//�ӿͻ����ٴζ�ȡ�û�����
	Mem(recvBuf); reLen =  recv(recvClientSocket, recvBuf, 1024, 0);;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	if (strcmp(recvBuf, KEY.c_str()) != 0) {
		playerNum--;
		cout << "\n�������벻ͬ��ע��ʧ�ܣ���Ϣ����!" << endl;
		return;
	}

	//�ж��û�ID�Ƿ��Ѿ����ڣ�true��ʾ�Ѿ�����
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
		cout << "\n�û��Ѿ����ڣ�ע��ʧ�ܣ���Ϣ����!" << endl;
		return;
	}

	player[playerNum] = Player(NAME, ID, KEY);

	cout << "\n\n===ע��ɹ�!===\n" << endl;
	playerOutput(recvClientSocket, player[playerNum]);

	playerOutputToFile();

}

//�����߳�
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET *recvClientSocket = (SOCKET*)lpParameter;
	//ѭ������+�ж�
	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(*recvClientSocket, recvBuf, 1024, 0);

		cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << *recvClientSocket << endl;
		//�����������ȴ���������

		if (strcmp(recvBuf, "1") == 0) {//�û���¼
			Login(*recvClientSocket);
		}
		else if (strcmp(recvBuf, "2") == 0) {//�û�ע��
			Register(*recvClientSocket);
		}
		else if (strcmp(recvBuf, "0") == 0) {//�˳�
			cout << "�˳�" << endl;

			break;
		}
		else {
			cout << "����ָ���������������!" << endl;
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
	cout << "\t\t|            �����            |\n";
	cout << "\t\t|                              |\n";
	cout << "\t\t-------------------------------\n";
	//	1	��ʼ��
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);	//make word,�������Ƶ�WSAStartup���������б�,�ǲ��Ǿ���һ��word��


	//	2	�������������׽���
	SOCKET serviceSocket;
	serviceSocket = socket(AF_INET, SOCK_STREAM, 0);	//socket(Э����,socket���ݴ��䷽ʽ,ĳ��Э��)	����Ĭ��Ϊ0,��ʵ����һ����
	if (SOCKET_ERROR == serviceSocket) {
		cout << "�׽��ִ���ʧ��....." << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "�׽��ִ����ɹ�....." << endl;
	}


	//	3	���׽���	ָ���󶨵�IP��ַ�Ͷ˿ں�
	sockaddr_in socketAddr;								//һ���󶨵�ַ:��IP��ַ,�ж˿ں�,��Э����
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	socketAddr.sin_port = htons(1234);
	int bRes = bind(serviceSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));	//��ע���һ����Ǽǵ�ǿ������ת��
	if (SOCKET_ERROR == bRes) {
		cout << "��ʧ��....." << endl;
		cout << "�����˳�����ˣ�����������!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "�󶨳ɹ�....." << endl;
	}

	//	4	����������	
	int lLen = listen(serviceSocket, 20);	//�����ĵڶ�����������:�ܴ�Ŷ��ٸ��ͻ�������,��������̵�ʱ�������Ŷ
	if (SOCKET_ERROR == lLen) {
		cout << "����ʧ��....." << endl;
		cout << "�����˳�����ˣ�����������!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "�����ɹ�....." << endl;
	}

	//��playerInformationStore.txt�����ȡ��Ϣ����ȡ��ʷ��¼
	GetInformationFromFile();


	while (true) {//���̣߳�ѭ�����տͻ����������󲢴��������߳�
		SOCKET *ClientSocket = new SOCKET;
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		//���տͻ�����������
		int SockAddrlen = sizeof(sockaddr);
		*ClientSocket = accept(serviceSocket, 0, 0);
		cout << "һ���ͻ��������ӵ���������socket�ǣ�" << *ClientSocket << endl;
		CreateThread(NULL, 0, &ServerThread, ClientSocket, 0, NULL);
	}
	/*
	//	5	��������
	sockaddr_in revClientAddr;
	SOCKET recvClientSocket = INVALID_SOCKET;	//��ʼ��һ�����ܵĿͻ���socket
	int _revSize = sizeof(sockaddr_in);
	recvClientSocket = accept(serviceSocket, (SOCKADDR*)&revClientAddr, &_revSize);
	if (INVALID_SOCKET == recvClientSocket) {
		cout << "����˽�������ʧ��....." << endl;
		cout << "�����˳�����ˣ�����������!" << endl;
		Sleep(1000);
		return 0;
	}
	else {
		cout << "����˽�������ɹ�....." << endl;
	}

	Sleep(1000);

	//6 ����/��������
	//ѭ������+�ж�
	while (true) {
		char recvBuf[1024] = {};
		int reLen = recv(recvClientSocket, recvBuf, 1024, 0);;
		//�����������ȴ���������

		if (strcmp(recvBuf, "1") == 0) {//�û���¼
			Login(recvClientSocket);
			
		}
		else if (strcmp(recvBuf, "2") == 0) {//�û�ע��
			Register(recvClientSocket);
		}
		else if (strcmp(recvBuf, "0") == 0) {//�˳�
			cout << "�˳�" << endl;
			
			break;
		}
		else {
			cout << "����ָ���������������!" << endl;
		}

	}*/

	//	7	�ر�socket
	//closesocket(recvClientSocket);
	closesocket(serviceSocket);

	//	8	��ֹ
	WSACleanup();

	cout << "������ֹͣ" << endl;
	cin.get();
	return 0;
}
