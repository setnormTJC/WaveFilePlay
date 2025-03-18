import sys
import numpy as np
import matplotlib.pyplot as plt


# Check if the CSV filename argument is passed
if len(sys.argv) != 2:
    print("example of CORRECT way to call this file: python plotSoundWave.py csvFilename.csv")
    sys.exit(1)



# Get the CSV filename from the command line argument
csv_filename = sys.argv[1]

# Load the data from the CSV file
# Assume the CSV has columns: frequency, amplitude
data = np.loadtxt(csv_filename, delimiter=',', skiprows=0)

# Separate the columns into variables
times = data[:, 0]  # First column: frequencies
amplitudes = data[:, 1]  # Second column: amplitudes



# Plot the data
plt.plot(times, amplitudes)
plt.xlabel('Time (44.1K = 1 second)')
plt.ylabel('Amplitude')
plt.title('Sound wave data - ain\'t it lovely?')


#uncomment the two lines below if comparing orders of magnitude of frequencies 
# plt.yscale('log')
# plt.ylim(bottom=1)

# plt.xscale('log')

# plt.xlim(0, 5000)

plt.grid(True)
plt.show()