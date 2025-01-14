#ifndef __CONTAINERS__HPP___
#define __CONTAINERS__HPP___

/*
    OPTIMIZATION POSSIBILITIES:
    - The C++ standard offers much more opperations for common containers (as an example, the insertion of ranges could be included)
    - More containers, such as lists or hash tables could be included
*/

#include <stack>
#include <queue>
#include <deque>
#include <set>

template<typename T> class container{
    public:
        virtual ~container() {};

        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
};
template<typename T> class single_input_container: public container<T>{
    public:
        virtual ~single_input_container() {}

        virtual void insert(const T &) = 0;
};
template<typename T> class single_output_container: public container<T>{
    public:
        virtual ~single_output_container() {}

        virtual void extract() = 0;

        virtual const T &read() const = 0;
        virtual T &read() = 0;
};
template<typename T> class double_input_container: public container<T>{
    public:
        virtual ~double_input_container() {}

        virtual void insert_front(const T &) = 0;
        virtual void insert_back(const T &) = 0;
};
template<typename T> class double_output_container: public container<T>{
    public:
        virtual ~double_output_container() {}

        virtual void extract_front() = 0;
        virtual void extract_back() = 0;

        virtual const T &read_front() const = 0;
        virtual T read_front() = 0;

        virtual const T &read_back() const = 0;
        virtual T read_back() = 0;
};

template<typename T> class fifo_container: public single_input_container<T>, single_output_container<T>{
    private:
        std::stack<T> underlying;

    public:
        inline void insert(const T &something) { underlying.push(something); }
        inline void extract() { underlying.pop(); }

        inline const T &read() const { return underlying.top(); }
        inline T &read() { return underlying.top(); }

        inline bool empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};
template<typename T> class lifo_container: public single_input_container<T>, single_output_container<T>{
    private:
        std::queue<T> underlying;

    public:
        inline void insert(const T &something) { underlying.push(something); }
        inline void extract() { underlying.pop(); }

        inline const T &read() const { return underlying.front(); }
        inline T &read() { return underlying.front(); }

        inline bool empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};
template<typename T> class ordered_container: public single_input_container<T>, double_output_container<T>{
    private:
        std::multiset<T> underlying;

    public:
        inline void insert(const T &something) { underlying.insert(something); }

        inline void extract_front() { underlying.erase(underlying.begin()); }
        inline void extract_back() { underlying.erase(std::prev(underlying.end())); }

        inline const T &read_front() const { return *(underlying.cbegin()); }
        inline T read_front() { return *(underlying.begin()); }

        inline const T &read_back() const { return *underlying.rbegin(); }
        inline T read_back() { return *underlying.crbegin(); }

        inline bool empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }

        inline bool contains(const T &element) const { return underlying.find(element)!=underlying.end(); }
        inline std::size_t count(const T &element) const { return underlying.count(element); }
};
template<typename T> class double_entry_container: public double_input_container<T>, double_output_container<T>{
    private:
        std::deque<T> underlying;

    public:
        inline void insert_front(const T &something) { underlying.push_front(something); }
        inline void insert_back(const T &something) { underlying.push_back(something); }

        inline void extract_front() { underlying.pop_front(); }
        inline void extract_back() { underlying.pop_back(); }

        inline const T &read_front() const { return underlying.front(); }
        inline T read_front() { return underlying.front(); }

        inline const T &read_back() const { return underlying.back(); }
        inline T read_back() { return underlying.back(); }

        inline bool empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};

#endif