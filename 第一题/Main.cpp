#include "Pokemon.h"
//测试
int main() {
	srand(time(NULL));

	//四个种族的小精灵
	Pokemon A = Pokemon(DefensiveType, "杰尼龟");
	A.print();
	Pokemon B = Pokemon(PowerType, "小火龙");
	B.print();
	Pokemon C = Pokemon(MeatShieldType, "妙蛙种子");
	C.print();
	Pokemon D = Pokemon(AgileType, "皮卡丘");
	D.print();

	//升级
	A.gainExp(33);
	B.gainExp(3);
	
	A.print();
	B.print();

	//战斗准备以及战斗
	A.recovery();
	B.recovery();

	A._race[A._raceId]->attack(A, B, 1);
	B._race[B._raceId]->attack(B, A, 0);

	//重新升级
	B.gainExp(30);

	A.print();
	B.print();

	A.recovery();
	B.recovery();
	B._race[B._raceId]->attack(B, A, 0);
	A._race[A._raceId]->attack(A, B, 0);

	//同等级四种精灵属性比较
	C.gainExp(33);
	D.gainExp(33);
	C.print();
	D.print();


	//战斗信息
	A.gainExp(103);
	B.gainExp(103);
	A.recovery();
	B.recovery();
	while (true) {
		if (A.attack(B, true)) {
			cout << "精灵" << A._name << "战胜了精灵" << B._name << "!\n" << endl;
			A.gainExp(4100);
			break;
		}
		if (B.attack(A, true)) {
			cout << "精灵" << B._name << "战胜了精灵" << A._name << "!\n" << endl;
			B.gainExp(4100);
			break;
		}
	}
	/*
	cout << A._raceId << endl;
	cout << A._name << endl;
	cout << A._exp << endl;
	cout << A._atk << endl;
	cout << A._def << endl;
	cout << A._hp << endl;
	cout << A._sp << endl;
	cout << A.getRaceName() << endl;
	cout << A._race[A._raceId]->pokemonType << endl;
	cout << A._race[A._raceId]->baseAtk << endl;
	cout << A._race[A._raceId]->baseDef << endl;
	cout << A._race[A._raceId]->baseHp << endl;
	cout << A._race[A._raceId]->baseSp << endl;
	for (int i = 1; i <= 15; i++) cout << A.getExpValue(i) << endl;
	for (int i = 0; i < 5; i++) cout << A.getSkillName(i) << endl;
	for (int i = 0; i < 5; i++) cout << A.getPp(i) << endl;
	*/

	return 0;
}