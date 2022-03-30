#include <graphics.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

struct Circle{
	int x, y, r;
};


int main () {
	
	const int R = 255, G = 255, B = 255;
	// ����ɫ
	////////////////////////////// ��С�뾶����ע����ɶ��
	const int radius = 50;
	// ���뾶
	const int WW = 800, WH = 600, times = 3000;
	// ���ڿ�ȣ����ڸ߶ȣ�Ȧ��
	// �������������ڵĴ�С����������ͼƬ�Ĵ�С
	std::vector <Circle> CircleList;
	int count = 0;
	// ԲȦ������

	srand (time (nullptr));
	initgraph (WW, WH);

	do {
		int x = rand () % WW;
		int y = rand () % WH;
		int r = rand () % radius;
		// ȡģ ��ֹԲ get ���� pixel���޶�԰��СΪ 50 px
		for (auto a : CircleList) {
			int l = sqrt (pow (abs (x - a.x), 2) +
			              pow (abs (y - a.y), 2)
						 ) - a.r;
			// ��ǰ������µ����뾶
			// �����ÿһ��Բ����������Ӧ�����Ż���������û���������µ��˺����ٶȾͷǳ�����
			r = r > l ? l : r;
			// ѡ��뾶��С����һ��
			if (r <= 0) break;
			else 
				Sleep(20);
			// ������Ϸ��������ѭ��(���� while ���Ǹ�)
			// Ҳ����ԲҪ�ǳ������˱��Բ��������������
		}

		// circle (x, y, r);
		// �ֶ�ѡ��ʵ�Ļ��ǿ���
		fillellipse (x, y, r, r);
		// ����ͼ��
		CircleList.push_back ({x,y,r});
		count += 1;
		// ����ֵ��һ
		
	} while (count <= times);

	getch();
	closegraph ();
	return 0;
}
