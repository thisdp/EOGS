#include "../EOGS.h"
#include "widget.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../animation/animation.h"
#include "menuitem.h"
#include "scrollbar.h"

inline int16_t fastMod(int16_t x, int16_t a) {
    int16_t r = x % a;
    return r+a*(r < 0);
}

class EOGSMenu : public EOGSMenuItem {  //EOGSMenu继承自EOGSMenuItem，作为菜单本体，也是根菜单项
public:
    static constexpr bool ScrollBarRefViewOffset = 0;
    static constexpr bool ScrollBarRefSelectIndex = 1;
protected:
    //默认设置
    const unsigned char* font;                // 字体
    //需要内存填充
    DrawColor textColor : 2;                  // 文本颜色(默认)
    DrawColor selectedTextColor : 2;          // 选中文本颜色(默认)
    DrawColor imageColor : 2;                 // 图片颜色(默认)
    DrawColor selectedImageColor : 2;         // 选中图片颜色(默认)
    DrawColor selectedBackgroundColor : 2;    // 选中背景颜色
    uint8_t navigateLoop : 1;                 // 选择框是否允许循环导航
    uint8_t selectionFill : 1;                // 选择框是否填充
    uint8_t scrollBarReference : 1;           // (scrollBarReference=0, Refer to viewOffset, scrollBarReference=1, Refer to selectIndex)
    uint8_t scrollBarThumbAutoAdjust : 1;     // 滚动条是否自动调整滑块
    uint8_t scrollBarThumbSizeRatioUpdated : 1;   // 滚动条滑块是否已更新
    uint8_t subMenuNavigationDirection : 1;     // 子菜单导航方向（用于动画）
    int16_t defaultItemHeight;
    // 选择框相关变量
    EOGSAnimBase selectionAnimation;          // 选择框动画
    int16_t selectionStartX;                  // 选择框动画起始位置，相对于菜单
    int16_t selectionStartY;                  // 选择框动画起始位置，相对于菜单
    int16_t selectionStartW;                  // 选择框动画起始位置，相对于菜单
    int16_t selectionStartH;                  // 选择框动画起始位置，相对于菜单
    int16_t selectionX;                       // 选择框动画当前位置，相对于菜单
    int16_t selectionY;                       // 选择框动画当前位置，相对于菜单
    int16_t selectionW;                       // 选择框动画当前位置，相对于菜单
    int16_t selectionH;                       // 选择框动画当前位置，相对于菜单
    int8_t selectionHorizontalExpansion;      // 选择框水平扩展
    int8_t selectionVerticalExpansion;        // 选择框垂直扩展
    int16_t selectionRadius;                  // 选择框框圆角半径
    
    
    // 滚动动画相关变量
    int16_t scrollStartOffset;                // 滚动动画起始偏移量
    int16_t scrollTargetOffset;               // 滚动动画目标偏移量
    EOGSAnimBase scrollAnimation;             // 滚动动画

    // 子菜单切换相关变量
    EOGSAnimBase subMenuNavigationAnimation;  // 子菜单切换动画
    EOGSMenuItem* currentMenuItem;            // 当前选中的菜单项
    EOGSMenuItem* prevMenuItem;               // 上一个菜单项（用于过渡）

    // 滚动条相关变量
    EOGSScrollBar scrollBarVertical;          // 水平滚动条

public:
    // 构造函数
    EOGSMenu(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSMenuItem(_x, _y, _w, _h, _isRelative),
          font(u8g2_font_t0_11_tf),
          textColor(DrawColor::WHITE), selectedTextColor(DrawColor::BLACK),
          imageColor(DrawColor::WHITE),selectedImageColor(DrawColor::WHITE),
          selectedBackgroundColor(DrawColor::WHITE),
          navigateLoop(true),
          selectionFill(true),
          scrollBarReference(false),
          scrollBarThumbSizeRatioUpdated(false),
          defaultItemHeight(10),
          selectionAnimation(250,EOGSBuiltInEasing::OutSine()),
          selectionStartX(0),
          selectionStartY(0),
          selectionStartW(0),
          selectionStartH(0),
          selectionX(0),
          selectionY(0),
          selectionW(0),
          selectionH(0),
          selectionHorizontalExpansion(1),
          selectionVerticalExpansion(1),
          selectionRadius(0),
          scrollStartOffset(0),
          scrollTargetOffset(0),
          scrollAnimation(250, EOGSBuiltInEasing::OutSine()),
          subMenuNavigationAnimation(500,EOGSBuiltInEasing::InOutSine()),
          currentMenuItem(this), prevMenuItem(this),
          scrollBarVertical(_w-8,0,8,_h,false,true){}

    ~EOGSMenu() {}
    bool isContainer() const override { return true; }
    int16_t getMenuHeight() override { return getAbsH(); }
    EOGSMenuItem* getParentMenuItem() override { return static_cast<EOGSMenuItem*>(this); }
    EOGSMenuItem* getRootMenuItem() override { return static_cast<EOGSMenuItem*>(this); }
    EOGSScrollBar* getVerticalScrollBar() { return &scrollBarVertical; }

    void requestScrollBarThumbSizeRatioUpdate(){ scrollBarThumbSizeRatioUpdated = false; }
    bool updateScrollBarThumbSizeRatio(){
        if(scrollBarThumbSizeRatioUpdated) return false;    //已经最新
        scrollBarThumbSizeRatioUpdated = true;
        if(!scrollBarThumbAutoAdjust) return true;
        Serial.println(getAbsH());
        Serial.println(currentMenuItem->getTotalChildrenHeight());
        scrollBarVertical.setThumbSizeRatio(static_cast<float>(getAbsH())/static_cast<float>(currentMenuItem->getTotalChildrenHeight()));
        return true;
    }
    
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false; renderPosUpdated = true;
        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        return true;
    }
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        //渲染菜单子项仅是Menu的职能
        if (!visible || eogs == nullptr) return;
        bool wasDimensionUpdated = updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        if(wasDimensionUpdated) {
            currentMenuItem->requestViewRangeUpdate();
            // 更新滚动条的位置和尺寸
            scrollBarVertical.setH(getAbsH(), false);       // 与菜单同高
            scrollBarVertical.updateDimension(w,h);    // 更新长宽
            scrollBarVertical.setX(getAbsW() - scrollBarVertical.getAbsW(), false);  // 计算滚动条的X坐标
        }
        currentMenuItem->updateViewRange();  // 更新可见项范围
        int16_t clipEndX = renderX + w;
        int16_t clipEndY = renderY + h;
        eogs->setClipWindow(renderX, renderY, clipEndX, clipEndY); // 设置裁剪区域
        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            if(currentMenuItem == nullptr) return;
            int16_t shiftInOffset = 0;
            int16_t shiftOutOffset = 0;
            // 更新子菜单切换动画
            subMenuNavigationAnimation.update(eogs);
            if(subMenuNavigationAnimation.isRunning()){ //如果动画正在运行
                Serial.println(subMenuNavigationAnimation.getProgress());
                if(subMenuNavigationDirection){ //去父菜单，上一菜单往右侧移出
                    shiftInOffset = -(1-subMenuNavigationAnimation.getProgress())*w;    //-w->0
                    shiftOutOffset = (subMenuNavigationAnimation.getProgress())*w;      //0->w
                }else{  //去子菜单，上一菜单往左侧移出
                    shiftInOffset = (1-subMenuNavigationAnimation.getProgress())*w;     //w->0
                    shiftOutOffset = -(subMenuNavigationAnimation.getProgress())*w;     //0->-w
                }
                // 渲染子上一个菜单的子项
                const std::vector<EOGSMenuItem*>* prevChildren = prevMenuItem->getChildrenMenuItems();
                int16_t startIndex = prevMenuItem->getTopViewIndex();
                int16_t endIndex = prevMenuItem->getBottomViewIndex();
                int16_t viewOffset = prevMenuItem->getViewOffset();
                for (int16_t i = startIndex; i <= endIndex && i < static_cast<int16_t>(prevChildren->size()); i++) {
                    EOGSMenuItem* item = (*prevChildren)[i];
                    if (item == nullptr) continue;
                    item->requestRenderPosUpdate();
                    eogs->setClipWindow(renderX, renderY, clipEndX, clipEndY); // 设置裁剪区域
                    item->render(eogs, renderX+shiftOutOffset, renderY-viewOffset, w, h);   //子项由自己渲染
                }
            }
            // 渲染当前菜单
            int16_t startIndex = currentMenuItem->getTopViewIndex();
            int16_t endIndex = currentMenuItem->getBottomViewIndex();
            const std::vector<EOGSMenuItem*>* currentChildrenItems = currentMenuItem->getChildrenMenuItems();
            int16_t viewOffset = currentMenuItem->getViewOffset();
            int16_t selected = currentMenuItem->getSelectedSubItemIndex();
            bool selectedAvailable = (selected >= 0 && selected < static_cast<int16_t>(currentChildrenItems->size()));
            // 渲染选择框
            if(selectedAvailable){
                if(selectionRadius > 0){
                    if(selectionFill){
                        eogs->drawRBox(selectionX, selectionY, selectionW, selectionH, selectionRadius);
                    }else{
                        eogs->drawRFrame(selectionX, selectionY, selectionW, selectionH, selectionRadius);
                    }
                }else{
                    if(selectionFill){
                        eogs->drawBox(selectionX, selectionY, selectionW, selectionH);
                    }else{
                        eogs->drawFrame(selectionX, selectionY, selectionW, selectionH);
                    }
                }
            }
            // 渲染当前菜单子项
            for (int16_t i = startIndex; i <= endIndex && i < static_cast<int16_t>(currentChildrenItems->size()); i++) {
                EOGSMenuItem* item = (*currentChildrenItems)[i];
                if (item == nullptr) continue;
                item->requestRenderPosUpdate();
                eogs->setClipWindow(renderX, renderY, clipEndX, clipEndY); // 设置裁剪区域
                item->render(eogs, renderX+shiftInOffset, renderY-viewOffset, w, h);   //子项由自己渲染
            }
            // 更新选择框位置
            selectionAnimation.update(eogs);    //更新选择动画
            if(selectedAvailable){
                EOGSMenuItem *selectedItem = (*currentChildrenItems)[selected];
                if(selectionAnimation.isRunning()){
                    selectionX = selectionStartX + selectionAnimation.getProgress() * ( selectedItem->getSelectedX() - selectionStartX - selectionHorizontalExpansion);
                    selectionY = selectionStartY + selectionAnimation.getProgress() * ( selectedItem->getAbsY()+renderY-viewOffset - selectionStartY - selectionVerticalExpansion);
                    selectionW = selectionStartW + selectionAnimation.getProgress() * ( selectedItem->getSelectedW() - selectionStartW + selectionHorizontalExpansion+selectionHorizontalExpansion);
                    selectionH = selectionStartH + selectionAnimation.getProgress() * ( selectedItem->getAbsH() - selectionStartH + selectionVerticalExpansion+selectionVerticalExpansion);
                    
                }else{
                    selectionX = selectedItem->getSelectedX() - selectionHorizontalExpansion;
                    selectionY = selectedItem->getAbsY()+renderY-viewOffset - selectionVerticalExpansion;
                    selectionW = selectedItem->getSelectedW() + selectionHorizontalExpansion+selectionHorizontalExpansion;
                    selectionH = selectedItem->getAbsH() + selectionVerticalExpansion+selectionVerticalExpansion;
                }
            }
            // 更新滚动动画
            scrollAnimation.update(eogs);
            if (scrollAnimation.isRunning()) {
                int16_t newViewOffset = scrollStartOffset + scrollAnimation.getProgress() * (scrollTargetOffset - scrollStartOffset);
                currentMenuItem->setViewOffset(newViewOffset);
            }else{
                currentMenuItem->setViewOffset(scrollTargetOffset);
            }
            float scrollPercent = 0;
            if(scrollBarReference){ //以selectOffset为参考点
                int16_t prevIndex = currentMenuItem->getPreviousSelectedSubItemIndex();
                int16_t index = currentMenuItem->getSelectedSubItemIndex();
                int16_t itemCount = static_cast<int16_t>(currentChildrenItems->size());
                if(prevIndex < 0 || prevIndex >= itemCount){
                    prevIndex = index;
                }
                
                if (scrollAnimation.isRunning()) {
                    scrollPercent = (static_cast<float>(prevIndex)+scrollAnimation.getProgress()*static_cast<float>(index-prevIndex))/static_cast<float>(itemCount-1);
                }else{
                    scrollPercent = static_cast<float>(index)/static_cast<float>(itemCount-1);
                }
            }else{  //以viewOffset为参考点
                int16_t childrenHeight = currentMenuItem->getTotalChildrenHeight();
                if(childrenHeight > 0){
                    scrollPercent = static_cast<float>(currentMenuItem->getViewOffset())/static_cast<float>((childrenHeight-getAbsH()));
                }
            }
            scrollBarVertical.setScrollValuePercent(scrollPercent);
            eogs->setMaxClipWindow(); // 设置裁剪区域
            //渲染滚动条
            updateScrollBarThumbSizeRatio();
            scrollBarVertical.render(eogs, renderX, renderY, w, h);
        }
    }

    int16_t getDefaultItemHeight() override { return defaultItemHeight; }
    EOGSMenu* setDefaultItemHeight(int16_t height) { defaultItemHeight = height; return this; }
    
    int16_t getTopViewIndex() const { return currentMenuItem->getTopViewIndex(); }
    int16_t getBottomViewIndex() const { return currentMenuItem->getBottomViewIndex(); }

// 显示设置方法
    EOGSMenu* setPadding(uint8_t left, uint8_t right) {
        paddingLeftOrTop = left;
        paddingRightOrBottom = right;
        return this;
    }
// 字体
    const unsigned char* getFont() const { return font; }
    EOGSMenu* setFont(const unsigned char* _font) { font = _font; return this; }
// 文本颜色 
    DrawColor getTextColor() const { return textColor; }
    EOGSMenu* setTextColor(DrawColor color) { textColor = color; return this; }
// 选中项颜色
    DrawColor getSelectedTextColor() const { return selectedTextColor; }
    EOGSMenu* setSelectedTextColor(DrawColor color) { selectedTextColor = color; return this; }
// 选中项背景颜色
    DrawColor getSelectedBackgroundColor() const { return selectedBackgroundColor; }
    EOGSMenu* setSelectedBackgroundColor(DrawColor color) { selectedBackgroundColor = color; return this; }
// 导航
    EOGSMenu* setNavigateLoop(bool loop) { navigateLoop = loop; return this; }
    bool getNavigateLoop() const { return navigateLoop; }
// 选择
    EOGSMenu* setSelectedSubItemIndex(int16_t index){ return itemNavigate(index); }
    int16_t getSelectedSubItemIndex() const { return currentMenuItem->getSelectedSubItemIndex(); }
    EOGSMenuItem *getSelectedSubItem() const { return currentMenuItem->getSelectedSubItem(); }
//选择框水平扩展
    int16_t getSelectionHorizontalExpansion() const { return selectionHorizontalExpansion; }
    EOGSMenu* setSelectionHorizontalExpansion(int16_t value) {
        selectionHorizontalExpansion = value;
        return this;
    }
//选择框垂直扩展
    int16_t getSelectionVerticalExpansion() const { return selectionVerticalExpansion; }
    EOGSMenu* setSelectionVerticalExpansion(int16_t value) {
        selectionVerticalExpansion = value;
        return this;
    }
// 导航
    EOGSMenu* itemNavigateUp() {
        int16_t currentIndex = getSelectedSubItemIndex();
        int16_t targetIndex = findNextSelectableIndex(currentIndex, false);  // 向前查找
        if(targetIndex != -1) {
            return itemNavigate(targetIndex);
        }
        return this;
    }
    EOGSMenu* itemNavigateDown() {
        int16_t currentIndex = getSelectedSubItemIndex();
        int16_t targetIndex = findNextSelectableIndex(currentIndex, true);  // 向后查找
        if(targetIndex != -1) {
            return itemNavigate(targetIndex);
        }
        return this;
    }
    
    // 从指定index开始，查找下一个可选的index
    int16_t findNextSelectableIndex(int16_t startIndex, bool forward = true) {
        if(currentMenuItem == nullptr) return -1;
        const std::vector<EOGSMenuItem*>* items = currentMenuItem->getChildrenMenuItems();
        int16_t itemCount = static_cast<int16_t>(items->size());
        if(itemCount == 0) return -1;
        int16_t currentIndex = startIndex;
        while(true) {
            // 根据方向更新索引
            if(forward) {
                currentIndex++;
                if(currentIndex >= itemCount) {
                    if(!navigateLoop) return -1;    // 非循环模式，到达末尾
                    currentIndex = 0;  // 循环到开头
                }
            } else {
                currentIndex--;
                if(currentIndex < 0) {
                    if(!navigateLoop) return -1;    // 非循环模式，到达开头
                    currentIndex = itemCount - 1;  // 循环到末尾
                }
            }
            
            // 检查是否回到起始位置（循环模式）
            if(navigateLoop && currentIndex == startIndex) return -1;  // 已经循环一圈，没有找到可选项
            
            // 检查是否找到可选项
            if(currentIndex >= 0 && currentIndex < itemCount) {
                EOGSMenuItem* item = (*items)[currentIndex];
                if(item != nullptr && item->getSelectable()) return currentIndex;   // 找到了可选项
            }
        }
        return -1;  // 没有找到可选项
    }
    
    EOGSMenu* itemNavigate(int16_t targetIndex){
        if(currentMenuItem == nullptr) return this;
        const std::vector<EOGSMenuItem*>* items = currentMenuItem->getChildrenMenuItems();
        int16_t itemCount = static_cast<int16_t>(items->size());
        if(itemCount == 0) return this;
        if(navigateLoop) {  //循环模式
            targetIndex = fastMod(targetIndex, itemCount);
        }else if(targetIndex < 0 || targetIndex >= itemCount){  //非循环模式
            return this;
        }
        
        // 检查目标项是否可选
        EOGSMenuItem* targetItem = (*items)[targetIndex];
        if(targetItem != nullptr && !targetItem->getSelectable()) {
            // 如果不可选，直接返回，不执行任何操作
            return this;
        }
        
        EOGSMenuItem* selectedItem = (*items)[targetIndex];
        //滑动前强制更新一次目标子项的相关尺寸
        selectedItem->requestDimensionUpdate(true); //Force Update
        selectedItem->requestRenderPosUpdate(true); //Force Update
        selectedItem->requestTextUpdate(true);  //Force Update Text
        selectedItem->requestTextAlignUpdate(true); //Force Update Text Align
        selectedItem->requestImageAlignUpdate(true); //Force Update Image Align
        selectedItem->requestSelectAreaUpdate(true); //Force Update Selection Aera
        
        selectionStartX = selectionX;
        selectionStartY = selectionY;
        selectionStartW = selectionW;
        selectionStartH = selectionH;
        selectionAnimation.start();

        currentMenuItem->setSelectedSubItemIndex(targetIndex);
        // 更新 viewOffset 以确保选中项可见，并启动滚动动画
        int16_t itemTop = selectedItem->getAbsY();  // 使用预计算的Y坐标
        int16_t itemBottom = itemTop + selectedItem->getAbsH();
        int16_t menuHeight = currentMenuItem->getMenuHeight();
        int16_t currentViewOffset = currentMenuItem->getViewOffset();
        // 计算目标滚动偏移量
        int16_t targetViewOffset = currentViewOffset;
        // 如果选中项的y在可视区域上方，则向上滚动
        if (itemTop < currentViewOffset) targetViewOffset = itemTop;
        // 如果选中项的y+h在可视区域的下方，则向下滚动
        else if (itemBottom - menuHeight > currentViewOffset) targetViewOffset = itemBottom - menuHeight;
        // 启动滚动动画
        scrollStartOffset = currentViewOffset;
        scrollTargetOffset = targetViewOffset;
        scrollAnimation.start();
        return this;
    }

// 导航至菜单
    EOGSMenu* subMenuNavigateTo(EOGSMenuItem* item) {
        if(item == nullptr) return this;
        if(!item->getEnterable()) return this;  // 不可进入
        if(item->getChildrenMenuItems()->size() == 0) return this;   //无子项目，不可进入
        subMenuNavigationDirection = 0;
        if(currentMenuItem != nullptr){
            subMenuNavigationDirection = currentMenuItem->isParentMenuItem(item);    //如果是父级，则往右滑动，否则往左滑动
        }

        prevMenuItem = currentMenuItem;
        currentMenuItem = item;
        //导航后需要移动至目标视图偏移
        //scrollStartOffset = prevMenuItem->getViewOffset();
        scrollTargetOffset = currentMenuItem->getViewOffset();  //直接切换
        //scrollAnimation.start();
        //导航后加入子菜单切换动画
        Serial.println(subMenuNavigationDirection);
        subMenuNavigationAnimation.start();

        
        selectionStartX = selectionX;
        selectionStartY = selectionY;
        selectionStartW = selectionW;
        selectionStartH = selectionH;
        selectionAnimation.start();
        return this;
    }
// 返回上级菜单
    EOGSMenu* subMenuNavigateToParent() {
        if(currentMenuItem == this) return this;    // 当前菜单为根菜单，不再返回上级菜单
        EOGSMenuItem *parentItem = currentMenuItem->getParentMenuItem();
        //可能需要考虑上级菜单无法进入的问题? todo
        if(parentItem == nullptr) return this;  // 当前菜单没有父菜单，无法返回上级菜单
        subMenuNavigateTo(parentItem);  //导航至目标菜单
        return this;
    }
// 滚动条滑块参考 (ScrollBarRefViewOffset/ScrollBarRefSelectIndex)
    bool getScrollBarReference() const { return scrollBarReference; }
    EOGSMenu* setScrollBarReference(bool reference){
        scrollBarReference = reference;
        return this;
    }
// 滚动条自动调节 (当ChilrenHeight变化时，自动调节滚动条滑块高度)
    void onChildrenHeightUpdate() override { requestScrollBarThumbSizeRatioUpdate(); }
    bool getScrollBarAutoAdjust() const { return scrollBarThumbAutoAdjust; }
    EOGSMenu* setScrollBarAutoAdjust(bool state){
        scrollBarThumbAutoAdjust = state;
        return this;
    }
};
