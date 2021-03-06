#include "StructureOfArrays.h"

#include <cassert>
#include <vector>

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3& Vec3::operator+=(const Vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}


struct Object::Shared {
    std::vector<Object> objects;
    Object* freeHead = nullptr;
    Offset numObjects = 0;

    std::vector<Object*> offsetToObj;

    // Structure of arrays
    std::vector<Vec3> position;
    std::vector<Vec3> velocity;
    std::vector<float> health;
    std::vector<float> damage;
};

Object::All Object::all;
Object::Shared Object::shared;

// But additional data can be added to the Object. guid, for example.
static_assert(sizeof(Object) == sizeof(void*));

void Object::Init(Offset maxObjects) {
    shared.objects.resize(maxObjects);

    // Create free list.
    for (Object& obj : shared.objects)
        obj.next = &obj + 1;
    shared.objects[maxObjects - 1].next = nullptr;
    shared.freeHead = shared.objects.data();

    shared.offsetToObj.resize(maxObjects, nullptr);

    shared.position.resize(maxObjects);
    shared.velocity.resize(maxObjects);
    shared.health.resize(maxObjects);
    shared.damage.resize(maxObjects);
}

void Object::Done() {
    assert(shared.numObjects == 0);
}

Object* Object::New() {
    Object* obj = shared.freeHead;
    if (obj) {
        shared.freeHead = obj->next;
        obj->offset = shared.numObjects;
        shared.offsetToObj[obj->offset] = obj;
        ++shared.numObjects;
    }
    return obj;
}

void Object::Delete(Object* obj) {
    if (!obj)
        return;
    assert(shared.numObjects > 0);

    Object* last = shared.offsetToObj[shared.numObjects - 1];
    if (last != obj) {
        assert(last->offset == shared.numObjects - 1);
        // Defrag SoA data so it stays continous:
        CopyData(last->offset, obj->offset);
        // Let the last object points at deleted obj:
        last->offset = obj->offset;
		shared.offsetToObj[last->offset] = last;
    }

    obj->next = shared.freeHead;
    --shared.numObjects;
    shared.freeHead = obj;
}

Object* Object::All::iterator::operator->() {
    return shared.offsetToObj[offset];
}
Object& Object::All::iterator::operator*() {
    return *shared.offsetToObj[offset];
}

Object::All::iterator Object::All::begin() {
    return iterator(0);
}

Object::All::iterator Object::All::end() {
    return iterator(shared.numObjects);
}

Vec3& Object::Position() {
    return shared.position[offset];
}

Vec3& Object::Velocity() {
    return shared.velocity[offset];
}

float& Object::Health() {
    return shared.health[offset];
}

float& Object::Damage() {
    return shared.damage[offset];
}

void Object::Update() {
    Position() += Velocity();
}

void Object::CopyData(Offset from, Offset to) {
    assert(from != to);
    shared.position[to] = shared.position[from];
    shared.velocity[to] = shared.velocity[from];
    shared.health[to] = shared.health[from];
    shared.damage[to] = shared.damage[from];
}
