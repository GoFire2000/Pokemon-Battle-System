#include "Pokemon.h"
//����
int main() {
	srand(time(NULL));

	//�ĸ������С����
	Pokemon A = Pokemon(DefensiveType, "�����");
	A.print();
	Pokemon B = Pokemon(PowerType, "С����");
	B.print();
	Pokemon C = Pokemon(MeatShieldType, "��������");
	C.print();
	Pokemon D = Pokemon(AgileType, "Ƥ����");
	D.print();

	//����
	A.gainExp(33);
	B.gainExp(3);
	
	A.print();
	B.print();

	//ս��׼���Լ�ս��
	A.recovery();
	B.recovery();

	A._race[A._raceId]->attack(A, B, 1);
	B._race[B._raceId]->attack(B, A, 0);

	//��������
	B.gainExp(30);

	A.print();
	B.print();

	A.recovery();
	B.recovery();
	B._race[B._raceId]->attack(B, A, 0);
	A._race[A._raceId]->attack(A, B, 0);

	//ͬ�ȼ����־������ԱȽ�
	C.gainExp(33);
	D.gainExp(33);
	C.print();
	D.print();


	//ս����Ϣ
	A.gainExp(103);
	B.gainExp(103);
	A.recovery();
	B.recovery();
	while (true) {
		if (A.attack(B, true)) {
			cout << "����" << A._name << "սʤ�˾���" << B._name << "!\n" << endl;
			A.gainExp(4100);
			break;
		}
		if (B.attack(A, true)) {
			cout << "����" << B._name << "սʤ�˾���" << A._name << "!\n" << endl;
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