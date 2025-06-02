#ifndef __CONTAINERS__HPP___
#define __CONTAINERS__HPP___

/*
    OPTIMIZATION POSSIBILITIES:
    - The C++ standard offers much more opperations for common containers (as an example, the insertion of ranges could be included)
    - More containers, such as lists or hash tables could be included
*/
#include <functional>

#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <list>

template<typename T> class container{
    public:
        virtual bool is_empty() const = 0;
        virtual std::size_t size() const = 0;
};
template<typename T> class single_input_container: public container<T>{
    public:
        virtual void insert(const T &) = 0;
};
template<typename T> class single_output_container: public container<T>{
    public:
        virtual void extract() = 0;

        virtual const T &read() const = 0;
        virtual T &read() = 0;
};
template<typename T> class double_input_container: public container<T>{
    public:
        virtual void insert_front(const T &) = 0;
        virtual void insert_back(const T &) = 0;
};
template<typename T> class double_output_container: public container<T>{
    public:
        virtual void extract_front() = 0;
        virtual void extract_back() = 0;

        virtual const T &read_front() const = 0;
        virtual T read_front() = 0;

        virtual const T &read_back() const = 0;
        virtual T read_back() = 0;
};

template<typename T> class fifo_container: public single_input_container<T>, single_output_container<T>{            // Stack
    private:
        std::stack<T> underlying;

    public:
        inline void insert(const T &something) { underlying.push(something); }
        inline void extract() { underlying.pop(); }

        inline const T &read() const { return underlying.top(); }
        inline T &read() { return underlying.top(); }

        inline bool is_empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};
template<typename T> class lifo_container: public single_input_container<T>, single_output_container<T>{            // Queue
    private:
        std::queue<T> underlying;

    public:
        inline void insert(const T &something) { underlying.push(something); }
        inline void extract() { underlying.pop(); }

        inline const T &read() const { return underlying.front(); }
        inline T &read() { return underlying.front(); }

        inline bool is_empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};
template<typename T> class ordered_container: public single_input_container<T>, double_output_container<T>{         // Priority queue
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

        inline bool is_empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }

        inline bool contains(const T &element) const { return underlying.find(element)!=underlying.end(); }
        inline std::size_t count(const T &element) const { return underlying.count(element); }
};
template<typename T> class double_entry_container: public double_input_container<T>, double_output_container<T>{    // Deque
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

        inline bool is_empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
};

template<typename T> class iterable_container: public container<T>{
    public:
        class iterator{
            public:
                virtual const T &payload() const = 0;

                virtual void advance() = 0;
    
                virtual bool finished() const = 0;
        };
        class reading_iterator: public iterator{
            private:
                const iterable_container<T> &underlying_container;
        
            public:
                inline reading_iterator(const iterable_container<T> &container): underlying_container(container){}

                inline const iterable_container<T> &get_container() const { return underlying_container; }
        
                friend class iterable_container<T>;
        };
        class modifying_iterator: public iterator{
            private:
                iterable_container<T> &underlying_container;
        
            public:
                inline modifying_iterator(iterable_container<T> &container): underlying_container(container){}
        
                inline iterable_container<T> &get_container() { return underlying_container; }
        
                friend class iterable_container<T>;
        };
};

template<typename T> class list_container: public iterable_container<T>{           // List
    private:
        std::list<T> underlying;

    public:
        class forward_reading_iterator: public list_container<T>::reading_iterator{
            private:
                const typename std::list<T> &underlying_container;
                typename std::list<T>::const_iterator underlying_iterator;

                inline forward_reading_iterator(const list_container<T> &container): list_container<T>::reading_iterator(container),
                underlying_container(container.underlying), underlying_iterator(container.underlying.cbegin()){}
                
            public:
                inline const T &payload() const { return *underlying_iterator; }

                inline void advance() { ++underlying_iterator; }

                inline bool finished() const { return underlying_iterator==underlying_container.cend(); }

                friend class list_container<T>;
        };
        class backward_reading_iterator: public list_container<T>::reading_iterator{
            private:
                const typename std::list<T> &underlying_container;
                typename std::list<T>::const_reverse_iterator underlying_iterator;

                inline backward_reading_iterator(const list_container<T> &container): list_container<T>::reading_iterator(container),
                underlying_container(container.underlying), underlying_iterator(container.underlying.crbegin()){}

            public:
                inline const T &payload() const { return *underlying_iterator; }

                inline void advance() { ++underlying_iterator; }

                inline bool finished() const { return underlying_iterator==underlying_container.crend(); }

                friend class list_container<T>;
        };
        class forward_modifying_iterator: public list_container<T>::modifying_iterator{
            private:
                typename std::list<T> &underlying_container;
                typename std::list<T>::iterator underlying_iterator;
                
                inline forward_modifying_iterator(list_container<T> &container): list_container<T>::modifying_iterator(container),
                underlying_container(container.underlying), underlying_iterator(container.underlying.begin()){}

            public:
                inline T &payload() { return *underlying_iterator; }
                inline const T &payload() const { return *underlying_iterator; }

                inline void advance() { ++underlying_iterator; }

                inline bool finished() const { return underlying_iterator==underlying_container.end(); }

                inline void insert_ahead(const T &something) { underlying_iterator=underlying_container.insert(underlying_iterator, something); }
                inline void insert_behind(const T &something) { underlying_iterator=underlying_container.insert(std::next(underlying_iterator), something); }
                inline void extract() { underlying_iterator=underlying_container.erase(underlying_iterator); }

                friend class list_container<T>;
        };
        class backward_modifying_iterator: public list_container<T>::modifying_iterator{
            private:
                typename std::list<T> &underlying_container;
                typename std::list<T>::reverse_iterator underlying_iterator;

                inline backward_modifying_iterator(list_container<T> &container): list_container<T>::modifying_iterator(container),
                underlying_container(container.underlying), underlying_iterator(container.underlying.rbegin()){}

            public:
                inline T &payload() { return *underlying_iterator; }
                inline const T &payload() const { return *underlying_iterator; }

                inline void advance() { ++underlying_iterator; }

                inline bool finished() const { return underlying_iterator==underlying_container.rend(); }

                inline void insert_ahead(const T &something) {
                    underlying_iterator = std::reverse_iterator(std::next(underlying_container.insert(underlying_iterator.base(), something))); 
                }
                inline void insert_behind(const T &something) {
                    underlying_iterator = std::reverse_iterator(underlying_container.insert(underlying_iterator.base(), something)); 
                }
                inline void extract() { underlying_iterator=underlying_container.erase(underlying_iterator); }

                friend class list_container<T>;
        };
        
        inline void insert_front(const T &something) { underlying.push_front(something); }
        inline void insert_back(const T &something) { underlying.push_back(something); }

        inline void extract_front() { underlying.pop_front(); }
        inline void extract_back() { underlying.pop_back(); }

        inline const T &read_front() const { return underlying.front(); }
        inline T read_front() { return underlying.front(); }
        
        inline const T &read_back() const { return underlying.back(); }
        inline T read_back() { return underlying.back(); }

        inline bool is_empty() const { return underlying.empty(); }
        inline std::size_t size() const { return underlying.size(); }
        
        inline forward_reading_iterator get_forward_reading_iterator() const { return forward_reading_iterator(*this); }
        inline backward_reading_iterator get_backward_reading_iterator() const { return backward_reading_iterator(*this); }
        inline forward_modifying_iterator get_forward_modifying_iterator() { return forward_modifying_iterator(*this); }
        inline backward_modifying_iterator get_backward_modifying_iterator() { return backward_modifying_iterator(*this); }
};

#endif
