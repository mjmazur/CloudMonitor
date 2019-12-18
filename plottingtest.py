import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from scipy import interpolate

data = np.genfromtxt('test.csv', delimiter=',')
print(len(data))
print(data.shape[0])

x = data[:,0]
y = data[:,1]-data[:,2]

step = 200

f = interpolate.interp1d(x,y)
xnew = np.arange(min(data[:,0]),max(data[:,0]),step)
ynew = f(xnew)

interval = 1

n = int(len(ynew))

a = ynew[0:(n-1)*interval].reshape(1,interval,n-1)

plt.figure(figsize = (10,6))
gs1 = gridspec.GridSpec(2, 1, width_ratios=[1], height_ratios=[1,8])
gs1.update(wspace=0.025, hspace=0.0) # set the spacing between axes. 

ax0 = plt.subplot(gs1[0])
ax0.pcolor(np.mean(a, axis=1),cmap='Blues_r')
ax0.set_xticklabels([])
ax0.set_xticks([])
ax0.set_yticklabels([])
ax0.set_yticks([])

ax1 = plt.subplot(gs1[1])
ax1.plot(data[0:n*step-1,0], data[0:n*step-1,1]-data[0:n*step-1,2], color='black')
ax1.set_xlim(min(data[0:n*step-1,0]), max(data[0:n*step-1,0]))
ax1.set_xlabel('Time', size=15)
ax1.set_ylabel('Sky Temp minus Ground Temp (*C)', size=15)

plt.tight_layout()
plt.show()