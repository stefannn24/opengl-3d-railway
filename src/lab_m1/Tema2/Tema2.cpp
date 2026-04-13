#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/objects.h"
#include "lab_m1/Tema2/minimap.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2() {}
Tema2::~Tema2() {}

void Tema2::Init() {
    camera = new tema2_camera::Camera();
    camera->Set(glm::vec3(0, 35, 40), glm::vec3(0, 0, 5), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio,
        0.01f, 200.0f);

    minimapCamera = new tema2_camera::Camera();
    minimapCamera->Set(glm::vec3(0, 80, 0), glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -1));
    float orthoSize = 80.0f;
    orthoProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize,
        orthoSize, 0.1f, 200.0f);

    // load meshes
    meshes["train_chassis"] = objects::CreateBox("train_chassis",
        objects::TrainSpecs::chassisLength, objects::TrainSpecs::chassisHeight,
        objects::TrainSpecs::chassisWidth, objects::TrainSpecs::colorChassis);
    meshes["train_cabin"] = objects::CreateBox("train_cabin",
        objects::TrainSpecs::cabinLength, objects::TrainSpecs::cabinHeight,
        objects::TrainSpecs::cabinWidth, objects::TrainSpecs::colorCabin);
    meshes["train_boiler"] = objects::CreateCylinder("train_boiler",
        objects::TrainSpecs::boilerLen, objects::TrainSpecs::boilerRadius, 32,
        objects::TrainSpecs::colorBoiler);
    meshes["train_chimney"] = objects::CreateCylinder("train_chimney",
        0.8f, 0.3f, 16, objects::TrainSpecs::colorDetail);
    meshes["train_wheel"] = objects::CreateCylinder("train_wheel",
        objects::TrainSpecs::wheelWidth, objects::TrainSpecs::wheelRadius, 32,
        objects::TrainSpecs::colorWheel);
    meshes["train_plow"] = objects::CreateTrapezoid("train_plow",
        1.6f, 0.5f, 0.5f, objects::TrainSpecs::colorCabin);
    meshes["wagon_body"] = objects::CreateBox("wagon_body",
        objects::WagonSpecs::bodyLength, objects::WagonSpecs::bodyHeight,
        objects::WagonSpecs::bodyWidth, objects::WagonSpecs::colorBody);
    meshes["train_coupling"] = objects::CreateBox("train_coupling",
        0.7f, 0.15f, 0.3f, glm::vec3(0.75f, 0.75f, 0.75f));

    meshes["terrain_grass"] = objects::CreateSquare("terrain_grass",
        objects::MapSpecs::tileSize, objects::MapSpecs::colorGrass);
    meshes["terrain_water"] = objects::CreateSquare("terrain_water",
        objects::MapSpecs::tileSize, objects::MapSpecs::colorWater);
    meshes["terrain_mountain"] = objects::CreateBox("terrain_mountain",
        objects::MapSpecs::tileSize, objects::MapSpecs::mountainHeight,
        objects::MapSpecs::tileSize, objects::MapSpecs::colorMountain);

    meshes["track_normal"] = objects::CreateTrackNormal("track_normal");
    meshes["track_bridge"] = objects::CreateTrackBridge("track_bridge");
    meshes["track_tunnel"] = objects::CreateTrackTunnel("track_tunnel");

    meshes["station_simple"] = objects::CreateStationSimple("station_simple");
    meshes["station_circle"] = objects::CreateStationCircle("station_circle");
    meshes["station_pyramid"] = objects::CreateStationPyramid("station_pyramid");

    meshes["passenger_simple"] = objects::CreatePassengerSimple(
        "passenger_simple");
    meshes["passenger_circle"] = objects::CreatePassengerCircle(
        "passenger_circle");
    meshes["passenger_pyramid"] = objects::CreatePassengerPyramid(
        "passenger_pyramid");

    minimap::CreateMinimapMeshes(meshes);

    wheelRotationAngle = 0;
    cameraLock = false;
    trainRunning = true;
    trainPosition = glm::vec3(0, 0, 0);
    cameraOffset = glm::vec3(-12.0f, 6.0f, 0.0f);
    distanceTraveled = 0.0f;
    renderingMinimap = false;

    // calculate initial position immediately so it doesn't drift at start
    glm::vec3 tPos; float tRot;
    GetPositionOnPath(0.0f, tPos, tRot);

    // apply rotation to offset
    glm::vec3 initialOffset = glm::vec3(glm::rotate(glm::mat4(1.0f), tRot, glm::vec3(0, 1, 0)) * glm::vec4(cameraOffset, 1.0f));
    camera->position = tPos + initialOffset;
}

void Tema2::FrameStart() {
    glClearColor(0.5f, 0.7f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);
}

void Tema2::Update(float deltaTimeSeconds) {
    if (trainRunning) wheelRotationAngle += deltaTimeSeconds * 4.0f;

    const int gridSize = 14;
    float cellSize = objects::MapSpecs::tileSize;
    float offset = (gridSize - 1) * cellSize / 2.0f;
    float yTrack = 0.05f + objects::TrackSpecs::railThickness;
    const int minIdx = 2; const int maxIdx = 11;

    if (trainRunning) {
        auto getPosDummy = [&](int i, int j) {
            return glm::vec3(i * cellSize - offset, yTrack,
                j * cellSize - offset);
            };

        glm::vec3 P0 = getPosDummy(minIdx, minIdx);
        glm::vec3 P1 = getPosDummy(maxIdx, minIdx);
        glm::vec3 P2 = getPosDummy(maxIdx, maxIdx);
        glm::vec3 P3 = getPosDummy(minIdx, maxIdx);
        
        float totalDist = glm::distance(P0, P1) + glm::distance(P1, P2) +
            glm::distance(P2, P3);

        if (distanceTraveled < totalDist) {
            distanceTraveled += deltaTimeSeconds * 8.0f;
        }
        else {
            distanceTraveled = totalDist;
        }
    }

    glm::vec3 tPos; float tRot;
    GetPositionOnPath(distanceTraveled, tPos, tRot);
    trainPosition = tPos;

    if (cameraLock) {
        // rotate the base offset vector by the train's current rotation angle around Y
        glm::vec3 rotatedOffset = glm::vec3(glm::rotate(glm::mat4(1.0f), tRot, glm::vec3(0, 1, 0)) * glm::vec4(cameraOffset, 1.0f));
        glm::vec3 targetPosition = trainPosition + rotatedOffset;

        float smoothSpeed = 3.0f; 
        
        // if distance is too big, snap to position immediately
        if (glm::distance(camera->position, targetPosition) > 50.0f) {
             camera->position = targetPosition;
        } else {
             camera->position = glm::mix(camera->position, targetPosition, deltaTimeSeconds * smoothSpeed);
        }
    }

    // render main scene
    DrawScene();
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);

    // render minimap
    RenderMinimap();
}

void Tema2::DrawScene() {
    const int gridSize = 14;
    float cellSize = objects::MapSpecs::tileSize;
    float offset = (gridSize - 1) * cellSize / 2.0f;

    const int minIdx = 2;
    const int maxIdx = 11;
    const int midIdx = (minIdx + maxIdx) / 2;
    const int bridgeStartI = 5;
    const int bridgeEndI = 7;
    const int waterStartZ = maxIdx - 2;
    const int waterEndZ = maxIdx + 2;
    float stOffset = cellSize * 0.55f;

    float yGround = 0.01f;
    float yTrack = 0.05f;
    float yWater = 0.03f;
    float yMountain = yGround + (objects::MapSpecs::mountainHeight / 2.0f);

    glm::mat4 modelMatrix;
    Shader* shader = shaders["VertexColor"];

    // map loop
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float x = i * cellSize - offset;
            float z = j * cellSize - offset;
            glm::vec3 pos(x, yGround, z);

            bool isWater = (j >= maxIdx && j <= maxIdx + 2 &&
                i >= bridgeStartI && i <= bridgeEndI) ||
                (i >= bridgeStartI && i <= bridgeEndI &&
                    j >= waterStartZ && j <= waterEndZ);

            if (isWater) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix,
                    glm::vec3(x, yWater, z));
                RenderMesh(meshes["terrain_water"], shader, modelMatrix);
            }
            else {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos);
                RenderMesh(meshes["terrain_grass"], shader, modelMatrix);
            }

            if (j == minIdx) {
                if (i == minIdx) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 0.0f);
                    float stationZ = z - stOffset;
                    RenderStation("simple", glm::vec3(x, yGround, stationZ),
                        0.0f);
                    RenderPassenger("simple", glm::vec3(x + 3.5f, yGround + 0.5f,
                        stationZ), 0.0f);
                }
                else if (i > minIdx && i < maxIdx - 1) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 0.0f);
                }
                else if (i == maxIdx - 1) {
                    RenderTrack("tunnel", glm::vec3(x, yGround, z), 0.0f);
                }
                else if (i == maxIdx) {
                    modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix,
                        glm::vec3(x, yMountain, z));
                    RenderMesh(meshes["terrain_mountain"], shader, modelMatrix);
                }
            }
            else if (i == maxIdx) {
                if (j == minIdx + 1) {
                    RenderTrack("tunnel", glm::vec3(x, yGround, z), 90.0f);
                }
                else if (j > minIdx + 1 && j < maxIdx - 1 && j != midIdx) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 90.0f);
                }
                else if (j == midIdx) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 90.0f);
                    float stationX = x + stOffset;
                    RenderStation("circle", glm::vec3(stationX, yGround, z),
                        270.0f);
                    RenderPassenger("circle", glm::vec3(stationX,
                        yGround + 0.5f, z + 3.5f), 0.0f);
                }
                else if (j == maxIdx - 1) {
                    RenderTrack("tunnel", glm::vec3(x, yGround, z), 90.0f);
                }
                else if (j == maxIdx) {
                    modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix,
                        glm::vec3(x, yMountain, z));
                    RenderMesh(meshes["terrain_mountain"], shader, modelMatrix);
                }
            }
            else if (j == maxIdx) {
                if (i == maxIdx - 1) {
                    RenderTrack("tunnel", glm::vec3(x, yGround, z), 0.0f);
                }
                else if (i > bridgeEndI && i < maxIdx - 1) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 0.0f);
                }
                else if (i >= bridgeStartI && i <= bridgeEndI) {
                    RenderTrack("bridge", glm::vec3(x, yTrack, z), 0.0f);
                }
                else if (i > minIdx && i < bridgeStartI) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 0.0f);
                }
                else if (i == minIdx) {
                    RenderTrack("normal", glm::vec3(x, yTrack, z), 0.0f);
                    float stationZ = z + stOffset;
                    RenderStation("pyramid", glm::vec3(x, yGround, stationZ),
                        0.0f);
                    RenderPassenger("pyramid", glm::vec3(x -3.5f, yGround + 0.5f,
                        stationZ), 0.0f);
                }
            }
        }
    }

    glm::vec3 tPos; float tRot;
    GetPositionOnPath(distanceTraveled, tPos, tRot);
    RenderTrain(tPos, tRot);

    float wagonDist = distanceTraveled - 5.5f;
    if (wagonDist >= 0) {
        glm::vec3 wPos; float wRot;
        GetPositionOnPath(wagonDist, wPos, wRot);
        RenderWagon(wPos, wRot);
    }
}

void Tema2::RenderMinimap() {
    glm::mat4 mainProj = projectionMatrix;
    tema2_camera::Camera* mainCam = camera;

    projectionMatrix = orthoProjection;
    camera = minimapCamera;

    glm::ivec2 resolution = window->GetResolution();
    int miniSize = resolution.y / 3;
    glViewport(resolution.x - miniSize - 20, resolution.y - miniSize - 20,
        miniSize, miniSize);

    glClear(GL_DEPTH_BUFFER_BIT);

    renderingMinimap = true;
    DrawScene();
    renderingMinimap = false;

    camera = mainCam;
    projectionMatrix = mainProj;
}

// modified render track
void Tema2::RenderTrack(const std::string& type, glm::vec3 position, float rotationY) {
    if (renderingMinimap) {
        if (type == "normal" || type == "bridge") {
            Shader* shader = shaders["VertexColor"];
            glm::mat4 m = glm::mat4(1);
            m = glm::translate(m, position + glm::vec3(0, 1.0f, 0));
            m = glm::rotate(m, RADIANS(rotationY), glm::vec3(0, 1, 0));

            m = glm::scale(m, glm::vec3(objects::MapSpecs::tileSize, 1.0f, 2.5f));

            if (type == "bridge") {
                RenderMesh(meshes["map_bridge"], shader, m);
            }
            else {
                RenderMesh(meshes["map_track"], shader, m);
            }
            return;
        }
    }

    std::string n = "track_" + type;
    if (meshes.find(n) == meshes.end()) return;
    glm::mat4 m = glm::translate(glm::mat4(1), position);
    m = glm::rotate(m, RADIANS(rotationY), glm::vec3(0, 1, 0));
    RenderMesh(meshes[n], shaders["VertexColor"], m);
}

void Tema2::GetPositionOnPath(float dist, glm::vec3& outPos, float& outRotY) {
    float yTrack = 0.05f + objects::TrackSpecs::railThickness;
    float cellSize = objects::MapSpecs::tileSize;
    float offset = (14 - 1) * cellSize / 2.0f;
    auto getPos = [&](int i, int j) {
        return glm::vec3(i * cellSize - offset, yTrack, j * cellSize - offset);
        };

    glm::vec3 P0 = getPos(2, 2); 
    glm::vec3 P1 = getPos(11, 2);
    glm::vec3 P2 = getPos(11, 11); 
    glm::vec3 P3 = getPos(2, 11);

    float d1 = glm::distance(P0, P1); 
    float d2 = glm::distance(P1, P2);

    if (dist <= d1) {
        float t = dist / d1; 
        outPos = glm::mix(P0, P1, t); 
        outRotY = 0.0f;
    }
    else if (dist <= d1 + d2) {
        float t = (dist - d1) / d2;
        outPos = glm::mix(P1, P2, t);
        outRotY = -1.5708f;
    }
    else {
        float distSoFar = d1 + d2; 
        float d3 = glm::distance(P2, P3);
        float t = (dist - distSoFar) / d3;
        if (t > 1.0f) t = 1.0f;
        outPos = glm::mix(P2, P3, t); 
        outRotY = -3.14159f;
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_SPACE) 
        cameraLock = !cameraLock;
    if (key == GLFW_KEY_R) { 
        distanceTraveled = 0.0f; 
        trainRunning = true; 
    }
    if (key == GLFW_KEY_P) 
        trainRunning = !trainRunning;
}

// rendering helpers
void Tema2::RenderStation(const std::string& type, glm::vec3 position,
    float rotationY) 
{
    std::string n = "station_" + type;
    
    if (meshes.find(n) == meshes.end())
        return;
    
    glm::mat4 m = glm::translate(glm::mat4(1), position);
    m = glm::rotate(m, RADIANS(rotationY), glm::vec3(0, 1, 0));
    
    RenderMesh(meshes[n], shaders["VertexColor"], m);
}
void Tema2::RenderPassenger(const std::string& type, glm::vec3 position,
    float rotationY) 
{
    std::string n = "passenger_" + type;

    if (meshes.find(n) == meshes.end())
        return;
    
    glm::mat4 m = glm::translate(glm::mat4(1), position);
    m = glm::rotate(m, RADIANS(rotationY), glm::vec3(0, 1, 0));
    
    RenderMesh(meshes[n], shaders["VertexColor"], m);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader,
    const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->program) 
        return;

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
        glm::value_ptr(camera->GetViewMatrix()));

    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
        glm::value_ptr(projectionMatrix));

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
        glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderTrain(glm::vec3 position, float angleY) {
    glm::mat4 base = glm::translate(glm::mat4(1), position);
    base = glm::rotate(base, angleY, glm::vec3(0, 1, 0));

    Shader* s = shaders["VertexColor"];
    
    float chassisY = objects::TrainSpecs::wheelRadius +
        (objects::TrainSpecs::chassisHeight / 2.0f);
    
    glm::mat4 m = base;
    m = glm::translate(m, glm::vec3(0, chassisY, 0));
    
    RenderMesh(meshes["train_chassis"], s, m);
    
    float cX = -(objects::TrainSpecs::chassisLength / 2) +
        (objects::TrainSpecs::cabinLength / 2);
    
    float cY = chassisY + objects::TrainSpecs::chassisHeight / 2 +
        objects::TrainSpecs::cabinHeight / 2;
    
    m = base; m = glm::translate(m, glm::vec3(cX, cY, 0));
    
    RenderMesh(meshes["train_cabin"], s, m);
    
    float bX = -(objects::TrainSpecs::chassisLength / 2) +
        objects::TrainSpecs::cabinLength + objects::TrainSpecs::boilerLen / 2;
    
    float bY = chassisY + objects::TrainSpecs::chassisHeight / 2 +
        objects::TrainSpecs::boilerRadius;
    
    m = base; m = glm::translate(m, glm::vec3(bX, bY, 0));
    m = glm::rotate(m, RADIANS(90), glm::vec3(0, 0, 1));
    
    RenderMesh(meshes["train_boiler"], s, m);
    
    float chX = bX + objects::TrainSpecs::chimneyOffsetFront;
    
    float chY = chassisY + objects::TrainSpecs::chassisHeight / 2 +
        objects::TrainSpecs::boilerRadius * 2;
    
    m = base; m = glm::translate(m, glm::vec3(chX, chY, 0));
    
    RenderMesh(meshes["train_chimney"], s, m);
    
    float fX = (objects::TrainSpecs::chassisLength / 2) + 0.1f;
    
    m = base; 
    m = glm::translate(m, glm::vec3(fX, chassisY, 0));
    m = glm::rotate(m, RADIANS(-90), glm::vec3(0, 1, 0));
    m = glm::scale(m, glm::vec3(1, 0.5f, 0.5f));
    
    RenderMesh(meshes["train_plow"], s, m);
    
    float stickX = -(objects::TrainSpecs::chassisLength / 2.0f) - 0.35f + 0.1f;
    
    m = base; 
    m = glm::translate(m, glm::vec3(stickX, chassisY, 0));
    
    RenderMesh(meshes["train_coupling"], s, m);
    
    int wC = 5;
    float zO = objects::TrainSpecs::chassisWidth / 2 +
        objects::TrainSpecs::wheelWidth / 2;
    float sX = -(objects::TrainSpecs::chassisLength / 2) +
        objects::TrainSpecs::wheelSpacingX;
    float stepX = ((objects::TrainSpecs::chassisLength / 2) -
        objects::TrainSpecs::wheelSpacingX - sX) / (wC - 1);
    
    for (int i = 0; i < wC; i++) {
        float curX = sX + i * stepX;
        m = base; 
        m = glm::translate(m, glm::vec3(curX,
            objects::TrainSpecs::wheelRadius, zO));
        
        m = glm::rotate(m, RADIANS(90), glm::vec3(1, 0, 0));
        m = glm::rotate(m, wheelRotationAngle, glm::vec3(0, 1, 0));
        
        RenderMesh(meshes["train_wheel"], s, m);
        
        m = base; 
        m = glm::translate(m, glm::vec3(curX,
            objects::TrainSpecs::wheelRadius, -zO));
        m = glm::rotate(m, RADIANS(90), glm::vec3(1, 0, 0));
        m = glm::rotate(m, wheelRotationAngle, glm::vec3(0, 1, 0));
        
        RenderMesh(meshes["train_wheel"], s, m);
    }
}

void Tema2::RenderWagon(glm::vec3 position, float angleY) {
    glm::mat4 base = glm::translate(glm::mat4(1), position);
    base = glm::rotate(base, angleY, glm::vec3(0, 1, 0));
    Shader* s = shaders["VertexColor"];
    
    float chassisY = objects::TrainSpecs::wheelRadius +
        (objects::TrainSpecs::chassisHeight / 2.0f);
    
    glm::mat4 m;
    m = base; m = glm::translate(m, glm::vec3(0, chassisY, 0));
    
    RenderMesh(meshes["train_chassis"], s, m);
    
    float bodyY = chassisY + (objects::TrainSpecs::chassisHeight / 2.0f) +
        (objects::WagonSpecs::bodyHeight / 2.0f);
    
    m = base;
    m = glm::translate(m, glm::vec3(0, bodyY, 0));
    
    RenderMesh(meshes["wagon_body"], s, m);
    
    int wC = 3;
    
    float zO = objects::TrainSpecs::chassisWidth / 2 +
        objects::TrainSpecs::wheelWidth / 2;
    
    float sX = -(objects::TrainSpecs::chassisLength / 2) +
        objects::TrainSpecs::wheelSpacingX;
    
    float stepX = ((objects::TrainSpecs::chassisLength / 2) -
        objects::TrainSpecs::wheelSpacingX - sX) / (wC - 1);
    
    for (int i = 0; i < wC; i++) {
        float curX = sX + i * stepX;
        m = base;
        
        m = glm::translate(m, glm::vec3(curX,
            objects::TrainSpecs::wheelRadius, zO));
        
        m = glm::rotate(m, RADIANS(90), glm::vec3(1, 0, 0));
        
        m = glm::rotate(m, wheelRotationAngle, glm::vec3(0, 1, 0));
        
        RenderMesh(meshes["train_wheel"], s, m);
        
        m = base; m = glm::translate(m, glm::vec3(curX,
            objects::TrainSpecs::wheelRadius, -zO));
        
        m = glm::rotate(m, RADIANS(90), glm::vec3(1, 0, 0));
        m = glm::rotate(m, wheelRotationAngle, glm::vec3(0, 1, 0));
        
        RenderMesh(meshes["train_wheel"], s, m);
    }
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (cameraLock) return;
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float speed = 20.0f * deltaTime;
        if (window->KeyHold(GLFW_KEY_W)) camera->MoveForward(speed);
        if (window->KeyHold(GLFW_KEY_S)) camera->MoveForward(-speed);
        if (window->KeyHold(GLFW_KEY_A)) camera->TranslateRight(-speed);
        if (window->KeyHold(GLFW_KEY_D)) camera->TranslateRight(speed);
        if (window->KeyHold(GLFW_KEY_Q)) camera->TranslateUpward(-speed);
        if (window->KeyHold(GLFW_KEY_E)) camera->TranslateUpward(speed);
    }
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sens = 0.001f;
        camera->RotateFirstPerson_OX(-deltaY * sens);
        camera->RotateFirstPerson_OY(-deltaX * sens);
    }
}

void Tema2::OnKeyRelease(int key, int mods) {}
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Tema2::OnWindowResize(int width, int height) {}
void Tema2::FrameEnd() {}