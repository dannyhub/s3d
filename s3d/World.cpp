#include "World.h"

namespace s3d
{

World::World() {
}


World::~World() {
}

void World::addToWorld(ObjectPtr obj, const Point3FD& pos) {
    Matrix4x4FD translateMat = {1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                pos.x_, pos.y_, pos.z_, 1};

    //obj->setWorldPosition({pos.x_, pos.y_, pos.z_});
    //obj->transVertexList_.clear();
    //for (auto &vertex : obj->localVertexList_) {
    //  
    //  obj->transVertexList_.push_back(vertex * translateMat);
    //}

    objects_.push_back(obj);
}

}// namespace s3d