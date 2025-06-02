#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "scene.h"

#include "aris_player.h"
#include "util.h"

extern IMAGE img_aris_chosen;
extern IMAGE img_aris_unchosen;
extern IMAGE img_unlocked_chosen;
extern IMAGE img_unlocked_unchosen;
extern IMAGE img_selector_background;
extern IMAGE img_aris_selector_background;
//资源


extern SceneManager scene_manager;

int selector_background_scroll_offset_x = 0;  // 背景滚动偏移量

class SelecorScene : public Scene
{
public:
	SelecorScene() = default;
	~SelecorScene() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_select repeat from 0"), NULL, 0, NULL);

        int total_width = 4 * 300 + 3 * 10;  // 4张图片总宽度 + 3个间距
        int start_x = (getwidth() - total_width) / 2;  // 起始x坐标
        
        pos_img_selector_1.x = start_x;
        pos_img_selector_1.y = 40;  // 紧贴标题下方
        
        pos_img_selector_2.x = start_x + 300 + 10;  // 第一张图片宽度 + 间距
        pos_img_selector_2.y = 40;
        
        pos_img_selector_3.x = start_x + (300 + 10) * 2;  // 前两张图片宽度 + 两个间距
        pos_img_selector_3.y = 40;
        
        pos_img_selector_4.x = start_x + (300 + 10) * 3;  // 前三张图片宽度 + 三个间距
        pos_img_selector_4.y = 40;
		current_selection = PlayerType::Aris;
        //我们的

		/*
		// static const int OFFSET_X = 50;

		// animation_peashooter.set_atlas(&atlas_peashooter_idle_right);
		// animation_sunflower.set_atlas(&atlas_sunflower_idle_right);
		// animation_peashooter.set_interval(100);
		// animation_sunflower.set_interval(100);

		// pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		// pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
		// pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		// pos_img_tip.y = getheight() - 125;
		// pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		// pos_img_1P.y = 35;
		// pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		// pos_img_2P.y = pos_img_1P.y;
		// pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		// pos_img_1P_desc.y = getheight() - 150;
		// pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		// pos_img_2P_desc.y = pos_img_1P_desc.y;
		// pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		// pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
		// pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		// pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
		// pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		// pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
		// pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
		// pos_animation_2P.y = pos_animation_1P.y;
		// pos_img_1P_name.y = pos_animation_1P.y + 155;
		// pos_img_2P_name.y = pos_img_1P_name.y;
		// pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
		// pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
		// pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
		// pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		// pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
		// pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
		// pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
		// pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		*/
	}

	void on_update(int delta)
	{
		// 更新背景滚动偏移量
		selector_background_scroll_offset_x += 1;
		if (selector_background_scroll_offset_x >= img_aris_selector_background.getwidth())
			selector_background_scroll_offset_x = 0;
	}

	void on_draw(const Camera& camera)
	{
		putimage_alpha(0, 0, &img_selector_background);
		// 绘制滚动背景
		putimage_alpha(selector_background_scroll_offset_x - img_aris_selector_background.getwidth(), 0, &img_aris_selector_background);
		putimage_alpha(selector_background_scroll_offset_x, 0, &img_aris_selector_background);

		// 绘制四个角色图片
		putimage_alpha(pos_img_selector_1.x, pos_img_selector_1.y, 
			current_selection == PlayerType::Aris ? &img_aris_chosen : &img_aris_unchosen);

		// Midori (第二个位置)
		putimage_alpha(pos_img_selector_2.x, pos_img_selector_2.y, &img_unlocked_unchosen);

		// Yuzu (第三个位置)
		putimage_alpha(pos_img_selector_3.x, pos_img_selector_3.y, &img_unlocked_unchosen);

		// Momoi (第四个位置)
		putimage_alpha(pos_img_selector_4.x, pos_img_selector_4.y, &img_unlocked_unchosen);

		LPCTSTR str_aris_name =_T("alice");
    	LPCTSTR str_midori_name =_T("midori");
    	LPCTSTR str_yuzu_name =_T("momoi");
    	LPCTSTR str_momoi_name =_T("yuzu");
		    // 在角色图片下方绘制文字
		// Aris
		outtextxy_shaded(
			pos_img_selector_1.x + (300 - textwidth(str_aris_name)) / 2,  // 水平居中
			pos_img_selector_1.y + 300,  // 图片下方
			str_aris_name
		);

		// Midori
		outtextxy_shaded(
			pos_img_selector_2.x + (300 - textwidth(str_midori_name)) / 2,
			pos_img_selector_2.y + 300,
			str_midori_name
		);

		// Yuzu
		outtextxy_shaded(
			pos_img_selector_3.x + (300 - textwidth(str_yuzu_name)) / 2,
			pos_img_selector_3.y + 300,
			str_yuzu_name
		);

		// Momoi
		outtextxy_shaded(
			pos_img_selector_4.x + (300 - textwidth(str_momoi_name)) / 2,
			pos_img_selector_4.y + 300,
			str_momoi_name
		);
{
// 		IMAGE* img_p1_selector_background = nullptr;
// 		IMAGE* img_p2_selector_background = nullptr;

// 		switch (player_type_2)
// 		{
// 		case PlayerType::Peashooter:
// 			img_p1_selector_background = &img_peashooter_selector_background_right;
// 			break;
// 		case PlayerType::Sunflower:
// 			img_p1_selector_background = &img_sunflower_selector_background_right;
// 			break;
// 		default:
// 			img_p1_selector_background = &img_peashooter_selector_background_right;
// 			break;
// 		}

// 		switch (player_type_1)
// 		{
// 		case PlayerType::Peashooter:
// 			img_p2_selector_background = &img_peashooter_selector_background_left;
// 			break;
// 		case PlayerType::Sunflower:
// 			img_p2_selector_background = &img_sunflower_selector_background_left;
// 			break;
// 		default:
// 			img_p2_selector_background = &img_peashooter_selector_background_left;
// 			break;
// 		}

// 		putimage(0, 0, &img_selector_background);

// 		putimage_alpha(selector_background_scroll_offset_x - img_p1_selector_background->getwidth(), 0, img_p1_selector_background);
// 		putimage_alpha(selector_background_scroll_offset_x, 0, img_p1_selector_background->getwidth() - selector_background_scroll_offset_x, 0, img_p1_selector_background, 0, 0);
// 		putimage_alpha(getwidth() - img_p2_selector_background->getwidth(), 0, img_p2_selector_background->getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background, selector_background_scroll_offset_x, 0);
// 		putimage_alpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background);

// 		putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

// 		putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
// 		putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
// 		putimage_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);
// 		putimage_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);

// 		switch (player_type_1)
// 		{
// 		case PlayerType::Peashooter:
// 			animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
// 			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
// 			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
// 			break;
// 		case PlayerType::Sunflower:
// 			animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
// 			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
// 			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
// 			break;
// 		}

// 		switch (player_type_2)
// 		{
// 		case PlayerType::Peashooter:
// 			animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
// 			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;
// 			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
// 			break;
// 		case PlayerType::Sunflower:
// 			animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
// 			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;
// 			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
// 			break;
// 		}

// 		putimage_alpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y, is_btn_1P_left_down ?  &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
// 		putimage_alpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y, is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
// 		putimage_alpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y, is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
// 		putimage_alpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y, is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

// 		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
// 		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

// 		putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
}
		LPCTSTR str_select_title =_T("SELECT YOUR CHARACTER");
		outtextxy_shaded((getwidth() - textwidth(_T("SELECT YOUR CHARACTER"))) / 2, 10, str_select_title);
	}

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case VK_LEFT:
				// 向左切换，但只能切换到已解锁的角色
				if (current_selection >= PlayerType::Aris)
				{
					current_selection = PlayerType::Aris;
					mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				}
				break;
			case VK_RIGHT:
				// 向右切换，但只能切换到已解锁的角色
				if (current_selection <= PlayerType::Aris)
				{
					current_selection = PlayerType::Aris;
					mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				}
				break;
			case VK_RETURN:  // 添加回车键处理
				mciSendString(_T("stop bgm_select"), NULL, 0, NULL);
				// 播放确认音效
				mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
				// 切换到游戏场景
				scene_manager.set_current_scene(game_scene);
				break;
			}
			break;
		}
	}

	void on_exit() 
	{
		mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
		// 根据当前选择的角色创建对应的玩家实例
		switch (current_selection)
		{
		case PlayerType::Aris:
			player_1 = new ArisPlayer();  // 需要创建 ArisPlayer 类
			break;
		case PlayerType::Midori:
			// 暂时不处理，因为未解锁
			break;
		case PlayerType::Yuzu:
			// 暂时不处理，因为未解锁
			break;
		case PlayerType::Momoi:
			// 暂时不处理，因为未解锁
			break;
		}

		
	}

private:
	enum class PlayerType
	{
		Aris = 0,
		Midori,
		Yuzu,
		Momoi,
		Invalid
	};

private:
	PlayerType current_selection = PlayerType::Aris;  // 当前选择的角色

    POINT pos_img_selector_1 = { 0 };
    POINT pos_img_selector_2 = { 0 };
    POINT pos_img_selector_3 = { 0 };
    POINT pos_img_selector_4 = { 0 };
    //在四个位置根据情况显示不同的图片

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}
};

#endif // !_SELECTOR_SCENE_H_
