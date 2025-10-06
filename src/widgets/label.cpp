#include "label.h"

//Flag 1
#define textUpdated generalFlag1
//Flag 2
#define textAlignUpdated generalFlag2
//Flag 3
#define isFontTransparent generalFlag3
//Flag 4
#define scroll generalFlag4
//Flag 5
#define clip generalFlag5
//Flag 6
#define isUTF8 generalFlag6
//Flag 8~9
#define drawColor generalColor8
//Flag 10~11
#define hAlign generalHAlign10
//Flag 12~13
#define vAlign generalVAlign12

// 简化构造函数，使用统一的相对标志
EOGSLabel::EOGSLabel(float _x, float _y, float _w, float _h, bool _isRelative, const std::string& _text)
    : EOGSWidget<EOGSLabel>(_x, _y, _w, _h, _isRelative),
      text(_text), font(nullptr){
        setFontTransparent(true);
        setHAlign(HAlign::LEFT);
        setVAlign(VAlign::TOP);
        setScroll(false);   // 当字体超出边界后默认不滚动
        setClip(false); // 默认不剪裁
        setColor(DrawColor::WHITE);
        setUTF8Enabled(true);   //UTF8 默认启用，并且不允许强制更新文本内容
    }

// 更新控件尺寸和位置
bool EOGSLabel::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;

    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    requestTextAlignUpdate();  // 位置改变后需要重新计算文本位置
    return true;
}

void EOGSLabel::requestTextAlignUpdate(bool instant) {
    textAlignUpdated = false;
    if(!instant) return;
    updateTextAlign();
}
bool EOGSLabel::updateTextAlign(){
    if (textAlignUpdated) return false;
    textAlignUpdated = true;

    switch (hAlign) {
        case HAlign::CENTER:    textX = static_cast<int16_t>(renderX + (w - textW) / 2);  break;
        case HAlign::RIGHT:     textX = static_cast<int16_t>(renderX + w - textW);        break;
        case HAlign::LEFT:
        default:                textX = static_cast<int16_t>(renderX);                        break;
    }
    switch (vAlign) {   //文本默认是左上角为基准
        case VAlign::CENTER:    textY = static_cast<int16_t>(renderY + (h - textH) / 2); break;
        case VAlign::BOTTOM:    textY = static_cast<int16_t>(renderY + h - textH);       break;
        case VAlign::TOP:
        default:                textY = static_cast<int16_t>(renderY);             break;
    }
    return true;
}

void EOGSLabel::requestTextUpdate(bool instant) {
    textUpdated = false;
    if(!instant) return;
    updateText(getEOGS());
}
bool EOGSLabel::updateText(EOGS* eogs) {
    if (textUpdated) return false;
    textUpdated = true;  // 标记文本已更新

    eogs->setFont(font == nullptr ? eogs->getDefaultFont() : font);
    textW = isUTF8?eogs->getUTF8FontWidth(text):eogs->getFontWidth(text);
    textH = eogs->getFontHeight();
    requestTextAlignUpdate();  // 位置改变后需要重新计算文本位置
    return true;
}

void EOGSLabel::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    updateDimension(parentW, parentH);
    updateText(eogs);
    updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    updateTextAlign();
    if(insideOfRenderArea || renderOutsideOfRenderArea){
        if (!text.empty()){
            eogs->setFont(font == nullptr ? eogs->getDefaultFont() : font);
            eogs->setDrawColor(drawColor);
            eogs->setFontMode(isFontTransparent);
            if(clip) eogs->setClipWindow(x, y, x+w, y+h);   // 由于使用率不为100%，不缓存
            if(!scroll){
                if(isUTF8) eogs->drawUTF8Text(textX, textY, text);
                else eogs->drawText(textX, textY, text);
            }else{
                if(textW <= w){
                    if(isUTF8) eogs->drawUTF8Text(textX, textY, text);
                    else eogs->drawText(textX, textY, text);
                }else{
                    int16_t diff = textW - w;
                    int16_t offset = ((eogs->millis()/100)%(diff+10)) - 5;
                    offset = mathClamp(offset, 0, diff);
                    if(isUTF8) eogs->drawUTF8Text(textX, textY, text);
                    else eogs->drawText(textX, textY, text);
                }
            }
            if(clip) eogs->setMaxClipWindow();
        }
    }
}
// 文本颜色
DrawColor EOGSLabel::getColor() const { return drawColor; }
EOGSLabel* EOGSLabel::setColor(DrawColor _drawColor){
    drawColor = _drawColor;
    return this;
}
// 裁剪
bool EOGSLabel::getClip() const { return clip; }
EOGSLabel* EOGSLabel::setClip(bool _clip){
    clip = _clip;
    return this;
}
// 滚动
bool EOGSLabel::getScroll() const { return scroll; }
EOGSLabel* EOGSLabel::setScroll(bool _scroll){
    scroll = _scroll;
    return this;
}
// 文本
const std::string& EOGSLabel::getText() const { return text; }
EOGSLabel* EOGSLabel::setText(const uint32_t _numText){
    text = std::to_string(_numText);
    return this;
}
EOGSLabel* EOGSLabel::setText(const int32_t _numText){
    text = std::to_string(_numText);
    return this;
}
EOGSLabel* EOGSLabel::setText(const std::string& _text) {
    text = _text;
    requestTextUpdate();  // 文本改变后需要重新计算文本大小
    return this;
}
//UTF8标记
bool EOGSLabel::getUTF8Enabled() const { return isUTF8; }
EOGSLabel* EOGSLabel::setUTF8Enabled(bool enabled) { 
    isUTF8 = enabled;
    requestTextUpdate();  // UTF8标记改变后需要重新计算文本大小
    return this;
}
// 字体
const unsigned char* EOGSLabel::getFont() const { return font; }
EOGSLabel* EOGSLabel::setFont(const unsigned char* _font) {
    font = _font;
    requestTextUpdate();  // 字体改变后需要重新计算文本大小
    return this;
}
// 字体透明
bool EOGSLabel::getFontTransparent() const { return isFontTransparent; }
EOGSLabel* EOGSLabel::setFontTransparent(bool _isFontTransparent) {
    isFontTransparent = static_cast<uint8_t>(_isFontTransparent);
    return this;
}
// 文本对齐
HAlign EOGSLabel::getHAlign() const { return hAlign; }
EOGSLabel* EOGSLabel::setHAlign(HAlign align) {
    hAlign = align;
    return this;
}

VAlign EOGSLabel::getVAlign() const { return vAlign; }
EOGSLabel* EOGSLabel::setVAlign(VAlign align) {
    vAlign = align;
    return this;
}

// Label容器控件类
//构造析构
EOGSLabelContainer::EOGSLabelContainer(float _x, float _y, float _w, float _h, bool _isRelative, const std::string& _text)
    : EOGSLabel(_x, _y, _w, _h, _isRelative, _text) {
}

EOGSLabelContainer::~EOGSLabelContainer() {
    std::vector<EOGSWidgetBase*>* children = getChildren();
    for (EOGSWidgetBase* child : *children) {
        if (child) delete child;
    }
    children->clear();
}

//容器实现
bool EOGSLabelContainer::isContainer() const { return true; }
std::vector<EOGSWidgetBase*>* EOGSLabelContainer::getChildren() { return &children; }

void EOGSLabelContainer::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    bool wasDimensionUpdate = !dimensionUpdated;
    EOGSLabel::render(eogs, parentRX, parentRY, parentW, parentH);
    renderChildren(eogs,wasDimensionUpdate);
}


#undef textUpdated
#undef textAlignUpdated
#undef isFontTransparent
#undef scroll
#undef clip
#undef isUTF8
#undef drawColor
#undef hAlign
#undef vAlign