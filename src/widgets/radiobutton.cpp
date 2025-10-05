#include "radiobutton.h"

// Flag 1
#define isChecked generalFlag1
// Flag 4~5
#define outerCircleColor generalColor4
// Flag 6~7
#define innerCircleColor generalColor6

// 构造函数
EOGSRadioButton::EOGSRadioButton(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSWidget<EOGSRadioButton>(_x, _y, _w, _h, _isRelative),
      outerRadius(1), innerRadius(0.75),
      cx(0), cy(0),
      renderOuterRadius(0), renderInnerRadius(0),
      group(nullptr) {
    setOuterCircleColor(DrawColor::WHITE);
    setInnerCircleColor(DrawColor::WHITE);
    on(EOGSEvent::Click, onClick);
}

EOGSRadioButton::~EOGSRadioButton() {
    if (group == nullptr) return;
    // 从组中移除自身
    auto it = std::find(group->begin(), group->end(), this);
    if (it != group->end()) group->erase(it);
    // 如果当前按钮是组内选中的按钮，需要清除选中状态
    if (group->selectedButton == this){
        group->selectedButton = nullptr;
    }
}

// 更新控件尺寸和位置
bool EOGSRadioButton::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX + x;
    renderY = parentRY + y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    
    // 计算外圆半径
    renderOuterRadius = min(w, h)/2 * outerRadius;
    // 内圆半径为外圆的1/2
    renderInnerRadius = innerRadius * renderOuterRadius;
    // 圆心位于控件中心
    cx = renderX + w / 2;
    cy = renderY + h / 2;
    
    return true;
}

void EOGSRadioButton::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    
    updateDimension(parentW, parentH);
    updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);

    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        // 绘制外圆
        eogs->setDrawColor(outerCircleColor);
        eogs->drawCircle(cx, cy, renderOuterRadius);
        // 如果选中，绘制内圆
        if (isChecked) {
            eogs->setDrawColor(innerCircleColor);
            eogs->drawDisc(cx, cy, renderInnerRadius);
        }
    }
}

// 选中状态
bool EOGSRadioButton::getChecked() const { return isChecked; }
EOGSRadioButton* EOGSRadioButton::setChecked(bool checked) {
    if(group == nullptr){
        isChecked = checked;
        return this;
    }
    if (group->selectedButton != nullptr){
        group->selectedButton->isChecked = false;
        group->selectedButton = nullptr;
    }
    if(checked){
        group->selectedButton = this;
        isChecked = checked;
    }
    return this;
}

EOGSRadioButtonGroup* EOGSRadioButton::getGroup() const { return group; }
EOGSRadioButton* EOGSRadioButton::setGroup(EOGSRadioButtonGroup& _group) {
    setGroup(&_group);
    return this;
}
EOGSRadioButton* EOGSRadioButton::setGroup(EOGSRadioButtonGroup* _group) {
    if (group) {
        auto it = std::find(group->begin(), group->end(), this);
        if (it != group->end()) group->erase(it);
        if (group->selectedButton == this){
            group->selectedButton->isChecked = false;
            group->selectedButton = nullptr;
        }
    }
    group = _group;
    if (group) group->push_back(this);
    return this;
}

// 颜色设置
DrawColor EOGSRadioButton::getOuterCircleColor() const { return outerCircleColor; }
EOGSRadioButton* EOGSRadioButton::setOuterCircleColor(DrawColor color) {
    outerCircleColor = color;
    return this;
}

DrawColor EOGSRadioButton::getInnerCircleColor() const { return innerCircleColor; }
EOGSRadioButton* EOGSRadioButton::setInnerCircleColor(DrawColor color) {
    innerCircleColor = color;
    return this;
}

// 获取圆形半径
int16_t EOGSRadioButton::getCircleRadius() const { return static_cast<int16_t>(outerRadius); }
int16_t EOGSRadioButton::getInnerCircleRadius() const { return static_cast<int16_t>(innerRadius); }

// 静态函数
void EOGSRadioButton::onClick(EOGSEvent* event){  //EOGSRadioButton
    EOGSRadioButton *switchButton = static_cast<EOGSRadioButton*>(event->self);
    switchButton->setChecked(true);
}

#undef isChecked
#undef outerCircleColor
#undef innerCircleColor