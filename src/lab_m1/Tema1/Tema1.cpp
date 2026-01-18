#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 40;
    int distanta = 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);


    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 0, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(square4);

    Mesh* radialSquare = object2D::CreateRadialSquare("radialSquare", corner, squareSide);
    AddMeshToList(radialSquare);

    Mesh* motor = object2D::CreateMotor("motor", corner, squareSide);
    AddMeshToList(motor);

    Mesh* bumper = object2D::CreateBumper("bumper", corner, squareSide);
    AddMeshToList(bumper);

    Mesh* tun = object2D::CreateTun("tun", corner, squareSide);
    AddMeshToList(tun);

    Mesh* start = object2D::CreateStart("start", corner, squareSide);
    AddMeshToList(start);



}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    float scale = 2;
    float squareSide = 40;
    float spatiu = 10;



    // Primul grid rosu
    //un patrat rosu are 240 x 180 

    modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= transform2D::Scale(6, 4.5f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);

    modelMatrix2 *= transform2D::Translate(0, 40);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);

    modelMatrix2 *= transform2D::Translate(0, 40);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);

    modelMatrix2 *= transform2D::Translate(0, 40);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);

    glm::vec3 radialSquarePos = glm::vec3(100, 70, 0);

    //randare bloc
    modelMatrix_radial = glm::mat3(1);
    modelMatrix_radial *= transform2D::Translate(radialSquarePos.x, radialSquarePos.y);
    RenderMesh2D(meshes["radialSquare"], shaders["VertexColor"], modelMatrix_radial);

    //randare motor

    glm::vec3 motorPos = glm::vec3(100, 280, 0);
    modelMatrix_motor = glm::mat3(1);
    modelMatrix_motor *= transform2D::Translate(motorPos.x, motorPos.y);
    RenderMesh2D(meshes["motor"], shaders["VertexColor"], modelMatrix_motor);

    //randare bumper
    glm::vec3 bumperPos = glm::vec3(100, 600, 0);
    modelMatrix_bumper = glm::mat3(1);
    modelMatrix_bumper *= transform2D::Translate(bumperPos.x, bumperPos.y);
    RenderMesh2D(meshes["bumper"], shaders["VertexColor"], modelMatrix_bumper);

    //randare tun
    glm::vec3 tunPos = glm::vec3(100, 370, 0);
    modelMatrix_tun = glm::mat3(1);
    modelMatrix_tun *= transform2D::Translate(tunPos.x, tunPos.y);
    RenderMesh2D(meshes["tun"], shaders["VertexColor"], modelMatrix_tun);


    //Patratele verzi
    // Un patrat verde are 48 x 48
    //duc patratul verde mai sus de al treilea patrat
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(350, 620);
    modelMatrix *= transform2D::Scale(1.2f, 1.2f);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    modelMatrix_start = glm::mat3(1);

    for (int i = 0; i < 10; i++)
    {
       
        modelMatrix *= transform2D::Translate(squareSide + 20, 0);

        if (i < 9)
        {
           
            RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
        }
        else 
        {
         
            RenderMesh2D(meshes["start"], shaders["VertexColor"], modelMatrix);
        }
    }

    //patrat albastru de dimensiune 400 x 280

    float conturX = 500;
    float conturY = 100;
    float conturWidth = 15 * squareSide; // 600
    float conturHeight = 10 * squareSide; // 400

    
    modelMatrix3 = glm::mat3(1);
    modelMatrix3 *= transform2D::Translate(conturX, conturY);
    modelMatrix3 *= transform2D::Scale(15, 10);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix3);


    int numCols = 12;
    int numRows = 7;

    //tot gridul
    float gridWidth = numCols * squareSide + (numCols - 1) * spatiu; // 590
    float gridHeight = numRows * squareSide + (numRows - 1) * spatiu; // 340

    //pentru a-l centra
    float offsetX = (conturWidth - gridWidth) / 2.0f;
    float offsetY = (conturHeight - gridHeight) / 2.0f;


    float startX = conturX + offsetX;
    float startY = conturY + offsetY;


    float step = squareSide + spatiu;

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            modelMatrix4 = glm::mat3(1);

            float currentX = startX + j * step;
            float currentY = startY + i * step;

            modelMatrix4 *= transform2D::Translate(currentX, currentY);
            RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix4);
        }
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
