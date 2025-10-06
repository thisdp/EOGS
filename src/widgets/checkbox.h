//
// Created by thisdp 2025/10/4.
// Checkbox Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

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

class EOGSCheckBox : public EOGSWidget<EOGSCheckBox> {
protected:
    int16_t padding;           // 统一内边距
    int16_t borderRadius;      // 复选框圆角半径
    int16_t checkMarkRenderX;  // 勾选标记渲染X坐标
    int16_t checkMarkRenderY;  // 勾选标记渲染Y坐标
    int16_t checkMarkRenderW;  // 勾选标记渲染宽度
    int16_t checkMarkRenderH;  // 勾选标记渲染高度
    int16_t boxRenderX;        // 复选框渲染X坐标
    int16_t boxRenderY;        // 复选框渲染Y坐标
    int16_t boxRenderW;        // 复选框渲染宽度
    int16_t boxRenderH;        // 复选框渲染高度

public:
    // 静态函数
    static void onConfirm(EOGSEvent* event);  //进入此函数，self一定是EOGSCheckBox
    
    // 构造函数
    EOGSCheckBox(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSCheckBox() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 请求更新勾选标记位置
    void requestCheckMarkUpdate();
    bool updateCheckMark();

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 状态获取和设置
    bool getChecked() const;
    EOGSCheckBox* setChecked(bool checked);
    
    // 正方形模式设置
    bool getSquare() const;
    EOGSCheckBox* setSquare(bool square);

    // 颜色设置
    DrawColor getBorderColor() const;
    EOGSCheckBox* setBorderColor(DrawColor color);
    
    DrawColor getCheckMarkColor() const;
    EOGSCheckBox* setCheckMarkColor(DrawColor color);
    
    DrawColor getCheckMarkDisabledColor() const;
    EOGSCheckBox* setCheckMarkDisabledColor(DrawColor color);
    
    // 内边距设置
    int16_t getPadding() const;
    EOGSCheckBox* setPadding(int16_t padding);
    
    // 圆角半径设置
    int16_t getBorderRadius() const;
    EOGSCheckBox* setBorderRadius(int16_t radius);
};

#undef isChecked
#undef checkMarkUpdated
#undef borderColor
#undef checkMarkColor