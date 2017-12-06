#include "GameHeader.h"
#include"ObjectModel.h"
#include "Buffers.h"
int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle("Model Loading");

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetMousePos(1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

	// Load the texture
	GLuint MarsTexture = loadBMP_custom("MarsMap.bmp");
	GLuint SaturnTexture = loadBMP_custom("saturn.bmp");
	GLuint MercuryTexture = loadBMP_custom("mercury.bmp");
	GLuint VenusTexture = loadBMP_custom("venus.bmp");
	GLuint EarthTexture = loadBMP_custom("earth.bmp");
	GLuint SceneTexture = loadBMP_custom("skysphere.bmp");


	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	/*Vertices , UV and Normals Loading*/

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.


	/*Load Object and its Buffer*/

	//bool scene = loadOBJ("skysphere.obj", vertices, uvs, normals);
	/*GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);*/


	Buffers*  SpaceShipBuffers= new Buffers("spaceship_clean.obj"); 
	SpaceShipBuffers->Create(); 
	ObjectModel SpaceShip("Diffuse.bmp", SpaceShipBuffers);

	Buffers BlackHoleBuffers("blackhole2.obj"); 
	BlackHoleBuffers.Create();
	ObjectModel BlackHole("FittedBH.bmp", &BlackHoleBuffers);
	ObjectModel LightTunnel("Tunnel.bmp", &BlackHoleBuffers);

	Buffers MeteorBuffers("meteor.obj");
	MeteorBuffers.Create();
	ObjectModel Meteor("mercury.bmp", &MeteorBuffers);

	Buffers AstroidBuffers("astroid.obj");
	AstroidBuffers.Create();
	ObjectModel Astroid("astroid.bmp", &AstroidBuffers);

	Buffers FuelTankBuffers("fueltank.obj");
	FuelTankBuffers.Create();
	ObjectModel FuelTank("redbmp.bmp", &FuelTankBuffers);

	Buffers GiftBuffers("cube.obj");
	GiftBuffers.Create();
	ObjectModel Gift("wrap1.bmp", &GiftBuffers);

	//Creating planets' buffers and models
	//Same object for all planets -> Skyshpere
	Buffers SphereBuffers("skysphere.obj");
	SphereBuffers.Create();

	ObjectModel Mars("MarsMap.bmp", &SphereBuffers);
	ObjectModel Saturn("saturn.bmp", &SphereBuffers);
	ObjectModel Mercury("mercury.bmp", &SphereBuffers);
	ObjectModel Venus("venus.bmp", &SphereBuffers);
	ObjectModel Earth("earth.bmp", &SphereBuffers);
	ObjectModel SkySphere("skysphere.bmp", &SphereBuffers);


	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;
	float orientation = 0.0f;
	float orientation_sin = 0.0f;
	float BHorientation = 3.14159f / 2.0f;
	glm::mat4 PlanetsScaling = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));

	do
	{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Measure speed
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			nbFrames = 0;
			lastTime += 1.0;
		}
		orientation += 3.14159f / 6.0f * deltaTime;
		orientation_sin = sin(3.14159f / 2.0f * currentTime);

		glm::mat4 PlanetsRotation = eulerAngleY(orientation);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();


		/////////////////////////////////////////////////////////////*Object Loading*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

		/*Mars*/
		computeMatricesFromInputs();
		glm::mat4 MarsTranslation = translate(mat4(), vec3(-2.0f, 2.0f, -2.0f));
		glm::mat4 MarsModel = MarsTranslation* PlanetsRotation* PlanetsScaling;
		Mars.setModelMatrix(MarsModel);
		Mars.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*Saturn*/
		computeMatricesFromInputs();
		glm::mat4 SaturnTranslation = translate(mat4(), vec3(2.0f, 2.0f, -2.0f));
		glm::mat4 SaturnModel = SaturnTranslation* PlanetsRotation* PlanetsScaling;
		Saturn.setModelMatrix(SaturnModel);
		Saturn.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);


		/*Mercury*/
		computeMatricesFromInputs();
		glm::mat4 MercuryTranslation = translate(mat4(), vec3(-2.0f, 2.0f, -5.0f));
		glm::mat4 MercuryModel = MercuryTranslation* PlanetsRotation* PlanetsScaling;
		Mercury.setModelMatrix(MercuryModel);
		Mercury.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*Venus*/
		computeMatricesFromInputs();
		glm::mat4 VenusTranslation = translate(mat4(), vec3(2.0f, -2.0f, -3.0f));
		glm::mat4 VenusModel = VenusTranslation* PlanetsRotation* PlanetsScaling;
		Venus.setModelMatrix(VenusModel);
		Venus.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*Earth*/
		computeMatricesFromInputs();
		glm::mat4 EarthRotation = eulerAngleY(10.0f);
		glm::mat4 EarthTranslation = translate(mat4(), vec3(-2.0f, -2.0f, 0.0f));
		glm::mat4 EarthModel = EarthTranslation* PlanetsRotation* EarthRotation*PlanetsScaling;
		Earth.setModelMatrix(EarthModel);
		Earth.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*Environment (SkySphere Space Environment) */
		computeMatricesFromInputs();
		glm::mat4 SceneScaling = scale(mat4(), vec3(100.0f, 100.0f, 100.0f));
		glm::mat4 SceneTranslation = translate(mat4(), getCameraPosition());
		glm::mat4 SceneModel = SceneTranslation* SceneScaling;
		SkySphere.setModelMatrix(SceneModel);
		SkySphere.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);


		// SPACESHIP
		glm::mat4 SpaceshipScaling = scale(mat4(), vec3(0.25f, 0.25f, 0.5f));
		glm::mat4 SpaceshipTranslation = translate(mat4(), vec3(getCameraPosition().x, (getCameraPosition().y - 0.5f), (getCameraPosition().z) - 3.0f));
		glm::mat4 SSModel = SpaceshipTranslation*SpaceshipScaling;
		SpaceShip.setModelMatrix(SSModel);
		SpaceShip.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*BlackHole*/

		glm::mat4 BlackholeTranslation = translate(mat4(), vec3(-1.0f, 0.0f, 0.0f));
		glm::mat4 BlackholeScaling = scale(mat4(), vec3(0.1, 0.1, 0.1));
		glm::mat4 BlackholeXRotation = eulerAngleX(BHorientation);
		glm::mat4 BlackholeYRotation = eulerAngleY(orientation * 37);
		glm::mat4 BHModel = BlackholeTranslation*BlackholeXRotation*BlackholeYRotation*BlackholeScaling;// = PlanetsRotation;
		BlackHole.setModelMatrix(BHModel);
		BlackHole.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);


		/*LightTunnel*/
		glm::mat4 LTunnelTranslation = translate(mat4(), vec3(0.0f, -0.2f, 0.0f));
		//glm::mat4 LTunnelScaling = scale(mat4(), vec3(0.2, 0.2, 0.2));
		glm::mat4 LTunnelModel = LTunnelTranslation*BlackholeXRotation*BlackholeScaling;
		LightTunnel.setModelMatrix(LTunnelModel);
		LightTunnel.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/*Meteor*/

		glm::mat4 MeteorTranslation = translate(mat4(), vec3(-5.0f, -5.0f, -5.0f));
		glm::mat4 MeteorZRotation = eulerAngleZ(orientation);
		glm::mat4 MeteorModel = MeteorTranslation*MeteorZRotation* PlanetsScaling;
		Meteor.setModelMatrix(MeteorModel);
		Meteor.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);


		/*Astroid*/

		glm::mat4 AstroidTranslation = translate(mat4(), vec3(5.0f, 5.0f, 5.0f));
		glm::mat4 AstroidZRotation = eulerAngleZ(orientation);
		glm::mat4 AstroidModel = AstroidTranslation*AstroidZRotation* PlanetsScaling;
		Astroid.setModelMatrix(AstroidModel);
		Astroid.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		/* Fuel Tank*/

		glm::mat4 FuelTranslation = translate(mat4(), vec3(5.0f, orientation_sin, 5.0f));
		glm::mat4 FuelRotationMatrix = eulerAngleYXZ(orientation_sin, 0.0f, 0.0f);//yaw, pitch and roll. Measured in radians
		glm::mat4 FuelScalingMatrix = scale(mat4(), vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 FuelModel = FuelTranslation * FuelRotationMatrix * FuelScalingMatrix;
		FuelTank.setModelMatrix(FuelModel);
		FuelTank.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);



		/* Gift Box*/

		glm::mat4 GiftTranslation = translate(mat4(), vec3(5.0f, orientation_sin, 7.0f));
		glm::mat4 GiftRotationMatrix = eulerAngleYXZ(orientation_sin, 0.0f, 0.0f);
		glm::mat4 GiftScalingMatrix = scale(mat4(), vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 GiftModel = GiftTranslation * GiftRotationMatrix * GiftScalingMatrix;
		Gift.setModelMatrix(GiftModel);
		Gift.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glfwSwapBuffers();	//Swap buffers

	}

	while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));


	// Cleanup VBO and shader
	
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glfwTerminate();
	return 0;
}

