#include "lab_m1/Tema2/objects.h"
#include <vector>
#include <cmath>

using namespace std;

// helper declarations
void AddBoxToMesh(vector<VertexFormat>& vertices, vector<unsigned int>& indices,
    glm::vec3 center, float length, float height, float width, glm::vec3 color);
void AddBoxRotatedToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float length,
    float height, float width, float rotationY, glm::vec3 color);
void AddCylinderToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float height,
    float radius, int numSegments, glm::vec3 color);
void AddTrapezoidToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float bottomLen,
    float topLen, float height, float width, glm::vec3 color);

// helper implementations
void AddBoxToMesh(vector<VertexFormat>& vertices, vector<unsigned int>& indices,
    glm::vec3 center, float length, float height, float width, glm::vec3 color)
{
    AddBoxRotatedToMesh(vertices, indices, center, length, height, width, 0,
        color);
}

void AddBoxRotatedToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float length,
    float height, float width, float rotationY, glm::vec3 color)
{
    float w = length / 2.0f; float h = height / 2.0f; float d = width / 2.0f;
    unsigned int startIdx = (unsigned int)vertices.size();

    glm::vec3 corners[8] = {
        glm::vec3(-w, -h,  d), glm::vec3(w, -h,  d),
        glm::vec3(-w,  h,  d), glm::vec3(w,  h,  d),
        glm::vec3(-w, -h, -d), glm::vec3(w, -h, -d),
        glm::vec3(-w,  h, -d), glm::vec3(w,  h, -d)
    };

    float c = cos(rotationY); float s = sin(rotationY);
    for (int i = 0; i < 8; i++) {
        float rx = corners[i].x * c + corners[i].z * s;
        float rz = -corners[i].x * s + corners[i].z * c;
        vertices.push_back(VertexFormat(
            center + glm::vec3(rx, corners[i].y, rz), color));
    }
    vector<unsigned int> boxIndices = {
        0, 1, 2, 1, 3, 2, 2, 3, 7, 2, 7, 6, 1, 7, 3, 1, 5, 7,
        6, 7, 4, 7, 5, 4, 0, 4, 1, 1, 4, 5, 2, 6, 4, 0, 2, 4
    };
    for (unsigned int idx : boxIndices) indices.push_back(startIdx + idx);
}

void AddCylinderToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float height,
    float radius, int numSegments, glm::vec3 color)
{
    unsigned int startIdx = (unsigned int)vertices.size();
    vertices.push_back(VertexFormat(
        center + glm::vec3(0, -height / 2.0f, 0), color));
    vertices.push_back(VertexFormat(
        center + glm::vec3(0, height / 2.0f, 0), color));
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        vertices.push_back(VertexFormat(
            center + glm::vec3(x, -height / 2.0f, z), color));
        vertices.push_back(VertexFormat(
            center + glm::vec3(x, height / 2.0f, z), color));
    }
    for (int i = 0; i < numSegments; i++) {
        int c = 2 + i * 2; int n = 2 + ((i + 1) % numSegments) * 2;
        indices.push_back(startIdx + c); indices.push_back(startIdx + c + 1);
        indices.push_back(startIdx + n); indices.push_back(startIdx + n);
        indices.push_back(startIdx + c + 1); indices.push_back(startIdx + n + 1);
        indices.push_back(startIdx + 0); indices.push_back(startIdx + n);
        indices.push_back(startIdx + c); indices.push_back(startIdx + 1);
        indices.push_back(startIdx + c + 1); indices.push_back(startIdx + n + 1);
    }
}

void AddTrapezoidToMesh(vector<VertexFormat>& vertices,
    vector<unsigned int>& indices, glm::vec3 center, float bottomLen,
    float topLen, float height, float width, glm::vec3 color)
{
    float wB = bottomLen / 2; float wT = topLen / 2;
    float h = height / 2; float d = width / 2;
    unsigned int startIdx = (unsigned int)vertices.size();

    vertices.push_back(VertexFormat(center + glm::vec3(-wB, -h, d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(-wB, -h, -d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(wB, -h, -d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(wB, -h, d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(-wT, h, d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(-wT, h, -d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(wT, h, -d), color));
    vertices.push_back(VertexFormat(center + glm::vec3(wT, h, d), color));

    vector<unsigned int> idx = {
        0,3,2, 0,2,1, 4,5,6, 4,6,7, 0,4,7, 0,7,3,
        2,1,5, 2,5,6, 0,1,5, 0,5,4, 3,7,6, 3,6,2
    };
    for (unsigned int i : idx) indices.push_back(startIdx + i);
}

// primitive creation
Mesh* objects::CreateCylinder(const std::string& name, float height,
    float radius, int numSegments, glm::vec3 color) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddCylinderToMesh(v, i, glm::vec3(0), height, radius, numSegments, color);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateBox(const std::string& name, float length, float height,
    float width, glm::vec3 color) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddBoxToMesh(v, i, glm::vec3(0), length, height, width, color);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateTrapezoid(const std::string& name, float length,
    float height, float width, glm::vec3 color) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddTrapezoidToMesh(v, i, glm::vec3(0), length, length * 0.5f, height, width,
        color);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateSquare(const std::string& name, float size,
    glm::vec3 color) {
    float s = size / 2;
    vector<VertexFormat> v = {
        VertexFormat(glm::vec3(-s,0,-s), color),
        VertexFormat(glm::vec3(s,0,-s), color),
        VertexFormat(glm::vec3(s,0,s), color),
        VertexFormat(glm::vec3(-s,0,s), color)
    };
    vector<unsigned int> i = { 0,1,2, 3,0,2 };
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

// track creation
Mesh* objects::CreateTrackNormal(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    float len = MapSpecs::tileSize;
    AddBoxToMesh(v, i, glm::vec3(0, TrackSpecs::railThickness / 2,
        TrackSpecs::railSpacing / 2), len, TrackSpecs::railThickness,
        TrackSpecs::railWidth, TrackSpecs::colorRailNormal);
    AddBoxToMesh(v, i, glm::vec3(0, TrackSpecs::railThickness / 2,
        -TrackSpecs::railSpacing / 2), len, TrackSpecs::railThickness,
        TrackSpecs::railWidth, TrackSpecs::colorRailNormal);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateTrackBridge(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    int str = 4; float len = MapSpecs::tileSize;
    float sw = TrackSpecs::trackWidth / str;
    float z = -TrackSpecs::trackWidth / 2 + sw / 2;
    for (int k = 0; k < str; k++) {
        AddBoxToMesh(v, i, glm::vec3(0, TrackSpecs::railThickness / 2, z),
            len, TrackSpecs::railThickness, sw * 0.9f,
            (k % 2 == 0) ? TrackSpecs::colorBridge1 : TrackSpecs::colorBridge2);
        z += sw;
    }
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateTrackTunnel(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    float l = MapSpecs::tileSize; float tw = MapSpecs::tileSize;
    float th = MapSpecs::mountainHeight;
    float wW = (tw - TrackSpecs::tunnelClearanceWidth) / 5.0f;
    float wZ = (TrackSpecs::tunnelClearanceWidth / 2.0f) + (wW / 2.0f);
    AddBoxToMesh(v, i, glm::vec3(0, th / 2, wZ), l, th, wW,
        MapSpecs::colorMountain);
    AddBoxToMesh(v, i, glm::vec3(0, th / 2, -wZ), l, th, wW,
        MapSpecs::colorMountain);
    float rH = th - TrackSpecs::tunnelClearanceHeight;
    float rY = TrackSpecs::tunnelClearanceHeight + (rH / 2.0f);
    AddBoxToMesh(v, i, glm::vec3(0, rY, 0), l, rH,
        TrackSpecs::tunnelClearanceWidth, MapSpecs::colorMountain);
    float rW = 0.2f; float sp = 1.8f;
    AddBoxToMesh(v, i, glm::vec3(0, TrackSpecs::railThickness / 2, sp / 2),
        l, TrackSpecs::railThickness, rW, TrackSpecs::colorRailNormal);
    AddBoxToMesh(v, i, glm::vec3(0, TrackSpecs::railThickness / 2, -sp / 2),
        l, TrackSpecs::railThickness, rW, TrackSpecs::colorRailNormal);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

// station creation
Mesh* objects::CreateStationSimple(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    float pH = StationSpecs::platformHeight;
    AddBoxToMesh(v, i, glm::vec3(0, pH / 2, 0), StationSpecs::platformLength,
        pH, StationSpecs::platformWidth, StationSpecs::colorPlatform);
    float bH = StationSpecs::buildingHeight; float bW = StationSpecs::buildingWidth;
    float bY = pH + bH / 2;
    AddBoxToMesh(v, i, glm::vec3(0, bY, 0), StationSpecs::buildingLength, bH, bW,
        StationSpecs::colorBuilding1);
    float rH = StationSpecs::roofHeight; float rY = pH + bH + rH / 2;
    AddBoxToMesh(v, i, glm::vec3(0, rY, 0), StationSpecs::buildingLength + 1.0f,
        rH, StationSpecs::buildingWidth + 1.0f, StationSpecs::colorRoof1);
    float wZ = bW / 2 + 0.1f;
    AddBoxToMesh(v, i, glm::vec3(0, bY, wZ), 1.2f, 1.0f, 0.2f,
        StationSpecs::colorWindowFrame);
    AddBoxToMesh(v, i, glm::vec3(0, bY, wZ + 0.01f), 0.9f, 0.7f, 0.2f,
        StationSpecs::colorWindowGlass);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateStationCircle(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    float pH = StationSpecs::platformHeight;
    AddBoxToMesh(v, i, glm::vec3(0, pH / 2, 0), StationSpecs::platformLength,
        pH, StationSpecs::platformWidth, StationSpecs::colorPlatform);
    float bH = 3.0f; float bY = pH + bH / 2; float bR = 1.8f;
    AddCylinderToMesh(v, i, glm::vec3(0, bY, 0), bH, bR, 32,
        StationSpecs::colorBuilding2);
    AddCylinderToMesh(v, i, glm::vec3(0, pH + bH + 0.25f, 0), 0.5f, bR + 0.5f,
        32, StationSpecs::colorRoof2);
    float wZ = bR;
    AddBoxToMesh(v, i, glm::vec3(0, bY, wZ), 1.0f, 0.8f, 0.4f,
        StationSpecs::colorWindowFrame);
    AddBoxToMesh(v, i, glm::vec3(0, bY, wZ + 0.02f), 0.8f, 0.6f, 0.4f,
        StationSpecs::colorWindowGlass);
    Mesh* m = new Mesh(name); m->InitFromData(v, i); return m;
}

Mesh* objects::CreateStationPyramid(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    float pH = StationSpecs::platformHeight;
    AddBoxToMesh(v, i, glm::vec3(0, pH / 2, 0), StationSpecs::platformLength,
        pH, StationSpecs::platformWidth, StationSpecs::colorPlatform);
    float bH = 4.0f; float bY = pH + bH / 2; float bB = 3.5f;
    AddTrapezoidToMesh(v, i, glm::vec3(0, bY, 0), bB, 0, bH, bB,
        StationSpecs::colorBuilding3);
    AddBoxToMesh(v, i, glm::vec3(0, pH + bH, 0), 2.0f, 1.5f, bB + 0.2f,
        StationSpecs::colorRoof3);
    float wY = pH + bH / 2; float wZ = -1.1f;
    AddBoxToMesh(v, i, glm::vec3(0, wY, wZ - 0.5f), 0.8f, 0.8f, 0.4f,
        StationSpecs::colorWindowFrame);
    AddBoxToMesh(v, i, glm::vec3(0, wY, wZ - 0.6f), 0.6f, 0.6f, 0.4f,
        StationSpecs::colorWindowGlass);
    Mesh* m = new Mesh(name); 
    m->InitFromData(v, i); 
    return m;
}

// passenger creation
Mesh* objects::CreatePassengerSimple(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddBoxToMesh(v, i, glm::vec3(0, 0.6f, 0), 0.6f, 1.2f, 0.6f,
        PassengerSpecs::color);
    Mesh* m = new Mesh(name); 
    m->InitFromData(v, i); 
    return m;
}

Mesh* objects::CreatePassengerCircle(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddCylinderToMesh(v, i, glm::vec3(0, 0.6f, 0), 1.2f, 0.3f, 16,
        PassengerSpecs::color);
    Mesh* m = new Mesh(name);
    m->InitFromData(v, i); 
    return m;
}

Mesh* objects::CreatePassengerPyramid(const std::string& name) {
    vector<VertexFormat> v; vector<unsigned int> i;
    AddTrapezoidToMesh(v, i, glm::vec3(0, 0.6f, 0), 0.6f, 0, 1.2f, 0.6f,
        PassengerSpecs::color);
    Mesh* m = new Mesh(name); 
    m->InitFromData(v, i); 
    return m;
}