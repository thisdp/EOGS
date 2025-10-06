//
// Created by thisdp 2025/10/5.
// Event System for EOGS
//

#pragma once

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

// 前向声明
class EOGSEvent;
class EOGSWidgetBase;

// 使用 uint32_t 作为事件ID类型，支持动态分配
using EOGSEventID = uint32_t;
using EOGSEventListenerID = uint32_t;

// 事件传播模式枚举
enum class EOGSEventPropagate {
    None,     // 不传播事件
    Up,     // 向上传播事件
    Down,   // 向下传播事件
    Both    // 双向传播事件
};

// 事件监听器类
class EOGSEventListener {
public:
    static EOGSEventListenerID nextListenerId;  // 用于生成回调函数的唯一索引
    // 回调ID管理方法
    static EOGSEventListenerID getNextAvailableListenerId();  // 获取下一个可用的回调ID
public:
    std::function<void(EOGSEvent*)> callback;
    EOGSEventListenerID id : 24;  // 监听器的唯一ID  (24位 0~16777216) ，这么多应该用不完的
    uint32_t receivePropagate : 1;
    uint32_t priority : 7;   // 0~127
    
    EOGSEventListener(std::function<void(EOGSEvent*)> cb, bool prop, uint8_t prio);
};

// 事件类声明
class EOGSEvent {   //基础事件类
public: // 预定义事件
    static constexpr EOGSEventID Confirm = 0;           //EOGSEventConfirm
    static constexpr EOGSEventID ProgressChange = 1;
    static constexpr EOGSEventID EventEnd = 10;
    // 静态变量用于事件相关的ID分配
    static EOGSEventID nextEventId;  // 用于生成动态事件ID
    /*  结构:
        [事件ID] = {
            [对象ID] = [ 监听器指针向量 ],
        },
    */
    static std::map<EOGSEventID, std::map<uint32_t, std::vector<EOGSEventListener*>>> eventMap;  // 静态事件映射，按事件ID、对象ID和监听器指针向量存储（支持优先级）

    // 动态事件ID分配
    static EOGSEventID requestEventID();  // 请求一个新的动态事件ID
    
public:
    EOGSEventID eventId : 31;  // 事件ID (31位 0~2147483648)
    uint32_t cancelled : 1;
    EOGSWidgetBase *source;
    EOGSWidgetBase *self;
    EOGSEvent(EOGSEventID id);
    EOGSEvent(EOGSEventID id, EOGSWidgetBase *_source);
    void cancel();
    bool wasCancelled();
    void makeTrigger(EOGSWidgetBase *_source);
};

class EOGSEventConfirm : public EOGSEvent {   // 确认类事件
public:
    static constexpr EOGSEventID EventID = EOGSEvent::Confirm;
public:
    EOGSEventConfirm(EOGSWidgetBase *src = nullptr);
};

class EOGSEventProgressChange : public EOGSEvent {   //进度事件类
public:
    static constexpr EOGSEventID EventID = EOGSEvent::ProgressChange;
public:
    EOGSEventProgressChange(EOGSWidgetBase *src = nullptr);
    uint16_t oldProgress;
};