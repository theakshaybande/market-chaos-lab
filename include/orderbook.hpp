#pragma once

#include "order.hpp"

#include <map>

class OrderBook {
public:
    explicit OrderBook(double initial_price = 100.0);

    void submitOrder(const Order& order);
    double lastPrice() const;
    double bestBid() const;
    double bestAsk() const;

private:
    using BidBook = std::map<double, int, std::greater<double>>;
    using AskBook = std::map<double, int, std::less<double>>;

    void matchBuy(Order incoming);
    void matchSell(Order incoming);

    BidBook bids_;
    AskBook asks_;
    double last_price_;
};
