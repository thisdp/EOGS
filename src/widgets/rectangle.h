//
// Created by thisdp 2025/8/22.
// Rectangle Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

//Flag 1
#define filled generalFlag1
//Flag 2~3
#define drawColor generalColor2
class EOGSRectangle : public EOGSWidget<EOGSRectangle> {
protected:
    int16_t radius;

public:
    // 简化构造函数，使用统一的相对标志
    EOGSRectangle(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _filled = true);

    ~EOGSRectangle() = default;
    
    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
//颜色
    DrawColor getColor() const;
    EOGSRectangle* setColor(DrawColor _drawColor);
//填充
    bool isFilled() const;
    EOGSRectangle* setFilled(bool _filled);
//圆角
    int16_t getCornerRadius() const;
    EOGSRectangle* setCornerRadius(int16_t _radius);
    
};

// Label容器控件类
class EOGSRectangleContainer : public EOGSRectangle {
//构造析构
public:
    EOGSRectangleContainer(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSRectangleContainer();

//容器实现
protected:
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
public:
    bool isContainer() const override;
    std::vector<EOGSWidgetBase*>* getChildren();

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
};

#undef filled
#undef drawColor