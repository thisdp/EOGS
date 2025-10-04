//
// Created by thisdp 2025/9/3.
// Scroll Bar Widget for EOGS
//

#include "scrollbar.h"

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

// 构造函数
EOGSScrollBar::EOGSScrollBar(float _x, float _y, float _w, float _h, bool _isRelative, bool _isVertical)
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

// 更新控件尺寸和位置
bool EOGSScrollBar::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
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
void EOGSScrollBar::requestThumbSizeUpdate() { thumbSizeUpdated = false; }
bool EOGSScrollBar::updateThumbSize() {
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
void EOGSScrollBar::requestThumbDimensionUpdate() { thumbDimensionUpdated = false; }
bool EOGSScrollBar::updateThumbDimension() {
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

void EOGSScrollBar::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
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
bool EOGSScrollBar::getVertical() const { return isVertical; }
EOGSScrollBar* EOGSScrollBar::setVertical(bool vertical) {
    isVertical = vertical;
    requestThumbSizeUpdate();
    requestThumbDimensionUpdate();
    return this;
}
//滚动最大/最小值
float EOGSScrollBar::getMinScrollValue() const { return minScrollValue; }
EOGSScrollBar* EOGSScrollBar::setMinScrollValue(float min) { minScrollValue = min; requestThumbDimensionUpdate(); return this; }

float EOGSScrollBar::getMaxScrollValue() const { return maxScrollValue; }
EOGSScrollBar* EOGSScrollBar::setMaxScrollValue(float max) { maxScrollValue = max; requestThumbDimensionUpdate(); return this; }

float EOGSScrollBar::getScrollValue() const { return scrollValue; }
EOGSScrollBar* EOGSScrollBar::setValueRange(float min, float max) {
    minScrollValue = min;
    maxScrollValue = max;
    requestThumbDimensionUpdate();
    return this;
}
//滚动值
EOGSScrollBar* EOGSScrollBar::setScrollValue(float val) {
    if(val == scrollValue) return this;
    scrollValue = mathClamp(val, minScrollValue, maxScrollValue);
    requestThumbDimensionUpdate();
    return this;
}
EOGSScrollBar* EOGSScrollBar::setScrollValuePercent(float percent){    //按照0~1为最小到最大
    setScrollValue(minScrollValue + percent * (maxScrollValue - minScrollValue));
    return this;
}
//滑块尺寸比例(仅用于动态改变滑块尺寸)
float EOGSScrollBar::getThumbSizeRatio() const { return thumbSizeRatio; }
EOGSScrollBar* EOGSScrollBar::setThumbSizeRatio(float r) { thumbSizeRatio = r; thumbSizeUpdated = false; requestThumbDimensionUpdate(); return this; }
//滑块尺寸
int16_t EOGSScrollBar::getThumbSize() const { return thumbSize; }
//滑块最小尺寸
int16_t EOGSScrollBar::getThumbMinSize() const { return thumbMinSize; }
EOGSScrollBar* EOGSScrollBar::setThumbMinSize(int16_t minSize) { thumbMinSize = minSize; thumbSizeUpdated = false; requestThumbDimensionUpdate(); return this; }
//滑块尺寸固定
bool EOGSScrollBar::getThumbSizeFixed() const { return thumbSizeFixed; }
EOGSScrollBar* EOGSScrollBar::setThumbSizeFixed(bool fixed, int16_t size) {
    thumbSizeFixed = fixed;
    if (fixed) thumbSize = size;
    requestThumbSizeUpdate();
    requestThumbDimensionUpdate();
    return this;
}
//滑块颜色
DrawColor EOGSScrollBar::getThumbColor() const { return thumbColor; }
EOGSScrollBar* EOGSScrollBar::setThumbColor(DrawColor color) { thumbColor = color; return this; }
//滑块填充
bool EOGSScrollBar::getThumbFilled() const { return thumbFilled; }
EOGSScrollBar* EOGSScrollBar::setThumbFilled(bool filled) { thumbFilled = filled; return this; }
//滑块圆角半径
uint8_t EOGSScrollBar::getThumbRadius() const { return thumbRadius; }
EOGSScrollBar* EOGSScrollBar::setThumbRadius(uint8_t radius) { thumbRadius = radius; return this; }
//滑块边距
EOGSScrollBar* EOGSScrollBar::setThumbPadding(int16_t padding) { return setThumbPadding(padding, padding, padding, padding); }
EOGSScrollBar* EOGSScrollBar::setThumbPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
    thumbPaddingTop = top;
    thumbPaddingRight = right;
    thumbPaddingBottom = bottom;
    thumbPaddingLeft = left;
    requestThumbDimensionUpdate();
    return this;
}

int16_t EOGSScrollBar::getThumbPaddingTop() const { return thumbPaddingTop; }
EOGSScrollBar* EOGSScrollBar::setThumbPaddingTop(int16_t padding) { thumbPaddingTop = padding; requestThumbDimensionUpdate(); return this; }

int16_t EOGSScrollBar::getThumbPaddingRight() const { return thumbPaddingRight; }
EOGSScrollBar* EOGSScrollBar::setThumbPaddingRight(int16_t padding) { thumbPaddingRight = padding; requestThumbDimensionUpdate(); return this; }

int16_t EOGSScrollBar::getThumbPaddingBottom() const { return thumbPaddingBottom; }
EOGSScrollBar* EOGSScrollBar::setThumbPaddingBottom(int16_t padding) { thumbPaddingBottom = padding; requestThumbDimensionUpdate(); return this; }

int16_t EOGSScrollBar::getThumbPaddingLeft() const { return thumbPaddingLeft; }
EOGSScrollBar* EOGSScrollBar::setThumbPaddingLeft(int16_t padding) { thumbPaddingLeft = padding; requestThumbDimensionUpdate(); return this; }
//滑块端点固定
bool EOGSScrollBar::getThumbEndPointFixed() const { return thumbEndPointFixed; }
EOGSScrollBar* EOGSScrollBar::setThumbEndPointFixed(bool fixed) { thumbEndPointFixed = fixed; requestThumbDimensionUpdate(); return this; }
bool EOGSScrollBar::getThumbStartPointFixed() const { return thumbStartPointFixed; }
EOGSScrollBar* EOGSScrollBar::setThumbStartPointFixed(bool fixed) { thumbStartPointFixed = fixed; requestThumbDimensionUpdate(); return this; }

//显示/隐藏轨道
bool EOGSScrollBar::getTrackShow() const { return trackEnabled; }
EOGSScrollBar* EOGSScrollBar::setTrackEnabled(bool show) { trackEnabled = show; return this; }
//轨道颜色
DrawColor EOGSScrollBar::getTrackColor() const { return trackColor; }
EOGSScrollBar* EOGSScrollBar::setTrackColor(DrawColor color) { trackColor = color; return this; }
//轨道填充
bool EOGSScrollBar::getTrackFilled() const { return trackFilled; }
EOGSScrollBar* EOGSScrollBar::setTrackFilled(bool filled) { trackFilled = filled; return this; }
//轨道圆角半径
uint8_t EOGSScrollBar::getTrackRadius() const { return trackRadius; }
EOGSScrollBar* EOGSScrollBar::setTrackRadius(uint8_t radius) { trackRadius = radius; return this; }

#undef trackEnabled
#undef isVertical
#undef thumbSizeFixed
#undef trackFilled
#undef thumbFilled
#undef thumbDimensionUpdated
#undef thumbSizeUpdated
#undef thumbColor
#undef trackColor