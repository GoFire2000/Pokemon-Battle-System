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

char recvBuf[65536];//�洢�ӷ��������յ�����

int string_to_int(char *s) {//��string��int
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

void playerOutput(SOCKET & clientSocket) {//����û���Ϣ
	   
	int reLen;
	cout << "==================�û���Ϣ==================" << endl;

	Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
	Print(recvBuf);

	cout << "=============================================" << endl;
}

void Login(SOCKET & clientSocket) {//�û���¼
	//�����˺�ID
	cout << "�����������˺�: " ;
	string ID; getline(cin, ID);
	send(clientSocket, (char*)ID.c_str(), (int)ID.length(), 0);//���͵������

	//�����˺�����
	cout << "��������������: " ;
	string KEY; getline(cin, KEY);
	send(clientSocket, (char*)KEY.c_str(), (int)KEY.length(), 0);//���͵������

	   
	Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);//���մӷ���˵�������Ϣ��0Ϊʧ�ܣ�1Ϊ�ɹ�

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
		cout << "\t\t|         1.������             |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         2.������             |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         3.��ѯ������Ϣ       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         4.��ѯ�����û�       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         5.��ѯ�����û�����   |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         6.��ѯ�û�ʤ��       |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         7.�޸�����           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         8.�˳���¼           |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t|         0.����������         |\n";
		cout << "\t\t|                              |\n";
		cout << "\t\t-------------------------------\n";

		cout << "����������ѡ��: ";
		int Type; cin >> Type;
		send(clientSocket, (char*)to_string(Type).c_str(), to_string(Type).length(), 0);
		getchar();
		if (Type == 1) {//����������
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);

			//����ѡ����ҷ������Ų�����ŷ��͵������
			int pokemonId1; cin >> pokemonId1;
			int sLen = send(clientSocket, (char*)to_string(pokemonId1).c_str(), to_string(pokemonId1).length(), 0);



			Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);

			//����ѡ��ĵз������Ų�����ŷ��͵������
			int pokemonId2; cin >> pokemonId2;
			sLen = send(clientSocket, (char*)to_string(pokemonId2).c_str(), to_string(pokemonId2).length(), 0);

			//����ս��ģʽ������ŷ�����������
			cout << "��ѡ��ս��ģʽ:" << endl;
			cout << "1: �ֶ�ʩ��ս��" << endl;
			cout << "2: �Զ�ս��" << endl;

			int Mode; cin >> Mode;
			sLen = send(clientSocket, (char*)to_string(Mode).c_str(), to_string(Mode).length(), 0);
			if (Mode == 2) {
				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf);
			}
			else {
				while (true) {
					Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
					if (strcmp(recvBuf, "����") == 0) break;
					Print(recvBuf);
					int skillId; cin >> skillId;
					sLen = send(clientSocket, (char*)to_string(skillId).c_str(), to_string(skillId).length(), 0);

				}
				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf);
		
			}

		}
		else if (Type == 2) {//���о�����
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);

			//����ѡ����ҷ������Ų�����ŷ��͵������
			int pokemonId1; cin >> pokemonId1;
			int sLen = send(clientSocket, (char*)to_string(pokemonId1).c_str(), to_string(pokemonId1).length(), 0);



			Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);

			//����ѡ��ĵз������Ų�����ŷ��͵������
			int pokemonId2; cin >> pokemonId2;
			sLen = send(clientSocket, (char*)to_string(pokemonId2).c_str(), to_string(pokemonId2).length(), 0);

			//����ս��ģʽ������ŷ�����������
			cout << "��ѡ��ս��ģʽ:" << endl;
			cout << "1: �ֶ�ʩ��ս��" << endl;
			cout << "2: �Զ�ս��" << endl;

			int Mode; cin >> Mode;
			sLen = send(clientSocket, (char*)to_string(Mode).c_str(), to_string(Mode).length(), 0);
			if (Mode == 2) {
				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf);

				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf + 1);
				//cout << recvBuf << endl;
				if (recvBuf[0] == '~') {
					int Id; cin >> Id;
					sLen = send(clientSocket, (char*)to_string(Id).c_str(), to_string(Id).length(), 0);

					Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
					Print(recvBuf);
				}



			}
			else {
				while (true) {
					Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
					if (strcmp(recvBuf, "����") == 0) break;
					Print(recvBuf);
					int skillId; cin >> skillId;
					sLen = send(clientSocket, (char*)to_string(skillId).c_str(), to_string(skillId).length(), 0);

				}
				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf);

				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				if (strcmp(recvBuf, "lose") == 0) {
					int Id; cin >> Id;
					sLen = send(clientSocket, (char*)to_string(Id).c_str(), to_string(Id).length(), 0);

				}

				Mem(recvBuf); reLen = recv(clientSocket, recvBuf, 65536, 0);
				Print(recvBuf);

			}
		}
		else if (Type == 3) {//��ѯ������Ϣ
			playerOutput(clientSocket);
		}
		else if (Type == 4) {//��ѯ�����û�
			cout << "\n���������û�:" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);
		}
		else if (Type == 5) {//��ѯ�����û�����
			cout << "\n�����û��ľ���" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);
		}
		else if (Type == 6) {//��ѯ�û�ʤ��
			cout << "\n�û�ʤ��" << endl;
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			Print(recvBuf);
		}
		else if (Type == 7) {//�޸�����
			cout << "\n�޸�����" << endl;
			
			cout << "����������ǰ������: ";
			string verStr = ""; getline(cin, verStr);
			int sLen = send(clientSocket, (char*)verStr.c_str(), verStr.length(), 0);
			
			Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
			int flag = string_to_int(recvBuf);
			if (flag == 0) {
				cout << "������������²���!" << endl;
			}
			else {
				cout << "����������������: ";
				getline(cin, verStr);
				sLen = send(clientSocket, (char*)verStr.c_str(), verStr.length(), 0);

				cout << "�����޸ĳɹ�!" << endl;
			}
		}
		else if (Type == 8) {//�˳���¼
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

void Register(SOCKET & clientSocket) {//�û�ע��
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

	   
	Mem(recvBuf); int reLen = recv(clientSocket, recvBuf, 65536, 0);
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
			   
			int reLen = recv(clientSocket, recvBuf, 65536, 0);//��������,�ȴ���������
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
				   
				int reLen = recv(clientSocket, recvBuf, 65536, 0);//��������,�ȴ���������
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
		   
		recv(clientSocket, recvBuf, 65536, 0);
		cout << "�ͻ��˽������ݣ�" << recvBuf <<endl << endl;
	} */
	/*
	//	4	��������
	char sendBuf[65536] = "from Client:   hello service.";
	send(clientSocket, sendBuf, strlen(sendBuf), 0);


	//	5	����/���� ����
	   
	recv(clientSocket, recvBuf, 65536, 0);
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


