# EOGS (Embedded OLED Graphical System) Wiki

## 概述

EOGS (Embedded OLED Graphical System) 是一个专为嵌入式系统设计的轻量级OLED图形用户界面库。它提供了一套完整的控件和工具，用于在微控制器项目中创建图形用户界面。

### 主要特性

- 轻量级且高效
- 支持多种OLED显示驱动（通过U8g2库）
- 提供可定制的控件集
- 支持动画效果
- 支持输入处理

## 快速开始

### 安装

1. 下载或克隆此仓库
2. 将EOGS文件夹放置在Arduino库目录中
3. 安装U8g2库（如果尚未安装）

### 基本用法

```cpp
#include <Arduino.h>
#include <U8g2lib.h>
#include "EOGS.h"
#include "EOGS_U8G2_HAL.h"
#include "widgets/switchbutton.h"

// U8g2构造函数（根据您的硬件配置修改）
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// EOGS HAL实例
U8G2_EOGS_HAL hal(u8g2);

// EOGS实例
EOGS eogs(hal, 128, 64);

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化U8g2
  u8g2.begin();
  
  // 创建一个开关按钮
  EOGSSwitchButton* switchBtn = eogs.create<EOGSSwitchButton>(10, 20, 20, 20, false);
  switchBtn->setChecked(false);
}

void loop() {
  // 渲染EOGS界面
  eogs.render();
  
  delay(100);
}
```

## 核心类

### EOGS类

EOGS是系统的核心类，负责管理整个图形界面系统。

#### 构造函数
```cpp
EOGS(EOGS_HAL& _hal, int16_t scrWidth, int16_t scrHeight, uint32_t targetFPS = 0);
```

#### 主要方法
- `void render()` - 渲染界面
- `EOGSScreen* getScreen()` - 获取屏幕根容器
- `void addAnimation(EOGSAnimBase* anim)` - 添加动画
- `template<typename WidgetType, typename... Args> WidgetType* create(Args&&... args)` - 创建控件

### EOGS_HAL类

硬件抽象层，为不同硬件平台提供统一接口。

#### 主要方法
- `virtual void drawPixel(int16_t x, int16_t y)` - 绘制像素点
- `virtual void drawText(int16_t x, int16_t y, const std::string &text)` - 绘制文本
- `virtual void drawBox(int16_t x, int16_t y, int16_t w, int16_t h)` - 绘制填充矩形
- `virtual void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h)` - 绘制矩形框

### U8G2_EOGS_HAL类

U8G2硬件抽象层实现，用于与U8g2库配合使用。

### EOGS_HAL类

硬件抽象层基类，为不同硬件平台提供统一接口。

#### 主要方法
- `virtual void drawPixel(int16_t x, int16_t y)` - 绘制像素点
- `virtual void drawText(int16_t x, int16_t y, const std::string &text)` - 绘制文本
- `virtual void drawBox(int16_t x, int16_t y, int16_t w, int16_t h)` - 绘制填充矩形
- `virtual void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h)` - 绘制矩形框
- `virtual void drawCircle(int16_t x, int16_t y, int16_t r)` - 绘制圆形
- `virtual void drawDisc(int16_t x, int16_t y, int16_t r)` - 绘制填充圆形
- `virtual void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)` - 绘制线条
- `virtual void setDrawColor(uint8_t _type)` - 设置绘制颜色
- `virtual void canvasUpdate()` - 更新显示
- `virtual void canvasClear()` - 清除画布

### 自定义HAL实现

要为其他显示驱动程序创建自定义HAL实现，需要继承EOGS_HAL类并实现所有纯虚函数。以下是一个基本框架：

```cpp
class Custom_EOGS_HAL : public EOGS_HAL {
public:
    Custom_EOGS_HAL(CustomDisplayDriver& driver) : displayDriver(driver) {}
    
    // 实现所有必需的虚函数
    void drawPixel(int16_t x, int16_t y) override {
        // 使用自定义驱动程序绘制像素点
    }
    
    void drawText(int16_t x, int16_t y, const std::string &text) override {
        // 使用自定义驱动程序绘制文本
    }
    
    // ... 其他函数的实现
    
private:
    CustomDisplayDriver& displayDriver;
};
```

## 控件系统

### 基础控件类 (EOGSWidgetBase)

所有控件的基类，提供基础功能：
- 位置和尺寸管理
- 父子关系管理
- 事件系统
- 渲染接口

### 控件列表

#### Label (标签)
用于显示文本的控件。

```cpp
// 创建标签
EOGSLabel* label = eogs.create<EOGSLabel>(0, 0, 128, 20, false, "Hello World");
label->setHAlign(HAlign::CENTER);  // 设置水平对齐
label->setVAlign(VAlign::MIDDLE);  // 设置垂直对齐
```

#### Rectangle (矩形)
用于绘制矩形的控件。

```cpp
// 创建矩形
EOGSRectangle* rect = eogs.create<EOGSRectangle>(10, 10, 50, 30, false);
rect->setFilled(true);  // 设置填充
rect->setColor(DrawColor::WHITE);  // 设置颜色
```

#### Circle (圆形)
用于绘制圆形的控件。

```cpp
// 创建圆形
EOGSCircle* circle = eogs.create<EOGSCircle>(50, 50, 30, 30, false);
circle->setFilled(true);  // 设置填充
circle->setColor(DrawColor::WHITE);  // 设置颜色
```

#### SwitchButton (开关按钮)
切换开关控件，用于表示开/关或启用/禁用状态。

```cpp
// 创建开关按钮
EOGSSwitchButton* switchBtn = eogs.create<EOGSSwitchButton>(10, 20, 20, 20, false);
switchBtn->setChecked(true);  // 设置初始状态

// 获取当前状态
bool state = switchBtn->getChecked();
```

#### CheckBox (复选框)
复选框控件，用于表示选中/未选中状态。

```cpp
// 创建复选框
EOGSCheckBox* checkBox = eogs.create<EOGSCheckBox>(10, 20, 20, 20, false);
checkBox->setChecked(true);  // 设置初始状态

// 获取当前状态
bool state = checkBox->getChecked();
```

#### RadioButton (单选按钮)
单选按钮控件，用于在一组选项中选择一个。

```cpp
// 创建单选按钮组
EOGSRadioButtonGroup radioGroup;

// 创建单选按钮
EOGSRadioButton* radio1 = eogs.create<EOGSRadioButton>(10, 20, 20, 20, false);
EOGSRadioButton* radio2 = eogs.create<EOGSRadioButton>(10, 50, 20, 20, false);

// 将单选按钮添加到组
radio1->setGroup(radioGroup);
radio2->setGroup(radioGroup);

// 设置初始选中状态
radio1->setChecked(true);
```

#### ProgressBar (进度条)
显示进度的控件，包括矩形和圆形两种类型。

```cpp
// 创建矩形进度条
EOGSRectProgressBar* rectProgress = eogs.create<EOGSRectProgressBar>(10, 10, 100, 20, false);
rectProgress->setProgress(50);  // 设置进度值

// 创建圆形进度条
EOGSCircleProgressBar* circleProgress = eogs.create<EOGSCircleProgressBar>(50, 50, 40, 40, false);
circleProgress->setProgress(75);  // 设置进度值
```

#### Menu (菜单)
菜单控件，用于创建层次化菜单系统。

```cpp
// 创建菜单
EOGSMenu* menu = eogs.create<EOGSMenu>(0, 0, 128, 64, false);

// 添加菜单项
EOGSMenuSubItem* item1 = menu->create<EOGSMenuSubItem>(0, 0, 128, 20, false, "Item 1");
EOGSMenuSubItem* item2 = menu->create<EOGSMenuSubItem>(0, 0, 128, 20, false, "Item 2");

// 添加子菜单项
menu->addChildMenuItem(item1);
menu->addChildMenuItem(item2);
```

#### ScrollBar (滚动条)
滚动条控件，用于在内容超出显示区域时进行滚动。

```cpp
// 创建垂直滚动条
EOGSScrollBar* scrollBar = eogs.create<EOGSScrollBar>(120, 0, 8, 64, false, true);
scrollBar->setScrollValue(25);  // 设置滚动值
```

#### Primitive (图元)
用于绘制基本图形图元的控件。

```cpp
// 创建图元
EOGSPrimitive* primitive = eogs.create<EOGSPrimitive>(0, 0, 128, 64, false, EOGSPrimitiveType::LINE_STRIP);

// 添加点
primitive->addPoint(10, 10, false);
primitive->addPoint(50, 30, false);
primitive->addPoint(100, 20, false);
```

## 动画系统

EOGS提供了一个灵活的动画系统，支持多种缓动函数。

### 内置缓动函数
- Linear (线性)
- OutSine (正弦输出)
- InSine (正弦输入)
- InOutSine (正弦输入输出)
- OutQuad (二次输出)
- InQuad (二次输入)
- InOutQuad (二次输入输出)
- OutBounce (弹跳输出)
- InBounce (弹跳输入)
- InOutBounce (弹跳输入输出)
- OutBack (回退输出)
- InBack (回退输入)
- InOutBack (回退输入输出)

### 动画类型
- EOGSAnimMove (移动动画)
- EOGSAnimSize (尺寸动画)

### 使用示例
```cpp
// 创建移动动画
EOGSAnimMove* moveAnim = eogs.createAnim<EOGSAnimMove>(widget, 0, 0, 50, 50, false, false, 1000, EOGSBuiltInEasing::OutSine());
moveAnim->start();  // 启动动画
```

## 事件系统

EOGS提供了一个事件系统，支持事件的触发和监听。事件系统支持事件传播，可以向上或向下传播事件。

### 预定义事件
- Confirm (确认事件)
- ProgressChange (进度变化事件)

### 事件传播模式
- None: 不传播事件
- Up: 向上传播事件
- Down: 向下传播事件
- Both: 双向传播事件

### 使用示例
```cpp
// 添加确认事件监听器
widget->on(EOGSEvent::Confirm, [](EOGSEvent* event) {
    Serial.println("Widget confirmed!");
});

// 添加进度变化事件监听器
widget->on(EOGSEvent::ProgressChange, [](EOGSEvent* event) {
    Serial.println("Progress changed!");
});

// 触发事件
EOGSEventConfirm confirmEvent;
widget->triggerEvent(&confirmEvent, EOGSEventPropagate::Up);

// 移除特定事件的所有监听器
widget->off(EOGSEvent::Confirm);

// 移除特定事件的指定监听器
EOGSEventListener* listener = widget->on(EOGSEvent::Confirm, [](EOGSEvent* event) {
    Serial.println("Widget confirmed!");
});
widget->off(EOGSEvent::Confirm, listener);

// 移除所有事件监听器
widget->off();
```

## 输入处理

EOGS提供了一套输入处理类，用于处理数字和模拟输入。

### EOGSDigitalButton
用于处理数字按钮输入，支持单击、双击、长按等操作。

### EOGSAnalogHysteresis
用于处理模拟输入，具有滞后特性以避免抖动。

## 许可证

本项目采用MIT许可证，详情请参见LICENSE文件。