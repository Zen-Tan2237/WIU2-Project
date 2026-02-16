#ifndef SCENE_LIGHT_SOURCE_H
#define SCENE_LIGHT_SOURCE_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Material.h"

class SceneLightSource : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_SPHERE,
		GEO_PYRAMID,
		GEO_LIGHT,
		NUM_GEOMETRY,
	};

	enum LIGHT_UNI_TYPE {
		LU_POSITION = 0,
		LU_COLOR,
		LU_POWER,
		LU_KC,
		LU_KL,
		LU_KQ,
		LU_TYPE,
		LU_SPOTDIRECTION,
		LU_COSCUTOFF,
		LU_COSINNER,
		LU_EXPONENT,
		LU_TOTAL,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		// Add the following constants
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1,
		U_LIGHTENABLED,
		U_TOTAL,
	};

	//Set up RAII Guard for matrix stack
	struct PushPop {
		// Constructor pushes matrix, destructor pops matrix
		// Explicit keyword to avoid implicit conversions
		explicit PushPop(MatrixStack& ms) : stack(&ms) {
			stack->PushMatrix();
		}
		~PushPop() {
			stack->PopMatrix();
		}
		MatrixStack* stack;
	};

	SceneLightSource();
	~SceneLightSource();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();
	void RenderMesh(Mesh* mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	int projectionMode;
	AltAzCamera camera;	

	MatrixStack modelStack, viewStack, projectionStack;

	Light light[1];
	bool enableLight;
	bool lightOff;

	// Specific variables for this scene put here
	float time; // keeps track of the time for the interpolation to run
	float duration; // length of the interpolation time
	glm::vec3 currentPos; // keeps track of current position
};

#endif