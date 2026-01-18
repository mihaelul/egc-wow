#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include "lab_m1/Tema3/meshes/meshes.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
    mixTextures = 0;
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    elapsedTime = 0;
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    // snow textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow_diffuse.jpg").c_str(), GL_REPEAT);
        mapTextures["snow_diffuse"] = texture;
    }

    
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow_normal"] = texture;
    }

    // bark textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "tree_diffuse.jpg").c_str(), GL_REPEAT);
        mapTextures["tree_diffuse"] = texture;
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "tree.jpg").c_str(), GL_REPEAT);
        mapTextures["tree_normal"] = texture;
    }

    //leaves textures

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves_diffuse.jpg").c_str(), GL_REPEAT);
        mapTextures["leaves_diffuse"] = texture;
    }


    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves.jpg").c_str(), GL_REPEAT);
        mapTextures["leaves_normal"] = texture;
    }



    // Load meshes
  

    // teren
    {
        Mesh* terrain = MeshUtils::CreateTerrain("terrain", 50.0f, 20.0f);
        meshes[terrain->GetMeshID()] = terrain;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //shader trunchi copac
    {
    Shader* shader = new Shader("TreeShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "TreeShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
    }

    // Leaves shader
    {
        Shader* shader = new Shader("LeavesShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "LeavesShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("FireflyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FireflyVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FireflyFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //copaci

    srand(time(NULL)); 

    int nrCopaci = 80; 
    //BONUS: randare random pozitie si marime
    for (int i = 0; i < nrCopaci; i++) {
        TreeData t;


        float x = (rand() % 5000) / 100.0f - 25.0f;
        float z = (rand() % 5000) / 100.0f - 25.0f;


        if (glm::length(glm::vec2(x, z)) < 2.0f) {
            i--; 
            continue;
        }

        t.position = glm::vec3(x, 0, z);

        t.scale = 0.7f + (rand() % 100) / 100.0f * 0.8f;

        sceneTrees.push_back(t);
    }

    //licurici

    int N = 20; // nr licurici

    fireflies.clear();
    fireflies.reserve(N);

    for (int i = 0; i < N; i++)
    {
        Firefly f;
        f.radius = 2.0f + (rand() % 100) / 100.0f * 6.0f;      
        f.omega = 0.05f + (rand() % 100) / 100.0f * 0.05f;  
        f.phase = (rand() % 100) / 100.0f * 6.28318f;         // [0..2pi]

        f.yAmp = 0.2f + (rand() % 100) / 100.0f * 0.6f;      // [0.2..0.8]
        f.yOmega = 1.0f + (rand() % 100) / 100.0f * 2.0f;      // [1..3]
        f.yPhase = (rand() % 100) / 100.0f * 6.28318f;

        // BONUS: galben-verzui
        f.color = glm::vec3(0.7f + (rand() % 30) / 100.0f, 1.0f, 0.4f);

        fireflies.push_back(f);
    }

}


void Tema3::FrameStart()
{
    glDepthMask(GL_TRUE);
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.55f, 0.6f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);


}

void Tema3::RenderTree(const glm::vec3& pos, float scale)
{
    glm::mat4 base = glm::mat4(1);
    base = glm::translate(base, pos);
    base = glm::scale(base, glm::vec3(scale)); 

    float treeHeight = 2.0f;
 
    {
        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(0, treeHeight * 0.5f, 0));
        model = glm::scale(model, glm::vec3(0.5f, treeHeight, 0.5f));

        RenderSimpleMesh(meshes["cylinder"], shaders["TreeShader"], model,
            mapTextures["tree_diffuse"], mapTextures["tree_normal"]);
    }


    int levels = 7;
    float startY =  0.8f;
    float stepY = 0.22f;


    float bendPhase = pos.x * 0.3f + pos.z * 0.7f;

    for (int i = 0; i < levels; i++)
    {
        float scale = 1.3f - 0.16f * i;
        float y = startY + stepY * i;

        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(0, y, 0));
        model = glm::scale(model, glm::vec3(scale, 0.16f, scale));


        float bendStrength = 0.05f;
        if (i >= 2) bendStrength = 0.08f;
        if (i >= 5) bendStrength = 0.12f;

        glUseProgram(shaders["LeavesShader"]->program);
        glUniform1f(glGetUniformLocation(shaders["LeavesShader"]->program, "uTime"), elapsedTime);
        glUniform1f(glGetUniformLocation(shaders["LeavesShader"]->program, "TREE_HEIGHT"), treeHeight);
        glUniform1f(glGetUniformLocation(shaders["LeavesShader"]->program, "BendPhase"), bendPhase);
        glUniform1f(glGetUniformLocation(shaders["LeavesShader"]->program, "BendStrength"), bendStrength);
        glUniform1f(glGetUniformLocation(shaders["LeavesShader"]->program, "BendFrequency"), 2.0f);

        RenderSimpleMesh(meshes["box"], shaders["LeavesShader"], model,
            mapTextures["leaves_diffuse"], mapTextures["leaves_normal"]);
    }
}

void Tema3::RenderTower(const glm::vec3& pos)
{
    glm::mat4 base = glm::mat4(1);
    base = glm::translate(base, pos);

    float towerHeight = 4.0f;
    float platformY = 3.5f;
    float roofY = platformY + 0.8f;

    float platformSize = 1.3f;
    float poleOffset = platformSize * 0.4f;

    //stalpi
    glm::vec3 poleScale = glm::vec3(0.08f, towerHeight, 0.08f);

    glm::vec3 poles[4] = {
        glm::vec3(-poleOffset, towerHeight * 0.5f, -poleOffset),
        glm::vec3(poleOffset, towerHeight * 0.5f, -poleOffset),
        glm::vec3(-poleOffset, towerHeight * 0.5f,  poleOffset),
        glm::vec3(poleOffset, towerHeight * 0.5f,  poleOffset),
    };

    for (int i = 0; i < 4; i++)
    {
        glm::mat4 model = base;
        model = glm::translate(model, poles[i]);
        model = glm::scale(model, poleScale);

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], model,
            mapTextures["tree_diffuse"], nullptr);
    }

    //platforma
    {
        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(0, platformY, 0));
        model = glm::scale(model, glm::vec3(platformSize, 0.2f, platformSize));

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], model,
            mapTextures["tree_diffuse"], nullptr);
    }

    //acoperis
    {
        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(0, roofY, 0));
        model = glm::scale(model, glm::vec3(platformSize * 0.75f, 0.3f, platformSize * 0.75f));

        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model,
            mapTextures["tree_diffuse"], nullptr);
    }

    
    {
        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(-platformSize * 0.6f, platformY, 0.0f));
        model = glm::rotate(model, RADIANS(90.0f), glm::vec3(0, 1, 0));
        model = glm::rotate(model, RADIANS(-70.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.1f, 0.15f, 0.1f));

        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model,
            mapTextures["tree_diffuse"], nullptr);
    }

    {
        glm::mat4 model = base;
        model = glm::translate(model, glm::vec3(platformSize * 0.6f, platformY, 0.0f));
        model = glm::rotate(model, RADIANS(-90.0f), glm::vec3(0, 1, 0));
        model = glm::rotate(model, RADIANS(-70.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.1f, 0.15f, 0.1f));

        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model,
            mapTextures["tree_diffuse"], nullptr);
    }
}


glm::vec3 Tema3::GetFireflyPos(const Firefly& f, float t, const glm::vec3& center)
{
    float x = center.x + f.radius * cos(f.omega * t + f.phase);
    float z = center.z + f.radius * sin(f.omega * t + f.phase);

    float y = center.y + 2.2f + f.yAmp * sin(f.yOmega * t + f.yPhase);

    return glm::vec3(x, y, z);
}

void Tema3::RenderFireflies(const glm::vec3& center)
{
    if (fireflies.empty())
        return;

    Shader* sh = shaders["FireflyShader"];
    if (!sh)
        return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // additive glow
    glDisable(GL_CULL_FACE);

    glDepthMask(GL_FALSE);  

    for (int i = 0; i < (int)fireflies.size(); i++)
    {
        const Firefly& f = fireflies[i];

        glm::vec3 p = GetFireflyPos(f, elapsedTime, center);

        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, p);
        model = glm::scale(model, glm::vec3(0.06f));

        glUseProgram(sh->program);
        glUniform1f(glGetUniformLocation(sh->program, "uTime"), elapsedTime);
        glUniform1f(glGetUniformLocation(sh->program, "phase"), f.phase);
        glUniform3fv(glGetUniformLocation(sh->program, "fireflyColor"), 1, glm::value_ptr(f.color));

        RenderSimpleMesh(meshes["sphere"], sh, model, nullptr, nullptr);
    }

    glDepthMask(GL_TRUE);         
    glDisable(GL_BLEND);          
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void Tema3::Update(float deltaTimeSeconds)
{
    elapsedTime += deltaTimeSeconds;
     //teren
    {
        glm::mat4 modelMatrix = glm::mat4(1);

      
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.f, 0));

        RenderSimpleMesh(meshes["terrain"], shaders["LabShader"], modelMatrix,
            mapTextures["snow_diffuse"],
            mapTextures["snow_normal"]);
    }

    //turn
    RenderTower(glm::vec3(0, 0, 0));

    //copaci

    for (const auto& tree : sceneTrees) {
        RenderTree(tree.position, tree.scale);
    }

    RenderFireflies(glm::vec3(0, 0, 0));


}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    Texture2D* textureDiffuse, Texture2D* textureNormal)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    // Model
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // View
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    GLint loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Projection
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    GLint loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Light uniforms
    glm::vec3 lightPos = glm::vec3(0, 10, 0);
    glm::vec3 lightColor = glm::vec3(0.8f, 0.85f, 1.0f);
    glm::vec3 viewPos = GetSceneCamera()->m_transform->GetWorldPosition();

    glUniform3fv(glGetUniformLocation(shader->program, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shader->program, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader->program, "viewPos"), 1, glm::value_ptr(viewPos));

    glUniform1i(glGetUniformLocation(shader->program, "use_normal_map"), textureNormal != nullptr);

    // Diffuse texture 
    if (textureDiffuse)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureDiffuse->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Normal map 
    if (textureNormal)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureNormal->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    if (shader->GetName() == "TreeShader")
    {
        glUniform1f(glGetUniformLocation(shader->program, "TREE_HEIGHT"), 2.0f);
    }

    if (shader->GetName() == "LeavesShader")
    {
        glUniform1f(glGetUniformLocation(shader->program, "uTime"), elapsedTime);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);

    // Setează distanța maximă până la care se vede (ajustează valoarea după plac)
    float maxDist = 25.0f;
    glUniform1f(glGetUniformLocation(shader->program, "max_distance"), maxDist);

    // Setează culoarea ceții (gri deschis/albăstrui pentru iarnă)
    glm::vec3 fogColor = glm::vec3(0.5f, 0.55f, 0.6f);
    glUniform3fv(glGetUniformLocation(shader->program, "fog_color"), 1, glm::value_ptr(fogColor));
    glDrawElements(mesh->GetDrawMode(), (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);
}






/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
