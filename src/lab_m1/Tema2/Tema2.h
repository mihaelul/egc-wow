#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "sineRandare.h"
#include "tipuriStatii.h"

namespace m1
{

    enum class TerrainType {
        Grass,
        Water,
        Mountain
    };



    struct TrainState {
        int railIndex = 0;
        int lastRailIndex = -1;
        float t = 0.0f;    
        float speed = 1.0f; 
        bool stopped = false;

        bool movingForward = true;

        int direction = 1.f;

        bool waitingAtStation = false;
        float stationTimer = 0.0f;
        int lastStationRailIndex = -1;
        int wagonCount = 0;

        bool waitingAtIntersection = false;
        int chosenNextRail = -1;
    };



    



    float DirToAngle(RailDir dir);
    bool IsRailValidForTerrain(RailType rail, TerrainType terrain);
    glm::vec3 RailEnd(const RailSegment& r);
    bool SamePos(const glm::vec3& a, const glm::vec3& b, float eps = 0.05f);
    glm::vec3 Snap(const glm::vec3& p, float step = 0.1f);



    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
  
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(
            Mesh* mesh,
            Shader* shader,
            const glm::mat4& modelMatrix,
            Texture2D* texture1 = nullptr,
            Texture2D* texture2 = nullptr
        );




        RailDir RotateDir(RailDir base, int delta);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        //teren

        static constexpr int MAP_W = 50;
        static constexpr int MAP_H = 30;

        TerrainType terrainGrid[MAP_H][MAP_W];

        glm::vec3 currentColor;
        std::unordered_map<std::string, Texture2D*> mapTextures;

        TerrainType GetTerrainUnderRail(const RailSegment& rail) const; 

        //tren
        void DrawTrainWagon(const glm::mat4& modelMatrix);
        void DrawLocomotive(const glm::mat4& modelMatrix);
        std::vector<RailSegment> railMap;
        TrainState trainState;

        //statie
        void DrawStation(const RailSegment& rail);
        bool CheckSphereCollision(const glm::vec3& aPos, float aRadius,
            const glm::vec3& bPos, float bRadius);
        std::vector<Station> stations;

        //CARGO DELIVERY
        Station centralStation;
        std::vector<StationType> currentOrder;
        float gameTimer = 60.0f;
        bool isGameOver = false;
        bool wasAtCentralStation = false;

        void ShowRailConnections(const std::vector<RailSegment>& railMap);

        void DrawCentralStation();
        void GenerateNewOrder();
        std::string StationTypeToString(StationType type);
    };
}
