#ifndef EOGS_ANIM_H
#define EOGS_ANIM_H

#include <EOGS.h>
#include <functional>
class EOGS;
//--------------------------------------Easing
// 定义缓动函数类型
using EOGSEasing = std::function<float(float)>;

// 工厂函数声明
namespace EOGSBuiltInEasing{
    EOGSEasing Linear();
    EOGSEasing OutSine();
    EOGSEasing InSine();
    EOGSEasing InOutSine();
    EOGSEasing OutQuad();
    EOGSEasing InQuad();
    EOGSEasing InOutQuad();
    EOGSEasing OutBounce();
    EOGSEasing InBounce();
    EOGSEasing InOutBounce();
    EOGSEasing OutBack(float s = 1.70158f);
    EOGSEasing InBack(float s = 1.70158f);
    EOGSEasing InOutBack(float s = 1.70158f);
};

//--------------------------------------Animation Base Class
class EOGSAnimBase {
public:
    EOGSAnimBase(uint32_t time, EOGSEasing easingCallback);
    virtual ~EOGSAnimBase();

    void update(EOGS* eogs);
    float calculateProgress(uint32_t ms);
    void start();
    void stop();
    bool isRunning() const;
    void setDuration(uint32_t duration);
    uint32_t getDuration() const;
    void setLooped(bool loop);
    bool getLooped() const;
    void setEasing(EOGSEasing easingCallback);

    // 需要重载的
    virtual void onUpdate(float progress) {}
    float getProgress();

protected:
    EOGSEasing easing;
    uint32_t startTime;
    uint32_t duration;
    float progress;
    uint8_t startOnUpdate : 1;  // 请求启动动画，将在onUpdate中处理
    uint8_t stopOnUpdate : 1;   // 请求停止动画，将在onUpdate中处理
    uint8_t running : 1;
    uint8_t loop : 1;
    uint8_t reserved : 4;
};

// 支持链式调用的动画类模板（使用CRTP实现链式调用的协变返回）
template<typename Derived>
class EOGSAnim : public EOGSAnimBase {
public:
    EOGSAnim(uint32_t time, EOGSEasing easingCallback)
        : EOGSAnimBase(time, easingCallback) {}

    Derived* setDuration(uint32_t duration) { EOGSAnimBase::setDuration(duration); return static_cast<Derived*>(this); }
    Derived* setLooped(bool loop) { EOGSAnimBase::setLooped(loop); return static_cast<Derived*>(this); }
    Derived* start() { EOGSAnimBase::start(); return static_cast<Derived*>(this); }
    Derived* stop() { EOGSAnimBase::stop(); return static_cast<Derived*>(this); }

};

//--------------------------------------Animation Move Class
class EOGSAnimMove : public EOGSAnim<EOGSAnimMove> {
public:
    template<typename T1, typename T2, typename T3, typename T4>
    EOGSAnimMove(EOGSWidgetBase *eogsWidget, T1 xStart, T2 yStart, T3 xEnd, T4 yEnd, bool isXRel, bool isYRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnim<EOGSAnimMove>(time, easingCallback),
          widget(eogsWidget),
          startX(static_cast<float>(xStart)),
          startY(static_cast<float>(yStart)),
          endX(static_cast<float>(xEnd)),
          endY(static_cast<float>(yEnd)),
          isXRelative(isXRel),
          isYRelative(isYRel) {}

    template<typename T1, typename T2, typename T3, typename T4>
    EOGSAnimMove(EOGSWidgetBase *eogsWidget, T1 xStart, T2 yStart, T3 xEnd, T4 yEnd, bool isXYRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnimMove(eogsWidget, xStart, yStart, xEnd, yEnd, isXYRel, isXYRel, time, easingCallback) {}
        
    //如果你很懒，不想要填写起始位置
    template<typename T3, typename T4>
    EOGSAnimMove(EOGSWidgetBase *eogsWidget, T3 xEnd, T4 yEnd, bool isXRel, bool isYRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnim<EOGSAnimMove>(time, easingCallback),
          widget(eogsWidget),
          startX(eogsWidget->getX(isXRel)),
          startY(eogsWidget->getX(isYRel)),
          endX(static_cast<float>(xEnd)),
          endY(static_cast<float>(yEnd)),
          isXRelative(isXRel),
          isYRelative(isYRel) {}

    template<typename T3, typename T4>
    EOGSAnimMove(EOGSWidgetBase *eogsWidget, T3 xEnd, T4 yEnd, bool isXYRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnimMove(eogsWidget, xEnd, yEnd, isXYRel, isXYRel, time, easingCallback) {}

    EOGSAnimMove* setStartPosition(float startX, float startY);
    
    virtual void onUpdate(float progress) override;

private:
    EOGSWidgetBase* widget;
    float startX, startY;
    float endX, endY;
    uint8_t isXRelative : 1;
    uint8_t isYRelative : 1;
    uint8_t reserved : 6;
};

//--------------------------------------Animation Size Class
class EOGSAnimSize : public EOGSAnim<EOGSAnimSize> {
public:
    template<typename T1, typename T2, typename T3, typename T4>
    EOGSAnimSize(EOGSWidgetBase *eogsWidget, T1 wStart, T2 hStart, T3 wEnd, T4 hEnd, bool isWRel, bool isHRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnim<EOGSAnimSize>(time, easingCallback),
          widget(eogsWidget),
          startW(static_cast<float>(wStart)),
          startH(static_cast<float>(hStart)),
          endW(static_cast<float>(wEnd)),
          endH(static_cast<float>(hEnd)),
          isWRelative(isWRel),
          isHRelative(isHRel) {}

    template<typename T1, typename T2, typename T3, typename T4>
    EOGSAnimSize(EOGSWidgetBase *eogsWidget, T1 wStart, T2 hStart, T3 wEnd, T4 hEnd, bool isWHRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnimSize(eogsWidget, wStart, hStart, wEnd, hEnd, isWHRel, isWHRel, time, easingCallback) {}
        
    //如果你很懒，不想要填写起始大小
    template<typename T3, typename T4>
    EOGSAnimSize(EOGSWidgetBase *eogsWidget, T3 wEnd, T4 hEnd, bool isWRel, bool isHRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnim<EOGSAnimSize>(time, easingCallback),
          widget(eogsWidget),
          startW(eogsWidget->getW(isWRel)),
          startH(eogsWidget->getH(isHRel)),
          endW(static_cast<float>(wEnd)),
          endH(static_cast<float>(hEnd)),
          isWRelative(isWRel),
          isHRelative(isHRel) {}

    template<typename T3, typename T4>
    EOGSAnimSize(EOGSWidgetBase *eogsWidget, T3 wEnd, T4 hEnd, bool isWHRel, uint32_t time, EOGSEasing easingCallback = 0)
        : EOGSAnimSize(eogsWidget, wEnd, hEnd, isWHRel, isWHRel, time, easingCallback) {}

    virtual void onUpdate(float progress) override;

private:
    EOGSWidgetBase* widget;
    float startW, startH;
    float endW, endH;
    uint8_t isWRelative : 1;
    uint8_t isHRelative : 1;
    uint8_t reserved : 6;
};

#endif // EOGS_ANIM_H
