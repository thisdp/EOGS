//
// Created by thisdp 2025/8/25.
// Circle Progress Bar for EOGS
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
    EOGSCircleProgressBar(float _x, float _y, float _w, float _h, bool _isRelative = false);

    ~EOGSCircleProgressBar();

    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

    // 更新指示器缓存
    bool updateIndicatorDimension(EOGS* eogs);

    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;

//相对半径
    float getRelativeRadius() const;
    EOGSCircleProgressBar* setRelativeRadius(float r);
//进度最小值
    float getMinProgress() const;
    EOGSCircleProgressBar* setMinProgress(float min);
//进度最大值
    float getMaxProgress() const;
    EOGSCircleProgressBar* setMaxProgress(float max);
//进度范围
    EOGSCircleProgressBar* setProgressRange(float min, float max);
//进度值
    float getProgress() const;
    EOGSCircleProgressBar* setProgress(float val);
//显示百分比
    EOGSLabel* getPercentageLabel() const;
    bool getPercentageTextEnabled() const;
    EOGSCircleProgressBar* setPercentageTextEnabled(bool show);
//指示器颜色
    DrawColor getIndicatorColor() const;
    EOGSCircleProgressBar* setIndicatorColor(DrawColor color);
//指示器边距
    uint8_t getIndicatorPadding() const;
    EOGSCircleProgressBar* setIndicatorPadding(uint8_t padding);
//起始角度
    int16_t getStartAngle() const;
    EOGSCircleProgressBar* setStartAngle(int16_t angle);
//指示器宽度
    uint8_t getIndicatorWidth() const;
    EOGSCircleProgressBar* setIndicatorWidth(uint8_t width);
//边框填充
    bool getBorderFill() const;
    EOGSCircleProgressBar* setBorderFill(bool fill);
//启用外圈边框
    bool getOutterBorderEnabled() const;
    EOGSCircleProgressBar* setOutterBorderEnabled(bool enable);
//外圈边框颜色
    DrawColor getOutterBorderColor() const;
    EOGSCircleProgressBar* setOutterBorderColor(DrawColor color);
//启用内圈边框
    bool getInnerBorderEnabled() const;
    EOGSCircleProgressBar* setInnerBorderEnabled(bool enable);
//内圈边框颜色
    DrawColor getInnerBorderColor() const;
    EOGSCircleProgressBar* setInnerBorderColor(DrawColor color);
//文本格式化方法
    std::function<std::string(float, float, float)> getTextFormat() const;
    EOGSCircleProgressBar* setTextFormat(std::function<std::string(float, float, float)> formatFunc);
};

#undef indicatorDimensionUpdated
#undef borderFill
#undef enableOutterBorder
#undef enableInnerBorder