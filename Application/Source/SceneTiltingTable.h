#ifndef SCENE_TILTING_TABLE_H
#define SCENE_TILTING_TABLE_H

#include "BaseScene.h"
class SceneTiltingTable : public BaseScene
{
public:
	SceneTiltingTable();
	~SceneTiltingTable();
	void Init() override;
	void Update(double dt) override;
	void Render() override;
	void Exit() override;
};
#endif

