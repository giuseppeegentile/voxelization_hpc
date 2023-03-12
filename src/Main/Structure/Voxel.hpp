
#include "Vector3D.hpp"

// Container per il singolo Voxel nell'octree. 
// Memorizza la posizione del voxel, la dimensione e gli indici di ogni atomo che contiene.
class Voxel {
    Voxel::Voxel(const Vector3D& center_, size_t voxel_size_) : center(center_), voxel_size(voxel_size_) {}
    public:
        bool is_leaf; // se è alla fine dell'albero => 1. altrimenti 0.
        Vector3D center; // Da capire come impostare il sistema di riferimento. Sfruttare simmetrie/bilanciamenti ?
                        // Per capire che branch dell'octree prendere quando cerchiamo atomi con un certo raggio, in quanto abbiamo un riferimento per quale branch 
                        // ha raggio più vicino a quello che stiamo cercando.
        double voxel_size;    // Lunghezza di una dimensione del voxel.
        std::vector<size_t> atom_indices;   // Indici di tutti gli atomi contenuti nel voxel.
        std::array<Voxel, 8> children;      // Array con 8 puntatori a Voxel: gli 8 figli.
                                            // Se un voxel non ha atomi, i suoi figli vengono ricorsivamente divisi in voxel più piccoli fino a un minimo di voxel size.
                                            // Ci permette di fare una ricerca per raggio dell'atomo.
};