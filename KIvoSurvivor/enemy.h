#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "timer.h"
#include "bullet.h"
#include "vector2.h"
#include "player.h"

#include <vector>
#include <graphics.h>

extern bool is_debug;
extern std::vector<Bullet*> bullet_list;

class Enemy
{
public:
    Enemy(bool facing_right = true) : is_facing_right(facing_right)
    {
        // 设置朝向
        current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;

        // 设置受伤闪烁计时器
        timer_hurt_blink.set_wait_time(75);
        timer_hurt_blink.set_one_shot(true);
        timer_hurt_blink.set_callback([&]()
            {
                is_showing_sketch_frame = false;
            });
    }

    ~Enemy() = default;

    virtual void on_update(int delta){
        // 更新动画
        current_animation->on_update(delta);

        // 更新计时器
        timer_hurt_blink.on_update(delta);

        // 直接用util的函数 受伤闪烁效果
        // 养兵千日用兵一时啊
        if (is_showing_sketch_frame)
            sketch_image(current_animation->get_frame(), &img_sketch);

        // 处理移动和碰撞
        move_and_collide(delta);
    }

    virtual void on_draw(const Camera& camera){
        // 绘制敌人
        if (is_showing_sketch_frame)
            putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
        else
            current_animation->on_draw(camera, (int)position.x, (int)position.y);

        // 调试模式
        if (is_debug)
        {
            setlinecolor(RGB(255, 0, 0));
            rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
        }
    }

    
    
    // 设置
    void set_target(Player* target) { this->target = target; }
    void set_position(float x, float y) { position.x = x; position.y = y; }

    // 获取敌人状态一系列函数
    bool is_alive() const { return hp > 0; }
    int get_hp() const { return hp; }
    const Vector2& get_position() const { return position; }
    const Vector2& get_size() const { return size; }

protected:
    // 基础属性
    Vector2 size;
    Vector2 position;
    Vector2 velocity;
    Vector2 last_hurt_direction;
    int hp = 100;
    float move_speed = 0.15f;  // 移动速度，随便改

    // 状态标志
    bool is_facing_right = true;
    bool is_showing_sketch_frame = false;

    // 动画相关
    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_run_left;
    Animation animation_run_right;
    Animation animation_hurt_left;
    Animation animation_hurt_right;
    Animation animation_die_left;
    Animation animation_die_right;
    Animation* current_animation = nullptr;

    // 计时器
    Timer timer_hurt_blink;

    // 目标玩家
    Player* target = nullptr;

    // 贴图
    IMAGE img_sketch;

protected:
    virtual void move_and_collide(int delta)
    {
        if (!target) return;

        // 计算到玩家的方向
        Vector2 dir = target->get_postion() - position;
        float len = dir.length();
        if (len > 0)    {dir.x /= len;  dir.y /= len;}

        // 更新朝向
        is_facing_right = dir.x > 0;
        current_animation = is_facing_right ? &animation_run_right : &animation_run_left;

        // 平滑移动
        position.x += dir.x * move_speed * delta;
        position.y += dir.y * move_speed * delta;

        // 边界检测
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + size.x > getwidth()) position.x = getwidth() - size.x;
        if (position.y + size.y > getheight()) position.y = getheight() - size.y;

        // 检查与玩家的碰撞
        if (check_collision(target->get_postion(), target->get_size())){
            on_collide_with_player();
        }

        // 检查子弹碰撞
        for (Bullet* bullet : bullet_list)
        {
            if (!bullet->get_valid() || bullet->get_owner() == this) continue;
            
            if (check_collision(bullet->get_position(), bullet->get_size()))
            {
                target->set_mp(target->get_mp() + 10);
                on_hurt(bullet->get_damage());
                bullet->on_collide();
                bullet->set_valid(false);
                last_hurt_direction = bullet->get_position() - position;
            }
        }
    }

    bool check_collision(const Vector2& target_pos, const Vector2& target_size)
    {
        return (position.x < target_pos.x + target_size.x &&
                position.x + size.x > target_pos.x &&
                position.y < target_pos.y + target_size.y &&
                position.y + size.y > target_pos.y);
    }

    virtual void on_collide_with_player(){
        // 对玩家造成伤害
        target->set_hp(target->get_hp() - 10);
    }

    virtual void on_hurt(int damage)
    {
        hp -= damage;
        is_showing_sketch_frame = true;
        timer_hurt_blink.restart();

        if (hp <= 0){
            current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;
        }
        else{
            current_animation = last_hurt_direction.x < 0 ? &animation_hurt_left : &animation_hurt_right;
        }
    }
};


#endif // !_ENEMY_H_
