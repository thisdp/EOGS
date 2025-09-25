//
// Created by Roo 2025/9/19.
// EOGS Input Controller - A lightweight input handling library for embedded systems
//

#pragma once

#include "eogs_hal/eogs_hal.h"
#include <Arduino.h>
#include <vector>
#include <functional>

template <typename T>
class EOGSAnalogHysteresis {
    T lowerBound;
    T upperBound;
    bool state;
public:
    EOGSAnalogHysteresis(T _lowerBound, T _upperBound, bool _state = false) : lowerBound(_lowerBound), upperBound(_upperBound), state(_state) {}

    void update(T value) {
        if(value <= lowerBound){
            if(state) state = false;
        }else if(value >= upperBound){
            if(!state) state = true;
        }
    }

    bool getState() const { return state; }
    void setState(bool _state) { state = _state; }
    void setLowerBound(T lowBound) { lowerBound = lowBound; }
    void setUpperBound(T upBound) { upperBound = upBound; }
    T getLowerBound() const { return lowerBound; }
    T getUpperBound() const { return upperBound; }
};

class EOGSDigitalInput{
public:
    constexpr static uint8_t STABLE = 0;
    constexpr static uint8_t WAITING = 1;
protected:
    EOGS_MCU *hal;
    uint32_t previousTime;
    uint16_t filterInterval;
    uint8_t stateMachine;
    bool state;
    virtual void onStateChange() {}
public:
    EOGSDigitalInput(EOGS_MCU& eogsHAL, uint16_t _filterInterval = 10)
    :   hal(&eogsHAL),
        previousTime(0),
        filterInterval(_filterInterval),
        stateMachine(STABLE),
        state(false){}
    void update(bool inputState){
        if(hal == nullptr) return;
        if(stateMachine == STABLE){
            if(inputState != state){    //开始检测
                stateMachine = WAITING;
                previousTime = hal->millis();
            }
        }else{
            if(inputState == state){    //如果状态相同
                stateMachine = STABLE;  //恢复状态
            }else{  //如果状态不同
                if(hal->millis()-previousTime >= static_cast<uint16_t>(filterInterval)){ //并且保持到间隔时间
                    state = inputState;
                    stateMachine = STABLE;  //稳定
                    onStateChange();
                }
            }
        }
    }
    bool getState() const { return state; }
    void setState(bool _state) { state = _state; }
    void setFilterInterval(uint16_t _interval) { filterInterval = _interval; }
    uint16_t getFilterInterval() const { return filterInterval; }
};

class EOGSDigitalButton : public EOGSDigitalInput {
public: //事件
    using EventOnPress = void(*)(EOGSDigitalButton* theButton, uint16_t pressTimes);
    using EventOnRelease = void(*)(EOGSDigitalButton* theButton, uint16_t releaseTimes);
    using EventOnHold = void(*)(EOGSDigitalButton* theButton);
    using EventOnHolding = void(*)(EOGSDigitalButton* theButton, uint16_t holdingTime);

protected:
    uint32_t pressStartTime;// 按下开始时间
    uint16_t clickCount;    // 连续点击次数
    uint16_t clickInterval; // 点击间隔
    uint16_t holdThreshold;   // 按住持续时间
    uint8_t holdTriggered : 1;
    uint8_t reserved:7;
public:
    EOGSDigitalButton(EOGS_MCU &eogsHAL, uint16_t _clickInterval = 500, uint16_t _holdThreshold = 1000)
        : EOGSDigitalInput(eogsHAL),
          pressStartTime(0),
          clickCount(0),
          clickInterval(_clickInterval),
          holdThreshold(_holdThreshold),
          holdTriggered(false),
          reserved(0) {}
    void update(bool inputState) {
        if(hal == nullptr) return;
        EOGSDigitalInput::update(inputState);
        if(state){
            if(!holdTriggered){
                uint32_t ms = hal->millis();
                if(ms - pressStartTime >= holdThreshold){
                    holdTriggered = true;
                    if(onHold) onHold(this);
                }
            }
            if(holdTriggered){
                if(onHolding){
                    uint32_t ms = hal->millis();
                    onHolding(this,ms-pressStartTime);
                }
            }
        }
    }
    void clear() {  //清除状态
        clickCount = 0;
        pressStartTime = 0;
        holdTriggered = false;
    }
    uint16_t getholdThreshold() const { return holdThreshold; }
    void setholdThreshold(uint16_t _holdThreshold) {
        holdThreshold = _holdThreshold;
    }
    uint16_t getClickInterval() const { return clickInterval; }
    void setClickInterval(uint16_t _clickInterval) {
        clickInterval = _clickInterval;
    }
    void onStateChange() override{
        if(hal == nullptr) return;
        holdTriggered = false;
        if(getState()){ //当按下
            uint32_t ms = hal->millis();
            if(ms - pressStartTime <= clickInterval){ // 如果持续时间未超过clickInterval
                clickCount++;
            }else{  // 否则
                clickCount = 1;
            }
            pressStartTime = ms;
            if(onPress) onPress(this, clickCount);
        }else{
            if(onRelease) onRelease(this,1);
        }
    }
    bool isPressed() { return getState(); }
    bool isReleased() { return !getState(); }

    EventOnPress onPress = nullptr;
    EventOnRelease onRelease = nullptr;
    EventOnHold onHold = nullptr;
    EventOnHolding onHolding = nullptr;

};

