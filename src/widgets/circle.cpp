#include "circle.h"

//Flag1
#define filled generalFlag1
//Flag2~3
#define drawColor generalColor2

EOGSCircle::EOGSCircle(float _x, float _y, float _w, float _h, bool _isRelative, bool _filled)
    : EOGSWidget<EOGSCircle>(_x, _y, _w, _h, _isRelative), radius(1) {
        filled = _filled;
        setColor(DrawColor::WHITE);
    }

bool EOGSCircle::updateRenderPos(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
    if (renderPosUpdated) return false;
    renderPosUpdated = true;
    
    if (eogs == nullptr) return false;
    renderX = parentRX+x;
    renderY = parentRY+y;
    insideOfRenderArea = !isOutsideOfParent(parentW, parentH);
    cx = renderX+w/2;
    cy = renderY+h/2;
    renderRadius = radius*std::min(w, h)/2;
    return true;
}
    
void EOGSCircle::render(EOGS* eogs, int16_t parentRX, int16_t parentRY, int16_t parentW, int16_t parentH) {
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

DrawColor EOGSCircle::getColor() const { return drawColor; }
EOGSCircle* EOGSCircle::setColor(DrawColor _drawColor){
    drawColor = _drawColor;
    return this;
}

bool EOGSCircle::isFilled() const { return filled; }
EOGSCircle* EOGSCircle::setFilled(bool _filled) { filled = _filled; return this; }
int16_t EOGSCircle::getAbsoluteRadius() const { return renderRadius; }
float EOGSCircle::getRelativeRadius() const { return radius; }
EOGSCircle* EOGSCircle::setRelativeRadius(float _radius) { radius = _radius; return this; }


#undef filled
#undef drawColor