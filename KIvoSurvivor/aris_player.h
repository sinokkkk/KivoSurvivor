#ifndef _ARIS_PLAYER_
#define _ARIS_PLAYER_

#include "player.h"
#include "timer.h"
#include "camera.h"
#include "bullet_aris.h"
#include "easyx.h"
// 资源文件
extern Atlas atlas_aris_idle_left;
extern Atlas atlas_aris_idle_right;
extern Atlas atlas_aris_run_left;
extern Atlas atlas_aris_run_right;
extern Atlas atlas_aris_attack_ex_left;
extern Atlas atlas_aris_attack_ex_right;
extern Atlas atlas_aris_die_left;
extern Atlas atlas_aris_die_right;

extern Camera main_camera;

class ArisPlayer : public Player
{
public:
    ArisPlayer(bool facing_right = true) : Player(facing_right){
        // 绑定动画图集
        animation_idle_left.set_atlas(&atlas_aris_idle_left);
        animation_idle_right.set_atlas(&atlas_aris_idle_right);
        animation_run_left.set_atlas(&atlas_aris_run_left);
        animation_run_right.set_atlas(&atlas_aris_run_right);
        animation_attack_ex_left.set_atlas(&atlas_aris_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_aris_attack_ex_right);
        animation_die_left.set_atlas(&atlas_aris_die_left);
        animation_die_right.set_atlas(&atlas_aris_die_right);

        // 设置动画帧间隔
        animation_idle_left.set_interval(75);
        animation_idle_right.set_interval(75);
        animation_run_left.set_interval(75);
        animation_run_right.set_interval(75);
        animation_attack_ex_left.set_interval(75);
        animation_attack_ex_right.set_interval(75);
        animation_die_left.set_interval(150);
        animation_die_right.set_interval(150);
        
        //设置循环类型，默认为true
        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);

        // 技能冷却计时器
        timer_attack_ex.set_wait_time(attack_ex_duration);
        timer_attack_ex.set_one_shot(true);
        timer_attack_ex.set_callback([&]()
            {
                is_attacking_ex = false;
            });

        timer_spawn_bullet_ex.set_wait_time(100);
        timer_spawn_bullet_ex.set_callback([&]()
            {
                spawn_aris_bullet(speed_aris_ex);
            });
        // 属于Aris的计时器或特效

        // 设置角色大小
        size.x = 70, size.y = 70;

        // 设置攻击冷却
        attack_cd = 120; // Aris攻击速度略慢
        
    }

    ~ArisPlayer() = default;


    // 普通攻击
    void on_attack() override{

        spawn_aris_bullet(speed_aris);
        // 播放发射音效(待定)
        //mciSendString(_T("play aris_shoot from 0"), NULL, 0, NULL);
    }


    // EX技能攻击
    void on_attack_ex() override {
        is_attacking_ex = true;
        timer_attack_ex.restart();

        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        // 触发EX技能音效
        mciSendString(_T("play aris_ex from 0"), NULL, 0, NULL);
        
    }

    void on_update(int delta) override {
        Player::on_update(delta);//调用基类方法

        if (is_attacking_ex){
            main_camera.shake(4, 200); // 震屏效果
            timer_attack_ex.on_update(delta);
            timer_spawn_bullet_ex.on_update(delta);
            // 可以在这里随便添加ex期间的特殊逻辑
        }
    }

private:
    

    const float speed_aris = 0.9f;//子弹速度
    const float speed_aris_ex = 3.0f;//ex子弹
	const int attack_ex_duration = 2000; // EX技能持续时间

    Timer timer_attack_ex;
    Timer timer_spawn_bullet_ex;

    void spawn_aris_bullet(float speed){
        Bullet* bullet = new ArisBullet(this); // 传入this作为owner

        const Vector2& bullet_size = bullet->get_size();
        Vector2 bullet_position;
        bullet_position.x = position.x + size.x / 2 - bullet_size.x / 2; // 中心对齐
        bullet_position.y = position.y + size.y / 2 - bullet_size.y / 2;
    


        // 计算鼠标位置相对于玩家的方向
        MOUSEMSG msg = GetMouseMsg();
        Vector2 mouse_pos(msg.x, msg.y);
        Vector2 direction = mouse_pos - position;


        float magnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (magnitude > 0) {
            direction.x /= magnitude;
            direction.y /= magnitude;
        }
        else {
            direction = Vector2(1, 0); // 默认向右
        }

        Vector2 bullet_velocity;
        bullet_velocity.x = direction.x * speed;
        bullet_velocity.y = direction.y * speed;
        
        // 设置子弹位置（从玩家位置发射）  子弹方向  添加到子弹列表
        bullet->set_position(bullet_position.x, bullet_position.y);
        bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);
        bullet_list.push_back(bullet);
    }
};

#endif // !_ARIS_PLAYER_