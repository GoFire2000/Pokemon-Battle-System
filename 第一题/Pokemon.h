#pragma once

#include <ctime>
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

//�������ͣ������͡�����͡������͡������� 
#define PowerType 0
#define MeatShieldType 1
#define DefensiveType 2
#define AgileType 3
#define Inf 2000000000
class Pokemon;

int getRand(int x, int y);

class PokemonBase {
public:
	PokemonBase(int Type);//���캯��
	~PokemonBase() {};//��������
	
	string raceName;//��������
	int pokemonType;//��������
	int level;//�ȼ� 
	int expValue[16];//�����ü����辭��ֵ
	int baseAtk;//���������� 
	int baseDef;//���������� 
	int baseHp;//��������ֵ 
	int baseSp;//���������ٶ� 
	string skillName[5];//�������ƣ���ͨ�������ĸ����⼼��
	//�����趨����ͨ����(1��)�����⹥������1(1��)�����漼��(5��)�����⹥������2(10��)����ɱ��(15��)
	int pp[5];//����ʣ��ʹ�ô���

	//��������,trueΪ��ɱ��falseΪδ��ɱ
	virtual bool attack(Pokemon &attacker, Pokemon &target, int skillId) = 0;

	//���ܺ���,trueΪ���ܣ�falseΪδ����
	bool dodge(int atkSp, int tarSp);

};

class Power : public PokemonBase {
public:
	Power();//���캯��
	~Power() {};//��������
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class MeatShield : public PokemonBase {
public:
	MeatShield();//���캯��
	~MeatShield() {};//��������
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Defensive : public PokemonBase {
public:
	Defensive();//���캯��
	~Defensive() {};//��������
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Agile : public PokemonBase {
public:
	Agile();//���캯��
	~Agile() {};//��������
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Pokemon {
public:
	Pokemon(int RaceId, string Name);//�������幹��
	Pokemon(int RaceId, string Name, int Level, int Exp, int Atk, int Def, int Hp, int Sp);//ֱ�Ӱ������ݹ���
	~Pokemon() {};//��������

	int _raceId;//����ı��
	string _name;//��������
	int _level;//�ȼ�
	int _exp;//����ֵ
	int _atk, _nowAtk;//������
	int _def, _nowDef;//������
	int _hp, _nowHp;//Ѫ��
	int _sp, _nowSp;//�����ٶ�
	int _nowPp[5];//����ʣ��ʹ�ô���
	static PokemonBase *_race[4];//�ĸ���������

	//��ȡ������Ϣ
	string getRaceName() {//��������
		return _race[_raceId]->raceName;
	}
	int getExpValue(int id) {
		return _race[_raceId]->expValue[id];
	}
	string getSkillName(int id) {//��������
		return _race[_raceId]->skillName[id];
	}
	int getPp(int id) {
		return _race[_raceId]->pp[id];
	}

	//ս����Ϣ
	void recovery();//��ԭ������Ϣ
	bool attack(Pokemon & target, bool autoFight = true);//����˺���trueΪĿ��������falseδ����
	bool beAttacked(int val);//���˺���true�Լ�������falseδ����

	//������trueΪ������falseΪδ����
	bool crit() {//�����ͱ����ʸ�����������,�����ʺ͵ȼ��й�
		if (_raceId == PowerType) {
			if (rand() % (23-_level) == 0) return true;
			return false;
		}
		if (rand() % (25-_level) == 0) return true;
		return false;
	}

	//����
	bool gainExp(int val);//����������true������falseδ����

	//�ı侫�鵱ǰ��Ϣ
	void changeAtk(int val);
	void changeDef(int val);
	void changeHp(int val);
	void changeSp(int val);

	//��Ϣ���
	void print();

};

