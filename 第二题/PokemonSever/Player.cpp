#include "player.h"

#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;


void getPokemon(Pokemon & pok, int id){
	static Pokemon POKEMON[16] = {
						Pokemon(PowerType, "С����"), Pokemon(PowerType, "��β"), Pokemon(PowerType, "�һ���"), Pokemon(PowerType, "������"),
						Pokemon(MeatShieldType, "��������"), Pokemon(MeatShieldType, "������"), Pokemon(MeatShieldType, "������"), Pokemon(MeatShieldType, "�����"),
						Pokemon(DefensiveType, "�����"), Pokemon(DefensiveType, "����"), Pokemon(DefensiveType, "����Ӿʿ"), Pokemon(DefensiveType, "����ˮĸ"),
						Pokemon(AgileType, "Ƥ����"), Pokemon(AgileType, "������"), Pokemon(AgileType, "����"), Pokemon(AgileType, "������")
					  };
	pok = Pokemon(POKEMON[id]);

	//return POKEMON[id];
}

Player::Player() {

}

Player::Player(string Name, string Id, string Key){
	name = Name;
	id = Id;
	key = Key;
	number = 3;
	winNum = totNum = 0;
	loginStatus = false;
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
