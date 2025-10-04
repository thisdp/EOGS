//
// Created by thisdp 2025/9/3.
// Scroll Bar Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"

//Flag 1
#define trackEnabled generalFlag1
//Flag 2
#define isVertical generalFlag2
//Flag 3
#define thumbSizeFixed generalFlag3
//Flag 4
#define trackFilled generalFlag4
//Flag 5
#define thumbFilled generalFlag5
//Flag 6
#define thumbDimensionUpdated generalFlag6
//Flag 7
#define thumbSizeUpdated generalFlag7
//Flag 8
#define thumbStartPointFixed generalFlag8
//Flag 9
#define thumbEndPointFixed generalFlag9
//Flag 10~11
#define thumbColor generalColor10
//Flag 12~13
#define trackColor generalColor12

class EOGSScrollBar : public EOGSWidget<EOGSScrollBar> {
protected:
    float minScrollValue;           // 最小滚动值
    float maxScrollValue;           // 最大滚动值
    float scrollValue;              // 当前滚动值
    float thumbSizeRatio;           // 比例值 (显示区域/需滚动区域)
    int16_t thumbMinSize;           // 滑块最小尺寸
    int16_t thumbSize;              // 滑块尺寸
    int16_t thumbPosition;          // 滑块位置
    int16_t trackRadius;            // 轨道圆角半径
    int16_t thumbRadius;            // 滑块圆角半径
    int16_t thumbPaddingTop;        // 滑块上内边距
    int16_t thumbPaddingRight;      // 滑块右内边距
    int16_t thumbPaddingBottom;     // 滑块下内边距
    int16_t thumbPaddingLeft;       // 滑块左内边距
    int16_t thumbRenderX;           // 滑块渲染X坐标
    int16_t thumbRenderY;           // 滑块渲染Y坐标
    int16_t thumbRenderW;           // 滑块渲染宽度
    int16_t thumbRenderH;           // 滑块渲染高度

public:
    // 构造函数
    EOGSScrollBar(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _isVertical = false);

    ~EOGSScrollBar() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 更新滑块尺寸
    void requestThumbSizeUpdate();
    bool updateThumbSize();

    // 更新滑块位置
    void requestThumbDimensionUpdate();
    bool updateThumbDimension();

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

//垂直/水平
    bool getVertical() const;
    EOGSScrollBar* setVertical(bool vertical);
//滚动最大/最小值
    float getMinScrollValue() const;
    EOGSScrollBar* setMinScrollValue(float min);

    float getMaxScrollValue() const;
    EOGSScrollBar* setMaxScrollValue(float max);

    float getScrollValue() const;
    EOGSScrollBar* setValueRange(float min, float max);
//滚动值
    EOGSScrollBar* setScrollValue(float val);
    EOGSScrollBar* setScrollValuePercent(float percent);    //按照0~1为最小到最大
//滑块尺寸比例(仅用于动态改变滑块尺寸)
    float getThumbSizeRatio() const;
    EOGSScrollBar* setThumbSizeRatio(float r);
//滑块尺寸
    int16_t getThumbSize() const;
//滑块最小尺寸
    int16_t getThumbMinSize() const;
    EOGSScrollBar* setThumbMinSize(int16_t minSize);
//滑块尺寸固定
    bool getThumbSizeFixed() const;
    EOGSScrollBar* setThumbSizeFixed(bool fixed, int16_t size = 20);
//滑块颜色
    DrawColor getThumbColor() const;
    EOGSScrollBar* setThumbColor(DrawColor color);
//滑块填充
    bool getThumbFilled() const;
    EOGSScrollBar* setThumbFilled(bool filled);
//滑块圆角半径
    uint8_t getThumbRadius() const;
    EOGSScrollBar* setThumbRadius(uint8_t radius);
//滑块边距
    EOGSScrollBar* setThumbPadding(int16_t padding);
    EOGSScrollBar* setThumbPadding(int16_t top, int16_t right, int16_t bottom, int16_t left);

    int16_t getThumbPaddingTop() const;
    EOGSScrollBar* setThumbPaddingTop(int16_t padding);

    int16_t getThumbPaddingRight() const;
    EOGSScrollBar* setThumbPaddingRight(int16_t padding);

    int16_t getThumbPaddingBottom() const;
    EOGSScrollBar* setThumbPaddingBottom(int16_t padding);

    int16_t getThumbPaddingLeft() const;
    EOGSScrollBar* setThumbPaddingLeft(int16_t padding);
//滑块端点固定
    bool getThumbEndPointFixed() const;
    EOGSScrollBar* setThumbEndPointFixed(bool fixed);
    bool getThumbStartPointFixed() const;
    EOGSScrollBar* setThumbStartPointFixed(bool fixed);

//显示/隐藏轨道
    bool getTrackShow() const;
    EOGSScrollBar* setTrackEnabled(bool show);
//轨道颜色
    DrawColor getTrackColor() const;
    EOGSScrollBar* setTrackColor(DrawColor color);
//轨道填充
    bool getTrackFilled() const;
    EOGSScrollBar* setTrackFilled(bool filled);
//轨道圆角半径
    uint8_t getTrackRadius() const;
    EOGSScrollBar* setTrackRadius(uint8_t radius);
};

#undef trackEnabled
#undef isVertical
#undef thumbSizeFixed
#undef trackFilled
#undef thumbFilled
#undef thumbDimensionUpdated
#undef thumbSizeUpdated
#undef thumbColor
#undef trackColor