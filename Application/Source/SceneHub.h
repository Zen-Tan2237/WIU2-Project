#ifndef SCENE_HUB_H
#define SCENE_HUB_H

#include "BaseScene.h"
class SceneHub : public BaseScene
{
public:
	SceneHub();
	~SceneHub();
	void Init() override;
	void Update(double dt) override;
	void Render() override;
	void Exit() override;
};
#endif

