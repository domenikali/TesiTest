import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re

def create_violin_plot():
    """
    Reads all .csv files in the current directory, groups them by test case,
    and generates a combined violin plot with a 21:9 aspect ratio.
    """
    # --- 1. Discover and load data ---
    data_dir = '.'
    all_files = [f for f in os.listdir(data_dir) if f.endswith('.csv')]
    
    if not all_files:
        print("No .csv files found in the current directory.")
        return

    # Create a single DataFrame to hold all data
    df_list = []
    for filename in all_files:
        try:
            # Extract algorithm and case from filename (e.g., "new_mvm_mtd_16_4s_2l.csv")
            # This regex finds the last part that looks like "Xs_Yl" as the case name.
            match = re.search(r'(_\d+Sector_\d+Layer)\.csv$', filename)
            if not match:
                print(f"Skipping file with unexpected name format: {filename}")
                continue
            
            case_name = match.group(1).lstrip('_')
            algo_name = filename[:match.start()]

            # Read the data and add columns for algorithm and case
            temp_df = pd.read_csv(os.path.join(data_dir, filename))
            temp_df['algorithm'] = algo_name
            temp_df['case'] = case_name
            df_list.append(temp_df)
        except Exception as e:
            print(f"Could not process file {filename}: {e}")

    if not df_list:
        print("Failed to load data from any .csv file.")
        return

    full_df = pd.concat(df_list, ignore_index=True)

    # --- 2. Create the plot ---
    
    # Determine the order of cases and algorithms for consistent plotting
    case_order = sorted(full_df['case'].unique())
    algo_order = sorted(full_df['algorithm'].unique())
    
    # Use seaborn for easier grouped violin plots
    plt.style.use('seaborn-v0_8-whitegrid')
    # Set a wide 21:9 aspect ratio
    fig, ax = plt.subplots(figsize=(21, 9))

    sns.violinplot(data=full_df, x='case', y='duration_ns', hue='algorithm', 
                   order=case_order, hue_order=algo_order, ax=ax, inner='quartile')

    # --- 3. Customize the plot ---
    ax.set_title('MVM Algorithm Performance by Test Case O3 GCC optimisation', fontsize=18, pad=20)
    ax.set_xlabel('Test Case', fontsize=14)
    # Update Y-axis label to nanoseconds
    ax.set_ylabel('Execution Time (nanoseconds)', fontsize=14)
    ax.set_yscale('log') # Use a logarithmic scale for better visibility of variations

    # Improve legend
    ax.legend(title='Algorithm', bbox_to_anchor=(1.02, 1), loc='upper left')

    # Rotate x-axis labels for better readability
    plt.xticks(rotation=15, ha='right')
    
    fig.tight_layout()

    # --- 4. Save the plot ---
    output_filename = 'mvm_benchmark_violin_plot.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    print(f"Violin plot saved as '{output_filename}'")


def create_dispersion_plot():
    """
    Reads all .csv files in the current directory, groups them by test case,
    and generates a combined vertical dispersion plot.
    """
    # --- 1. Discover and load data ---
    data_dir = '.'
    all_files = [f for f in os.listdir(data_dir) if f.endswith('.csv')]
    
    if not all_files:
        print("No .csv files found in the current directory.")
        return

    # Create a single DataFrame to hold all data
    df_list = []
    for filename in all_files:
        try:
            # Extract algorithm and case from filename (e.g., "4_Threads_1Sector_1Layer.csv")
            match = re.search(r'(_\d+Sector_\d+Layer)\.csv$', filename)
            if not match:
                print(f"Skipping file with unexpected name format: {filename}")
                continue
            
            case_name = match.group(1).lstrip('_')
            algo_name = filename[:match.start()]

            # Read the data and add columns for algorithm and case
            temp_df = pd.read_csv(os.path.join(data_dir, filename))
            temp_df['algorithm'] = algo_name
            temp_df['case'] = case_name
            df_list.append(temp_df)
        except Exception as e:
            print(f"Could not process file {filename}: {e}")

    if not df_list:
        print("Failed to load data from any .csv file.")
        return

    full_df = pd.concat(df_list, ignore_index=True)

    # --- 2. Create the plot ---
    
    # Define a natural sorting key for test cases (e.g., 2 before 10)
    def natural_sort_key(s):
        return [int(text) if text.isdigit() else text.lower() for text in re.split('([0-9]+)', s)]

    # Determine the order of cases and define the specific algorithm order
    case_order = sorted(full_df['case'].unique(), key=natural_sort_key)
    algo_order = [
        "Naive_Flat_Buffer", 
        "Flat_Buffer_Cycle_Opt", 
        "Flat_Buffer_Cycle_Opt_Reg_Opt", 
        "4_Threads", 
        "8_Threads",
        "16_Threads"
    ]
    
    # Use seaborn for easier grouped plots
    plt.style.use('seaborn-v0_8-whitegrid')
    # Adjust aspect ratio for a vertical plot
    fig, ax = plt.subplots(figsize=(9, 16))

    # Create a vertical dispersion plot by swapping x and y
    sns.stripplot(data=full_df, y='case', x='duration_ns', hue='algorithm', 
                  order=case_order, hue_order=algo_order, ax=ax, 
                  jitter=True, dodge=True, alpha=0.7, size=4, orient='h')

    # --- 3. Customize the plot ---
    ax.set_title('MVM Algorithm Performance by Test Case (O3 GCC Optimisation)', fontsize=18, pad=20)
    ax.set_ylabel('Test Case', fontsize=14, labelpad=15)
    ax.set_xlabel('Execution Time (nanoseconds)', fontsize=14)
    ax.set_xscale('log') # Use a logarithmic scale for the time axis

    # Position legend below the plot
    ax.legend(title='Algorithm', loc='upper center', bbox_to_anchor=(0.5, -0.1), 
              ncol=3, fancybox=True, shadow=True, fontsize=12)

    # Adjust layout to prevent legend from being cut off
    fig.tight_layout(rect=[0, 0.1, 1, 1])

    # --- 4. Save the plot ---
    output_filename = 'mvm_benchmark_dispersion_plot_vertical.png'
    plt.savefig(output_filename, dpi=300)
    print(f"Vertical dispersion plot saved as '{output_filename}'")


if __name__ == "__main__":
    create_dispersion_plot()