//���ֲ��Ź���
#pragma once
#include "graphics.h"
#include <map>
#include <iostream>
class Musicplayer
{
private:
	std::map<std::string, MUSIC*> musics;
public:
	bool play(std::string s); //��������
	bool stop(std::string s); //ֹͣ����
	bool checkend(std::string s); //�жϲ����Ƿ����
	bool SetVolume(std::string s, double v); //������������
	bool isplay(std::string s); //�ж��Ƿ��ڲ���
	Musicplayer();
};

extern Musicplayer musicplayer;
