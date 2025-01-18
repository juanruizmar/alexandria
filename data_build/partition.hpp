#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <cassert>

#include <vector>

class union_find{
    private:
        mutable std::vector<size_t> p;

        inline size_t find(size_t i) const { assert(i<size()); 
            size_t r=i, aux;
            while(r!=p[r]) r=p[r];
            while(i!=p[i]) { aux=p[i]; p[i]=r; i=aux; }
            return r;
        }

    public:
        inline union_find(size_t n): p(n,0) { for(size_t i=0; i<n; ++i) p[i]=i; }

        inline void unite(size_t i, size_t j) { assert(i<size()); assert(j<size()); p[find(i)]=find(j); }

        inline bool are_united(size_t i, size_t j) const { assert(i<size()); assert(j<size()); return find(i)==find(j); }

        inline size_t size() const { return p.size(); }
};

#endif