#include "InteractPromptUI.h"

void InteractPromptUI::Update(double dt)
{
	AnimatedUIElement::Update(dt);
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
