import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.use('Agg')

xpoints = []
ypoints_naive = []
ypoints_bitwise = []
fp = open("./time.txt", "r")

index = 1
while True:
	line = fp.readline()
	if not line:
		break
	tokens = line.split( )
	xpoints.append(index)
	ypoints_naive.append(int(tokens[0]))
	ypoints_bitwise.append(int(tokens[1]))
	index = index + 1

fp.close()

xpoints = np.array(xpoints)
ypoints_naive = np.array(ypoints_naive)
ypoints_bitwise = np.array(ypoints_bitwise)

plt.plot(xpoints, ypoints_naive, '+c', label = "naive")
plt.plot(xpoints, ypoints_bitwise, 'xg', label = "bitwise")

plt.xscale('linear')
plt.yscale('linear')
plt.legend()
plt.ylabel('ns')
plt.ylabel('round')

plt.savefig('time.png')