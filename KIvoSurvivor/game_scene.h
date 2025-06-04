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
extern bool is_game_over;

class GameScene : public Scene{
public:
	GameScene()
	{
		// 设置敌人生成计时器
		timer_enemy_spawn.set_wait_time(1500);  // 每2秒生成一次
		timer_enemy_spawn.set_callback([&]()
			{
				spawn_enemy();
			});
	}

	void on_enter() override
	{
		mciSendString(_T("stop bgm_shop "), NULL, 0, NULL);
		// 如果玩家已经存在，说明是从商店返回
		if (!player_1) {
			player_1 = new ArisPlayer();
			player_1->set_position(getwidth() / 2, getheight() / 2);
		}
		
		status_bar.set_position(20, 20);
		pos_img_background.x = 0;
		pos_img_background.y = 0;
		bullet_list.clear();
		mciSendString(_T("play aris_in from 0"), NULL, 0, NULL);
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		// 初始化波次
		if (current_wave == 0) {  // 只在第一次进入时初始化
			current_wave = 1;
			enemies_in_wave = 5;  // 第一波5个敌人
		}
		enemies_remaining = enemies_in_wave;

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

		// 检查玩家生命值，如果小于等于 0，触发游戏结束
		if (player_1 && player_1->get_hp() <= 0)
		{
			mciSendString(_T("play aris_defeat from 0"), NULL, 0, NULL);
			// 弹出消息框
			MessageBox(GetHWnd(), _T("游戏结束！"), _T("提示"), MB_OK);
			// 设置游戏结束标志，退出主循环
			is_game_over = true;
			return;  // 直接返回，不执行后续逻辑
		}

		// 更新敌人生成计时器
		timer_enemy_spawn.on_update(delta);

		// 更新所有敌人
		for (Enemy* enemy : enemy_list)		{enemy->on_update(delta);}

		// 移除死亡的敌人
		enemy_list.erase(std::remove_if(
			enemy_list.begin(), 
			enemy_list.end(),
			[this](Enemy* enemy){
				if (!enemy->is_alive()){
					enemies_remaining--;  // 减少剩余敌人计数
					delete enemy;
					return true;
				}
				return false;
			}),
			enemy_list.end());

		// 检查波次是否完成
		if (enemies_remaining <= 0 && enemy_list.empty()) {
			// 波次完成，切换到商店场景
			scene_manager.switch_to(SceneManager::SceneType::Shop);
		}
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

		// 绘制波次信息
		settextcolor(RGB(255, 255, 255));
		TCHAR wave_info[32];
		_stprintf_s(wave_info, _T("波次: %d"), current_wave);
		outtextxy(20, 90, wave_info);

		//// 绘制剩余敌人数量，有bug，不显示了
		//TCHAR enemy_info[32];
		//_stprintf_s(enemy_info, _T("剩余敌人: %d"), enemies_remaining);
		//outtextxy(20, 120, enemy_info);

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
		// 只有在游戏结束时才删除玩家
		if (is_game_over) {
			delete player_1;
			player_1 = nullptr;
		}
		
		is_debug = false;
		main_camera.reset();
		// 清理所有敌人
		for (Enemy* enemy : enemy_list)
		{
			delete enemy;
		}
		enemy_list.clear();

		
	}

public:  // 改为public，允许ShopScene调用
	void start_new_wave() {
		current_wave++;
		// 更陡峭的难度曲线
		enemies_in_wave = 5 + current_wave * 3;  // 每波增加3个敌人
		// 减少敌人生成间隔
		timer_enemy_spawn.set_wait_time(2000 - (current_wave - 1) * 100);  // 每波减少100ms，最低500ms
		enemies_remaining = enemies_in_wave;
		timer_enemy_spawn.restart();
	}

private:
	void spawn_enemy()
	{
		// 如果已经生成了足够的敌人，不再生成
		if (enemy_list.size() >= enemies_in_wave) {
			return;
		}

		// 随机选择敌人类型，随着波次增加快速敌人的比例
		Enemy* enemy = nullptr;
		int fast_chance = 30 + (current_wave - 1) * 5;  // 每波增加5%的快速敌人概率
		if (rand() % 100 < fast_chance)
			enemy = new FastEnemy();
		else
			enemy = new SlowEnemy();

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

	// 波次相关
	int current_wave = 0;          // 当前波次，0表示未初始化
	int enemies_in_wave = 5;       // 当前波次的敌人数量
	int enemies_remaining = 5;     // 当前波次剩余的敌人数量
};

#endif // !_GAME_SCENE_H_
