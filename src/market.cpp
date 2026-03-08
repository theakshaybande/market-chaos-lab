#include "market.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

Market::Market(double initial_price, std::uint32_t seed)
    : orderbook_(initial_price),
      rng_(seed),
      next_order_id_(1U) {
    price_series_.push_back(initial_price);
}

void Market::addTrader(std::unique_ptr<Trader> trader) {
    traders_.push_back(std::move(trader));
}

void Market::run(std::size_t steps) {
    for (std::size_t step = 0; step < steps; ++step) {
        const MarketSnapshot s = snapshot(step);

        for (const auto& trader : traders_) {
            if (auto maybe_order = trader->generateOrder(s, next_order_id_, rng_)) {
                ++next_order_id_;
                orderbook_.submitOrder(*maybe_order);
            }
        }

        price_series_.push_back(orderbook_.lastPrice());
    }
}

void Market::writePriceSeriesCsv(const std::string& filepath) const {
    const std::filesystem::path out_path(filepath);
    if (out_path.has_parent_path()) {
        std::filesystem::create_directories(out_path.parent_path());
    }

    std::ofstream out(filepath, std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Unable to write CSV file: " + filepath);
    }

    out << "step,price\n";
    for (std::size_t i = 0; i < price_series_.size(); ++i) {
        out << i << ',' << price_series_[i] << '\n';
    }
}

const std::vector<double>& Market::priceSeries() const {
    return price_series_;
}

MarketSnapshot Market::snapshot(std::size_t step) const {
    const double last = orderbook_.lastPrice();
    const double prev = price_series_.size() >= 2 ? price_series_[price_series_.size() - 2] : last;

    return MarketSnapshot{
        .last_price = last,
        .prev_price = prev,
        .best_bid = orderbook_.bestBid(),
        .best_ask = orderbook_.bestAsk(),
        .step = step
    };
}
