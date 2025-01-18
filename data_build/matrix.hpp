#ifndef MATRIX__250118
#define MATRIX__250118

#include <algorithm>
#include <cmath>

#include <fstream>

#include <valarray>

#include <cassert>

template<typename T> class matrix_interface{
    public:
        virtual std::size_t n_rows() const = 0;
        virtual std::size_t n_cols() const = 0;
        inline std::size_t size() const { return n_rows()*n_cols(); }

        virtual const T& get(std::size_t i, std::size_t j) const = 0;
        virtual void set(std::size_t i, std::size_t j, const T& value) = 0;

        void display(std::ostream &os) const{
            for(size_t i=0; i<n_rows(); ++i){
                for(size_t j=0; j<n_cols(); ++j) os << get(i,j) << " ";
                os << std::endl;
            }
        }
};
template<typename T> class square_matrix_interface: public matrix_interface<T>{
    public:
        virtual std::size_t range() const = 0;
};

template<typename T> class matrix: public matrix_interface<T>{
    private:
        std::valarray<T> payload;
        std::size_t n_rows_;

    public:
        inline matrix(std::size_t n_rows, std::size_t n_cols): payload(n_rows*n_cols), n_rows_(n_rows) {}
        inline matrix(std::size_t n_rows, std::size_t n_cols, const T &default_value): payload(default_value, n_rows*n_cols), n_rows_(n_rows) {}

        inline std::size_t n_rows() const { return n_rows_; }
        inline std::size_t n_cols() const { return payload.size()/n_rows_; }

        inline const T& get(std::size_t i, std::size_t j) const { return payload[i*n_cols()+j]; }
        inline void set(std::size_t i, std::size_t j, const T& value) { payload[i*n_cols()+j]=value; }
};
template<typename T> class sq_matrix: public square_matrix_interface<T>{
    private:
        std::valarray<T> payload;
        std::size_t range_;

    public:
        inline sq_matrix(std::size_t range): payload(range*range), range_(range) {}
        inline sq_matrix(std::size_t range, const T &default_value): payload(default_value, range*range), range_(range) {}

        inline std::size_t n_rows() const { return range(); }
        inline std::size_t n_cols() const { return range(); }
        inline std::size_t range() const { return range_; }

        inline const T& get(std::size_t i, std::size_t j) const { return payload[i*range()+j]; }
        inline void set(std::size_t i, std::size_t j, const T& value) { payload[i*range()+j]=value; }
};
template<typename T> class sym_matrix: public square_matrix_interface<T>{
    private:
        matrix<T> payload;

        inline const T& internal_get(std::size_t i, std::size_t j) const{
            assert(i<=j);
            if(i<payload.n_rows()) return payload.get(i, j + payload.n_cols()/2 - payload.n_rows() +1);
            else return payload.get(2*payload.n_rows()-i-1, 2*payload.n_rows()-j + payload.n_cols()/2 - payload.n_rows() -1);
        }
        inline void internal_set(std::size_t i, std::size_t j, const T& value){
            assert(i<=j);
            if(i<payload.n_rows()) payload.set(i, j + payload.n_cols()/2 - payload.n_rows() +1, value);
            else payload.set(2*payload.n_rows()-i-1, 2*payload.n_rows()-j + payload.n_cols()/2 - payload.n_rows() -1, value);
        }

    public:
        inline sym_matrix(std::size_t range): payload(1 + (range-1)/2, range + ((range+1)%2)) {}
        inline sym_matrix(std::size_t range, const T &default_value): payload(1 + (range-1)/2, range + ((range+1)%2), default_value) {}

        inline std::size_t n_rows() const { return range(); }
        inline std::size_t n_cols() const { return range(); }
        inline std::size_t range() const { return payload.n_cols() + payload.n_rows() - payload.n_cols()/2 -1; }

        inline const T& get(std::size_t i, std::size_t j) const { return i<=j ? internal_get(i,j) : internal_get(j,i); }
        inline void set(std::size_t i, std::size_t j, const T& value) { i<=j ? internal_set(i,j,value) : internal_set(j,i,value); }
};

#endif
