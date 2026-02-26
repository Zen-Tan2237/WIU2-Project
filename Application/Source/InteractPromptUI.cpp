#include "InteractPromptUI.h"

void InteractPromptUI::Update(double dt)
{
	AnimatedUIElement::Update(dt);
}

void InteractPromptUI::setText(const std::string& t)
{
    text = t;
}

void InteractPromptUI::setColor(const glm::vec3& c)
{
	color = c;
}

void InteractPromptUI::show(const std::string& t)
{
    text = t;
    visible = true;
    resetPosition(glm::vec2(-25.f, 0));
    setTargetPosition(glm::vec2(0, 0));
}

void InteractPromptUI::hide()
{
    visible = false; text = "";
}

std::string InteractPromptUI::getText() const
{
    return text;
}

glm::vec3 InteractPromptUI::getColor() const
{
    return color;
}
