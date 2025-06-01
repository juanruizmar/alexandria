#ifndef UUID_GENERATOR_HPP_250601
#define UUID_GENERATOR_HPP_250601

#include <string>
#include <random>

#include "rand_generator.hpp"

typedef std::string uuid;

class identifiable {
    private:
        uuid id_;

    public:
        identifiable(): id_(uuid_generator().get()) {}

        const uuid& get_id() const { return id_; }

        inline bool operator==(const identifiable &other) const { return id_ == other.id_; }
        inline bool operator<(const identifiable &other) const { return id_ < other.id_; }
};

#endif // UUID_GENERATOR_HPP_250601