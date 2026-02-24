#pragma once
#include <glm/glm.hpp>
#include <string>


class UIElement
{
protected:
    bool visible;
    glm::vec2 position;
    glm::vec2 scale;

public:
    UIElement(glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
        : visible(true), position(pos), scale(scale) {
    }

    virtual void Update(double dt) {};
    virtual void Render() {};

    void setVisible(bool v);
    bool isVisible() const;
    glm::vec2 getPosition() const;
    glm::vec2 getScale() const;
};

