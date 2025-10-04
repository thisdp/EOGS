//
// Created by thisdp 2025/8/22.
// Circle Widget for EOGS
//
#pragma once

#include "../EOGS.h"
#include "widget.h"
#include <algorithm>

//Flag1
#define filled generalFlag1
//Flag2~3
#define drawColor generalColor2
class EOGSCircle : public EOGSWidget<EOGSCircle> {
protected:
    int16_t cx, cy;
    int16_t renderRadius;
    float radius;   // 0 ~ 1

public:
    EOGSCircle(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _filled = true);

    ~EOGSCircle() = default;

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override;
    
    DrawColor getColor() const;
    EOGSCircle* setColor(DrawColor _drawColor);
    
    bool isFilled() const;
    EOGSCircle* setFilled(bool _filled);
    int16_t getAbsoluteRadius() const;
    float getRelativeRadius() const;
    EOGSCircle* setRelativeRadius(float _radius);
};

#undef filled
#undef drawColor