#!/usr/bin/env python3
import subprocess
import matplotlib.pyplot as plt
import re
import sys

# Constants
SIMULATOR_PATH = "./branch_sim_TwoBit"
TRACE_FILE_PATH = "../misc/block_profile"
BTB_SIZES = [10, 32, 64, 100, 128, 256, 512]

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

    try:
        # Extract the values directly using more flexible regex patterns
        static_acc = float(re.search(r"Static.*accuracy:\s+(\d+\.\d+)%", output, re.DOTALL).group(1))
        dynamic_acc = float(re.search(r"Two-Level.*accuracy:\s+(\d+\.\d+)%", output, re.DOTALL).group(1))
        improvement = float(re.search(r"Improvement over static prediction:\s+(\d+\.\d+)%", output).group(1))
        btb_hit_rate = float(re.search(r"BTB hit rate:\s+(\d+\.\d+)%", output).group(1))

        return {
            "static_accuracy": static_acc,
            "dynamic_accuracy": dynamic_acc,
            "improvement": improvement,
            "btb_hit_rate": btb_hit_rate
        }
    except (AttributeError, ValueError) as e:
        print(f"Error parsing results: {e}")
        return None

def main():
    results = []

    # Option 1: For testing, use hardcoded values from the output
    # This is useful if the simulator isn't available or to debug the plotting
    hardcoded_results = [
        {"btb_size": 10, "static_accuracy": 50.93, "dynamic_accuracy": 68.68, "improvement": 17.75, "btb_hit_rate": 50.07},
        {"btb_size": 32, "static_accuracy": 50.93, "dynamic_accuracy": 73.35, "improvement": 22.42, "btb_hit_rate": 74.03},
        {"btb_size": 64, "static_accuracy": 50.93, "dynamic_accuracy": 77.42, "improvement": 26.48, "btb_hit_rate": 79.75},
        {"btb_size": 100, "static_accuracy": 50.93, "dynamic_accuracy": 77.55, "improvement": 26.62, "btb_hit_rate": 87.04},
        {"btb_size": 128, "static_accuracy": 50.93, "dynamic_accuracy": 80.44, "improvement": 29.51, "btb_hit_rate": 90.69},
        {"btb_size": 256, "static_accuracy": 50.93, "dynamic_accuracy": 83.92, "improvement": 32.99, "btb_hit_rate": 96.92},
        {"btb_size": 512, "static_accuracy": 50.93, "dynamic_accuracy": 86.81, "improvement": 35.88, "btb_hit_rate": 98.76}
    ]

    # Option 2: Run simulations (uncomment this block to run simulations instead of using hardcoded values)
    """
    print("Running simulations with different BTB sizes...")
    for btb_size in BTB_SIZES:
        print(f"Testing BTB size: {btb_size}")
        output = run_simulation(btb_size)
        stats = parse_results(output)
        if stats:
            stats["btb_size"] = btb_size
            results.append(stats)
            print(f"  Static Accuracy: {stats['static_accuracy']:.2f}%, Dynamic Accuracy: {stats['dynamic_accuracy']:.2f}%")
            print(f"  Improvement: {stats['improvement']:.2f}%, BTB Hit Rate: {stats['btb_hit_rate']:.2f}%")
        else:
            print(f"  Failed to parse results for BTB size {btb_size}")
    """

    # Using the hardcoded results for now
    results = hardcoded_results

    if not results:
        print("No valid results to plot.")
        return

    # Extract data for plotting
    btb_sizes = [r["btb_size"] for r in results]
    static_accuracy = [r["static_accuracy"] for r in results]
    dynamic_accuracy = [r["dynamic_accuracy"] for r in results]

    # Create figure
    plt.figure(figsize=(10, 6))

    # Plot accuracies
    plt.xlabel('BTB Size (entries)', fontsize=12)
    plt.ylabel('Prediction Accuracy (%)', fontsize=12)

    # Use distinct colors and markers for better visibility
    plt.plot(btb_sizes, static_accuracy, 'b-o', linewidth=2, markersize=8, label='Static Prediction')
    plt.plot(btb_sizes, dynamic_accuracy, 'r-s', linewidth=2, markersize=8, label='Two-Level Prediction')

    # Add grid for better readability
    plt.grid(True, linestyle='--', alpha=0.7)

    # Set y-axis range to highlight differences
    plt.ylim([45, 90])  # Adjusted based on the actual data values

    # Title and legend
    plt.title('Branch Prediction Accuracy: Static vs. Two-Level', fontsize=14)
    plt.legend(loc='upper left')

    # Show x-ticks at each BTB size with logarithmic x-axis to better show the relationship
    plt.xscale('log', base=2)
    plt.xticks(btb_sizes, labels=[str(size) for size in btb_sizes])

    # Add data labels to each point
    for i, (static, dynamic) in enumerate(zip(static_accuracy, dynamic_accuracy)):
        plt.text(btb_sizes[i], static - 2, f"{static:.1f}%", ha='center', va='top', fontsize=9)
        plt.text(btb_sizes[i], dynamic + 2, f"{dynamic:.1f}%", ha='center', va='bottom', fontsize=9)

    plt.tight_layout()

    # Save figure
    plt.savefig('../misc/branch_predictionTwoBit_comparison.png', dpi=300)
    print("Plot saved as 'branch_prediction_comparison.png'")

    plt.show()

if __name__ == "__main__":
    main()