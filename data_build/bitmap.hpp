#ifndef BITMAP___HPP__
#define BITMAP___HPP__

#include <vector>
#include <stdexcept>

class bitmap{
    private:
        std::vector<bool> underlying;

    public: 
        inline bitmap(std::size_t size): underlying(size, false) {}

        inline bool read(std::size_t idx) const { if(idx>= size()) throw std::out_of_range("Bitmap reading with index out of range"); return underlying.at(idx); }
        inline void write(std::size_t idx, bool value) { if(idx>= size()) throw std::out_of_range("Bitmap writing with index out of range"); underlying[idx]=value; }

        inline void reverse(std::size_t idx) { if(idx>= size()) throw std::out_of_range("Bitmap bit-reversing with index out of range"); write(idx, !read(idx)); }

        inline std::size_t size() const { return underlying.size(); }
};

#endif
