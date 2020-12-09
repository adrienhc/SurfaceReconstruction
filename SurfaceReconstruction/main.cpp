#include "main.h"

int main(int argc, char** argv)
{

    //Sleep(5000.0f);

    //GLFW WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //for OpenGL 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

    if(!CapFPS)
       glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

   
    //
    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Surface Reconstruction", NULL, NULL);
                                                                                    //glfwGetPrimaryMonitor()   --for fullscreen app automatically

    //GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", glfwGetPrimaryMonitor(), NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //if(noCapFPS)
    //    glfwSwapInterval( 0 );

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //setup GLAD, gives pointers to OS specific OpenGL function pointers
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);  

    glClearColor(0.423f, 0.701f, 0.756f, 1.0f);

    glEnable(GL_MULTISAMPLE); 
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_SMOOTH);
    // glEnable(GL_FRAMEBUFFER_SRGB); 
    
    //CDMLINE ARGS
    std::string heightmap_path_decimated(argv[1]);
    std::string heightmap_path_reconstructed(argv[2]);
    std::string heightmap_path_original(argv[3]);
    float height_scale = atof(argv[4]);
    std::string noisemap_path(argv[5]);
    float noise_scale = atof(argv[6]);

    float model_size = 10.0f;
    int lod = 1;
    bool heightmap_as_texture = false;

    float model_height = -1.0f;
    glm::vec3 light_offset = glm::vec3(-5.0f, 0.0f, model_size/2.0f);
    float light_height = 10.0f;
    float light_radius = 40.0f;

    //TERRAIN DECIMATED - ROOM O
    nNode* RootDecimated = new nNode();
    PointLight* pointLightDecimated = new PointLight(1, glm::vec3(0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.007f, 0.0002f);
    pointLightDecimated->setRadius(light_radius, 0);
    nNode* RefDecimated = RootDecimated->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, light_height, 0.0f) + light_offset));
    RefDecimated->AddChildren(new nPointLight(pointLightDecimated, 0));
    Group* g_light_decimated = new Group(RootDecimated);
    Terrain terrainDecimated = Terrain(glm::vec3(0.0f, model_height, 0.0f), 'N', height_scale, model_size, lod, heightmap_path_decimated, heightmap_as_texture, 0.0f);

    //TERRAIN RECONSTRUCTED - ROOM 1
    nNode* RootReconstructed = new nNode();
    PointLight* pointLightReconstructed = new PointLight(1, glm::vec3(0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.007f, 0.0002f);
    pointLightReconstructed->setRadius(light_radius, 0);
    nNode* RefReconstructed = RootReconstructed->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, light_height, model_size) + light_offset));
    RefReconstructed->AddChildren(new nPointLight(pointLightReconstructed, 0));
    Group* g_light_reconstructed = new Group(RootReconstructed);
    Terrain terrainReconstructed = Terrain(glm::vec3(0.0f, model_height, model_size), 'N', height_scale, model_size, lod, heightmap_path_reconstructed, heightmap_as_texture, 0.0f);
    

    //TERRAIN NOISE - ROOM 2
    nNode* RootNoise = new nNode();
    PointLight* pointLightNoise = new PointLight(1, glm::vec3(0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.007f, 0.0002f);
    pointLightNoise->setRadius(light_radius, 0);
    nNode* RefNoise = RootNoise->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, light_height, 2.0f * model_size) + light_offset));
    RefNoise->AddChildren(new nPointLight(pointLightNoise, 0));
    Group* g_light_noise = new Group(RootNoise);
    Terrain terrainNoise = Terrain(glm::vec3(0.0f, model_height, 2.0f * model_size), 'N', height_scale, model_size, lod, heightmap_path_reconstructed, heightmap_as_texture, 0.0f);
    terrainNoise.AddNoise(noise_scale, noisemap_path, 0.5f);

    //TERRAIN ORIGINAL- ROOM 3
    nNode* RootOriginal = new nNode();
    PointLight* pointLightOriginal = new PointLight(1, glm::vec3(0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.007f, 0.0002f);
    pointLightOriginal->setRadius(light_radius, 0);
    nNode* RefOriginal = RootOriginal->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, light_height, 3.0f * model_size) + light_offset));
    RefOriginal->AddChildren(new nPointLight(pointLightOriginal, 0));
    Group* g_light_original = new Group(RootOriginal);
    Terrain terrainOriginal = Terrain(glm::vec3(0.0f, model_height, 3.0f * model_size), 'N', height_scale, model_size, lod, heightmap_path_original, heightmap_as_texture, 0.0f);
   
   //RENDERER
    Renderer renderer = Renderer();

    //SKYBOX
    Skybox skybox("./skybox/PositiveX.png",
                  "./skybox/NegativeX.png",
                  "./skybox/PositiveY.png",
                  "./skybox/NegativeY.png",
                  "./skybox/PositiveZ.png",
                  "./skybox/NegativeZ.png");

    // CAMERA
    camera = Camera(glm::vec3(5.0f, 5.0f, 3.0f));
    lastX = WINDOW_WIDTH / 2.0f;
    lastY = WINDOW_HEIGHT / 2.0f;
    float ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    camera.SetView(0.005f, 200.0f, ratio);

    Player Player(&camera, nullptr);

    //LAYERS
    Shader batchShaderDecimated = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs"); 
    SceneLayer sceneDecimated = SceneLayer(&camera, &batchShaderDecimated);
    
    Shader batchShaderReconstructed = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs");
    SceneLayer sceneReconstructed = SceneLayer(&camera, &batchShaderReconstructed);
    
    Shader batchShaderNoise = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs");
    SceneLayer sceneNoise = SceneLayer(&camera, &batchShaderNoise);

    Shader batchShaderOriginal = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs");
    SceneLayer sceneOriginal = SceneLayer(&camera, &batchShaderOriginal);

    Shader batchShaderPoints = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs");
    SceneLayer scenePoints = SceneLayer(&camera, &batchShaderPoints);

    Shader depthShader = Shader("shaders/batch/depthShader.vs", "shaders/batch/depthShader.gs", "shaders/batch/depthShader.fs");
    DepthmapLayer depthmap = DepthmapLayer(&camera, &depthShader); 

    //RNG SEED
    srand (static_cast <unsigned> (glfwGetTime()));
    //Fragment Shader supports 32 textures!
    float deltaTimeAcc = 0.0f;
    int Frames = 0;

    sceneDecimated.AddLight(g_light_decimated, true);
    terrainDecimated.addLayer(&sceneDecimated);

    sceneReconstructed.AddLight(g_light_reconstructed, true);
    terrainReconstructed.addLayer(&sceneReconstructed);

    sceneNoise.AddLight(g_light_noise, true);
    terrainNoise.addLayer(&sceneNoise);
    
    sceneOriginal.AddLight(g_light_original, true);
    terrainOriginal.addLayer(&sceneOriginal);

    //RENDER LOOP
    bool printNext = false;

    while(!glfwWindowShouldClose(window))
    {

        //TIMING
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        if(DisplayFPS)
        {
            deltaTimeAcc += deltaTime;
            Frames++;
            if(deltaTimeAcc > 1.0f)
            {
                std::cout << Frames << " fps " << deltaTime << std::endl;
                Frames = 0;
                deltaTimeAcc = 0.0f;
            }
        }
        lastFrame = current_frame;

        if(printNext)
        {
            std::cout << "Next dt " << deltaTime << std::endl;
            printNext = false;
        }

        //WORLD BUILDING
        if(BuildWorld == true)
        {
            BuildWorld = false;

            depthmap.Clear();
            depthmap.AddLight(g_light_decimated, false);
            terrainDecimated.addLayer(&depthmap);
            depthmap.Render(); 

            depthmap.Clear();
            depthmap.AddLight(g_light_reconstructed, false);
            terrainReconstructed.addLayer(&depthmap);
            depthmap.Render();
            
            depthmap.Clear();
            depthmap.AddLight(g_light_noise, false);
            terrainNoise.addLayer(&depthmap);
            depthmap.Render();

            depthmap.Clear();
            depthmap.AddLight(g_light_original, false);
            terrainOriginal.addLayer(&depthmap);
            depthmap.Render();
        }
        
        //INPUT PROCESSING
        process_input(window);

        //GAME LOGIC

        //TIME UPDATE
        Player.Update(deltaTime);
        
        //RENDERING
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
        //ROOM LAYER
        //STATIC GEOMETRY
        if (polygon)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        sceneDecimated.RenderKeep();
        sceneReconstructed.RenderKeep();
        sceneNoise.RenderKeep();
        sceneOriginal.RenderKeep();

        //SKYBOX LAYER
        renderer.RenderSkybox(&skybox, &camera);

        if(firstPass)
            firstPass = false;

        //Swap Buffers, Poll IO events
        if(CapFPS)
            glfwSwapBuffers(window);
        else
            glFlush();
            
        glfwPollEvents();
    }

    //terminate GLFW Window
    //glfwTerminate();

    return 0;
}


void framebuffer_size_callback( GLFWwindow* window, int width, int height)
{
   // glfwSetWindowSizeCallback(window, &width, &height);
   glViewport(0, 0, width, height); 
}

void process_input(GLFWwindow* window)
{
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    if(!FixCamera)
    {
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            Weapon::ads = true;    
        }
        else
            Weapon::ads = false;

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            Weapon::fire = true;    
        }
        else
            Weapon::fire = false;

        if (Room::collideOn() && Gravity)
            camera.Gravity(deltaTime);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime, flyCam);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARDS, deltaTime, flyCam);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime, flyCam);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime, flyCam);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime, flyCam);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            if(!Weapon::reload)
                Weapon::reload = true;
        }
    }
    
        
    

    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        polygon = !polygon; 

    /*if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend < 1.0f)
        blend += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend > 0.0f)
        blend -= 0.05f;*/

    

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        BuildWorld = true;
        FixCamera = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#if defined(LINUX)
        sleep(0.5f);
#elif defined(WINDOWS)
        Sleep(0.5f);
#endif
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        FixCamera = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#if defined(LINUX)
        sleep(0.3f);
#elif defined(WINDOWS)
        Sleep(0.3f);
#endif
    }



}

void mouse_callback( GLFWwindow* window, double xpos, double ypos)
{
    if(!FixCamera)
    {
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovements(xoffset, yoffset);    
    }
    
}

//void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
//{
    //camera.ProcessMouseScroll(yoffset);
//}

void build_world(std::vector<Room*>& World, Asset* floor, Asset* wall, Asset* door, Asset* beam, Asset* ceiling, Asset* crate)
{
#if defined(LINUX)
    for(int i = 0; i < World.size(); i++)
    {
        delete World[i];
    }
    World.clear();
    

    for(int i = 0; i < NumRooms; i++)
    {
        auto dlRoom = new DLClass<BaseRoom>("./dynamic_class/libRoom"+std::to_string(i)+".so");
        std::shared_ptr<BaseRoom> room = dlRoom->make_obj();
        delete dlRoom;

        if(!room) 
        {
            std::cerr << "Failed to instantiate room - "+std::to_string(i)+"." << std::endl;
            // return 1;
        }
        else
        {
            std::cerr << "Success to instantiate room - "+std::to_string(i)+"." << std::endl;   
        }

        PointLight* pointLight = new PointLight(room->lightPos.size(), room->lightAmbient,  room->LightColor, room->LightColor, room->lightConstant, room->lightLinear, room->lightQuadratic);
        
        Room* current_room = new Room(room->length, room->width, room->height, room->offset, room->DoorN, room->DoorS, room->DoorE, room->DoorW, 
            room->lightPos, room->lightRadius, room->vertical, room->horizontal, room->target, room->stairs,
            floor, wall, door, beam, ceiling, crate, pointLight);
        
        current_room->makeRoom();

        World.push_back(current_room);
    }
#endif

    BuildWorld = false;
}