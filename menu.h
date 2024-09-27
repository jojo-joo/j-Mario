//��ҳ�˵�
#pragma once
#include "graphics.h"
#include <vector>
#include <map>
#include <iostream>
class Menu
{
private:
	bool isshow = true;
	bool isrun = true;
	PIMAGE title;
	MUSIC music; //���ڴ������漰��ʼ������ͨ������������

	bool render();
	void reset();
public:
	bool update();
	void stop();
	void start();
	Menu();
};
extern Menu menu;

class Option_cursor
{
private:
	bool render();
	bool isshow = true;
	bool isrun = true;
	int level_id = -1;
	PIMAGE icon;
	std::vector<std::string>levels; //���йؿ�����
public:
	bool update();
	Option_cursor();
	std::map<std::string, int>levels_top_score; //�ؿ���߷�
};
extern Option_cursor option_cursor;
