#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "FPCamera.h"
#include "PhysicsObject.h"
#include <vector> // added

static const int TOTAL_INTERACTIVES = 100;
static const int TOTAL_PICKABLES = 50;
static const int TOTAL_LIGHTS = 8;

static const int TOTAL_PARTS = 30;
static const int TOTAL_PHASES = 30;

static const int TOTAL_PHYSICSOBJECT = 50;

class BaseScene : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,

		// MODELS
		GEO_BASEBALL,
		GEO_CANSCOKE,
		GEO_CANSMTNDEW,
		GEO_CANSSPRITE,
		GEO_CANSPEPSI,
		GEO_BACKGROUND_BUILDINGS,
		GEO_FLOOR,
		GEO_FENCE,

		// SKYBOX
		GEO_FRONT,
		GEO_BACK,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,

		// GUI
		GEO_MENU_GUI,
		GEO_SWITCHSCENE_GUI,
		GEO_INTERACTFADE_GUI,
		GEO_CROSSHAIRTRANSLUCENT_GUI,
		GEO_CROSSHAIROPAQUE_GUI,

		GEO_ITEMINHANDFADE_GUI,
		GEO_ITEMINHANDBORDER_GUI,
		GEO_ITEMINHANDFADEBACKGROUND_GUI,

		// EUI
		GEO_INTERACT_EUI,
		GEO_INTERACTED_EUI,

		// FONTS
		GEO_CARNIVALEEFREAKSHOW_FONT,
		GEO_SATOSHIREGULAR_FONT,
		GEO_HOMEVIDEO_FONT,
		GEO_HOMEVIDEOBOLD_FONT,
		GEO_VCROSDMONO_FONT,
		GEO_MINGLIUEXTB_FONT,

		//debug

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
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,
		U_LIGHT3_TYPE,
		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,
		U_LIGHT4_TYPE,
		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,
		U_LIGHT5_TYPE,
		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,
		U_LIGHT6_TYPE,
		U_LIGHT6_POSITION,
		U_LIGHT6_COLOR,
		U_LIGHT6_POWER,
		U_LIGHT6_KC,
		U_LIGHT6_KL,
		U_LIGHT6_KQ,
		U_LIGHT6_SPOTDIRECTION,
		U_LIGHT6_COSCUTOFF,
		U_LIGHT6_COSINNER,
		U_LIGHT6_EXPONENT,
		U_LIGHT7_TYPE,
		U_LIGHT7_POSITION,
		U_LIGHT7_COLOR,
		U_LIGHT7_POWER,
		U_LIGHT7_KC,
		U_LIGHT7_KL,
		U_LIGHT7_KQ,
		U_LIGHT7_SPOTDIRECTION,
		U_LIGHT7_COSCUTOFF,
		U_LIGHT7_COSINNER,
		U_LIGHT7_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	struct Pickable {
		std::string name;
		PhysicsObject body;
		bool isHeld;
	};

	BaseScene();
	~BaseScene();

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

	void HandleKeyPress(double dt);
	void HandleMouseInput();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizeX, float sizeY);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y, char alignment, float spacingPercentage);
	void RenderSkybox();
	void RenderUI();

	void setCameraOrigin(glm::vec3 position, glm::vec3 target, glm::vec3 up);

	void resetInteractives();
	void addInteractives(std::string name, char type, glm::vec3 position, int pickableIndex = 0.f);
	void addPickables(std::string name, glm::vec3 position);
	void removePickables(int index);
	void initializePickablesInteractives();
	void getClosestInteractive();

	void dropItemInHand(int amountToRemove);
	void addItemInHand(int index);

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float fps = 0;

	// CAMERA PROPERTIES
	FPCamera camera;
	PhysicsObject cameraBody;

	glm::vec3 cameraOriginPosition;
	glm::vec3 cameraOriginTarget;
	glm::vec3 cameraOriginUp;
	float bobAmplitudeVertical = 0.02f;
	float bobAmplitudeHorizontal = 0.015f;
	float bobFrequency = 12.0f;

	float bobDistanceAccumulated = 0.0f;
	float currentBobWeight = 0.0f;

	glm::vec3 currentPlayerPosition, previousPlayerPosition;
	glm::vec3 previousBobOffset = glm::vec3(0.0f);

	// INTERACTIVES
	int noOfInteractives;

	std::string interactives[TOTAL_INTERACTIVES];
	char interactivesType[TOTAL_INTERACTIVES];
	glm::vec3 interactivesPos[TOTAL_INTERACTIVES];

	int interactedIndexes[TOTAL_INTERACTIVES];
	int interactivePickablesIndex[TOTAL_INTERACTIVES];
	int interactedIndex;
	int previousInteractedIndex;

	int noOfPickables;
	Pickable pickables[TOTAL_PICKABLES];

	float interactedEUI_scale;
	float interactedEUI_targetScale;

	glm::vec2 interactGUI_positionOffset;
	glm::vec2 interactGUI_targetPositionOffset;

	// ITEM IN HAND
	Pickable* itemInHand;
	int amountOfItem;
	std::string previousItemInHandName;
	double itemInHandElapsed;

	bool itemInUse;

	glm::vec3 itemInHandGUI_scaleOffset;
	glm::vec3 itemInHandGUI_targetScaleOffset;

	double dropKeybindHeldElapsed;
	bool droppedFirst;

	// SCENE SWITCH UI
	float sceneSwitchUI_scalePercentage;
	float sceneSwitchUI_targetScalePercentage;

	// DIALOGUE HANDLING
	int oldPart;
	int part;
	int oldPhase;
	int phase;

	double currPhaseElapsed;
	double phaseDurations[TOTAL_PARTS][TOTAL_PHASES];

	// OTHERS
	PhysicsObject worldObjects[TOTAL_PHYSICSOBJECT];

	int projType = 1; // fix to 0 for orthographic, 1 for projection

	MatrixStack modelStack, viewStack, projectionStack;


	Light light[TOTAL_LIGHTS];
	bool enableLight;

	//next scene (will immediately switch when its not 0
};

#endif