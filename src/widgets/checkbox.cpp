//
// Created by thisdp 2025/10/4.
// Checkbox Widget for EOGS
//

#include "checkbox.h"
// Flag 1
#define isChecked generalFlag1
// Flag 2
#define checkMarkUpdated generalFlag2
// Flag 3
#define isSquare generalFlag3
// Flag 4~5
#define borderColor generalColor4
// Flag 6~7
#define checkMarkColor generalColor6

// 静态函数
void EOGSCheckBox::onClick(EOGSEvent* event){  //进入此函数，self一定是EOGSCheckBox
    EOGSCheckBox *checkBox = static_cast<EOGSCheckBox*>(event->self);
    checkBox->setChecked(!checkBox->getChecked());
}

// 构造函数
EOGSCheckBox::EOGSCheckBox(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSWidget<EOGSCheckBox>(_x, _y, _w, _h, _isRelative),
      padding(2),             // 默认内边距
      borderRadius(2),        // 默认小圆角
      checkMarkRenderX(0),
      checkMarkRenderY(0),
      checkMarkRenderW(0),
      checkMarkRenderH(0),
      boxRenderX(0),
      boxRenderY(0),
      boxRenderW(0),
      boxRenderH(0) {
    // 初始化颜色属性
    setBorderColor(DrawColor::WHITE);
    setCheckMarkColor(DrawColor::WHITE);
    setChecked(false);
    setSquare(true);  // 默认为正方形
    on(EOGSEventID::Click, onClick);
}

// 更新控件尺寸和位置
bool EOGSCheckBox::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX + x;
    renderY = parentRY + y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    
    // 计算复选框绘制区域
    boxRenderW = w - padding * 2;
    boxRenderH = h - padding * 2;
    boxRenderX = renderX + padding;
    boxRenderY = renderY + padding;
    
    // 如果是正方形模式，使用较小的尺寸作为基准
    if (isSquare) {
        int16_t size = min(boxRenderW, boxRenderH);
        // 居中绘制正方形
        boxRenderX += (boxRenderW - size) / 2;
        boxRenderY += (boxRenderH - size) / 2;
        boxRenderW = size;
        boxRenderH = size;
    }
    
    requestCheckMarkUpdate();  // 如果更新了位置，则需要更新勾选标记位置
    return true;
}

// 请求更新勾选标记位置
void EOGSCheckBox::requestCheckMarkUpdate() { checkMarkUpdated = false; }
bool EOGSCheckBox::updateCheckMark() {
    if (checkMarkUpdated) return false;
    checkMarkUpdated = true;

    // 计算勾选标记大小（框大小的60%）
    checkMarkRenderW = boxRenderW * 0.8;
    checkMarkRenderH = boxRenderH * 0.8;
    
    // 计算勾选标记位置（居中）
    checkMarkRenderX = boxRenderX + (boxRenderW - checkMarkRenderW) / 2;
    checkMarkRenderY = boxRenderY + (boxRenderH - checkMarkRenderH) / 2;
    
    return true;
}

void EOGSCheckBox::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    updateDimension(parentW, parentH);
    updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    updateCheckMark();

    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        // 绘制复选框边框
        eogs->setDrawColor(borderColor);
        
        if (borderRadius > 0) {
            // 使用圆角绘制复选框
            eogs->drawRFrame(boxRenderX, boxRenderY, boxRenderW, boxRenderH, borderRadius);
        } else {
            // 使用普通矩形绘制复选框
            eogs->drawFrame(boxRenderX, boxRenderY, boxRenderW, boxRenderH);
        }
        
        // 如果选中，绘制勾选标记
        if (isChecked) {
            eogs->setDrawColor(checkMarkColor);
            // 绘制勾选标记（使用简单的线条组成一个勾）
            int16_t x1 = checkMarkRenderX + checkMarkRenderW / 4;
            int16_t y1 = checkMarkRenderY + checkMarkRenderH / 2;
            int16_t x2 = checkMarkRenderX + checkMarkRenderW / 2;
            int16_t y2 = checkMarkRenderY + checkMarkRenderH * 3 / 4;
            int16_t x3 = checkMarkRenderX + checkMarkRenderW * 3 / 4;
            int16_t y3 = checkMarkRenderY + checkMarkRenderH / 4;
            
            // 绘制勾选标记的两条线
            eogs->drawLine(x1, y1, x2, y2);
            eogs->drawLine(x2, y2, x3, y3);
        }
    }
}

// 状态获取和设置
bool EOGSCheckBox::getChecked() const { return isChecked; }
EOGSCheckBox* EOGSCheckBox::setChecked(bool checked) {
    if (isChecked != checked) {
        isChecked = checked;
    }
    return this;
}

// 正方形模式设置
bool EOGSCheckBox::getSquare() const { return isSquare; }
EOGSCheckBox* EOGSCheckBox::setSquare(bool square) {
    if (isSquare != square) {
        isSquare = square;
        checkMarkUpdated = false; // 模式改变时需要更新勾选标记位置
    }
    return this;
}


// 颜色设置
DrawColor EOGSCheckBox::getBorderColor() const { return borderColor; }
EOGSCheckBox* EOGSCheckBox::setBorderColor(DrawColor color) {
    borderColor = color;
    return this;
}

DrawColor EOGSCheckBox::getCheckMarkColor() const { return checkMarkColor; }
EOGSCheckBox* EOGSCheckBox::setCheckMarkColor(DrawColor color) {
    checkMarkColor = color;
    return this;
}

// 内边距设置
int16_t EOGSCheckBox::getPadding() const { return padding; }
EOGSCheckBox* EOGSCheckBox::setPadding(int16_t _padding) {
    if (padding != _padding) {
        padding = _padding;
        checkMarkUpdated = false; // 内边距改变时需要更新勾选标记位置
    }
    return this;
}

// 圆角半径设置
int16_t EOGSCheckBox::getBorderRadius() const { return borderRadius; }
EOGSCheckBox* EOGSCheckBox::setBorderRadius(int16_t radius) {
    borderRadius = radius;
    return this;
}

#undef isChecked
#undef checkMarkUpdated
#undef isSquare
#undef borderColor
#undef checkMarkColor