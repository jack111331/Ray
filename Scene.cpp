//
// Created by Edge on 2020/7/8.
//

#include <Photon.h>
#include <Dielectric.h>
#include "Scene.h"
#include "Material.h"
#include "HittableList.h"
#include "IllumModel.h"
#include <pthread.h>
#include <functional>
#include <Lambertian.h>
#include "AreaLight.h"
#include <map>

bool Scene::readSceneInfo(const YAML::Node &node) {
    m_hittableList = new HittableList();

    if (node["light"]) {
        for (YAML::const_iterator it = node["light"].begin(); it != node["light"].end(); ++it) {
            if (it->second["type"].as<std::string>() == "point-light") {
                Light *light = new Light();
                light->readLightInfo(it->second);
                m_lightList.push_back(light);
            } else if (it->second["type"].as<std::string>() == "area-light") {
                AreaLight *light = new AreaLight();
                light->readLightInfo(it->second);
                m_lightList.push_back(light);
            }
        }
    }
    if (node["material"]) {
        for (YAML::const_iterator it = node["material"].begin(); it != node["material"].end(); ++it) {
            if (it->second["type"].as<std::string>() == "lambertian") {
                LambertianMaterial *material = new LambertianMaterial();
                material->readMaterialInfo(it->second);
                m_materialTable[it->first.as<std::string>()] = material;
            } else if (it->second["type"].as<std::string>() == "dielectric") {
                DielectricMaterial *material = new DielectricMaterial();
                material->readMaterialInfo(it->second);
                m_materialTable[it->first.as<std::string>()] = material;
            }
        }
    }
    else if (cmd == "T") {
        if (!boundMaterial) {
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
        ss >> *(LambertianMaterial *) boundMaterial;
    } else if (cmd == "MDI") {
        boundMaterial = new DielectricMaterial();
        ss >> *(DielectricMaterial *) boundMaterial;
    } else if (cmd == "UBM") {
        boundMaterial = nullptr;
    } else if (cmd == "L") {
        Coord lightOrigin;
        ss >> lightOrigin;
        scene->m_lightList.push_back(new Light(lightOrigin));
    } else if (cmd == "AL") {
        AreaLight *areaLight = new AreaLight();
        ss >> *areaLight;
        scene->m_lightList.push_back(areaLight);
        boundMaterial = areaLight;

        Coord coordList[3] = {areaLight->m_origin, areaLight->m_origin + areaLight->m_uDirection,
                              areaLight->m_origin + areaLight->m_uDirection + areaLight->m_vDirection};
        Hittable *triangle = new Triangle(&coordList);
        triangle->setMaterial(boundMaterial);
        scene->m_hittableList->addHittable(triangle);

        coordList[1] = areaLight->m_origin + areaLight->m_vDirection;
        triangle = new Triangle(&coordList);
        triangle->setMaterial(boundMaterial);
        scene->m_hittableList->addHittable(triangle);
    }
}

Color Scene::castRay(Ray &ray) {
    if (m_model) {
        return m_model->castRay(this, ray);
    } else {
        return {.0, .0, .0};
    }
}

void *Scene::castJitteredRay(void *castRayThreadArgs) {
//    srand(0);
    CastRayThreadArgs *args = (CastRayThreadArgs *) castRayThreadArgs;
    for (int k = 0; k < args->sampleAmount; ++k) {
        Ray perturbatedRay = {args->ray.origin,
                              args->ray.velocity + Util::randomInUnit() * args->unitHorizontalScreen +
                              Util::randomInUnit() * args->unitVerticalScreen};
        args->scene->m_camera->m_screen[args->i][args->j] += args->scene->castRay(perturbatedRay);
    }
    args->scene->m_camera->m_screen[args->i][args->j] /= (float) args->sampleAmount;
    args->scene->m_camera->m_screen[args->i][args->j].clamp();
    return nullptr;
}

void Scene::displayScene() {
    if (!m_camera) {
        return;
    }
    // Upper left start
    // Lower right end
    Velocity left = m_camera->m_direction.cross(m_camera->m_up);
    const double PI = acos(-1);
    Coord center = m_camera->m_eyeCoord + m_camera->m_direction.normalize();
    double widthFactor = tan(m_camera->m_fov / 180.0 * PI);
    double heightFactor = widthFactor * m_camera->m_height / m_camera->m_width;
    Coord leftUpper = center + widthFactor * left.normalize() + heightFactor * m_camera->m_up.normalize();
    Velocity unitHorizontalScreen = 1 / (float) (m_camera->m_width - 1) * 2.0 * widthFactor * left.normalize();
    Velocity unitVerticalScreen =
            1 / (float) (m_camera->m_height - 1) * 2.0 * heightFactor * m_camera->m_up.normalize();
    const int sampleAmount = 2;//30
    std::vector<ThreadInfo> threadList;
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            m_camera->m_screen[i][j] = Color{0.0f, 0.0f, 0.0f};
            Coord currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            pthread_t threadCastRay;
            CastRayThreadArgs *args = new CastRayThreadArgs;
            args->scene = this;
            args->i = i;
            args->j = j;
            args->ray = ray;
            args->sampleAmount = sampleAmount;
            args->unitVerticalScreen = unitVerticalScreen;
            args->unitHorizontalScreen = unitHorizontalScreen;
            castJitteredRay(args);
            delete args;
//            pthread_create(&threadCastRay, nullptr, &Scene::castJitteredRay, (void *) args);
//            threadList.push_back({threadCastRay, args});
//            std::cout << "[" << i << ", " << j << "] Rendering" << std::endl;
//            fflush(stdout);
//            if (threadList.size() >= 40) {
//                std::cout << "batch Rendering" << std::endl;
//                fflush(stdout);
//                for (auto thread : threadList) {
//                    pthread_join(thread.id, nullptr);
//                    delete thread.castRayThreadArgs;
//                }
//                threadList.clear();
//            }
        }
    }
}

bool Scene::photonTracing(Ray &ray, float power, int depth) {
    if (power < 1e-6 || depth > 16) {
        return true;
    }
    // hit photon and store photon
    HitRecord record;
    if (m_hittableList->isHit(0.001, ray, record)) {
        // calculate refractivity and reflectivity
        float reflectivity = 0.0, refractivity = 0.0;
        Velocity refractedDirection;
        if (record.material->getType() == Material::MaterialType::DIELECTRIC) {
            DielectricMaterial *dielectricMaterial = (DielectricMaterial *) record.material;
            Velocity outwardNormal;
            float cosine;
            float niOverNt;
            if (ray.velocity.dot(record.normal) > 0.0) {
                outwardNormal = -record.normal;
                niOverNt = dielectricMaterial->m_constantReferenceIndex;
                cosine = dielectricMaterial->m_constantReferenceIndex * ray.velocity.dot(record.normal) /
                         ray.velocity.length();
            } else {
                outwardNormal = record.normal;
                niOverNt = 1.0 / dielectricMaterial->m_constantReferenceIndex;
                cosine = -ray.velocity.dot(record.normal) / ray.velocity.length();
            }
            reflectivity = Util::schlickApprox(cosine, dielectricMaterial->m_constantReferenceIndex);
            if (!ray.velocity.refract(outwardNormal, niOverNt, refractedDirection)) {
                reflectivity = 1.0;
            }
            refractivity = 1.0 - reflectivity;
        } else if (record.material->getType() == Material::MaterialType::LAMBERTIAN) {
            PhotonMappingModel *model = (PhotonMappingModel *) m_model;
            Photon photon(
                    record.point,
                    power,
                    ray.velocity,
                    Photon::Flag::NORMAL
            );
            model->m_kdTree->insert(photon);
        } else if (record.material->getType() == Material::MaterialType::AREALIGHT) {
            PhotonMappingModel *model = (PhotonMappingModel *) m_model;
            Photon photon(
                    record.point,
                    power,
                    ray.velocity,
                    Photon::Flag::NORMAL
            );
            model->m_kdTree->insert(photon);
        }
        EmitType emitType = Util::russianRoulette(reflectivity, refractivity);
        if (emitType == EmitType::ABSORBED) {
            // reflected or transmitted ray
            // influenced power
        } else if (emitType == EmitType::REFLECTED) {
            Velocity reflectedDirection = ray.velocity.reflect(record.normal);
            ray = {record.point, reflectedDirection};
            photonTracing(ray, power, depth + 1);
        } else if (emitType == EmitType::TRANSMITTED) {
            ray = {record.point, refractedDirection};
            photonTracing(ray, power, depth + 1);
        }
        return true;
    }
    return false;
}

void Scene::displayPhotonMappingScene() {
    if (!m_camera) {
        return;
    }
    // Upper left start
    // Lower right end
    Velocity left = m_camera->m_direction.cross(m_camera->m_up);
    const double PI = acos(-1);
    Coord center = m_camera->m_eyeCoord + m_camera->m_direction.normalize();
    double widthFactor = tan(m_camera->m_fov / 180.0 * PI);
    double heightFactor = widthFactor * m_camera->m_height / m_camera->m_width;
    Coord leftUpper = center + widthFactor * left.normalize() + heightFactor * m_camera->m_up.normalize();
    Velocity unitHorizontalScreen = 1 / (float) (m_camera->m_width - 1) * 2.0 * widthFactor * left.normalize();
    Velocity unitVerticalScreen =
            1 / (float) (m_camera->m_height - 1) * 2.0 * heightFactor * m_camera->m_up.normalize();
    const int sampleAmount = 1;//30
    // first pass: photon tracing
    const float photonPower = .2;// watt
    const float photonAmount = 500;
    if (m_lightList.empty()) {
        std::cerr << "No light" << std::endl;
        exit(1);
    }
    photonGenerating(photonPower, photonAmount);
    std::cout << "Photon traced" << std::endl;
    // second pass: ray tracing photon
    std::vector<ThreadInfo> threadList;
    srand(time(0));
    for (int i = 0; i < m_camera->m_height; ++i) {
        for (int j = 0; j < m_camera->m_width; ++j) {
            m_camera->m_screen[i][j] = Color{0.0f, 0.0f, 0.0f};
            Coord currentRayOnScreen = leftUpper - (float) j * unitHorizontalScreen - (float) i * unitVerticalScreen;
            Ray ray = {
                    m_camera->m_eyeCoord,
                    currentRayOnScreen - m_camera->m_eyeCoord
            };
            pthread_t threadCastRay;
            CastRayThreadArgs *args = new CastRayThreadArgs;
            args->scene = this;
            args->i = i;
            args->j = j;
            args->ray = ray;
            args->sampleAmount = sampleAmount;
            args->unitVerticalScreen = unitVerticalScreen;
            args->unitHorizontalScreen = unitHorizontalScreen;
            castJitteredRay(args);
            delete args;
//            pthread_create(&threadCastRay, nullptr, &Scene::castJitteredRay, args);
//            threadList.push_back({threadCastRay, args});
//            if(threadList.size() >= 40) {
//                for (auto thread : threadList) {
//                    int err;
//                    void *retval;
//                    if((err = pthread_join(thread.id, &retval))) {
//                        std::cout << err << std::endl;
//                    }
//                    delete thread.castRayThreadArgs;
//                }
//                threadList.clear();
//            }
        }
    }
}

void Scene::photonGenerating(float photonPower, float photonAmount) {
    for (auto light : m_lightList) {
        for (int i = 0; i < photonAmount; ++i) {
            Ray photonTraceRay;
            Coord photonPosition;
            Velocity photonDirection;
            do {
                // determine photon position and emit direction
                photonPosition = light->getLightOrigin();
                photonDirection = Util::randomSphere();

                // photon tracing and store photon on hitted surface
                photonTraceRay = {photonPosition, photonDirection};
            } while (!photonTracing(photonTraceRay, photonPower, 0));
        }
    }
}
