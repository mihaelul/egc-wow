#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRadialSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length)
{

    const glm::vec3 outerColor = glm::vec3(0.5f, 0.5f, 0.5f);

    const glm::vec3 centerColor = glm::vec3(0.8f, 0.8f, 0.8f);

    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {

        VertexFormat(corner, outerColor),
        VertexFormat(corner + glm::vec3(length, 0, 0), outerColor),
        VertexFormat(corner + glm::vec3(length, length, 0), outerColor),
        VertexFormat(corner + glm::vec3(0, length, 0), outerColor)
    };

    glm::vec3 centerPoint = corner + glm::vec3(length / 2.0f, length / 2.0f, 0);
    vertices.push_back(VertexFormat(centerPoint, centerColor));
    Mesh* square = new Mesh(name);


    std::vector<unsigned int> indices = {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    square->SetDrawMode(GL_TRIANGLES);

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateMotor(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length)
{
    glm::vec3 corner = leftBottomCorner;

    const glm::vec3 bodyColor = glm::vec3(0.95f, 0.6f, 0.1f);
    const glm::vec3 flameColor = glm::vec3(0.9f, 0.35f, 0.0f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;



    //patrat simplu
    vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, length, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), bodyColor));
    vertices.push_back(VertexFormat(corner, bodyColor));

    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);


    float flameHeight = length * 0.75f;
    float spikeDepth = flameHeight * 0.7f;
    int numSpikes = 4;

    unsigned int flame_part_start_idx = vertices.size();


    vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), flameColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), flameColor));


    float x_step = length / numSpikes;
    float current_x = 0;


    for (int i = 0; i <= numSpikes; ++i) {
        vertices.push_back(VertexFormat(corner + glm::vec3(current_x, -flameHeight, 0), flameColor));
        current_x += x_step;
    }


    current_x = 0;
    for (int i = 0; i < numSpikes; ++i) {
        current_x = (i + 0.5f) * x_step;
        vertices.push_back(VertexFormat(corner + glm::vec3(current_x, -flameHeight - spikeDepth, 0), flameColor));
    }

    // Flame indicies
    unsigned int base_line_start_idx = flame_part_start_idx + 2;
    indices.push_back(flame_part_start_idx + 0); indices.push_back(flame_part_start_idx + 1); indices.push_back(base_line_start_idx + numSpikes);
    indices.push_back(flame_part_start_idx + 0); indices.push_back(base_line_start_idx + numSpikes); indices.push_back(base_line_start_idx);


    unsigned int spike_tip_start_idx = flame_part_start_idx + 2 + numSpikes + 1;
    for (int i = 0; i < numSpikes; ++i) {
        indices.push_back(base_line_start_idx + i);
        indices.push_back(spike_tip_start_idx + i);
        indices.push_back(base_line_start_idx + i + 1);
    }


    Mesh* motor = new Mesh(name);
    motor->SetDrawMode(GL_TRIANGLES);
    motor->InitFromData(vertices, indices);
    return motor;
}

Mesh* object2D::CreateBumper(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float squareLength)
{
    glm::vec3 corner = leftBottomCorner;

    const glm::vec3 stemColor = glm::vec3(0.85f, 0.85f, 0.85f);
    const glm::vec3 capColor = glm::vec3(0.75f, 0.85f, 0.4f);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    //patrat 
    unsigned int stem_start_idx = 0;
    vertices.push_back(VertexFormat(corner, stemColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(squareLength, 0, 0), stemColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(squareLength, squareLength, 0), stemColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, squareLength, 0), stemColor));

    indices.push_back(stem_start_idx + 0); indices.push_back(stem_start_idx + 1); indices.push_back(stem_start_idx + 2);
    indices.push_back(stem_start_idx + 0); indices.push_back(stem_start_idx + 2); indices.push_back(stem_start_idx + 3);

    //semicerc 
    unsigned int cap_start_idx = vertices.size();

    float radiusX = 1.5f * squareLength;
    float radiusY = 1.0f * squareLength;


    glm::vec3 ellipseCenter = corner + glm::vec3(squareLength / 2.0f, squareLength, 0);


    vertices.push_back(VertexFormat(ellipseCenter, capColor));

    int numSegments = 30;


    for (int i = 0; i <= numSegments; ++i) {
        float angle = M_PI - (float)i / numSegments * M_PI;
        float x = ellipseCenter.x + radiusX * cos(angle);
        float y = ellipseCenter.y + radiusY * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), capColor));
    }


    for (int i = 0; i < numSegments; ++i) {
        indices.push_back(cap_start_idx + 0);
        indices.push_back(cap_start_idx + 1 + i);
        indices.push_back(cap_start_idx + 1 + i + 1);
    }

    Mesh* bumper = new Mesh(name);
    bumper->SetDrawMode(GL_TRIANGLES);
    bumper->InitFromData(vertices, indices);
    return bumper;
}

Mesh* object2D::CreateTun(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length)
{
    glm::vec3 corner = leftBottomCorner;

    const glm::vec3 bodyColor = glm::vec3(0.3f, 0.3f, 0.3f);
    const glm::vec3 baseColor = glm::vec3(0.7f, 0.7f, 0.7f);
    const glm::vec3 baseBottomColor = glm::vec3(0.85f, 0.85f, 0.85f);

    float bodyWidth = 1.2f * length;
    float bodyHeight = 3.0f * length;
    float baseRectHeight = 0.5f * length;
    float baseArcRadius = 0.7f * length;
    float capHeight = 0.5f * length;
    float capWidthFactor = 0.5f;

    float epsilon = 0.001f;
    //suprapunere
    float overlap_amount = 0.7f * baseArcRadius;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    unsigned int current_index_start = 0;

    // dreptunghi de sub semicerc
    current_index_start = vertices.size();

    vertices.push_back(VertexFormat(corner, baseBottomColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(bodyWidth, 0, 0), baseBottomColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(bodyWidth, baseRectHeight, 0), baseBottomColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, baseRectHeight, 0), baseBottomColor));

    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 1); indices.push_back(current_index_start + 2);
    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 2); indices.push_back(current_index_start + 3);

    // semicerc
    current_index_start = vertices.size();

    float radiusX = bodyWidth / 2.0f;
    float radiusY = baseArcRadius;

    glm::vec3 ellipseCenter = corner + glm::vec3(bodyWidth / 2.0f, baseRectHeight - epsilon, 0);

    vertices.push_back(VertexFormat(ellipseCenter, baseColor));

    int numSegments = 30;

    for (int i = 0; i <= numSegments; ++i) {
        float angle = M_PI - (float)i / numSegments * M_PI;
        float x = ellipseCenter.x + radiusX * cos(angle);
        float y = ellipseCenter.y + radiusY * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), baseColor));
    }

    for (int i = 0; i < numSegments; ++i) {
        indices.push_back(current_index_start + 0);
        indices.push_back(current_index_start + 1 + i);
        indices.push_back(current_index_start + 1 + i + 1);
    }

    //baza
    current_index_start = vertices.size();


    float bodyRectYStart = baseRectHeight + baseArcRadius - overlap_amount;

    vertices.push_back(VertexFormat(corner + glm::vec3(0, bodyRectYStart, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(bodyWidth, bodyRectYStart, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(bodyWidth, bodyRectYStart + bodyHeight, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, bodyRectYStart + bodyHeight, 0), bodyColor));

    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 1); indices.push_back(current_index_start + 2);
    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 2); indices.push_back(current_index_start + 3);

    //trapez
    current_index_start = vertices.size();

    float capYStart = bodyRectYStart + bodyHeight;
    float topWidth = bodyWidth * capWidthFactor;
    float topXCenter = corner.x + bodyWidth / 2.0f;

    vertices.push_back(VertexFormat(corner + glm::vec3(0, capYStart, 0), bodyColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(bodyWidth, capYStart, 0), bodyColor));
    vertices.push_back(VertexFormat(glm::vec3(topXCenter + topWidth / 2.0f, capYStart + capHeight, 0), bodyColor));
    vertices.push_back(VertexFormat(glm::vec3(topXCenter - topWidth / 2.0f, capYStart + capHeight, 0), bodyColor));

    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 1); indices.push_back(current_index_start + 2);
    indices.push_back(current_index_start + 0); indices.push_back(current_index_start + 2); indices.push_back(current_index_start + 3);


    Mesh* rocket = new Mesh(name);
    rocket->SetDrawMode(GL_TRIANGLES);
    rocket->InitFromData(vertices, indices);
    return rocket;
}

Mesh* object2D::CreateStart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length)
{
    glm::vec3 corner = leftBottomCorner;

    const glm::vec3 GREEN_COLOR = glm::vec3(0, 1, 0);

    float width = 1.0f * length;
    float height = 1.0f * length;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;


    vertices.push_back(VertexFormat(corner, GREEN_COLOR));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, height, 0), GREEN_COLOR));
    vertices.push_back(VertexFormat(corner + glm::vec3(width, height, 0), GREEN_COLOR));
    vertices.push_back(VertexFormat(corner + glm::vec3(width, 0, 0), GREEN_COLOR));

    vertices.push_back(VertexFormat(corner + glm::vec3(width * 0.50f, height / 2.0f, 0), GREEN_COLOR));


    indices.push_back(0); indices.push_back(1); indices.push_back(4);
    indices.push_back(0); indices.push_back(4); indices.push_back(3);
    indices.push_back(1); indices.push_back(2); indices.push_back(4);

    Mesh* flag = new Mesh(name);
    flag->SetDrawMode(GL_TRIANGLES);
    flag->InitFromData(vertices, indices);
    return flag;
}