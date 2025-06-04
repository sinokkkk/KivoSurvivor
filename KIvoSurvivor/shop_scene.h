#ifndef _SHOP_SCENE_H_
#define _SHOP_SCENE_H_

#include "scene.h"
#include "camera.h"
#include "scene_manager.h"
#include "player.h"
#include "util.h"
#include "game_scene.h"
#include <vector>


extern IMAGE img_shop_background;
extern IMAGE img_shop_attack_up;
extern IMAGE img_shop_speed_up;
extern IMAGE img_shop_health_up;

extern Player* player_1;
extern SceneManager scene_manager;
extern Scene* game_scene;  // 改回Scene*类型

// 商品基类
class ShopItem {
public:
    ShopItem() = default;
    virtual ~ShopItem() = default;
    
    virtual void apply_effect() = 0;  // 应用商品效果
    virtual const TCHAR* get_name() const = 0;  // 获取商品名称
};

// 攻击力提升商品
class AttackUpItem : public ShopItem {
public:
    void apply_effect() override {
        if (player_1) {
            // 减少攻击冷却时间
            float current_cd = player_1->get_attack_cd();
            player_1->set_attack_cd(current_cd * 0.8f);  // 减少20%的冷却时间
        }
    }
    const TCHAR* get_name() const override {
        return _T("攻击速度提升");
    }
};

// 速度提升商品
class SpeedUpItem : public ShopItem {
public:
    void apply_effect() override {
        if (player_1) {
            // 增加移动速度
            float current_speed = player_1->get_run_velocity();
            player_1->set_run_velocity(current_speed * 1.2f);  // 增加20%的移动速度
        }
    }
    const TCHAR* get_name() const override {
        return _T("速度提升");
    }
};

// 生命值提升商品
class HealthUpItem : public ShopItem {
public:
    void apply_effect() override {
        if (player_1) {
            // 增加最大生命值并恢复生命值
            int current_hp = player_1->get_hp();
            int max_hp = player_1->get_max_hp();
            player_1->set_max_hp(max_hp + 50);  // 增加50点最大生命值
            player_1->set_hp(current_hp + 50);  // 恢复50点生命值
        }
    }
    const TCHAR* get_name() const override {
        return _T("生命值提升");
    }
};

// 商店场景类
class ShopScene : public Scene {
public:
    ShopScene() : current_selection(0), is_chosen(false) {}

    ~ShopScene() {
        for (auto item : items) {
            delete item;
        }
        items.clear();
    }

    void on_enter() override {
        mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
        mciSendString(_T("play bgm_shop from 0"), NULL, 0, NULL);
        mciSendString(_T("play aris_shop from 0"), NULL, 0, NULL);
        init_items();
        current_selection = 0;
        is_chosen = false;

        // 计算商品位置（上下均匀分布）
        int item_height = 200;  // 每个商品的高度（包含文字）
        int spacing = 20;      // 商品之间的间距
        int total_height = 3 * item_height + 2 * spacing;  // 总高度
        int start_y = (getheight() - total_height) / 2;    // 起始y坐标
        
        pos_item_1.x = (getwidth() - img_shop_attack_up.getwidth()) / 2;  // 水平居中
        pos_item_1.y = start_y;
        
        pos_item_2.x = (getwidth() - img_shop_speed_up.getwidth()) / 2;
        pos_item_2.y = start_y + item_height + spacing;
        
        pos_item_3.x = (getwidth() - img_shop_health_up.getwidth()) / 2;
        pos_item_3.y = start_y + (item_height + spacing) * 2;
    }

    void on_update(int delta) override {
        // 暂时不需要更新逻辑
    }

    void on_draw(const Camera& camera) override {
        // 绘制背景
        putimage_alpha(0, 0, &img_shop_background);
        
        // 绘制标题
        LPCTSTR str_shop_title = _T("Angel 24");
        outtextxy_shaded((getwidth() - textwidth(str_shop_title)) / 2, 10, str_shop_title);
        
        // 绘制商品
        draw_items();
    }

    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYDOWN) {
            switch (msg.vkcode) {
                case VK_UP:
                    select_prev_item();
                    break;
                case VK_DOWN:
                    select_next_item();
                    break;
                case VK_RETURN:  // Enter键
                    purchase_current_item();
                    break;
            }
        }
    }

    void on_exit() override {
        // 开始新的波次
        if (game_scene) {
            game_scene->start_new_wave();
        }
    }

private:
    void init_items() {
        // 清空现有商品//反正先写一个在这
        for (auto item : items) {
            delete item;
        }
        items.clear();

        // 添加三个固定商品
        items.push_back(new AttackUpItem());
        items.push_back(new SpeedUpItem());
        items.push_back(new HealthUpItem());
    }

    void draw_items() {
        // 绘制第一个商品（攻击速度提升）
        putimage_alpha(pos_item_1.x, pos_item_1.y, &img_shop_attack_up);
        if (current_selection == 0) {
            draw_selection_frame(pos_item_1.x, pos_item_1.y, img_shop_attack_up.getwidth(), img_shop_attack_up.getheight());
        }
        outtextxy_shaded(
            pos_item_1.x + (img_shop_attack_up.getwidth() - textwidth(items[0]->get_name())) / 2,
            pos_item_1.y + img_shop_attack_up.getheight() -70,
            items[0]->get_name()
        );

        // 绘制第二个商品（速度提升）
        putimage_alpha(pos_item_2.x, pos_item_2.y, &img_shop_speed_up);
        if (current_selection == 1) {
            draw_selection_frame(pos_item_2.x, pos_item_2.y, img_shop_speed_up.getwidth(), img_shop_speed_up.getheight());
        }
        outtextxy_shaded(
            pos_item_2.x + (img_shop_speed_up.getwidth() - textwidth(items[1]->get_name())) / 2,
            pos_item_2.y + img_shop_speed_up.getheight()-70 ,
            items[1]->get_name()
        );

        // 绘制第三个商品（生命值提升）
        putimage_alpha(pos_item_3.x, pos_item_3.y, &img_shop_health_up);
        if (current_selection == 2) {
            draw_selection_frame(pos_item_3.x, pos_item_3.y, img_shop_health_up.getwidth(), img_shop_health_up.getheight());
        }
        outtextxy_shaded(
            pos_item_3.x + (img_shop_health_up.getwidth() - textwidth(items[2]->get_name())) / 2,
            pos_item_3.y + img_shop_health_up.getheight()-70 ,
            items[2]->get_name()
        );
    }

    void draw_selection_frame(int x, int y, int width, int height) {
        setlinecolor(RGB(255, 255, 0));  // 淡黄色
        setlinestyle(PS_SOLID, 3);  // 3像素宽的实线
        rectangle(x - 5, y - 5, x + width + 5, y + height + 5);
    }

    void select_next_item() {
        if (!is_chosen) {
            current_selection = (current_selection + 1) % items.size();
        }
    }

    void select_prev_item() {
        if (!is_chosen) {
            current_selection = (current_selection - 1 + items.size()) % items.size();
        }
    }

    void purchase_current_item() {
        if (!is_chosen && current_selection >= 0 && current_selection < items.size()) {
            items[current_selection]->apply_effect();
            
            scene_manager.switch_to(SceneManager::SceneType::Game);
        }
    }

    void outtextxy_shaded(int x, int y, LPCTSTR str) {
        settextcolor(RGB(45, 45, 45));
        outtextxy(x + 1, y + 1, str);
        settextcolor(RGB(255, 255, 255));
        outtextxy(x, y, str);
    }//重载一下

private:
    std::vector<ShopItem*> items;  // 商品列表
    int current_selection;  // 当前选中的商品索引
    bool is_chosen;  // 是否已选择商品

    // 商品位置
    POINT pos_item_1 = { 0 };
    POINT pos_item_2 = { 0 };
    POINT pos_item_3 = { 0 };
};

#endif // !_SHOP_SCENE_H_ 