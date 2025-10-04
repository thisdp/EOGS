#include "screen.h"

// 构造函数
EOGSScreen::EOGSScreen(float _x, float _y, float _w, float _h)
    : EOGSWidget<EOGSScreen>(_x, _y, _w, _h, false) {}

// 析构函数
EOGSScreen::~EOGSScreen() {
    std::vector<EOGSWidgetBase*>* children = getChildren();
    for (EOGSWidgetBase* child : *children) {
        if (child) delete child;
    }
    children->clear();
}

EOGS* EOGSScreen::getEOGS() { return eogs; }
void EOGSScreen::setEOGS(EOGS* _eogs) { eogs = _eogs; }

// 容器实现
bool EOGSScreen::isContainer() const { return true; }
std::vector<EOGSWidgetBase*>* EOGSScreen::getChildren() { return &children; }
void EOGSScreen::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) { 
    renderChildren(eogs); 
}