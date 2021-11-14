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
	string name;//用户名 
	string id;//登陆时账户 
	string key;//密码 
	int number;//精灵数量
	int winNum, totNum;//比赛胜场数，比赛总场数，用于计算胜率

	Pokemon pok[100];//所拥有的精灵

	string numberMedal();//宠物个数徽章（金银铜）,根据拥有的宠物个数的多少颁发
	string superPokemonMedal();//高级宠物勋章（金银铜），根据拥有的高级宠物（15级）个数颁发
	
	//返回true表示密码修改成功 
	void changeKey(string newKey);
};
