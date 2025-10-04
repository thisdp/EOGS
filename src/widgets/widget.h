//
// Created by thisdp 2025/8/20.
// Widget Base for EOGS
//
#pragma once

#include <Arduino.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <map>
#include "../EOGS_hal/EOGS_hal.h"
#define mathClamp(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

class EOGSWidgetBase;

enum class EOGSEventID : uint32_t {
    Click = 0,
};

// 事件传播模式枚举
enum class EOGSEventPropagate {
    None,     // 不传播事件
    Up,     // 向上传播事件
    Down,   // 向下传播事件
    Both    // 双向传播事件
};

// 事件类声明
class EOGSEvent {   //基础事件类
public:
    EOGSEventID eventId;
    EOGSWidgetBase *source;
    EOGSWidgetBase *self;
    bool cancelled;
    EOGSEvent(EOGSEventID id);
    void cancel();
    bool wasCancelled();
};

// 前向声明
class EOGS;
class EOGSAnimBase;
// 基础控件类基类
class EOGSWidgetBase {
protected:
    // 事件回调信息结构体
    struct EventCallbackInfo {
        std::function<void(EOGSEvent*)> callback;
        bool receivePropagate;
        int priority;

        EventCallbackInfo(std::function<void(EOGSEvent*)> cb, bool prop, int prio = 0) : callback(cb), receivePropagate(prop), priority(prio) {}
    };
    
    /*  结构:
        [事件ID] = {
            [对象ID] = [ (callbackId, 回调信息), ... ],
        },
    */
    static std::map<EOGSEventID, std::map<uint32_t, std::vector<std::pair<uint32_t, EventCallbackInfo>>>> eventMap;  // 静态事件映射，按事件ID、对象ID和回调向量存储（支持优先级）
    static uint32_t nextCallbackId;  // 用于生成回调函数的唯一索引
    static uint32_t nextId;  // 用于生成唯一ID

    uint32_t id;  // 对象的唯一ID
    EOGSWidgetBase* parent;  // 父级元素指针
    float relX, relY, relW, relH;  // 相对坐标和大小
    int16_t renderX, renderY;
    int16_t x, y, w, h;  // 绝对坐标和大小
    union{
        uint32_t flags;
        struct{
            uint8_t isXRel : 1;
            uint8_t isYRel : 1;
            uint8_t isWRel : 1;
            uint8_t isHRel : 1;
            uint8_t dimensionUpdated : 1;
            uint8_t renderPosUpdated : 1;
            uint8_t insideOfRenderArea : 1;
            uint8_t renderOutsideOfRenderArea: 1;
            
            uint8_t visible : 1;
            uint8_t generalFlag1 : 1;
            uint8_t generalFlag2 : 1;
            uint8_t generalFlag3 : 1;
            uint8_t generalFlag4 : 1;
            uint8_t generalFlag5 : 1;
            uint8_t generalFlag6 : 1;
            uint8_t generalFlag7 : 1;

            uint8_t generalFlag8 : 1;
            uint8_t generalFlag9 : 1;
            uint8_t generalFlag10 : 1;
            uint8_t generalFlag11 : 1;
            uint8_t generalFlag12 : 1;
            uint8_t generalFlag13 : 1;
            uint8_t generalFlag14 : 1;
            uint8_t generalFlag15 : 1;
            
            uint8_t generalFlag16 : 1;
            uint8_t generalFlag17 : 1;
            uint8_t generalFlag18 : 1;
            uint8_t generalFlag19 : 1;
            uint8_t generalFlag20 : 1;
            uint8_t generalFlag21 : 1;
            uint8_t generalFlag22 : 1;
            uint8_t generalFlag23 : 1;

        };
        struct{
            uint8_t : 8;

            uint8_t : 2;
            DrawColor generalColor2 : 2;
            DrawColor generalColor4 : 2;
            DrawColor generalColor6 : 2;
            
            DrawColor generalColor8 : 2;
            DrawColor generalColor10 : 2;
            DrawColor generalColor12 : 2;
            DrawColor generalColor14 : 2;
            
            DrawColor generalColor16 : 2;
            DrawColor generalColor18 : 2;
            DrawColor generalColor20 : 2;
            DrawColor generalColor22 : 2;
        };
        struct{
            uint8_t : 8;
            
            uint8_t : 2;
            HAlign generalHAlign2 : 2;
            HAlign generalHAlign4 : 2;
            HAlign generalHAlign6 : 2;
            
            HAlign generalHAlign8 : 2;
            HAlign generalHAlign10 : 2;
            HAlign generalHAlign12 : 2;
            HAlign generalHAlign14 : 2;
            
            HAlign generalHAlign16 : 2;
            HAlign generalHAlign18 : 2;
            HAlign generalHAlign20 : 2;
            HAlign generalHAlign22 : 2;
        };
        struct{
            uint8_t : 8;
            
            uint8_t : 2;
            VAlign generalVAlign2 : 2;
            VAlign generalVAlign4 : 2;
            VAlign generalVAlign6 : 2;
            
            VAlign generalVAlign8 : 2;
            VAlign generalVAlign10 : 2;
            VAlign generalVAlign12 : 2;
            VAlign generalVAlign14 : 2;
            
            VAlign generalVAlign16 : 2;
            VAlign generalVAlign18 : 2;
            VAlign generalVAlign20 : 2;
            VAlign generalVAlign22 : 2;
        };
        struct{
            uint8_t : 8;
            
            uint8_t : 2;
            uint8_t generalAlign2 : 2;
            uint8_t generalAlign4 : 2;
            uint8_t generalAlign6 : 2;
            
            uint8_t generalAlign8 : 2;
            uint8_t generalAlign10 : 2;
            uint8_t generalAlign12 : 2;
            uint8_t generalAlign14 : 2;
            
            uint8_t generalAlign16 : 2;
            uint8_t generalAlign18 : 2;
            uint8_t generalAlign20 : 2;
            uint8_t generalAlign22 : 2;
        };
    };
    

public:
    EOGSWidgetBase(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : id(nextId++), parent(nullptr),
        relX(_x), relY(_y), relW(_w), relH(_h),
        renderX(0), renderY(0),
        x(static_cast<int16_t>(_x)), y(static_cast<int16_t>(_y)), w(static_cast<int16_t>(_w)), h(static_cast<int16_t>(_h))
        
    {
        flags = 0;
        isXRel = _isRelative;
        isYRel = _isRelative;
        isWRel = _isRelative;
        isHRel = _isRelative;
        visible = true;
    }

    virtual ~EOGSWidgetBase();
    
    // 新Class需要重载的
public:
    // 事件系统
    // 增加 priority 参数（越大优先级越高）。在添加时将对该对象的回调按优先级排序
    uint32_t on(const EOGSEventID event, std::function<void(EOGSEvent*)> callback, bool receivePropagate = true, int priority = 0);  // 自动使用this->id作为objectId，返回回调函数的索引
    EOGSEvent makeTrigger(const EOGSEventID event);  // 根据事件ID生成事件对象
    void trigger(EOGSEvent* event, EOGSEventPropagate propagationMode = EOGSEventPropagate::None);  // 触发事件，接受事件对象指针
    void off(const EOGSEventID event);  // 移除特定事件的所有监听器
    void off(const EOGSEventID event, uint32_t callbackId);  // 移除特定事件的指定监听器
    void off();  // 移除this对象的所有监听器
    // 纯虚函数接口
    virtual void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) = 0;
    virtual bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH);   //计算渲染位置
    virtual bool updateDimension(int16_t parentW, int16_t parentH);    // 仅计算Position/Size
    virtual bool isContainer() const { return false; };
    virtual std::vector<EOGSWidgetBase*>* getChildren() { return nullptr; }
    
    // Getter方法
    float getX(bool relative = false) const { return relative ? getRelX() : getAbsX(); }
    float getY(bool relative = false) const { return relative ? getRelY() : getAbsY(); }
    float getW(bool relative = false) const { return relative ? getRelW() : getAbsW(); }
    float getH(bool relative = false) const { return relative ? getRelH() : getAbsH(); }
    virtual float getRelX() const { return relX; }
    virtual float getRelY() const { return relY; }
    virtual float getRelW() const { return relW; }
    virtual float getRelH() const { return relH; }
    virtual int16_t getAbsX() const { return x; }
    virtual int16_t getAbsY() const { return y; }
    virtual int16_t getAbsW() const { return w; }
    virtual int16_t getAbsH() const { return h; }
    virtual int16_t getRenderX() const { return renderX; }
    virtual int16_t getRenderY() const { return renderY; }
    virtual bool isXRelative() const { return isXRel; }
    virtual bool isYRelative() const { return isYRel; }
    virtual bool isWRelative() const { return isWRel; }
    virtual bool isHRelative() const { return isHRel; }
    virtual bool isVisible() const { return visible; }
    virtual bool isInsideOfCanvas() const { return insideOfRenderArea; }
    virtual bool getForceRenderOutsideOfCanvas() const { return renderOutsideOfRenderArea; }
    virtual EOGSWidgetBase* getParent() const { return parent; }
    virtual EOGS* getEOGS() { return parent != nullptr ? parent->getEOGS() : nullptr; }
    virtual void requestDimensionUpdate(bool instant = false);
    virtual void requestRenderPosUpdate(bool instant = false);
    
    void setPosition(float _x, float _y);
    void setPosition(float _x, float _y, bool _isXRel, bool _isYRel);
    void setPosition(float _x, float _y, bool _isXYRel);
    void setX(float _x, bool _isRelative);
    void setY(float _y, bool _isRelative);
    void setSize(float _w, float _h);
    void setSize(float _w, float _h, bool _isWRel, bool _isHRel);
    void setSize(float _w, float _h, bool _isWHRel);
    void setW(float _w, bool _isRelative);
    void setH(float _h, bool _isRelative);
    void setParent(EOGSWidgetBase* _parent, bool isUpdateChild = true);
    void setVisible(bool _visible);
    void setForceRenderOutsideOfCanvas(bool forceRenderOutsideOfCanvas);
    void addChild(EOGSWidgetBase* child, bool isUpdateParent = true);
    void removeChild(EOGSWidgetBase* child, bool isUpdateParent = true);

    // 新Class可能需要重载的
protected:
    virtual void renderChildren(EOGS* eogs, bool requestDimensionUpdate = false) {
        std::vector<EOGSWidgetBase*>* children = getChildren();
        if(children == nullptr) return;
        for (auto child : *children){
            if(child == nullptr) continue;
            if (requestDimensionUpdate) child->requestDimensionUpdate();
            if (child->isVisible()) child->render(eogs,renderX,renderY,w,h);
        }
    }
    virtual bool isOutsideOfParent(int16_t parentW, int16_t parentH) {
        return (x + w < 0) || (x > parentW) || (y + h < 0) || (y > parentH);
    }

    // 结束
public:

    // 模板方法，用于创建任何类型的Widget
    template<typename WidgetType, typename... Args>
    [[nodiscard]] WidgetType* create(Args&&... args) {
        static_assert(std::is_base_of<EOGSWidgetBase, WidgetType>::value, "WidgetType must derive from EOGSWidgetBase");
        WidgetType* newWidget = new WidgetType(std::forward<Args>(args)...);
        if(newWidget != nullptr && isContainer()) addChild(newWidget);
        return newWidget;
    }
    
    template<typename AnimType, typename... Args>
    [[nodiscard]] AnimType* createAnim(Args&&... args) {
        static_assert(std::is_base_of<EOGSAnimBase, AnimType>::value, "AnimType must be a valid animation type");
        AnimType* newAnim = new AnimType(static_cast<EOGSWidgetBase*>(this), std::forward<Args>(args)...);
        if (newAnim != nullptr) addAnimToEOGS(newAnim);
        return newAnim;
    }
    
private:
    // 辅助函数，用于将动画添加到EOGS实例中
    void addAnimToEOGS(void* anim);
};

// 基础控件类模板（使用CRTP实现链式调用的协变返回）
template<typename Derived>
class EOGSWidget : public EOGSWidgetBase {
public:
    EOGSWidget(float _x, float _y, float _w, float _h, bool _isRelative) : EOGSWidgetBase(_x, _y, _w, _h, _isRelative) {}
    virtual Derived* addChild(EOGSWidgetBase* child, bool isUpdateParent = true) { EOGSWidgetBase::addChild(child, isUpdateParent); return static_cast<Derived*>(this); }
    virtual Derived* removeChild(EOGSWidgetBase* child, bool isUpdateParent = true) { EOGSWidgetBase::removeChild(child, isUpdateParent); return static_cast<Derived*>(this); }
    virtual Derived* setPosition(float _x, float _y) { EOGSWidgetBase::setPosition(_x, _y); return static_cast<Derived*>(this); }
    virtual Derived* setPosition(float _x, float _y, bool _isXRel, bool _isYRel) { EOGSWidgetBase::setPosition(_x, _y, _isXRel, _isYRel); return static_cast<Derived*>(this); }
    virtual Derived* setPosition(float _x, float _y, bool _isXYRel) { EOGSWidgetBase::setPosition(_x, _y, _isXYRel); return static_cast<Derived*>(this); }
    virtual Derived* setX(float _x, bool _isRelative) { EOGSWidgetBase::setX(_x, _isRelative); return static_cast<Derived*>(this); }
    virtual Derived* setY(float _y, bool _isRelative) { EOGSWidgetBase::setY(_y, _isRelative); return static_cast<Derived*>(this); }
    virtual Derived* setSize(float _w, float _h) { EOGSWidgetBase::setSize(_w, _h); return static_cast<Derived*>(this); }
    virtual Derived* setSize(float _w, float _h, bool _isWRel, bool _isHRel) { EOGSWidgetBase::setSize(_w, _h, _isWRel, _isHRel); return static_cast<Derived*>(this); }
    virtual Derived* setSize(float _w, float _h, bool _isWHRel) { EOGSWidgetBase::setSize(_w, _h, _isWHRel); return static_cast<Derived*>(this); }
    virtual Derived* setW(float _w, bool _isRelative) { EOGSWidgetBase::setW(_w, _isRelative); return static_cast<Derived*>(this); }
    virtual Derived* setH(float _h, bool _isRelative) { EOGSWidgetBase::setH(_h, _isRelative); return static_cast<Derived*>(this); }
    virtual Derived* setParent(EOGSWidgetBase* _parent, bool isUpdateChild = true) { EOGSWidgetBase::setParent(_parent, isUpdateChild); return static_cast<Derived*>(this); }
    virtual Derived* setVisible(bool _visible) { EOGSWidgetBase::setVisible(_visible); return static_cast<Derived*>(this); }
    virtual Derived* setForceRenderOutsideOfCanvas(bool forceRenderOutsideOfCanvas) { EOGSWidgetBase::setForceRenderOutsideOfCanvas(forceRenderOutsideOfCanvas); return static_cast<Derived*>(this); }
    // Chainable event API wrappers (covariant return type)
    virtual Derived* trigger(EOGSEvent* event, EOGSEventPropagate propagationMode = EOGSEventPropagate::None) { EOGSWidgetBase::trigger(event, propagationMode); return static_cast<Derived*>(this); }
    virtual Derived* trigger(EOGSEvent &event, EOGSEventPropagate propagationMode = EOGSEventPropagate::None) { EOGSWidgetBase::trigger(&event, propagationMode); return static_cast<Derived*>(this); }
    virtual Derived* off(const EOGSEventID event) { EOGSWidgetBase::off(event); return static_cast<Derived*>(this); }
    virtual Derived* off(const EOGSEventID event, uint32_t callbackId) { EOGSWidgetBase::off(event, callbackId); return static_cast<Derived*>(this); }
    virtual Derived* off() { EOGSWidgetBase::off(); return static_cast<Derived*>(this); }
};
