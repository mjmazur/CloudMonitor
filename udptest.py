from socket import *
import time
import csv
import numpy as np
import matplotlib.pyplot as plt


name = time.strftime('%Y_%m_%d_%H_%M_%S', time.gmtime())
date = time.strftime('%Y%m%d', time.localtime())

f=open(name+'.csv', 'a+');
writer = csv.writer(f,delimiter=',')
value_array = np.empty((0,3))

def plotData(data):
    print('Plotting...')
    x = data[:,0].astype(np.int)
    y1 = data[:,1].astype(np.float)
    y2 = data[:,2].astype(np.float)

    fig, ax = plt.subplots(figsize=(14,10))
    plt.scatter(x, y1-y2)

    ax.set_xlabel('Time', size=15)
    ax.set_ylabel('Temperature Difference (*C)', size=15)

    plt.tight_layout()
    plt.savefig('test.png', dpi=300)

address= ( b'192.168.1.10', 8888) #define server IP and port
client_socket =socket(AF_INET, SOCK_DGRAM) #Set up the Socket
client_socket.settimeout(2) #Only wait 1 second for a response

cnt = 0

while(1):
    req_data = b'All' # Request all data

    client_socket.sendto( req_data, address) # Send the data request

    try:
        rec_data, addr = client_socket.recvfrom(2048) # Read the response from arduino
        write_buffer = str(int(time.time())) + ' ' + str(rec_data, 'utf-8') # Format string with Unix time and rec_data
        value_array = np.append(value_array, [write_buffer.split()], axis=0) # Append data to an array for plotting
        print('Time: ' + str(value_array[cnt,0]) + '   Sky T: ' + str(value_array[cnt,1]) + '   Gnd T: ' + str(value_array[cnt,2]))
        writer.writerow(write_buffer.split()) # Append data to csv file
        cnt += 1

        if cnt % 12 == 0:
            plotData(value_array)
    except:
        pass

    time.sleep(5) #delay before sending next command

f.close()