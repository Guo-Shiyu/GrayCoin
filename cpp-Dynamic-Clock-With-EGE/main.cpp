#include <graphics.h>
#include <math.h>
#include <ctime>
#include <iostream>
#include "class.h"
#include <cstdlib>


void one_sec_repeat_alerm_example()
{
	ClockDial* clock = new ClockDial();
	Timer* timer = new Timer(clock->inner(), true, (time_t)Period::Second);
	clock->insert_timer(*timer);
	
	for (size_t frame = 1; is_run(); delay_fps(60), frame++ )
	{	
		if (frame % 60 == 0) 
			clock->tick();
		cleardevice();
	 	clock->display();
	 	if (frame == 60 * 25 ) 
			break;
	}
	delete clock;
	delete timer;
}

void one_min_unrepeat_alerm_example()
{
	ClockDial* clock = new ClockDial();
	Timer* timer = new Timer(clock->inner(), false, (time_t)Period::Mintue);
	clock->insert_timer(*timer);
	
	for (size_t frame = 1; is_run(); delay_fps(60), frame++ )
	{	
		if (frame % 60 == 0) 
			clock->tick();
		cleardevice();
	 	clock->display();
	 	if (frame == 60 * 80) 
	 		break;
	}
	
	delete clock;
	delete timer;
}

void timer(size_t second)
{
	ClockDial* clock = new ClockDial(second);	
	
	int sec = 0;
	for (size_t frame = 1; is_run(); delay_fps(60), frame++ )
	{	
		if (frame % 60 == 0) 
			clock->fly(-1);
		cleardevice();
	 	clock->display();
	 	if (frame % 60  == 0) // stop 2s 
	 		sec++;
	 	if (sec == second + 2)
	 		return;
	}
	
	delete clock;
}

size_t read_time(std::string info = " ") 
{
	std::string buffer;
		for(;is_run();delay_fps(15)) 
		{
			xyprintf(50,30, info.c_str());
			xyprintf(50,50, "������ʱ�䣺 ����  12:23:24, �����Ը�ʽ����");
			xyprintf(70, 70, buffer.c_str());
			
			// deal with keyboard message 
			while(kbmsg())
			{
				key_msg key = getkey();
				if (key.msg == key_msg_char) 
				{
					int ch = key.key;
					if (std::isdigit(ch) || ch == ':')
						buffer.push_back(ch);
					if (ch == VK_SPACE || ch == 0x0d)	// enter or space  
					{
						int hour = std::stoi(buffer);
						buffer.erase(buffer.begin(), buffer.begin() + buffer.find_first_of(':') + 1);
						int min = std::stoi(buffer);
						buffer.erase(buffer.begin(), buffer.begin() + buffer.find_first_of(':') + 1);
						int sec = std::stoi(buffer);
						int ret = hour * (size_t)Period::Hour + min * (size_t)Period::Mintue + sec;
						buffer.clear();
						return ret;
					}
				}
			}
			continue;
		}
	return -1;
}

void test() 
{
	ClockDial* clock = new ClockDial();
	for(size_t frame = 1;is_run();delay_fps(60), frame++)
	{
		// prepare for draw 
		cleardevice();
		xyprintf(50, 30, "ע�⣬ ÿ���л�����ʱ������ն�ʱ������");
		xyprintf(50, 50, "1.��ǰʱ��");
		xyprintf(50, 70, "2.����ʱ�� ����ʱ������ֹ����");
		xyprintf(50, 90, "3.�����ظ���ʱ��");
		xyprintf(50, 110, "4.���ò��ظ���ʱ��");
		xyprintf(50, 130, "5.����ÿ�����ظ��Ķ�ʱ������ʾ��ʱ���ܣ�");
		xyprintf(50, 150, "6.�Զ���ʱ��");
		xyprintf(50, 170, "7.���ò��ظ����ӣ���ʱ����");
		xyprintf(50, 190, "8.�����ظ����ӣ�ͬ��");
		
		
		// deal with keyboard message 
		while(kbmsg())
		{
			key_msg key = getkey();
			if (key.msg == key_msg_char) 
			{
				int ch = key.key;
				if (std::isdigit(ch))
				{
					cleardevice();
					switch (ch) {
						case '1':
							delete clock;
							clock = new ClockDial();
							break;
						
						case '2':
							timer(read_time("����ʱ��"));
							return test();
							break;

						case '3':
							{
							Timer* timer = new Timer(clock->inner(), true, (time_t)read_time());
							clock->insert_timer(*timer);
							}
							break;
							
						case '4':
							{
							Timer* timer = new Timer(clock->inner() + (time_t)read_time(), false, 0);
							clock->insert_timer(*timer);
							}
							break;
							
						case '5':
							{
								Timer* timer = new Timer(clock->inner(), true, (time_t)Period::Second);
								clock->insert_timer(*timer);
							}
							break;
							
						case '6':
							{
								std::time_t cur = clock->inner();
								cur = cur - clock->hour() * (size_t)Period::Hour - clock->mintue() * (size_t)Period::Mintue - clock->second();
								delete clock;
								clock = new ClockDial(cur + read_time("��Ҫ��ʱ���ǣ�"));
							}
							break;
							
						case '7':
							{
								std::time_t cur = clock->inner();
								cur = cur - clock->hour() * (size_t)Period::Hour - clock->mintue() * (size_t)Period::Mintue - clock->second();
								Timer* timer = new Timer(read_time("��Ҫ�趨�������ǣ�") + cur, false, 0);
								clock->insert_timer(*timer);
							}
							break;
							
						case '8':
							{
								std::time_t cur = clock->inner();
								cur = cur - clock->hour() * (size_t)Period::Hour - clock->mintue() * (size_t)Period::Mintue - clock->second();
								std::time_t target = read_time("��Ҫ�趨�������ǣ�") + cur;
								cleardevice();
								std::time_t freq = read_time("��Ҫ�趨���ظ�Ƶ����(�೤ʱ���ظ�һ��)��") + cur;
								Timer* timer = new Timer(target, true, freq);
								clock->insert_timer(*timer);
							}
							break;						
						default:
							break;
					}
					if(ch != '6')
						clock->fly(std::time(nullptr) - clock->inner());
					break;
				}
			}
		}
		
		// show graph 
		clock->display();
		
		// logic 
		if (frame % 60 == 0) 
		{
			clock->tick();
		}
	}
}



int main()
{
	initgraph( 1000, 900 );
	setcolor( RGB(0xff, 0xff, 0xff) );
	setfillcolor( RGB(0, 0, 0xff) );
	setrendermode(RENDER_MANUAL);
	ege::ege_enable_aa(true);
	cleardevice();
	
	test();
	
	one_sec_repeat_alerm_example();
	one_min_unrepeat_alerm_example();
	// timer_for_30s_example();
	
    return 0;
}



/*


 * 
     
     
     
 * 
 * 
 * 
 */
