#!/usr/bin/env python3
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import re
import os

# Some variables to be used
SIMULATOR_PATH = "./branch_sim"
TRACE_FILE_PATH = "../misc/block_profile"
# Range of BTB sizes to test
BTB_SIZES = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]


def run_simulation(btb_size):
    """Run the branch predictor with a specific BTB size and return the results."""
    try:
        result = subprocess.run(
            [SIMULATOR_PATH, TRACE_FILE_PATH, str(btb_size)],
            capture_output=True,
            text=True,
            check=True
        )
        return result.stdout
    except subprocess.CalledProcessError as e:
        print(f"Error running simulation with BTB size {btb_size}: {e}")
        print(f"Error output: {e.stderr}")
        return None

def parse_results(output):
    """Parse the simulation output to extract relevant statistics."""
    if not output:
        return None

    # Extract the statistics using regex
    try:
        total_instructions = int(re.search(r"Total instructions processed: (\d+)", output).group(1))
        btb_hits = int(re.search(r"BTB hits: (\d+)", output).group(1))
        btb_misses = int(re.search(r"BTB misses: (\d+)", output).group(1))

        # Assuming in this case that each btb miss will cause an additional miss
        instructions_executed = total_instructions
        instructions_fetched = total_instructions + btb_misses  # Assume each miss causes an extra fetch

        overhead = instructions_fetched / instructions_executed if instructions_executed > 0 else 0

        return {
            "total_instructions": total_instructions,
            "btb_hits": btb_hits,
            "btb_misses": btb_misses,
            "overhead": overhead,
            "btb_hit_rate": btb_hits / (btb_hits + btb_misses) if (btb_hits + btb_misses) > 0 else 0
        }
    except (AttributeError, ValueError) as e:
        print(f"Error parsing results: {e}")
        print(f"Output was: {output}")
        return None

def main():
    results = []

    print("Running simulations with different BTB sizes...")
    for btb_size in BTB_SIZES:
        print(f"Testing BTB size: {btb_size}")
        output = run_simulation(btb_size)
        stats = parse_results(output)
        if stats:
            stats["btb_size"] = btb_size
            results.append(stats)
            print(f"  Overhead: {stats['overhead']:.4f}, Hit rate: {stats['btb_hit_rate']:.4f}")

    if not results:
        print("No valid results to plot.")
        return

    # Extract data for plotting
    btb_sizes = [r["btb_size"] for r in results]
    overheads = [r["overhead"] * 100 for r in results] # convert to percentage
    hit_rates = [r["btb_hit_rate"] for r in results]

    # Create the plot
    plt.figure(figsize=(10, 6))

    # Plot overhead vs BTB size
    plt.subplot(1, 2, 1)
    plt.plot(btb_sizes, overheads, 'o-', color='orange', label='Overhead')
    plt.axhline(y=100, color='gray', linestyle='--', alpha=0.7) # 100% line
    plt.xlabel('BTB Size')
    plt.ylabel('Overhead (%)') # Updated label
    plt.title('BTB Size vs. Overhead')
    plt.grid(True, alpha=0.3)
    plt.xscale('log', base=2)  # Log scale makes it easier to see the pattern
    plt.xticks(btb_sizes, [str(s) for s in btb_sizes])
    plt.legend()

    # For curiousity also plotting the relationship between hit rate and btb size

    # Plot hit rate vs BTB size
    plt.subplot(1, 2, 2)
    plt.plot(btb_sizes, hit_rates, 'o-', color='blue', label='BTB Hit Rate')
    plt.xlabel('BTB Size')
    plt.ylabel('BTB Hit Rate')
    plt.title('BTB Size vs. Hit Rate')
    plt.grid(True, alpha=0.3)
    plt.xscale('log', base=2)
    plt.xticks(btb_sizes, [str(s) for s in btb_sizes])
    plt.ylim(0, 1.05)
    plt.legend()

    plt.tight_layout()
    plt.savefig('../misc/btb_size_vs_overhead.png', dpi=300)
    print(f"Plot saved as 'btb_size_vs_overhead.png'")
    plt.show()

if __name__ == "__main__":
    main()