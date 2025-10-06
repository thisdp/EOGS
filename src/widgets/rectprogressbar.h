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
    EOGSRectProgressBar(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSRectProgressBar();

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 更新指示器缓存
    void requestIndicatorDimensionUpdate();
    bool updateIndicatorDimension(EOGS* eogs);

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

// 链式调用方法
    EOGSRectProgressBar* setMinProgress(float min);
    EOGSRectProgressBar* setMaxProgress(float max);
    EOGSRectProgressBar* setProgressRange(float min, float max);
    EOGSRectProgressBar* setProgress(float val);
    EOGSRectProgressBar* setBorderColor(DrawColor color);
    EOGSRectProgressBar* setIndicatorColor(DrawColor color);
    EOGSRectProgressBar* setBorderFilled(bool filled);
    EOGSRectProgressBar* setIndicatorFilled(bool filled);
    EOGSRectProgressBar* setPercentageTextEnabled(bool show);
    EOGSRectProgressBar* updateProgressText();

    EOGSLabel* getPercentageLabel() const;
    EOGSRectProgressBar* setBorderRadius(uint8_t radius);
    EOGSRectProgressBar* setIndicatorRadius(uint8_t radius);
    EOGSRectProgressBar* setIndicatorPadding(int16_t padding);
    EOGSRectProgressBar* setIndicatorPadding(int16_t top, int16_t right, int16_t bottom, int16_t left);

// 单独设置各个方向的padding
    EOGSRectProgressBar* setIndicatorPaddingLeft(int16_t left);
    EOGSRectProgressBar* setIndicatorPaddingRight(int16_t right);
    EOGSRectProgressBar* setIndicatorPaddingTop(int16_t top);
    EOGSRectProgressBar* setIndicatorPaddingBottom(int16_t bottom);
// 获取padding值的方法
    int16_t getIndicatorPaddingTop() const;
    int16_t getIndicatorPaddingRight() const;
    int16_t getIndicatorPaddingBottom() const;
    int16_t getIndicatorPaddingLeft() const;

// 获取属性值的方法
    float getMinProgress() const;
    float getMaxProgress() const;
    float getProgress() const;
    DrawColor getBorderColor() const;
    DrawColor getIndicatorColor() const;
    bool getPercentageTextEnabled() const;
    uint8_t getBorderRadius() const;
    bool getBorderFilled() const;
    bool getIndicatorFilled() const;

// 文本格式化方法
    EOGSRectProgressBar* setTextFormat(std::function<std::string(float, float, float)> formatFunc);

    std::function<std::string(float, float, float)> getTextFormat() const;
    uint8_t getIndicatorRadius() const;

};

#undef indicatorDimensionUpdated
#undef borderFilled
#undef indicatorFilled
#undef borderColor
#undef indicatorColor
