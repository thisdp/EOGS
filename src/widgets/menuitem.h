//
// Created by thisdp 2025/8/26.
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
    EOGSMenuItem(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSWidget<EOGSMenuItem>(_x, _y, _w, _h, _isRelative),
        selectedIndex(-1), topViewIndex(0), bottomViewIndex(0), viewOffset(0),
        paddingLeftOrTop(2), paddingRightOrBottom(2){}
    // 析构函数
    ~EOGSMenuItem() { for (EOGSMenuItem* childItem : childMenuItems) { if (childItem) delete childItem; } childMenuItems.clear(); }
    // 重写基类方法，明确标记为Container
    bool isContainer() const override { return true; }

    std::vector<EOGSWidgetBase*>* getChildren() override { return &children; }

    virtual EOGSMenuItem* setSelected(bool selected) { return this; }
    virtual bool getSelected() const { return false; }
    virtual int16_t getSelectedX() const { return 0; }
    virtual int16_t getSelectedW() const { return 0; }
    virtual bool getEnterable() const { return true; }
    virtual bool getSelectable() const { return true; }

    virtual int16_t getMenuHeight() {
        EOGSMenuItem* rootItem = getRootMenuItem();
        if(rootItem == nullptr) return 0;
        return rootItem->getMenuHeight();
    }  // 菜单的显示高度
    
    virtual int16_t getDefaultItemHeight() {
        EOGSMenuItem* rootItem = getRootMenuItem();
        if(rootItem == nullptr) return 20;
        return rootItem->getDefaultItemHeight();
    }  // 默认菜单项高度

    virtual void requestTextUpdate(bool instant = false){}
    virtual bool updateText(EOGS* eogs){ return false; }
    virtual void requestTextAlignUpdate(bool instant = false){}
    virtual bool updateTextAlign(){ return false; }
    virtual void requestImageAlignUpdate(bool instant = false){}
    virtual bool updateImageAlign() { return false; }
    virtual void requestSelectAreaUpdate(bool instant = false) {}
    virtual bool updateSelectArea() { return false; }
    virtual void requestViewRangeUpdate(){ viewUpdated = false; }
    virtual bool updateViewRange() {    // 更新可见项范围
        if(viewUpdated == true) return false;
        viewUpdated = true;
        updateChildrenHeight();    //更新可见范围时候，需要重新计算子项高度和Y坐标
        std::pair<int16_t, int16_t> visibleRange = findVisibleItem();
        topViewIndex = visibleRange.first;
        bottomViewIndex = visibleRange.second;
        // 计算顶部可见项的偏移量
        return true;
    }
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {}

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false;
        renderPosUpdated = true;
        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        return true;
    }

    //事件
    virtual void onChildrenHeightUpdate() {}
    void requestChildrenHeightUpdate() { childPositionUpdated = false; }
    bool updateChildrenHeight() {   //需要在Menu中更新
        if(childPositionUpdated) return false;
        childPositionUpdated = true;
        int16_t cumulativeHeight = 0;
        for (EOGSMenuItem* child : childMenuItems) {
            if (child != nullptr) {
                child->setY(cumulativeHeight, false);   // 更新子项的Y坐标
                int16_t childHeight = child->getAbsH(); // 累计总高度
                cumulativeHeight += childHeight;
            }
        }
        onChildrenHeightUpdate();
        return true;
    }

    int16_t getTotalChildrenHeight() const {
        if (childMenuItems.empty()) return 0;
        int16_t childY = childMenuItems[childMenuItems.size() - 1]->getAbsY();
        return childY + childMenuItems[childMenuItems.size() - 1]->getAbsH();
    }
// 根菜单
    virtual EOGSMenuItem* getRootMenuItem() { return nullptr; }
// 父级菜单
    virtual EOGSMenuItem* getParentMenuItem() { return nullptr; }
    virtual void setParentMenuItem(EOGSMenuItem* _parentMenuItem, bool isUpdateChildItem = true) {
        EOGSMenuItem* parentItem = getParentMenuItem();
        if(parentItem != _parentMenuItem){
            if(isUpdateChildItem && parentItem != nullptr)  parentItem->removeChildMenuItem(this, false);
            if(_parentMenuItem == nullptr){
                setParentMenuItem(_parentMenuItem);
            }else{
                setParentMenuItem(_parentMenuItem);
                if(isUpdateChildItem) parentItem->addChildMenuItem(this,false);   //新增
            }
        }
    }
    virtual bool isParentMenuItem(EOGSMenuItem* item) {
        if(item == this) return false;  // 避免自身
        EOGSMenuItem* parentItem = this;
        while(1){
            if(parentItem == parentItem->getParentMenuItem()) return false;   // 追溯到根节点，仍然未找到
            parentItem = parentItem->getParentMenuItem();
            if(parentItem == this || parentItem == nullptr) return false;  // 避免自身
            if(parentItem == item) return true;
        }
        return false;
    }
// 子菜单
    const std::vector<EOGSMenuItem*>* getChildMenuItems() { return &childMenuItems; }
    void addChildMenuItem(EOGSMenuItem* child, bool isUpdateParentItem = true) {
        if (child == nullptr) return;
        if (child->getParentMenuItem() != nullptr) child->getParentMenuItem()->removeChildMenuItem(child);
        if(isUpdateParentItem) child->setParentMenuItem(this,false);
        childMenuItems.push_back(child);
        child->setParent(getRootMenuItem());
        child->setH(getDefaultItemHeight(),false);
        child->requestDimensionUpdate(true); //强制更新高度
        requestViewRangeUpdate();   //Child数量变化，重新计算视图区域
        requestChildrenHeightUpdate();  //Child数量变化 重新计算所有Children的高度
        if(selectedIndex == -1) selectedIndex = 0;
    }
    void removeChildMenuItem(EOGSMenuItem* child, bool isUpdateParentItem = true) {
        if (child == nullptr) return;
        auto it = std::find(childMenuItems.begin(), childMenuItems.end(), child);
        if (it != childMenuItems.end()){
            //如果当前选中的item是自己，则移动至上一个item
            if(child == getSelectedSubItem()) selectedIndex = selectedIndex - 1;
            if(isUpdateParentItem) child->setParentMenuItem(nullptr,false);
            childMenuItems.erase(it);
            child->setParent(nullptr);
            requestViewRangeUpdate();   //Child数量变化，重新计算视图区域
            requestChildrenHeightUpdate();  //Child数量变化 重新计算所有Children的高度
        }
    }
// 边距
    int16_t getPaddingLeft() const { return paddingLeftOrTop; }
    EOGSMenuItem* setPaddingLeft(int16_t padding) { paddingLeftOrTop = padding; return this; }
    int16_t getPaddingRight() const { return paddingRightOrBottom; }
    EOGSMenuItem* setPaddingRight(int16_t padding) { paddingRightOrBottom = padding; return this; }
    EOGSMenuItem* setPadding(int16_t left, int16_t right) { paddingLeftOrTop = left; paddingRightOrBottom = right; return this; }
// 选中子项
    int16_t getPreviousSelectedSubItemIndex() const { return previousSelectedIndex; }
    EOGSMenuItem* getSelectedSubItem() const {
        if(selectedIndex < 0) return nullptr;   //没有选中任何子项
        if(selectedIndex >= childMenuItems.size()) return nullptr;   //选项超出范围
        return childMenuItems[selectedIndex];
    }
    int16_t getSelectedSubItemIndex() const { return selectedIndex; }
    void setSelectedSubItemIndex(int16_t index) {
        if(selectedIndex == index) return;
        if(index < -1) index = -1;
        int16_t childrenCount = static_cast<int>(childMenuItems.size());
        if(index >= childrenCount) index = childrenCount - 1;
        if(selectedIndex != -1) childMenuItems[selectedIndex]->setSelected(false);
        if(index != -1) childMenuItems[index]->setSelected(true);
        previousSelectedIndex = selectedIndex;
        selectedIndex = index;
    }

    int16_t getTopViewIndex() const { return topViewIndex; }
    void setTopViewIndex(int16_t index) { topViewIndex = index; }

    int16_t getBottomViewIndex() const { return bottomViewIndex; }
    void setBottomViewIndex(int16_t index) { bottomViewIndex = index; }

    void setViewIndex(int16_t top, int16_t bottom) { topViewIndex = top; bottomViewIndex = bottom; }

    int16_t getViewOffset() const { return viewOffset; }
    void setViewOffset(int16_t offset) { if (viewOffset != offset) { viewOffset = offset; requestViewRangeUpdate();} }

    template<typename... Args>
    EOGSMenuSubItem* createSubItem(Args&&... args);

    std::pair<int16_t, int16_t> findVisibleItem() {
        if (childMenuItems.empty()) return std::make_pair(0, -1);
        int16_t itemCount = static_cast<int16_t>(childMenuItems.size());
        int16_t visibleTop = viewOffset;
        int16_t visibleBottom = viewOffset + getMenuHeight();
        // === 二分查找 top，并记录其 y 坐标 ===
        int16_t top = itemCount;
        int16_t topY = 0;
        int16_t left = 0, right = itemCount - 1;
        while (left <= right) {
            int16_t mid = left + (right - left) / 2;
            int16_t y = childMenuItems[mid]->getAbsY();  // 使用预计算的Y坐标
            int16_t itemBottom = y + childMenuItems[mid]->getAbsH();
            if (itemBottom > visibleTop) {
                top = mid;
                topY = y;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        if (top == itemCount) return std::make_pair(0, -1); // 没有可见项（全部在视口上方）
        // === 线性扫描：找出第一个不可见项 ===
        int16_t currentY = topY;
        int16_t bottom = top;
        while (bottom < itemCount) {
            int16_t itemH = childMenuItems[bottom]->getAbsH();
            if (currentY >= visibleBottom) break;
            currentY += itemH;
            ++bottom;
        }
        return std::make_pair(top, bottom - 1);
    }
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
    EOGSMenuSubItem(float _x, float _y, float _w, float _h, bool _isRelative = false, const std::string& _text = "")
        : EOGSMenuItem(_x, _y, _w, _h, _isRelative),
          parentMenuItem(nullptr),
          text(_text), font(u8g2_font_t0_11_tf),
          textX(0), textY(0), textW(0), textH(0),
          image(nullptr), imageX(0), imageY(0),
          imageW(0), imageH(0), selectedX(0), selectedW(0),
          paddingLeftOrTopText(2), paddingRightOrBottomText(2),
          paddingLeftOrTopImage(2), paddingRightOrBottomImage(2)
    {
        setFontTransparent(true);
        setTextColor(DrawColor::XOR);
        setSelectedTextColor(DrawColor::XOR);
        setImageColor(DrawColor::XOR);
        setSelectedImageColor(DrawColor::XOR);
        setTextAlign(0);        // 水平/垂直对齐
        setImageAlign(0);       // 水平/垂直对齐
        setEnterable(true);
        setSelectable(true);
    }
    
//选中
    EOGSMenuItem *setSelected(bool selected) override { isSelected = selected; return this; }
    bool getSelected() const override { return isSelected; }
    int16_t getSelectedX() const override { return selectedX; }
    int16_t getSelectedW() const override { return selectedW; }
//父菜单项
    void setParentMenuItem(EOGSMenuItem* _parentMenuItem, bool isUpdateParentItem) override { parentMenuItem = _parentMenuItem; }
    EOGSMenuItem* getParentMenuItem() override { return parentMenuItem; }
    EOGSMenuItem* getRootMenuItem() override { return parentMenuItem != nullptr ? parentMenuItem->getRootMenuItem() : nullptr; }
//核心程序
    void requestTextUpdate(bool instant = false) override {
        textUpdated = false;
        updateText(getEOGS());
    }
    bool updateText(EOGS* eogs) override {
        if (textUpdated) return false;
        textUpdated = true;  // 标记文本已更新
        if (font != nullptr) eogs->setFont(font);
        textW = isUTF8 ? eogs->getUTF8FontWidth(text) : eogs->getFontWidth(text);
        textH = eogs->getFontHeight();
        requestTextAlignUpdate();
        return true;
    }
    
    void requestTextAlignUpdate(bool instant = false) override {
        textAlignUpdated = false;
        if(!instant) return;
        updateTextAlign();
    }
    bool updateTextAlign() override {
        if (textAlignUpdated) return false;
        textAlignUpdated = true;
        switch (textAlign) {
            case (uint8_t)HAlign::CENTER:    textX = static_cast<int16_t>(renderX + paddingLeftOrTopText + (w - textW - paddingLeftOrTopText - paddingRightOrBottomText) / 2);  break;
            case (uint8_t)HAlign::RIGHT:     textX = static_cast<int16_t>(renderX + w - textW - paddingRightOrBottomText);        break;
            case (uint8_t)HAlign::LEFT:
            default:                textX = static_cast<int16_t>(renderX + paddingLeftOrTopText);                        break;
        }
        textY = static_cast<int16_t>(renderY + (h - textH) / 2);
        requestSelectAreaUpdate();
        return true;
    }

    void requestImageAlignUpdate(bool instant = false) override {
        imageAlignUpdated = false;
        if(!instant) return;
        updateImageAlign();
    }
    bool updateImageAlign() override {
        if(imageAlignUpdated) return false;
        imageAlignUpdated = true;
        switch (imageAlign) {
            case (uint8_t)HAlign::CENTER:    imageX = static_cast<int16_t>(renderX + paddingLeftOrTopImage + (w - imageW - paddingLeftOrTopImage - paddingRightOrBottomImage) / 2);  break;
            case (uint8_t)HAlign::RIGHT:     imageX = static_cast<int16_t>(renderX + w - imageW - paddingRightOrBottomImage);        break;
            case (uint8_t)HAlign::LEFT:
            default:                imageX = static_cast<int16_t>(renderX + paddingLeftOrTopImage);                        break;
        }
        imageY = static_cast<int16_t>(renderY + (h - imageH) / 2);
        requestSelectAreaUpdate();
        return true;
    }

    void requestSelectAreaUpdate(bool instant = false) override {
        selectAreaUpdated = false;
        if(!instant) return;
        updateSelectArea();
    }
    bool updateSelectArea() override  {
        if(selectAreaUpdated) return false;
        selectAreaUpdated = true;
        bool hasImage = image != nullptr;
        bool hasText = !text.empty();
        if (hasText && hasImage){   //存在字符，则需要考虑字符位置
            int16_t a=imageX, b=imageX+imageW, c=textX, d=textX+textW;
            int16_t max1 = a > b ? a : b;
            int16_t min1 = a < b ? a : b;
            int16_t max2 = c > d ? c : d;
            int16_t min2 = c < d ? c : d;
            int16_t selectedEndX = max1 > max2 ? max1 : max2;
            int16_t selectedStartX = min1 < min2 ? min1 : min2;
            selectedX = selectedStartX;
            selectedW = selectedEndX - selectedStartX;
        }else if(hasImage){
            selectedX = imageX;
            selectedW = imageW;
        }else if(hasText){
            selectedX = textX;
            selectedW = textW;
        }
        return true;
    }

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false;
        renderPosUpdated = true;
        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        requestTextAlignUpdate();
        requestImageAlignUpdate();
        //insideOfRenderArea = !isOutsideOfParent(parentW, parentH);    //menuItem不需要这个
        return true;
    }

    //本体渲染，不渲染子菜单项，子菜单项由菜单渲染
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (eogs == nullptr) return;
        bool wasDimensionUpdated = updateDimension(parentW, parentH);   //如果尺寸更新了，则需要重新计算Children的尺寸
        updateText(eogs);
        bool wasRenderPosUpdated = updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateImageAlign();
        updateTextAlign();
        updateSelectArea();
        // 渲染文本
        if (!text.empty()) {
            if (font != nullptr) eogs->setFont(font);
            eogs->setDrawColor(isSelected? selectedTextColor : textColor);
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
        if (image != nullptr) {
            eogs->setDrawColor(isSelected? selectedImageColor : imageColor);
            eogs->drawBMP(imageX, imageY, imageW, imageH, image);
        }
        //Render Children展开
        std::vector<EOGSWidgetBase*>* children = getChildren();
        if(children == nullptr) return;
        for (auto child : *children){
            if(child == nullptr) continue;
            if (wasDimensionUpdated) child->requestDimensionUpdate();
            if (wasRenderPosUpdated) child->requestRenderPosUpdate();
            if (child->isVisible()) child->render(eogs,renderX,renderY,w,h);
        }
    }

    bool getEnterable() const override { return isEnterable; }
    EOGSMenuSubItem* setEnterable(bool _isEnterable) {
        isEnterable = _isEnterable;
        return this;
    }
    bool getSelectable() const override { return isSelectable; }
    EOGSMenuSubItem* setSelectable(bool _isSelectable) {
        isSelectable = _isSelectable;
        return this;
    }
/*********文本方法 开始*********/
//字体
    const unsigned char* getFont() const { return font; }
    EOGSMenuSubItem* setFont(const unsigned char* _font) {
        font = _font;
        requestTextUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//字体透明度
    bool getFontTransparent() const { return isFontTransparent; }
    EOGSMenuSubItem* setFontTransparent(bool _isFontTransparent) {
        isFontTransparent = _isFontTransparent;
        return this;
    }
//文本
    const std::string& getText() const { return text; }
    EOGSMenuSubItem* setText(const std::string& _text) {
        text = _text;
        requestTextUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//UTF8标记
    bool getUTF8Enabled() const { return isUTF8; }
    EOGSMenuSubItem* setUTF8Enabled(bool enabled) { 
        isUTF8 = enabled;
        requestTextUpdate();  // UTF8标记改变后需要重新计算文本大小
        requestSelectAreaUpdate(true);
        return this;
    }
//颜色
    DrawColor getTextColor() const { return textColor; }
    EOGSMenuSubItem* setTextColor(DrawColor color) { textColor = color; return this; }

    DrawColor getSelectedTextColor() const { return selectedTextColor; }
    EOGSMenuSubItem* setSelectedTextColor(DrawColor color) { selectedTextColor = color; return this; }
//裁剪
    bool getClip() const { return clip; }
    EOGSMenuSubItem* setClip(bool _clip){
        clip = _clip;
        return this;
    }
//滚动
    bool getScroll() const { return scroll; }
    EOGSMenuSubItem* setScroll(bool _scroll){
        scroll = _scroll;
        return this;
    }
//对齐
    uint8_t getTextAlign() const { return textAlign; }
    EOGSMenuSubItem* setTextAlign(uint8_t align) {
        textAlign = align;
        requestTextAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//填充
    int16_t getPaddingLeftText() const { return paddingLeftOrTopText; }
    EOGSMenuSubItem* setPaddingLeftText(int16_t padding) {
        paddingLeftOrTopText = padding;
        requestTextAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }

    int16_t getPaddingRightText() const { return paddingRightOrBottomText; }
    EOGSMenuSubItem* setPaddingRightText(int16_t padding) {
        paddingRightOrBottomText = padding;
        requestTextAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }

    EOGSMenuSubItem* setTextPadding(int16_t left, int16_t right){
        paddingLeftOrTopText = left;
        paddingRightOrBottomText = right;
        requestTextAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
/*********文本方法 结束*********/
/*********图像方法 开始*********/
//图像
    const unsigned char* getImage() const { return image; }
    EOGSMenuSubItem* setImage(const unsigned char* _image, int16_t _imageW = 0, int16_t _imageH = 0) {
        image = _image; imageW = _imageW; imageH = _imageH;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//颜色
    DrawColor getImageColor() const { return imageColor; }
    EOGSMenuSubItem* setImageColor(DrawColor color) { imageColor = color; return this; }

    DrawColor getSelectedImageColor() const { return selectedImageColor; }
    EOGSMenuSubItem* setSelectedImageColor(DrawColor color) { selectedImageColor = color; return this; }
//对齐
    uint8_t getImageAlign() const { return imageAlign; }
    EOGSMenuSubItem* setImageAlign(uint8_t align) {
        imageAlign = align;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//填充
    int16_t getPaddingLeftImage() const { return paddingLeftOrTopImage; }
    EOGSMenuSubItem* setPaddingLeftImage(int16_t padding) {
        paddingLeftOrTopImage = padding;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }

    int16_t getPaddingRightImage() const { return paddingRightOrBottomImage; }
    EOGSMenuSubItem* setPaddingRightImage(int16_t padding) {
        paddingRightOrBottomImage = padding;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }

    EOGSMenuSubItem* setImagePadding(int16_t left, int16_t right) {
        paddingLeftOrTopImage = left;
        paddingRightOrBottomImage = right;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
//宽高
    int16_t getImageW() const { return imageW; }
    EOGSMenuSubItem* setImageW(int16_t _imageW) {
        imageW = _imageW;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }

    int16_t getImageH() const { return imageH; }
    EOGSMenuSubItem* setImageH(int16_t _imageH) {
        imageH = _imageH;
        requestImageAlignUpdate(true);
        requestSelectAreaUpdate(true);
        return this;
    }
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