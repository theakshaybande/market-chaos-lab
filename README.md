# market-chaos-lab

A C++20 market simulation that models a simple order-driven market where heterogeneous traders interact through a shared limit order book.

## What this models (market microstructure)

The simulator uses a basic limit order book with two sides:

- `bids`: buy interest sorted from highest to lowest price
- `asks`: sell interest sorted from lowest to highest price

Each trader submits limit orders containing `id`, `price`, `quantity`, and `side`.

Matching logic:

- A buy order matches against the best ask while `buy_price >= best_ask`
- A sell order matches against the best bid while `sell_price <= best_bid`
- Trades execute at the resting order price (top of book)
- Unfilled quantity rests on the book as liquidity
- `last_price` updates on every trade

## Trader types and emergent behavior

The market includes three strategy archetypes:

- `RandomTrader`: provides noisy flow around the current mid-price
- `MomentumTrader`: follows short-term direction and amplifies trends
- `MeanReversionTrader`: fades short-term moves and adds stabilizing pressure

Because these agents interact through shared liquidity, price dynamics emerge from feedback loops:

- Trend bursts can appear when momentum flow dominates
- Reversals occur when mean-reversion flow absorbs directional pressure
- Spread/liquidity states influence execution frequency and volatility

This is intentionally simplified but still useful for experimentation with agent composition, order intensity, and microstructure effects.

## Build and run

### 1. Configure and build

```bash
cmake -S . -B build
cmake --build build
```

### 2. Run simulation

```bash
./build/market-chaos-lab
```

On Windows (PowerShell):

```powershell
.\build\market-chaos-lab.exe
```

This runs `10,000` steps and writes:

- `results/price_series.csv`

### 3. Plot the generated series

Install plotting dependencies if needed:

```bash
pip install pandas matplotlib
```

Then run:

```bash
python scripts/plot.py
```

Output:

- `results/price_series.png`

## Project layout

- `include/order.hpp`
- `include/orderbook.hpp`
- `include/trader.hpp`
- `include/market.hpp`
- `src/orderbook.cpp`
- `src/trader.cpp`
- `src/market.cpp`
- `src/main.cpp`
- `scripts/plot.py`
- `results/`
