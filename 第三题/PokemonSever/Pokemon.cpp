#include <ctime>
#include <string>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "Pokemon.h"
using namespace std;


//==================PokemonBase���===================
PokemonBase::PokemonBase(int Type) {//���캯��
	//������Ϣ�趨
	pokemonType = Type;
	level = 1;
	baseAtk = 15;
	baseDef = 10;
	baseHp = 50;
	baseSp = 10;

	//������������ض�����
	if (Type == PowerType) {//������
		baseAtk += 5;
	}
	else if (Type == MeatShieldType) {//�����
		baseHp += 10;
	}
	else if (Type == DefensiveType) {//������
		baseDef += 5;
	}
	else if (Type == AgileType) {//������
		baseSp += 5;
	}

	//����ֵ�趨
	int exp = 0;
	for (int i = 1; i <= 15; i++) {
		expValue[i] = exp;
		exp += i * 5;
	}

}

//�����жϣ��������ٶ��йأ�����һ������ԣ��������false��ʾδ���ܣ�����true��ʾ���ܳɹ�
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


int getRand(int x, int y) {//��x��y֮��������

	if (x > y) swap(x, y);
	return x + rand() % (y - x + 1);
}

int getExperience(int Level) {//�õ�������Level�ľ���ֵ
	int exp = 0;
	for (int i = 1; i <= Level; i++) {
		exp += i * 5;
	}
	return exp;
}

//==================�ĸ������������===================
Power::Power() :PokemonBase(PowerType) {//��С����Ϊ����
	raceName = "������";

	skillName[0] = "ײ��";//��ͨ����
	skillName[1] = "��������";//�˺��ͼ�
	skillName[2] = "��ŭ";//ǿ��������
	skillName[3] = "�������";//�˺��м�
	skillName[4] = "������";//�˺��߼�

	pp[0] = Inf;//��ͨ����
	pp[1] = 8;//���⹥������
	pp[2] = 2;//ǿ�����漼��
	pp[3] = 6;//���⹥������
	pp[4] = 4;//��ɱ��
}
//�����;��鹥��
bool Power::attack(string& verStr, Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "����" << attacker._name << "�Ծ���" << target._name << "ʹ�ü���" << attacker.getSkillName(skillId) << ",";
	verStr += "����" + attacker._name + "�Ծ���" + target._name + "ʹ�ü���" + attacker.getSkillName(skillId) + ",";
	if (skillId == 0) {//ײ��
		if (dodge(attacker._nowSp, target._nowSp)) { 
			cout << "�����ܡ�\n" << endl; 
			verStr += "�����ܡ�\n"; verStr += "#";
			return false; 
		}

		int val = (double)1.1*attacker._nowAtk - target._nowDef+getRand(0,attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//��������
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//��ŭ
		attacker.changeAtk(attacker._nowAtk/10);
		cout << "����������" << attacker._nowAtk / 10 << "�㹥������\n" << endl;
		verStr += "����������" + to_string(attacker._nowAtk / 10) + "�㹥������\n"; verStr += "#";
		return false;
	}
	else if (skillId == 3) {//�������
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.3*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//������
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.6*attacker._nowAtk - target._nowDef + getRand(0, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	return false;
}

MeatShield::MeatShield() : PokemonBase(MeatShieldType) {//����������Ϊ����
	raceName = "�����";

	skillName[0] = "ײ��";//��ͨ����
	skillName[1] = "�ٱ�";//����
	skillName[2] = "��������";//�������ظ�
	skillName[3] = "��Ҷ�쵶";//����
	skillName[4] = "��������";//����

	pp[0] = Inf;//��ͨ����
	pp[1] = 6;//���⹥������
	pp[2] = 6;//ǿ�����漼��
	pp[3] = 6;//���⹥������
	pp[4] = 2;//��ɱ��
}
//����;��鹥��
bool MeatShield::attack(string& verStr, Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "����" << attacker._name << "�Ծ���" << target._name << "ʹ�ü���" << attacker.getSkillName(skillId) << ",";
	verStr += "����" + attacker._name + "�Ծ���" + target._name + "ʹ�ü���" + attacker.getSkillName(skillId) + ",";
	if (skillId == 0) {//ײ��
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//�ٱ�
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//��������
		attacker.changeHp(attacker._nowHp / 6);
		cout << "����������" << attacker._nowHp / 6 << "������ֵ��\n" << endl;
		verStr += "����������" + to_string(attacker._nowHp / 6) + "������ֵ��\n"; verStr += "#";
		return false;
	}
	else if (skillId == 3) {//��Ҷ�쵶
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//��������
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	return false;
}

Defensive::Defensive() : PokemonBase(DefensiveType) {//�Խ����Ϊ����
	raceName = "������";

	skillName[0] = "ײ��";//��ͨ����
	skillName[1] = "ˮǹ";//����
	skillName[2] = "����";//���������
	skillName[3] = "��ĭ����";//����
	skillName[4] = "��Ծ����";//����

	pp[0] = Inf;//��ͨ����
	pp[1] = 8;//���⹥������
	pp[2] = 4;//ǿ�����漼��
	pp[3] = 6;//���⹥������
	pp[4] = 2;//��ɱ��
}
bool Defensive::attack(string& verStr, Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "����" << attacker._name << "�Ծ���" << target._name << "ʹ�ü���" << attacker.getSkillName(skillId) << ",";
	verStr += "����" + attacker._name + "�Ծ���" + target._name + "ʹ�ü���" + attacker.getSkillName(skillId) + ",";
	if (skillId == 0) {//ײ��
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//ˮǹ
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);

		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//����
		attacker.changeDef(attacker._nowDef / 10);
		cout << "����������" << attacker._nowDef / 10 << "���������\n" << endl;
		verStr += "����������" + to_string(attacker._nowDef / 10) + "���������\n"; verStr += "#";
		return false;
	}
	else if (skillId == 3) {//��ĭ����
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//��Ծ����
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	return false;
}

Agile::Agile() : PokemonBase(AgileType) {//��Ƥ����Ϊ����
	raceName = "������";

	skillName[0] = "ײ��";//��ͨ����
	skillName[1] = "���";//����
	skillName[2] = "�����ƶ�";//�ٶ����
	skillName[3] = "ʮ�����";//����
	skillName[4] = "�����";//����

	pp[0] = Inf;//��ͨ����
	pp[1] = 7;//���⹥������
	pp[2] = 4;//ǿ�����漼��
	pp[3] = 6;//���⹥������
	pp[4] = 3;//��ɱ��
}
bool Agile::attack(string& verStr, Pokemon & attacker, Pokemon & target, int skillId) {
	cout << "����" << attacker._name << "�Ծ���" << target._name << "ʹ�ü���" << attacker.getSkillName(skillId) << ",";
	verStr += "����" + attacker._name + "�Ծ���" + target._name + "ʹ�ü���" + attacker.getSkillName(skillId) + ",";
	if (skillId == 0) {//ײ��
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 1) {//���
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.1*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 2) {//�����ƶ�
		attacker.changeSp(attacker._nowSp / 10);
		cout << "����������" << attacker._nowSp / 10 << "���ٶȡ�\n" << endl; 
		verStr += "����������" + to_string(attacker._nowSp / 10) + "���ٶȡ�\n"; verStr += "#";

		return false;
	}
	else if (skillId == 3) {//ʮ�����
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.2*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	else if (skillId == 4) {//�����
		if (dodge(attacker._nowSp, target._nowSp)) {
			cout << "�����ܡ�\n" << endl;
			verStr += "�����ܡ�\n"; verStr += "#";
			return false;
		}

		int val = (double)1.4*attacker._nowAtk - target._nowDef + getRand(-5, attacker._level);
		val = max(val, 1);
		if (attacker.crit()) {
			val *= 2;
			cout << "�����˱���,";
			verStr += "�����˱���,";
		}
		cout << "���" << val << "���˺���\n" << endl;
		verStr += "���" + to_string(val) + "���˺���\n"; verStr += "#";
		return target.beAttacked(val);
	}
	return false;
}

PokemonBase *Pokemon::_race[4] = { new Power(),new MeatShield(),new Defensive(),new Agile() };


//==================Pokemon���===================

//�չ���
Pokemon::Pokemon() {
	
} 
//������������ֹ���
Pokemon::Pokemon(int RaceId, string Name) {
	_raceId = RaceId;
	_name = Name;
	_level = 1
	_exp = 0;
	
	//������Ļ����ϼ�һЩ�����������
	_atk = _race[_raceId]->baseAtk + getRand(-3, 3);
	_def = _race[_raceId]->baseDef + getRand(-4, 4);
	_hp = _race[_raceId]->baseHp + getRand(-5, 10);
	_sp = _race[_raceId]->baseSp + getRand(-4, 4);

	for (int i = 0; i < 5; i++) _nowPp[i] = _race[_raceId]->pp[i];
}

//������Ϣ����
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

Pokemon::Pokemon(const Pokemon & A) {
	_raceId = A._raceId;
	_name = A._name;
	_level = A._level;
	_exp = A._exp;
	_atk = A._atk;
	_def = A._def;
	_hp = A._hp;
	_sp = A._sp;
}

//��ս������ı�����ʼ��
void Pokemon::recovery() {
	_nowAtk = _atk;
	_nowDef = _def;
	_nowHp = _hp;
	_nowSp = _sp;
	for (int i = 0; i < 5; i++) _nowPp[i] = _race[_raceId]->pp[i];
}

//�����з�����,����з���������������true�����򷵻�false
bool Pokemon::attack(string& verStr, Pokemon & target, bool autoFight, int oriSkillId) {
	if (autoFight) {//�Զ�����������ѡ���ܣ�û�м���ʹ����ͨ����
		bool flag[5];
		int tot = 0;
		for (int i = 1; i <= 4; i++) {
			if (_level >= (i - 1) * 5 && _nowPp[i]) {
				flag[i] = true;
				tot += _nowPp[i];
			}
			else flag[i] = false;
		}
		if (tot==0){//û�м��ܣ�ʹ����ͨ����
			return _race[_raceId]->attack(verStr, *this, target, 0);
		}
		//�м��ܣ�����ʹ�ü���
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
		return _race[_raceId]->attack(verStr, *this, target, skillId);
	}
	else {
		/*for (int i = 0; i <= 4; i++) {
			if (_level >= (i - 1) * 5 && _nowPp[i]) {
				cout << "����" << i << ": " << _race[_raceId]->skillName[i] << ",ʣ��ʹ�ô���";
				if (i == 0) cout << "����" << endl;
				else cout << _nowPp[i] << endl;
			}
		}
		cout << "������ʹ�õļ��ܱ��:";
		int skillId; cin >> skillId;*/
		if (_level >= (oriSkillId - 1) * 5 && _nowPp[oriSkillId]) {
			if (oriSkillId >= 1) _nowPp[oriSkillId]--;
			return _race[_raceId]->attack(verStr, *this, target, oriSkillId);
		}
		else {
			cout << "������Ч�������²���" << endl;
			return false;
		}
	}
	return false;
}

//�����˺�������Լ�����������true�����򷵻�false
bool Pokemon::beAttacked(int val) {
	val = max(val, 1);
	_nowHp -= val;
	_nowHp = max(_nowHp, 0);
	if (_nowHp <= 0) return true;
	return false;   
}


//ս�����޸Ĺ�����
void Pokemon::changeAtk(int val) {
	_nowAtk += val;
	_nowAtk = max(_nowAtk, 0);
}

//ս�����޸ķ�����
void Pokemon::changeDef(int val) {
	_nowDef += val;
	_nowDef = max(_nowDef, 0);
}

//ս�����޸�����ֵ
void Pokemon::changeHp(int val) {
	_nowHp += val;
	_nowHp = max(_nowHp, 0);
}

//ս�����޸��ٶ�
void Pokemon::changeSp(int val) {
	_nowSp += val;
	_nowSp = max(_nowSp, 0);
}

//��þ���ֵ���ж�����,trueΪ������falseδ����
bool Pokemon::gainExp(string& verStr, int val) {
	val = max(val, 1);
	cout << "����" << _name << "���" << val << "����ֵ!" << endl;
	verStr += "����" + _name + "���" + to_string(val) + "����ֵ!";
	verStr += "#";

	if (_level == 15) {
		cout << "����������������������!" << endl;
		verStr += "����������������������!";
		verStr += "#";
		return false;
	}

	_exp += val;
	bool tag = false;
	while (_level < 15 && _exp >= _race[_raceId]->expValue[_level + 1]) {
		tag = true;
		_level++;

		_atk += 5 + getRand(0, 4);
		_def += 4 + getRand(0, 6);
		_hp += 10 + getRand(0, 8);
		_sp += 4 + getRand(0, 4);

		if (_raceId == PowerType) _atk += 5;
		else if (_raceId == MeatShieldType) _hp += 10;
		else if (_raceId == DefensiveType) _def += 5;
		else if (_raceId == AgileType) _sp += 3;

	}
	if (tag) {
		cout << "��ϲ������" << _name << "�����ɹ�����ǰ�ȼ�Ϊ" << _level << "!" << endl << endl;
		verStr += "��ϲ������" + _name + "�����ɹ�����ǰ�ȼ�Ϊ" + to_string(_level) + "!";
		verStr += "##";
		return true;
	}
	cout << endl;
	verStr += "#";
	return false;
}

//��þ���ֵ���ж�����,trueΪ������falseδ����
bool Pokemon::gainExp(int val) {
	val = max(val, 1);
	cout << "����" << _name << "���" << val << "����ֵ!" << endl;

	if (_level == 15) {
		cout << "����������������������!" << endl;
		return false;
	}

	_exp += val;
	bool tag = false;
	while (_level < 15 && _exp >= _race[_raceId]->expValue[_level + 1]) {
		tag = true;
		_level++;

		_atk += 5 + getRand(0, 4);
		_def += 4 + getRand(0, 6);
		_hp += 10 + getRand(0, 8);
		_sp += 4 + getRand(0, 4);

		if (_raceId == PowerType) _atk += 5;
		else if (_raceId == MeatShieldType) _hp += 10;
		else if (_raceId == DefensiveType) _def += 5;
		else if (_raceId == AgileType) _sp += 3;

	}
	if (tag) {
		cout << "��ϲ������" << _name << "�����ɹ�����ǰ�ȼ�Ϊ" << _level << "!" << endl << endl;
		return true;
	}
	cout << endl;
	return false;
}


//��Ϣ���
void Pokemon::print() {
	cout << "==================���鵵��==================" << endl;
	cout << "����������:" << _raceId << endl;
	cout << "��������:" << _name << endl;
	cout << "�ȼ�:" << _level << endl;
	cout << "����ֵ:" << _exp << endl;
	cout << "������:" << _atk << endl;
	cout << "������:" << _def << endl;
	cout << "����ֵ:" << _hp << endl;
	cout << "�����ٶ�:" << _sp << endl<<endl;
	cout << "��������:" << getRaceName() << endl;
	cout << "������:" << _race[_raceId]->pokemonType << endl;
	cout << "���幥����:" << _race[_raceId]->baseAtk << endl;
	cout << "���������:" << _race[_raceId]->baseDef << endl;
	cout << "��������ֵ" << _race[_raceId]->baseHp << endl;
	cout << "���幥���ٶ�:" << _race[_raceId]->baseSp << endl;
	cout << "��������::" << "   ";
	for (int i = 1; i <= 15; i++) cout << getExpValue(i) << " "; cout << endl;
	cout << "��������::" << "   ";
	for (int i = 0; i < 5; i++) cout << getSkillName(i) << " "; cout << endl;
	cout << "���ܿ��ô���::" << "   ";
	for (int i = 0; i < 5; i++) cout << getPp(i) << " "; cout << endl;
	cout << "================================================" << endl << endl;
}

