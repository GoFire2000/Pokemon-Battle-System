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
	string name;//�û��� 
	string id;//��½ʱ�˻� 
	string key;//���� 
	int number;//��������
	int winNum, totNum;//����ʤ�����������ܳ��������ڼ���ʤ��

	Pokemon pok[100];//��ӵ�еľ���

	string numberMedal();//����������£�����ͭ��,����ӵ�еĳ�������Ķ��ٰ䷢
	string superPokemonMedal();//�߼�����ѫ�£�����ͭ��������ӵ�еĸ߼����15���������䷢
	
	//����true��ʾ�����޸ĳɹ� 
	void changeKey(string newKey);
};
