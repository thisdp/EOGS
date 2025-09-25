//
// Created by thisdp on 2025/8/20.
//
/*移植注意事项
1. 移植时请新建一个文件，继承EOGS_HAL类，并实现所有函数。
2. 系统并不知道字体的锚点位置，所以需要将字体锚点设置为左上角处，否则会出现文字偏移。
*/
#pragma once
#ifndef EOGS_HAL_H_
#define EOGS_HAL_H_

#include <string>

enum class HAlign : uint8_t {
    LEFT,
    CENTER,
    RIGHT
};
enum class VAlign : uint8_t {
    TOP,
    CENTER,
    BOTTOM
};

enum class DrawColor : uint8_t {
    BLACK = 0,
    WHITE = 1,
    XOR = 2,
    None = 3,
};

/**
 * @brief hardware abstraction layer.
 * @note provide a set of interfaces to access hardware.
 * @note provide some default implementation.
 */
class EOGS_MCU{
public:
  virtual uint32_t millis() { return 0; }
  virtual uint32_t micros() { return 0; }
  virtual void delay(uint32_t _mill) = 0;
  virtual void delayMicroseconds(uint32_t _micros) = 0;
};

class EOGS_HAL : public EOGS_MCU {
public:

  EOGS_HAL(uint16_t _arcMaxAngle = 255) : arcMaxAngle(_arcMaxAngle){};
  virtual ~EOGS_HAL() = default;
  uint16_t arcMaxAngle = 255;
  uint16_t getArcMaxAngle() { return arcMaxAngle; }
public:
  virtual void *getCanvasBuffer() { return 0; }
  virtual uint16_t getBufferTileHeight() { return 0; }
  virtual uint16_t getBufferTileWidth() { return 0; }
  virtual void canvasUpdate() = 0;
  virtual void canvasClear() = 0;
  virtual void setFont(const uint8_t *_font) = 0;
  virtual void setFontMode(uint8_t fontMode) = 0;
  virtual void setFontDirection(uint8_t fontDirection) = 0;
  virtual uint16_t getFontWidth(std::string &_text) { return 0; }
  virtual uint16_t getUTF8FontWidth(std::string &_text) { return 0; }
  virtual uint16_t getFontHeight() { return 0; }
  virtual void setFontPosBaseline() = 0;
  virtual void setFontPosBottom() = 0;
  virtual void setFontPosTop() = 0;
  virtual void setFontPosCenter() = 0;
  virtual void setDrawColor(uint8_t _type) = 0;

  //notice: _x和_y是字体左下角的坐标 _x and _y is the coordinate the lower left corner of the font
  virtual void drawText(int16_t x, int16_t y, const std::string &text) = 0;
  virtual void drawUTF8Text(int16_t x, int16_t y, const std::string &text) = 0;
  virtual void drawPixel(int16_t x, int16_t y) = 0;
  virtual void drawVDottedLine(int16_t x, int16_t y, int16_t h) = 0;
  virtual void drawHDottedLine(int16_t x, int16_t y, int16_t l) = 0;
  virtual void drawVLine(int16_t x, int16_t y, int16_t h) = 0;
  virtual void drawHLine(int16_t x, int16_t y, int16_t l) = 0;
  virtual void drawBMP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *bitMap) = 0;
  virtual void drawBox(int16_t x, int16_t y, int16_t w, int16_t h) = 0;
  virtual void drawRBox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) = 0;
  virtual void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h) = 0;
  virtual void drawRFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) = 0;
  virtual void drawCircle(int16_t x, int16_t y, int16_t r) = 0;
  virtual void drawDisc(int16_t x, int16_t y, int16_t r) = 0;
  virtual void drawArc(int16_t x, int16_t y, int16_t r, int16_t start, int16_t end) = 0;
  virtual void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) = 0;
  virtual void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) = 0;
  virtual void setClipWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) = 0;
  virtual void setMaxClipWindow() = 0;

};

#endif //EOGS_HAL_H_