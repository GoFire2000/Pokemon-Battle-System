#pragma once

#include <ctime>
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

//四种类型，力量型、肉盾型、防御型、敏捷型 
#define PowerType 0
#define MeatShieldType 1
#define DefensiveType 2
#define AgileType 3
#define Inf 2000000000
class Pokemon;

int getRand(int x, int y);

class PokemonBase {
public:
	PokemonBase(int Type);//构造函数
	~PokemonBase() {};//析构函数
	
	string raceName;//种族名称
	int pokemonType;//精灵种类
	int level;//等级 
	int expValue[16];//升到该级所需经验值
	int baseAtk;//基础攻击力 
	int baseDef;//基础防御力 
	int baseHp;//基础生命值 
	int baseSp;//基础攻击速度 
	string skillName[5];//技能名称，普通攻击和四个特殊技能
	//技能设定：普通攻击(1级)，特殊攻击技能1(1级)，增益技能(5级)，特殊攻击技能2(10级)，必杀技(15级)
	int pp[5];//技能剩余使用次数

	//攻击函数,true为击杀，false为未击杀
	virtual bool attack(Pokemon &attacker, Pokemon &target, int skillId) = 0;

	//闪避函数,true为闪避，false为未闪避
	bool dodge(int atkSp, int tarSp);

};

class Power : public PokemonBase {
public:
	Power();//构造函数
	~Power() {};//析构函数
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class MeatShield : public PokemonBase {
public:
	MeatShield();//构造函数
	~MeatShield() {};//析构函数
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Defensive : public PokemonBase {
public:
	Defensive();//构造函数
	~Defensive() {};//析构函数
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Agile : public PokemonBase {
public:
	Agile();//构造函数
	~Agile() {};//析构函数
	bool attack(Pokemon &attacker, Pokemon & target, int skillId);
};

class Pokemon {
public:
	Pokemon(int RaceId, string Name);//根据种族构造
	Pokemon(int RaceId, string Name, int Level, int Exp, int Atk, int Def, int Hp, int Sp);//直接按照数据构造
	~Pokemon() {};//析构函数

	int _raceId;//种类的编号
	string _name;//精灵名称
	int _level;//等级
	int _exp;//经验值
	int _atk, _nowAtk;//攻击力
	int _def, _nowDef;//防御力
	int _hp, _nowHp;//血量
	int _sp, _nowSp;//攻击速度
	int _nowPp[5];//技能剩余使用次数
	static PokemonBase *_race[4];//四个精灵种类

	//获取种族信息
	string getRaceName() {//种类名称
		return _race[_raceId]->raceName;
	}
	int getExpValue(int id) {
		return _race[_raceId]->expValue[id];
	}
	string getSkillName(int id) {//技能名称
		return _race[_raceId]->skillName[id];
	}
	int getPp(int id) {
		return _race[_raceId]->pp[id];
	}

	//战斗信息
	void recovery();//复原精灵信息
	bool attack(Pokemon & target, bool autoFight = true);//造成伤害，true为目标死亡，false未死亡
	bool beAttacked(int val);//被伤害，true自己死亡，false未死亡

	//暴击，true为暴击，false为未暴击
	bool crit() {//力量型暴击率高于其他种族,暴击率和等级有关
		if (_raceId == PowerType) {
			if (rand() % (23-_level) == 0) return true;
			return false;
		}
		if (rand() % (25-_level) == 0) return true;
		return false;
	}

	//升级
	bool gainExp(int val);//升级函数，true升级，false未升级

	//改变精灵当前信息
	void changeAtk(int val);
	void changeDef(int val);
	void changeHp(int val);
	void changeSp(int val);

	//信息输出
	void print();

};

