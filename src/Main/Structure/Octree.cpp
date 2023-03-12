#include "Octree.hpp"

// Visita albero in profondità ricorsivo, ma senza chiamate ricorsive (smontato)
std::vector<size_t> Octree::get_atoms_within_radius(const Vector3D& point, double radius) const {
    std::vector<size_t> result;      // Ci metterò gli indici degli atomi trovati nella ricerca
    std::vector<const Voxel*> stack; // Voxel da percorrere
    stack.push_back(&root);
    
    while (!stack.empty()) {        // Stop quando avrò percorso tutto
        const Voxel* current = stack.back(); 
        stack.pop_back();
        if (current->is_leaf) { //fine albero ==> metto in result gli indici degli atomi con raggio che soddisfa la mia ricerca
            for (size_t atom_index : current->atom_indices) {
                if (point.distance(atoms[atom_index].position) <= radius) {
                    result.push_back(atom_index);
                }
            }
        } else {                // Analizza tutti gli 8 voxel figli: per ognuno controlla che soddisfi il mio raggio di ricerca
            for (const Voxel& child : current->children) {
                if (child.center.distance(point) < radius + child.voxel_size) { 
                    stack.push_back(&child);
                }
            }
        }
    }
    return result;
}


void Octree::build_octree(const std::vector<Atom>& atoms) {
    // Bordi della proteina
    Vector3D min_pos = atoms[0].position;
    Vector3D max_pos = atoms[0].position;
    for (const Atom& atom : atoms) {
        min_pos.x = std::min(min_pos.x, atom.position.x);
        min_pos.y = std::min(min_pos.y, atom.position.y);
        min_pos.z = std::min(min_pos.z, atom.position.z);
        max_pos.x = std::max(max_pos.x, atom.position.x);
        max_pos.y = std::max(max_pos.y, atom.position.y);
        max_pos.z = std::max(max_pos.z, atom.position.z);
    }

    // Dimensioni dei bordi della proteina
    const double dim_x = max_pos.x - min_pos.x;
    const double dim_y = max_pos.y - min_pos.y;
    const double dim_z = max_pos.z - min_pos.z;
    Vector3D dimensions(dim_x, dim_y, dim_z);


    /**************DA APPROFONDIRE E SOPRATTUTTO CONTROLLARE-   */

    // Precisione massima: usare come max_depth come il raggio dell'atomo più piccolo della proteina. 
    // In questo modo avrei che nessuno atomo abbia più di un voxel da un'altro atomo -> collision detection.
    const double max_dim = std::max(max_pos.x, max_pos.y,max_pos.z);
    const int m_max_depth = std::max(1, int(std::log2(max_dim / smallest_voxel)));
    

    // Determine the maximum dimension of the bounding box
    double max_dim = std::max(dimensions.x, dimensions.y, dimensions.z);

    // Calculate the size of the root voxel
    double voxel_size = max_dim / static_cast<double>(1 << m_max_depth);

    Vector3D center(min_pos.x + dimensions.x / 2, min_pos.y + dimensions.y / 2, min_pos.z + dimensions.z / 2);
    
    // Create the root voxel and add all the atoms to it
    std::unique_ptr<Voxel> m_root = std::make_unique<Voxel>(center, voxel_size);

    for(auto &a : atoms)
        add_atom_to_voxel(a, *m_root);
    
}
