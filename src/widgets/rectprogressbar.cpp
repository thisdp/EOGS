#include "rectprogressbar.h"

//Flag 1
#define indicatorDimensionUpdated generalFlag1
//Flag 2
#define borderFill generalFlag2
//Flag 3
#define indicatorFill generalFlag3
//Flag 4~5
#define borderColor generalColor4
//Flag 6~7
#define indicatorColor generalColor6

// 构造函数
EOGSRectProgressBar::EOGSRectProgressBar(float _x, float _y, float _w, float _h, bool _isRelative)
    : EOGSWidget<EOGSRectProgressBar>(_x, _y, _w, _h, _isRelative),
      minProgress(0.0f), maxProgress(100.0f), progress(0.0f),
      borderRadius(0), indicatorRadius(0),
      indicatorPaddingTop(2), indicatorPaddingRight(2), indicatorPaddingBottom(2), indicatorPaddingLeft(2),
      indicatorX(0), indicatorY(0), indicatorW(0), indicatorH(0),
      indicatorXE(0), indicatorYE(0),
      percentageLabel(nullptr),
      textFormat(nullptr){
        setIndicatorFilled(true);
        setBorderFilled(false);
        setBorderColor(DrawColor::WHITE);
        setIndicatorColor(DrawColor::WHITE);
      }

EOGSRectProgressBar::~EOGSRectProgressBar() {
    if (percentageLabel != nullptr) {
        delete percentageLabel;
        percentageLabel = nullptr;
    }
}

// 更新控件尺寸和位置
bool EOGSRectProgressBar::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH)  {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    if(percentageLabel != nullptr) percentageLabel->requestDimensionUpdate(); //如果坐标更新，则需要同步更新label的尺寸
    requestIndicatorDimensionUpdate();  //如果更新了尺寸，则需要更新指示器
    return true;
}

// 更新指示器缓存
void EOGSRectProgressBar::requestIndicatorDimensionUpdate() { indicatorDimensionUpdated = false; }
bool EOGSRectProgressBar::updateIndicatorDimension(EOGS* eogs) {
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

void EOGSRectProgressBar::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH)  {
    if (!visible || eogs == nullptr) return;
    bool dimensionUpdated = updateDimension(parentW, parentH);
    bool renderPosUpdated = updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    updateIndicatorDimension(eogs);
    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        // 绘制背景
        eogs->setDrawColor(borderColor);
        if (borderFill) {// 绘制填充背景
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
        if (indicatorFill) {
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
EOGSRectProgressBar* EOGSRectProgressBar::setMinProgress(float min) { minProgress = min; setProgress(progress); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setMaxProgress(float max) { maxProgress = max; setProgress(progress); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setProgressRange(float min, float max) { minProgress = min; maxProgress = max; setProgress(progress); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setProgress(float val) {
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
EOGSRectProgressBar* EOGSRectProgressBar::setBorderColor(DrawColor color) { borderColor = color; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorColor(DrawColor color) { indicatorColor = color; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setBorderFilled(bool filled) { borderFill = filled; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorFilled(bool filled) { indicatorFill = filled; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setPercentageTextEnabled(bool show) {
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

EOGSLabel* EOGSRectProgressBar::getPercentageLabel() const { return percentageLabel; }
EOGSRectProgressBar* EOGSRectProgressBar::setBorderRadius(uint8_t radius) { borderRadius = radius; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorRadius(uint8_t radius) { indicatorRadius = radius; return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPadding(int16_t padding) {
    indicatorPaddingTop = padding, indicatorPaddingRight = padding, indicatorPaddingBottom = padding, indicatorPaddingLeft = padding;
    requestIndicatorDimensionUpdate();
    return this;
}
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPadding(int16_t top, int16_t right, int16_t bottom, int16_t left) {
    indicatorPaddingTop = top, indicatorPaddingRight = right, indicatorPaddingBottom = bottom, indicatorPaddingLeft = left;
    requestIndicatorDimensionUpdate();
    return this;
}

// 单独设置各个方向的padding
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPaddingLeft(int16_t left) { indicatorPaddingLeft = left; requestIndicatorDimensionUpdate(); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPaddingRight(int16_t right) { indicatorPaddingRight = right; requestIndicatorDimensionUpdate(); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPaddingTop(int16_t top) { indicatorPaddingTop = top; requestIndicatorDimensionUpdate(); return this; }
EOGSRectProgressBar* EOGSRectProgressBar::setIndicatorPaddingBottom(int16_t bottom) { indicatorPaddingBottom = bottom; requestIndicatorDimensionUpdate(); return this; }
// 获取padding值的方法
int16_t EOGSRectProgressBar::getIndicatorPaddingTop() const { return indicatorPaddingTop; }
int16_t EOGSRectProgressBar::getIndicatorPaddingRight() const { return indicatorPaddingRight; }
int16_t EOGSRectProgressBar::getIndicatorPaddingBottom() const { return indicatorPaddingBottom; }
int16_t EOGSRectProgressBar::getIndicatorPaddingLeft() const { return indicatorPaddingLeft; }

// 获取属性值的方法
float EOGSRectProgressBar::getMinProgress() const { return minProgress; }
float EOGSRectProgressBar::getMaxProgress() const { return maxProgress; }
float EOGSRectProgressBar::getProgress() const { return progress; }
DrawColor EOGSRectProgressBar::getBorderColor() const { return borderColor; }
DrawColor EOGSRectProgressBar::getIndicatorColor() const { return indicatorColor; }
bool EOGSRectProgressBar::getPercentageTextEnabled() const { return percentageLabel != nullptr; }
uint8_t EOGSRectProgressBar::getBorderRadius() const { return borderRadius; }
bool EOGSRectProgressBar::getBorderFilled() const { return borderFill; }
bool EOGSRectProgressBar::getIndicatorFilled() const { return indicatorFill; }

// 文本格式化方法
EOGSRectProgressBar* EOGSRectProgressBar::setTextFormat(std::function<std::string(float, float, float)> formatFunc) { 
    textFormat = formatFunc; 
    // 更新文本显示
    setProgress(progress);
    return this; 
}

std::function<std::string(float, float, float)> EOGSRectProgressBar::getTextFormat() const { return textFormat; }
uint8_t EOGSRectProgressBar::getIndicatorRadius() const { return indicatorRadius; }

#undef indicatorDimensionUpdated
#undef borderFilled
#undef indicatorFilled
#undef borderColor
#undef indicatorColor