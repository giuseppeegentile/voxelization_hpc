#include "Vector3D.hpp"

class Atom {
    public:
        std::string name;
        Vector3D position;

        Atom() : name(""), position(0, 0, 0) {}
        Atom(const std::string& name, const Vector3D& position) : name(name), position(position) {}
};