//
// Created by thisdp 2025/8/26.
// Menu Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../animation/animation.h"
#include "menuitem.h"
#include "scrollbar.h"

inline int16_t fastMod(int16_t x, int16_t a);

class EOGSMenu : public EOGSMenuItem {  //EOGSMenu继承自EOGSMenuItem，作为菜单本体，也是根菜单项
public:
    static constexpr bool ScrollBarRefViewOffset = 0;
    static constexpr bool ScrollBarRefSelectIndex = 1;
protected:
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
    uint8_t subItemMenuNavigationDirection : 1;     // 子菜单导航方向（用于动画）
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
    EOGSAnimBase subItemMenuNavigationAnimation;  // 子菜单切换动画
    EOGSMenuItem* currentMenuItem;            // 当前选中的菜单项
    EOGSMenuItem* prevMenuItem;               // 上一个菜单项（用于过渡）

    // 滚动条相关变量
    EOGSScrollBar scrollBarVertical;          // 水平滚动条

public:
    // 构造函数
    EOGSMenu(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSMenu() = default;
    bool isContainer() const override;
    int16_t getMenuHeight() override;
    EOGSMenuItem* getParentMenuItem() override;
    EOGSMenuItem* getRootMenuItem() override;
    EOGSScrollBar* getVerticalScrollBar();

    void requestScrollBarThumbSizeRatioUpdate();
    bool updateScrollBarThumbSizeRatio();
    
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    int16_t getDefaultItemHeight() override;
    EOGSMenu* setDefaultItemHeight(int16_t height);
    
    int16_t getTopViewIndex() const;
    int16_t getBottomViewIndex() const;

// 显示设置方法
    EOGSMenu* setPadding(uint8_t left, uint8_t right);
// 字体
    const unsigned char* getFont() const;
    EOGSMenu* setFont(const unsigned char* _font);
// 文本颜色
    DrawColor getTextColor() const;
    EOGSMenu* setTextColor(DrawColor color);
// 选中项颜色
    DrawColor getSelectedTextColor() const;
    EOGSMenu* setSelectedTextColor(DrawColor color);
// 选中项背景颜色
    DrawColor getSelectedBackgroundColor() const;
    EOGSMenu* setSelectedBackgroundColor(DrawColor color);
// 导航
    EOGSMenu* setNavigateLoop(bool loop);
    bool getNavigateLoop() const;
// 选择
    EOGSMenu* setSelectedSubItemIndex(int16_t index);
    int16_t getSelectedSubItemIndex() const;
    EOGSMenuItem *getSelectedSubItem() const;
//选择框水平扩展
    int16_t getSelectionHorizontalExpansion() const;
    EOGSMenu* setSelectionHorizontalExpansion(int16_t value);
//选择框垂直扩展
    int16_t getSelectionVerticalExpansion() const;
    EOGSMenu* setSelectionVerticalExpansion(int16_t value);
// 导航
    EOGSMenu* subItemNavigateUp();
    EOGSMenu* subItemNavigateDown();
    
    // 从指定index开始，查找下一个可选的index
    int16_t findNextSelectableIndex(int16_t startIndex, bool forward = true);

    EOGSMenu* subItemNavigateTo(int16_t targetIndex);
// 导航至菜单
    EOGSMenu* subItemNavigateInto(EOGSMenuItem* item);
// 导航至当前选中菜单
    EOGSMenu* subItemNavigateIntoCurrent();
// 返回上级菜单
    EOGSMenu* subItemNavigateIntoParent();
// 查询当前选中菜单是否可进入
    bool canSelectedSubItemNavigateInto();
// 滚动条滑块参考 (ScrollBarRefViewOffset/ScrollBarRefSelectIndex)
    bool getScrollBarReference() const;
    EOGSMenu* setScrollBarReference(bool reference);
// 滚动条自动调节 (当ChilrenHeight变化时，自动调节滚动条滑块高度)
    void onChildrenHeightUpdate() override;
    bool getScrollBarAutoAdjust() const;
    EOGSMenu* setScrollBarAutoAdjust(bool state);
};
