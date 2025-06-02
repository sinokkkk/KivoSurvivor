#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "timer.h"
#include "scene.h"
#include "bullet.h"
#include "status_bar.h"
#include "aris_player.h"

#include <vector>
#include "scene_manager.h"

extern bool is_debug;
extern IMAGE img_game_background;
extern Camera main_camera;
extern std::vector<Bullet*> bullet_list;
extern Player* player_1;
extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() 
	{
		player_1 = new ArisPlayer();
		status_bar.set_position(20, 20);
		player_1->set_position(getwidth() / 2, getheight() / 2);
		pos_img_background.x = 0;
		pos_img_background.y = 0;
		bullet_list.clear();
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{
		main_camera.on_update(delta);
		player_1->on_update(delta);
		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end());
		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);
		status_bar.set_hp(player_1->get_hp());
		status_bar.set_mp(player_1->get_mp());
	}

	void on_draw(const Camera& camera) 
	{
		putimage_alpha(camera, pos_img_background.x, pos_img_background.y, &img_game_background);
		player_1->on_draw(camera);
		for (const Bullet* bullet : bullet_list)
			bullet->on_draw(camera);
		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			//outtextxy(15, 15,_T("调试模式已开启，按 'Q' 键关闭"));
		}
		status_bar.on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		switch (msg.message)
		{
		case WM_KEYUP:
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}
	
	void on_exit()
	{
		delete player_1; player_1 = nullptr;
		is_debug = false;
		main_camera.reset();
	}

private:
	StatusBar status_bar;
	POINT pos_img_background = { 0 };
};

#endif // !_GAME_SCENE_H_
