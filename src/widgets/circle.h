//
// Created by thisdp 2025/8/22.
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
    EOGSCircle(float _x, float _y, float _w, float _h, bool _isRelative = false, bool _filled = true)
        : EOGSWidget<EOGSCircle>(_x, _y, _w, _h, _isRelative), radius(1) {
            filled = _filled;
            setColor(DrawColor::WHITE);
        }
    ~EOGSCircle() = default;

    bool updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (renderPosUpdated) return false; renderPosUpdated = true;
        if (eogs == nullptr) return false;
        renderX = parentRX+x;
        renderY = parentRY+y;
        insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
        cx = renderX+w/2;
        cy = renderY+h/2;
        renderRadius = radius*std::min(w, h)/2;
        return true;
    }
    
    void render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) override {
        if (!visible || eogs == nullptr) return;
        updateDimension(parentW, parentH);
        updateRenderPos(eogs, parentRX, parentRY, parentW, parentH);
        if(insideOfRenderArea || renderOutsideOfRenderArea){
            eogs->setDrawColor(drawColor);
            if (filled)
                eogs->drawDisc(cx, cy, renderRadius);
            else
                eogs->drawCircle(cx, cy, renderRadius);
        }
    }
    
    DrawColor getColor() const { return drawColor; }
    EOGSCircle* setColor(DrawColor _drawColor){
        drawColor = _drawColor;
        return this;
    }
    
    bool isFilled() const { return filled; }
    EOGSCircle* setFilled(bool _filled) { filled = _filled; return this; }
    int16_t getAbsoluteRadius() const { return renderRadius; }
    float getRelativeRadius() const { return radius; }
    EOGSCircle* setRelativeRadius(float _radius) { radius = _radius; return this; }
};

#undef filled 
#undef drawColor