#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "FPCamera.h"

class SceneFPCamera : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_SPHERE,
		GEO_CUBE,
		GEO_PLANE,
		GEO_GUI_QUAD,
		GEO_DOORMAN,
		GEO_MODEL_MTL1,
		GEO_MODEL_MTL2,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	SceneFPCamera();
	~SceneFPCamera();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//RAII Guard for model stack
	struct PushPop
	{
		PushPop(MatrixStack& ms) : m_ms(ms)
		{
			m_ms.PushMatrix();
		}
		~PushPop()
		{
			m_ms.PopMatrix();
		}
		MatrixStack& m_ms;
	};

private:
	void HandleKeyPress();
	void HandleMouseInput();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y, char alignment, float spacingPercentage);

	void setCameraOrigin(glm::vec3 position, glm::vec3 target, glm::vec3 up);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float fps = 0;

	// CAMERA PROPERTIES
	FPCamera camera;
	glm::vec3 cameraOriginPosition;
	glm::vec3 cameraOriginTarget;
	glm::vec3 cameraOriginUp;
	float bobAmplitudeVertical = 0.035f;
	float bobAmplitudeHorizontal = 0.025f;
	float bobFrequency = 6.0f;

	float bobDistanceAccumulated = 0.0f;
	float currentBobWeight = 0.0f;

	glm::vec3 currentPlayerPosition, previousPlayerPosition;
	glm::vec3 previousBobOffset = glm::vec3(0.0f);
	//

	int projType = 1; // fix to 0 for orthographic, 1 for projection

	MatrixStack modelStack, viewStack, projectionStack;

	static const int NUM_LIGHTS = 1;
	Light light[NUM_LIGHTS];
	bool enableLight;
};

#endif