#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "timer.h"
#include "scene.h"
#include "bullet.h"
#include "status_bar.h"
#include "aris_player.h"

#include <vector>
#include "scene_manager.h"
#include "enemy.h"
#include "enemy_slow.h"
#include "enemy_fast.h"

extern bool is_debug;
extern IMAGE img_game_background;
extern Camera main_camera;
extern std::vector<Bullet*> bullet_list;
extern Player* player_1;
extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene()
	{
		// 设置敌人生成计时器
		timer_enemy_spawn.set_wait_time(2000);  // 每2秒生成一次
		timer_enemy_spawn.set_callback([&]()
			{
				spawn_enemy();
			});
	}

	void on_enter() override
	{
		player_1 = new ArisPlayer();
		status_bar.set_position(20, 20);
		player_1->set_position(getwidth() / 2, getheight() / 2);
		pos_img_background.x = 0;
		pos_img_background.y = 0;
		bullet_list.clear();
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		// 开始生成敌人
		timer_enemy_spawn.restart();
	}

	void on_update(int delta) override
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




		// 更新敌人生成计时器
		timer_enemy_spawn.on_update(delta);

		// 更新所有敌人
		for (Enemy* enemy : enemy_list)		{enemy->on_update(delta);}

		// 移除死亡的敌人
		enemy_list.erase(std::remove_if(
			enemy_list.begin(), 
			enemy_list.end(),
			[](Enemy* enemy){
				if (!enemy->is_alive()){delete enemy;  return true;}
				return false;
			}),//wtf lamda
			enemy_list.end());

		
	}

	void on_draw(const Camera& camera) override{
		putimage_alpha(camera, pos_img_background.x, pos_img_background.y, &img_game_background);
		player_1->on_draw(camera);
		for (const Bullet* bullet : bullet_list)
			bullet->on_draw(camera);
		
		if (is_debug){
			settextcolor(RGB(255, 0, 0));
			//outtextxy(15, 15,_T("调试模式已开启，按 'Q' 键关闭"));
		}
		
		// 绘制所有敌人
		for (Enemy* enemy : enemy_list){
			enemy->on_draw(camera);
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
	
	void on_exit() override
	{
		delete player_1; player_1 = nullptr;
		is_debug = false;
		main_camera.reset();
		// 清理所有敌人
		for (Enemy* enemy : enemy_list)
		{
			delete enemy;
		}
		enemy_list.clear();
	}

private:
	void spawn_enemy()
	{
		// 随机选择敌人类型
		Enemy* enemy = nullptr;
		if (rand() % 2 == 0)
			enemy = new SlowEnemy();
		else
			enemy = new FastEnemy();

		// 随机选择生成位置（屏幕边界）
		enum class SpawnEdge { Up, Down, Left, Right };
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		
		switch (edge)
		{
		case SpawnEdge::Up:
			enemy->set_position(rand() % getwidth(), -enemy->get_size().y);
			break;
		case SpawnEdge::Down:
			enemy->set_position(rand() % getwidth(), getheight());
			break;
		case SpawnEdge::Left:
			enemy->set_position(-enemy->get_size().x, rand() % getheight());
			break;
		case SpawnEdge::Right:
			enemy->set_position(getwidth(), rand() % getheight());
			break;
		}

		// 设置目标玩家
		enemy->set_target(player_1);

		// 添加到敌人列表
		enemy_list.push_back(enemy);
	}

private:
	StatusBar status_bar;
	POINT pos_img_background = { 0 };
	Timer timer_enemy_spawn;
	std::vector<Enemy*> enemy_list;
};

#endif // !_GAME_SCENE_H_
