//
// Created by thisdp 2025/9/25.
// SwitchButton Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

// Flag 1
#define indicatorDimensionUpdated generalFlag1
// Flag 2
#define isIndicatorWidthRelative generalFlag2
// Flag 3
#define isChecked generalFlag3
// Flag 4
#define bgOnFilled generalFlag4
// Flag 5
#define bgOffFilled generalFlag5
// Flag 6
#define indicatorOnFilled generalFlag6
// Flag 7
#define indicatorOffFilled generalFlag7
// Flag 8~9
#define indicatorFrameColorOn generalColor8
// Flag 10~11
#define indicatorFrameColorOff generalColor10
// Flag 12~13
#define indicatorFilledColorOn generalColor12
// Flag 14~15
#define indicatorFilledColorOff generalColor14
// Flag 16~17
#define indicatorColorBase generalColor16
// Flag 18~19
#define bgColorOn generalColor18
// Flag 20~21
#define bgColorOff generalColor20

//切换按钮，兼复选框实现
class EOGSSwitchButton : public EOGSWidget<EOGSSwitchButton> {
protected:
    int16_t paddingTop;        // 上内边距
    int16_t paddingRight;      // 右内边距
    int16_t paddingBottom;     // 下内边距
    int16_t paddingLeft;       // 左内边距
    // 动画相关变量
    EOGSAnimBase indicatorAnimation;        // 指示器位置动画
    float indicatorWidth;         // 指示器宽度（支持相对模式）
    int16_t indicatorAnimOffsetX;                 // 动画X坐标（偏移)
    int16_t bgRadius;             // 背景圆角半径
    int16_t indicatorRadius;      // 指示器圆角半径
    int16_t indicatorRenderX;     // 指示器渲染X坐标
    int16_t indicatorRenderY;     // 指示器渲染Y坐标
    int16_t indicatorRenderW;     // 指示器渲染宽度
    int16_t indicatorRenderH;     // 指示器渲染高度

public:
    // 静态函数
    static void onClick(EOGSEvent* event);  //进入此函数，self一定是EOGSSwitchButton
    
    // 构造函数
    EOGSSwitchButton(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSSwitchButton() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 请求更新指示器尺寸
    void requestIndicatorDimensionUpdate();
    bool updateIndicatorDimension();

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 状态获取和设置
    bool getState() const;
    EOGSSwitchButton* setState(bool checked);

    // 颜色设置
    DrawColor getIndicatorFrameColorOn() const;
    EOGSSwitchButton* setIndicatorFrameColorOn(DrawColor color);
    
    DrawColor getIndicatorFrameColorOff() const;
    EOGSSwitchButton* setIndicatorFrameColorOff(DrawColor color);

    DrawColor getIndicatorFilledColorOn() const;
    EOGSSwitchButton* setIndicatorFilledColorOn(DrawColor color);
    
    DrawColor getIndicatorFilledColorOff() const;
    EOGSSwitchButton* setIndicatorFilledColorOff(DrawColor color);
    
    DrawColor getBgColorOn() const;
    EOGSSwitchButton* setBgColorOn(DrawColor color);
    
    DrawColor getBgColorOff() const;
    EOGSSwitchButton* setBgColorOff(DrawColor color);
    
    // 背景填充设置
    bool getBgOnFilled() const;
    EOGSSwitchButton* setBgOnFilled(bool filled);
    
    bool getBgOffFilled() const;
    EOGSSwitchButton* setBgOffFilled(bool filled);

    // 指示器填充设置
    bool getIndicatorOnFilled() const;
    EOGSSwitchButton* setIndicatorOnFilled(bool filled);

    bool getIndicatorOffFilled() const;
    EOGSSwitchButton* setIndicatorOffFilled(bool filled);
    
    // 内边距设置
    int16_t getPadding() const;
    EOGSSwitchButton* setPadding(int16_t padding);

    int16_t getPaddingTop() const;
    EOGSSwitchButton* setPaddingTop(int16_t padding);

    int16_t getPaddingRight() const;
    EOGSSwitchButton* setPaddingRight(int16_t padding);

    int16_t getPaddingBottom() const;
    EOGSSwitchButton* setPaddingBottom(int16_t padding);

    int16_t getPaddingLeft() const;
    EOGSSwitchButton* setPaddingLeft(int16_t padding);

    EOGSSwitchButton* setPadding(int16_t top, int16_t right, int16_t bottom, int16_t left);
    
    // 圆角半径设置
    int16_t getbgRadius() const;
    EOGSSwitchButton* setBgRadius(int16_t radius);
    
    int16_t getIndicatorRadius() const;
    EOGSSwitchButton* setIndicatorRadius(int16_t radius);
    
    // 指示器宽度设置
    float getIndicatorWidth(bool relative = false) const;
    EOGSSwitchButton* setIndicatorWidth(float width, bool isRelative = false);
    
    bool getIndicatorWidthRelative() const;
    EOGSSwitchButton* setIndicatorWidthRelative(bool isRelative);
};

#undef indicatorDimensionUpdated
#undef isIndicatorWidthRelative
#undef isChecked
#undef bgFilled
#undef indicatorFilled
#undef indicatorColorOn
#undef indicatorColorOff
#undef bgColorOn
#undef bgColorOff