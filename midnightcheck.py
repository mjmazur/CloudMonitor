#from datetime import datetime as dt
from pytz import timezone
import time

while(1):
	#now = dt.now()
	t = time.localtime()
	#midnight = now.hour == 23 and now.minute ==59 and now.second == 0
	midnight = t.tm_hour == 11 and t.tm_min ==58 and t.tm_sec == 0

	#print(now)
	print(t)
	print(midnight)