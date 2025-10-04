//
// Created by thisdp 2025/8/20.
//
#pragma once

#include "eogs_hal/eogs_hal.h"
#include "U8g2lib.h"

// U8G2驱动类
class U8G2_EOGS_HAL : public EOGS_HAL {
private:

    U8G2* u8g2Interface;
    
public:
    //MCU函数
    void delay(uint32_t _mill) override {
        ::delay(_mill);
    }

    void delayMicroseconds(uint32_t _micros) override {
        ::delayMicroseconds(_micros);
    }
    
    uint32_t millis() override {
        return ::millis();
    }

    uint32_t micros() override {
        return ::micros();
    }

    //屏幕
    U8G2_EOGS_HAL(U8G2& _u8g2) : EOGS_HAL(), u8g2Interface(&_u8g2) {}
    
    ~U8G2_EOGS_HAL() = default;

    const uint8_t *getDefaultFont() override {
        return u8g2_font_t0_11_tf;  // 默认字体
    }
    
    void *getCanvasBuffer() override {
        return u8g2Interface->getBufferPtr();
    }
    
    uint16_t getBufferTileHeight() override {
        return u8g2Interface->getBufferTileHeight();
    }
    
    uint16_t getBufferTileWidth() override {
        return u8g2Interface->getBufferTileWidth();
    }
    
    void canvasUpdate() override {
        u8g2Interface->sendBuffer();
    }
    
    void canvasClear() override {
        u8g2Interface->clearBuffer();
    }
    
    void setFont(const uint8_t *_font) override {
        u8g2Interface->setFont(_font);
    }
    
    void setFontMode(uint8_t fontMode) override {
        u8g2Interface->setFontMode(fontMode);
    }
    
    void setFontDirection(uint8_t fontDirection) override {
        u8g2Interface->setFontDirection(fontDirection);
    }

    uint16_t getFontWidth(std::string &_text) override {
        return u8g2Interface->getStrWidth(_text.c_str());
    }

    uint16_t getUTF8FontWidth(std::string &_text) override {
        return u8g2Interface->getUTF8Width(_text.c_str());
    }
    
    uint16_t getFontHeight() override {
        return u8g2Interface->getAscent() - u8g2Interface->getDescent();
    }

    void setFontPosBaseline() override {
        u8g2Interface->setFontPosBaseline();
    }

    void setFontPosBottom() override {
        u8g2Interface->setFontPosBottom();
    }

    void setFontPosTop() override {
        u8g2Interface->setFontPosTop();
    }

    void setFontPosCenter() override {
        u8g2Interface->setFontPosCenter();
    }
    
    void setDrawColor(unsigned char _type) override {
        u8g2Interface->setDrawColor(_type);
    }
    
    void drawPixel(int16_t x, int16_t y) override {
        u8g2Interface->drawPixel(x, y);
    }
    
    void drawText(int16_t x, int16_t y, const std::string &text) override {
        u8g2Interface->drawStr(x, y, text.c_str());
    }
    
    void drawUTF8Text(int16_t x, int16_t y, const std::string &text) override {
        u8g2Interface->drawUTF8(x, y, text.c_str());
    }
    
    void drawVDottedLine(int16_t x, int16_t y, int16_t h) override {
        for (int i = 0; i < (int)h; i += 2) {
            u8g2Interface->drawPixel((int16_t)x, (int16_t)(y + i));
        }
    }
    
    void drawHDottedLine(int16_t x, int16_t y, int16_t l) override {
        for (int i = 0; i < (int)l; i += 2) {
            u8g2Interface->drawPixel((int16_t)x, (int16_t)(y + i));
        }
    }
    
    void drawVLine(int16_t x, int16_t y, int16_t h) override {
        u8g2Interface->drawVLine(x, y, h);
    }
    
    void drawHLine(int16_t x, int16_t y, int16_t l) override {
        u8g2Interface->drawHLine(x, y, l);
    }
    
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1){
        u8g2Interface->drawLine(x0, y0, x1, y1);
    }

    void drawBMP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *bitMap) override {
        u8g2Interface->drawXBM(x, y, w, h, bitMap);
    }
    
    void drawBox(int16_t x, int16_t y, int16_t w, int16_t h) override {
        u8g2Interface->drawBox(x, y, w, h);
    }
    
    void drawRBox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) override {
        u8g2Interface->drawRBox(x, y, w, h, r);
    }
    
    void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h) override {
        u8g2Interface->drawFrame(x, y, w, h);
    }
    
    void drawRFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) override {
        u8g2Interface->drawRFrame(x, y, w, h, r);
    }

    void drawCircle(int16_t x, int16_t y, int16_t r) override {
        u8g2Interface->drawCircle(x, y, r);
    }

    void drawDisc(int16_t x, int16_t y, int16_t r) override {
        u8g2Interface->drawDisc(x, y, r);
    }
    
    void drawArc(int16_t x, int16_t y, int16_t r, int16_t start, int16_t end) override{
        u8g2Interface->drawArc(x, y, r+1, start, end);
    }

    void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) override {
        u8g2Interface->drawTriangle(x1, y1, x2, y2, x3, y3);
    }

    void setClipWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override {
        if(x0<0) x0 = 0;
        if(x1<0) x1 = 0;
        if(y0<0) y0 = 0;
        if(y1<0) y1 = 0;
        u8g2Interface->setClipWindow(x0, y0, x1, y1);
    }

    void setMaxClipWindow() override {
        u8g2Interface->setMaxClipWindow();
    }
    
};