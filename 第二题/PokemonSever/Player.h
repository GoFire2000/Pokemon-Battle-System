#pragma once

#include "Pokemon.h"
using namespace std;

#define PLAYER_FILE "player.txt"
#define PLAYERSTORE_FILE "playerInformationStore.txt"

void getPokemon(Pokemon & pok, int id);

class Player{//��½��ע�ᡢ�ǳ����޸����� 
public:
	Player();
	Player(string Name, string Id, string Key);
	~Player() { }; 
	bool loginStatus;//1Ϊ���ߣ�0Ϊ������
	int winNum, totNum;
	string name;//�û��� 
	string id;//��½ʱ�˻� 
	string key;//���� 
	int number;//��������
	Pokemon pok[100];//��ӵ�еľ���
	
	//����true��ʾ�����޸ĳɹ� 
	void changeKey(string newKey);
};
