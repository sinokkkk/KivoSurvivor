#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
extern Atlas atlas_intro;

extern SceneManager scene_manager;

class MenuScene : public Scene{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
		animation_intro.set_atlas(&atlas_intro);
		animation_intro.set_interval(3000 );
		animation_intro.set_loop(true);
	}
	void on_update(int delta) {
		animation_intro.on_update(delta);
	}
	void on_draw(const Camera& camera) 
	{
		animation_intro.on_draw(camera, 0, 0);
	}

	void on_input(const ExMessage& msg) 
	{ 
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
	void on_exit(){
		mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
	}
private:
	Animation animation_intro;
};

#endif // !_MENU_SCENE_H_
