//�ؿ�����
#pragma once
#include "collider.h"
#include "global.h"
#include "mario.h"
#include <vector>
#include <iostream>
class Level
{
private:
	bool isshow = true;
	bool isrun = true;
	void reset();
	void basic_block();
	char s[256];
	std::string now_music; //��ǰ������������
	int update_pos = 0; //�������ҵ�λ��
	bool invincible = false; //�޵���״̬
public:
	std::vector<Collider*>mp[MAX_LEVEL_LAYER + 5][MAX_LEVEL_RANGE + 5]; //��̬��ɫ
	std::vector<Collider*>actors[MAX_LEVEL_LAYER + 5]; //��̬��ɫ
	std::vector<Collider*>unrun_actors[MAX_LEVEL_RANGE + 5]; //�����еĶ�̬��ɫ
	Collider* addobject(char* s, double x, double y); //��x,y ������ s ������ɫ
	Mario* mario; //����µĵ�ַ
	void death(); //����
	void finish(); //�ؿ�����
	void restart(); //���¿�ʼ
	void start(const char* path); //��ȡ�ؿ��ļ�
	void start(); //��ʼ
	void stop(); //ֹͣ
	bool remove(Collider* t); //ɾ����ɫ
	bool update(); //�ؿ�����
	bool running(); //�Ƿ�������
	bool freeze = true; //�Ƿ��ڶ���״̬
	int map_range; //�ؿ���Χ
	int limit_time = 300; //ʱ������
	int start_time; //��ʼʱ��
	int last_time; //��һ֡ʱ���
	int now_time; //��ǰ����ʱ���
	int death_time = 0; //����ʱ��
	int finish_time = 0; //����ʱ��
	bool finish_move = false; //�Ƿ�ʼ�����ƶ�����
	Level();
};

extern Level level;