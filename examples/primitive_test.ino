// EOGS Primitive Widget Test
// This example demonstrates how to use the new EOGS Primitive widget with different primitive types

#include <Arduino.h>
// #include "EOGS.h"
// #include "EOGS_U8G2_HAL.h"
// #include "widgets/primitive.h"

// U8G2库的初始化代码（根据你的显示屏类型调整）
// 例如，对于SSD1306 128x64显示屏：
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// EOGS_U8G2_HAL hal(u8g2);
// EOGS eogs(hal, 128, 64);

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化显示屏和EOGS
  // u8g2.begin();
  // eogs.clear();
  
  Serial.println("EOGS Primitive Widget Test");
  
  // 创建一个点列表
  // EOGSPrimitive* pointList = eogs.create<EOGSPrimitive>(10, 10, 50, 30, EOGSPrimitiveType::POINT_LIST);
  // pointList->setColor(DrawColor::WHITE);
  // pointList->addPoint(0.0f, 0.0f);
  // pointList->addPoint(0.5f, 0.5f);
  // pointList->addPoint(1.0f, 1.0f);
  
  // 创建一个线段列表
  // EOGSPrimitive* lineList = eogs.create<EOGSPrimitive>(70, 10, 50, 30, EOGSPrimitiveType::LINE_LIST);
  // lineList->setColor(DrawColor::WHITE);
  // lineList->addPoint(0.0f, 0.0f);
  // lineList->addPoint(1.0f, 1.0f);
  // lineList->addPoint(0.0f, 1.0f);
  // lineList->addPoint(1.0f, 0.0f);
  
  // 创建一个连续线段
  // EOGSPrimitive* lineStrip = eogs.create<EOGSPrimitive>(10, 50, 50, 30, EOGSPrimitiveType::LINE_STRIP);
  // lineStrip->setColor(DrawColor::WHITE);
  // lineStrip->addPoint(0.0f, 0.0f);
  // lineStrip->addPoint(0.5f, 1.0f);
  // lineStrip->addPoint(1.0f, 0.0f);
  
  // 创建一个三角形列表
  // EOGSPrimitive* triangleList = eogs.create<EOGSPrimitive>(70, 50, 50, 30, EOGSPrimitiveType::TRIANGLE_LIST);
  // triangleList->setColor(DrawColor::WHITE);
  // triangleList->setFilled(true);
  // triangleList->addPoint(0.0f, 0.0f);
  // triangleList->addPoint(1.0f, 0.0f);
  // triangleList->addPoint(0.5f, 1.0f);
  
  Serial.println("Primitive widgets created");
}

void loop() {
  // 渲染EOGS界面
  // eogs.render();
  
  // 这里可以添加动画或其他交互逻辑
  delay(100);
}