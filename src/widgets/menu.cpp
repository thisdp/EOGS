#include "menu.h"

inline int16_t fastMod(int16_t x, int16_t a) {
    int16_t r = x % a;
    return r+a*(r < 0);
}

// 构造函数
EOGSMenu::EOGSMenu(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSMenuItem(_x, _y, _w, _h, _isRelative),
      font(0),
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
      subItemMenuNavigationAnimation(500,EOGSBuiltInEasing::InOutSine()),
      currentMenuItem(this), prevMenuItem(this),
      scrollBarVertical(_w-8,0,8,_h,false,true){}

bool EOGSMenu::isContainer() const { return true; }
int16_t EOGSMenu::getMenuHeight() { return getAbsH(); }
EOGSMenuItem* EOGSMenu::getParentMenuItem() { return static_cast<EOGSMenuItem*>(this); }
EOGSMenuItem* EOGSMenu::getRootMenuItem() { return static_cast<EOGSMenuItem*>(this); }
EOGSScrollBar* EOGSMenu::getVerticalScrollBar() { return &scrollBarVertical; }

void EOGSMenu::requestScrollBarThumbSizeRatioUpdate(){ scrollBarThumbSizeRatioUpdated = false; }
bool EOGSMenu::updateScrollBarThumbSizeRatio(){
    if(scrollBarThumbSizeRatioUpdated) return false;    //已经最新
    scrollBarThumbSizeRatioUpdated = true;

    if(!scrollBarThumbAutoAdjust) return true;
    scrollBarVertical.setThumbSizeRatio(static_cast<float>(getAbsH())/static_cast<float>(currentMenuItem->getTotalChildrenHeight()));
    return true;
}

bool EOGSMenu::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    return true;
}

void EOGSMenu::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    //渲染菜单子项仅是Menu的职能
    if (!visible || eogs == nullptr) return;
    bool wasDimensionUpdated = updateDimension(parentW, parentH);
    bool wasRenderPosUpdated = updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    if(wasDimensionUpdated) {
        currentMenuItem->requestViewRangeUpdate();
        // 更新滚动条的位置和尺寸
        scrollBarVertical.setH(getAbsH(), false);       // 与菜单同高
        scrollBarVertical.updateDimension(w,h);    // 更新长宽
        scrollBarVertical.setX(getAbsW() - scrollBarVertical.getAbsW(), false);  // 计算滚动条的X坐标
    }
    if(wasRenderPosUpdated) scrollBarVertical.requestRenderPosUpdate();
    currentMenuItem->updateViewRange();  // 更新可见项范围
    int16_t clipEndX = renderX + w;
    int16_t clipEndY = renderY + h;
    eogs->setClipWindow(renderX, renderY, clipEndX, clipEndY); // 设置裁剪区域
    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        if(currentMenuItem == nullptr) return;
        int16_t shiftInOffset = 0;
        int16_t shiftOutOffset = 0;
        // 更新子菜单切换动画
        subItemMenuNavigationAnimation.update(eogs);
        if(subItemMenuNavigationAnimation.isRunning()){ //如果动画正在运行
            if(subItemMenuNavigationDirection){ //去父菜单，上一菜单往右侧移出
                shiftInOffset = -(1-subItemMenuNavigationAnimation.getProgress())*w;    //-w->0
                shiftOutOffset = (subItemMenuNavigationAnimation.getProgress())*w;      //0->w
            }else{  //去子菜单，上一菜单往左侧移出
                shiftInOffset = (1-subItemMenuNavigationAnimation.getProgress())*w;     //w->0
                shiftOutOffset = -(subItemMenuNavigationAnimation.getProgress())*w;     //0->-w
            }
            // 渲染子上一个菜单的子项
            const std::vector<EOGSMenuItem*>* prevChildren = prevMenuItem->getChildMenuItems();
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
        const std::vector<EOGSMenuItem*>* currentChildrenItems = currentMenuItem->getChildMenuItems();
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

int16_t EOGSMenu::getDefaultItemHeight() { return defaultItemHeight; }
EOGSMenu* EOGSMenu::setDefaultItemHeight(int16_t height) { defaultItemHeight = height; return this; }

int16_t EOGSMenu::getTopViewIndex() const { return currentMenuItem->getTopViewIndex(); }
int16_t EOGSMenu::getBottomViewIndex() const { return currentMenuItem->getBottomViewIndex(); }

// 显示设置方法
EOGSMenu* EOGSMenu::setPadding(uint8_t left, uint8_t right) {
    paddingLeftOrTop = left;
    paddingRightOrBottom = right;
    return this;
}
// 字体
const unsigned char* EOGSMenu::getFont() const { return font; }
EOGSMenu* EOGSMenu::setFont(const unsigned char* _font) { font = _font; return this; }
// 文本颜色 
DrawColor EOGSMenu::getTextColor() const { return textColor; }
EOGSMenu* EOGSMenu::setTextColor(DrawColor color) { textColor = color; return this; }
// 选中项颜色
DrawColor EOGSMenu::getSelectedTextColor() const { return selectedTextColor; }
EOGSMenu* EOGSMenu::setSelectedTextColor(DrawColor color) { selectedTextColor = color; return this; }
// 选中项背景颜色
DrawColor EOGSMenu::getSelectedBackgroundColor() const { return selectedBackgroundColor; }
EOGSMenu* EOGSMenu::setSelectedBackgroundColor(DrawColor color) { selectedBackgroundColor = color; return this; }
// 导航
EOGSMenu* EOGSMenu::setNavigateLoop(bool loop) { navigateLoop = loop; return this; }
bool EOGSMenu::getNavigateLoop() const { return navigateLoop; }
// 选择
EOGSMenu* EOGSMenu::setSelectedSubItemIndex(int16_t index){ return subItemNavigateTo(index); }
int16_t EOGSMenu::getSelectedSubItemIndex() const { return currentMenuItem->getSelectedSubItemIndex(); }
EOGSMenuItem *EOGSMenu::getSelectedSubItem() const { return currentMenuItem->getSelectedSubItem(); }
//选择框水平扩展
int16_t EOGSMenu::getSelectionHorizontalExpansion() const { return selectionHorizontalExpansion; }
EOGSMenu* EOGSMenu::setSelectionHorizontalExpansion(int16_t value) {
    selectionHorizontalExpansion = value;
    return this;
}
//选择框垂直扩展
int16_t EOGSMenu::getSelectionVerticalExpansion() const { return selectionVerticalExpansion; }
EOGSMenu* EOGSMenu::setSelectionVerticalExpansion(int16_t value) {
    selectionVerticalExpansion = value;
    return this;
}
// 导航
EOGSMenu* EOGSMenu::subItemNavigateUp() {
    int16_t currentIndex = getSelectedSubItemIndex();
    int16_t targetIndex = findNextSelectableIndex(currentIndex, false);  // 向前查找
    if(targetIndex != -1) {
        return subItemNavigateTo(targetIndex);
    }
    return this;
}
EOGSMenu* EOGSMenu::subItemNavigateDown() {
    int16_t currentIndex = getSelectedSubItemIndex();
    int16_t targetIndex = findNextSelectableIndex(currentIndex, true);  // 向后查找
    if(targetIndex != -1) {
        return subItemNavigateTo(targetIndex);
    }
    return this;
}

// 从指定index开始，查找下一个可选的index
int16_t EOGSMenu::findNextSelectableIndex(int16_t startIndex, bool forward) {
    if(currentMenuItem == nullptr) return -1;
    const std::vector<EOGSMenuItem*>* items = currentMenuItem->getChildMenuItems();
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

EOGSMenu* EOGSMenu::subItemNavigateTo(int16_t targetIndex){
    if(currentMenuItem == nullptr) return this;
    const std::vector<EOGSMenuItem*>* items = currentMenuItem->getChildMenuItems();
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
EOGSMenu* EOGSMenu::subItemNavigateInto(EOGSMenuItem* item) {
    if(item == nullptr) return this;
    if(!item->getEnterable()) return this;  // 不可进入
    if(item->getChildMenuItems()->size() == 0) return this;   //无子项目，不可进入
    subItemMenuNavigationDirection = 0;
    if(currentMenuItem != nullptr){
        subItemMenuNavigationDirection = currentMenuItem->isParentMenuItem(item);    //如果是父级，则往右滑动，否则往左滑动
    }

    prevMenuItem = currentMenuItem;
    currentMenuItem = item;
    //导航后需要移动至目标视图偏移
    //scrollStartOffset = prevMenuItem->getViewOffset();
    scrollTargetOffset = currentMenuItem->getViewOffset();  //直接切换
    //scrollAnimation.start();
    //导航后加入子菜单切换动画
    subItemMenuNavigationAnimation.start();

    selectionStartX = selectionX;
    selectionStartY = selectionY;
    selectionStartW = selectionW;
    selectionStartH = selectionH;
    selectionAnimation.start();
    return this;
}
// 导航至当前选中菜单
EOGSMenu* EOGSMenu::subItemNavigateIntoCurrent() { 
    subItemNavigateInto(getSelectedSubItem());
    return this;
}
// 返回上级菜单
EOGSMenu* EOGSMenu::subItemNavigateIntoParent() {
    if(currentMenuItem == this) return this;    // 当前菜单为根菜单，不再返回上级菜单
    EOGSMenuItem *parentItem = currentMenuItem->getParentMenuItem();
    //可能需要考虑上级菜单无法进入的问题? todo
    subItemNavigateInto(parentItem);  //导航至目标菜单
    return this;
}

// 查询当前选中菜单是否可进入
bool EOGSMenu::canSelectedSubItemNavigateInto() {
    if(getSelectedSubItem() == this) return true;    // 当前菜单为根菜单，可进入
    if(getSelectedSubItem() == nullptr) return false;
    if(!getSelectedSubItem()->getEnterable()) return false;  // 不可进入
    if(getSelectedSubItem()->getChildMenuItems()->size() == 0) return false;   //无子项目，不可进入
    return true;
}
// 滚动条滑块参考 (ScrollBarRefViewOffset/ScrollBarRefSelectIndex)
bool EOGSMenu::getScrollBarReference() const { return scrollBarReference; }
EOGSMenu* EOGSMenu::setScrollBarReference(bool reference){
    scrollBarReference = reference;
    return this;
}
// 滚动条自动调节 (当ChilrenHeight变化时，自动调节滚动条滑块高度)
void EOGSMenu::onChildrenHeightUpdate() { requestScrollBarThumbSizeRatioUpdate(); }
bool EOGSMenu::getScrollBarAutoAdjust() const { return scrollBarThumbAutoAdjust; }
EOGSMenu* EOGSMenu::setScrollBarAutoAdjust(bool state){
    scrollBarThumbAutoAdjust = state;
    return this;
}