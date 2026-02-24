#include "UIElement.h"

void UIElement::setVisible(bool v)
{
	visible = v;
}

bool UIElement::isVisible() const
{
	return visible;
}

glm::vec2 UIElement::getPosition() const
{
	return position;
}

glm::vec2 UIElement::getScale() const
{
	return scale;
}
