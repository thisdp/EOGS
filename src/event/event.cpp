#include "event.h"
#include "../widgets/widget.h"
#include <algorithm>

// EOGSEvent类的静态变量初始化
EOGSEventListenerID EOGSEventListener::nextListenerId = 0; // 用于生成回调ID
EOGSEventID EOGSEvent::nextEventId = 20;    // 用于生成事件ID

// 事件系统实现
// 使用 vector 保持 EOGSEventListener* 的有序集合，以支持优先级
std::map<EOGSEventID, std::map<uint32_t, std::vector<EOGSEventListener*>>> EOGSEvent::eventMap;  // 静态事件映射，按事件ID、对象ID和监听器指针向量存储（支持优先级）

// 事件监听器系统
EOGSEventListener::EOGSEventListener(std::function<void(EOGSEvent*)> cb, bool prop, uint8_t prio)
    : callback(cb), receivePropagate(prop), priority(prio) {
        id = getNextAvailableListenerId();
    }

// 回调ID管理实现
EOGSEventListenerID EOGSEventListener::getNextAvailableListenerId() {
    if(nextListenerId == 0) nextListenerId ++; // 0 为特殊值
    EOGSEventListenerID currentId = nextListenerId;
    // 循环查找可用的ID
    do {
        // 检查当前ID是否已被占用
        bool isIdInUse = false;
        for (const auto& eventEntry : EOGSEvent::eventMap) {
            for (const auto& objectEntry : eventEntry.second) {
                for (const auto* listener : objectEntry.second) {
                    if (listener->id == currentId) {
                        isIdInUse = true;
                        break;
                    }
                }
                if (isIdInUse) break;
            }
            if (isIdInUse) break;
        }
        
        // 如果当前ID未被占用，则使用它
        if (!isIdInUse) {
            nextListenerId = currentId + 1;  // 更新nextListenerId
            return currentId;
        }
        
        // 否则尝试下一个ID
        currentId++;
        currentId &= 0x00FFFFFF; //24位
        
        // 防止无限循环（虽然理论上不应该发生）
        if (currentId == nextListenerId) { // 所有ID都被占用了，这是一个极端情况
            return 0;  // 0表示无法分配ID
        }
        
    } while (true);
}

//事件系统
EOGSEvent::EOGSEvent(EOGSEventID id) : eventId(id), cancelled(false) , source(nullptr), self(nullptr){}

void EOGSEvent::cancel() {
    cancelled = true;
}

bool EOGSEvent::wasCancelled() {
    return cancelled;
}

// 动态事件ID分配实现
EOGSEventID EOGSEvent::requestEventID() {
    return nextEventId++;
}