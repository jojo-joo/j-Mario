//�÷ָ�����ʾģ��
#pragma once
#include "collider.h"
class Add_score :
    public Collider
{
private:
	double sx = 0, sy = 0; //������Ⱦ��������ĵ�ƫ��
	bool type; //�Ƿ�Ϊ�յ�÷���ʾ
	int score; //��ʾ�ĵ÷�
	int alpha = 255; //͸���ȣ�����Ч����
	int appear_time; //�÷ֳ���ʱ��
public:
	std::pair<double, double> getctpos();
	bool update();
	Add_score(double x, double y, int score, bool type);
	void render(double x, double y);
};