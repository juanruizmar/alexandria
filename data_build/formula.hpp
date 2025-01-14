#ifndef FORMULA__HPP___
#define FORMULA__HPP___

#include <list>

class symbol{};

class operation{};
class variable{};
class constant{};

class formula{
    private:
        symbol *root;
        std::list<formula> sub_formulae;

    public:
};

#endif
