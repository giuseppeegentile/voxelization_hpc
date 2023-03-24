#include <deque>
#include <map>
#include <vector>
#include <numeric>
#include "IR.h"
constexpr size_t width = 3;
constexpr size_t heigth = 3;


/**
 * @brief SPD Covariance Matrix
 * Implemented only get by col, since is symmetric, is the same of getting the same row
 * 
 */
class CovarianceMatrix {
    public:


        CovarianceMatrix(IR &ir) {
            float avg_x = 0.0;
            float avg_y= 0.0;
            float avg_z= 0.0;
            const size_t dim = ir.getData().size();
            for(auto &c: ir.getData()){
                avg_x += c[0];
                avg_y += c[1];
                avg_z += c[2];
            }
            avg_x /= dim;avg_y /= dim;avg_z /= dim;

            for(int i = 0; i < ir.getData().size(); i++){
                ir.setValueAt(i, Coordinate(ir.getData()[i][0] - avg_x,
                                            ir.getData()[i][1] - avg_y,
                                            ir.getData()[i][2] - avg_z));
            }

            std::vector<float> x = ir.getVectorBasis(0);
            std::vector<float> y = ir.getVectorBasis(1);
            std::vector<float> z = ir.getVectorBasis(2);
            float x_squared = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
            float xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
            float xz = std::inner_product(x.begin(), x.end(), z.begin(), 0.0);
            float y_squared = std::inner_product(y.begin(), y.end(), y.begin(), 0.0);
            float yz = std::inner_product(y.begin(), y.end(), z.begin(), 0.0);
            float z_squared = std::inner_product(z.begin(), z.end(), z.begin(), 0.0);

            storage[0].push_back(x_squared);
            storage[0].push_back(xy);
            storage[0].push_back(xz);

            storage[1].push_back(xy);
            storage[1].push_back(y_squared);
            storage[1].push_back(yz);

            storage[2].push_back(xz);
            storage[2].push_back(yz);
            storage[2].push_back(z_squared);

        }

        void print(){
            for(auto &c: storage)
                std::cout << c.second[0] << "       " << c.second[1]<< "        " << c.second[2]<< std::endl;
        }


        //CovarianceMatrix(const CovarianceMatrix& mat);

        /**
         * @brief getter of the element row-col using (row, col)
         * 
         * @param row 
         * @param column 
         * @return const double& 
         */
        const double &operator()(const size_t row, const size_t column) const {
            return storage.at(column)[row]; 
        }

        /**
         * @brief Add in column-major 
         * 
         * @param row the column where to add 
         * @param vals the (eigen)vector to add
         */
        void add(const size_t column, const std::vector<double> &vals);

        /**
         * @brief Get a Row of the matrix
         * 
         * @param col 
         */
        std::vector<double> getCol(const size_t col);

        //const void print() const;

        /**
         * 
         * @brief Perform row-column product (Ab)
         * 
         * @param b vector to multiply with the matrix
         * @param res std::vector<double> 
         */
        std::vector<double> dotProduct(const std::vector<double> &b);

        void swapColumn(int permutation);


    private:
        std::map<int, std::vector<double>> storage; //= {{1, {0., 0., 0. }}, {2, {0., 0., 0. }}, {3, {0., 0., 0. }}};

        std::map<int, std::vector<int>> permutations = {{1, {0,1,2}}, 
                                                        {2, {0,2,1}},
                                                        {3, {1,0,2}},
                                                        {4, {1,2,0}},
                                                        {5, {2,0,1}},
                                                        {6, {2,1,0}}};

};
