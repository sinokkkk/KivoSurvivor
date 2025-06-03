#ifndef _ENEMY_SLOW_H_
#define _ENEMY_SLOW_H_

#include "enemy.h"

extern Atlas atlas_enemy_slow_idle_left;
extern Atlas atlas_enemy_slow_idle_right;
extern Atlas atlas_enemy_slow_run_left;
extern Atlas atlas_enemy_slow_run_right;
extern Atlas atlas_enemy_slow_hurt_left;
extern Atlas atlas_enemy_slow_hurt_right;
extern Atlas atlas_enemy_slow_die_left;
extern Atlas atlas_enemy_slow_die_right;

class SlowEnemy : public Enemy
{
public:
    SlowEnemy() : Enemy()
    {
        // 设置基础属性
        size.x = 60;
        size.y = 60;
        move_speed = 0.1f;
        hp = 10;

        // 初始化动画
        animation_idle_left.set_atlas(&atlas_enemy_slow_idle_left);
        animation_idle_right.set_atlas(&atlas_enemy_slow_idle_right);
        animation_run_left.set_atlas(&atlas_enemy_slow_run_left);
        animation_run_right.set_atlas(&atlas_enemy_slow_run_right);
        animation_hurt_left.set_atlas(&atlas_enemy_slow_hurt_left);
        animation_hurt_right.set_atlas(&atlas_enemy_slow_hurt_right);
        animation_die_left.set_atlas(&atlas_enemy_slow_die_left);
        animation_die_right.set_atlas(&atlas_enemy_slow_die_right);

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

    ~SlowEnemy() = default;

protected:
    void on_collide_with_player() override
    {
        // 慢速敌人造成更多伤害
        target->set_hp(target->get_hp() - 20);
    }
};

#endif // !_ENEMY_SLOW_H_
