#ifndef MATRIX__250118
#define MATRIX__250118

#include <utility>

#include <valarray>

#include <cassert>

#include <fstream>
#include <iostream>

#include <alexandria/data_build/rand_generator.hpp>

template<typename T> class matrix_interface{
    public:
        virtual std::size_t n_rows() const = 0;
        virtual std::size_t n_cols() const = 0;

        virtual bool is_square() const = 0;

        virtual const T& get(std::size_t i, std::size_t j) const = 0;

        void display(std::ostream &os = std::cout) const {
            for (std::size_t i = 0; i < n_rows(); ++i){
                for (std::size_t j = 0; j < n_cols(); ++j) os << get(i,j) << " ";
                os << std::endl;
            }
        }
        inline void display_size(std::ostream &os) const { os << n_rows() << " x " << n_cols() << std::endl; }
};

template<typename T> class matrix: public matrix_interface<T>{
    private:
        std::size_t n_cols_;

        matrix<T> add(const matrix<T>& other) const {
            assert(n_rows() == other.n_rows());
            assert(n_cols() == other.n_cols());

            matrix<T> res(n_rows(), n_cols());
            for(std::size_t i=0; i<n_rows(); ++i){
                for(std::size_t j=0; j<n_cols(); ++j){
                    res.payload[i*n_cols()+j] = payload[i*n_cols()+j] + other.payload[i*n_cols()+j];
                }
            }
            return res;
        }
        matrix<T> sub(const matrix<T>& other) const {
            assert(n_rows() == other.n_rows());
            assert(n_cols() == other.n_cols());

            matrix<T> res(n_rows(), n_cols());
            for(std::size_t i=0; i<n_rows(); ++i){
                for(std::size_t j=0; j<n_cols(); ++j){
                    res.payload[i*n_cols()+j] = payload[i*n_cols()+j] - other.payload[i*n_cols()+j];
                }
            }
            return res;
        }
        matrix<T> mul_naive(const matrix<T>& other) const {
            assert(n_cols() == other.n_rows());

            matrix<T> res(n_rows(), other.n_cols());
            for(std::size_t i=0; i<n_rows(); ++i){
                for(std::size_t j=0; j<other.n_cols(); ++j){
                    res.payload[i*other.n_cols()+j] = 0;
                    for(std::size_t k=0; k<n_cols(); ++k){
                        res.payload[i*other.n_cols()+j] += payload[i*n_cols()+k] * other.payload[k*other.n_cols()+j];
                    }
                }
            }
            return res;
        }

    protected:
        std::valarray<T> payload;

    public:
        class incorrect_matrix_size_add: std::exception{
            private:
                std::size_t n_rows_lhs_, n_cols_lhs_, n_rows_rhs_, n_cols_rhs_;

            public:
                inline incorrect_matrix_size_add(std::size_t n_rows_lhs, std::size_t n_cols_lhs, std::size_t n_rows_rhs, std::size_t n_cols_rhs):
                n_rows_lhs_(n_rows_lhs), n_cols_lhs_(n_cols_lhs), n_rows_rhs_(n_rows_rhs), n_cols_rhs_(n_cols_rhs) {}

                inline const char *what() { return ("Incorrect size in operation with matrix addition: [" + 
                    std::to_string(n_rows_lhs_) + "] x [" + std::to_string(n_cols_lhs_) + "] + [" +  
                    std::to_string(n_rows_rhs_) + "] x [" + std::to_string(n_cols_rhs_) + "]").c_str(); 
                }
        };
        class incorrect_matrix_size_mul: std::exception{
            private:
                std::size_t n_rows_lhs_, n_cols_lhs_, n_rows_rhs_, n_cols_rhs_;

            public:
                inline incorrect_matrix_size_mul(std::size_t n_rows_lhs, std::size_t n_cols_lhs, std::size_t n_rows_rhs, std::size_t n_cols_rhs):
                n_rows_lhs_(n_rows_lhs), n_cols_lhs_(n_cols_lhs), n_rows_rhs_(n_rows_rhs), n_cols_rhs_(n_cols_rhs) {}

                inline const char *what() { return ("Incorrect size in operation with matrix multiplication: [" + 
                    std::to_string(n_rows_lhs_) + "] x [" + std::to_string(n_cols_lhs_) + "] + [" +  
                    std::to_string(n_rows_rhs_) + "] x [" + std::to_string(n_cols_rhs_) + "]").c_str(); 
                }
        };

        inline matrix(std::size_t n_rows, std::size_t n_cols): n_cols_(n_cols), payload(n_rows*n_cols){}
        inline matrix(std::size_t n_rows, std::size_t n_cols, const T &default_value): n_cols_(n_cols), payload(default_value, n_rows*n_cols) {}

        inline std::size_t n_rows() const { return payload.size() / n_cols_; }
        inline std::size_t n_cols() const { return n_cols_; }

        inline bool is_square() const { return n_rows() == n_cols(); }

        inline const T& get(std::size_t i, std::size_t j) const{
            if(i>=n_rows() || j>=n_cols()) throw std::out_of_range("Index out of range");
            return payload[i*n_cols()+j];
        }
        inline void set(std::size_t i, std::size_t j, const T &value){
            if(i>=n_rows() || j>=n_cols()) throw std::out_of_range("Index out of range");
            payload[i*n_cols()+j] = value;
        }

        friend matrix<T> operator*(const T &k, const matrix<T>& rhs){
            matrix<T> res = rhs;
            for(auto &i: res.payload) i*=k;
            return res;
        }
        friend matrix<T> operator*(const matrix<T>& rhs, const T &k){
            matrix<T> res = rhs;
            for(auto &i: res.payload) i*=k;
            return res;
        }
        friend matrix<T> operator/(const T &k, const matrix<T>& rhs){
            matrix<T> res = rhs;
            for(auto &i: res.payload){
                if(i==0) throw std::invalid_argument("Division by zero is not allowed.");
                i = k/i;
            }
            return res;
        }
        friend matrix<T> operator/(const matrix<T>& rhs, const T &k){
            if(k==0) throw std::invalid_argument("Division by zero is not allowed.");
            matrix<T> res = rhs;
            for(auto &i: res.payload) i/=k;
            return res;
        }

        friend matrix<T> operator+(const matrix<T>& lhs, const matrix<T>& rhs){
            if(lhs.n_rows()!=rhs.n_rows() || lhs.n_cols()!=rhs.n_cols()) throw incorrect_matrix_size_add(lhs.n_rows(), lhs.n_cols(), rhs.n_rows(), rhs.n_cols());
            return lhs.add(rhs);
        }
        friend matrix<T> operator-(const matrix<T>& lhs, const matrix<T>& rhs){
            if(lhs.n_rows()!=rhs.n_rows() || lhs.n_cols()!=rhs.n_cols()) throw incorrect_matrix_size_add(lhs.n_rows(), lhs.n_cols(), rhs.n_rows(), rhs.n_cols());
            return lhs.sub(rhs);
        }
        friend matrix<T> operator*(const matrix<T>& lhs, const matrix<T>& rhs){
            if(lhs.n_cols() != rhs.n_rows()) throw incorrect_matrix_size_mul(lhs.n_rows(), lhs.n_cols(), rhs.n_rows(), rhs.n_cols());
            return lhs.mul_naive(rhs);
        }

        static matrix<T> identity(std::size_t range){
            matrix<T> res(range, range, 0);
            for(std::size_t i=0; i<range; ++i) res.set(i,i,1);
            return res;
        }
};
template<typename T> class sym_matrix: public matrix_interface<T>{
    private:
        matrix<T> payload;

        inline std::pair<std::size_t,std::size_t> translate_ij(std::size_t i, std::size_t j) const{
            assert(i<=j);
            if(i<payload.n_rows()) return std::make_pair(i, j + payload.n_cols()/2 - payload.n_rows() +1);
            else return std::make_pair(2*payload.n_rows()-i-1, 2*payload.n_rows()-j + payload.n_cols()/2 - payload.n_rows() -1);
        }

        inline const T& internal_get(const std::pair<std::size_t,std::size_t> &ij) const { return payload.get(ij.first, ij.second); }
        inline void internal_set(const std::pair<std::size_t,std::size_t> &ij, const T &value) { return payload.set(ij.first, ij.second, value); }

        inline sym_matrix(matrix<T> &&external_payload): payload(external_payload) {}

    public:
        inline sym_matrix(std::size_t range): payload(1 + (range-1)/2, range + ((range+1)%2)) {}
        inline sym_matrix(std::size_t range, const T &default_value): payload(1 + (range-1)/2, range + ((range+1)%2), default_value) {}

        operator matrix<T>(){
            matrix<T> res(n_rows(), n_cols(), 0);
            for(std::size_t i=0; i<n_rows(); ++i) for(std::size_t j=i; j<n_cols(); ++j){
                std::pair<std::size_t,std::size_t> ij = translate_ij(i,j);
                res.set(i, j, internal_get(ij));
                res.set(j, i, internal_get(ij));
            }
            return res;
        }

        inline std::size_t n_rows() const { return range(); }
        inline std::size_t n_cols() const { return range(); }
        inline std::size_t range() const { return payload.n_cols() + payload.n_rows() - payload.n_cols()/2 -1; }

        inline bool is_square() const { return true; }
        inline bool is_symmetric() const { return true; }

        inline const T& get(std::size_t i, std::size_t j) const{
            if(i>=n_rows() || j>=n_cols()) throw std::out_of_range("Index out of range");
            return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i));
        }
        inline void set(std::size_t i, std::size_t j, const T& value){
            if(i>=n_rows() || j>=n_cols()) throw std::out_of_range("Index out of range");
            i<=j ? internal_set(translate_ij(i,j), value) : internal_set(translate_ij(j,i), value);
        }
        
        friend sym_matrix<T> operator+(const sym_matrix<T>& lhs, const sym_matrix<T>& rhs) { return sym_matrix<T>(lhs.payload+rhs.payload); }
        friend sym_matrix<T> operator-(const sym_matrix<T>& lhs, const sym_matrix<T>& rhs) { return sym_matrix<T>(lhs.payload-rhs.payload); }
};

class incorrect_matrix_size_add: std::exception{
    private:
        std::size_t n_rows_lhs_, n_cols_lhs_, n_rows_rhs_, n_cols_rhs_;
    public:
        inline incorrect_matrix_size_add(std::size_t n_rows_lhs, std::size_t n_cols_lhs, std::size_t n_rows_rhs, std::size_t n_cols_rhs):
        n_rows_lhs_(n_rows_lhs), n_cols_lhs_(n_cols_lhs), n_rows_rhs_(n_rows_rhs), n_cols_rhs_(n_cols_rhs) {}

        inline const char *what() { return ("Incorrect size in operation with matrix addition: [" + 
            std::to_string(n_rows_lhs_) + "] x [" + std::to_string(n_cols_lhs_) + "] + [" +  
            std::to_string(n_rows_rhs_) + "] x [" + std::to_string(n_cols_rhs_) + "]").c_str(); 
        }
};
class incorrect_matrix_size_mul: std::exception{
    private:
        std::size_t n_rows_lhs_, n_cols_lhs_, n_rows_rhs_, n_cols_rhs_;
    public:
        inline incorrect_matrix_size_mul(std::size_t n_rows_lhs, std::size_t n_cols_lhs, std::size_t n_rows_rhs, std::size_t n_cols_rhs):
        n_rows_lhs_(n_rows_lhs), n_cols_lhs_(n_cols_lhs), n_rows_rhs_(n_rows_rhs), n_cols_rhs_(n_cols_rhs) {}

        inline const char *what() { return ("Incorrect size in operation with matrix multiplication: [" + 
            std::to_string(n_rows_lhs_) + "] x [" + std::to_string(n_cols_lhs_) + "] + [" +  
            std::to_string(n_rows_rhs_) + "] x [" + std::to_string(n_cols_rhs_) + "]").c_str(); 
        }
};

class real_numbers_matrix: public matrix<double>{
    private:
        inline void rand_fill(const random_generator<double> &gen) { gen.fill(begin(payload), end(payload)); }

    public:
        inline real_numbers_matrix(std::size_t n_rows, std::size_t n_cols): matrix<double>(n_rows, n_cols) {}
        inline real_numbers_matrix(std::size_t n_rows, std::size_t n_cols, double default_value): matrix<double>(n_rows, n_cols, default_value) {}

        void gaussian_rand_fill(double mean, double stddev) { rand_fill(gaussian_generator(mean, stddev)); }
        void uniform_rand_fill(double min, double max) { rand_fill(uniform_generator(min, max)); }
};

#endif
