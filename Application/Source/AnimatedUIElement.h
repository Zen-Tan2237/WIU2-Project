#pragma once
#include "UIElement.h"
#include <glm/glm.hpp>
#include <string>

class AnimatedUIElement : public UIElement
{
protected:
    glm::vec2 targetPosition;
    glm::vec2 targetScale;
    float lerpSpeed;

public:
    AnimatedUIElement(glm::vec2 startPos = glm::vec2(0), glm::vec2 startScale = glm::vec2(1), float speed = 10.f)
        : UIElement(startPos, startScale), targetPosition(startPos), targetScale(startScale), lerpSpeed(speed) {
    }

    void Update(double dt) override;

    void setTargetPosition(glm::vec2 target);
    void setTargetScale(glm::vec2 target);
    void resetPosition(glm::vec2 pos);
    void resetScale(glm::vec2 s);
};