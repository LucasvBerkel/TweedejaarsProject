import argparse
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')

parser = argparse.ArgumentParser()
parser.add_argument("csv_file")
parser.add_argument("--png_file")
parser.add_argument("--dpi", type = int, default = 80)
parser.add_argument("--skiprows", type = int, default = 1)
parser.add_argument("--delimiter", default = ",")
parser.add_argument("--fields", type = lambda s: [item for item in s.split(',')], default = "meanq, average_reward,nr_games,meancost")
parser.add_argument("--figure_width", type = int, default = 16)
parser.add_argument("--figure_height", type = int, default = 9)
args = parser.parse_args()

# field definitions for numpy
dtype = [
  ("epoch", "int"),
  ("nr_games", "int"),
  ("average_reward", "float"),
  ("meanq", "float"),
  ("meancost", "float"),
]
data = np.loadtxt(args.csv_file, skiprows = args.skiprows, delimiter = args.delimiter, dtype = dtype)

print(data['nr_games'][0:2])
import sys
sys.exit(0)
# definitions for plot titles
labels = {
  "epoch": "Epoch",
  "phase": "Phase",
  "steps": "Number of steps",
  "nr_games": "Number of games",
  "average_reward": "Average reward",
  "min_game_reward": "Min. game reward",
  "max_game_reward": "Max. game reward",
  "last_exploration_rate": "Exploration rate",
  "total_train_steps": "Exploration steps",
  "replay_memory_count": "Replay memory size",
  "meanq": "Average Q-value",
  "meancost": "Average loss",
  "weight_updates": "Number of weight updates",
  "total_time": "Total time elapsed",
  "phase_time": "Phase time",
  "steps_per_second": "Number of steps per second"
}

# calculate number of subplots
nr_fields = len(args.fields)
cols = math.ceil(math.sqrt(nr_fields))
rows = math.ceil(nr_fields / float(cols))

plt.figure(figsize = (args.figure_width, args.figure_height))

# plot all fields
for i, field in enumerate(args.fields):
  plt.subplot(rows, cols, i + 1)

  # plt.plot(data['epoch'][train_idx], list(data[field][random_idx]) * len(data['epoch'][train_idx]))
  plt.plot(data['epoch'], list(data[field]))

  # plt.plot(data['epoch'][train_idx], data[field][train_idx])
  # plt.plot(data['epoch'][test_idx], data[field][test_idx])
  plt.legend(["Test"], loc = "best")
  plt.ylabel(labels[field])
  plt.xlabel(labels['epoch'])
  plt.title(labels[field])

plt.tight_layout()

# if png_file argument given, save to file
if args.png_file is not None:
  plt.savefig(args.png_file, dpi = args.dpi)
else:
  plt.show()
