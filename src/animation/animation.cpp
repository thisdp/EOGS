#include "animation.h"
#include <cmath>
#include <Arduino.h>

//--------------------------------------Easing
// 工厂函数实现
namespace EOGSBuiltInEasing{
EOGSEasing Linear() {
    return [](float t) { return t; };
}

EOGSEasing OutSine() {
    return [](float t) { return sinf(M_PI_2 * t); };
}

EOGSEasing InSine() {
    return [](float t) { return 1.0f - cosf(M_PI_2 * t); };
}

EOGSEasing InOutSine() {
    return [](float t) { return -0.5f * (cosf(M_PI * t) - 1.0f); };
}

EOGSEasing OutQuad() {
    return [](float t) { return t * (2.0f - t); };
}

EOGSEasing InQuad() {
    return [](float t) { return t * t; };
}

EOGSEasing InOutQuad() {
    return [](float t) {
        return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
    };
}

EOGSEasing OutBounce() {
    return [](float t) {
        const float div1 = 1.0f / 2.75f;
        const float div2 = 2.0f / 2.75f;
        const float div3 = 2.5f / 2.75f;

        if (t < div1) {
            return 7.5625f * t * t;
        } else if (t < div2) {
            t -= 1.5f / 2.75f;
            return 7.5625f * t * t + 0.75f;
        } else if (t < div3) {
            t -= 2.25f / 2.75f;
            return 7.5625f * t * t + 0.9375f;
        } else {
            t -= 2.625f / 2.75f;
            return 7.5625f * t * t + 0.984375f;
        }
    };
}

EOGSEasing InBounce() {
    return [](float t) { return 1.0f - OutBounce()(1.0f - t); };
}

EOGSEasing InOutBounce() {
    return [](float t) {
        if (t < 0.5f) return InBounce()(t * 2.0f) * 0.5f;
        return OutBounce()(t * 2.0f - 1.0f) * 0.5f + 0.5f;
    };
}

EOGSEasing OutBack(float s) {
    return [s](float t) {
        t -= 1.0f;
        return t * t * ((s + 1.0f) * t + s) + 1.0f;
    };
}

EOGSEasing InBack(float s) {
    return [s](float t) { return t * t * ((s + 1.0f) * t - s); };
}

EOGSEasing InOutBack(float s) {
    return [s](float t) {
        if (t < 0.5f) return InBack(s)(t * 2.0f) * 0.5f;
        return OutBack(s)(t * 2.0f - 1.0f) * 0.5f + 0.5f;
    };
}
};

//--------------------------------------Animation Base Implement
EOGSAnimBase::EOGSAnimBase(uint32_t time, EOGSEasing easingCallback)
    : easing(easingCallback),
      startTime(0),
      duration(time),
      progress(0.0f),
      startOnUpdate(0),
      stopOnUpdate(0),
      running(false),
      loop(false),
      reserved(0) {}

EOGSAnimBase::~EOGSAnimBase() {}

void EOGSAnimBase::update(EOGS* eogs) {
    uint32_t ms = eogs->millis();
    if (startOnUpdate) {
        startTime = ms;
        running = true;
        startOnUpdate = false;
    }
    if (!running) return;
    float progress = calculateProgress(ms);
    onUpdate(progress);
    if (stopOnUpdate) {
        running = false;
        stopOnUpdate = false;
    }
}

float EOGSAnimBase::calculateProgress(uint32_t ms) {
    if (!running) return 0;
    uint32_t elapsed = ms - startTime;
    if (elapsed >= duration || duration == 0) {
        if (loop) start(); else stop();
        progress = 1.0f;  // Set progress to 1.0 when animation ends
    } else {
        progress = static_cast<float>(elapsed) / static_cast<float>(duration);
    }
    progress = easing ? easing(progress) : progress;  // Apply easing if available
    return progress;
}

float EOGSAnimBase::getProgress() {
    return progress;
}

void EOGSAnimBase::start() { startOnUpdate = true; stopOnUpdate = false; progress = 0; }
void EOGSAnimBase::stop() { startOnUpdate = false; stopOnUpdate = true; }
bool EOGSAnimBase::isRunning() const { return running; }
void EOGSAnimBase::setDuration(uint32_t duration) { this->duration = duration; }
uint32_t EOGSAnimBase::getDuration() const { return duration; }
void EOGSAnimBase::setLooped(bool loop) { this->loop = loop; }
bool EOGSAnimBase::getLooped() const { return loop; }

void EOGSAnimBase::setEasing(EOGSEasing easingCallback){
    this->easing = easingCallback;
}

//--------------------------------------Animation Move Implement
void EOGSAnimMove::onUpdate(float progress) {
    float currentX = startX + (endX - startX) * progress;
    float currentY = startY + (endY - startY) * progress;
    widget->setPosition(currentX, currentY, isXRelative, isYRelative);
}
EOGSAnimMove* EOGSAnimMove::setStartPosition(float _startX, float _startY) {
    startX = _startX, startY = _startY;
    return this;
}
//--------------------------------------Animation Size Implement
void EOGSAnimSize::onUpdate(float progress) {
    float currentW = startW + (endW - startW) * progress;
    float currentH = startH + (endH - startH) * progress;
    widget->setSize(currentW, currentH, isWRelative, isHRelative);
}
