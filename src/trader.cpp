#include "trader.hpp"

#include <algorithm>
#include <cmath>

namespace {
Order makeOrder(std::uint64_t id, double price, int quantity, Side side) {
    return Order{ id, std::max(0.01, price), std::max(1, quantity), side };
}
}

Trader::Trader(std::uint32_t trader_id)
    : trader_id_(trader_id) {}

RandomTrader::RandomTrader(std::uint32_t trader_id)
    : Trader(trader_id) {}

std::optional<Order> RandomTrader::generateOrder(const MarketSnapshot& snapshot,
                                                 std::uint64_t next_order_id,
                                                 std::mt19937& rng) {
    std::bernoulli_distribution active(0.75);
    if (!active(rng)) {
        return std::nullopt;
    }

    std::bernoulli_distribution side_dist(0.5);
    std::normal_distribution<double> px_noise(0.0, 0.45);
    std::uniform_int_distribution<int> qty_dist(1, 12);

    const Side side = side_dist(rng) ? Side::Buy : Side::Sell;
    const double anchor = (snapshot.best_bid + snapshot.best_ask) / 2.0;
    const double price = anchor + px_noise(rng);
    return makeOrder(next_order_id, price, qty_dist(rng), side);
}

MomentumTrader::MomentumTrader(std::uint32_t trader_id)
    : Trader(trader_id) {}

std::optional<Order> MomentumTrader::generateOrder(const MarketSnapshot& snapshot,
                                                   std::uint64_t next_order_id,
                                                   std::mt19937& rng) {
    const double delta = snapshot.last_price - snapshot.prev_price;
    if (std::abs(delta) < 1e-8) {
        return std::nullopt;
    }

    std::uniform_int_distribution<int> qty_dist(3, 15);
    std::normal_distribution<double> offset(0.18, 0.08);
    const bool uptrend = delta > 0.0;

    if (uptrend) {
        return makeOrder(next_order_id,
                         snapshot.last_price + std::max(0.02, offset(rng)),
                         qty_dist(rng),
                         Side::Buy);
    }

    return makeOrder(next_order_id,
                     snapshot.last_price - std::max(0.02, offset(rng)),
                     qty_dist(rng),
                     Side::Sell);
}

MeanReversionTrader::MeanReversionTrader(std::uint32_t trader_id)
    : Trader(trader_id) {}

std::optional<Order> MeanReversionTrader::generateOrder(const MarketSnapshot& snapshot,
                                                        std::uint64_t next_order_id,
                                                        std::mt19937& rng) {
    const double delta = snapshot.last_price - snapshot.prev_price;
    if (std::abs(delta) < 1e-8) {
        return std::nullopt;
    }

    std::uniform_int_distribution<int> qty_dist(2, 10);
    std::normal_distribution<double> offset(0.15, 0.06);

    if (delta > 0.0) {
        return makeOrder(next_order_id,
                         snapshot.last_price - std::max(0.02, offset(rng)),
                         qty_dist(rng),
                         Side::Sell);
    }

    return makeOrder(next_order_id,
                     snapshot.last_price + std::max(0.02, offset(rng)),
                     qty_dist(rng),
                     Side::Buy);
}
