#!/usr/bin/env python3

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def main() -> None:
    root = Path(__file__).resolve().parent.parent
    csv_path = root / "results" / "price_series.csv"
    png_path = root / "results" / "price_series.png"

    if not csv_path.exists():
        raise FileNotFoundError(f"CSV not found: {csv_path}")

    df = pd.read_csv(csv_path)

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(11, 5.5))
    ax.plot(df["step"], df["price"], color="#0F766E", linewidth=1.2)
    ax.set_title("Market Chaos Lab Price Series", fontsize=14)
    ax.set_xlabel("Step")
    ax.set_ylabel("Price")

    fig.tight_layout()
    fig.savefig(png_path, dpi=150)
    print(f"Saved plot to {png_path}")


if __name__ == "__main__":
    main()
