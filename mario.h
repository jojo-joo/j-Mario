//�����
#pragma once
#include "collider.h"
#include "global.h"
#include <vector>
#include <iostream>
class Mario :
	public Collider
{
private:
	int jump_time = 0; //��Ծ��ʼʱ��
	std::string state = "walk"; //״̬
	double sx = 0, sy = 0;
	Costume ct = Costume{2, 0, 6};
	Costume change_ct;
	int input_direction = 0; //���뷽�������
	int mario_level = 2;     //����µȼ���1������£�2С����£�3���������
	int animation_time = 0;      //����ʱ���
	int change_time = 0;         //�ı�ȼ�ʱ���
	int invincible_animation_time = 0; //�޵ж�����˸ʱ���
	int fire_time = 0; //�������ʱ��
	bool jump_key = false;   
	bool jump_sound = false;
	bool pole_direction;     //���췽��
	bool is_squat = false; //�Ƿ��¶�
	bool is_dash = false; //�Ƿ��ڳ��״̬
	bool change_size = false; //�Ƿ�ı�״̬��С
	void downgrade(); //��ʧ״̬
	bool standup();   //��������
	void squat(); //�¶�
	void change_level(int target);
protected:
	bool report_collision(int direction, Collider* target, int target_collider_layer);
public:
	Costume getcostume();
	std::pair<double, double> getctpos();
	bool update();
	std::vector<Costume>costumes;
	Mario();
	void render(double x, double y);
	int mario_fire_num = 0; //����������
	int invincible_state_time = 0; //�޵���ʱ��
};