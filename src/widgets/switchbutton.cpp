//
// Created by thisdp 2025/9/25.
// SwitchButton Widget for EOGS
//

#include "switchbutton.h"
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

// 静态函数
void EOGSSwitchButton::onClick(EOGSEvent* event){  //进入此函数，self一定是EOGSSwitchButton
    EOGSSwitchButton *switchButton = static_cast<EOGSSwitchButton*>(event->self);
    switchButton->setState(!switchButton->getState());
}

// 构造函数
EOGSSwitchButton::EOGSSwitchButton(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSWidget<EOGSSwitchButton>(_x, _y, _w, _h, _isRelative),
      paddingTop(2), paddingRight(2), paddingBottom(2), paddingLeft(2),
      indicatorAnimation(250, EOGSBuiltInEasing::OutSine()),
      indicatorWidth(0.5f),  // 默认为开关宽度的一半
      indicatorAnimOffsetX(0),
      bgRadius(0),           // 默认无圆角
      indicatorRadius(0),    // 默认无圆角
      indicatorRenderX(0),
      indicatorRenderY(0),
      indicatorRenderW(0),
      indicatorRenderH(0){
    // 初始化颜色属性
    setIndicatorFilledColorOn(DrawColor::BLACK);
    setIndicatorFilledColorOff(DrawColor::BLACK);
    setIndicatorFrameColorOn(DrawColor::WHITE);
    setIndicatorFrameColorOff(DrawColor::WHITE);
    setBgColorOn(DrawColor::WHITE);
    setBgColorOff(DrawColor::WHITE);
    setState(false);
    setIndicatorWidthRelative(true);
    setBgOnFilled(true);
    setBgOffFilled(false);
    setIndicatorOnFilled(false);
    setIndicatorOffFilled(false);
    on(EOGSEventID::Click, onClick);
}

// 更新控件尺寸和位置
bool EOGSSwitchButton::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
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
void EOGSSwitchButton::requestIndicatorDimensionUpdate() { indicatorDimensionUpdated = false; }
bool EOGSSwitchButton::updateIndicatorDimension() {
    if (indicatorDimensionUpdated) return false;
    indicatorDimensionUpdated = true;
    
    // 计算指示器宽度
    if (isIndicatorWidthRelative) {
        // 使用相对模式，indicatorWidth是相对于开关宽度的比例
        indicatorRenderW = static_cast<int16_t>((w - paddingLeft - paddingRight) * indicatorWidth);
    } else {
        // 使用绝对模式，indicatorWidth是实际像素值
        indicatorRenderW = static_cast<int16_t>(indicatorWidth);
    }
    
    // 确保指示器宽度不会超过开关宽度减去内边距 且 确保指示器宽度至少为1
    indicatorRenderW = mathClamp(indicatorRenderW, 1, w - paddingLeft - paddingRight);
    // 计算指示器高度
    indicatorRenderH = h - paddingTop - paddingBottom;
    
    // 计算指示器位置
    // 动画模式下，indicatorRenderX 由 render() 中的动画插值计算

    indicatorRenderX = renderX + paddingLeft+indicatorAnimOffsetX;
    indicatorRenderY = renderY + paddingTop;
    
    return true;
}

void EOGSSwitchButton::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    if(updateDimension(parentW, parentH)) requestIndicatorDimensionUpdate();
    updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    if (indicatorAnimation.isRunning()) {
        float progress = indicatorAnimation.getProgress();
        int16_t startPos = 0;
        int16_t endPos = 0;
        if(isChecked){
            endPos = w - indicatorRenderW - paddingRight - paddingLeft;
        }else{
            startPos = w - indicatorRenderW - paddingRight - paddingLeft;
        }
        indicatorAnimOffsetX = startPos + progress * (endPos - startPos);
        requestIndicatorDimensionUpdate();
    }
    indicatorAnimation.update(eogs);
    updateIndicatorDimension();


    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        // 绘制开关背景
        eogs->setDrawColor(isChecked ? bgColorOn : bgColorOff);
        if (isChecked ? bgOnFilled : bgOffFilled) {
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
        
        if (isChecked ? indicatorOnFilled : indicatorOffFilled) {
            // 填充指示器
            eogs->setDrawColor(isChecked ? indicatorFilledColorOn : indicatorFilledColorOff);
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
                eogs->setDrawColor(isChecked ? indicatorFilledColorOn : indicatorFilledColorOff);
                eogs->drawRBox(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH, indicatorRadius);
                eogs->setDrawColor(isChecked ? indicatorFrameColorOn : indicatorFrameColorOff);
                eogs->drawRFrame(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH, indicatorRadius);
            } else {
                // 使用普通矩形绘制指示器边框
                eogs->setDrawColor(isChecked ? indicatorFilledColorOn : indicatorFilledColorOff);
                eogs->drawBox(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH);
                eogs->setDrawColor(isChecked ? indicatorFrameColorOn : indicatorFrameColorOff);
                eogs->drawFrame(indicatorRenderX, indicatorRenderY, indicatorRenderW, indicatorRenderH);
            }
        }
    }
}

// 状态获取和设置
bool EOGSSwitchButton::getState() const { return isChecked; }
EOGSSwitchButton* EOGSSwitchButton::setState(bool checked) {
    if (isChecked != checked) {
        isChecked = checked;
        indicatorAnimation.start();
    }
    return this;
}

// 颜色设置
DrawColor EOGSSwitchButton::getIndicatorFrameColorOn() const { return indicatorFrameColorOn; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorFrameColorOn(DrawColor color) {
    indicatorFrameColorOn = color;
    return this;
}

DrawColor EOGSSwitchButton::getIndicatorFrameColorOff() const { return indicatorFrameColorOff; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorFrameColorOff(DrawColor color) {
    indicatorFrameColorOff = color;
    return this;
}

DrawColor EOGSSwitchButton::getIndicatorFilledColorOn() const { return indicatorFilledColorOn; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorFilledColorOn(DrawColor color) {
    indicatorFilledColorOn = color;
    return this;
}

DrawColor EOGSSwitchButton::getIndicatorFilledColorOff() const { return indicatorFilledColorOff; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorFilledColorOff(DrawColor color) {
    indicatorFilledColorOff = color;
    return this;
}

DrawColor EOGSSwitchButton::getBgColorOn() const { return bgColorOn; }
EOGSSwitchButton* EOGSSwitchButton::setBgColorOn(DrawColor color) {
    bgColorOn = color;
    return this;
}

DrawColor EOGSSwitchButton::getBgColorOff() const { return bgColorOff; }
EOGSSwitchButton* EOGSSwitchButton::setBgColorOff(DrawColor color) {
    bgColorOff = color;
    return this;
}

// 背景填充设置
bool EOGSSwitchButton::getBgOnFilled() const { return bgOnFilled; }
EOGSSwitchButton* EOGSSwitchButton::setBgOnFilled(bool filled) {
    bgOnFilled = filled;
    return this;
}

bool EOGSSwitchButton::getBgOffFilled() const { return bgOffFilled; }
EOGSSwitchButton* EOGSSwitchButton::setBgOffFilled(bool filled) {
    bgOffFilled = filled;
    return this;
}

// 指示器填充设置
bool EOGSSwitchButton::getIndicatorOnFilled() const { return indicatorOnFilled; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorOnFilled(bool filled) {
    indicatorOnFilled = filled;
    return this;
}

bool EOGSSwitchButton::getIndicatorOffFilled() const { return indicatorOffFilled; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorOffFilled(bool filled) {
    indicatorOffFilled = filled;
    return this;
}

// 内边距设置
int16_t EOGSSwitchButton::getPadding() const { return paddingTop; }
EOGSSwitchButton* EOGSSwitchButton::setPadding(int16_t padding) {
    if (paddingTop != padding || paddingRight != padding || paddingBottom != padding || paddingLeft != padding) {
        paddingTop = padding;
        paddingRight = padding;
        paddingBottom = padding;
        paddingLeft = padding;
        indicatorDimensionUpdated = false; // 内边距改变时需要更新指示器位置
    }
    return this;
}

int16_t EOGSSwitchButton::getPaddingTop() const { return paddingTop; }
EOGSSwitchButton* EOGSSwitchButton::setPaddingTop(int16_t padding) {
    if (paddingTop != padding) {
        paddingTop = padding;
        indicatorDimensionUpdated = false;
    }
    return this;
}

int16_t EOGSSwitchButton::getPaddingRight() const { return paddingRight; }
EOGSSwitchButton* EOGSSwitchButton::setPaddingRight(int16_t padding) {
    if (paddingRight != padding) {
        paddingRight = padding;
        indicatorDimensionUpdated = false;
    }
    return this;
}

int16_t EOGSSwitchButton::getPaddingBottom() const { return paddingBottom; }
EOGSSwitchButton* EOGSSwitchButton::setPaddingBottom(int16_t padding) {
    if (paddingBottom != padding) {
        paddingBottom = padding;
        indicatorDimensionUpdated = false;
    }
    return this;
}

int16_t EOGSSwitchButton::getPaddingLeft() const { return paddingLeft; }
EOGSSwitchButton* EOGSSwitchButton::setPaddingLeft(int16_t padding) {
    if (paddingLeft != padding) {
        paddingLeft = padding;
        indicatorDimensionUpdated = false;
    }
    return this;
}

EOGSSwitchButton* EOGSSwitchButton::setPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
    if (paddingTop != top || paddingRight != right || paddingBottom != bottom || paddingLeft != left) {
        paddingTop = top;
        paddingRight = right;
        paddingBottom = bottom;
        paddingLeft = left;
        indicatorDimensionUpdated = false;
    }
    return this;
}

// 圆角半径设置
int16_t EOGSSwitchButton::getbgRadius() const { return bgRadius; }
EOGSSwitchButton* EOGSSwitchButton::setBgRadius(int16_t radius) {
    bgRadius = radius;
    return this;
}

int16_t EOGSSwitchButton::getIndicatorRadius() const { return indicatorRadius; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorRadius(int16_t radius) {
    indicatorRadius = radius;
    return this;
}

// 指示器宽度设置
float EOGSSwitchButton::getIndicatorWidth(bool relative) const { 
    return relative ? indicatorWidth : (isIndicatorWidthRelative ? (indicatorWidth * w) : indicatorWidth); 
}
EOGSSwitchButton* EOGSSwitchButton::setIndicatorWidth(float width, bool isRelative) {
    if (indicatorWidth != width || isIndicatorWidthRelative != isRelative) {
        indicatorWidth = width;
        setIndicatorWidthRelative(isRelative);
        requestIndicatorDimensionUpdate();
    }
    return this;
}

bool EOGSSwitchButton::getIndicatorWidthRelative() const { return isIndicatorWidthRelative; }
EOGSSwitchButton* EOGSSwitchButton::setIndicatorWidthRelative(bool isRelative) {
    if (isIndicatorWidthRelative != isRelative) {
        isIndicatorWidthRelative = isRelative;
        requestIndicatorDimensionUpdate();
    }
    return this;
}


#undef indicatorDimensionUpdated
#undef isIndicatorWidthRelative
#undef isChecked
#undef bgFilled
#undef indicatorFilled
#undef indicatorColorOn
#undef indicatorColorOff
#undef bgColorOn
#undef bgColorOff