#ifndef MATRIX__250118
#define MATRIX__250118

#include <utility>

#include <valarray>

#include <cassert>

template<typename T> class matrix_interface{
    public:
        virtual std::size_t n_rows() const = 0;
        virtual std::size_t n_cols() const = 0;
        inline std::size_t size() const { return n_rows()*n_cols(); }

        virtual T& get(std::size_t i, std::size_t j) = 0;
        virtual const T& get(std::size_t i, std::size_t j) const = 0;
        virtual void set(std::size_t i, std::size_t j, const T& value) = 0;

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

                static inline matrix_core add(const matrix_core &lhs, const matrix_core &rhs){
                    assert(lhs.n_rows()==rhs.n_rows());
                    assert(lhs.n_cols()==rhs.n_cols());
                    assert(lhs.payload.size()==lhs.n_cols()*lhs.n_rows());
                    return matrix_core(lhs.payload+rhs.payload, lhs.n_cols_);
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
        };

        matrix_core payload;
        bool is_transposed;

        inline matrix(matrix_core &&external_payload, bool external_is_transposed): payload(external_payload), is_transposed(external_is_transposed) {}

        static inline matrix add(const matrix &lhs, const matrix &rhs){
            assert(lhs.n_rows()==rhs.n_rows());
            assert(lhs.n_cols()==rhs.n_cols());
            return matrix(matrix_core::add(lhs.payload, ( lhs.is_transposed == rhs.is_transposed ? rhs.payload : rhs.payload.low_level_transposed())), lhs.is_transposed);
        }

    public:
        inline matrix(std::size_t n_rows, std::size_t n_cols): payload(n_rows, n_cols), is_transposed(false) {}
        inline matrix(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(n_rows, n_cols, default_value), is_transposed(false) {}

        inline std::size_t n_rows() const { return is_transposed ? payload.n_cols() : payload.n_rows(); }
        inline std::size_t n_cols() const { return is_transposed ? payload.n_rows() : payload.n_cols(); }

        inline matrix<T> transposed() const { return matrix<T>(payload.deep_copy(), !is_transposed); }

        inline T& get(std::size_t i, std::size_t j) { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { (is_transposed ? payload.get(j,i) : payload.get(i,j)) = value; }
        
        template<typename S> friend matrix<S> operator +(const matrix<S> &, const matrix<S> &);
};
template<typename T> class sq_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

        inline sq_matrix(matrix<T> &&external_payload): payload(external_payload) {}

        static inline sq_matrix add(const sq_matrix &lhs, const sq_matrix &rhs){
            assert(lhs.range()==rhs.range());
            return sq_matrix(lhs.payload+rhs.payload);
        }

    public:
        inline sq_matrix(std::size_t range): payload(range, range) {}
        inline sq_matrix(std::size_t range, const T &default_value): payload(range, range, default_value) {}

        inline std::size_t range() const { return payload.n_cols(); }

        inline sq_matrix<T> transposed() const { return sq_matrix(payload.transposed()); }

        inline T& get(std::size_t i, std::size_t j) { return payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { payload.set(i,j,value); }
        
        template<typename S> friend sq_matrix<S> operator +(const sq_matrix<S> &, const sq_matrix<S> &);
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

        inline T& get(std::size_t i, std::size_t j) { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline const T& get(std::size_t i, std::size_t j) const { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline void set(std::size_t i, std::size_t j, const T& value) { i<=j ? internal_get(translate_ij(i,j))=value : internal_get(translate_ij(j,i))=value; }

        template<typename S> friend sym_matrix<S> operator +(const sym_matrix<S> &, const sym_matrix<S> &);
};

class incorrect_matrix_size: std::exception{
    private:
        std::size_t n_rows_got_, n_rows_expected_, n_cols_got_, n_cols_expected_;
    public:
        inline incorrect_matrix_size(std::size_t n_rows_got, std::size_t n_rows_expected, std::size_t n_cols_got, std::size_t n_cols_expected):
        n_rows_got_(n_rows_got), n_rows_expected_(n_rows_expected), n_cols_got_(n_cols_got), n_cols_expected_(n_cols_expected) {}

        inline const char *what() { return ("Incorrect size in operation with matrix: Expected" + 
            std::to_string(n_rows_expected_) + " x " + std::to_string(n_cols_expected_) + ", but received" +  
            std::to_string(n_rows_got_) + " x " + std::to_string(n_cols_got_)).c_str(); 
        }
};

// PENDIENTE: Optimize that (a template of templates ?): 
template <typename T> matrix<T> operator +(const matrix<T> &lhs, const matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> sq_matrix<T> operator +(const sq_matrix<T> &lhs, const sq_matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sq_matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}
template <typename T> sym_matrix<T> operator +(const sym_matrix<T> &lhs, const sym_matrix<T> &rhs){
    if(lhs.n_rows()==rhs.n_rows() && lhs.n_cols()==rhs.n_cols()) return sym_matrix<T>::add(lhs, rhs);
    else throw incorrect_matrix_size(lhs.n_rows(), rhs.n_rows(), lhs.n_cols(), rhs.n_cols());
}

#endif
