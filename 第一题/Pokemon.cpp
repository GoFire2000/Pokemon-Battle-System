#include <ctime>
#include <string>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "Pokemon.h"
using namespace std;

int getRand(int x, int y) {

	if (x > y) swap(x, y);
	return x + rand() % (y - x + 1);
}

//==================PokemonBase相关===================
PokemonBase::PokemonBase(int Type) {//构造函数
	//基础信息设定
	pokemonType = Type;
	level = 1;
	baseAtk = 20;
	baseDef = 15;
	baseHp = 100;
	baseSp = 10;

	//按照种类提高特定属性
	if (Type == PowerType) {//力量型
		baseAtk += 5;
	}
	else if (Type == MeatShieldType) {//肉盾型
		baseHp += 20;
	}
	else if (Type == DefensiveType) {//防御型
		baseDef += 5;
	}
	else if (Type == AgileType) {//敏捷型
		baseSp += 5;
	}

	//经验值设定
	int exp = 0;
	for (int i = 1; i <= 15; i++) {
		expValue[i] = exp;
		exp += i * 5;
	}

}

//闪避判断，跟两者速度有关，带有一定随机性，如果返回false表示未闪避，返回true表示闪避成功
bool PokemonBase::dodge(int atkSp, int tarSp) {
	int sub = atkSp - tarSp;
	if (sub > 30) return false;
	else if (sub > 20) return rand() % 100 < 3;
	else if (sub > 10)return rand() % 100 < 5;
	else if (sub > 0) return rand() % 100 < 10;
	else if (sub > -10) return rand() % 100 < 10;
	else if (sub > -20) return rand() % 100 < 15;
	else if (sub > -30) return rand() % 100 < 25;
	else if (sub > -50) return rand() % 100 < 35;
	else return rand() % 100 < 50;
}

//==================四个精灵种类相关===================
Power::Power() :PokemonBase(PowerType) {//以小火龙为代表
	raceName = "力量型";

	skillName[0] = "撞击";//普通攻击
	skillName[1] = "火焰旋涡";//伤害低级
	skillName[2] = "愤怒";//强化攻击力
	skillName[3] = "喷射火焰";//伤害中级
	skillName[4] = "闪焰冲锋";//伤害高级

	pp[0] = Inf;//普通攻击
	pp[1] = 8;//特殊攻击技能
	pp[2] = 2;//强化增益技能
	pp[3] = 6;//特殊攻击技能
	pp[4] = 4;//必杀技
}
//力量型精灵攻击
bool Power::attack(Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "精灵" << attacker._name << "对精灵" << target._name << "使用技能" << attacker.getSkillName(skillId) << ",";
	if (skillId == 0) {//撞击
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.1*attacker._nowAtk - target._nowDef+getRand(0,attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//火焰旋涡
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//愤怒
		attacker.changeAtk(attacker._nowAtk/10);
		cout << "提升了自身" << attacker._nowAtk / 10 << "点攻击力。\n" << endl;
		return false;
	}
	else if (skillId == 3) {//喷射火焰
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.3*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//闪焰冲锋
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.6*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	return false;
}

MeatShield::MeatShield() : PokemonBase(MeatShieldType) {//以妙蛙种子为代表
	raceName = "肉盾型";

	skillName[0] = "撞击";//普通攻击
	skillName[1] = "藤编";//低伤
	skillName[2] = "寄生种子";//生命力回复
	skillName[3] = "飞叶快刀";//中伤
	skillName[4] = "阳光烈焰";//高伤

	pp[0] = Inf;//普通攻击
	pp[1] = 6;//特殊攻击技能
	pp[2] = 6;//强化增益技能
	pp[3] = 6;//特殊攻击技能
	pp[4] = 2;//必杀技
}
//肉盾型精灵攻击
bool MeatShield::attack(Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "精灵" << attacker._name << "对精灵" << target._name << "使用技能" << attacker.getSkillName(skillId) << ",";
	if (skillId == 0) {//撞击
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//藤编
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//寄生种子
		attacker.changeAtk(attacker._nowAtk / 6);
		cout << "提升了自身" << attacker._nowAtk / 6 << "点生命值。\n" << endl;
		return false;
	}
	else if (skillId == 3) {//飞叶快刀
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//阳光烈焰
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	return false;
}

Defensive::Defensive() : PokemonBase(DefensiveType) {//以杰尼龟为代表
	raceName = "防御型";

	skillName[0] = "撞击";//普通攻击
	skillName[1] = "水枪";//低伤
	skillName[2] = "缩壳";//防御力提高
	skillName[3] = "泡沫光线";//中伤
	skillName[4] = "鱼跃龙门";//高伤

	pp[0] = Inf;//普通攻击
	pp[1] = 8;//特殊攻击技能
	pp[2] = 4;//强化增益技能
	pp[3] = 6;//特殊攻击技能
	pp[4] = 2;//必杀技
}
bool Defensive::attack(Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "精灵" << attacker._name << "对精灵" << target._name << "使用技能" << attacker.getSkillName(skillId) << ",";
	if (skillId == 0) {//撞击
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//水枪
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);

		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//缩壳
		attacker.changeAtk(attacker._nowAtk / 10);
		cout << "提升了自身" << attacker._nowAtk / 10 << "点防御力。\n" << endl;
		return false;
	}
	else if (skillId == 3) {//泡沫光线
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//鱼跃龙门
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	return false;
}

Agile::Agile() : PokemonBase(AgileType) {//以皮卡丘为代表
	raceName = "敏捷型";

	skillName[0] = "撞击";//普通攻击
	skillName[1] = "电击";//低伤
	skillName[2] = "高速移动";//速度提高
	skillName[3] = "十万伏特";//中伤
	skillName[4] = "电磁炮";//高伤

	pp[0] = Inf;//普通攻击
	pp[1] = 7;//特殊攻击技能
	pp[2] = 4;//强化增益技能
	pp[3] = 6;//特殊攻击技能
	pp[4] = 3;//必杀技
}
bool Agile::attack(Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "精灵" << attacker._name << "对精灵" << target._name << "使用技能" << attacker.getSkillName(skillId) << ",";
	if (skillId == 0) {//撞击
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//电击
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//高速移动
		attacker.changeAtk(attacker._nowAtk / 10);
		cout << "提升了自身" << attacker._nowAtk / 10 << "点速度。\n" << endl;
		return false;
	}
	else if (skillId == 3) {//十万伏特
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//电磁炮
		if (dodge(attacker._nowSp, target._nowSp)) { cout << "被闪避。\n" << endl; return false; }

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "触发了暴击,";
		}
		cout << "造成" << val << "点伤害。\n" << endl;
		return target.beAttacked(val);
	}
	return false;
}

PokemonBase *Pokemon::_race[4] = { new Power(),new MeatShield(),new Defensive(),new Agile() };


//==================Pokemon相关===================

//根据种族和名字构造
Pokemon::Pokemon(int RaceId, string Name) {
	_raceId = RaceId;
	_name = Name;
	_level = 1;
	_exp = 0;
	
	//在种族的基础上加一些随机化的内容
	_atk = _race[_raceId]->baseAtk + getRand(-3, 3);
	_def = _race[_raceId]->baseDef + getRand(-4, 4);
	_hp = _race[_raceId]->baseHp + getRand(-10, 10);
	_sp = _race[_raceId]->baseSp + getRand(-4, 4);

	for (int i = 0; i < 5; i++) _nowPp[i] = _race[_raceId]->pp[i];
}

//根据信息构造
Pokemon::Pokemon(int RaceId, string Name, int Level, int Exp, int Atk, int Def, int Hp, int Sp) {
	_raceId = RaceId;
	_name = Name;
	_level = Level;
	_exp = Exp;
	_atk = Atk;
	_def = Def;
	_hp = Hp;
	_sp = Sp;
}

//将战斗所需的变量初始化
void Pokemon::recovery() {
	_nowAtk = _atk;
	_nowDef = _def;
	_nowHp = _hp;
	_nowSp = _sp;
	for (int i = 0; i < 5; i++) _nowPp[i] = _race[_raceId]->pp[i];
}

//攻击敌方精灵,如果敌方精灵死亡，返回true，否则返回false
bool Pokemon::attack(Pokemon & target, bool autoFight) {
	if (autoFight) {//自动攻击，优先选择技能，没有技能使用普通攻击
		bool flag[5];
		int tot = 0;
		for (int i = 1; i <= 4; i++) {
			if (_level >= (i - 1) * 5 && _nowPp[i]) {
				flag[i] = true;
				tot += _nowPp[i];
			}
			else flag[i] = false;
		}
		if (tot==0){//没有技能，使用普通攻击
			return _race[_raceId]->attack(*this, target, 0);
		}
		//有技能，优先使用技能
		int num = rand() % tot + 1,skillId=0;
		for (int i = 1; i <= 4; i++) {
			if (flag[i]) {
				if (num <= _nowPp[i]) {
					skillId = i;
					_nowPp[i]--;
					break;
				}
				else {
					num -= _nowPp[i];
				}
			}
		}
		return _race[_raceId]->attack(*this, target, skillId);
	}
	else {
		for (int i = 0; i <= 4; i++) {
			if (_level >= (i - 1) * 5 && _nowPp[i]) {
				cout << "技能" << i << ": " << _race[_raceId]->skillName[i] << ",剩余使用次数";
				if (i == 0) cout << "无限" << endl;
				else cout << _nowPp[i] << endl;
			}
		}
		cout << "请输入使用的技能编号:";
		int skillId; cin >> skillId;
		if (_level >= (skillId - 1) * 5 && _nowPp[skillId]) {
			if (skillId >= 1) _nowPp[skillId]--;
			return _race[_raceId]->attack(*this, target, skillId);
		}
		else {
			cout << "输入无效，请重新操作" << endl;
		}
	}
	
}

//承受伤害，如果自己死亡，返回true，否则返回false
bool Pokemon::beAttacked(int val) {
	val = max(val, 1);
	_nowHp -= val;
	if (_nowHp <= 0) return true;
	return false;   
}


//战斗中修改攻击力
void Pokemon::changeAtk(int val) {
	_nowAtk += val;
	_nowAtk = max(_nowAtk, 0);
}

//战斗中修改防御力
void Pokemon::changeDef(int val) {
	_nowDef += val;
	_nowDef = max(_nowDef, 0);
}

//战斗中修改生命值
void Pokemon::changeHp(int val) {
	_nowDef += val;
	_nowDef = min(_nowDef, _hp);
	_nowDef = max(_nowDef, 0);
}

//获得经验值，判断升级,true为升级，false未升级
bool Pokemon::gainExp(int val) {
	val = max(val, 1);
	cout << "精灵" << _name << "获得" << val << "经验值!" << endl;
	
	if (_level == 15) {
		cout << "精灵已满级，不能再升级!" << endl;
		return false;
	}

	_exp += val;
	bool tag = false;
	while (_level < 15 && _exp >= _race[_raceId]->expValue[_level + 1]) {
		tag = true;
		_level++;

		_atk += 5 + getRand(0, 4);
		_def += 4 + getRand(0, 6);
		_hp += 20 + getRand(0, 8);
		_sp += 4 + getRand(0, 4);

		if (_raceId == PowerType) _atk += 5;
		else if (_raceId == MeatShieldType) _hp += 20;
		else if (_raceId == DefensiveType) _def += 5;
		else if (_raceId == AgileType) _sp += 3;

	}
	if (tag) {
		cout << "恭喜，精灵" << _name << "升级成功，当前等级为" << _level << "!" << endl << endl;
		return true;
	}
	cout << endl;
	return false;
}

//战斗中修改速度
void Pokemon::changeSp(int val) {
	_nowSp += val;
	_nowDef = max(_nowDef, 0);
}

//信息输出
void Pokemon::print() {
	cout << "==================精灵档案==================" << endl;
	cout << "所属种族编号:" << _raceId << endl;
	cout << "精灵名称:" << _name << endl;
	cout << "等级:" << _level << endl;
	cout << "经验值:" << _exp << endl;
	cout << "攻击力:" << _atk << endl;
	cout << "防御力:" << _def << endl;
	cout << "生命值:" << _hp << endl;
	cout << "攻击速度:" << _sp << endl<<endl;
	cout << "种族名称:" << getRaceName() << endl;
	cout << "种族编号:" << _race[_raceId]->pokemonType << endl;
	cout << "种族攻击力:" << _race[_raceId]->baseAtk << endl;
	cout << "种族防御力:" << _race[_raceId]->baseDef << endl;
	cout << "种族生命值" << _race[_raceId]->baseHp << endl;
	cout << "种族攻击速度:" << _race[_raceId]->baseSp << endl;
	cout << "升级经验::" << "   ";
	for (int i = 1; i <= 15; i++) cout << getExpValue(i) << " "; cout << endl;
	cout << "技能名称::" << "   ";
	for (int i = 0; i < 5; i++) cout << getSkillName(i) << " "; cout << endl;
	cout << "技能可用次数::" << "   ";
	for (int i = 0; i < 5; i++) cout << getPp(i) << " "; cout << endl;
	cout << "================================================" << endl << endl;
}
