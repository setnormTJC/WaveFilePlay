import sys
import numpy as np
import matplotlib.pyplot as plt


# Check if the CSV filename argument is passed
if len(sys.argv) != 2:
    print("example of CORRECT way to call this file: python plotFourierTransform.py csvFilename.csv")
    sys.exit(1)


# Get the CSV filename from the command line argument
csv_filename = sys.argv[1]

# Load the data from the CSV file
# Assume the CSV has columns: frequency, amplitude
data = np.loadtxt(csv_filename, delimiter=',', skiprows=0)

# Separate the columns into variables
frequencies = data[:, 0]  # First column: frequencies
amplitudes = data[:, 1]  # Second column: amplitudes



# Plot the data
plt.plot(frequencies, amplitudes)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title('Fourier Transform')


#uncomment the two lines below if comparing orders of magnitude of frequencies 
# plt.yscale('log')
# plt.ylim(bottom=1)

# plt.xscale('log')

plt.xlim(0, 5000)

plt.grid(True)
plt.show()