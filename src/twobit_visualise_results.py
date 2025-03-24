#!/usr/bin/env python3
import subprocess
import matplotlib.pyplot as plt
import re
import sys

# Constants
SIMULATOR_PATH = "./branch_sim_TwoBit"
TRACE_FILE_PATH = "../misc/block_profile"
BTB_SIZES = [2,4,8,16,32,64,128,256,512,1024,2048, 5096, 10192]

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
        static_match = re.search(r"Static Accuracy: (\d+\.\d+)%", output)
        dynamic_match = re.search(r"Dynamic Accuracy: (\d+\.\d+)%", output)

        if static_match and dynamic_match:
            static_acc = float(static_match.group(1))
            dynamic_acc = float(dynamic_match.group(1))

            return {
                "static_accuracy": static_acc,
                "dynamic_accuracy": dynamic_acc,
            }
        else:
            print("Error: Could not find Static or Dynamic accuracy in output.")
            return None

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
            print(f"  Static Accuracy: {stats['static_accuracy']:.2f}%, Dynamic Accuracy: {stats['dynamic_accuracy']:.2f}%")
        else:
            print(f"  Failed to parse results for BTB size {btb_size}")

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
    plt.plot(btb_sizes, static_accuracy, 'b-o', linewidth=2, markersize=8, label='Static Accuracy')
    plt.plot(btb_sizes, dynamic_accuracy, 'r-s', linewidth=2, markersize=8, label='Dynamic Accuracy')

    # Add grid for better readability
    plt.grid(True, linestyle='--', alpha=0.7)

    # Title and legend
    plt.title('Static vs. Dynamic Prediction Accuracy by BTB Size', fontsize=14)
    plt.legend(loc='upper left')

    # Show x-ticks at each BTB size with logarithmic x-axis
    plt.xscale('log', base=2)
    plt.xticks(btb_sizes, labels=[str(size) for size in btb_sizes])

    # Add data labels to each point
    for i, (static, dynamic) in enumerate(zip(static_accuracy, dynamic_accuracy)):
        plt.text(btb_sizes[i], static + 1, f"{static:.1f}%", ha='center', va='bottom', fontsize=9)
        plt.text(btb_sizes[i], dynamic - 2, f"{dynamic:.1f}%", ha='center', va='top', fontsize=9)

    plt.xlabel('BTB Size (entries)', fontsize=12)
    plt.ylabel('Prediction Accuracy (%)', fontsize=12)

    #Adjust the y axis scale to be more appropriate for the data.
    plt.ylim(min(min(static_accuracy), min(dynamic_accuracy)) - 5, max(max(static_accuracy), max(dynamic_accuracy)) + 5)

    plt.tight_layout()

    # Save figure
    plt.savefig('../misc/branch_predictionTwoBit_accuracies.png', dpi=300)
    print("Plot saved as 'branch_predictionTwoBit_accuracies.png'")

    plt.show()

if __name__ == "__main__":
    main()