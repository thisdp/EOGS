#include "primitive.h"

// Flag 1
#define filled generalFlag1
// Flag 2~3
#define drawColor generalColor2

EOGSPrimitive::EOGSPrimitive(float _x, float _y, float _w, float _h, bool _isRelative, EOGSPrimitiveType _type)
    : EOGSWidget<EOGSPrimitive>(_x, _y, _w, _h, _isRelative), primitiveType(_type) {
    setFilled(true);
    setColor(DrawColor::WHITE);
}

// 更新控件尺寸和位置
bool EOGSPrimitive::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;

    if (eogs == nullptr) return false;
    renderX = parentRX + x;
    renderY = parentRY + y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    return true;
}

void EOGSPrimitive::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (!visible || eogs == nullptr) return;
    updateDimension(parentW, parentH);
    updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
    if (insideOfRenderArea || renderOutsideOfRenderArea) {
        eogs->setDrawColor(drawColor);
        
        // 根据图元类型绘制
        drawPrimitive(eogs);
    }
}

// 根据图元类型绘制
void EOGSPrimitive::drawPrimitive(EOGS* eogs) {
    if (points.size() < 1) return;
    
    switch (primitiveType) {
        case EOGSPrimitiveType::POINT_LIST:
            drawPointList(eogs);
            break;
        case EOGSPrimitiveType::LINE_LIST:
            drawLineList(eogs);
            break;
        case EOGSPrimitiveType::LINE_STRIP:
            drawLineStrip(eogs);
            break;
        case EOGSPrimitiveType::TRIANGLE_LIST:
            drawTriangleList(eogs);
            break;
        case EOGSPrimitiveType::TRIANGLE_STRIP:
            drawTriangleStrip(eogs);
            break;
        case EOGSPrimitiveType::TRIANGLE_FAN:
            drawTriangleFan(eogs);
            break;
    }
}

// 将相对坐标转换为绝对坐标
int16_t EOGSPrimitive::getXCoord(float x, bool relative) {
    if (relative) {
        return renderX + static_cast<int16_t>(x * w);
    } else {
        return renderX + static_cast<int16_t>(x);
    }
}

int16_t EOGSPrimitive::getYCoord(float y, bool relative) {
    if (relative) {
        return renderY + static_cast<int16_t>(y * h);
    } else {
        return renderY + static_cast<int16_t>(y);
    }
}

// 绘制点列表
void EOGSPrimitive::drawPointList(EOGS* eogs) {
    for (const auto& point : points) {
        int16_t px = getXCoord(point.x, point.relative);
        int16_t py = getYCoord(point.y, point.relative);
        eogs->drawPixel(px, py);
    }
}

// 绘制线段列表
void EOGSPrimitive::drawLineList(EOGS* eogs) {
    for (size_t i = 0; i + 1 < points.size(); i += 2) {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];
        
        int16_t x1 = getXCoord(p1.x, p1.relative);
        int16_t y1 = getYCoord(p1.y, p1.relative);
        int16_t x2 = getXCoord(p2.x, p2.relative);
        int16_t y2 = getYCoord(p2.y, p2.relative);
        
        eogs->drawLine(x1, y1, x2, y2);
    }
}

// 绘制连续线段
void EOGSPrimitive::drawLineStrip(EOGS* eogs) {
    if (points.size() < 2) return;
    
    for (size_t i = 0; i + 1 < points.size(); i++) {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];
        
        int16_t x1 = getXCoord(p1.x, p1.relative);
        int16_t y1 = getYCoord(p1.y, p1.relative);
        int16_t x2 = getXCoord(p2.x, p2.relative);
        int16_t y2 = getYCoord(p2.y, p2.relative);
        
        eogs->drawLine(x1, y1, x2, y2);
    }
}

// 绘制三角形列表
void EOGSPrimitive::drawTriangleList(EOGS* eogs) {
    for (size_t i = 0; i + 2 < points.size(); i += 3) {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];
        const auto& p3 = points[i + 2];
        
        int16_t x1 = getXCoord(p1.x, p1.relative);
        int16_t y1 = getYCoord(p1.y, p1.relative);
        int16_t x2 = getXCoord(p2.x, p2.relative);
        int16_t y2 = getYCoord(p2.y, p2.relative);
        int16_t x3 = getXCoord(p3.x, p3.relative);
        int16_t y3 = getYCoord(p3.y, p3.relative);
        
        if (filled) {
            eogs->drawTriangle(x1, y1, x2, y2, x3, y3);
        } else {
            // 绘制三角形边框
            eogs->drawLine(x1, y1, x2, y2);
            eogs->drawLine(x2, y2, x3, y3);
            eogs->drawLine(x3, y3, x1, y1);
        }
    }
}

// 绘制三角形带
void EOGSPrimitive::drawTriangleStrip(EOGS* eogs) {
    if (points.size() < 3) return;
    
    for (size_t i = 0; i + 2 < points.size(); i++) {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];
        const auto& p3 = points[i + 2];

        int16_t x1 = getXCoord(p1.x, p1.relative);
        int16_t y1 = getYCoord(p1.y, p1.relative);
        int16_t x2 = getXCoord(p2.x, p2.relative);
        int16_t y2 = getYCoord(p2.y, p2.relative);
        int16_t x3 = getXCoord(p3.x, p3.relative);
        int16_t y3 = getYCoord(p3.y, p3.relative);

        if (filled) {
            eogs->drawTriangle(x1, y1, x2, y2, x3, y3);
        } else {
            // 绘制三角形边框，避免重复绘制共用边
            // 第一个三角形绘制所有边
            if (i == 0) {
                eogs->drawLine(x1, y1, x2, y2);
                eogs->drawLine(x2, y2, x3, y3);
                eogs->drawLine(x3, y3, x1, y1);
            } else {
                // 后续三角形只需要绘制新边
                eogs->drawLine(x2, y2, x3, y3);
                eogs->drawLine(x3, y3, x1, y1);
            }
        }
    }
}

// 绘制三角形扇
void EOGSPrimitive::drawTriangleFan(EOGS* eogs) {
    if (points.size() < 3) return;
    
    // 第一个点作为中心点
    const auto& center = points[0];
    int16_t centerX = getXCoord(center.x, center.relative);
    int16_t centerY = getYCoord(center.y, center.relative);
    
    for (size_t i = 1; i + 1 < points.size(); i++) {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];
        
        int16_t x1 = getXCoord(p1.x, p1.relative);
        int16_t y1 = getYCoord(p1.y, p1.relative);
        int16_t x2 = getXCoord(p2.x, p2.relative);
        int16_t y2 = getYCoord(p2.y, p2.relative);
        
        if (filled) {
            eogs->drawTriangle(centerX, centerY, x1, y1, x2, y2);
        } else {
            // 绘制三角形边框，避免重复绘制共用边
            // 每个三角形需要绘制两条新边（第三条边是中心点到第一个点，已经在前一个三角形中绘制）
            eogs->drawLine(centerX, centerY, x1, y1);
            eogs->drawLine(x1, y1, x2, y2);
            eogs->drawLine(x2, y2, centerX, centerY);
        }
    }
}

// 颜色设置
DrawColor EOGSPrimitive::getColor() const { return drawColor; }
EOGSPrimitive* EOGSPrimitive::setColor(DrawColor _drawColor) {
    drawColor = _drawColor;
    return this;
}

// 填充设置 仅对triangle有效
bool EOGSPrimitive::isFilled() const { return filled; }
EOGSPrimitive* EOGSPrimitive::setFilled(bool _filled) {
    filled = _filled;
    return this;
}

// 图元类型设置
EOGSPrimitiveType EOGSPrimitive::getPrimitiveType() const { return primitiveType; }
EOGSPrimitive* EOGSPrimitive::setPrimitiveType(EOGSPrimitiveType _type) {
    primitiveType = _type;
    return this;
}

// 点集操作
EOGSPrimitive* EOGSPrimitive::clearPoints() {
    points.clear();
    return this;
}

EOGSPrimitive* EOGSPrimitive::addPoint(float x, float y, bool relative) {
    points.emplace_back(x, y, relative);
    return this;
}

EOGSPrimitive* EOGSPrimitive::addPoint(const EOGSPrimitivePoint& point) {
    points.push_back(point);
    return this;
}

EOGSPrimitive* EOGSPrimitive::removePoint(size_t index) {
    if (index < points.size()) {
        points.erase(points.begin() + index);
    }
    return this;
}

EOGSPrimitive* EOGSPrimitive::setPointPosition(uint16_t index, float x, float y, bool relative) {
    if(index >= points.size()) return this;
    points[index].x = x;
    points[index].y = y;
    points[index].relative = relative;
    return this;
}

size_t EOGSPrimitive::getPointCount() const {
    return points.size();
}

EOGSPrimitivePoint* EOGSPrimitive::getPoint(size_t index) {
    if (index < points.size()) {
        return &points[index];
    }
    return nullptr;
}


#undef filled
#undef drawColor