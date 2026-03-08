#include "market.hpp"
#include "trader.hpp"

#include <iostream>
#include <memory>

int main() {
    try {
        Market market(100.0, 42U);

        std::uint32_t id = 1;
        for (int i = 0; i < 14; ++i) {
            market.addTrader(std::make_unique<RandomTrader>(id++));
        }
        for (int i = 0; i < 8; ++i) {
            market.addTrader(std::make_unique<MomentumTrader>(id++));
        }
        for (int i = 0; i < 8; ++i) {
            market.addTrader(std::make_unique<MeanReversionTrader>(id++));
        }

        constexpr std::size_t kSteps = 10000;
        market.run(kSteps);
        market.writePriceSeriesCsv("results/price_series.csv");

        std::cout << "Simulation complete. Wrote results/price_series.csv with "
                  << market.priceSeries().size() << " points.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}
