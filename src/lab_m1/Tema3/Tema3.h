#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    struct TreeData {
        glm::vec3 position;
        float scale;
    };

    struct Firefly
    {
        float radius;
        float omega;
        float phase;

        float yAmp;
        float yOmega;
        float yPhase;

        glm::vec3 color;

        float intensity;
    };

    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* textureDiffuse, Texture2D* textureNormal);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
        GLboolean mixTextures;

        float elapsedTime;

        //copac
        std::vector<TreeData> sceneTrees;
        void RenderTree(const glm::vec3& pos, float scale);

        //turn
        void RenderTower(const glm::vec3& pos);

        //licurici

        std::vector<Firefly> fireflies;
        glm::vec3 GetFireflyPos(const Firefly& f, float t, const glm::vec3& center);
        void RenderFireflies(const glm::vec3& center);
        void RenderMeshSimple(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        //lumini
        void SendFireflyLights(Shader* shader, const glm::vec3& center);
        void SendSpotlights(Shader* shader);
    };
}   // namespace m1
