//������Ϣ����
#pragma once
#include "graphics.h"
#include <vector>
class Keymsg
{
private:
	std::vector<key_msg>msg; //������Ϣ�б�
public:
	void update();
	bool getmsg(key_msg& m, int key);
	bool is_down[256]; //�Ƿ񱻰���
	int down_time[256]; //���µ�ʱ��
};

extern Keymsg keymsg;
