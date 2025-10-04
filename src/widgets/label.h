//
// Created by thisdp 2025/8/20.
// Label Widget for EOGS
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
//Flag 6
#define isUTF8 generalFlag6
//Flag 8~9
#define drawColor generalColor8
//Flag 10~11
#define hAlign generalHAlign10
//Flag 12~13
#define vAlign generalVAlign12
class EOGSLabel : public EOGSWidget<EOGSLabel> {
protected:
    std::string text;
    const unsigned char* font;
    int16_t textX, textY;
    uint16_t textW, textH;
    
public:
    // 简化构造函数，使用统一的相对标志
    EOGSLabel(float _x, float _y, float _w, float _h, bool _isRelative = false,const std::string& _text = "");

    ~EOGSLabel() = default;
    
    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    void requestTextAlignUpdate(bool instant = false);
    bool updateTextAlign();

    void requestTextUpdate(bool instant = false);
    bool updateText(EOGS* eogs);
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
// 文本颜色
    DrawColor getColor() const;
    EOGSLabel* setColor(DrawColor _drawColor);
// 裁剪
    bool getClip() const;
    EOGSLabel* setClip(bool _clip);
// 滚动
    bool getScroll() const;
    EOGSLabel* setScroll(bool _scroll);
// 文本
    const std::string& getText() const;
    EOGSLabel* setText(const uint32_t _numText);
    EOGSLabel* setText(const int32_t _numText);
    EOGSLabel* setText(const std::string& _text);
//UTF8标记
    bool getUTF8Enabled() const;
    EOGSLabel* setUTF8Enabled(bool enabled);
// 字体
    const unsigned char* getFont() const;
    EOGSLabel* setFont(const unsigned char* _font);
// 字体透明
    bool getFontTransparent() const;
    EOGSLabel* setFontTransparent(bool _isFontTransparent);
// 文本对齐
    HAlign getHAlign() const;
    EOGSLabel* setHAlign(HAlign align);

    VAlign getVAlign() const;
    EOGSLabel* setVAlign(VAlign align);
    
    
};

// Label容器控件类
class EOGSLabelContainer : public EOGSLabel {
//构造析构
public:
    EOGSLabelContainer(float _x, float _y, float _w, float _h, bool _isRelative = false, const std::string& _text = "");

    ~EOGSLabelContainer();

//容器实现
protected:
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
public:
    bool isContainer() const override;
    std::vector<EOGSWidgetBase*>* getChildren();

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
};

#undef textUpdated
#undef textAlignUpdated
#undef isFontTransparent
#undef scroll
#undef clip
#undef isUTF8
#undef drawColor
#undef hAlign
#undef vAlign