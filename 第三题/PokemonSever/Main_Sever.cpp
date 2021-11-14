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
//#include "battleController.h"
using namespace std;


char recvBuf[65536];//存储从客户端接收的字符串
//string verStr;//用于存储向客户端发送的字符串

Player player[105];//用户类数组
int playerNum;//用户的数量

int string_to_int(char *s) {//从string到int
	int len = strlen(s);
	int val = 0;
	for (int i = 0; i < len; i++) {
		val = val * 10 + s[i] - '0';
	}
	return val;
}

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
	verStr += "宠物个数勋章: " + p.numberMedal() + "#";
	verStr += "高级宠物勋章: " + p.superPokemonMedal() + "#"; 
	if (p.totNum == 0) verStr += "战斗场数: 0   胜利场数: 0   胜率: 无#";//没有参加过比赛，无法计算胜率
	else verStr += "战斗场数: " + to_string(p.totNum) + "   胜利场数: " + to_string(p.winNum) + "   胜率: " + to_string(1000 * p.winNum / p.totNum / 10) + "." + to_string((1000 * p.winNum / p.totNum) % 10) + "%#";


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
	verStr += to_string(p.winNum) + "#";//战斗胜利场数
	verStr += to_string(p.totNum) + "#";//战斗总场数
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
		cout << "载入数据成功....." << endl;
		Sleep(500);
	}
	string strVer = "";
	playerNum = 0;

	ifs >> playerNum;
	//cout << playerNum;

	for (int i = 1; i <= playerNum; i++) {
		ifs >> player[i].name;//用户名
		ifs >> player[i].id;//账号
		ifs >> player[i].key;//密码
		ifs >> player[i].number;//精灵数量
		ifs >> player[i].winNum;//胜场数
		ifs >> player[i].totNum;//总场数
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

	verStr = "";
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

void playerOutput(SOCKET & recvClientSocket, Player p) {//用户信息输出
	string verStr = "";
	int sLen;

	cout << "=================用户信息==================" << endl;

	verStr = playerInformation(p);
	Print(verStr);
	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	cout << "=============================================" <<  endl;
}

void Update(string& verStr, Pokemon& pok1, Pokemon& pok2) {//用于在战斗中输出精灵状态
	verStr += "==================================================================================";
	verStr += "#";
	verStr += pok1._name + ":  生命值: " + to_string(pok1._nowHp) + "    攻击力 : " + to_string(pok1._nowAtk) + "    防御力 : " + to_string(pok1._nowDef) + "    速度 : " + to_string(pok1._nowSp);
	verStr += "#";
	verStr += pok2._name + ":  生命值: " + to_string(pok2._nowHp) + "    攻击力 : " + to_string(pok2._nowAtk) + "    防御力 : " + to_string(pok2._nowDef) + "    速度 : " + to_string(pok2._nowSp);
	verStr += "#";
	verStr += "==================================================================================";
	verStr += "###";
}

void getSkill(string& verStr, Pokemon& pok) { //把精灵pok的技能加工成一个字符串，加入到verStr中
	verStr += "该" + pok._name + "释放技能了!"; verStr += "#";
	for (int i = 0; i <= 4; i++) {
		if (pok._level >= (i - 1) * 5 && pok._nowPp[i]) {//可用技能和使用次数
			verStr += "技能" + to_string(i) + ": " + pok._race[pok._raceId]->skillName[i] + ",剩余使用次数";
			if (i == 0) verStr += "无限";
			else verStr += to_string(pok._nowPp[i]);
			verStr += "#";
		}
	}
	verStr += "请选择你要使用的技能编号:";
	verStr += "#";
}


//升级赛
void UpgradeCompetition(SOCKET & recvClientSocket, Player & p) {//无论输赢，只是增加经验值
	string verStr = "";
	//首先输出自己所拥有所有的精灵，然后选择一个 
	verStr += "用户拥有的精灵列表:"; verStr += "#";
	for (int i = 1; i <= p.number; i++) {
		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "精灵名称: " + p.pok[i]._name + "    精灵种族: " + p.pok[i].getRaceName() + "    精灵等级: " + to_string(p.pok[i]._level); 
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   生命值: " + to_string(p.pok[i]._hp) + "    攻击力: " + to_string(p.pok[i]._atk) + "    防御力: " + to_string(p.pok[i]._def) + "    速度: " + to_string(p.pok[i]._sp); 
		verStr += "##";
	}
	verStr += "请选择你要参加升级赛的精灵:";
	verStr += "#";

	int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//接收用户选择的精灵编号
	Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	int pokemonId = string_to_int(recvBuf);//用户选择的精灵

	Pokemon &pok1 = p.pok[pokemonId];


	//系统虚拟精灵，共四个，等级相差不大，选择一个
	verStr = "可战斗精灵列表:";
	int _level = pok1._level;//标准等级，虚拟列表的精灵按照这个等级设定
	verStr += ":"; verStr += "#";
	Pokemon pok[5];
	for (int i = 1; i <= 4; i++) {
		getPokemon(pok[i], rand() % 16);
		int Level = getRand(max(1, _level - 3), min(15, _level + 2));//随机一个等级
		pok[i].gainExp(pok1.getExpValue(Level));//使精灵升级到设定的等级

		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "精灵名称: " + pok[i]._name + "    精灵种族: " + pok[i].getRaceName() + "    精灵等级: " + to_string(pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   生命值: " + to_string(pok[i]._hp) + "    攻击力: " + to_string(pok[i]._atk) + "    防御力: " + to_string(pok[i]._def) + "    速度: " + to_string(pok[i]._sp);
		verStr += "##";
	}
	verStr += "请选择你在升级赛要战斗的精灵:";
	verStr += "#";

	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//接收用户选择的精灵编号
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	int pokemonId2 = string_to_int(recvBuf);//用户选择的精灵

	Pokemon &pok2 = pok[pokemonId2];
	



	//p1 vs p2
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	
	int autoFight = string_to_int(recvBuf) - 1;//0为手动操作，1为自动战斗

	int tim1 = 0, tim2 = 0;

	if (autoFight == 1) {//自动战斗，比较简单，不需要询问用户使用那个技能
		verStr = "";
		cout << "===精灵" << pok1._name << "=== 对战 ===精灵" << pok2._name << "===" << endl;
		verStr += "===精灵" + pok1._name + "=== 对战 ===精灵" + pok2._name + "===";
		verStr += "#";

		//比赛前复原
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//战斗跟速度有关，速度快表示攻击间隔低
		Update(verStr, pok1, pok2);
		while (true) {
			while (tim1 < pok1._nowSp && tim2 < pok2._nowSp) {//没人释放
				tim1++;
				tim2++;
			}
			if (tim1 >= pok1._nowSp && tim2 >= pok2._nowSp) {//都可以释放，按照sp看谁先
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
			else if (tim1 >= pok1._nowSp) {//pok2释放
				if (pok2.attack(verStr, pok1, true)) break;
				Update(verStr, pok1, pok2);
				tim1 = 0;
			}
			else if (tim2 >= pok2._nowSp) {//pok1释放
				if (pok1.attack(verStr, pok2, autoFight)) break;
				Update(verStr, pok1, pok2);
				tim2 = 0;
			}
		}
		Update(verStr, pok1, pok2);
		if (pok1._nowHp > 0) {
			p.totNum++;
			p.winNum++;
			cout << "恭喜，您的精灵" << pok1._name << "获胜!" << endl;
			verStr += "恭喜，您的精灵" + pok1._name + "获胜!";
			verStr += "#";
			//获取经验值，根据决斗赛、升级赛、获胜、失败、等级差设置不同的经验
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 5 * 8);
			else if (sub == -1) pok1.gainExp(verStr, 5 * 6);
			else if (sub == 0) pok1.gainExp(verStr, 5 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 5 * 2);
			else pok1.gainExp(verStr, 5 * 1);
		}
		else {
			p.totNum++;
			cout << "可惜，您的精灵" << pok1._name << "战败!" << endl;
			verStr += "可惜，您的精灵" + pok1._name + "战败!";
			verStr += "#";

			//获取经验值，根据决斗赛、升级赛、获胜、失败、等级差设置不同的经验
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 3 * 5);
			else if (sub == -1) pok1.gainExp(verStr, 3 * 4);
			else if (sub == 0) pok1.gainExp(verStr, 3 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 3 * 2);
			else pok1.gainExp(verStr, 3 * 1);
		}
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
	}
	else {//手动施法战斗，比较复杂
		verStr = "";
		cout << "===精灵" << pok1._name << "=== 对战 ===精灵" << pok2._name << "===" << endl;
		verStr += "===精灵" + pok1._name + "=== 对战 ===精灵" + pok2._name + "===";
		verStr += "#";

		//比赛前复原
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//战斗跟速度有关，速度快表示攻击间隔低
		Update(verStr, pok1, pok2);
		while (true) {//在每次pok1施法前，需要将存储的字符串发送过去，然后询问技能编号
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
		verStr += "请输入1确认结束战斗"; verStr += "#";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);

		verStr = "结束";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		verStr = "";

		if (pok1._nowHp > 0) {//获胜，获取经验
			p.totNum++;
			p.winNum++;
			cout << "恭喜，您的精灵" << pok1._name << "获胜!" << endl;
			verStr += "恭喜，您的精灵" + pok1._name + "获胜!";
			verStr += "#";
			int sub = pok1._level - pok2._level;
			if (sub == -2) pok1.gainExp(verStr, 5 * 8);
			else if (sub == -1) pok1.gainExp(verStr, 5 * 6);
			else if (sub == 0) pok1.gainExp(verStr, 5 * 3);
			else if (sub == 1) pok1.gainExp(verStr, 5 * 2);
			else pok1.gainExp(verStr, 5 * 1);
		}
		else {//失败，经验较少
			p.totNum++;
			cout << "可惜，您的精灵" << pok1._name << "战败!" << endl;
			verStr += "可惜，您的精灵" + pok1._name + "战败!";
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

	playerOutputToFile();//用户更新，将用户信息输出到文件

}

//决斗赛
void DuelCompetition(SOCKET & recvClientSocket, Player & p) {
	//赢了获得精灵，输了系统选择三只精灵，自己选择一只精灵送出，如果精灵数目为0，系统送出一只
	//大部分注释同升级赛
	string verStr = "";

	verStr += "用户拥有的精灵列表:"; verStr += "#";
	for (int i = 1; i <= p.number; i++) {
		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "精灵名称: " + p.pok[i]._name + "    精灵种族: " + p.pok[i].getRaceName() + "    精灵等级: " + to_string(p.pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   生命值: " + to_string(p.pok[i]._hp) + "    攻击力: " + to_string(p.pok[i]._atk) + "    防御力: " + to_string(p.pok[i]._def) + "    速度: " + to_string(p.pok[i]._sp);
		verStr += "##";
	}
	verStr += "请选择你要参加决斗赛的精灵:";
	verStr += "#";

	int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//接收用户选择的精灵编号
	Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	int pokemonId = string_to_int(recvBuf);//用户选择的精灵

	Pokemon &pok1 = p.pok[pokemonId];

	verStr = "可战斗精灵列表:";
	int _level = pok1._level;//标准等级，虚拟列表的精灵按照这个等级设定
	verStr += ":"; verStr += "#";
	Pokemon pok[5];
	for (int i = 1; i <= 4; i++) {
		getPokemon(pok[i], rand() % 16);
		int Level = getRand(max(1, _level - 3), min(15, _level + 2));//随机一个等级
		pok[i].gainExp(pok1.getExpValue(Level));//使精灵升级到设定的等级

		verStr += "\t";
		verStr += to_string(i) + ":    ||   " + "精灵名称: " + pok[i]._name + "    精灵种族: " + pok[i].getRaceName() + "    精灵等级: " + to_string(pok[i]._level);
		verStr += "#";

		verStr += "\t";
		verStr += "      ||   生命值: " + to_string(pok[i]._hp) + "    攻击力: " + to_string(pok[i]._atk) + "    防御力: " + to_string(pok[i]._def) + "    速度: " + to_string(pok[i]._sp);
		verStr += "##";
	}
	verStr += "请选择你在决斗赛要战斗的精灵:";
	verStr += "#";

	sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

	//接收用户选择的精灵编号
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	int pokemonId2 = string_to_int(recvBuf);//用户选择的精灵

	Pokemon &pok2 = pok[pokemonId2];




	//p1 vs p2
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

	int autoFight = string_to_int(recvBuf) - 1;//0为手动操作，1为自动战斗

	int tim1 = 0, tim2 = 0;

	if (autoFight == 1) {//自动战斗
		verStr = "";
		cout << "===精灵" << pok1._name << "=== 对战 ===精灵" << pok2._name << "===" << endl;
		verStr += "===精灵" + pok1._name + "=== 对战 ===精灵" + pok2._name + "===";
		verStr += "#";

		//比赛前复原
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//战斗跟速度有关，速度快表示攻击间隔低
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
		//判断胜负并获取经验
		if (pok1._nowHp > 0) {
			p.totNum++;
			p.winNum++;
			cout << "恭喜，您的精灵" << pok1._name << "获胜!" << endl;
			verStr += "恭喜，您的精灵" + pok1._name + "获胜!";
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
			cout << "可惜，您的精灵" << pok1._name << "战败!" << endl;
			verStr += "可惜，您的精灵" + pok1._name + "战败!";
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
		if (pok1._nowHp > 0) {//获胜，得到战胜的精灵

			verStr = "?";

			verStr += "#";
			verStr += "恭喜，您在决斗赛中取得了胜利，获得了精灵" + pok2._name + "!";
			verStr += "#";
			p.number++;
			p.pok[p.number] = Pokemon(pok2);
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
		else {//失败，舍弃一个精灵，再次交互

			verStr = "~";
			verStr += "#";
			verStr += "可惜，您在决斗赛中被击败，您需要舍弃您的一个精灵,系统选择了下列精灵:";
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
				verStr += to_string(i) + ":    ||   " + "精灵名称: " + p.pok[id]._name + "    精灵种族: " + p.pok[id].getRaceName() + "    精灵等级: " + to_string(p.pok[id]._level);
				verStr += "#";

				verStr += "\t";
				verStr += "      ||   生命值: " + to_string(p.pok[id]._hp) + "    攻击力: " + to_string(p.pok[id]._atk) + "    防御力: " + to_string(p.pok[id]._def) + "    速度: " + to_string(p.pok[id]._sp);
				verStr += "##";
			}

			verStr += "请选择您要舍弃的精灵:"; verStr += "#";
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

			Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
			int pos = string_to_int(recvBuf);

			cout << "here" << recvBuf << endl;

			verStr = "";

			verStr += "您舍弃了精灵" + p.pok[seq[pos]]._name;
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
	else {//手动施法战斗
		verStr = "";
		cout << "===精灵" << pok1._name << "=== 对战 ===精灵" << pok2._name << "===" << endl;
		verStr += "===精灵" + pok1._name + "=== 对战 ===精灵" + pok2._name + "===";
		verStr += "#";

		//比赛前复原
		pok1.recovery();
		pok2.recovery();

		tim1 = tim2 = 0;
		//战斗跟速度有关，速度快表示攻击间隔低
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
		verStr += "请输入1确认结束战斗"; verStr += "#";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);

		verStr = "结束";
		sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		verStr = "";

		if (pok1._nowHp > 0) {//获胜，获得一个精灵

			verStr = "?";

			verStr += "#";
			verStr += "恭喜，您在决斗赛中取得了胜利，获得了精灵" + pok2._name + "!";
			verStr += "#";
			p.number++;
			p.pok[p.number] = Pokemon(pok2);
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);
		}
		else {//失败，选一个精灵送出去

			verStr = "~";
			verStr += "#";
			verStr += "可惜，您在决斗赛中被击败，您需要舍弃您的一个精灵,系统选择了下列精灵:";
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
				verStr += to_string(i) + ":    ||   " + "精灵名称: " + p.pok[id]._name + "    精灵种族: " + p.pok[id].getRaceName() + "    精灵等级: " + to_string(p.pok[id]._level);
				verStr += "#";

				verStr += "\t";
				verStr += "      ||   生命值: " + to_string(p.pok[id]._hp) + "    攻击力: " + to_string(p.pok[id]._atk) + "    防御力: " + to_string(p.pok[id]._def) + "    速度: " + to_string(p.pok[id]._sp);
				verStr += "##";
			}

			verStr += "请选择您要舍弃的精灵:"; verStr += "#";
			sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

			Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
			int pos = string_to_int(recvBuf);


			verStr = "";

			verStr += "您舍弃了精灵" + p.pok[seq[pos]]._name;
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

	playerOutputToFile();//用户更新，将用户信息输出到文件
}

void Login(SOCKET & recvClientSocket) {//用户登录
	cout << endl << "用户登录" << endl;
	  
	int AccExist = 0, KeyRight = 0;//分别为账号是否存在，密码是否正确
	int reLen;
	//从客户端读取账号id
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
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
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);
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
	playerOutput(recvClientSocket, player[AccExist]);//输出当前用户信息

	player[AccExist].loginStatus = true;//设置登陆状态为true

	while (true) {//该用户执行操作
		 
		Mem(recvBuf); int reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
		//阻塞函数，等待接收数据
		cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;

		string verStr = "";

		if (strcmp(recvBuf, "1") == 0) {//升级赛
			UpgradeCompetition(recvClientSocket,player[AccExist]);
		}
		else if (strcmp(recvBuf, "2") == 0) {//决斗赛
			DuelCompetition(recvClientSocket,player[AccExist]);
		}
		else if (strcmp(recvBuf, "3") == 0) {//查询个人信息
			playerOutput(recvClientSocket, player[AccExist]);
		}
		else if (strcmp(recvBuf, "4") == 0) {//查询在线用户
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) if (player[i].loginStatus) {
				verStr += "第" + to_string(++tot) + "个用户:    ||   " + "用户名: " + player[i].name + "  用户账号: " + player[i].id;
				verStr += "#";
			}
			verStr += "共" + to_string(tot) + "个在线用户";
			verStr += "##";
			
			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "5") == 0) {//查询所有用户的精灵

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
		else if (strcmp(recvBuf, "6") == 0) {//查询用户胜率
			int tot = 0;
			for (int i = 1; i <= playerNum; i++) {
				verStr += "用户" + player[i].name + ":    ||   ";

				if (player[i].totNum == 0) verStr += "战斗场数: 0   胜利场数: 0   胜率: 无#";//没有参加过比赛，无法计算胜率
				else verStr += "战斗场数: " + to_string(player[i].totNum) + "   胜利场数: " + to_string(player[i].winNum) + "   胜率: " + to_string(1000 * player[i].winNum / player[i].totNum / 10) + "." + to_string((1000 * player[i].winNum / player[i].totNum) % 10) + "%#";

				verStr += "#";
			}
			verStr += "##";

			Print(verStr);
			int sLen = send(recvClientSocket, (char*)verStr.c_str(), verStr.length(), 0);

		}
		else if (strcmp(recvBuf, "7") == 0) {//修改密码
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
		else if (strcmp(recvBuf, "8") == 0) {//用户登出，退出
			player[AccExist].loginStatus = false;
			cout << "退出登录" << endl;
			break;
		}
		else if (strcmp(recvBuf, "0") == 0) {//不登出，只退出
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
	  
	string NAME, ID, KEY;
	int reLen;

	++playerNum;

	//从客户端读取用户名
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "用户名: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	NAME = recvBuf;

	//从客户端读取用户账号
	Mem(recvBuf); reLen  = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "用户账号: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	ID = recvBuf;
	
	//从客户端读取用户密码
	Mem(recvBuf); reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
	cout << "用户密码: " << recvBuf << endl;
	cout << "接收到消息: " << recvBuf << "               来自客户端:" << recvClientSocket << endl;
	KEY = recvBuf;

	//从客户端再次读取用户密码
	Mem(recvBuf); reLen =  recv(recvClientSocket, recvBuf, 65536, 0);;
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

	playerOutputToFile();//将用户信息输出到文件

}

//服务线程
DWORD WINAPI ServerThread(LPVOID lpParameter) {
	SOCKET *recvClientSocket = (SOCKET*)lpParameter;
	//循环接收+判断
	while (true) {
		  
		Mem(recvBuf); int reLen = recv(*recvClientSocket, recvBuf, 65536, 0);

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

	//for (int i = 1; i <= 5; i++) cout << rand() << endl;

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
		  
		int reLen = recv(recvClientSocket, recvBuf, 65536, 0);;
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
