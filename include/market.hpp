#pragma once

#include "orderbook.hpp"
#include "trader.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <vector>

class Market {
public:
    explicit Market(double initial_price = 100.0, std::uint32_t seed = 42U);

    void addTrader(std::unique_ptr<Trader> trader);
    void run(std::size_t steps);
    void writePriceSeriesCsv(const std::string& filepath) const;

    const std::vector<double>& priceSeries() const;

private:
    MarketSnapshot snapshot(std::size_t step) const;

    OrderBook orderbook_;
    std::vector<std::unique_ptr<Trader>> traders_;
    std::vector<double> price_series_;
    mutable std::mt19937 rng_;
    std::uint64_t next_order_id_;
};
