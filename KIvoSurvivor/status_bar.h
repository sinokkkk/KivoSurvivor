#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "util.h"
#include <algorithm>  // 新增：用于std::max和std::min函数

class StatusBar{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_position(int x, int y)
	{
		position.x = x, position.y = y;
	}

	void set_hp(int val)
	{
		hp = val;
	}

	void set_mp(int val)
	{
		mp = val;
	}

	void on_draw()
	{
		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x, position.y, position.x + width + 3 * 2, position.y + 26, 8, 8);
		
		float hp_bar_width = width * max(0, hp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x, position.y, position.x + (int)hp_bar_width + 3, position.y + 23, 8, 8);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x, position.y + 35, position.x + width + 3 * 2, position.y + 61, 8, 8);
		
		float mp_bar_width = width * min(100, mp) / 100.0f;
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x, position.y + 35, position.x + (int)mp_bar_width + 3, position.y + 58, 8, 8);
	}

private:
	const int width = 275;

private:
	int hp = 0;
	int mp = 0;
	POINT position = { 0 };
};


#endif // !_STATUS_BAR_H_
