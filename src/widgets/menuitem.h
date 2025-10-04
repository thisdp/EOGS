//
// Created by thisdp 2025/8/26.
// Menu Iem Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include <vector>
#include <string>
#include <algorithm>

//Flag 1
#define viewUpdated generalFlag1
//Flag 2
#define childPositionUpdated generalFlag2
//Flag 3
#define imageAlignUpdated generalFlag3
//Flag 4
#define textAlignUpdated generalFlag4
//Flag 5
#define selectAreaUpdated generalFlag5
//Flag 6~7
#define textAlign generalAlign6
//Flag 8~9
#define imageAlign generalAlign8
//Flag 10
#define textUpdated generalFlag10
//Flag 11
#define isFontTransparent generalFlag11
//Flag 12
#define isSelected generalFlag12
//Flag 13
#define scroll generalFlag13
//Flag 14
#define clip generalFlag14
//Flag 15
#define isUTF8 generalFlag15
//Flag 16
#define isEnterable generalFlag16
//Flag 17
#define isSelectable generalFlag17

class EOGSMenuSubItem;
class EOGSMenuItem : public EOGSWidget<EOGSMenuItem> {
protected:
    std::vector<EOGSMenuItem*> childMenuItems; // 子菜单项列表（菜单系统中的childrenItem）
    std::vector<EOGSWidgetBase*> children;  // 子级元素列表
    //菜单上下文
    int16_t selectedIndex;      // 当前选中的项索引，动画由menu完成
    int16_t previousSelectedIndex;
    int16_t topViewIndex;       // 顶部可见项索引（用于滚动）
    int16_t bottomViewIndex;    // 底部可见项索引（用于滚动）
    int16_t viewOffset;       // 滚动偏移量
    int16_t paddingLeftOrTop, paddingRightOrBottom;    // 左右/上下边距
    //颜色 (内存对齐)
    DrawColor textColor;
    DrawColor selectedTextColor;
    DrawColor imageColor;
    DrawColor selectedImageColor;
public:
    // 构造函数
    EOGSMenuItem(float _x, float _y, float _w, float _h, bool _isRelative = false);

    // 析构函数
    ~EOGSMenuItem();

    // 重写基类方法，明确标记为Container
    bool isContainer() const override;

    std::vector<EOGSWidgetBase*>* getChildren() override;

    virtual EOGSMenuItem* setSelected(bool selected);
    virtual bool getSelected() const;
    virtual int16_t getSelectedX() const;
    virtual int16_t getSelectedW() const;
    virtual bool getEnterable() const;
    virtual bool getSelectable() const;

    virtual int16_t getMenuHeight();

    virtual int16_t getDefaultItemHeight();

    virtual void requestTextUpdate(bool instant = false);
    virtual bool updateText(EOGS* eogs);
    virtual void requestTextAlignUpdate(bool instant = false);
    virtual bool updateTextAlign();
    virtual void requestImageAlignUpdate(bool instant = false);
    virtual bool updateImageAlign();
    virtual void requestSelectAreaUpdate(bool instant = false);
    virtual bool updateSelectArea();
    virtual void requestViewRangeUpdate();
    virtual bool updateViewRange();
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    //事件
    virtual void onChildrenHeightUpdate();
    void requestChildrenHeightUpdate();
    bool updateChildrenHeight();

    int16_t getTotalChildrenHeight() const;

// 根菜单
    virtual EOGSMenuItem* getRootMenuItem();
// 父级菜单
    virtual EOGSMenuItem* getParentMenuItem();
    virtual void setParentMenuItem(EOGSMenuItem* _parentMenuItem, bool isUpdateChildItem = true);
    virtual bool isParentMenuItem(EOGSMenuItem* item);
// 子菜单
    const std::vector<EOGSMenuItem*>* getChildMenuItems();
    void addChildMenuItem(EOGSMenuItem* child, bool isUpdateParentItem = true);
    void removeChildMenuItem(EOGSMenuItem* child, bool isUpdateParentItem = true);
// 边距
    int16_t getPaddingLeft() const;
    EOGSMenuItem* setPaddingLeft(int16_t padding);
    int16_t getPaddingRight() const;
    EOGSMenuItem* setPaddingRight(int16_t padding);
    EOGSMenuItem* setPadding(int16_t left, int16_t right);
// 选中子项
    int16_t getPreviousSelectedSubItemIndex() const;
    EOGSMenuItem* getSelectedSubItem() const;
    int16_t getSelectedSubItemIndex() const;
    void setSelectedSubItemIndex(int16_t index);

    int16_t getTopViewIndex() const;
    void setTopViewIndex(int16_t index);

    int16_t getBottomViewIndex() const;
    void setBottomViewIndex(int16_t index);

    void setViewIndex(int16_t top, int16_t bottom);

    int16_t getViewOffset() const;
    void setViewOffset(int16_t offset);

    template<typename... Args>
    EOGSMenuSubItem* createSubItem(Args&&... args);

    std::pair<int16_t, int16_t> findVisibleItem();
};

// EOGSMenuSubItem类定义
// 菜单子项，从EOGSMenuItem派生，添加label和image功能以及parentMenuItem
class EOGSMenuSubItem : public EOGSMenuItem {
protected:
    EOGSMenuItem* parentMenuItem;         // 父菜单项（菜单系统中的parentItem）
    std::string text;                     // 文本内容（轻量级实现）
    const unsigned char* font;            // 字体
    int16_t textX, textY;                 // 文本渲染坐标
    int16_t textW, textH;                 // 文本宽度高
    const unsigned char* image;           // 图像数据
    int16_t imageX, imageY;               // 图像渲染坐标
    int16_t imageW, imageH;               // 图像宽高
    int16_t selectedX, selectedW;         // 选择器坐标和宽度
    // Padding设置（只保留左右边距）
    int16_t paddingLeftOrTopText,paddingRightOrBottomText;    // 文本左右/上下边距
    int16_t paddingLeftOrTopImage, paddingRightOrBottomImage;  // 图像左右/上下边距

public:
    // 构造函数
    EOGSMenuSubItem(float _x, float _y, float _w, float _h, bool _isRelative = false, const std::string& _text = "");

//选中
    EOGSMenuItem *setSelected(bool selected) override;
    bool getSelected() const override;
    int16_t getSelectedX() const override;
    int16_t getSelectedW() const override;
//父菜单项
    void setParentMenuItem(EOGSMenuItem* _parentMenuItem, bool isUpdateParentItem) override;
    EOGSMenuItem* getParentMenuItem() override;
    EOGSMenuItem* getRootMenuItem() override;
//核心程序
    void requestTextUpdate(bool instant = false) override;
    bool updateText(EOGS* eogs) override;
    
    void requestTextAlignUpdate(bool instant = false) override;
    bool updateTextAlign() override;

    void requestImageAlignUpdate(bool instant = false) override;
    bool updateImageAlign() override;

    void requestSelectAreaUpdate(bool instant = false) override;
    bool updateSelectArea() override;

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    //本体渲染，不渲染子菜单项，子菜单项由菜单渲染
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    bool getEnterable() const override;
    EOGSMenuSubItem* setEnterable(bool _isEnterable);
    bool getSelectable() const override;
    EOGSMenuSubItem* setSelectable(bool _isSelectable);
/*********文本方法 开始*********/
//字体
    const unsigned char* getFont() const;
    EOGSMenuSubItem* setFont(const unsigned char* _font);
//字体透明度
    bool getFontTransparent() const;
    EOGSMenuSubItem* setFontTransparent(bool _isFontTransparent);
//文本
    const std::string& getText() const;
    EOGSMenuSubItem* setText(const std::string& _text);
//UTF8标记
    bool getUTF8Enabled() const;
    EOGSMenuSubItem* setUTF8Enabled(bool enabled);
//颜色
    DrawColor getTextColor() const;
    EOGSMenuSubItem* setTextColor(DrawColor color);

    DrawColor getSelectedTextColor() const;
    EOGSMenuSubItem* setSelectedTextColor(DrawColor color);
//裁剪
    bool getClip() const;
    EOGSMenuSubItem* setClip(bool _clip);
//滚动
    bool getScroll() const;
    EOGSMenuSubItem* setScroll(bool _scroll);
//对齐
    uint8_t getTextAlign() const;
    EOGSMenuSubItem* setTextAlign(uint8_t align);
//填充
    int16_t getPaddingLeftText() const;
    EOGSMenuSubItem* setPaddingLeftText(int16_t padding);

    int16_t getPaddingRightText() const;
    EOGSMenuSubItem* setPaddingRightText(int16_t padding);

    EOGSMenuSubItem* setTextPadding(int16_t left, int16_t right);
/*********文本方法 结束*********/
/*********图像方法 开始*********/
//图像
    const unsigned char* getImage() const;
    EOGSMenuSubItem* setImage(const unsigned char* _image, int16_t _imageW = 0, int16_t _imageH = 0);
//颜色
    DrawColor getImageColor() const;
    EOGSMenuSubItem* setImageColor(DrawColor color);

    DrawColor getSelectedImageColor() const;
    EOGSMenuSubItem* setSelectedImageColor(DrawColor color);
//对齐
    uint8_t getImageAlign() const;
    EOGSMenuSubItem* setImageAlign(uint8_t align);
//填充
    int16_t getPaddingLeftImage() const;
    EOGSMenuSubItem* setPaddingLeftImage(int16_t padding);

    int16_t getPaddingRightImage() const;
    EOGSMenuSubItem* setPaddingRightImage(int16_t padding);

    EOGSMenuSubItem* setImagePadding(int16_t left, int16_t right);
//宽高
    int16_t getImageW() const;
    EOGSMenuSubItem* setImageW(int16_t _imageW);

    int16_t getImageH() const;
    EOGSMenuSubItem* setImageH(int16_t _imageH);
/*********图像方法 结束*********/
};
// 在EOGSMenuSubItem类定义之后添加createSubItem函数的实现
template<typename... Args>
EOGSMenuSubItem* EOGSMenuItem::createSubItem(Args&&... args) {
    EOGSMenuSubItem* item = new EOGSMenuSubItem(0,0,1,1,true,std::forward<Args>(args)...);
    addChildMenuItem(static_cast<EOGSMenuItem*>(item));
    item->requestDimensionUpdate(true); //Force Update
    item->requestRenderPosUpdate(true); //Force Update
    item->requestTextUpdate(true);  //Force Update Text
    item->requestTextAlignUpdate(true); //Force Update Text Align
    item->requestImageAlignUpdate(true); //Force Update Image Align
    item->requestSelectAreaUpdate(true); //Force Update Selection Aera
    return item;
}
#undef viewUpdated
#undef childPositionUpdated
#undef imageAlignUpdated
#undef textAlignUpdated
#undef selectAreaUpdated
#undef textAlign
#undef imageAlign
#undef textUpdated
#undef isFontTransparent
#undef isSelected
#undef scroll
#undef clip
#undef isUTF8
#undef isEnterable