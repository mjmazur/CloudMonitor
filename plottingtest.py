import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from scipy import interpolate
from datetime import datetime

data = pd.read_csv('test.csv')
data.columns = ['timestamp','SkyT','GroundT']

t = data['timestamp']
y = data['SkyT']-data['GroundT']

# Convert timestamp to hours and put in a column in data
data['timeh'] = 99

for i in range(len(data.index)):
	t2 = int(datetime.fromtimestamp(data['timestamp'][i]).strftime('%H')) + float(datetime.fromtimestamp(data['timestamp'][i]).strftime('%M'))/60 + float(datetime.fromtimestamp(data['timestamp'][i]).strftime('%S'))/3600
	data.loc[i,'timeh'] = t2

# Arrange the data into blocks and take the mean of each block
samplerate = 5
blocksize = 900*4
step = int(blocksize/samplerate)

f = interpolate.interp1d(t,y)
xnew = np.arange(min(data['timestamp']),max(data['timestamp']),step)
ynew = f(xnew)

interval = 1

n = int(len(ynew))

a = ynew[0:(n-1)*interval].reshape(1,interval,n-1)
block = np.mean(a, axis=1)

### Plotting Section

plt.figure(figsize = (10,6))
gs1 = gridspec.GridSpec(2, 1, width_ratios=[1], height_ratios=[1,8])
gs1.update(wspace=0.025, hspace=0.0) # set the spacing between axes. 

ax0 = plt.subplot(gs1[0])
ax0.pcolor(block,cmap='Blues_r', vmin=-25, vmax=-6)
ax0.set_xticklabels([])
ax0.set_xticks([])
ax0.set_yticklabels([])
ax0.set_yticks([])
plt.title('Cloud Cover', size=20)

ax1 = plt.subplot(gs1[1])
ax1.plot(data['timeh'], y, color='black')

ax1.set_xlim(min(data['timeh']), max(data['timeh']))
ax1.set_xlabel('Local Time', size=15)
ax1.set_ylabel('Sky Temp minus Ground Temp (*C)', size=15)

plt.tight_layout()
plt.show()