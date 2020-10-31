//
// Created by Edge on 2020/7/8.
//

#include <string>
#include <fstream>
#include <sstream>
#include <Dielectric.h>
#include <BVH.h>
#include "Config.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "HittableList.h"

using namespace std;

void Config::loadConfig(const string &configFilename, Scene *scene) {
    ifstream ifs;
    ifs.open(configFilename);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file " << configFilename << std::endl;
        exit(1);
    }
    string line;

    scene->m_hittableList = new HittableList();

    Coord eyeCoord;
    int screenWidth, screenHeight;
    Velocity direction, up;
    double fov;
    Material *boundMaterial = nullptr;

    while (getline(ifs, line)) {
        stringstream ss(line);
        string cmd;
        ss >> cmd;
        if (cmd == "E") {
            ss >> eyeCoord;
        } else if (cmd == "V") {
            // Viewing Direction
            ss >> direction >> up;
            direction = direction.normalize();
            up = up.normalize();
        } else if (cmd == "F") {
            ss >> fov;
        } else if (cmd == "R") {
            ss >> screenWidth >> screenHeight;
        } else if (cmd == "S") {
            if(!boundMaterial) {
                cerr << "No valid material bound in this object" << endl;
                exit(1);
            }
            Coord coord;
            double radius;
            ss >> coord;
            ss >> radius;
            Hittable *sphere = new Sphere(coord, radius);
            sphere->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(sphere);
        } else if (cmd == "T") {
            if(!boundMaterial) {
                cerr << "No valid material bound in this object" << endl;
                exit(1);
            }
            Coord coord[3];
            for (int i = 0; i < 3; ++i) {
                ss >> coord[i];
            }
            Hittable *triangle = new Triangle(&coord);
            triangle->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(triangle);
        } else if (cmd == "OBJ") {
            if(!boundMaterial) {
                cerr << "No valid material bound in this object" << endl;
                exit(1);
            }
            string objPath;
            ss >> objPath;
            Hittable *polygonMesh = new PolygonMesh(objPath);
            polygonMesh->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(polygonMesh);
        } else if (cmd == "OBJBV") {
            if(!boundMaterial) {
                cerr << "No valid material bound in this object" << endl;
                exit(1);
            }
            string objPath;
            ss >> objPath;
            Hittable *polygonMeshBV = new PolygonMeshBV(objPath);
            polygonMeshBV->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(polygonMeshBV);
        } else if (cmd == "OBJBVH") {
            if(!boundMaterial) {
                cerr << "No valid material bound in this object" << endl;
                exit(1);
            }
            string objPath;
            ss >> objPath;
            Hittable *polygonMeshBVH = new PolygonMeshBVH(objPath, boundMaterial);
            scene->m_hittableList->addHittable(polygonMeshBVH);
        } else if (cmd == "MDF") {
            boundMaterial = new Material();
            ss >> *boundMaterial;
        } else if (cmd == "MDI") {
            DielectricMaterial *material = new DielectricMaterial();
            ss >> *material;
            boundMaterial = material;
        } else if(cmd == "UBM") {
            boundMaterial = nullptr;
        } else if(cmd == "L") {
            Coord lightOrigin;
            ss >> lightOrigin;
            scene->m_lightList.push_back(new Light(lightOrigin));
        } else if(cmd == "AL") {
            Coord planePoint[4];
            for(int i = 0;i < 4;++i) {
                ss >> planePoint[i];
            }
            AreaLight *areaLight = new AreaLight(&planePoint);
            ss >> *areaLight;
            scene->m_areaLightList.push_back(areaLight);
            boundMaterial = areaLight;
        }
    }
    scene->m_camera = new Camera(screenWidth, screenHeight, eyeCoord, fov, direction, up);
}