#include "player.h"

#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;


void getPokemon(Pokemon & pok, int id){
	static Pokemon POKEMON[16] = {
						Pokemon(PowerType, "Ğ¡»ğÁú"), Pokemon(PowerType, "ÁùÎ²"), Pokemon(PowerType, "ÁÒ»ğÂí"), Pokemon(PowerType, "»ğÑæÄñ"),
						Pokemon(MeatShieldType, "ÃîÍÜÖÖ×Ó"), Pokemon(MeatShieldType, "ÌÙÂû¹Ö"), Pokemon(MeatShieldType, "°ÔÍõ»¨"), Pokemon(MeatShieldType, "²İÃç¹ê"),
						Pokemon(DefensiveType, "½ÜÄá¹ê"), Pokemon(DefensiveType, "ºğ¾¨Íõ"), Pokemon(DefensiveType, "ÎÃÏãÓ¾Ê¿"), Pokemon(DefensiveType, "¶¾´ÌË®Ä¸"),
						Pokemon(AgileType, "Æ¤¿¨Çğ"), Pokemon(AgileType, "ÉÁµçÄñ"), Pokemon(AgileType, "µçÁú"), Pokemon(AgileType, "À×ÒÁ²¼")
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
