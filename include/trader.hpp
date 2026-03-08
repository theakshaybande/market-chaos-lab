#pragma once

#include "order.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>

struct MarketSnapshot {
    double last_price;
    double prev_price;
    double best_bid;
    double best_ask;
    std::size_t step;
};

class Trader {
public:
    explicit Trader(std::uint32_t trader_id);
    virtual ~Trader() = default;

    virtual std::optional<Order> generateOrder(const MarketSnapshot& snapshot,
                                               std::uint64_t next_order_id,
                                               std::mt19937& rng) = 0;

protected:
    std::uint32_t trader_id_;
};

class RandomTrader final : public Trader {
public:
    explicit RandomTrader(std::uint32_t trader_id);

    std::optional<Order> generateOrder(const MarketSnapshot& snapshot,
                                       std::uint64_t next_order_id,
                                       std::mt19937& rng) override;
};

class MomentumTrader final : public Trader {
public:
    explicit MomentumTrader(std::uint32_t trader_id);

    std::optional<Order> generateOrder(const MarketSnapshot& snapshot,
                                       std::uint64_t next_order_id,
                                       std::mt19937& rng) override;
};

class MeanReversionTrader final : public Trader {
public:
    explicit MeanReversionTrader(std::uint32_t trader_id);

    std::optional<Order> generateOrder(const MarketSnapshot& snapshot,
                                       std::uint64_t next_order_id,
                                       std::mt19937& rng) override;
};
