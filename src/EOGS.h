//
// Created by thisdp 2025/8/20.
// Embedded OLED Graphical System (EOGS) - A lightweight OLED GUI library for embedded systems
//
#pragma once

// 包含核心组件
#include <vector>
#include "widgets/widget.h"
#include "eogs_hal/eogs_hal.h"
#include "widgets/screen.h"
#include "animation/animation.h"
#include "input/input.h"

using EventPreRender = void(*)(EOGS*);
class EOGSAnimBase;
// EOGS主类
class EOGS {
private:
    int16_t currentWidth, currentHeight;  // 当前屏幕尺寸
    EOGS_HAL* hal;
    EOGSScreen screen;  // 根容器
    std::vector<EOGSAnimBase*> animations;  // 动画列表
    uint32_t lastFrameTime;  // 上一帧的时间戳
    uint32_t frameCount;     // 帧计数器
    uint32_t fps;            // 当前FPS
    uint32_t lastFpsUpdate;  // 上次FPS更新时间 (1s)
    uint32_t targetFrameTime; // 目标帧时间（微秒）
public:
    EventPreRender preRender = 0;
    EOGS(EOGS_HAL& _hal, int16_t scrWidth, int16_t scrHeight, uint32_t targetFPS = 0);
    ~EOGS();
    EOGSScreen* getScreen() { return &screen; }
    void addAnimation(EOGSAnimBase* anim) { if (anim != nullptr) animations.push_back(anim); }
    // 渲染根控件
    void render();
    // 动画更新
    void updateAnimations();
    // 清屏
    void clear();
    // 获取HAL实例
    EOGS_HAL* getHAL() const { return hal; }
    // 获取当前屏幕尺寸
    int16_t getCurrentWidth() const { return currentWidth; }
    int16_t getCurrentHeight() const { return currentHeight; }
    // 获取默认字体
    const uint8_t *getDefaultFont();
    // 设置绘制颜色
    void setDrawColor(DrawColor type);
    // 绘制像素点
    void drawPixel(int16_t x, int16_t y);
    // 绘制ASCII文本
    void drawText(int16_t x, int16_t y, const std::string& text);
    // 绘制UTF8文本
    void drawUTF8Text(int16_t x, int16_t y, const std::string& text);
    // 绘制垂直虚线
    void drawVDottedLine(int16_t x, int16_t y, int16_t h);
    // 绘制水平虚线
    void drawHDottedLine(int16_t x, int16_t y, int16_t l);
    // 绘制垂直线
    void drawVLine(int16_t x, int16_t y, int16_t h);
    // 绘制水平线
    void drawHLine(int16_t x, int16_t y, int16_t l);
    // 绘制矩形框
    void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h);
    // 绘制图像
    void drawBMP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* bitMap);
    // 绘制填充矩形
    void drawBox(int16_t x, int16_t y, int16_t w, int16_t h);
    // 绘制圆角矩形框
    void drawRFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
    // 绘制填充圆角矩形
    void drawRBox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
    // 绘制圆形框
    void drawCircle(int16_t x, int16_t y, int16_t r);
    // 绘制填充圆形
    void drawDisc(int16_t x, int16_t y, int16_t r);
    // 绘制圆弧
    void drawArc(int16_t x, int16_t y, int16_t r, int16_t start, int16_t end);
    // 绘制任意线条
    void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    // 绘制填充三角形
    void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
    // 设置渲染区域
    void setClipWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void setMaxClipWindow();
    // 设置字体
    void setFont(const unsigned char* font);
    void setFontMode(uint8_t fontMode);
    void setFontDirection(uint8_t fontDirection);
    void setFontPosBaseline();
    void setFontPosBottom();
    void setFontPosTop();
    void setFontPosCenter();
    // 获取字体宽度
    unsigned char getFontWidth(const std::string& text);
    unsigned char getUTF8FontWidth(const std::string& text);
    // 获取字体高度
    unsigned char getFontHeight();
    
    // 模板方法，用于创建任何类型的Widget
    template<typename WidgetType, typename... Args>
    [[nodiscard]] WidgetType* create(Args&&... args) {
        WidgetType* newWidget = new WidgetType(std::forward<Args>(args)...);
        if(newWidget != nullptr) screen.addChild(newWidget);
        return newWidget;
    }
    template<typename AnimType, typename... Args>
    [[nodiscard]] AnimType* createAnim(Args&&... args) {
        AnimType* newAnim = new AnimType(std::forward<Args>(args)...);
        if (newAnim != nullptr) animations.push_back(newAnim);  //创建动画，但是不启动
        return newAnim;
    }
    
    void setFPSLimit(uint32_t fps);
    inline uint32_t getFPS() const { return fps; }
    
    inline void delay(uint32_t _mill) { hal->delay(_mill); }
    inline uint32_t millis() { return hal->millis(); }
    inline uint32_t micros() { return hal->micros(); }
};
