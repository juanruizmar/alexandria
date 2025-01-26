#ifndef MATRIX__250118
#define MATRIX__250118

#include <utility>

#include <valarray>

#include <cassert>

#include <iostream>

#include "rand_generator.hpp"

template<typename T> class matrix_interface{
    public:
        virtual std::size_t n_rows() const = 0;
        virtual std::size_t n_cols() const = 0;
        inline std::size_t size() const { return n_rows()*n_cols(); }

        virtual T& get(std::size_t i, std::size_t j) = 0;
        virtual const T& get(std::size_t i, std::size_t j) const = 0;
        virtual void set(std::size_t i, std::size_t j, const T& value) = 0;

        virtual bool is_square() const { return n_rows()==n_cols(); }

        void display(std::ostream &os) const{
            for(std::size_t i=0; i<n_rows(); ++i){
                for(std::size_t j=0; j<n_cols(); ++j) os << get(i,j) << " ";
                os << std::endl;
            }
            os << std::endl;
        }

        inline void display_size(std::ostream &os) const { os << n_rows() << " x " << n_cols() << std::endl; }
};
template<typename T> class square_matrix_interface: public matrix_interface<T>{
    public:
        inline std::size_t n_rows() const { return range(); }
        inline std::size_t n_cols() const { return range(); }
        virtual std::size_t range() const = 0;

        virtual bool is_square() const { return true; }
        virtual bool is_symmetric() const = 0;

};

template<typename T> class matrix: public matrix_interface<T>{
    private:
        class matrix_core{
            private:
                std::valarray<T> payload;
                std::size_t n_cols_;

                inline matrix_core(std::valarray<T> &&external_payload, std::size_t external_n_cols): payload(external_payload), n_cols_(external_n_cols) {}

                inline matrix_core low_level_transposed() const{
                    matrix_core res(n_cols(), n_rows());
                    for(std::size_t i=0; i<n_rows(); ++i) for(std::size_t j=0; j<n_cols(); ++j) res.set(j,i,get(i,j));
                    return res;
                }
                inline matrix_core deep_copy() const { return *this; }

                inline matrix_core extract(std::size_t i_min, std::size_t i_max, std::size_t j_min, std::size_t j_max) const{
                    matrix_core res(i_max-i_min, j_max-j_min);
                    for(std::size_t i=i_min; i<i_max; ++i) for(std::size_t j=j_min; j<j_max; ++j) res.set(i-i_min, j-j_min, get(i,j));
                    return res;
                }
                inline void insert(std::size_t i_min, std::size_t j_min, const matrix_core &M){
                    for(std::size_t i=0; i<M.n_rows(); ++i) for(std::size_t j=0; j<M.n_cols(); ++j) set(i+i_min, j+j_min, M.get(i, j));
                }

                inline matrix_core amplify_before_strassen() const{
                    std::size_t range_lower_bound = std::max(n_rows(), n_cols()), range=1;
                    while(range_lower_bound > range) range*=2;

                    matrix_core res(range, range, 0);
                    for(std::size_t i=0; i<n_rows(); ++i) for(std::size_t j=0; j<n_cols(); ++j) res.set(i,j,get(i,j));

                    return res;
                }
                inline matrix_core reduce_after_strassen(std::size_t n_rows_, std::size_t n_cols_) const{
                    matrix_core res(n_rows_, n_cols_);
                    for(std::size_t i=0; i<n_rows_; ++i) for(std::size_t j=0; j<n_cols_; ++j) res.set(i,j,get(i,j));
                    return res;
                }
               
                inline matrix_core top_left() const { return extract(0, n_rows()/2, 0, n_cols()/2); }
                inline matrix_core top_right() const { return extract(0, n_rows()/2, n_cols()/2, n_cols()); }
                inline matrix_core bottom_left() const { return extract(n_rows()/2, n_rows(), 0, n_cols()/2); }
                inline matrix_core bottom_right() const { return extract(n_rows()/2, n_rows(), n_cols()/2, n_cols()); }

                static inline matrix_core join(const matrix_core &top_left_, const matrix_core &top_right_, const matrix_core &bottom_left_, const matrix_core &bottom_right_){
                    assert(top_left_.n_rows()==top_right_.n_rows());
                    assert(bottom_left_.n_rows()==bottom_right_.n_rows());
                    assert(top_left_.n_cols()==bottom_left_.n_cols());
                    assert(top_right_.n_cols()==bottom_right_.n_cols());      

                    matrix_core res(top_left_.n_rows()+top_right_.n_rows(), top_left_.n_cols()+bottom_left_.n_cols(), 0);

                    res.insert(0, 0, top_left_);
                    res.insert(0, top_left_.n_cols(), top_right_);
                    res.insert(top_left_.n_rows(), 0, bottom_left_);
                    res.insert(top_left_.n_rows(), top_left_.n_cols(), bottom_right_);
                    
                    return res;
                }

                static inline matrix_core mul_naive(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_cols()==rhs.n_rows());
                    matrix_core res(lhs.n_rows(), rhs.n_cols(), 0);

                    for(size_t i=0; i<lhs.n_rows(); ++i)
                        for(size_t j=0; j<rhs.n_cols(); ++j)
                            for(size_t k=0; k<lhs.n_cols(); ++k)
                                res.set(i,j, res.get(i,j)+lhs.get(i,k)*rhs.get(k,j));
                            
                    return res;
                }
                static inline matrix_core mul_strassen(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_cols()==rhs.n_rows());
                    return mul_strassen_rec(lhs.amplify_before_strassen(), rhs.amplify_before_strassen(), 1).reduce_after_strassen(lhs.n_rows(), rhs.n_cols());
                }
                static inline matrix_core mul_strassen_rec(const matrix_core &lhs, const matrix_core &rhs, std::size_t naive_threshold){
                    if(lhs.n_rows()<=naive_threshold) return mul_naive(lhs, rhs);
                    else{
                        const matrix_core L11 = lhs.top_left();
                        const matrix_core L12 = lhs.top_right();
                        const matrix_core L21 = lhs.bottom_left();
                        const matrix_core L22 = lhs.bottom_right();
                        const matrix_core R11 = rhs.top_left();
                        const matrix_core R12 = rhs.top_right();
                        const matrix_core R21 = rhs.bottom_left();
                        const matrix_core R22 = rhs.bottom_right();

                        const matrix_core M1 = mul_strassen_rec(add(L11, L22), add(R11, R22), naive_threshold);
                        const matrix_core M2 = mul_strassen_rec(add(L21, L22), R11, naive_threshold);
                        const matrix_core M3 = mul_strassen_rec(L11, sub(R12, R22), naive_threshold);
                        const matrix_core M4 = mul_strassen_rec(L22, sub(R21, R11), naive_threshold);
                        const matrix_core M5 = mul_strassen_rec(add(L11, L12), R22, naive_threshold);
                        const matrix_core M6 = mul_strassen_rec(sub(L21, L11), add(R11, R12), naive_threshold);
                        const matrix_core M7 = mul_strassen_rec(sub(L12, L22), add(R21, R22), naive_threshold);
                        
                        return join(add(M1, add(M4, sub(M7, M5))), add(M3, M5), add(M2, M4), add(M1, add(M3, sub(M6, M2))));
                    }
                }
            
                static inline matrix_core add(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_rows()==rhs.n_rows());
                    assert(lhs.n_cols()==rhs.n_cols());
                    assert(lhs.payload.size()==lhs.n_cols()*lhs.n_rows());
                    return matrix_core(lhs.payload+rhs.payload, lhs.n_cols_);
                }
                static inline matrix_core sub(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_rows()==rhs.n_rows());
                    assert(lhs.n_cols()==rhs.n_cols());
                    assert(lhs.payload.size()==lhs.n_cols()*lhs.n_rows());
                    return matrix_core(lhs.payload-rhs.payload, lhs.n_cols_);
                }
                static inline matrix_core mul(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_cols()==rhs.n_rows());
                    return mul_strassen(lhs, rhs);
                }

            public:
                inline matrix_core(std::size_t n_rows, std::size_t n_cols): payload(n_rows*n_cols), n_cols_(n_cols) {}
                inline matrix_core(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(default_value, n_rows*n_cols), n_cols_(n_cols) {}

                inline std::size_t n_rows() const { return payload.size()/n_cols_; }
                inline std::size_t n_cols() const { return n_cols_; }

                inline T& get(std::size_t i, std::size_t j) { return payload[i*n_cols()+j]; }
                inline const T& get(std::size_t i, std::size_t j) const { return payload[i*n_cols()+j]; }
                inline void set(std::size_t i, std::size_t j, const T& value) { payload[i*n_cols()+j]=value; }

                friend class matrix;
                friend class real_numbers_matrix;
        };

        matrix_core payload;
        bool is_transposed;

        inline matrix(matrix_core &&external_payload, bool external_is_transposed = false): payload(external_payload), is_transposed(external_is_transposed) {}

        static inline matrix add(const matrix &lhs, const matrix &rhs){
            assert(lhs.n_rows()==rhs.n_rows());
            assert(lhs.n_cols()==rhs.n_cols());
            return matrix(matrix_core::add(lhs.payload, ( lhs.is_transposed == rhs.is_transposed ? rhs.payload : rhs.payload.low_level_transposed())), lhs.is_transposed);
        }
        static inline matrix mul(const matrix &lhs, const matrix &rhs){
            assert(lhs.n_cols()==rhs.n_rows());
            if(lhs.is_transposed && rhs.is_transposed) return matrix(matrix_core::mul(rhs.payload, lhs.payload), true);
            else if(lhs.is_transposed) return matrix_core::mul(lhs.payload.low_level_transposed(), rhs.payload);
            else if(rhs.is_transposed) return matrix_core::mul(lhs.payload, rhs.payload.low_level_transposed());
            else return matrix_core::mul(lhs.payload, rhs.payload);
        }

    public:
        inline matrix(const matrix<T>&) = default;

        inline matrix(std::size_t n_rows, std::size_t n_cols): payload(n_rows, n_cols), is_transposed(false) {}
        inline matrix(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(n_rows, n_cols, default_value), is_transposed(false) {}

        inline std::size_t n_rows() const { return is_transposed ? payload.n_cols() : payload.n_rows(); }
        inline std::size_t n_cols() const { return is_transposed ? payload.n_rows() : payload.n_cols(); }

        inline matrix<T> transposed() const { return matrix<T>(payload.deep_copy(), !is_transposed); }

        inline T& get(std::size_t i, std::size_t j) { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { (is_transposed ? payload.get(j,i) : payload.get(i,j)) = value; }
        
        template<typename S> friend matrix<S> operator +(const matrix<S> &, const matrix<S> &);
        template<typename S> friend matrix<S> operator *(const matrix<S> &, const matrix<S> &);

        friend class real_numbers_matrix;
};
template<typename T> class sq_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

        inline sq_matrix(matrix<T> &&external_payload): payload(external_payload) {}

        static inline sq_matrix add(const sq_matrix &lhs, const sq_matrix &rhs){
            assert(lhs.range()==rhs.range());
            return sq_matrix(lhs.payload+rhs.payload);
        }
        static inline sq_matrix mul(const sq_matrix &lhs, const sq_matrix &rhs){
            assert(lhs.range()==rhs.range());
            return sq_matrix(lhs.payload*rhs.payload);
        }

    public:
        inline sq_matrix(std::size_t range): payload(range, range) {}
        inline sq_matrix(std::size_t range, const T &default_value): payload(range, range, default_value) {}

        inline operator matrix<T>() const { return payload; }

        inline std::size_t range() const { return payload.n_cols(); }

        inline sq_matrix<T> transposed() const { return sq_matrix(payload.transposed()); }

        bool is_symmetric() const{
            bool res=true;
            for(std::size_t i=0; i<range() && res; ++i) for(std::size_t j=i+1; j<range() && res; ++j) res = get(i,j)==get(j,i);
            return res;
        }

        inline T& get(std::size_t i, std::size_t j) { return payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { payload.set(i,j,value); }
        
        template<typename S> friend sq_matrix<S> operator +(const sq_matrix<S> &, const sq_matrix<S> &);
        template<typename S> friend sq_matrix<S> operator *(const sq_matrix<S> &, const sq_matrix<S> &);
};
template<typename T> class sym_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

        inline std::pair<std::size_t,std::size_t> translate_ij(std::size_t i, std::size_t j) const{
            assert(i<=j);
            if(i<payload.n_rows()) return std::make_pair(i, j + payload.n_cols()/2 - payload.n_rows() +1);
            else return std::make_pair(2*payload.n_rows()-i-1, 2*payload.n_rows()-j + payload.n_cols()/2 - payload.n_rows() -1);
        }

        inline T& internal_get(const std::pair<std::size_t,std::size_t> &ij){ return payload.get(ij.first, ij.second); }
        inline const T& internal_get(const std::pair<std::size_t,std::size_t> &ij) const { return payload.get(ij.first, ij.second); }

        inline sym_matrix(matrix<T> &&external_payload): payload(external_payload) {}

        static inline sym_matrix add(const sym_matrix &lhs, const sym_matrix &rhs){
            assert(lhs.range()==rhs.range());
            return sym_matrix(lhs.payload + rhs.payload);
        }

    public:
        inline sym_matrix(std::size_t range): payload(1 + (range-1)/2, range + ((range+1)%2)) {}
        inline sym_matrix(std::size_t range, const T &default_value): payload(1 + (range-1)/2, range + ((range+1)%2), default_value) {}

        inline std::size_t range() const { return payload.n_cols() + payload.n_rows() - payload.n_cols()/2 -1; }

        inline sym_matrix<T> transposed() const { return *this; }

        inline bool is_symmetric() const { return true; }

        inline T& get(std::size_t i, std::size_t j) { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline const T& get(std::size_t i, std::size_t j) const { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline void set(std::size_t i, std::size_t j, const T& value) { i<=j ? internal_get(translate_ij(i,j))=value : internal_get(translate_ij(j,i))=value; }

        template<typename S> friend sym_matrix<S> operator +(const sym_matrix<S> &, const sym_matrix<S> &);
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

// PENDIENTE: Optimize that (a template of templates ?): 
template <typename T> matrix<T> operator +(const matrix<T> &lhs, const matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size_add(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> sq_matrix<T> operator +(const sq_matrix<T> &lhs, const sq_matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sq_matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size_add(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> sym_matrix<T> operator +(const sym_matrix<T> &lhs, const sym_matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sym_matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size_add(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> matrix<T> operator *(const matrix<T> &lhs, const matrix<T> &rhs){
    if(rhs.n_rows()==lhs.n_cols()) return matrix<T>::mul(lhs, rhs);
    else throw incorrect_matrix_size_mul(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> sq_matrix<T> operator *(const sq_matrix<T> &lhs, const sq_matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sq_matrix<T>::mul(lhs, rhs);
    else throw incorrect_matrix_size_mul(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
//template <typename T> sym_matrix<T> operator *(const sym_matrix<T> &lhs, const sym_matrix<T> &rhs){
//    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sym_matrix<T>::mul(lhs, rhs);
//    else throw incorrect_matrix_size_mul(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
//}

class real_numbers_matrix: public matrix<double>{
    private:
        inline void rand_fill(const random_generator<double> &gen) { gen.fill(begin(payload.payload), end(payload.payload)); }

    public:
        inline real_numbers_matrix(std::size_t n_rows, std::size_t n_cols): matrix<double>(n_rows, n_cols) {}
        inline real_numbers_matrix(std::size_t n_rows, std::size_t n_cols, double default_value): matrix<double>(n_rows, n_cols, default_value) {}

        void gaussian_rand_fill(double mean, double stddev) { rand_fill(gaussian_generator(mean, stddev)); }
        void uniform_rand_fill(double min, double max) { rand_fill(uniform_generator(min, max)); }
};

#endif
