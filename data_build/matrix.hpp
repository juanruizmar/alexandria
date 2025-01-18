#ifndef MATRIX__250118
#define MATRIX__250118

#include <algorithm>

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

        virtual void transpose() = 0;

        void display(std::ostream &os) const{
            for(size_t i=0; i<n_rows(); ++i){
                for(size_t j=0; j<n_cols(); ++j) os << get(i,j) << " ";
                os << std::endl;
            }
        }
};
template<typename T> class square_matrix_interface: public matrix_interface<T>{
    public:
        inline std::size_t n_rows() const { return range(); }
        inline std::size_t n_cols() const { return range(); }
        virtual std::size_t range() const = 0;

        virtual void transpose() = 0;
};

template<typename T> class matrix: public matrix_interface<T>{
    private:
        class matrix_core{
            private:
                std::valarray<T> payload;
                std::size_t n_cols_;

            public:
                inline matrix_core(std::size_t n_rows, std::size_t n_cols): payload(n_rows*n_cols), n_cols_(n_cols) {}
                inline matrix_core(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(default_value, n_rows*n_cols), n_cols_(n_cols) {}

                inline std::size_t n_rows() const { return payload.size()/n_cols_; }
                inline std::size_t n_cols() const { return n_cols_; }

                inline T& get(std::size_t i, std::size_t j) { return payload[i*n_cols()+j]; }
                inline const T& get(std::size_t i, std::size_t j) const { return payload[i*n_cols()+j]; }
                inline void set(std::size_t i, std::size_t j, const T& value) { payload[i*n_cols()+j]=value; }
        };

        matrix_core payload;
        bool is_transposed;

    public:
        inline matrix(std::size_t n_rows, std::size_t n_cols): payload(n_rows, n_cols), is_transposed(false) {}
        inline matrix(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(n_rows, n_cols, default_value), is_transposed(false) {}

        inline std::size_t n_rows() const { return is_transposed ? payload.n_cols() : payload.n_rows(); }
        inline std::size_t n_cols() const { return is_transposed ? payload.n_rows() : payload.n_cols(); }

        inline void transpose() { is_transposed=!is_transposed; }

        inline T& get(std::size_t i, std::size_t j) { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return is_transposed ? payload.get(j,i): payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { (is_transposed ? payload.get(j,i) : payload.get(i,j)) = value; }
};
template<typename T> class sq_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

    public:
        inline sq_matrix(std::size_t range): payload(range,range) {}
        inline sq_matrix(std::size_t range, const T &default_value): payload(range,range,default_value) {}

        inline std::size_t range() const { return payload.n_cols(); }

        inline void transpose() { payload.transpose(); }

        inline T& get(std::size_t i, std::size_t j) { return payload.get(i,j); }
        inline const T& get(std::size_t i, std::size_t j) const { return payload.get(i,j); }
        inline void set(std::size_t i, std::size_t j, const T& value) { payload.set(i,j,value); }
};
template<typename T> class sym_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

        inline std::pair<size_t,size_t> translate_ij(size_t i, size_t j) const{
            assert(i<=j);
            if(i<payload.n_rows()) return std::make_pair(i, j + payload.n_cols()/2 - payload.n_rows() +1);
            else return std::make_pair(2*payload.n_rows()-i-1, 2*payload.n_rows()-j + payload.n_cols()/2 - payload.n_rows() -1);
        }

        inline T& internal_get(std::pair<size_t,size_t> ij){ return payload.get(ij.first, ij.second); }
        inline const T& internal_get(std::pair<size_t,size_t> ij) const { return payload.get(ij.first, ij.second); }

    public:
        inline sym_matrix(std::size_t range): payload(1 + (range-1)/2, range + ((range+1)%2)) {}
        inline sym_matrix(std::size_t range, const T &default_value): payload(1 + (range-1)/2, range + ((range+1)%2), default_value) {}

        inline std::size_t range() const { return payload.n_cols() + payload.n_rows() - payload.n_cols()/2 -1; }

        inline void transpose() {}

        inline T& get(std::size_t i, std::size_t j) { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline const T& get(std::size_t i, std::size_t j) const { return i<=j ? internal_get(translate_ij(i,j)) : internal_get(translate_ij(j,i)); }
        inline void set(std::size_t i, std::size_t j, const T& value) { i<=j ? internal_get(translate_ij(i,j))=value : internal_get(translate_ij(j,i))=value; }
};

#endif
