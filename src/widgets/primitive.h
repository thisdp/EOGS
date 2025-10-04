//
// Created by thisdp 2025/8/22.
// Primitive Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include <vector>

// Flag 1
#define filled generalFlag1
// Flag 2~3
#define drawColor generalColor2

// 图元类型枚举
enum class EOGSPrimitiveType {
    POINT_LIST,      // 点列表
    LINE_LIST,       // 线段列表
    LINE_STRIP,      // 连续线段
    TRIANGLE_LIST,   // 三角形列表
    TRIANGLE_STRIP,  // 三角形带
    TRIANGLE_FAN     // 三角形扇
};

// 点结构体，用于表示图元上的点
struct EOGSPrimitivePoint {
    float x;          // x坐标
    float y;          // y坐标
    bool relative;    // 是否为相对坐标（相对于primitive的w和h）
    
    EOGSPrimitivePoint(float _x, float _y, bool _relative = true) 
        : x(_x), y(_y), relative(_relative) {}
};

class EOGSPrimitive : public EOGSWidget<EOGSPrimitive> {
protected:
    std::vector<EOGSPrimitivePoint> points;  // 点集
    EOGSPrimitiveType primitiveType;         // 图元类型

public:
    // 构造函数
    EOGSPrimitive(float _x, float _y, float _w, float _h, bool _isRelative = false, EOGSPrimitiveType _type = EOGSPrimitiveType::LINE_LIST);

    ~EOGSPrimitive() = default;
    
    // 更新控件尺寸和位置
    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
private:
    // 根据图元类型绘制
    void drawPrimitive(EOGS* eogs);
    
    // 将相对坐标转换为绝对坐标
    int16_t getXCoord(float x, bool relative);
    
    int16_t getYCoord(float y, bool relative);
    
    // 绘制点列表
    void drawPointList(EOGS* eogs);
    
    // 绘制线段列表
    void drawLineList(EOGS* eogs);
    
    // 绘制连续线段
    void drawLineStrip(EOGS* eogs);
    
    // 绘制三角形列表
    void drawTriangleList(EOGS* eogs);
    
    // 绘制三角形带
    void drawTriangleStrip(EOGS* eogs);
    
    // 绘制三角形扇
    void drawTriangleFan(EOGS* eogs);
    
public:
    // 颜色设置
    DrawColor getColor() const;
    EOGSPrimitive* setColor(DrawColor _drawColor);
    
    // 填充设置 仅对triangle有效
    bool isFilled() const;
    EOGSPrimitive* setFilled(bool _filled);
    
    // 图元类型设置
    EOGSPrimitiveType getPrimitiveType() const;
    EOGSPrimitive* setPrimitiveType(EOGSPrimitiveType _type);
    
    // 点集操作
    EOGSPrimitive* clearPoints();
    
    EOGSPrimitive* addPoint(float x, float y, bool relative);
    
    EOGSPrimitive* addPoint(const EOGSPrimitivePoint& point);
    
    EOGSPrimitive* removePoint(size_t index);

    EOGSPrimitive* setPointPosition(uint16_t index, float x, float y, bool relative);
    
    size_t getPointCount() const;
    
    EOGSPrimitivePoint* getPoint(size_t index);
};

#undef filled
#undef drawColor
