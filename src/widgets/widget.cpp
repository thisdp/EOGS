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

EOGSWidgetBase::~EOGSWidgetBase() {
    off();  // 在析构时自动清除该对象的所有监听器
    if(parent != nullptr && parent->isContainer())
        parent->removeChild(this);
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

// 事件系统实现
std::map<EOGSEventID, std::map<uint32_t, std::map<uint32_t, std::function<void(EOGSEvent*)>>>> EOGSWidgetBase::eventMap;  // 静态事件映射，按事件ID、对象ID和索引存储
uint32_t EOGSWidgetBase::nextId = 0;  // 初始化nextId
uint32_t EOGSWidgetBase::nextCallbackId = 0;  // 初始化nextCallbackId

uint32_t EOGSWidgetBase::on(const EOGSEventID event, std::function<void(EOGSEvent*)> callback) {
    uint32_t callbackId = nextCallbackId++;
    eventMap[event][id][callbackId] = callback;
    return callbackId;
}

EOGSEvent EOGSWidgetBase::makeTrigger(const EOGSEventID event) {
    return EOGSEvent(event, this);
}

void EOGSWidgetBase::trigger(EOGSEvent *event) {
    if (event == nullptr) return;
    auto it = eventMap.find(event->eventId);
    if (it == eventMap.end()) return;
    auto objectIt = it->second.find(id);  // 只查找this对象的监听器
    if (objectIt == it->second.end()) return;
    for (const auto& pair : objectIt->second) { //一个event可能被附加了多个，遍历
        pair.second(event);  // 传递this指针和事件对象
    }
}

void EOGSWidgetBase::off(const EOGSEventID event) {
    auto it = eventMap.find(event);
    if (it == eventMap.end()) return;
    // 清空当前对象的该事件的所有监听器
    it->second.erase(id);
}

void EOGSWidgetBase::off(const EOGSEventID event, uint32_t callbackId) {
    auto it = eventMap.find(event);
    if (it == eventMap.end()) return;
    // 清空当前对象的该事件的指定callback的监听器
    auto& objectCallbacks = it->second;  // std::map<uint32_t, std::map<uint32_t, std::function<void()>>>
    auto objectIt = objectCallbacks.find(id);
    if (objectIt == objectCallbacks.end()) return;
    
    // 通过索引删除指定的回调函数
    auto& callbacks = objectIt->second;  // std::map<uint32_t, std::function<void()>>
    callbacks.erase(callbackId);
}

void EOGSWidgetBase::off() {
    for (auto& eventListeners : eventMap) {
        eventListeners.second.erase(id);  // 移除this对象的所有监听器
    }
}

//事件系统
EOGSEvent::EOGSEvent(EOGSEventID id, EOGSWidgetBase *src) : eventId(id), source(src) {}