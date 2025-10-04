//
// Created by thisdp 2025/8/24.
// Rectangle Progress Bar Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include "label.h"
#include <functional>

//Flag 1
#define indicatorDimensionUpdated generalFlag1
//Flag 2
#define borderFilled generalFlag2
//Flag 3
#define indicatorFilled generalFlag3
//Flag 4~5
#define borderColor generalColor4
//Flag 6~7
#define indicatorColor generalColor6
class EOGSRectProgressBar : public EOGSWidget<EOGSRectProgressBar> {
protected:
    float minProgress;      // 最小进度值
    float maxProgress;      // 最大进度值
    float progress;         // 当前进度值
    int16_t borderRadius;   // 边框圆角半径
    int16_t indicatorRadius; // 指示器圆角半径
    int16_t indicatorPaddingTop;     // 指示器上内边距
    int16_t indicatorPaddingRight;   // 指示器右内边距
    int16_t indicatorPaddingBottom;  // 指示器下内边距
    int16_t indicatorPaddingLeft;    // 指示器左内边距
    int16_t indicatorX;      // 指示器X坐标缓存
    int16_t indicatorY;      // 指示器Y坐标缓存
    int16_t indicatorW;      // 指示器宽度缓存
    int16_t indicatorH;      // 指示器高度缓存
    int16_t indicatorXE;     // 指示器X+W
    int16_t indicatorYE;     // 指示器Y+H
    EOGSLabel* percentageLabel;  // 百分比文本标签
    std::function<std::string(float, float, float)> textFormat; // 文本格式化函数 (min, max, progress)

public:
    // 构造函数
    EOGSRectProgressBar(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSWidget<EOGSRectProgressBar>(_x, _y, _w, _h, _isRelative),
          minProgress(0.0f), maxProgress(100.0f), progress(0.0f),
          borderRadius(0), indicatorRadius(0),
          indicatorPaddingTop(2), indicatorPaddingRight(2), indicatorPaddingBottom(2), indicatorPaddingLeft(2),
          indicatorX(0), indicatorY(0), indicatorW(0), indicatorH(0),
          percentageLabel(nullptr),
          textFormat(nullptr){
            setIndicatorFilled(true);
            setBorderFilled(false);
            setBorderColor(DrawColor::WHITE);
            setIndicatorColor(DrawColor::WHITE);
          }

    ~EOGSRectProgressBar() {
        if (percentageLabel != nullptr) {
            delete percentageLabel;
            percentageLabel = nullptr;
        }
    }

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false; renderPosUpdated = true;

        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        if(percentageLabel != nullptr) percentageLabel->requestDimensionUpdate(); //如果坐标更新，则需要同步更新label的尺寸
        requestIndicatorDimensionUpdate();  //如果更新了尺寸，则需要更新指示器
        return true;
    }

    // 更新指示器缓存
    void requestIndicatorDimensionUpdate() { indicatorDimensionUpdated = false; }
    bool updateIndicatorDimension(EOGS* eogs) {
        if (indicatorDimensionUpdated) return false;
        indicatorDimensionUpdated = true;
        float progressBase = maxProgress - minProgress;
        float normalizedProgress = progressBase == 0 ? 0 : (progress - minProgress) / progressBase;
        indicatorX = renderX + indicatorPaddingLeft;
        indicatorY = renderY + indicatorPaddingTop;
        indicatorW = w - indicatorPaddingLeft - indicatorPaddingRight;
        indicatorH = h - indicatorPaddingTop - indicatorPaddingBottom;
        int16_t progressWidth = static_cast<int16_t>(indicatorW*normalizedProgress);
        indicatorXE = indicatorX + progressWidth;
        indicatorYE = indicatorY + indicatorH;
        return true;
    }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        bool dimensionUpdated = updateDimension(parentW, parentH);
        bool renderPosUpdated = updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateIndicatorDimension(eogs);
        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            // 绘制背景
            eogs->setDrawColor(borderColor);
            if (borderFilled) {// 绘制填充背景
                if (borderRadius > 0)
                    eogs->drawRBox(renderX, renderY, w, h, borderRadius);
                else
                    eogs->drawBox(renderX, renderY, w, h);
            } else {// 绘制边框背景
                if (borderRadius > 0)
                    eogs->drawRFrame(renderX, renderY, w, h, borderRadius);
                else
                    eogs->drawFrame(renderX, renderY, w, h);
            }
            // 绘制指示器
            eogs->setDrawColor(indicatorColor);
            eogs->setClipWindow(indicatorX,indicatorY,indicatorXE,indicatorYE);
            if (indicatorFilled) {
                // 绘制填充指示器
                if (indicatorRadius > 0){
                    eogs->drawRBox(indicatorX, indicatorY, indicatorW, indicatorH, indicatorRadius);
                }
                else
                    eogs->drawBox(indicatorX, indicatorY, indicatorW, indicatorH);
            } else {
                // 绘制边框指示器
                if (indicatorRadius > 0)
                    eogs->drawRFrame(indicatorX, indicatorY, indicatorW, indicatorH, indicatorRadius);
                else
                    eogs->drawFrame(indicatorX, indicatorY, indicatorW, indicatorH);
            }
            eogs->setMaxClipWindow();
            if (percentageLabel != nullptr){
                if(dimensionUpdated) percentageLabel->requestDimensionUpdate();
                if(renderPosUpdated) percentageLabel->requestRenderPosUpdate();
                percentageLabel->render(eogs, renderX, renderY, w, h);
            }
        }
    }

    // 链式调用方法
    EOGSRectProgressBar* setMinProgress(float min) { minProgress = min; setProgress(progress); return this; }
    EOGSRectProgressBar* setMaxProgress(float max) { maxProgress = max; setProgress(progress); return this; }
    EOGSRectProgressBar* setProgressRange(float min, float max) { minProgress = min; maxProgress = max; setProgress(progress); return this; }
    EOGSRectProgressBar* setProgress(float val) {
        float oldProgress = progress;
        progress = mathClamp(val, minProgress, maxProgress);
        // 使指示器缓存无效
        requestIndicatorDimensionUpdate();
        if (percentageLabel != nullptr && oldProgress != progress) {
            std::string text;
            if (textFormat) {
                // 使用自定义格式化函数
                text = textFormat(progress, minProgress, maxProgress);
            } else {
                // 默认格式化为百分比
                float progressBase = maxProgress - minProgress;
                float normalizedProgress = progressBase == 0 ? 0 : (progress - minProgress) / progressBase;
                int percentage = static_cast<int>(normalizedProgress * 100);
                text = std::to_string(percentage) + "%";
            }
            percentageLabel->setText(text);
        }
        return this;
    }
    EOGSRectProgressBar* setBorderColor(DrawColor color) { borderColor = color; return this; }
    EOGSRectProgressBar* setIndicatorColor(DrawColor color) { indicatorColor = color; return this; }
    EOGSRectProgressBar* setBorderFilled(bool filled) { borderFilled = filled; return this; }
    EOGSRectProgressBar* setIndicatorFilled(bool filled) { indicatorFilled = filled; return this; }
    EOGSRectProgressBar* setPercentageTextEnabled(bool show) {
        if (show) {
            if(percentageLabel != nullptr) return this;
            percentageLabel = new EOGSLabel(0.0f, 0.0f, 1.0f, 1.0f, true);
            percentageLabel->setParent(this,true);
            percentageLabel->setColor(DrawColor::XOR);
            percentageLabel->setHAlign(HAlign::CENTER);
            percentageLabel->setVAlign(VAlign::CENTER);
            percentageLabel->setForceRenderOutsideOfCanvas(true);
        } else if (!show) {
            if(percentageLabel == nullptr) return this;
            delete percentageLabel;
            percentageLabel = nullptr;
        }
        return this;
    }

    EOGSLabel* getPercentageLabel() const { return percentageLabel; }
    EOGSRectProgressBar* setBorderRadius(uint8_t radius) { borderRadius = radius; return this; }
    EOGSRectProgressBar* setIndicatorRadius(uint8_t radius) { indicatorRadius = radius; return this; }
    EOGSRectProgressBar* setIndicatorPadding(int16_t padding) {
        indicatorPaddingTop = padding, indicatorPaddingRight = padding, indicatorPaddingBottom = padding, indicatorPaddingLeft = padding;
        requestIndicatorDimensionUpdate();
        return this;
    }
    EOGSRectProgressBar* setIndicatorPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
        indicatorPaddingTop = top, indicatorPaddingRight = right, indicatorPaddingBottom = bottom, indicatorPaddingLeft = left;
        requestIndicatorDimensionUpdate();
        return this;
    }

    // 单独设置各个方向的padding
    EOGSRectProgressBar* setIndicatorPaddingLeft(int16_t left) { indicatorPaddingLeft = left; requestIndicatorDimensionUpdate(); return this; }
    EOGSRectProgressBar* setIndicatorPaddingRight(int16_t right) { indicatorPaddingRight = right; requestIndicatorDimensionUpdate(); return this; }
    EOGSRectProgressBar* setIndicatorPaddingTop(int16_t top) { indicatorPaddingTop = top; requestIndicatorDimensionUpdate(); return this; }
    EOGSRectProgressBar* setIndicatorPaddingBottom(int16_t bottom) { indicatorPaddingBottom = bottom; requestIndicatorDimensionUpdate(); return this; }
    // 获取padding值的方法
    int16_t getIndicatorPaddingTop() const { return indicatorPaddingTop; }
    int16_t getIndicatorPaddingRight() const { return indicatorPaddingRight; }
    int16_t getIndicatorPaddingBottom() const { return indicatorPaddingBottom; }
    int16_t getIndicatorPaddingLeft() const { return indicatorPaddingLeft; }

    // 获取属性值的方法
    float getMinProgress() const { return minProgress; }
    float getMaxProgress() const { return maxProgress; }
    float getProgress() const { return progress; }
    DrawColor getBorderColor() const { return borderColor; }
    DrawColor getIndicatorColor() const { return indicatorColor; }
    bool getPercentageTextEnabled() const { return percentageLabel != nullptr; }
    uint8_t getBorderRadius() const { return borderRadius; }
    bool getBorderFilled() const { return borderFilled; }
    bool getIndicatorFilled() const { return indicatorFilled; }
    
    // 文本格式化方法
    EOGSRectProgressBar* setTextFormat(std::function<std::string(float, float, float)> formatFunc) { 
        textFormat = formatFunc; 
        // 更新文本显示
        setProgress(progress);
        return this; 
    }
    
    std::function<std::string(float, float, float)> getTextFormat() const { return textFormat; }
    uint8_t getIndicatorRadius() const { return indicatorRadius; }

};

#undef indicatorDimensionUpdated
#undef borderFilled
#undef indicatorFilled
#undef borderColor
#undef indicatorColor
