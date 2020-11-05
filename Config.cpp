//
// Created by Edge on 2020/7/8.
//

#include <string>
#include <fstream>
#include <sstream>
#include <Dielectric.h>
#include <BVH.h>
#include <Lambertian.h>
#include <IllumModelFactory.h>
#include "Config.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "HittableList.h"
#include "AreaLight.h"

using namespace std;

Scene *Config::loadConfig(const string &configFilename) {
    ifstream ifs;
    ifs.open(configFilename);
    if (!ifs.is_open()) {
        std::cerr << "[Error] Failed to open file: " << configFilename << std::endl;
        exit(1);
    }
    string line;

    Scene *scene = new Scene();

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
        if (cmd == "MODEL") {
            string modelName;
            ss >> modelName;
            scene->setModel(IlluminationModelFactory::createModel(modelName));
        }else if (cmd == "E") {
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
            string objPath;
            ss >> objPath;
            Hittable *polygonMesh = new PolygonMesh(objPath);
            polygonMesh->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(polygonMesh);
        } else if (cmd == "OBJBV") {
            string objPath;
            ss >> objPath;
            Hittable *polygonMeshBV = new PolygonMeshBV(objPath);
            polygonMeshBV->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(polygonMeshBV);
        } else if (cmd == "OBJBVH") {
            string objPath;
            ss >> objPath;
            Hittable *polygonMeshBVH = new PolygonMeshBVH(objPath, boundMaterial);
            scene->m_hittableList->addHittable(polygonMeshBVH);
        } else if (cmd == "MDF") {
            boundMaterial = new LambertianMaterial();
            ss >> *(LambertianMaterial *)boundMaterial;
        } else if (cmd == "MDI") {
            boundMaterial = new DielectricMaterial();
            ss >> *(DielectricMaterial *)boundMaterial;
        } else if(cmd == "UBM") {
            boundMaterial = nullptr;
        } else if(cmd == "L") {
            Coord lightOrigin;
            ss >> lightOrigin;
            scene->m_lightList.push_back(new Light(lightOrigin));
        } else if(cmd == "AL") {
            AreaLight *areaLight = new AreaLight();
            ss >> *areaLight;
            scene->m_areaLightList.push_back(areaLight);
            boundMaterial = areaLight;

            Coord coordList[3] = {areaLight->m_point,areaLight->m_point + areaLight->m_uDirection, areaLight->m_point + areaLight->m_uDirection + areaLight->m_vDirection};
            Hittable *triangle = new Triangle(&coordList);
            triangle->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(triangle);

            coordList[1] = areaLight->m_point + areaLight->m_vDirection;
            triangle = new Triangle(&coordList);
            triangle->setMaterial(boundMaterial);
            scene->m_hittableList->addHittable(triangle);
        }
    }
    scene->m_camera = new Camera(screenWidth, screenHeight, eyeCoord, fov, direction, up);
    return scene;
}