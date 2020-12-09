import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.use('Agg')

xpoints = []
ypoints_for = []
ypoints_memcpy = []
ypoints_vm = []
fp = open("./time.txt", "r")

index = 1
while True:
	line = fp.readline()
	if not line:
		break
	tokens = line.split( )
	xpoints.append(index)
	ypoints_for.append(int(tokens[1]))
	ypoints_memcpy.append(int(tokens[2]))
	ypoints_vm.append(int(tokens[3]))
	index = index + 1

fp.close()

xpoints = np.array(xpoints)
ypoints_for = np.array(ypoints_for)
ypoints_memcpy = np.array(ypoints_memcpy)
ypoints_vm = np.array(ypoints_vm)

plt.plot(xpoints, ypoints_for, '+c', label = "for")
plt.plot(xpoints, ypoints_memcpy, 'xg', label = "memcpy")
plt.plot(xpoints, ypoints_vm, '+b', label = "VAS mapping")

plt.xscale('linear')
plt.yscale('linear')
plt.legend()
plt.ylabel('ns')
plt.xlabel('round')

plt.savefig('time.png')