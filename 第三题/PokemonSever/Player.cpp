#include "player.h"

#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;


void getPokemon(Pokemon & pok, int id){
	Pokemon POKEMON[16] = {
						Pokemon(PowerType, "小火龙"), Pokemon(PowerType, "六尾"), Pokemon(PowerType, "烈火马"), Pokemon(PowerType, "火焰鸟"),
						Pokemon(MeatShieldType, "妙蛙种子"), Pokemon(MeatShieldType, "藤蔓怪"), Pokemon(MeatShieldType, "霸王花"), Pokemon(MeatShieldType, "草苗龟"),
						Pokemon(DefensiveType, "杰尼龟"), Pokemon(DefensiveType, "吼鲸王"), Pokemon(DefensiveType, "蚊香泳士"), Pokemon(DefensiveType, "毒刺水母"),
						Pokemon(AgileType, "皮卡丘"), Pokemon(AgileType, "闪电鸟"), Pokemon(AgileType, "电龙"), Pokemon(AgileType, "雷伊布")
					  };
	pok = Pokemon(POKEMON[id]);

	//return POKEMON[id];
}

string Player::numberMedal() {//宠物个数徽章（金银铜）,根据拥有的宠物个数的多少颁发
	if (number >= 30) return "金牌勋章";
	else if (number >= 20) return "银牌勋章";
	else if (number >= 10) return "铜牌勋章";
	else return "无勋章";
}

string Player::superPokemonMedal() {//高级宠物勋章（金银铜），根据拥有的高级宠物（15级）个数颁发
	int Count = 0;
	for (int i = 1; i <= number; i++) {
		if (pok[i]._level == 15) Count++;
	}
	if (Count >= 15) return "金牌勋章";
	else if (Count >= 10) return "银牌勋章";
	else if (Count >= 5) return "铜牌勋章";
	return "无勋章";
}

Player::Player() {//空构造

}

Player::Player(string Name, string Id, string Key){//构造函数
	name = Name;
	id = Id;
	key = Key;
	number = 3;
	winNum = totNum = 0;
	loginStatus = false;
	//随机分配三个不同的精灵
	bool flag[16] = {false};
	memset(flag, 0, sizeof(flag));
	for(int i = 1; i <= 3; i++) {
		int id = rand()%16;
		while (flag[id]) id = rand() % 16;
		if(!flag[id]) {
			flag[id] = true;
			getPokemon(pok[i],id);
			cout << i << " " << pok[i]._name << endl;
		}
	} 
}
void Player::changeKey(string newKey){
	key = newKey;
}
