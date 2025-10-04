#include "rectangle.h"

//Flag 1
#define filled generalFlag1
//Flag 2~3
#define drawColor generalColor2

// 简化构造函数，使用统一的相对标志
EOGSRectangle::EOGSRectangle(float _x, float _y, float _w, float _h, bool _isRelative, bool _filled)
    : EOGSWidget<EOGSRectangle>(_x, _y, _w, _h, _isRelative),radius(0) {
        setFilled(true);
        setColor(DrawColor::WHITE);
    }


// 更新控件尺寸和位置
bool EOGSRectangle::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH)  {
    if (dimensionUpdated) return false;
    dimensionUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    return true;
}

void EOGSRectangle::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH)  {
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
DrawColor EOGSRectangle::getColor() const { return drawColor; }
EOGSRectangle* EOGSRectangle::setColor(DrawColor _drawColor){
    drawColor = _drawColor;
    return this;
}

//填充  
bool EOGSRectangle::isFilled() const { return filled; }
EOGSRectangle* EOGSRectangle::setFilled(bool _filled) {
    filled = _filled;
    return this;
}

//圆角
int16_t EOGSRectangle::getCornerRadius() const { return radius; }
EOGSRectangle* EOGSRectangle::setCornerRadius(int16_t _radius) {
    radius = _radius;
    return this;
}

// Label容器控件类
//构造析构
EOGSRectangleContainer::EOGSRectangleContainer(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSRectangle(_x, _y, _w, _h, _isRelative) {
}

EOGSRectangleContainer::~EOGSRectangleContainer() {
    std::vector<EOGSWidgetBase*>* children = getChildren();
    for (EOGSWidgetBase* child : *children) {
        if (child) delete child;
    }
    children->clear();
}

//容器实现
bool EOGSRectangleContainer::isContainer() const  { return true; }
std::vector<EOGSWidgetBase*>* EOGSRectangleContainer::getChildren() { return &children; }

void EOGSRectangleContainer::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH)  {
    if (!visible || eogs == nullptr) return;
    bool wasDimensionUpdate = !dimensionUpdated;
    EOGSRectangle::render(eogs, parentRX, parentRY, parentW, parentH);
    renderChildren(eogs,wasDimensionUpdate);
}

#undef filled
#undef drawColor