#include "StructureOfArrays.h"

int main() {
    Object::Init(1024);

    Object* obj0 = Object::New();
    Object* obj1 = Object::New();
    Object* obj2 = Object::New();

    // Fragment it!
    Object::Delete(obj1);
    obj1 = Object::New();

    // Traditional OOP-access.
    obj0->Position() = Vec3(10, 20, 30);
    obj1->Position() = Vec3(100, 200, 300);
    obj2->Position() = Vec3(1000, 2000, 3000);

    // The batch access is still sequential.
    for (Object& obj : Object::all)
        obj.Velocity() = Vec3(0.1f, 0.2f, 0.3f);

    for (Object& obj : Object::all) {
        obj.Health() = 100;
        obj.Damage() = 2;
    }

    for (Object& obj : Object::all)
        obj.Position() += obj.Velocity();

    for (Object& obj : Object::all)
        obj.Health() -= obj.Damage();

	// Traditionoal OOP update is still sequential as well:
	// position += velocity inside!
    for (Object& obj : Object::all)
        obj.Update();

    Object::Delete(obj2);
    Object::Delete(obj1);
    Object::Delete(obj0);

    Object::Done();
}

