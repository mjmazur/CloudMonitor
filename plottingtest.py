from matplotlib.pylab import *
from numpy import arange

pcolor(arange(20).reshape(1,20))
cb = colorbar(label='a label')
ax = cb.ax
text = ax.yaxis.label
font = matplotlib.font_manager.FontProperties(family='times new roman', style='italic', size=16)
text.set_font_properties(font)
show()