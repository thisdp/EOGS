//
// Created by thisdp 2025/8/20.
//

#include "EOGS.h"
#include <algorithm>
#include <Arduino.h>

//底层绘制函数必须使用uint16_t，因为浮点运算很慢且u8g2使用的是int16_t
EOGS::EOGS(EOGS_HAL& _hal, int16_t scrWidth, int16_t scrHeight, uint32_t targetFPS)
    : currentWidth(scrWidth), currentHeight(scrHeight), hal(&_hal),
      screen(0,0,scrWidth,scrHeight),
      lastFrameTime(0), frameCount(0), fps(0), lastFpsUpdate(0), targetFrameTime(0) {
        screen.setEOGS(this);
        targetFrameTime = targetFPS > 0? 1000000 / targetFPS : 0; // 微秒
      }

EOGS::~EOGS() {}

// 更新FPS计算
void EOGS::render() {
    uint32_t currentTime = hal->micros();
    if (currentTime - lastFpsUpdate >= 1000000) { // 每秒更新一次FPS
        fps = frameCount;
        frameCount = 0;
        lastFpsUpdate = currentTime;
    }
    if (currentTime - lastFrameTime >= targetFrameTime) {
        if(preRender) preRender(this);
        hal->setFontPosTop();
        updateAnimations();
        hal->canvasClear();
        screen.render(this,0,0,screen.getAbsW(),screen.getAbsH());
        hal->canvasUpdate();
        frameCount++;
        lastFrameTime = currentTime;
    }
}

// 动画更新
void EOGS::updateAnimations() {
    for (auto anim : animations)
        anim->update(this);
}

// 清屏
void EOGS::clear() {
    hal->canvasClear();
}

// 设置绘制颜色类型
void EOGS::setDrawColor(DrawColor type) {
    hal->setDrawColor(static_cast<uint8_t>(type));
}

// 绘制像素点
void EOGS::drawPixel(int16_t x, int16_t y) {
    hal->drawPixel(x, y);
}

// 绘制英文文本
void EOGS::drawText(int16_t x, int16_t y, const std::string& text) {
    hal->drawText(x, y, text);
}

// 绘制UTF8文本
void EOGS::drawUTF8Text(int16_t x, int16_t y, const std::string& text) {
    hal->drawUTF8Text(x, y, text);
}

// 绘制垂直虚线
void EOGS::drawVDottedLine(int16_t x, int16_t y, int16_t h) {
    hal->drawVDottedLine(x, y, h);
}

// 绘制水平虚线
void EOGS::drawHDottedLine(int16_t x, int16_t y, int16_t l) {
    hal->drawHDottedLine(x, y, l);
}

// 绘制垂直线
void EOGS::drawVLine(int16_t x, int16_t y, int16_t h) {
    hal->drawVLine(x, y, h);
}

// 绘制水平线
void EOGS::drawHLine(int16_t x, int16_t y, int16_t l) {
    hal->drawHLine(x, y, l);
}

// 绘制任意线条
void EOGS::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    hal->drawLine(x1, y1, x2, y2);
}

// 绘制矩形框
void EOGS::drawFrame(int16_t x, int16_t y, int16_t w, int16_t h) {
    if(!(w && h)) return;
    hal->drawFrame(x, y, w, h);
}

// 绘制图像
void EOGS::drawBMP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* bitMap) {
    if(!(w && h)) return;
    if(bitMap == nullptr) return;
    hal->drawBMP(x, y, w, h,bitMap);
}

// 绘制填充矩形
void EOGS::drawBox(int16_t x, int16_t y, int16_t w, int16_t h) {
    if(!(w && h)) return;
    hal->drawBox(x, y, w, h);
}

// 绘制圆角矩形框
void EOGS::drawRFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) {
    if(!(w && h)) return;
    r = min(r, min(w, h));
    hal->drawRFrame(x, y, w, h, r);
}

// 绘制填充圆角矩形
void EOGS::drawRBox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) {
    if(!(w && h)) return;
    r = min(r, min(w, h));
    hal->drawRBox(x, y, w, h, r);
}

// 绘制圆形框
void EOGS::drawCircle(int16_t x, int16_t y, int16_t r) {
    if(!r) return;
    hal->drawCircle(x, y, r);
}

// 绘制填充圆形
void EOGS::drawDisc(int16_t x, int16_t y, int16_t r) {
    if(!r) return;
    hal->drawDisc(x, y, r);
}

// 绘制圆弧
void EOGS::drawArc(int16_t x, int16_t y, int16_t r, int16_t start, int16_t end){
    if(!r || (start == end)) return;
    hal->drawArc(x, y, r, start, end);
}

// 绘制填充三角形
void EOGS::drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) {
    hal->drawTriangle(x1, y1, x2, y2, x3, y3);
}

// 设置渲染区域
void EOGS::setClipWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    hal->setClipWindow(x0, y0, x1, y1);
}

void EOGS::setMaxClipWindow() {
    hal->setMaxClipWindow();
}

// 设置字体
void EOGS::setFont(const unsigned char* font) {
    hal->setFont(font);
}

void EOGS::setFontMode(uint8_t fontMode) {
    hal->setFontMode(fontMode);
}

// 获取字体宽度
unsigned char EOGS::getFontWidth(const std::string& text) {
    return hal->getFontWidth(const_cast<std::string&>(text));
}

unsigned char EOGS::getUTF8FontWidth(const std::string& text) {
    return hal->getUTF8FontWidth(const_cast<std::string&>(text));
}

// 获取字体高度
unsigned char EOGS::getFontHeight() {
    return hal->getFontHeight();
}

void EOGS::setFontPosBaseline() {
    hal->setFontPosBaseline();
}

void EOGS::setFontPosBottom() {
    hal->setFontPosBottom();
}

void EOGS::setFontPosTop() {
    hal->setFontPosTop();
}

void EOGS::setFontPosCenter() {
    hal->setFontPosCenter();
}

void EOGS::setFPSLimit(uint32_t fps) {
    targetFrameTime = fps > 0 ? 1000000 / fps : 0; // 微秒
}
