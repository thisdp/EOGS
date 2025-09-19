//
// Created by thisdp 2025/8/20.
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

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
//Flag 6~7
#define drawColor generalColor6
//Flag 8~9
#define hAlign generalHAlign8
//Flag 10~11
#define vAlign generalVAlign10
class EOGSLabel : public EOGSWidget<EOGSLabel> {
protected:
    std::string text;
    const unsigned char* font;
    int16_t textX, textY;
    uint16_t textW, textH;
    
public:
    // 简化构造函数，使用统一的相对标志
    EOGSLabel(float _x, float _y, float _w, float _h, bool _isRelative = false,const std::string& _text = "")
        : EOGSWidget<EOGSLabel>(_x, _y, _w, _h, _isRelative),
          text(_text), font(u8g2_font_t0_11_tf){
            setFontTransparent(true);
            setHAlign(HAlign::LEFT);
            setVAlign(VAlign::TOP);
            setScroll(false);   // 当字体超出边界后默认不滚动
            setClip(false); // 默认不剪裁
            setColor(DrawColor::WHITE);
        }

    ~EOGSLabel() = default;
    
    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false; renderPosUpdated = true;

        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;

        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        textAlignUpdated = false;  // 位置改变后需要重新计算文本位置
        return true;
    }

    bool updateTextAlign(){
        if (textAlignUpdated) return false; textAlignUpdated = true;
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

    bool updateText(EOGS* eogs) {
        if (textUpdated) return false; textUpdated = true;  // 标记文本已更新
        if (font != nullptr) eogs->setFont(font);
        textW = eogs->getFontWidth(text);
        textH = eogs->getFontHeight();
        textAlignUpdated = false;  // 位置改变后需要重新计算文本位置
        return true;
    }
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        updateDimension(parentW, parentH);
        updateText(eogs);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateTextAlign();
        if(insideOfRenderArea || renderOutsideOfRenderArea){
            if (!text.empty()){
                if (font != nullptr) eogs->setFont(font);
                eogs->setDrawColor(drawColor);
                eogs->setFontMode(isFontTransparent);
                if(clip) eogs->setClipWindow(x, y, x+w, y+h);   // 由于使用率不为100%，不缓存
                if(!scroll){
                    eogs->drawText(textX, textY, text);
                }else{
                    if(textW <= w){
                        eogs->drawText(textX, textY, text);
                    }else{
                        int16_t diff = textW - w;
                        int16_t offset = ((eogs->millis()/100)%(diff+10)) - 5;
                        offset = mathClamp(offset, 0, diff);
                        eogs->drawText(textX-offset, textY, text);
                    }
                }
                if(clip) eogs->setMaxClipWindow();
            }
        }
    }
// 文本颜色
    DrawColor getColor() const { return drawColor; }
    EOGSLabel* setColor(DrawColor _drawColor){
        drawColor = _drawColor;
        return this;
    }
// 裁剪
    bool getClip() const { return clip; }
    EOGSLabel* setClip(bool _clip){
        clip = _clip;
        return this;
    }
// 滚动
    bool getScroll() const { return scroll; }
    EOGSLabel* setScroll(bool _scroll){
        scroll = _scroll;
        return this;
    }
// 文本
    const std::string& getText() const { return text; }
    EOGSLabel* setText(const std::string& _text) {
        text = _text;
        textUpdated = false;  // 文本改变后需要重新计算文本大小
        return this;
    }
// 字体
    const unsigned char* getFont() const { return font; }
    EOGSLabel* setFont(const unsigned char* _font) {
        font = _font;
        textUpdated = false;  // 字体改变后需要重新计算文本大小
        return this;
    }
// 字体透明
    bool getFontTransparent() const { return isFontTransparent; }
    EOGSLabel* setFontTransparent(bool _isFontTransparent) {
        isFontTransparent = static_cast<uint8_t>(_isFontTransparent);
        return this;
    }
// 文本对齐
    HAlign getHAlign() const { return hAlign; }
    EOGSLabel* setHAlign(HAlign align) {
        hAlign = align;
        return this;
    }

    VAlign getVAlign() const { return vAlign; }
    EOGSLabel* setVAlign(VAlign align) {
        vAlign = align;
        return this;
    }
    
    
};

// Label容器控件类
class EOGSLabelContainer : public EOGSLabel {
//构造析构
public:
    EOGSLabelContainer(float _x, float _y, float _w, float _h, bool _isRelative = false, const std::string& _text = "")
        : EOGSLabel(_x, _y, _w, _h, _isRelative, _text) {
    }
    
    ~EOGSLabelContainer() {
        std::vector<EOGSWidgetBase*>* children = getChildren();
        for (EOGSWidgetBase* child : *children) {
            if (child) delete child;
        }
        children->clear();
    }

//容器实现
protected:
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
public:
    bool isContainer() const override { return true; }
    std::vector<EOGSWidgetBase*>* getChildren() { return &children; }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        bool wasDimensionUpdate = !dimensionUpdated;
        EOGSLabel::render(eogs, parentRX, parentRY, parentW, parentH);
        renderChildren(eogs,wasDimensionUpdate);
    }
};
#undef textUpdated
#undef textAlignUpdated
#undef isFontTransparent
#undef scroll
#undef clip
#undef drawColor
#undef hAlign
#undef vAlign