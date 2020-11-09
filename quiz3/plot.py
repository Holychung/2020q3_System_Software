import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.use('Agg')

xpoints = []
ypoints_gcd = []
ypoints_gcd_fast = []
fp = open("./data_1_to_50000", "r")

while True:
	line = fp.readline()
	if not line:
		break
	tokens = line.split( )
	xpoints.append(int(tokens[0]))
	ypoints_gcd.append(int(tokens[1]))
	ypoints_gcd_fast.append(int(tokens[2]))
	if int(tokens[2]) > int(tokens[1]):
		print("{} {} {}".format(tokens[0], tokens[1], tokens[2]))

fp.close()

xpoints = np.array(xpoints)
ypoints_gcd = np.array(ypoints_gcd)
ypoints_gcd_fast = np.array(ypoints_gcd_fast)

plt.plot(xpoints, ypoints_gcd, '.', label = "gcd")
plt.plot(xpoints, ypoints_gcd_fast, '.', label = "faster")

plt.xscale('linear')
plt.yscale('linear')
plt.legend()

print("a")
plt.savefig('result_1_50000.png')
