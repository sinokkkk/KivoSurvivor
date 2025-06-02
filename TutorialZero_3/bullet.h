#ifndef _BULLET_H_
#define _BULLET_H_

#include "camera.h"
#include "vector2.h"
#include "animation.h"

#include <functional>

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	// 基础属性设置
	void set_damage(int val)
	{
		damage = val;
	}

	int get_damage() const
	{
		return damage;
	}

	void set_position(float x, float y)
	{
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_velocity(float x, float y)
	{
		velocity.x = x, velocity.y = y;
	}

	void set_valid(bool flag)
	{
		valid = flag;
	}

	bool get_valid() const
	{
		return valid;
	}

	bool check_can_remove() const
	{
		return can_remove;
	}

	// 设置子弹方向（朝向鼠标位置）
	virtual void set_direction(const Vector2& dir)
	{
		direction = dir;
		direction.normalize();
		velocity = direction * speed;
	}

	// 碰撞检测（使用圆形碰撞）
	virtual bool check_collision(const Vector2& target_pos, const Vector2& target_size)
	{
		Vector2 bullet_center = position + size * 0.5f;
		Vector2 target_center = target_pos + target_size * 0.5f;
		
		float distance = (bullet_center - target_center).length();
		float radius = max(size.x, size.y) * 0.5f;
		float target_radius = max(target_size.x, target_size.y) * 0.5f;
		
		return distance <= (radius + target_radius);
	}

	// 生命周期方法
	virtual void on_update(int delta)
	{
		if (!valid) return;
		
		// 基础移动
		position += velocity * (float)delta;
		
		// 更新动画
		if (animation)
			animation->on_update(delta);
		
		// 检查是否超出屏幕
		if (check_if_exceeds_screen())
			can_remove = true;
	}

	virtual void on_draw(const Camera& camera) const
	{
		if (!valid) return;

		// 绘制子弹贴图
		if (animation)
			animation->on_draw(camera, (int)position.x, (int)position.y);
		else if (image)
			putimage_alpha(camera, (int)position.x, (int)position.y, image);

		// 调试模式绘制
		if (is_debug)
		{
			// 绘制碰撞范围（圆形）
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			float radius = max(size.x, size.y) * 0.5f;
			solidcircle(
				(int)(position.x + size.x/2),
				(int)(position.y + size.y/2),
				(int)radius
			);
		}
	}

	// 碰撞处理
	virtual void on_collide()
	{
		if (callback)
			callback();
	}

protected:
	Vector2 size;          // 子弹大小
	Vector2 position;      // 位置
	Vector2 velocity;      // 速度
	Vector2 direction;     // 方向
	float speed = 0.8f;    // 基础速度
	int damage = 10;       // 基础伤害
	bool valid = true;     // 是否有效
	bool can_remove = false; // 是否可以移除
	std::function<void()> callback; // 碰撞回调

	// 贴图相关
	IMAGE* image = nullptr;        // 静态贴图
	Animation* animation = nullptr; // 动画贴图

protected:
	bool check_if_exceeds_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};

#endif // !_BULLET_H_