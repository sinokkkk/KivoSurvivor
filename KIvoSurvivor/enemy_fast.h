#ifndef _ENEMY_FAST_H_
#define _ENEMY_FAST_H_

#include "enemy.h"

extern Atlas atlas_enemy_fast_idle_left;
extern Atlas atlas_enemy_fast_idle_right;
extern Atlas atlas_enemy_fast_run_left;
extern Atlas atlas_enemy_fast_run_right;
extern Atlas atlas_enemy_fast_hurt_left;
extern Atlas atlas_enemy_fast_hurt_right;
extern Atlas atlas_enemy_fast_die_left;
extern Atlas atlas_enemy_fast_die_right;

class FastEnemy : public Enemy
{
public:
    FastEnemy() : Enemy()
    {
        // 设置基础属性
        size.x = 50;
        size.y = 50;
        move_speed = 0.2f;
        hp = 20;
        sensei_damage=20;
        
        // 初始化动画
        animation_idle_left.set_atlas(&atlas_enemy_fast_idle_left);
        animation_idle_right.set_atlas(&atlas_enemy_fast_idle_right);
        animation_run_left.set_atlas(&atlas_enemy_fast_run_left);
        animation_run_right.set_atlas(&atlas_enemy_fast_run_right);
        animation_hurt_left.set_atlas(&atlas_enemy_fast_hurt_left);
        animation_hurt_right.set_atlas(&atlas_enemy_fast_hurt_right);
        animation_die_left.set_atlas(&atlas_enemy_fast_die_left);
        animation_die_right.set_atlas(&atlas_enemy_fast_die_right);

        // 设置动画属性
        animation_idle_left.set_interval(45);
        animation_idle_right.set_interval(45);
        animation_run_left.set_interval(45);
        animation_run_right.set_interval(45);
        animation_hurt_left.set_interval(45);
        animation_hurt_right.set_interval(45);
        animation_die_left.set_interval(45);
        animation_die_right.set_interval(45);

        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);

        // 设置初始动画
        current_animation = &animation_idle_right;
    }

    ~FastEnemy() = default;

protected:
    void on_collide_with_player() override
    {
               // 对玩家造成伤害
        if (!target->is_invulnerable_state()) {  // 使用公共方法检查无敌状态
            target->set_hp(target->get_hp() - sensei_damage);
            target->trigger_invulnerable();  // 使用公共方法触发无敌状态
            mciSendString(_T("play aris_hurt from 0"), NULL, 0, NULL);
        }
        
    }
};

#endif // !_ENEMY_FAST_H_