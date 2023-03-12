#include <vector>
#include <array>
#include <cmath>
#include <algorithm>


class Vector3D {
    public:
        double x, y, z;

        Vector3D() : x(0), y(0), z(0) {}
        Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

        // Distanza tra due punti in R3
        double distance(const Vector3D& other) const {
            double dx = x - other.x;
            double dy = y - other.y;
            double dz = z - other.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }
};