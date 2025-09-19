//
// Created by thisdp 2025/8/25.
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include "label.h"
#include <functional>
#include <algorithm>

//Flag 1
#define indicatorDimensionUpdated generalFlag1
//Flag 2
#define borderFill generalFlag2
//Flag 3
#define enableOutterBorder generalFlag3
//Flag 4
#define enableInnerBorder generalFlag4

class EOGSCircleProgressBar : public EOGSWidget<EOGSCircleProgressBar> {
protected:
    float minProgress;      // 最小进度值
    float maxProgress;      // 最大进度值
    float progress;         // 当前进度值
    float radius;           // 圆形半径 (0~1的相对值)
    int16_t ocRadius,icRadius;       // 外/内圆计算后的实际半径
    int16_t startAngle,endAngle;     // 起始角度/结束角度
    int16_t cx, cy;         // 圆心坐标
    int16_t indicatorPadding; // 指示器内边距
    int16_t indicatorWidth;   // 指示器宽度
    int16_t indicatorRadius;    // 指示器半径
    //填充内存
    DrawColor outterBorderColor : 2; // 背景色
    DrawColor innerBorderColor : 2; // 背景色
    DrawColor indicatorColor : 2; // 指示器颜色
    uint8_t unused1 : 2;
    uint8_t unused2 : 8;
    EOGSLabel* percentageLabel;  // 百分比文本标签
    std::function<std::string(float, float, float)> textFormat; // 文本格式化函数 (min, max, progress)

public:
    // 构造函数
    EOGSCircleProgressBar(float _x, float _y, float _w, float _h, bool _isRelative = false)
        : EOGSWidget<EOGSCircleProgressBar>(_x, _y, _w, _h, _isRelative),
          minProgress(0.0f), maxProgress(100.0f), progress(0.0f),
          radius(0.5f), ocRadius(0), icRadius(0),
          startAngle(0), endAngle(0),
          cx(0), cy(0), indicatorPadding(0), indicatorWidth(0),
          indicatorRadius(0),
          unused1(0),unused2(0),
          percentageLabel(nullptr),
          textFormat(nullptr) {
            setOutterBorderEnabled(true);
            setOutterBorderColor(DrawColor::WHITE);
            setInnerBorderColor(DrawColor::WHITE);
            setIndicatorColor(DrawColor::WHITE);
          }

    ~EOGSCircleProgressBar() {
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
        ocRadius = static_cast<int16_t>(radius * std::min(w, h));
        icRadius = ocRadius - indicatorWidth - indicatorPadding*2+1;
        cx = renderX + w/2;
        cy = renderY + h/2;
        if(percentageLabel != nullptr) percentageLabel->requestDimensionUpdate(); //如果坐标更新，则需要同步更新label的尺寸
        indicatorDimensionUpdated = false;  //如果更新了尺寸，则需要更新指示器
        return true;
    }

    // 更新指示器缓存
    bool updateIndicatorDimension(EOGS* eogs) {
        if (indicatorDimensionUpdated) return false;
        indicatorDimensionUpdated = true;
        indicatorRadius = ocRadius - indicatorPadding;
        float progressBase = maxProgress - minProgress;
        float normalizedProgress = progressBase == 0 ? 0 : (progress - minProgress) / progressBase;
        endAngle = startAngle+static_cast<int16_t>(normalizedProgress * eogs->getHAL()->getArcMaxAngle());
        return true;
    }

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        updateIndicatorDimension(eogs);
        
        if (insideOfRenderArea || renderOutsideOfRenderArea) {
            if (enableOutterBorder){
                eogs->setDrawColor(outterBorderColor);
                if(borderFill)  eogs->drawDisc(cx, cy, ocRadius);
                else            eogs->drawCircle(cx, cy, ocRadius);
            }
            if (enableInnerBorder){
                eogs->setDrawColor(innerBorderColor);
                if(borderFill)  eogs->drawDisc(cx, cy, icRadius);
                else            eogs->drawCircle(cx, cy, icRadius);
            }
            eogs->setDrawColor(indicatorColor);
            for (int16_t i = indicatorRadius-indicatorWidth; i < indicatorRadius; i++) {
                if(i>0) eogs->drawArc(cx, cy, i, startAngle, endAngle);
            }
            if (percentageLabel != nullptr) percentageLabel->render(eogs, renderX, renderY, w, h);
        }
    }

//相对半径
    float getRelativeRadius() const { return radius; }
    EOGSCircleProgressBar* setRelativeRadius(float r) { radius = r; dimensionUpdated = false; indicatorDimensionUpdated = false; return this; }
//进度最小值
    float getMinProgress() const { return minProgress; }
    EOGSCircleProgressBar* setMinProgress(float min) { minProgress = min; setProgress(progress); return this; }
//进度最大值
    float getMaxProgress() const { return maxProgress; }
    EOGSCircleProgressBar* setMaxProgress(float max) { maxProgress = max; setProgress(progress); return this; }
//进度范围
    EOGSCircleProgressBar* setProgressRange(float min, float max) { minProgress = min; maxProgress = max; setProgress(progress); return this; }
//进度值
    float getProgress() const { return progress; }
    EOGSCircleProgressBar* setProgress(float val) {
        float oldProgress = progress;
        progress = mathClamp(val, minProgress, maxProgress);
        indicatorDimensionUpdated = false;
        
        if (percentageLabel != nullptr && oldProgress != progress) {
            std::string text;
            if (textFormat) {
                text = textFormat(progress, minProgress, maxProgress);
            } else {
                float progressBase = maxProgress - minProgress;
                float normalizedProgress = progressBase == 0 ? 0 : (progress - minProgress) / progressBase;
                int percentage = static_cast<int>(normalizedProgress * 100);
                text = std::to_string(percentage) + "%";
            }
            percentageLabel->setText(text);
        }
        return this;
    }
//显示百分比
    EOGSLabel* getPercentageLabel() const { return percentageLabel; }
    bool getPercentageTextEnabled() const { return percentageLabel != nullptr; }
    EOGSCircleProgressBar* setPercentageTextEnabled(bool show) {
        if (show) {
            if(percentageLabel != nullptr) return this;
            percentageLabel = new EOGSLabel(0.0f, 0.0f, 1.0f, 1.0f, true);
            percentageLabel->setParent(this,true);
            percentageLabel->setColor(DrawColor::WHITE);
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
//指示器颜色
    DrawColor getIndicatorColor() const { return indicatorColor; }
    EOGSCircleProgressBar* setIndicatorColor(DrawColor color) { indicatorColor = color; return this; }
//指示器边距
    uint8_t getIndicatorPadding() const { return indicatorPadding; }
    EOGSCircleProgressBar* setIndicatorPadding(uint8_t padding) { indicatorPadding = padding; indicatorDimensionUpdated = false; dimensionUpdated = false; return this; }
//起始角度
    int16_t getStartAngle() const { return startAngle; }
    EOGSCircleProgressBar* setStartAngle(int16_t angle) { startAngle = angle; indicatorDimensionUpdated = false; return this; }
//指示器宽度
    uint8_t getIndicatorWidth() const { return indicatorWidth; }
    EOGSCircleProgressBar* setIndicatorWidth(uint8_t width) { indicatorWidth = width; dimensionUpdated = false; return this; }
//边框填充
    bool getBorderFill() const { return borderFill; }
    EOGSCircleProgressBar* setBorderFill(bool fill) { borderFill = fill; return this; }
//启用外圈边框
    bool getOutterBorderEnabled() const { return enableOutterBorder; }
    EOGSCircleProgressBar* setOutterBorderEnabled(bool enable) { enableOutterBorder = enable; return this; }
//外圈边框颜色
    DrawColor getOutterBorderColor() const { return outterBorderColor; }
    EOGSCircleProgressBar* setOutterBorderColor(DrawColor color){ outterBorderColor = color; return this; }
//启用内圈边框
    bool getInnerBorderEnabled() const { return enableInnerBorder; }
    EOGSCircleProgressBar* setInnerBorderEnabled(bool enable) { enableInnerBorder = enable; return this; }
//内圈边框颜色
    DrawColor getInnerBorderColor() const { return innerBorderColor; }
    EOGSCircleProgressBar* setInnerBorderColor(DrawColor color) { innerBorderColor = color; return this; }
//文本格式化方法
    std::function<std::string(float, float, float)> getTextFormat() const { return textFormat; }
    EOGSCircleProgressBar* setTextFormat(std::function<std::string(float, float, float)> formatFunc) { 
        textFormat = formatFunc; 
        setProgress(progress);
        return this; 
    }
};

#undef indicatorDimensionUpdated
#undef borderFill
#undef enableOutterBorder
#undef enableInnerBorder