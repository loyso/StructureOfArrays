#pragma once

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    Vec3() = default;
    Vec3(float x, float y, float z);

    Vec3& operator+=(const Vec3& v);
};


class Object {
public:
    using Offset = size_t;

    static void Init(Offset maxObjects);
    static void Done();

    static Object* New();
    static void Delete(Object* obj);

    Vec3& Position();
    Vec3& Velocity();
    float& Health();
    float& Damage();

    // position += velocity;
    void Update();

    struct All {
        class iterator {
        public:
            explicit iterator(Offset offset) : offset(offset) {}
            iterator& operator++() {
                ++offset;
                return *this;
            }
            Object* operator->();
            Object& operator*();
            bool operator!=(iterator& i) {
                return i.offset != offset;
            }
        private:
            Offset offset;
        };

        iterator begin();
        iterator end();
    };

    static All all;

private:
    union {
        Offset offset = 0;
        Object* next;
    };
    
    static void CopyData(Offset from, Offset to);

    struct Shared;
    static Shared shared;
};


