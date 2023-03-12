

#include <array>
#include <vector>
#include <memory>

#include "Atom.hpp"
#include "Voxel.hpp"
#include "../parameters.hpp"



class Octree {
    public:
        Octree(const std::vector<Atom>& atoms_, const double voxel_size_) : atoms(atoms_), voxel_size(voxel_size_) {};

        std::vector<size_t> get_atoms_within_radius(const Vector3D& point, double radius) const;

    private:
        void build_octree(const std::vector<Atom>& atoms);
        void add_atom_to_voxel(const Atom& atom, Voxel& voxel);
        void subdivide_voxel(Voxel& voxel);
        std::vector<const Voxel*> get_leaf_voxels() const; // Visita trasversale dell'octree

        Voxel root;
        std::vector<Atom> atoms;
        const double voxel_size;
};
