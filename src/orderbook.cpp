#include "orderbook.hpp"

#include <algorithm>

OrderBook::OrderBook(double initial_price)
    : last_price_(initial_price) {}

void OrderBook::submitOrder(const Order& order) {
    if (order.quantity <= 0 || order.price <= 0.0) {
        return;
    }

    if (order.side == Side::Buy) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

double OrderBook::lastPrice() const {
    return last_price_;
}

double OrderBook::bestBid() const {
    if (bids_.empty()) {
        return last_price_;
    }
    return bids_.begin()->first;
}

double OrderBook::bestAsk() const {
    if (asks_.empty()) {
        return last_price_;
    }
    return asks_.begin()->first;
}

void OrderBook::matchBuy(Order incoming) {
    while (incoming.quantity > 0 && !asks_.empty()) {
        auto best_ask = asks_.begin();
        if (best_ask->first > incoming.price) {
            break;
        }

        const int traded = std::min(incoming.quantity, best_ask->second);
        incoming.quantity -= traded;
        best_ask->second -= traded;
        last_price_ = best_ask->first;

        if (best_ask->second == 0) {
            asks_.erase(best_ask);
        }
    }

    if (incoming.quantity > 0) {
        bids_[incoming.price] += incoming.quantity;
    }
}

void OrderBook::matchSell(Order incoming) {
    while (incoming.quantity > 0 && !bids_.empty()) {
        auto best_bid = bids_.begin();
        if (best_bid->first < incoming.price) {
            break;
        }

        const int traded = std::min(incoming.quantity, best_bid->second);
        incoming.quantity -= traded;
        best_bid->second -= traded;
        last_price_ = best_bid->first;

        if (best_bid->second == 0) {
            bids_.erase(best_bid);
        }
    }

    if (incoming.quantity > 0) {
        asks_[incoming.price] += incoming.quantity;
    }
}
