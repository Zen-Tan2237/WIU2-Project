#include "AnimatedUIElement.h"

void AnimatedUIElement::Update(double dt)
{
    float t = 1.f - std::exp(-lerpSpeed * (float)dt);
    position += (targetPosition - position) * t;
    scale += (targetScale - scale) * t;
}

void AnimatedUIElement::setTargetPosition(glm::vec2 target)
{
    targetPosition = target;
}

void AnimatedUIElement::setTargetScale(glm::vec2 target)
{
    targetScale = target;
}

void AnimatedUIElement::resetPosition(glm::vec2 pos)
{
    position = pos;
    //targetPosition = pos;
}

void AnimatedUIElement::resetScale(glm::vec2 s)
{
    scale = s;
    //targetScale = s;
}
