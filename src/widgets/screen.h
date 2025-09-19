//
// Created by thisdp 2025/8/21.
//
#pragma once

#include "widget.h"

// 屏幕，仅使用绝对大小和坐标
class EOGSScreen : public EOGSWidget<EOGSScreen> {
//构造析构
public:
    EOGSScreen(float _x = 0, float _y = 0, float _w = 128, float _h = 64)
        : EOGSWidget<EOGSScreen>(_x, _y, _w, _h, false){}

    ~EOGSScreen() {
        std::vector<EOGSWidgetBase*>* children = getChildren();
        for (EOGSWidgetBase* child : *children) {
            if (child) delete child;
        }
        children->clear();
    }
    EOGS *getEOGS() { return eogs; }
    void setEOGS(EOGS *_eogs) { eogs = _eogs; }
private:
    EOGS *eogs;
//容器实现
protected:
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
public:
    bool isContainer() const override { return true; }
    std::vector<EOGSWidgetBase*>* getChildren() override{ return &children; }
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override { renderChildren(eogs); }
};
