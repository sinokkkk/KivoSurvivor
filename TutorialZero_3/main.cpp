#include "atlas.h"
#include "bullet.h"
#include "player.h"
#include "platform.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "scene_manager.h"
#include "selector_scene.h"

#include <vector>

#pragma comment(lib, "Winmm.lib")

bool is_debug = false;

Camera main_camera;
SceneManager scene_manager;

Atlas atlas_intro;

IMAGE img_aris_chosen;
IMAGE img_aris_unchosen;
IMAGE img_unlocked_chosen;
IMAGE img_unlocked_unchosen;
IMAGE img_selector_background;
IMAGE img_aris_selector_background;
//我们的select

IMAGE img_game_background;
Atlas atlas_aris_idle_left;
Atlas atlas_aris_idle_right;
Atlas atlas_aris_run_left;
Atlas atlas_aris_run_right;
Atlas atlas_aris_attack_ex_left;
Atlas atlas_aris_attack_ex_right;
Atlas atlas_aris_die_left;
Atlas atlas_aris_die_right;

IMAGE img_aris_bullet;
Atlas atlas_aris_bullet_hit;


Atlas atlas_run_effect;
Atlas atlas_jump_effect;
Atlas atlas_land_effect;

// 慢速敌人动画
Atlas atlas_enemy_slow_idle_left;
Atlas atlas_enemy_slow_idle_right;
Atlas atlas_enemy_slow_run_left;
Atlas atlas_enemy_slow_run_right;
Atlas atlas_enemy_slow_hurt_left;
Atlas atlas_enemy_slow_hurt_right;
Atlas atlas_enemy_slow_die_left;
Atlas atlas_enemy_slow_die_right;

// 快速敌人动画
Atlas atlas_enemy_fast_idle_left;
Atlas atlas_enemy_fast_idle_right;
Atlas atlas_enemy_fast_run_left;
Atlas atlas_enemy_fast_run_right;
Atlas atlas_enemy_fast_hurt_left;
Atlas atlas_enemy_fast_hurt_right;
Atlas atlas_enemy_fast_die_left;
Atlas atlas_enemy_fast_die_right;



Scene* menu_scene = nullptr;
Scene* selector_scene = nullptr;
Scene* game_scene = nullptr;

Player* player_1 = nullptr;

std::vector<Bullet*> bullet_list;


void flip_atlas(Atlas& src, Atlas& dst)
{
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src.get_image(i), &img_flipped);
		dst.add_image(img_flipped);
	}
}

void load_game_resources()
{
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);

	atlas_intro.load_from_file(_T("resources/INTRO_%d.png"), 5);//开场动画
	loadimage(&img_aris_chosen, _T("resources/aris_chosen.png"));
	loadimage(&img_aris_unchosen, _T("resources/aris_unchosen.png"));
	loadimage(&img_unlocked_chosen, _T("resources/unlocked_chosen.png"));
	loadimage(&img_unlocked_unchosen, _T("resources/unlocked_unchosen.png"));
	loadimage(&img_selector_background, _T("resources/selector_background.png"));
	loadimage(&img_aris_selector_background, _T("resources/aris_selector_background.png"));

	loadimage(&img_game_background, _T("resources/game_background.png"));
	loadimage(&img_aris_bullet, _T("resources/aris_bullet.png"));

	atlas_aris_idle_left.load_from_file(_T("resources/aris_idle_%d.png"), 1);
	flip_atlas(atlas_aris_idle_left, atlas_aris_idle_right);
	atlas_aris_run_left.load_from_file(_T("resources/aris_run_%d.png"), 1);
	flip_atlas(atlas_aris_run_left, atlas_aris_run_right);
	atlas_aris_attack_ex_left.load_from_file(_T("resources/aris_attack_ex_%d.png"), 1);
	flip_atlas(atlas_aris_attack_ex_left, atlas_aris_attack_ex_right);
	atlas_aris_die_left.load_from_file(_T("resources/aris_die_%d.png"), 1);
	flip_atlas(atlas_aris_die_left, atlas_aris_die_right);

	atlas_aris_bullet_hit.load_from_file(_T("resources/aris_bullet_hit_%d.png"), 5);



	atlas_run_effect.load_from_file(_T("resources/run_effect_%d.png"), 4);
	// atlas_jump_effect.load_from_file(_T("resources/jump_effect_%d.png"), 5);
	// atlas_land_effect.load_from_file(_T("resources/land_effect_%d.png"), 2);

	atlas_enemy_slow_idle_left.load_from_file(_T("resources/enemy_slow_idle_%d.png"), 2);
	flip_atlas(atlas_enemy_slow_idle_left, atlas_enemy_slow_idle_right);
	atlas_enemy_slow_run_left.load_from_file(_T("resources/enemy_slow_run_%d.png"), 2);
	flip_atlas(atlas_enemy_slow_run_left, atlas_enemy_slow_run_right);
	atlas_enemy_slow_hurt_left.load_from_file(_T("resources/enemy_slow_hurt_%d.png"), 2);
	flip_atlas(atlas_enemy_slow_hurt_left, atlas_enemy_slow_hurt_right);
	atlas_enemy_slow_die_left.load_from_file(_T("resources/enemy_slow_die_%d.png"), 2);
	flip_atlas(atlas_enemy_slow_die_left, atlas_enemy_slow_die_right);

	atlas_enemy_fast_idle_left.load_from_file(_T("resources/enemy_fast_idle_%d.png"), 1);
	flip_atlas(atlas_enemy_fast_idle_left, atlas_enemy_fast_idle_right);
	atlas_enemy_fast_run_left.load_from_file(_T("resources/enemy_fast_run_%d.png"), 1);
	flip_atlas(atlas_enemy_fast_run_left, atlas_enemy_fast_run_right);
	atlas_enemy_fast_hurt_left.load_from_file(_T("resources/enemy_fast_hurt_%d.png"), 1);
	flip_atlas(atlas_enemy_fast_hurt_left, atlas_enemy_fast_hurt_right);
	atlas_enemy_fast_die_left.load_from_file(_T("resources/enemy_fast_die_%d.png"), 1);
	flip_atlas(atlas_enemy_fast_die_left, atlas_enemy_fast_die_right);



	// loadimage(&img_1P_winnner, _T("resources/1P_winner.png"));
	// loadimage(&img_2P_winnner, _T("resources/2P_winner.png"));
	// loadimage(&img_winnner_bar, _T("resources/winnner_bar.png"));


	mciSendString(_T("open resources/bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
	mciSendString(_T("open resources/bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
	mciSendString(_T("open resources/bgm_select.mp3 alias bgm_select"), NULL, 0, NULL);
	mciSendString(_T("open resources/aris_bullet_hit_1.mp3 alias aris_bullet_hit_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/aris_bullet_hit_2.mp3 alias aris_bullet_hit_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/aris_bullet_hit_3.mp3 alias aris_bullet_hit_3"), NULL, 0, NULL);

	mciSendString(_T("open resources/ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_switch.wav alias ui_switch"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_win.wav alias ui_win"), NULL, 0, NULL);
}

int main(int argc, char** argv)
{
	ExMessage msg;
	const int FPS = 360;

	load_game_resources();

	menu_scene = new MenuScene();
	selector_scene = new SelecorScene();
	game_scene = new GameScene();

	scene_manager.set_current_scene(menu_scene);

	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
	SetWindowText(hwnd, _T("ֲ1"));
	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);

	BeginBatchDraw();

	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
			scene_manager.on_input(msg);

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(main_camera);
		FlushBatchDraw();

		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
			Sleep(1000 / FPS - frame_delta_time);
	}

	EndBatchDraw();

	return 0;
}