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
#define bgFilled generalFlag4
// Flag 5
#define indicatorFilled generalFlag5
// Flag 6~7
#define indicatorColorOn generalColor6
// Flag 8~9
#define indicatorColorOff generalColor8
// Flag 10~11
#define bgColorOn generalColor10
// Flag 12~13
#define bgColorOff generalColor12

//切换按钮，兼复选框实现
class EOGSSwitchButton : public EOGSWidget<EOGSSwitchButton> {
protected:
    int16_t switchPadding;        // 开关内边距
    float indicatorWidth;         // 指示器宽度（支持相对模式）
    int16_t bgRadius;             // 背景圆角半径
    int16_t indicatorRadius;      // 指示器圆角半径
    int16_t indicatorRenderX;     // 指示器渲染X坐标
    int16_t indicatorRenderY;     // 指示器渲染Y坐标
    int16_t indicatorRenderW;     // 指示器渲染宽度
    int16_t indicatorRenderH;     // 指示器渲染高度

public:
    // 构造函数
    EOGSSwitchButton(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSWidget<EOGSSwitchButton>(_x, _y, _w, _h, _isRelative),
          switchPadding(2),
          indicatorWidth(0.5f),  // 默认为开关宽度的一半
          bgRadius(0),           // 默认无圆角
          indicatorRadius(0),    // 默认无圆角
          indicatorRenderX(0),
          indicatorRenderY(0),
          indicatorRenderW(0),
          indicatorRenderH(0) {
        // 初始化颜色属性
        setIndicatorColorOn(DrawColor::WHITE);
        setIndicatorColorOff(DrawColor::BLACK);
        setBgColorOn(DrawColor::WHITE);
        setBgColorOff(DrawColor::WHITE);
        setChecked(false);  // 默认关闭状态
        setIndicatorWidthRelative(true);  // 默认使用绝对模式
        setBgFilled(true);  // 默认填充背景
        setIndicatorFilled(true);  // 默认填充指示器
    }

    ~EOGSSwitchButton() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false;
        renderPosUpdated = true;

        if (eogs == nullptr) return false;
        renderX = parentRX + x;
        renderY = parentRY + y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        requestIndicatorDimensionUpdate();  // 如果更新了位置，则需要更新指示器尺寸
        return true;
    }

    // 请求更新指示器尺寸
    void requestIndicatorDimensionUpdate() { indicatorDimensionUpdated = false; }
    bool updateIndicatorDimension() {
        if (indicatorDimensionUpdated) return false;
        indicatorDimensionUpdated = true;
        
        // 计算指示器宽度
        if (isIndicatorWidthRelative) {
            // 使用相对模式，indicatorWidth是相对于开关宽度的比例
            indicatorRenderW = static_cast<int16_t>((w - 2 * switchPadding) * indicatorWidth);
        } else {
            // 使用绝对模式，indicatorWidth是实际像素值
            indicatorRenderW = static_cast<int16_t>(indicatorWidth);
        }
        
        // 确保指示器宽度不会超过开关宽度减去内边距 且 确保指示器宽度至少为1
        indicatorRenderW = mathClamp(indicatorRenderW, 1, w - 2 * switchPadding);
        // 计算指示器高度
        indicatorRenderH = h - 2 * switchPadding;
        
        // 计算指示器位置
        indicatorRenderX = isChecked ?
            (renderX + w - indicatorRenderW - switchPadding) :
            (renderX + switchPadding);
        indicatorRenderY = renderY + switchPadding;
        
        return true;
    }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateIndicatorDimension();

        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            // 绘制开关背景
            eogs->setDrawColor(isChecked ? bgColorOn : bgColorOff);
            if (bgFilled) {
                // 填充背景
                if (bgRadius > 0) {
                    // 使用圆角绘制填充背景
                    eogs->drawRBox(renderX, renderY, w, h, bgRadius);
                } else {
                    // 使用普通矩形绘制填充背景
                    eogs->drawBox(renderX, renderY, w, h);
                }
            } else {
                // 绘制背景边框
                if (bgRadius > 0) {
                    // 使用圆角绘制背景边框
                    eogs->drawRFrame(renderX, renderY, w, h, bgRadius);
                } else {
                    // 使用普通矩形绘制背景边框
                    eogs->drawFrame(renderX, renderY, w, h);
                }
            }
            
            // 绘制开关指示器
            eogs->setDrawColor(isChecked ? indicatorColorOn : indicatorColorOff);
            if (indicatorFilled) {
                // 填充指示器
                if (indicatorRadius > 0) {
                    // 使用圆角绘制填充指示器
                    eogs->drawRBox(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH, indicatorRadius);
                } else {
                    // 使用普通矩形绘制填充指示器
                    eogs->drawBox(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH);
                }
            } else {
                // 绘制指示器边框
                if (indicatorRadius > 0) {
                    // 使用圆角绘制指示器边框
                    eogs->drawRFrame(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH, indicatorRadius);
                } else {
                    // 使用普通矩形绘制指示器边框
                    eogs->drawFrame(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH);
                }
            }
        }
    }


    // 状态获取和设置
    bool getChecked() const { return isChecked; }
    EOGSSwitchButton* setChecked(bool checked) {
        if (isChecked != checked) {
            isChecked = checked;
            indicatorDimensionUpdated = false; // 状态改变时需要更新指示器位置
        }
        return this;
    }

    // 颜色设置
    DrawColor getIndicatorColorOn() const { return indicatorColorOn; }
    EOGSSwitchButton* setIndicatorColorOn(DrawColor color) {
        indicatorColorOn = color;
        return this;
    }
    
    DrawColor getIndicatorColorOff() const { return indicatorColorOff; }
    EOGSSwitchButton* setIndicatorColorOff(DrawColor color) {
        indicatorColorOff = color;
        return this;
    }
    
    DrawColor getBgColorOn() const { return bgColorOn; }
    EOGSSwitchButton* setBgColorOn(DrawColor color) {
        bgColorOn = color;
        return this;
    }
    
    DrawColor getBgColorOff() const { return bgColorOff; }
    EOGSSwitchButton* setBgColorOff(DrawColor color) {
        bgColorOff = color;
        return this;
    }
    
    // 背景填充设置
    bool getBgFilled() const { return bgFilled; }
    EOGSSwitchButton* setBgFilled(bool filled) {
        bgFilled = filled;
        return this;
    }
    
    // 指示器填充设置
    bool getIndicatorFilled() const { return indicatorFilled; }
    EOGSSwitchButton* setIndicatorFilled(bool filled) {
        indicatorFilled = filled;
        return this;
    }
    
    // 内边距设置
    int16_t getSwitchPadding() const { return switchPadding; }
    EOGSSwitchButton* setSwitchPadding(int16_t padding) {
        if (switchPadding != padding) {
            switchPadding = padding;
            indicatorDimensionUpdated = false; // 内边距改变时需要更新指示器位置
        }
        return this;
    }
    
    // 圆角半径设置
    int16_t getbgRadius() const { return bgRadius; }
    EOGSSwitchButton* setbgRadius(int16_t radius) {
        bgRadius = radius;
        return this;
    }
    
    int16_t getIndicatorRadius() const { return indicatorRadius; }
    EOGSSwitchButton* setIndicatorRadius(int16_t radius) {
        indicatorRadius = radius;
        return this;
    }
    
    // 指示器宽度设置
    float getIndicatorWidth(bool relative = false) const { 
        return relative ? indicatorWidth : (isIndicatorWidthRelative ? (indicatorWidth * w) : indicatorWidth); 
    }
    EOGSSwitchButton* setIndicatorWidth(float width, bool isRelative = false) {
        if (indicatorWidth != width || isIndicatorWidthRelative != isRelative) {
            indicatorWidth = width;
            setIndicatorWidthRelative(isRelative);
            requestIndicatorDimensionUpdate();
        }
        return this;
    }
    
    bool getIndicatorWidthRelative() const { return isIndicatorWidthRelative; }
    EOGSSwitchButton* setIndicatorWidthRelative(bool isRelative) {
        if (isIndicatorWidthRelative != isRelative) {
            isIndicatorWidthRelative = isRelative;
            requestIndicatorDimensionUpdate();
        }
        return this;
    }
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