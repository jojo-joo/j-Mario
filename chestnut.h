//�������
#pragma once
#include "collider.h"
class Chestnut :
    public Collider
{
private:
	double sx = 0, sy = 0;
	int direction;
	Costume ct;
	int animation_time = -1;
	int state = 0; //0������1���ᣬ2����
	bool killed = false; //�Ƿ񱻻�ɱ
protected:
	bool report_collision(int direction, Collider* target, int target_collider_layer);
public:
	Costume getcostume();
	std::pair<double, double> getctpos();
	bool update();
	void kill(int direction);
	Chestnut(char* s);
};

