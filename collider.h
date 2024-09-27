//��ײ�У����й��ڹؿ���ɫ����̳��ڴ���
#pragma once
#include "global.h"
#include "camera.h"
#include "graphics.h"
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3
class Collider
{
private:
	bool checkcollide(double x, double y, const Collider* b); //�ж��Ƿ��Ŀ�귢����ײ
	bool move(double& x, double& y, double dx, double dy); //�����ƶ�
	std::pair<double, bool> checkonfloor(double prex, double prey); //�Ƿ��ڵذ���
	std::pair<double, bool> checkceiling(double prex, double prey); //�Ƿ�������
	std::pair<double, bool> checkleftright(); //�ж������Ƿ�����ǽ
	const int collide_re[10][10] = { //��ײͼ���ϵ��ʾ
		{3, 3, 1, 1, 1, 1, 1},
		{3, 0, 3, 0, 3, 0, 1},
		{1, 3, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0},
		{1, 3, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 0, 0, 0},
	};
	//0���ųⲻ��⣬1����⣬2���ų⣬3�ų��Ҽ��
	//0 ���1 ש�飬2 ���3 ��ˣ�4 Ģ��/����ש, 5 ���, 6����»���/�޵���
	//-1��ͼ�㣬���κ����岻������ײ
	std::set<Collider*>collision_history[2]; //��ʷ��ײ��¼��������
	int collision_history_pos = 0;
protected:
	bool is_jump = false;
	double f = 15; //Ħ����
	double m = 1;  //���������
	double maxwx = 20, maxwy = 20; //�����
	bool static_y;
	bool out_of_range = true; //��������ͼ��Χ
	bool last_direction = false; // ������false��, true ��
	std::vector<Collider*> get_all_contacts();
	virtual bool report_collision(int direction, Collider* target, int target_collider_layer);
	//������ײ (��ײ������ײ������ײ������ײͼ��)
	bool checklastcollision(Collider* target); //�ж��Ƿ�����������ײ
public:
	int id;
	bool isrun = true;  //�Ƿ�����
	bool isshow = true; //�Ƿ���ʾ
	double x, y;   //��ǰλ��
	double vx = 0, vy = 0; //��ǰ�ٶ�
	double width, height; //��ײ����
	double fx = 0, fy = 0; //��ǰ����
	bool onfloor = false; //�Ƿ��ڵ���
	bool freeze = true; //�Ƿ��ڶ���״̬
	virtual Costume getcostume(); //��ȡ����
	virtual std::pair<double, double> getctpos() = 0; //��ȡ����λ��
	virtual bool update(); //��ɫ����
	virtual void render(double x, double y); //��Ⱦ��ɫ
	virtual void kill(int direction); //��ɱ��ɫ
	void setpos(double x, double y, double width, double height); //����λ��
	std::pair<double, double> getpos(); //���λ��
	int collider_layer = 0; //��ײͼ��
	int show_layer = 0; //��ʾͼ��
	void calc(); //��ײ���������
	bool operator < (const Collider& c);
	void start();
	std::string name; //��ɫ����
};