//
// Created by Roo 2025/10/3.
// Checkbox Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

// Flag 1
#define isChecked generalFlag1
// Flag 2
#define checkMarkFilled generalFlag2
// Flag 3
#define boxFilled generalFlag3
// Flag 4~5
#define boxFrameColor generalColor4
// Flag 6~7
#define boxFillColor generalColor6
// Flag 8~9
#define checkMarkColor generalColor8
// Flag 10~11
#define checkMarkBaseColor generalColor10

class EOGSCheckbox : public EOGSWidget<EOGSCheckbox> {
protected:
    int16_t paddingTop;        // 上内边距
    int16_t paddingRight;      // 右内边距
    int16_t paddingBottom;     // 下内边距
    int16_t paddingLeft;       // 左内边距
    int16_t boxRadius;         // 方框圆角半径
    int16_t checkMarkSize;     // 勾选标记大小
    int16_t checkMarkThickness; // 勾选标记粗细

public:
    // 静态函数
    static void onClick(EOGSEvent* event){
        EOGSCheckbox *checkbox = static_cast<EOGSCheckbox*>(event->self);
        checkbox->setChecked(!checkbox->getChecked());
    }
    
    // 构造函数
    EOGSCheckbox(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSWidget<EOGSCheckbox>(_x, _y, _w, _h, _isRelative),
          paddingTop(2), paddingRight(2), paddingBottom(2), paddingLeft(2),
          boxRadius(3),           // 默认有轻微圆角
          checkMarkSize(0),       // 默认自动计算
          checkMarkThickness(2) { // 默认勾线粗细
        // 初始化颜色属性
        setBoxFrameColor(DrawColor::WHITE);
        setBoxFillColor(DrawColor::WHITE);
        setCheckMarkColor(DrawColor::WHITE);
        setCheckMarkBaseColor(DrawColor::BLACK);
        setChecked(false);
        setBoxFilled(false);
        setCheckMarkFilled(true);
        on(EOGSEventID::Click, onClick);
    }

    ~EOGSCheckbox() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false;
        renderPosUpdated = true;

        if (eogs == nullptr) return false;
        renderX = parentRX + x;
        renderY = parentRY + y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        return true;
    }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        
        updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);

        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            // 计算方框和勾选标记的尺寸
            int16_t boxSize = min(w, h) - max(paddingTop + paddingBottom, paddingLeft + paddingRight);
            int16_t boxX = renderX + (w - boxSize) / 2;
            int16_t boxY = renderY + (h - boxSize) / 2;
            
            // 如果未设置勾选标记大小，则自动计算
            int16_t markSize = checkMarkSize > 0 ? checkMarkSize : boxSize * 0.7;
            int16_t markThickness = checkMarkThickness;
            
            // 绘制方框
            eogs->setDrawColor(boxFrameColor);
            if (boxFilled) {
                // 填充方框
                if (boxRadius > 0) {
                    eogs->drawRBox(boxX, boxY, boxSize, boxSize, boxRadius);
                } else {
                    eogs->drawBox(boxX, boxY, boxSize, boxSize);
                }
            } else {
                // 绘制方框边框
                if (boxRadius > 0) {
                    eogs->drawRFrame(boxX, boxY, boxSize, boxSize, boxRadius);
                } else {
                    eogs->drawFrame(boxX, boxY, boxSize, boxSize);
                }
            }
            
            // 如果选中，则绘制勾选标记
            if (isChecked) {
                eogs->setDrawColor(checkMarkColor);
                int16_t markX = boxX + (boxSize - markSize) / 2;
                int16_t markY = boxY + (boxSize - markSize) / 2;
                
                // 绘制勾选标记（简化版的对勾）
                int16_t startX = markX;
                int16_t startY = markY + markSize / 2;
                int16_t midX = markX + markSize / 3;
                int16_t midY = markY + markSize * 2 / 3;
                int16_t endX = markX + markSize;
                int16_t endY = markY;
                
                if (checkMarkFilled) {
                    // 填充模式的勾选标记
                    eogs->drawThickLine(startX, startY, midX, midY, markThickness);
                    eogs->drawThickLine(midX, midY, endX, endY, markThickness);
                } else {
                    // 边框模式的勾选标记（使用基础颜色）
                    eogs->setDrawColor(checkMarkBaseColor);
                    eogs->drawThickLine(startX, startY, midX, midY, markThickness);
                    eogs->drawThickLine(midX, midY, endX, endY, markThickness);
                }
            }
        }
    }

    // 状态获取和设置
    bool getChecked() const { return isChecked; }
    EOGSCheckbox* setChecked(bool checked) {
        if (isChecked != checked) {
            isChecked = checked;
        }
        return this;
    }

    // 颜色设置
    DrawColor getBoxFrameColor() const { return boxFrameColor; }
    EOGSCheckbox* setBoxFrameColor(DrawColor color) {
        boxFrameColor = color;
        return this;
    }
    
    DrawColor getBoxFillColor() const { return boxFillColor; }
    EOGSCheckbox* setBoxFillColor(DrawColor color) {
        boxFillColor = color;
        return this;
    }
    
    DrawColor getCheckMarkColor() const { return checkMarkColor; }
    EOGSCheckbox* setCheckMarkColor(DrawColor color) {
        checkMarkColor = color;
        return this;
    }
    
    DrawColor getCheckMarkBaseColor() const { return checkMarkBaseColor; }
    EOGSCheckbox* setCheckMarkBaseColor(DrawColor color) {
        checkMarkBaseColor = color;
        return this;
    }
    
    // 填充设置
    bool getBoxFilled() const { return boxFilled; }
    EOGSCheckbox* setBoxFilled(bool filled) {
        boxFilled = filled;
        return this;
    }
    
    bool getCheckMarkFilled() const { return checkMarkFilled; }
    EOGSCheckbox* setCheckMarkFilled(bool filled) {
        checkMarkFilled = filled;
        return this;
    }
    
    // 内边距设置
    int16_t getPadding() const { return paddingTop; }
    EOGSCheckbox* setPadding(int16_t padding) {
        if (paddingTop != padding || paddingRight != padding || paddingBottom != padding || paddingLeft != padding) {
            paddingTop = padding;
            paddingRight = padding;
            paddingBottom = padding;
            paddingLeft = padding;
        }
        return this;
    }

    int16_t getPaddingTop() const { return paddingTop; }
    EOGSCheckbox* setPaddingTop(int16_t padding) {
        if (paddingTop != padding) {
            paddingTop = padding;
        }
        return this;
    }

    int16_t getPaddingRight() const { return paddingRight; }
    EOGSCheckbox* setPaddingRight(int16_t padding) {
        if (paddingRight != padding) {
            paddingRight = padding;
        }
        return this;
    }

    int16_t getPaddingBottom() const { return paddingBottom; }
    EOGSCheckbox* setPaddingBottom(int16_t padding) {
        if (paddingBottom != padding) {
            paddingBottom = padding;
        }
        return this;
    }

    int16_t getPaddingLeft() const { return paddingLeft; }
    EOGSCheckbox* setPaddingLeft(int16_t padding) {
        if (paddingLeft != padding) {
            paddingLeft = padding;
        }
        return this;
    }

    EOGSCheckbox* setPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
        if (paddingTop != top || paddingRight != right || paddingBottom != bottom || paddingLeft != left) {
            paddingTop = top;
            paddingRight = right;
            paddingBottom = bottom;
            paddingLeft = left;
        }
        return this;
    }
    
    // 圆角半径设置
    int16_t getBoxRadius() const { return boxRadius; }
    EOGSCheckbox* setBoxRadius(int16_t radius) {
        boxRadius = radius;
        return this;
    }
    
    // 勾选标记大小设置
    int16_t getCheckMarkSize() const { return checkMarkSize; }
    EOGSCheckbox* setCheckMarkSize(int16_t size) {
        checkMarkSize = size;
        return this;
    }
    
    // 勾选标记粗细设置
    int16_t getCheckMarkThickness() const { return checkMarkThickness; }
    EOGSCheckbox* setCheckMarkThickness(int16_t thickness) {
        checkMarkThickness = thickness;
        return this;
    }
};

#undef isChecked
#undef checkMarkFilled
#undef boxFilled
#undef boxFrameColor
#undef boxFillColor
#undef checkMarkColor
#undef checkMarkBaseColor