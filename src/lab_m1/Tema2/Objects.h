#pragma once

#include <string>
#include <vector>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace objects
{
    // train structure attributes
    namespace TrainSpecs
    {
        // wheels
        const float wheelRadius = 0.3f;
        const float wheelWidth = 0.18f;
        const int   wheelSegments = 32;
        const glm::vec3 colorWheel = glm::vec3(0.341f, 0.278f, 0.114f);

        // chassis
        const float chassisLength = 5.0f;
        const float chassisHeight = 0.25f;
        const float chassisWidth = 2.1f;
        const glm::vec3 colorChassis = glm::vec3(0.1f, 0.1f, 0.1f);

        // engine
        const float boilerLen = 3.0f;
        const float boilerRadius = 0.8f;
        const int   boilerSegments = 32;
        const glm::vec3 colorBoiler = glm::vec3(0.2f, 0.2f, 0.2f);

        // cabin
        const float cabinLength = 1.5f;
        const float cabinHeight = 2.2f;
        const float cabinWidth = 1.9f;
        const glm::vec3 colorCabin = glm::vec3(0.15f, 0.15f, 0.15f);

        // detail
        const float detailLength = 0.2f;
        const float detailHeight = 0.4f;
        const float detailWidth = 0.6f;
        const glm::vec3 colorDetail = glm::vec3(0.6f, 0.6f, 0.6f);

        // chimeny and
        const float chimneyHeight = 0.8f;
        const float chimneyRadius = 0.3f;
        const float chimneyOffsetFront = 0.8f;
        const float plowLength = 1.6f;
        const float plowHeight = 0.5f;
        const float plowWidth = 0.5f;

        const float wheelOffsetZ = 0.1f;
        const float wheelSpacingX = 0.6f;
    }
    
	// waggon structure attributes
    namespace WagonSpecs
    {
        const float bodyLength = 4.5f;
        const float bodyHeight = 2.2f;
        const float bodyWidth = 1.9f;
        const glm::vec3 colorBody = TrainSpecs::colorCabin;
    }

	// map structure attributes
    namespace MapSpecs
    {
        const float tileSize = 10.0f;
        const float mountainHeight = 5.0f;

        const glm::vec3 colorGrass = glm::vec3(0.3f, 0.7f, 0.2f);
        const glm::vec3 colorWater = glm::vec3(0.2f, 0.5f, 0.9f);
        const glm::vec3 colorMountain = glm::vec3(0.5f, 0.3f, 0.1f);
    }

	// track structure attributes
    namespace TrackSpecs
    {
        const float trackWidth = 2.5f;
        const float railThickness = 0.2f;
        const float railWidth = 0.2f;
        const float railSpacing = 1.8f;

        const glm::vec3 colorRailNormal = glm::vec3(0.0f, 0.0f, 0.0f);
        const glm::vec3 colorBridge1 = glm::vec3(0.6f, 0.4f, 0.2f);
        const glm::vec3 colorBridge2 = glm::vec3(0.4f, 0.25f, 0.1f);
        const glm::vec3 colorTunnel1 = glm::vec3(0.5f, 0.5f, 0.5f);
        const glm::vec3 colorTunnel2 = glm::vec3(0.3f, 0.3f, 0.3f);

        const float tunnelClearanceWidth = 3.5f;
        const float tunnelClearanceHeight = 3.5f;
    }

	// station structure attributes
    namespace StationSpecs
    {
        const float platformLength = 12.0f;
        const float platformHeight = 0.5f;
        const float platformWidth = 4.0f;

        const glm::vec3 colorPlatform = glm::vec3(0.6f, 0.6f, 0.6f);
        const glm::vec3 colorBuilding1 = glm::vec3(0.7f, 0.3f, 0.3f);
        const glm::vec3 colorRoof1 = glm::vec3(0.2f, 0.2f, 0.2f);
        const glm::vec3 colorBuilding2 = glm::vec3(0.2f, 0.5f, 0.8f);
        const glm::vec3 colorRoof2 = glm::vec3(0.1f, 0.1f, 0.3f);
        const glm::vec3 colorBuilding3 = glm::vec3(0.8f, 0.7f, 0.2f);
        const glm::vec3 colorRoof3 = glm::vec3(0.4f, 0.3f, 0.1f);
        const glm::vec3 colorWindowGlass = glm::vec3(0.4f, 0.7f, 0.95f);
        const glm::vec3 colorWindowFrame = glm::vec3(0.85f, 0.85f, 0.85f);

        const float buildingLength = 4.0f;
        const float buildingHeight = 3.0f;
        const float buildingWidth = 2.5f;
        const float buildingRadius = 1.8f;
        const float buildingBase = 3.5f;
        const float buildingHeightPyramid = 4.0f;
        const float roofHeight = 0.5f;
        const float roofOverhang = 0.5f;
        const float doorWidth = 1.0f;
        const float doorHeight = 1.5f;
        const float doorDepth = 0.2f;
        const float windowWidth = 1.0f;
        const float windowHeight = 1.0f;
        const float windowDepth = 0.2f;
        const float windowFrameThick = 0.15f;
    }

	// passenger structure attributes
    namespace PassengerSpecs
    {
        const float height = 1.2f;
        const float width = 0.6f;
        const glm::vec3 color = glm::vec3(0.2f, 0.8f, 0.2f);
    }

    // functions for the creation of primitices
    Mesh* CreateCylinder(const std::string& name, float height, float radius, int numSegments, glm::vec3 color);
    Mesh* CreateBox(const std::string& name, float length, float height, float width, glm::vec3 color);
    Mesh* CreateTrapezoid(const std::string& name, float length, float height, float width, glm::vec3 color);
    Mesh* CreateSquare(const std::string& name, float size, glm::vec3 color);

    // functions to create the meshes for the tracks
    Mesh* CreateTrackNormal(const std::string& name);
    Mesh* CreateTrackBridge(const std::string& name);
    Mesh* CreateTrackTunnel(const std::string& name);
    Mesh* CreateTrackCurve(const std::string& name);

	// functions to create the meshes for the stations
    Mesh* CreateStationSimple(const std::string& name);
    Mesh* CreateStationCircle(const std::string& name);
    Mesh* CreateStationPyramid(const std::string& name);

	// functions to create the meshes for the passengers
    Mesh* CreatePassengerSimple(const std::string& name);
    Mesh* CreatePassengerCircle(const std::string& name);
    Mesh* CreatePassengerPyramid(const std::string& name);
}