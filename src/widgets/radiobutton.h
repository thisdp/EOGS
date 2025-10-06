//
// Created by thisdp 2025/10/2.
// Radio Button Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include <vector>
#include <algorithm>

// Flag 1
#define isChecked generalFlag1
// Flag 4~5
#define outerCircleColor generalColor4
// Flag 6~7
#define innerCircleColor generalColor6

class EOGSRadioButton;

// 单选按钮组类 - 仅保留对 vector<EOGSRadioButton *> 的 protected 派生，但包含选中状态管理
class EOGSRadioButtonGroup : protected std::vector<EOGSRadioButton*> {
    friend class EOGSRadioButton;
private:
    EOGSRadioButton* selectedButton;  // 组内当前选中的按钮
    EOGSRadioButton* getSelected() const;
};

class EOGSRadioButton : public EOGSWidget<EOGSRadioButton> {
    friend class EOGSRadioButtonGroup;
    
protected:
    float outerRadius;
    float innerRadius;
    int16_t cx, cy; // 圆心坐标
    int16_t renderOuterRadius;
    int16_t renderInnerRadius;
    EOGSRadioButtonGroup* group;         // 所属组

public:
    // 静态函数
    static void onConfirm(EOGSEvent* event);
    // 构造函数
    EOGSRadioButton(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSRadioButton();

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 选中状态
    bool getChecked() const;
    EOGSRadioButton* setChecked(bool checked);

    EOGSRadioButtonGroup* getGroup() const;
    EOGSRadioButton* setGroup(EOGSRadioButtonGroup& _group);
    EOGSRadioButton* setGroup(EOGSRadioButtonGroup* _group);

    // 颜色设置
    DrawColor getOuterCircleColor() const;
    EOGSRadioButton* setOuterCircleColor(DrawColor color);

    DrawColor getInnerCircleColor() const;
    EOGSRadioButton* setInnerCircleColor(DrawColor color);

    // 获取圆形半径
    int16_t getCircleRadius() const;
    int16_t getInnerCircleRadius() const;
};

#undef isChecked
#undef outerCircleColor
#undef innerCircleColor