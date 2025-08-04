import argparse
import matplotlib.pyplot as plt
import os
import pandas as pd
import sys

lines = []
lined = dict()

def on_pick(event):
    # on the pick event, find the orig line corresponding to the
    # legend proxy line, and toggle the visibility
    leg_line = event.artist
    if len(lined) == 1:
        orig_line = leg_line
    else:
        orig_line = lined[legline]
    vis = not orig_line.get_visible()
    orig_line.set_visible(vis)
    # Change the alpha on the line in the legend so we can see what lines
    # have been toggled
    if vis:
        leg_line.set_alpha(1.0)
    else:
        leg_line.set_alpha(0.2)
    plt.draw()

# Function to read data and plot
def plot_data(file_paths, title, xlabel, ylabel, plot_marker):
    plt.figure(figsize=(10, 5))

    for file_path in file_paths:
        # Read the data from the .dat file
        try:
            data = pd.read_csv(file_path, sep='\t', header=None, names=['Time', 'Value'])
            base_name = os.path.splitext(os.path.basename(file_path))[0]
            marker_style = ''
            if plot_marker == True:
                marker_style = 'o'
            line, = plt.plot(data['Time'], data['Value'], marker=marker_style, linestyle='-', label=base_name, picker=True)
            lines.append(line)  # Store the line for toggling
        except Exception as e:
            print(f"Error reading the file {file_path}: {e}")
            continue

    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.grid()
    legends = plt.legend(loc='upper right', fontsize='medium',labelspacing=1.0)
    
    for leg_line, orig_line in zip(legends.get_lines(), lines):
        leg_line.set_picker(5)  # 5 pts tolerance
        lined[leg_line] = orig_line

    # Connect the pick event
    plt.gcf().canvas.mpl_connect('pick_event', on_pick)
    plt.show()

# Set up argument parsing
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Plot data from multiple .dat files.')
    parser.add_argument('file_paths', type=str, nargs='+', help='Paths to the .dat files')
    parser.add_argument('title', type=str, help='Title of the plot')
    parser.add_argument('xlabel', type=str, help='Label for the x-axis')
    parser.add_argument('ylabel', type=str, help='Label for the y-axis')
    parser.add_argument('--plot_markers', help='Plot markers points', action=argparse.BooleanOptionalAction)

    args = parser.parse_args()

    # Check if the required arguments are provided
    if len(args.file_paths) < 1:
        print("Error: At least one file path must be provided.")
        parser.print_help()
        sys.exit(1)

    plot_data(args.file_paths, args.title, args.xlabel, args.ylabel, args.plot_markers)
