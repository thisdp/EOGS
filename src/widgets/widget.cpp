#include "widget.h"
#include "../EOGS.h"
#include <algorithm>


void EOGSWidgetBase::addChild(EOGSWidgetBase* child, bool isUpdateParent) {
    if (isContainer() && child != nullptr){
        if (child->getParent() != nullptr && child->getParent()->isContainer()) child->getParent()->removeChild(child);
        if(isUpdateParent) child->setParent(this,false);
        getChildren()->push_back(child);
    }
}

void EOGSWidgetBase::removeChild(EOGSWidgetBase* child, bool isUpdateParent) {
    if (isContainer() && child != nullptr){
        std::vector<EOGSWidgetBase*>* children = getChildren();
        auto it = std::find(children->begin(), children->end(), child);
        if (it != children->end()) {
            if(isUpdateParent) child->setParent(nullptr,false);
            children->erase(it);
        }
    }
}

void EOGSWidgetBase::setPosition(float _x, float _y) {
    x = _x, y = _y;
    requestDimensionUpdate();
}

void EOGSWidgetBase::setPosition(float _x, float _y, bool _isXRel, bool _isYRel) {
    isXRel = _isXRel, isYRel = _isYRel;
    setPosition(_x, _y);
}

void EOGSWidgetBase::setPosition(float _x, float _y, bool _isXYRel) {
    setPosition(_x, _y, _isXYRel, _isXYRel);
}

void EOGSWidgetBase::setX(float _x, bool _isRel) {
    x = _x, isXRel = _isRel;
    requestDimensionUpdate();
}
void EOGSWidgetBase::setY(float _y, bool _isRel) {
    y = _y, isYRel = _isRel;
    requestDimensionUpdate();
}

void EOGSWidgetBase::setSize(float _w, float _h) {
    w = _w, h = _h;
    requestDimensionUpdate();
}

void EOGSWidgetBase::setSize(float _w, float _h, bool _isWRel, bool _isHRel) {
    isWRel = _isWRel, isHRel = _isHRel;
    setSize(_w, _h);
}

void EOGSWidgetBase::setSize(float _w, float _h, bool _isWHRel) {
    setSize(_w, _h, _isWHRel, _isWHRel);
}

void EOGSWidgetBase::setW(float _w, bool _isRel) {
    w = _w, isWRel = _isRel;
    requestDimensionUpdate();
}
void EOGSWidgetBase::setH(float _h, bool _isRel) {
    h = _h, isHRel = _isRel;
    requestDimensionUpdate();
}

void EOGSWidgetBase::setParent(EOGSWidgetBase* _parent, bool isUpdateChild) {
    if (parent != _parent){
        requestDimensionUpdate();   //重定向parent之后，需要重新计算尺寸
        requestRenderPosUpdate();
        if(isUpdateChild && parent != nullptr && parent->isContainer()) parent->removeChild(this, false);    //移除
        if(_parent == nullptr){
            parent = _parent;
        }else{
            parent = _parent;
            if(isUpdateChild && parent->isContainer()) parent->addChild(this,false);   //新增
        }
    }
}

void EOGSWidgetBase::setVisible(bool _visible) {
    visible = _visible;
}

void EOGSWidgetBase::setForceRenderOutsideOfCanvas(bool forceRenderOutsideOfCanvas) {
    renderOutsideOfRenderArea = forceRenderOutsideOfCanvas;
}

void EOGSWidgetBase::requestRenderPosUpdate(bool instant) {
    renderPosUpdated = false; 
    if(!instant) return;
    updateRenderPos(getEOGS(),parent->getRenderX(),parent->getRenderY(),parent->getAbsW(),parent->getAbsH());
}

bool EOGSWidgetBase::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;
    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    return true;
}

void EOGSWidgetBase::requestDimensionUpdate(bool instant) {
    dimensionUpdated = false; 
    if(!instant) return;
    updateDimension(parent->getAbsW(),parent->getAbsH());
}

bool EOGSWidgetBase::updateDimension(int16_t parentW, int16_t parentH) {
    if (dimensionUpdated) return false;
    dimensionUpdated = true;

    if (isXRel) x = static_cast<int16_t>(relX * parentW);
    else if (parentW == 0) relX = 0;
    else relX = static_cast<float>(x) / parentW;

    if (isYRel) y = static_cast<int16_t>(relY * parentH);
    else if (parentH == 0) relY = 0;
    else relY = static_cast<float>(y) / parentH;

    if (isWRel) w = static_cast<int16_t>(relW * parentW);
    else if (parentW == 0) relW = 0;
    else relW = static_cast<float>(w) / parentW;

    if (isHRel) h = static_cast<int16_t>(relH * parentH);
    else if (parentH == 0) relH = 0;
    else relH = static_cast<float>(h) / parentH;

    return true;
}

void EOGSWidgetBase::addAnimToEOGS(void* anim) {
    EOGS* pEOGS = getEOGS();
    if (pEOGS != nullptr) pEOGS->addAnimation(static_cast<EOGSAnimBase*>(anim));
}
