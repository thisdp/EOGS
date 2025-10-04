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
    EOGSRectangle(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _filled = true)
        : EOGSWidget<EOGSRectangle>(_x, _y, _w, _h, _isRelative),radius(0) {
            setFilled(true);
            setColor(DrawColor::WHITE);
        }

    ~EOGSRectangle() = default;
    
    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (dimensionUpdated) return false; dimensionUpdated = true;

        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        return true;
    }
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        if(insideOfRenderArea || renderOutsideOfRenderArea){
            eogs->setDrawColor(drawColor);
            if (filled)
                if(radius > 0)
                    eogs->drawRBox(renderX, renderY, static_cast<int16_t>(w), static_cast<int16_t>(h), radius);
                else
                    eogs->drawBox(renderX, renderY, static_cast<int16_t>(w), static_cast<int16_t>(h));
            else
                if(radius > 0)
                    eogs->drawRFrame(renderX, renderY, static_cast<int16_t>(w), static_cast<int16_t>(h), radius);
                else
                    eogs->drawFrame(renderX, renderY, static_cast<int16_t>(w), static_cast<int16_t>(h));
        }
    }
//颜色
    DrawColor getColor() const { return drawColor; }
    EOGSRectangle* setColor(DrawColor _drawColor){
        drawColor = _drawColor;
        return this;
    }
//填充  
    bool isFilled() const { return filled; }
    EOGSRectangle* setFilled(bool _filled) {
        filled = _filled;
        return this;
    }
//圆角
    int16_t getCornerRadius() const { return radius; }
    EOGSRectangle* setCornerRadius(int16_t _radius) {
        radius = _radius;
        return this;
    }
    
};

// Label容器控件类
class EOGSRectangleContainer : public EOGSRectangle {
//构造析构
public:
    EOGSRectangleContainer(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSRectangle(_x, _y, _w, _h, _isRelative) {
    }
    
    ~EOGSRectangleContainer() {
        std::vector<EOGSWidgetBase*>* children = getChildren();
        for (EOGSWidgetBase* child : *children) {
            if (child) delete child;
        }
        children->clear();
    }

//容器实现
protected:
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
public:
    bool isContainer() const override { return true; }
    std::vector<EOGSWidgetBase*>* getChildren() { return &children; }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        bool wasDimensionUpdate = !dimensionUpdated;
        EOGSRectangle::render(eogs, parentRX, parentRY, parentW, parentH);
        renderChildren(eogs,wasDimensionUpdate);
    }
};

#undef filled
#undef drawColor