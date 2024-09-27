//�ڹ�
#pragma once
#include "collider.h"

class Tortoise :
	public Collider
{
private:
	double sx = 0, sy = 0;
	int direction;
	Costume ct;
	int animation_time = -1;
	int state = 0; //0��·��1˯����ֹ��2����
	int sleep_time; //˯�߿�ʼʱ��
	bool killed = false; //�Ƿ񱻻�ɱ
protected:
	bool report_collision(int direction, Collider* target, int target_collider_layer);
public:
	Costume getcostume();
	std::pair<double, double> getctpos();
	bool update();
	void kill(int direction);
	Tortoise(char* s);
};
