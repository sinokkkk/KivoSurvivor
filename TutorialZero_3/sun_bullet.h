#ifndef _SUN_BULLET_H_
#define _SUN_BULLET_H_

// 引入必要的头文件
#include "bullet.h"  // 子弹基类
#include "camera.h"  // 相机类

// 声明外部资源
extern Atlas atlas_sun;           // 太阳子弹的贴图集
extern Atlas atlas_sun_explode;   // 太阳子弹爆炸的贴图集
extern Camera main_camera;        // 主相机对象

// 太阳子弹类：继承自Bullet基类，实现了一个具有物理效果的太阳子弹
class SunBullet : public Bullet
{
public:
	// 构造函数：初始化子弹的基本属性和动画
	SunBullet()
	{
		// 设置子弹的尺寸
		size.x = 96, size.y = 96;

		// 设置子弹的伤害值
		damage = 20;

		// 初始化普通状态的动画
		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);  // 设置动画帧间隔为50毫秒

		// 初始化爆炸状态的动画
		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);  // 爆炸动画不循环播放
		// 设置爆炸动画结束时的回调函数，标记子弹可被移除
		animation_explode.set_callback([&]() { can_remove = true; });

		// 计算爆炸动画的渲染偏移量，确保爆炸效果居中显示
		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}

	// 析构函数
	~SunBullet() = default;

	// 碰撞处理函数：处理子弹碰撞时的效果
	void on_collide()
	{
		// 调用基类的碰撞处理
		Bullet::on_collide();

		// 触发相机抖动效果，增强打击感
		main_camera.shake(5, 250);

		// 播放爆炸音效
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	// 更新函数：处理子弹的物理运动和动画状态
	void on_update(int delta)
	{
		if (valid)
		{
			// 应用重力效果，使子弹呈抛物线运动
			velocity.y += gravity * delta;
			// 更新子弹位置
			position += velocity * (float)delta;
		}
		
		// 根据子弹状态更新相应的动画
		if (!valid)
			animation_explode.on_update(delta);  // 更新爆炸动画
		else
			animation_idle.on_update(delta);     // 更新普通状态动画

		// 检查子弹是否超出屏幕边界
		if (check_if_exceeds_screen())
			can_remove = true;
	}

	// 绘制函数：根据子弹状态绘制相应的动画
	void on_draw(const Camera& camera) const
	{
		if (valid)
			// 绘制普通状态的动画
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		else
		{
			// 绘制爆炸状态的动画，考虑偏移量确保居中显示
			animation_explode.on_draw(camera, (int)(position.x + explode_render_offset.x), 
				(int)(position.y + explode_render_offset.y));
		}

		// 调用基类的绘制函数
		Bullet::on_draw(camera);
	}

private:
	const float gravity = 1e-3f;  // 重力加速度常量，控制子弹下落速度

private:
	Animation animation_idle;     // 普通状态的动画
	Animation animation_explode;  // 爆炸状态的动画
	Vector2 explode_render_offset; // 爆炸动画的渲染偏移量，用于居中显示
};

#endif // !_SUN_BULLET_H_
