#pragma once
#include "AnimatedUIElement.h"


class InteractPromptUI : public AnimatedUIElement
{
private:
    std::string text;
    glm::vec3 color;

public:
    InteractPromptUI()
        : AnimatedUIElement(glm::vec2(-25.f, 0), glm::vec2(1600, 900), 10.f), text("") {
    }

    void Update(double dt) override;

    void setText(const std::string& t);
    void setColor(const glm::vec3& c);
    void show(const std::string& t);
    void hide();
    std::string getText() const;
	glm::vec3 getColor() const;
};
