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
//#include "battleController.h"
using namespace std;


char recvBuf[65536];//�洢�ӿͻ��˽��յ��ַ���
//string verStr;//���ڴ洢��ͻ��˷��͵��ַ���

Player player[105];//�û�������
int playerNum;//�û�������

int string_to_int(char *s) {//��string��int
	int len = strlen(s);
	int val = 0;
	for (int i = 0; i < len; i++) {
		val = val * 10 + s[i] - '0';
	}
	return val;
}

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
	verStr += "�������ѫ��: " + p.numberMedal() + "#";
	verStr += "�߼�����ѫ��: " + p.superPokemonMedal() + "#"; 
	if (p.totNum == 0) verStr += "ս������: 0   ʤ������: 0   ʤ��: ��#";//û�вμӹ��������޷�����ʤ��
	else verStr += "ս������: " + to_string(p.totNum) + "   ʤ������: " + to_string(p.winNum) + "   ʤ��: " + to_string(1000 * p.winNum / p.totNum / 10) + "." + to_string((1000 * p.winNum / p.totNum) % 10) + "%#";


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
	verStr += to_string(p.winNum) + "#";//ս��ʤ������
	verStr += to_string(p.totNum) + "#";//ս���ܳ���
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
		cout << "�������ݳɹ�....." << endl;
		Sleep(500);
	}
	string strVer = "";
	playerNum = 0;

	ifs >> playerNum;
	//cout << playerNum;

	for (int i = 1; i <= playerNum; i++) {
		ifs >> player[i].name;//�û���
		ifs >> player[i].id;//�˺�
		ifs >> player[i].key;//����
		ifs >> player[i].number;//��������
		ifs >> player[i].winNum;//ʤ����
		ifs >> player[i].totNum;//�ܳ���
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

	verStr = "";
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

void playerOutput(SOCKET & recvClientSocket, Player p) {//�û���Ϣ���
	string verStr = "";
	int sLen;

	cout << "=================�û���Ϣ==================" << endl;

	verStr = playerInformation(p);
	Print(verStr);
	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	cout << "=============================================" <<  endl;
}

void Update(string& verStr, Pokemon& pok1, Pokemon& pok2) {//������ս�����������״̬
	verStr += "==================================================================================";
	verStr += "#";
	verStr += pok1._name + ":  ����ֵ: " + to_string(pok1._nowHp) + "    ������ : " + to_string(pok1._nowAtk) + "    ������ : " + to_string(pok1._nowDef) + "    �ٶ� : " + to_string(pok1._nowSp);
	verStr += "#";
	verStr += pok2._name + ":  ����ֵ: " + to_string(pok2._nowHp) + "    ������ : " + to_string(pok2._nowAtk) + "    ������ : " + to_string(pok2._nowDef) + "    �ٶ� : " + to_string(pok2._nowSp);
	verStr += "#";
	verStr += "==================================================================================";
	verStr += "###";
}

void getSkill(string& verStr, Pokemon& pok) { //�Ѿ���pok�ļ��ܼӹ���һ���ַ��������뵽verStr��
	verStr += "��" + pok._name + "�ͷż�����!"; verStr += "#";
	for (int i = 0; i <= 4; i++) {
		if (pok._level >= (i - 1) * 5 && pok._nowPp[i]) {//���ü��ܺ�ʹ�ô���
			verStr += "����" + to_string(i) + ": " + pok._race[pok._raceId]->skillName[i] + ",ʣ��ʹ�ô���";
			if (i == 0) verStr += "����";
			else verStr += to_string(pok._nowPp[i]);
			verStr += "#";
		}
	}
	verStr += "��ѡ����Ҫʹ�õļ��ܱ��:";
	verStr += "#";
}


//������
void UpgradeCompetition(SOCKET & recvClientSocket, Player & p) {//������Ӯ��ֻ�����Ӿ���ֵ
	string verStr = "";
	//��������Լ���ӵ�����еľ��飬Ȼ��ѡ��һ�� 
	verStr += "�û�ӵ�еľ����б�:"; verStr += "#";
	for (int i = 1; i <= p.number; i++) {
		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "��������: " + p.pok[i]._name + "    ��������: " + p.pok[i].getRaceName() + "    ����ȼ�: " + to_string(p.pok[i]._level); 
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   ����ֵ: " + to_string(p.pok[i]._hp) + "    ������: " + to_string(p.pok[i]._atk) + "    ������: " + to_string(p.pok[i]._def) + "    �ٶ�: " + to_string(p.pok[i]._sp); 
		verStr += "##";
	}
	verStr += "��ѡ����Ҫ�μ��������ľ���:";
	verStr += "#";

	int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//�����û�ѡ��ľ�����
	Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	int pokemonId = string_to_int(recvBuf);//�û�ѡ��ľ���

	Pokemon &pok1 = p.pok[pokemonId];


	//ϵͳ���⾫�飬���ĸ����ȼ�����ѡ��һ��
	verStr = "��ս�������б�:";
	int _level = pok1._level;//��׼�ȼ��������б�ľ��鰴������ȼ��趨
	verStr += ":"; verStr += "#";
	Pokemon pok[5];
	for (int i = 1; i <= 4; i++) {
		getPokemon(pok[i], rand() % 16);
		int Level = getRand(max(1, _level - 3), min(15, _level + 2));//���һ���ȼ�
		pok[i].gainExp(pok1.getExpValue(Level));//ʹ�����������趨�ĵȼ�

		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "��������: " + pok[i]._name + "    ��������: " + pok[i].getRaceName() + "    ����ȼ�: " + to_string(pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   ����ֵ: " + to_string(pok[i]._hp) + "    ������: " + to_string(pok[i]._atk) + "    ������: " + to_string(pok[i]._def) + "    �ٶ�: " + to_string(pok[i]._sp);
		verStr += "##";
	}
	verStr += "��ѡ������������Ҫս���ľ���:";
	verStr += "#";

	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//�����û�ѡ��ľ�����
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	int pokemonId2 = string_to_int(recvBuf);//�û�ѡ��ľ���

	Pokemon &pok2 = pok[pokemonId2];
	



	//p1 vs p2
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	
	int autoFight = string_to_int(recvBuf) - 1;//0Ϊ�ֶ�������1Ϊ�Զ�ս��

	int tim1 = 0, tim2 = 0;

	if (autoFight == 1) {//�Զ�ս�����Ƚϼ򵥣�����Ҫѯ���û�ʹ���Ǹ�����
		verStr = "";
		cout << "===����" << pok1._name << "=== ��ս ===����" << pok2._name << "===" << endl;
		verStr += "===����" + pok1._name + "=== ��ս ===����" + pok2._name + "===";
		verStr += "#";

		//����ǰ��ԭ
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//ս�����ٶ��йأ��ٶȿ��ʾ���������
		Update(verStr, pok1, pok2);
		while (true) {
			while (tim1 < pok1._nowSp && tim2 < pok2._nowSp) {//û���ͷ�
				tim1++;
				tim2++;
			}
			if (tim1 >= pok1._nowSp && tim2 >= pok2._nowSp) {//�������ͷţ�����sp��˭��
				if (pok1._nowSp >= pok2._nowSp) {
					if (pok1.attack(verStr, pok2, autoFight)) break;
					Update(verStr, pok1, pok2);

					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);
				}
				else {
					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);

					if (pok1.attack(verStr, pok2, autoFight)) break;
					Update(verStr, pok1, pok2);
				}
				tim1 = tim2 = 0;
			}
			else if (tim1 >= pok1._nowSp) {//pok2�ͷ�
				if (pok2.attack(verStr, pok1, true)) break;
				Update(verStr, pok1, pok2);
				tim1 = 0;
			}
			else if (tim2 >= pok2._nowSp) {//pok1�ͷ�
				if (pok1.attack(verStr, pok2, autoFight)) break;
				Update(verStr, pok1, pok2);
				tim2 = 0;
			}
		}
		Update(verStr, pok1, pok2);
		if (pok1._nowHp > 0) {
			p.totNum++;
			p.winNum++;
			cout << "��ϲ�����ľ���" << pok1._name << "��ʤ!" << endl;
			verStr += "��ϲ�����ľ���" + pok1._name + "��ʤ!";
			verStr += "#";
			//��ȡ����ֵ�����ݾ�����������������ʤ��ʧ�ܡ��ȼ������ò�ͬ�ľ���
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 5 * 8);
			else if (sub == -1) pok1.gainExp(verStr, 5 * 6);
			else if (sub == 0) pok1.gainExp(verStr, 5 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 5 * 2);
			else pok1.gainExp(verStr, 5 * 1);
		}
		else {
			p.totNum++;
			cout << "��ϧ�����ľ���" << pok1._name << "ս��!" << endl;
			verStr += "��ϧ�����ľ���" + pok1._name + "ս��!";
			verStr += "#";

			//��ȡ����ֵ�����ݾ�����������������ʤ��ʧ�ܡ��ȼ������ò�ͬ�ľ���
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 3 * 5);
			else if (sub == -1) pok1.gainExp(verStr, 3 * 4);
			else if (sub == 0) pok1.gainExp(verStr, 3 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 3 * 2);
			else pok1.gainExp(verStr, 3 * 1);
		}
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
	}
	else {//�ֶ�ʩ��ս�����Ƚϸ���
		verStr = "";
		cout << "===����" << pok1._name << "=== ��ս ===����" << pok2._name << "===" << endl;
		verStr += "===����" + pok1._name + "=== ��ս ===����" + pok2._name + "===";
		verStr += "#";

		//����ǰ��ԭ
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//ս�����ٶ��йأ��ٶȿ��ʾ���������
		Update(verStr, pok1, pok2);
		while (true) {//��ÿ��pok1ʩ��ǰ����Ҫ���洢���ַ������͹�ȥ��Ȼ��ѯ�ʼ��ܱ��
			while (tim1 < pok1._nowSp && tim2 < pok2._nowSp) {
				tim1++;
				tim2++;
			}
			if (tim1 >= pok1._nowSp && tim2 >= pok2._nowSp) {
				if (pok1._nowSp >= pok2._nowSp) {
					Sleep(100);
					getSkill(verStr, pok1);
					sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
					Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
					verStr = "";
					int skillId = string_to_int(recvBuf);
					if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
					Update(verStr, pok1, pok2);

					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);
				}
				else {
					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);

					Sleep(300);
					getSkill(verStr, pok1);
					sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
					Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
					verStr = "";
					int skillId = string_to_int(recvBuf);
					if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
					Update(verStr, pok1, pok2);
				}
				tim1 = tim2 = 0;
			}
			else if (tim1 >= pok1._nowSp) {
				if (pok2.attack(verStr, pok1, true)) break;
				Update(verStr, pok1, pok2);
				tim1 = 0;
			}
			else if (tim2 >= pok2._nowSp) {
				Sleep(300);
				getSkill(verStr, pok1);
				sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
				Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
				verStr = "";
				int skillId = string_to_int(recvBuf);
				if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
				Update(verStr, pok1, pok2);
				tim2 = 0;
			}
		}

		Update(verStr, pok1, pok2);
		verStr += "������1ȷ�Ͻ���ս��"; verStr += "#";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);

		verStr = "����";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		verStr = "";

		if (pok1._nowHp > 0) {//��ʤ����ȡ����
			p.totNum++;
			p.winNum++;
			cout << "��ϲ�����ľ���" << pok1._name << "��ʤ!" << endl;
			verStr += "��ϲ�����ľ���" + pok1._name + "��ʤ!";
			verStr += "#";
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 5 * 8);
			else if (sub == -1) pok1.gainExp(verStr, 5 * 6);
			else if (sub == 0) pok1.gainExp(verStr, 5 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 5 * 2);
			else pok1.gainExp(verStr, 5 * 1);
		}
		else {//ʧ�ܣ��������
			p.totNum++;
			cout << "��ϧ�����ľ���" << pok1._name << "ս��!" << endl;
			verStr += "��ϧ�����ľ���" + pok1._name + "ս��!";
			verStr += "#";

			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 3 * 5);
			else if (sub == -1) pok1.gainExp(verStr, 3 * 4);
			else if (sub == 0) pok1.gainExp(verStr, 3 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 3 * 2);
			else pok1.gainExp(verStr, 3 * 1);
		}
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
	}

	playerOutputToFile();//�û����£����û���Ϣ������ļ�

}

//������
void DuelCompetition(SOCKET & recvClientSocket, Player & p) {
	//Ӯ�˻�þ��飬����ϵͳѡ����ֻ���飬�Լ�ѡ��һֻ�����ͳ������������ĿΪ0��ϵͳ�ͳ�һֻ
	//�󲿷�ע��ͬ������
	string verStr = "";

	verStr += "�û�ӵ�еľ����б�:"; verStr += "#";
	for (int i = 1; i <= p.number; i++) {
		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "��������: " + p.pok[i]._name + "    ��������: " + p.pok[i].getRaceName() + "    ����ȼ�: " + to_string(p.pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   ����ֵ: " + to_string(p.pok[i]._hp) + "    ������: " + to_string(p.pok[i]._atk) + "    ������: " + to_string(p.pok[i]._def) + "    �ٶ�: " + to_string(p.pok[i]._sp);
		verStr += "##";
	}
	verStr += "��ѡ����Ҫ�μӾ������ľ���:";
	verStr += "#";

	int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//�����û�ѡ��ľ�����
	Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	int pokemonId = string_to_int(recvBuf);//�û�ѡ��ľ���

	Pokemon &pok1 = p.pok[pokemonId];

	verStr = "��ս�������б�:";
	int _level = pok1._level;//��׼�ȼ��������б�ľ��鰴������ȼ��趨
	verStr += ":"; verStr += "#";
	Pokemon pok[5];
	for (int i = 1; i <= 4; i++) {
		getPokemon(pok[i], rand() % 16);
		int Level = getRand(max(1, _level - 3), min(15, _level + 2));//���һ���ȼ�
		pok[i].gainExp(pok1.getExpValue(Level));//ʹ�����������趨�ĵȼ�

		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "��������: " + pok[i]._name + "    ��������: " + pok[i].getRaceName() + "    ����ȼ�: " + to_string(pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   ����ֵ: " + to_string(pok[i]._hp) + "    ������: " + to_string(pok[i]._atk) + "    ������: " + to_string(pok[i]._def) + "    �ٶ�: " + to_string(pok[i]._sp);
		verStr += "##";
	}
	verStr += "��ѡ�����ھ�����Ҫս���ľ���:";
	verStr += "#";

	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//�����û�ѡ��ľ�����
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	int pokemonId2 = string_to_int(recvBuf);//�û�ѡ��ľ���

	Pokemon &pok2 = pok[pokemonId2];




	//p1 vs p2
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

	int autoFight = string_to_int(recvBuf) - 1;//0Ϊ�ֶ�������1Ϊ�Զ�ս��

	int tim1 = 0, tim2 = 0;

	if (autoFight == 1) {//�Զ�ս��
		verStr = "";
		cout << "===����" << pok1._name << "=== ��ս ===����" << pok2._name << "===" << endl;
		verStr += "===����" + pok1._name + "=== ��ս ===����" + pok2._name + "===";
		verStr += "#";

		//����ǰ��ԭ
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//ս�����ٶ��йأ��ٶȿ��ʾ���������
		Update(verStr, pok1, pok2);
		while (true) {
			while (tim1 < pok1._nowSp && tim2 < pok2._nowSp) {
				tim1++;
				tim2++;
			}
			if (tim1 >= pok1._nowSp && tim2 >= pok2._nowSp) {
				if (pok1._nowSp >= pok2._nowSp) {
					if (pok1.attack(verStr, pok2, autoFight)) break;
					Update(verStr, pok1, pok2);

					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);
				}
				else {
					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);

					if (pok1.attack(verStr, pok2, autoFight)) break;
					Update(verStr, pok1, pok2);
				}
				tim1 = tim2 = 0;
			}
			else if (tim1 >= pok1._nowSp) {
				if (pok2.attack(verStr, pok1, true)) break;
				Update(verStr, pok1, pok2);
				tim1 = 0;
			}
			else if (tim2 >= pok2._nowSp) {
				if (pok1.attack(verStr, pok2, autoFight)) break;
				Update(verStr, pok1, pok2);
				tim2 = 0;
			}
		}
		Update(verStr, pok1, pok2);
		//�ж�ʤ������ȡ����
		if (pok1._nowHp > 0) {
			p.totNum++;
			p.winNum++;
			cout << "��ϲ�����ľ���" << pok1._name << "��ʤ!" << endl;
			verStr += "��ϲ�����ľ���" + pok1._name + "��ʤ!";
			verStr += "##";
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 5 * 8 * 2);
			else if (sub == -1) pok1.gainExp(verStr, 5 * 6 * 2);
			else if (sub == 0) pok1.gainExp(verStr, 5 * 3 * 2);
			else if (sub == 1) pok1.gainExp(verStr, 5 * 2 * 2);
			else pok1.gainExp(verStr, 5 * 1);
		}
		else {
			p.totNum++;
			cout << "��ϧ�����ľ���" << pok1._name << "ս��!" << endl;
			verStr += "��ϧ�����ľ���" + pok1._name + "ս��!";
			verStr += "##";

			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 3 * 5 * 2);
			else if (sub == -1) pok1.gainExp(verStr, 3 * 4 * 2);
			else if (sub == 0) pok1.gainExp(verStr, 3 * 3 * 2);
			else if (sub == 1) pok1.gainExp(verStr, 3 * 2 * 2);
			else pok1.gainExp(verStr, 3 * 1);

		}
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		verStr = "";
		if (pok1._nowHp > 0) {//��ʤ���õ�սʤ�ľ���

			verStr = "?";

			verStr += "#";
			verStr += "��ϲ�����ھ�������ȡ����ʤ��������˾���" + pok2._name + "!";
			verStr += "#";
			p.number++;
			p.pok[p.number] = Pokemon(pok2);
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
		else {//ʧ�ܣ�����һ�����飬�ٴν���

			verStr = "~";
			verStr += "#";
			verStr += "��ϧ�����ھ������б����ܣ�����Ҫ��������һ������,ϵͳѡ�������о���:";
			verStr += "#";

			bool flag[105] = { false }; Mem(flag);
			int seq[5];
			int num = min(p.number, 3);
			for (int i = 1; i <= num; i++) {
				int id = rand() % p.number + 1;
				while (flag[id]) id = rand() % p.number + 1;
				flag[id] = true;
				seq[i] = id;

				verStr += "\t";
				verStr += to_string(i) + ":    ||   " + "��������: " + p.pok[id]._name + "    ��������: " + p.pok[id].getRaceName() + "    ����ȼ�: " + to_string(p.pok[id]._level);
				verStr += "#";

				verStr += "\t";
				verStr += "      ||   ����ֵ: " + to_string(p.pok[id]._hp) + "    ������: " + to_string(p.pok[id]._atk) + "    ������: " + to_string(p.pok[id]._def) + "    �ٶ�: " + to_string(p.pok[id]._sp);
				verStr += "##";
			}

			verStr += "��ѡ����Ҫ�����ľ���:"; verStr += "#";
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

			Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
			int pos = string_to_int(recvBuf);

			cout << "here" << recvBuf << endl;

			verStr = "";

			verStr += "�������˾���" + p.pok[seq[pos]]._name;
			verStr += "#";

			for (int i = seq[pos]; i < p.number; i++) {
				p.pok[i] = Pokemon(p.pok[i + 1]);
			}
			p.number--;
			if (p.number == 0) {
				p.number++;
				getPokemon(p.pok[p.number], rand() % 16);
			}
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
	}
	else {//�ֶ�ʩ��ս��
		verStr = "";
		cout << "===����" << pok1._name << "=== ��ս ===����" << pok2._name << "===" << endl;
		verStr += "===����" + pok1._name + "=== ��ս ===����" + pok2._name + "===";
		verStr += "#";

		//����ǰ��ԭ
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//ս�����ٶ��йأ��ٶȿ��ʾ���������
		Update(verStr, pok1, pok2);
		while (true) {
			while (tim1 < pok1._nowSp && tim2 < pok2._nowSp) {
				tim1++;
				tim2++;
			}
			if (tim1 >= pok1._nowSp && tim2 >= pok2._nowSp) {
				if (pok1._nowSp >= pok2._nowSp) {
					Sleep(100);
					getSkill(verStr, pok1);
					sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
					Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
					verStr = "";
					int skillId = string_to_int(recvBuf);
					if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
					Update(verStr, pok1, pok2);

					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);
				}
				else {
					if (pok2.attack(verStr, pok1, true)) break;
					Update(verStr, pok1, pok2);

					Sleep(300);
					getSkill(verStr, pok1);
					sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
					Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
					verStr = "";
					int skillId = string_to_int(recvBuf);
					if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
					Update(verStr, pok1, pok2);
				}
				tim1 = tim2 = 0;
			}
			else if (tim1 >= pok1._nowSp) {
				if (pok2.attack(verStr, pok1, true)) break;
				Update(verStr, pok1, pok2);
				tim1 = 0;
			}
			else if (tim2 >= pok2._nowSp) {
				Sleep(300);
				getSkill(verStr, pok1);
				sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
				Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
				verStr = "";
				int skillId = string_to_int(recvBuf);
				if (pok1.attack(verStr, pok2, autoFight, skillId)) break;
				Update(verStr, pok1, pok2);
				tim2 = 0;
			}
		}

		Update(verStr, pok1, pok2);
		verStr += "������1ȷ�Ͻ���ս��"; verStr += "#";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);

		verStr = "����";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		verStr = "";

		if (pok1._nowHp > 0) {//��ʤ�����һ������

			verStr = "?";

			verStr += "#";
			verStr += "��ϲ�����ھ�������ȡ����ʤ��������˾���" + pok2._name + "!";
			verStr += "#";
			p.number++;
			p.pok[p.number] = Pokemon(pok2);
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
		else {//ʧ�ܣ�ѡһ�������ͳ�ȥ

			verStr = "~";
			verStr += "#";
			verStr += "��ϧ�����ھ������б����ܣ�����Ҫ��������һ������,ϵͳѡ�������о���:";
			verStr += "#";

			bool flag[105] = { false }; Mem(flag);
			int seq[5];
			int num = min(p.number, 3);
			for (int i = 1; i <= num; i++) {
				int id = rand() % p.number + 1;
				while (flag[id]) id = rand() % p.number + 1;
				flag[id] = true;
				seq[i] = id;

				verStr += "\t";
				verStr += to_string(i) + ":    ||   " + "��������: " + p.pok[id]._name + "    ��������: " + p.pok[id].getRaceName() + "    ����ȼ�: " + to_string(p.pok[id]._level);
				verStr += "#";

				verStr += "\t";
				verStr += "      ||   ����ֵ: " + to_string(p.pok[id]._hp) + "    ������: " + to_string(p.pok[id]._atk) + "    ������: " + to_string(p.pok[id]._def) + "    �ٶ�: " + to_string(p.pok[id]._sp);
				verStr += "##";
			}

			verStr += "��ѡ����Ҫ�����ľ���:"; verStr += "#";
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

			Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
			int pos = string_to_int(recvBuf);


			verStr = "";

			verStr += "�������˾���" + p.pok[seq[pos]]._name;
			verStr += "#";

			for (int i = seq[pos]; i < p.number; i++) {
				p.pok[i] = Pokemon(p.pok[i + 1]);
			}
			p.number--;
			if (p.number == 0) {
				p.number++;
				getPokemon(p.pok[p.number], rand() % 16);
			}
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
	}

	playerOutputToFile();//�û����£����û���Ϣ������ļ�
}

void Login(SOCKET & recvClientSocket) {//�û���¼
	cout << endl << "�û���¼" << endl;
	  
	int AccExist = 0, KeyRight = 0;//�ֱ�Ϊ�˺��Ƿ���ڣ������Ƿ���ȷ
	int reLen;
	//�ӿͻ��˶�ȡ�˺�id
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
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
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
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
	playerOutput(recvClientSocket, player[AccExist]);//�����ǰ�û���Ϣ

	player[AccExist].loginStatus = true;//���õ�½״̬Ϊtrue

	while (true) {//���û�ִ�в���
		 
		Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
		//�����������ȴ���������
		cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;

		string verStr = "";

		if (strcmp(recvBuf, "1") == 0) {//������
			UpgradeCompetition(recvClientSocket,player[AccExist]);
		}
		else if (strcmp(recvBuf, "2") == 0) {//������
			DuelCompetition(recvClientSocket,player[AccExist]);
		}
		else if (strcmp(recvBuf, "3") == 0) {//��ѯ������Ϣ
			playerOutput(recvClientSocket, player[AccExist]);
		}
		else if (strcmp(recvBuf, "4") == 0) {//��ѯ�����û�
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) if (player[i].loginStatus) {
				verStr += "��" + to_string(++tot) + "���û�:    ||   " + "�û���: " + player[i].name + "  �û��˺�: " + player[i].id;
				verStr += "#";
			}
			verStr += "��" + to_string(tot) + "�������û�";
			verStr += "##";
			
			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "5") == 0) {//��ѯ�����û��ľ���

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
		else if (strcmp(recvBuf, "6") == 0) {//��ѯ�û�ʤ��
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) {
				verStr += "�û�" + player[i].name + ":    ||   ";

				if (player[i].totNum == 0) verStr += "ս������: 0   ʤ������: 0   ʤ��: ��#";//û�вμӹ��������޷�����ʤ��
				else verStr += "ս������: " + to_string(player[i].totNum) + "   ʤ������: " + to_string(player[i].winNum) + "   ʤ��: " + to_string(1000 * player[i].winNum / player[i].totNum / 10) + "." + to_string((1000 * player[i].winNum / player[i].totNum) % 10) + "%#";

				verStr += "#";
			}
			verStr += "##";

			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "7") == 0) {//�޸�����
			Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
			if (strcmp(recvBuf, player[AccExist].key.c_str()) == 0) {
				int sLen = send(recvClientSocket, (char*)to_string(1).c_str(), to_string(1).length(), 0);

				Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
				player[AccExist].changeKey(recvBuf);
			}
			else {
				sLen = send(recvClientSocket, (char*)to_string(0).c_str(), to_string(0).length(), 0);
			}
		}
		else if (strcmp(recvBuf, "8") == 0) {//�û��ǳ����˳�
			player[AccExist].loginStatus = false;
			cout << "�˳���¼" << endl;
			break;
		}
		else if (strcmp(recvBuf, "0") == 0) {//���ǳ���ֻ�˳�
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
	  
	string NAME, ID, KEY;
	int reLen;

	++playerNum;

	//�ӿͻ��˶�ȡ�û���
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "�û���: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	NAME = recvBuf;

	//�ӿͻ��˶�ȡ�û��˺�
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "�û��˺�: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	ID = recvBuf;
	
	//�ӿͻ��˶�ȡ�û�����
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "�û�����: " << recvBuf << endl;
	cout << "���յ���Ϣ: " << recvBuf << "               ���Կͻ���:" << recvClientSocket << endl;
	KEY = recvBuf;

	//�ӿͻ����ٴζ�ȡ�û�����
	Mem(recvBuf); reLen =  recv(recvClientSocket, recvBuf, 65536, 0);;
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

	playerOutputToFile();//���û���Ϣ������ļ�

}

//�����߳�
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET *recvClientSocket = (SOCKET*)lpParameter;
	//ѭ������+�ж�
	while (true) {
		  
		Mem(recvBuf); int reLen = recv(*recvClientSocket, recvBuf, 65536, 0);

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

	//for (int i = 1; i <= 5; i++) cout << rand() << endl;

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
		  
		int reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
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
