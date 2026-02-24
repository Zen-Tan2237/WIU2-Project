#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	int nextScene = 0;
	int accumulatedCash = 0;
	double nextSceneDelay = 0.f;
};

#endif