//
// Created by thisdp 2025/9/3.
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
    EOGSScrollBar(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _isVertical = false)
        : EOGSWidget<EOGSScrollBar>(_x, _y, _w, _h, _isRelative),
          minScrollValue(0.0f), maxScrollValue(100.0f), scrollValue(0.0f), thumbSizeRatio(0.5f),
          thumbMinSize(10), thumbSize(20), thumbPosition(0),
          trackRadius(0), thumbRadius(0),
          thumbPaddingTop(2), thumbPaddingRight(2),
          thumbPaddingBottom(2), thumbPaddingLeft(2),
          thumbRenderX(0), thumbRenderY(0), thumbRenderW(0), thumbRenderH(0){
            setTrackEnabled(true);
            setThumbFilled(true);
            setThumbColor(DrawColor::WHITE);
            setTrackColor(DrawColor::WHITE);
            setVertical(_isVertical);
          }

    ~EOGSScrollBar() = default;

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false;
        renderPosUpdated = true;

        if (eogs == nullptr) return true;
        renderX = parentRX + x;
        renderY = parentRY + y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        requestThumbDimensionUpdate();  // 如果更新了尺寸，则需要更新滑块位置
        return true;
    }

    // 更新滑块尺寸
    void requestThumbSizeUpdate() { thumbSizeUpdated = false; }
    bool updateThumbSize() {
        if (thumbSizeUpdated) return false;
        thumbSizeUpdated = true;
        requestThumbDimensionUpdate();
        if (thumbSizeFixed) return true;  // 使用固定尺寸，不需要重新计算
        thumbSize = static_cast<int16_t>((isVertical ? h : w) * thumbSizeRatio);
        thumbSize = max(thumbSize, thumbMinSize);
        thumbSize = min(thumbSize,static_cast<int16_t>(h-thumbPaddingTop-thumbPaddingBottom-thumbMinSize));
        return true;
    }

    // 更新滑块位置
    void requestThumbDimensionUpdate() { thumbDimensionUpdated = false; }
    bool updateThumbDimension() {
        if (thumbDimensionUpdated) return false;
        thumbDimensionUpdated = true;
        // 更新滑块尺寸（如果需要）
        updateThumbSize();
        // 计算滑块位置
        float valueRange = maxScrollValue - minScrollValue;
        // 计算应用内边距后的滑块坐标和尺寸
        thumbPosition = 0;
        float normalizedValue = (scrollValue - minScrollValue) / valueRange;
        if (isVertical) {
            int16_t thumbFullLen = h - thumbPaddingTop - thumbPaddingBottom;
            if (thumbStartPointFixed) {
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * thumbFullLen);
                }
                thumbRenderX = renderX + thumbPaddingLeft;
                thumbRenderY = renderY + thumbPaddingTop;   //Y不动
                thumbRenderW = w - thumbPaddingLeft - thumbPaddingRight;
                thumbRenderH = thumbPosition;    //H动
            } else if (thumbEndPointFixed) {
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * thumbFullLen);
                }
                thumbRenderX = renderX + thumbPaddingLeft;
                thumbRenderY = renderY + thumbPaddingTop + thumbPosition;   //Y动
                thumbRenderW = w - thumbPaddingLeft - thumbPaddingRight;
                thumbRenderH = h - thumbPaddingTop - thumbPaddingBottom - thumbPosition;    //H补
            }else{
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * (thumbFullLen - thumbSize));
                }
                thumbRenderX = renderX + thumbPaddingLeft;
                thumbRenderY = renderY + thumbPaddingTop + thumbPosition;
                thumbRenderW = w - thumbPaddingLeft - thumbPaddingRight;
                thumbRenderH = thumbSize;
            }
        } else {
            uint16_t thumbFullLen = w - thumbPaddingLeft - thumbPaddingRight;
            if (thumbStartPointFixed) {
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * thumbFullLen);
                }
                thumbRenderX = renderX + thumbPaddingLeft;   //X不动
                thumbRenderY = renderY + thumbPaddingTop;
                thumbRenderW = thumbPosition;    //W动
                thumbRenderH = h - thumbPaddingTop - thumbPaddingBottom;
            } else if (thumbEndPointFixed) {
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * thumbFullLen);
                }
                thumbRenderX = renderX + thumbPaddingLeft + thumbPosition;   //X动
                thumbRenderY = renderY + thumbPaddingTop;
                thumbRenderW = w - thumbPaddingLeft - thumbPaddingRight - thumbPosition;    //W补
                thumbRenderH = h - thumbPaddingTop - thumbPaddingBottom;
            }else{
                if (valueRange > 0){
                    thumbPosition = static_cast<int16_t>(normalizedValue * (thumbFullLen - thumbSize));
                }
                thumbRenderX = renderX + thumbPaddingLeft + thumbPosition;
                thumbRenderY = renderY + thumbPaddingTop;
                thumbRenderW = thumbSize;
                thumbRenderH = h - thumbPaddingTop - thumbPaddingBottom;
            }
        }
        return true;
    }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;

        bool wasDimensionUpdated = updateDimension(parentW, parentH);
        if(wasDimensionUpdated) requestThumbSizeUpdate();
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateThumbSize();
        updateThumbDimension();

        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            if (trackEnabled) {
                eogs->setDrawColor(trackColor);
                if (trackFilled) {
                    if (trackRadius > 0)
                        eogs->drawRBox(renderX, renderY, w, h, trackRadius);
                    else
                        eogs->drawBox(renderX, renderY, w, h);
                } else {
                    if (trackRadius > 0)
                        eogs->drawRFrame(renderX, renderY, w, h, trackRadius);
                    else
                        eogs->drawFrame(renderX, renderY, w, h);
                }
            }

            eogs->setDrawColor(thumbColor);
            if (thumbFilled) {
                if (thumbRadius > 0)
                    eogs->drawRBox(thumbRenderX, thumbRenderY, thumbRenderW, thumbRenderH, thumbRadius);
                else
                    eogs->drawBox(thumbRenderX, thumbRenderY, thumbRenderW, thumbRenderH);
            } else {
                if (thumbRadius > 0)
                    eogs->drawRFrame(thumbRenderX, thumbRenderY, thumbRenderW, thumbRenderH, thumbRadius);
                else
                    eogs->drawFrame(thumbRenderX, thumbRenderY, thumbRenderW, thumbRenderH);
            }
        }
    }

//垂直/水平
    bool getVertical() const { return isVertical; }
    EOGSScrollBar* setVertical(bool vertical) {
        isVertical = vertical;
        requestThumbSizeUpdate();
        requestThumbDimensionUpdate();
        return this;
    }
//滚动最大/最小值
    float getMinScrollValue() const { return minScrollValue; }
    EOGSScrollBar* setMinScrollValue(float min) { minScrollValue = min; requestThumbDimensionUpdate(); return this; }

    float getMaxScrollValue() const { return maxScrollValue; }
    EOGSScrollBar* setMaxScrollValue(float max) { maxScrollValue = max; requestThumbDimensionUpdate(); return this; }

    float getScrollValue() const { return scrollValue; }
    EOGSScrollBar* setValueRange(float min, float max) {
        minScrollValue = min;
        maxScrollValue = max;
        requestThumbDimensionUpdate();
        return this;
    }
//滚动值
    EOGSScrollBar* setScrollValue(float val) {
        if(val == scrollValue) return this;
        scrollValue = mathClamp(val, minScrollValue, maxScrollValue);
        requestThumbDimensionUpdate();
        return this;
    }
    EOGSScrollBar* setScrollValuePercent(float percent){    //按照0~1为最小到最大
        setScrollValue(minScrollValue + percent * (maxScrollValue - minScrollValue));
        return this;
    }
//滑块尺寸比例(仅用于动态改变滑块尺寸)
    float getThumbSizeRatio() const { return thumbSizeRatio; }
    EOGSScrollBar* setThumbSizeRatio(float r) { thumbSizeRatio = r; thumbSizeUpdated = false; requestThumbDimensionUpdate(); return this; }
//滑块尺寸
    int16_t getThumbSize() const { return thumbSize; }
//滑块最小尺寸
    int16_t getThumbMinSize() const { return thumbMinSize; }
    EOGSScrollBar* setThumbMinSize(int16_t minSize) { thumbMinSize = minSize; thumbSizeUpdated = false; requestThumbDimensionUpdate(); return this; }
//滑块尺寸固定
    bool getThumbSizeFixed() const { return thumbSizeFixed; }
    EOGSScrollBar* setThumbSizeFixed(bool fixed, int16_t size = 20) { 
        thumbSizeFixed = fixed; 
        if (fixed) thumbSize = size;
        requestThumbSizeUpdate();
        requestThumbDimensionUpdate(); 
        return this; 
    }
//滑块颜色
    DrawColor getThumbColor() const { return thumbColor; }
    EOGSScrollBar* setThumbColor(DrawColor color) { thumbColor = color; return this; }
//滑块填充
    bool getThumbFilled() const { return thumbFilled; }
    EOGSScrollBar* setThumbFilled(bool filled) { thumbFilled = filled; return this; }
//滑块圆角半径
    uint8_t getThumbRadius() const { return thumbRadius; }
    EOGSScrollBar* setThumbRadius(uint8_t radius) { thumbRadius = radius; return this; }
//滑块边距
    EOGSScrollBar* setThumbPadding(int16_t padding) { return setThumbPadding(padding, padding, padding, padding); }
    EOGSScrollBar* setThumbPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
        thumbPaddingTop = top;
        thumbPaddingRight = right;
        thumbPaddingBottom = bottom;
        thumbPaddingLeft = left;
        requestThumbDimensionUpdate();
        return this;
    }

    int16_t getThumbPaddingTop() const { return thumbPaddingTop; }
    EOGSScrollBar* setThumbPaddingTop(int16_t padding) { thumbPaddingTop = padding; requestThumbDimensionUpdate(); return this; }

    int16_t getThumbPaddingRight() const { return thumbPaddingRight; }
    EOGSScrollBar* setThumbPaddingRight(int16_t padding) { thumbPaddingRight = padding; requestThumbDimensionUpdate(); return this; }

    int16_t getThumbPaddingBottom() const { return thumbPaddingBottom; }
    EOGSScrollBar* setThumbPaddingBottom(int16_t padding) { thumbPaddingBottom = padding; requestThumbDimensionUpdate(); return this; }

    int16_t getThumbPaddingLeft() const { return thumbPaddingLeft; }
    EOGSScrollBar* setThumbPaddingLeft(int16_t padding) { thumbPaddingLeft = padding; requestThumbDimensionUpdate(); return this; }
//滑块端点固定
    bool getThumbEndPointFixed() const { return thumbEndPointFixed; }
    EOGSScrollBar* setThumbEndPointFixed(bool fixed) { thumbEndPointFixed = fixed; requestThumbDimensionUpdate(); return this; }
    bool getThumbStartPointFixed() const { return thumbStartPointFixed; }
    EOGSScrollBar* setThumbStartPointFixed(bool fixed) { thumbStartPointFixed = fixed; requestThumbDimensionUpdate(); return this; }

//显示/隐藏轨道
    bool getTrackShow() const { return trackEnabled; }
    EOGSScrollBar* setTrackEnabled(bool show) { trackEnabled = show; return this; }
//轨道颜色
    DrawColor getTrackColor() const { return trackColor; }
    EOGSScrollBar* setTrackColor(DrawColor color) { trackColor = color; return this; }
//轨道填充
    bool getTrackFilled() const { return trackFilled; }
    EOGSScrollBar* setTrackFilled(bool filled) { trackFilled = filled; return this; }
//轨道圆角半径
    uint8_t getTrackRadius() const { return trackRadius; }
    EOGSScrollBar* setTrackRadius(uint8_t radius) { trackRadius = radius; return this; }
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