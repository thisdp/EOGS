#include "circleprogressbar.h"

//Flag 1
#define indicatorDimensionUpdated generalFlag1
//Flag 2
#define borderFill generalFlag2
//Flag 3
#define enableOutterBorder generalFlag3
//Flag 4
#define enableInnerBorder generalFlag4

// 构造函数
EOGSCircleProgressBar::EOGSCircleProgressBar(float _x, float _y, float _w, float _h, bool _isRelative)
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

EOGSCircleProgressBar::~EOGSCircleProgressBar() {
    if (percentageLabel != nullptr) {
        delete percentageLabel;
        percentageLabel = nullptr;
    }
}

// 更新控件尺寸和位置
bool EOGSCircleProgressBar::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

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
bool EOGSCircleProgressBar::updateIndicatorDimension(EOGS* eogs) {
    if (indicatorDimensionUpdated) return false;
    indicatorDimensionUpdated = true;
    
    indicatorRadius = ocRadius - indicatorPadding;
    float progressBase = maxProgress - minProgress;
    float normalizedProgress = progressBase == 0 ? 0 : (progress - minProgress) / progressBase;
    endAngle = startAngle+static_cast<int16_t>(normalizedProgress * eogs->getHAL()->getArcMaxAngle());
    return true;
}

void EOGSCircleProgressBar::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
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
float EOGSCircleProgressBar::getRelativeRadius() const { return radius; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setRelativeRadius(float r) { radius = r; dimensionUpdated = false; indicatorDimensionUpdated = false; return this; }
//进度最小值
float EOGSCircleProgressBar::getMinProgress() const { return minProgress; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setMinProgress(float min) { minProgress = min; setProgress(progress); return this; }
//进度最大值
float EOGSCircleProgressBar::getMaxProgress() const { return maxProgress; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setMaxProgress(float max) { maxProgress = max; setProgress(progress); return this; }
//进度范围
EOGSCircleProgressBar* EOGSCircleProgressBar::setProgressRange(float min, float max) { minProgress = min; maxProgress = max; setProgress(progress); return this; }
//进度值
float EOGSCircleProgressBar::getProgress() const { return progress; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setProgress(float val) {
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
EOGSLabel* EOGSCircleProgressBar::getPercentageLabel() const { return percentageLabel; }
bool EOGSCircleProgressBar::getPercentageTextEnabled() const { return percentageLabel != nullptr; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setPercentageTextEnabled(bool show) {
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
DrawColor EOGSCircleProgressBar::getIndicatorColor() const { return indicatorColor; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setIndicatorColor(DrawColor color) { indicatorColor = color; return this; }
//指示器边距
uint8_t EOGSCircleProgressBar::getIndicatorPadding() const { return indicatorPadding; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setIndicatorPadding(uint8_t padding) { indicatorPadding = padding; indicatorDimensionUpdated = false; dimensionUpdated = false; return this; }
//起始角度
int16_t EOGSCircleProgressBar::getStartAngle() const { return startAngle; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setStartAngle(int16_t angle) { startAngle = angle; indicatorDimensionUpdated = false; return this; }
//指示器宽度
uint8_t EOGSCircleProgressBar::getIndicatorWidth() const { return indicatorWidth; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setIndicatorWidth(uint8_t width) { indicatorWidth = width; dimensionUpdated = false; return this; }
//边框填充
bool EOGSCircleProgressBar::getBorderFill() const { return borderFill; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setBorderFill(bool fill) { borderFill = fill; return this; }
//启用外圈边框
bool EOGSCircleProgressBar::getOutterBorderEnabled() const { return enableOutterBorder; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setOutterBorderEnabled(bool enable) { enableOutterBorder = enable; return this; }
//外圈边框颜色
DrawColor EOGSCircleProgressBar::getOutterBorderColor() const { return outterBorderColor; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setOutterBorderColor(DrawColor color){ outterBorderColor = color; return this; }
//启用内圈边框
bool EOGSCircleProgressBar::getInnerBorderEnabled() const { return enableInnerBorder; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setInnerBorderEnabled(bool enable) { enableInnerBorder = enable; return this; }
//内圈边框颜色
DrawColor EOGSCircleProgressBar::getInnerBorderColor() const { return innerBorderColor; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setInnerBorderColor(DrawColor color) { innerBorderColor = color; return this; }
//文本格式化方法
std::function<std::string(float, float, float)> EOGSCircleProgressBar::getTextFormat() const { return textFormat; }
EOGSCircleProgressBar* EOGSCircleProgressBar::setTextFormat(std::function<std::string(float, float, float)> formatFunc) { 
    textFormat = formatFunc; 
    setProgress(progress);
    return this; 
}


#undef indicatorDimensionUpdated
#undef borderFill
#undef enableOutterBorder
#undef enableInnerBorder