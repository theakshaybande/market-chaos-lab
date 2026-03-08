#pragma once

#include <cstdint>

enum class Side {
    Buy,
    Sell
};

struct Order {
    std::uint64_t id;
    double price;
    int quantity;
    Side side;
};
