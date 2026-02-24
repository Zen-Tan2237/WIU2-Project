#pragma once
#include "AnimatedUIElement.h"

class InteractPromptUI : public AnimatedUIElement
{
private:
    std::string text;

public:
    InteractPromptUI()
        : AnimatedUIElement(glm::vec2(-25.f, 0), glm::vec2(1600, 900), 10.f), text("") {
    }

    void Update(double dt) override;

    void show(const std::string& t);
    void hide();
    std::string getText() const;
};
