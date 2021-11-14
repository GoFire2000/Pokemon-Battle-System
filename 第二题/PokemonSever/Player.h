#pragma once

#include "Pokemon.h"
using namespace std;

#define PLAYER_FILE "player.txt"
#define PLAYERSTORE_FILE "playerInformationStore.txt"

void getPokemon(Pokemon & pok, int id);

class Player{//登陆、注册、登出、修改密码 
public:
	Player();
	Player(string Name, string Id, string Key);
	~Player() { }; 
	bool loginStatus;//1为在线，0为不在线
	int winNum, totNum;
	string name;//用户名 
	string id;//登陆时账户 
	string key;//密码 
	int number;//精灵数量
	Pokemon pok[100];//所拥有的精灵
	
	//返回true表示密码修改成功 
	void changeKey(string newKey);
};
