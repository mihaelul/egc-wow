#include "lab_m1/Tema2/Tema2.h"
#include "shapes/shapes.h"
#include "sineRandare.h"
#include "tipuriSine.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2() {}
Tema2::~Tema2() {}

void Tema2::ShowRailConnections(const std::vector<RailSegment>& railMap)
{
    std::cout << "\n------ DEBUG: CONEXIUNI GRAF SINE ------" << std::endl;

    for (int i = 0; i < (int)railMap.size(); i++)
    {
        const auto& r = railMap[i];
        int numEnd = (int)r.nextAtEnd.size();
        int numStart = (int)r.nextAtStart.size();

        std::cout << "Sina [" << i << "]: ";
        std::cout << "START are " << numStart << " conexiuni | ";
        std::cout << "END are " << numEnd << " conexiuni";

        if (r.isIntersection) {
            std::cout << " <--- Intersectie";
        }

        if (numEnd == 0 && numStart == 0) {
            std::cout << " <--- Izolata";
        }

        std::cout << std::endl;

        if (numEnd > 0) {
            std::cout << "   -> END se leaga de: ";
            for (int idx : r.nextAtEnd) std::cout << idx << " ";
            std::cout << std::endl;
        }

        if (numStart > 0) {
            std::cout << "   -> START se leaga de: ";
            for (int idx : r.nextAtStart) std::cout << idx << " ";
            std::cout << std::endl;
        }
    }
    std::cout << "----------------------------------------\n" << std::endl;
}


void Tema2::GenerateNewOrder() {
    currentOrder.clear();
    StationType resources[] = { StationType::Cube, StationType::Sphere, StationType::Pyramid };

    std::cout << "\n========================================" << std::endl;
    std::cout << "COMANDA NOUA PRIMITA!" << std::endl;
    std::cout << "Trebuie sa colectezi urmatoarele 5 resurse:" << std::endl;

    for (int i = 0; i < 5; i++) {
        StationType randomRes = resources[rand() % 3];
        currentOrder.push_back(randomRes);

       
        std::cout << "  " << (i + 1) << ". " << StationTypeToString(randomRes) << std::endl;
    }

    std::cout << "Timp ramas: " << (int)gameTimer << " secunde." << std::endl;
    std::cout << "========================================\n" << std::endl;

    gameTimer = 60.0f; 
}

void Tema2::Init()
{
    currentColor = glm::vec3(1);
    trainState.wagonCount = 0;

    // Meshuri pentru statii
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("pyramid");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    //Meshuri pentru tren
    train::CreateTrainMeshes(meshes);
    train::CreateLocomotiveMeshes(meshes);

    //Tile de pamant
    meshes["terrain_tile"] = train::CreateTerrainTile("terrain_tile", 5.0f);

    //Meshuri sine
    meshes["rail_normal"] = train::CreateNormalRail("rail_normal", 1.5f);
    meshes["rail_bridge"] = train::CreateBridgeRail("rail_bridge", 2.f);
    meshes["rail_tunnel"] = train::CreateTunnelRail("rail_tunnel", 2.f);

   
    //randare sine
    sineRandare::BuildRailMap(railMap);

    //construire graf
    sineRandare::BuildRailGraph(railMap,0.4f);
    sineRandare::MarkIntersections(railMap);

    //tipuri statii

    stations =
    {
        { StationType::Cube,    glm::vec3(-4.5f, 0.3f, -1.5f), 0.6f},
        { StationType::Sphere,  glm::vec3(2.5f, 0.3f,  4.5f), 0.6f },
        { StationType::Pyramid, glm::vec3(6.0f, 0.3f, 10.5f), 0.6f }
    };
  



    trainState.railIndex =35;
    trainState.t = 0.0f;
    trainState.speed =10.f;

    //generare teren 
    for (int y = 0; y < MAP_H; y++)
        for (int x = 0; x < MAP_W; x++)
            terrainGrid[y][x] = TerrainType::Grass;

    // apa

    //prima zona
    for (int i = 0; i < 30; i++)
    {
        terrainGrid[i][3] = TerrainType::Water;
        terrainGrid[i][4] = TerrainType::Water;

    }

	//a doua zona 

    for (int i = 3; i < 50; i++)
    {
        terrainGrid[4][i] = TerrainType::Water;
        terrainGrid[5][i] = TerrainType::Water;

    }

    //munte
    //prima zona
    terrainGrid[15][8] = TerrainType::Mountain;
    terrainGrid[16][8] = TerrainType::Mountain;
    terrainGrid[15][9] = TerrainType::Mountain;
    terrainGrid[16][9] = TerrainType::Mountain;
    //a doua zona

    for (int i = 18; i < 22; i++)
    {
        for(int j= 14; j < 17; j++)
        {
            terrainGrid[i][j] = TerrainType::Mountain;
		}
    }
   
    //a treia zona

    for (int i = 9; i < 15; i++)
    {
        for (int j = 29; j < 35; j++)
        {
            terrainGrid[i][j] = TerrainType::Mountain;
        }
    }

    //CARGO DELIVERY

    centralStation.type = StationType::Cube;
    centralStation.position = glm::vec3(12.0f, 0.5f, 9.0f); 
    centralStation.radius = 1.5f; 

    srand(time(NULL));
    GenerateNewOrder();


    // ===== Shader =====
    Shader* shader = new Shader("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    //camera

    auto camera = GetSceneCamera();

    // ridică camera pe Y
    camera->SetPosition(glm::vec3(15.0f, 20.0f, 7.0f));
    camera->SetRotation(glm::vec3(-RADIANS(90), 0.0f, 0.0f));

    camera->Update();

    ShowRailConnections(railMap);
}


void Tema2::FrameStart()
{
    glClearColor(0.52f, 0.81f, 0.92f, 1); // Cer albastru
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::DrawTrainWagon(const glm::mat4& modelMatrix)
{
    Shader* wagonShader = shaders["LabShader"];
    // Partea verde
    {
        glm::mat4 componentModel = glm::translate(modelMatrix, glm::vec3(0, 0.55f, 0));
        componentModel = glm::scale(componentModel, glm::vec3(0.5f));
        currentColor = glm::vec3(0, 1, 0);
        RenderSimpleMesh(meshes["vagon_verde"], wagonShader, componentModel);
    }
    // Partea galbena
    {
        glm::mat4 componentModel = glm::translate(modelMatrix, glm::vec3(0, 0.25f, 0));
        componentModel = glm::scale(componentModel, glm::vec3(0.5f));
        currentColor = glm::vec3(1, 1, 0);
        RenderSimpleMesh(meshes["vagon_galben"], wagonShader, componentModel);
    }
    // Rotile
    vector<glm::vec3> wheelPositions = {
        glm::vec3(0.25f, 0.1f, 0.1f), glm::vec3(-0.1f, 0.1f, 0.1f),
        glm::vec3(0.25f, 0.1f, 1.4f), glm::vec3(-0.1f, 0.1f, 1.4f)
    };
    for (const auto& pos : wheelPositions) {
        glm::mat4 componentModel = glm::translate(modelMatrix, pos);
        componentModel = glm::rotate(componentModel, RADIANS(90), glm::vec3(0, 0, 1));
        componentModel = glm::scale(componentModel, glm::vec3(0.5f));
        currentColor = glm::vec3(0.2f, 0.2f, 0.2f);
        RenderSimpleMesh(meshes["sphere"], wagonShader, componentModel);
    }
}

void Tema2::DrawLocomotive(const glm::mat4& modelMatrix)
{
    Shader* shader = shaders["LabShader"];

    // Partea albastra
    {
        glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, RADIANS(90), glm::vec3(1, 0, 0));


        currentColor = glm::vec3(0, 0, 1); // albastru
        RenderSimpleMesh(meshes["loco_body"], shader, model, mapTextures["grass"]);
    }

    // Partea verde
    {
        glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0.f, 0.55f, 1.f));
        model = glm::scale(model, glm::vec3(0.5f));

        currentColor = glm::vec3(0, 1, 0); // verde
        RenderSimpleMesh(meshes["loco_cabin"], shader, model, mapTextures["grass"]);
    }

    // Partea galbena
    {
        glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0, 0.25f, 0));
        model = glm::scale(model, glm::vec3(0.5f));

        currentColor = glm::vec3(1, 1, 0); // galben
        RenderSimpleMesh(meshes["vagon_galben"], shader, model, mapTextures["grass"]);
    }

    // Cilindru mov din centru
    {
        glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0.0f, 0.55f, -0.2f));
        model = glm::rotate(model, RADIANS(90), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.5f));

        currentColor = glm::vec3(0.7f, 0.2f, 0.8f); // mov
        RenderSimpleMesh(meshes["center_cylinder_body"], shader, model, mapTextures["grass"]);
    }

    //Roti
    vector<glm::vec3> wheels = {
        glm::vec3(0.2f, 0.1f, 0.1f),
        glm::vec3(0.2f, 0.1f, 0.3f),
        glm::vec3(0.2f, 0.1f, 0.5f),
        glm::vec3(0.2f, 0.1f, 0.7f),
        glm::vec3(0.2f, 0.1f, 0.9f),
        glm::vec3(0.2f, 0.1f, 1.1f),
        glm::vec3(0.2f, 0.1f, 1.3f)
    };

    for (const auto& pos : wheels)
    {
        glm::mat4 model = glm::translate(modelMatrix, pos);
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, RADIANS(90), glm::vec3(0, 0, 1));

        currentColor = glm::vec3(1, 0, 0); // roșu
        RenderSimpleMesh(meshes["wheels_body"], shader, model, mapTextures["grass"]);
    }
}



namespace m1 {

  
    //functie pentru a invarti trenul
    float DirToAngle(RailDir dir)
    {
        switch (dir) {
        case RailDir::Forward:  return 0.0f;
        case RailDir::Right:    return RADIANS(90);
        case RailDir::Backward: return RADIANS(180);
        case RailDir::Left:     return RADIANS(-90);
        }
        return 0.0f;
    }

    static glm::vec3 RailEnd(const RailSegment& r)
    {
        return r.start + DirToVector(r.dir) * r.length;
    }

    static glm::vec3 Snap(const glm::vec3& p, float step)
    {
        return glm::vec3(
            round(p.x / step) * step,
            round(p.y / step) * step,
            round(p.z / step) * step
        );
    }

    static bool SamePos(const glm::vec3& a, const glm::vec3& b, float eps)
    {
        glm::vec3 A = Snap(a, 0.1f);
        glm::vec3 B = Snap(b, 0.1f);
        return glm::distance(A, B) < eps;
    }

    //Validare teren
    bool IsRailValidForTerrain(RailType rail, TerrainType terrain)
    {
        switch (rail)
        {
        case RailType::Normal:
            return terrain == TerrainType::Grass;

        case RailType::Bridge:
            return terrain == TerrainType::Water;

        case RailType::Tunnel:
            return terrain == TerrainType::Mountain;
        }

        return false;
    }

    const char* TerrainToString(TerrainType t)
    {
        switch (t)
        {
        case TerrainType::Grass: return "Grass";
        case TerrainType::Water: return "Water";
        case TerrainType::Mountain: return "Mountain";
        default: return "Unknown";
        }
    }

    const char* RailTypeToString(RailType r)
    {
        switch (r)
        {
        case RailType::Normal: return "Normal";
        case RailType::Bridge: return "Bridge";
        case RailType::Tunnel: return "Tunnel";
        default: return "Unknown";
        }
    }



} 

//CARGO DELIVERY

std::string Tema2::StationTypeToString(StationType type) {
    switch (type) {
    case StationType::Cube:    return "Cilindru (Rosu)";
    case StationType::Sphere:  return "SFERA (Albastru)";
    case StationType::Pyramid: return "TEAPOT (Galben)";
    default:                   return "NECUNOSCUT";
    }
}

//detectie teren sub sina
TerrainType Tema2::GetTerrainUnderRail(const RailSegment& rail) const
{
    glm::vec3 gridOrigin = railMap[0].start;
    float tileSize = 1.0f;

    glm::vec3 dir = DirToVector(rail.dir);
    glm::vec3 center = rail.start + dir * (rail.length * 0.5f);

    int gx = int(floor((center.x - gridOrigin.x) / tileSize));
    int gy = int(floor((center.z - gridOrigin.z) / tileSize));

    if (gx < 0 || gx >= MAP_W || gy < 0 || gy >= MAP_H)
        return TerrainType::Grass;

    return terrainGrid[gy][gx];
}


//desenare statie 

void Tema2::DrawStation(const RailSegment& rail)
{
    if (rail.station == StationType::None)
        return;

   
    glm::vec3 dir = DirToVector(rail.dir);


    glm::vec3 side;
    if (rail.dir == RailDir::Forward || rail.dir == RailDir::Backward)
        side = glm::vec3(1, 0, 0);
    else
        side = glm::vec3(0, 0, 1);

  //pozitia statiei (mijlocul statiei)
    glm::vec3 stationPos =
        rail.start +
        dir * (rail.length * 0.5f) +
        side * 0.6f;

    glm::mat4 model = glm::translate(glm::mat4(1), stationPos);

    Mesh* mesh = nullptr;

    switch (rail.station)
    {
    case StationType::Cube:
        mesh = meshes["loco_body"];
        currentColor = glm::vec3(1.0f, 0.2f, 0.2f); // rosu
        model = glm::scale(model, glm::vec3(0.5f));
        break;

    case StationType::Sphere:
        mesh = meshes["sphere"];
        currentColor = glm::vec3(0.2f, 0.8f, 1.0f); // albastru
        model = glm::scale(model, glm::vec3(0.7f));
        break;

    case StationType::Pyramid:
        mesh = meshes["pyramid"];
        currentColor = glm::vec3(1.0f, 1.0f, 0.2f); // galben
        model = glm::scale(model, glm::vec3(0.7f));
        break;

    default:
        return;
    }

    RenderSimpleMesh(mesh, shaders["LabShader"], model);
}

//desenare gara centrala

void Tema2::DrawCentralStation()
{
    float maxOrderTime = 60.0f;
  
    float t = glm::clamp(gameTimer / maxOrderTime, 0.0f, 1.0f);

  
    glm::vec3 colorGreen = glm::vec3(0.0f, 1.0f, 0.0f); //verde
    glm::vec3 colorYellow = glm::vec3(1.0f, 1.0f, 0.0f); //galben
    glm::vec3 colorRed = glm::vec3(1.0f, 0.0f, 0.0f); //rosu

    glm::vec3 stationColor;

    if (t > 0.5f) {
 
        stationColor = glm::mix(colorYellow, colorGreen, (t - 0.5f) * 2.0f);
    }
    else {
   
        stationColor = glm::mix(colorRed, colorYellow, t * 2.0f);
    }

 
    if (t < 0.2f) {
      
        float pulse = 0.5f + 0.5f * sin(Engine::GetElapsedTime() * 10.0f);
        stationColor = glm::mix(stationColor, glm::vec3(1.0f, 0.5f, 0.5f), pulse * (1.0f - t / 0.2f));
    }

  
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), centralStation.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(centralStation.radius * 2.0f));

    currentColor = stationColor;
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix);
}
//detectare coliziune
bool Tema2::CheckSphereCollision(
    const glm::vec3& aPos, float aRadius,
    const glm::vec3& bPos, float bRadius)
{
    float dist = glm::distance(aPos, bPos);
    return dist <= (aRadius + bRadius);
}

//logica


void Tema2::Update(float deltaTimeSeconds)
{
    
    // generare tren
    float tileSize = 1.0f;
    float scaleFactor = 0.1f;
    glm::vec3 gridOrigin = railMap[0].start;

    if (isGameOver) return;
    float trainRadius = 1.f;

    gameTimer -= deltaTimeSeconds;
    if (gameTimer <= 0) {
        isGameOver = true;
        return;
    }

    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            float height = -0.01f;
            switch (terrainGrid[y][x]) {
            case TerrainType::Grass: currentColor = glm::vec3(0.6f, 1.0f, 0.6f); break;
            case TerrainType::Water: currentColor = glm::vec3(0.2f, 0.4f, 0.9f); height = 0.0f; break;
            case TerrainType::Mountain: currentColor = glm::vec3(0.5f, 0.35f, 0.15f); height = 0.05f; break;
            }
            glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(gridOrigin.x + x * tileSize, height, gridOrigin.z + y * tileSize));
            model = glm::scale(model, glm::vec3(scaleFactor));
            RenderSimpleMesh(meshes["terrain_tile"], shaders["LabShader"], model);
        }
    }

    //generare sine
    currentColor = glm::vec3(0.2f);
    for (auto& rail : railMap) {
        glm::mat4 model = glm::translate(glm::mat4(1), rail.start);
        model = glm::rotate(model, DirToAngle(rail.dir), glm::vec3(0, 1, 0));
        Mesh* mesh = nullptr;
        if (rail.type == RailType::Normal) mesh = meshes["rail_normal"];
        if (rail.type == RailType::Bridge) mesh = meshes["rail_bridge"];
        if (rail.type == RailType::Tunnel) mesh = meshes["rail_tunnel"];
        RenderSimpleMesh(mesh, shaders["LabShader"], model);
    }

   //generare statii
    for (const auto& rail : railMap) {
        if (rail.station != StationType::None)
            DrawStation(rail);
    }

    if (trainState.stopped) return;

    //logica de detectie a statiei
    RailSegment& currentRail = railMap[trainState.railIndex];
    bool freezeMovement = false;

    bool isResourceNeeded = false;
    if (currentRail.station != StationType::None) {
        auto it = std::find(currentOrder.begin(), currentOrder.end(), currentRail.station);
        if (it != currentOrder.end()) {
            isResourceNeeded = true;
        }
    }

   
    bool justEntered = (trainState.movingForward && trainState.t < 0.05f) ||
        (!trainState.movingForward && trainState.t > 0.95f);

    if (currentRail.station != StationType::None && !trainState.waitingAtStation && justEntered && isResourceNeeded) {
        trainState.waitingAtStation = true;
        trainState.stationTimer = 2.0f;

       //logica de colectare a resurselor
        auto it = std::find(currentOrder.begin(), currentOrder.end(), currentRail.station);

        if (it != currentOrder.end()) {
           
            currentOrder.erase(it);
            trainState.wagonCount++;
            std::cout << "Colectat: " << StationTypeToString(currentRail.station) << std::endl;
            std::cout << "Mai ai de adunat " << currentOrder.size() << " resurse." << std::endl;

            if (currentOrder.empty()) {
                std::cout << "TOATE RESURSELE COLECTATE!" << std::endl;
            }
        }
        else {
            std::cout << "Ignor gara: " << StationTypeToString(currentRail.station) << " (nu e pe lista)" << std::endl;
        }
    }

    if (trainState.waitingAtStation) {
        trainState.stationTimer -= deltaTimeSeconds;
        if (trainState.stationTimer <= 0.0f) {
            trainState.waitingAtStation = false;
            std::cout << "Plecare din statie\n"; //debug
        }
        else {
            freezeMovement = true;
        }
    }

    if (trainState.waitingAtIntersection) freezeMovement = true;

   // logica de miscare a trenului
    float remainingDistance = freezeMovement ? 0.0f : trainState.speed * deltaTimeSeconds;

    while (remainingDistance > 0.0f) {
        RailSegment& railRef = railMap[trainState.railIndex];

       
        float distToEdge = trainState.movingForward
            ? railRef.length * (1.0f - trainState.t)
            : railRef.length * trainState.t;

        if (remainingDistance < distToEdge) {
            float deltaT = remainingDistance / railRef.length;
            trainState.t += trainState.movingForward ? deltaT : -deltaT;
            remainingDistance = 0.0f;
        }
        else {
     
            remainingDistance -= distToEdge;

    
            auto& nextOptions = trainState.movingForward ? railRef.nextAtEnd : railRef.nextAtStart;
            auto& nextDirs = trainState.movingForward ? railRef.forwardAtEnd : railRef.forwardAtStart;

            if (nextOptions.size() >= 2) {
                trainState.waitingAtIntersection = true;
                trainState.t = trainState.movingForward ? 1.0f : 0.0f; 

                int noseDir = (int)railRef.dir;
                if (!trainState.movingForward) noseDir = (noseDir + 2) % 4;

                std::cout << "\n INTERSECTIE! Directii la tren:\n";

                for (size_t k = 0; k < nextOptions.size(); k++) {
                    int nextIdx = nextOptions[k];
                    bool entersForward = nextDirs[k];

                    int nextHeading = (int)railMap[nextIdx].dir;
                    if (!entersForward) nextHeading = (nextHeading + 2) % 4;

                    int diff = (nextHeading - noseDir + 4) % 4;

                    switch (diff) {
                    case 0: std::cout << "  - [W]: Inainte\n"; break;
                    case 1: std::cout << "  - [D]: La dreapta\n"; break;
                    case 2: std::cout << "  - [S]: Intoarcere (pe alta sina)\n"; break;
                    case 3: std::cout << "  - [A]: La stanga\n"; break;
                    }
                }
                return;
            }

 
            if (!nextOptions.empty()) {

                int nextIdx = nextOptions[0];
                bool nextMovingForward = nextDirs[0];

                trainState.railIndex = nextIdx;
                trainState.movingForward = nextMovingForward;
                trainState.t = nextMovingForward ? 0.0f : 1.0f;
            }
            else {

                trainState.movingForward = !trainState.movingForward;
                remainingDistance = 0.0f;
            }
        }
    }


    RailSegment& finalRail = railMap[trainState.railIndex];
    glm::vec3 trainPos = finalRail.start + DirToVector(finalRail.dir) * (finalRail.length * trainState.t);

   //validare tereb
    TerrainType terrainUnderRail = GetTerrainUnderRail(finalRail);
    if (!IsRailValidForTerrain(finalRail.type, terrainUnderRail)) {
        trainState.stopped = true;
        std::cout << "Teren incompatibil!\n";
        return;
    }

    // coliziune gara centrala
    float distanceToStation = glm::distance(trainPos, centralStation.position);
    bool isColliding = CheckSphereCollision(trainPos, 0.1f, centralStation.position, centralStation.radius);
    if (isColliding) {
        if (currentOrder.empty() && !wasAtCentralStation) {
            std::cout << "Misiune indeplinita! Se genereaza o noua comanda..." << std::endl;
            trainState.wagonCount = 0;
            GenerateNewOrder();
            wasAtCentralStation = true;
        }
    }
    else {
        wasAtCentralStation = false;
    }

   
    float angleCurrent = DirToAngle(finalRail.dir);

    if (!trainState.movingForward) angleCurrent += M_PI;

    float finalAngle = angleCurrent;

    //viraje
    auto& nextInterp = trainState.movingForward ? finalRail.nextAtEnd : finalRail.nextAtStart;
    auto& nextInterpDirs = trainState.movingForward ? finalRail.forwardAtEnd : finalRail.forwardAtStart;


    bool isNearExit = (trainState.movingForward && trainState.t > 0.4f) || (!trainState.movingForward && trainState.t < 0.6f);

    if (!nextInterp.empty() && isNearExit) {
        float angleNext = DirToAngle(railMap[nextInterp[0]].dir);

        if (!nextInterpDirs[0]) angleNext += M_PI;

        float delta = angleNext - angleCurrent;
        while (delta > M_PI)  delta -= 2 * M_PI;
        while (delta < -M_PI) delta += 2 * M_PI;

        float localT = trainState.movingForward
            ? (trainState.t - 0.4f) / 0.6f
            : (0.6f - trainState.t) / 0.6f;

        localT = glm::clamp(localT, 0.0f, 1.0f);
        finalAngle = angleCurrent + delta * localT;
    }


    DrawCentralStation();
    glm::mat4 trainMatrix = glm::translate(glm::mat4(1), trainPos);
    trainMatrix = glm::rotate(trainMatrix, finalAngle, glm::vec3(0, 1, 0));
    trainMatrix = glm::rotate(trainMatrix, RADIANS(180), glm::vec3(0, 1, 0));
    DrawLocomotive(trainMatrix);

    const float wagonSpacing = 1.6f;
    for (int i = 0; i < trainState.wagonCount; i++) {
        glm::mat4 wagonMatrix = trainMatrix;
        
        wagonMatrix = glm::translate(wagonMatrix, glm::vec3(0, 0, (i + 1) * wagonSpacing));
        DrawTrainWagon(wagonMatrix);
    }
}

RailDir Tema2::RotateDir(RailDir base, int delta)
{
    
    int d = (int(base) + delta) % 4;
    if (d < 0) d += 4;
    return (RailDir)d;
}

void Tema2::FrameEnd() { DrawCoordinateSystem(); }

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    GLint loc_model = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_proj = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint loc_color = glGetUniformLocation(shader->program, "objectColor");
    glUniform3fv(loc_color, 1, glm::value_ptr(currentColor));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {}
void Tema2::OnKeyPress(int key, int mods) {
    if (!trainState.waitingAtIntersection) return;

    RailSegment& rail = railMap[trainState.railIndex];
    bool atEnd = (trainState.t > 0.5f);
    auto& nextOptions = atEnd ? rail.nextAtEnd : rail.nextAtStart;
    auto& nextDirs = atEnd ? rail.forwardAtEnd : rail.forwardAtStart;

    glm::vec3 noseHeading = sineRandare::GetWorldHeading(rail, trainState.movingForward);
    if (key == GLFW_KEY_S) {
        trainState.movingForward = !trainState.movingForward;
        trainState.waitingAtIntersection = false;
        std::cout << "↩️ Intoarcere pe acelasi segment\n";
        return;
    }

    for (size_t k = 0; k < nextOptions.size(); k++) {
        int nextIdx = nextOptions[k];
        bool entersForward = nextDirs[k];

        glm::vec3 candidateHeading = sineRandare::GetWorldHeading(railMap[nextIdx], entersForward);
        float dot = glm::dot(glm::normalize(noseHeading), glm::normalize(candidateHeading));
        glm::vec3 cross = glm::cross(glm::normalize(noseHeading), glm::normalize(candidateHeading));

        bool match = false;


        if (key == GLFW_KEY_W && dot > 0.8f) match = true;


        if (key == GLFW_KEY_D && dot < 0.2f && dot > -0.2f && cross.y < -0.8f) match = true;


        if (key == GLFW_KEY_A && dot < 0.2f && dot > -0.2f && cross.y > 0.8f) match = true;

        if (match) {
            trainState.railIndex = nextIdx;
            trainState.movingForward = entersForward;
            trainState.t = trainState.movingForward ? 0.0f : 1.0f;
            trainState.waitingAtIntersection = false;
            std::cout << " Directie acceptata: " << (key == GLFW_KEY_W ? "Inainte" : (key == GLFW_KEY_A ? "Stanga" : "Dreapta")) << "\n";
            return;
        }
    }

    std::cout << "Nu exista o sina in acea directie.\n";
}

void Tema2::OnKeyRelease(int key, int mods) {}
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Tema2::OnWindowResize(int width, int height) {}