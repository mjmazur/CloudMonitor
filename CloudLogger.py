import logging
import time
import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from socket import *
from scipy import interpolate
from logging.handlers import TimedRotatingFileHandler

name = time.strftime('%Y_%m_%d_%H_%M_%S', time.gmtime())
date = time.strftime('%Y%m%d', time.localtime())

f=open(name+'.csv', 'a+');
writer = csv.writer(f,delimiter=',')
value_array = np.empty((0,3))

def plotData(data):
    print('Plotting...')
    t = data[:,0].astype(np.int)
    y = data[:,1].astype(np.float)-data[:,2].astype(np.float)

    fig, ax = plt.subplots(figsize=(14,10))
    plt.scatter(t, y)

    ax.set_xlabel('Time', size=15)
    ax.set_ylabel('Temperature Difference (*C)', size=15)

    plt.tight_layout()
    plt.savefig('test.png', dpi=300)

def fancyPlot(data):
    print('Plotting...')

    # Arrange the data into blocks and calculate the mean of each block
    # blocksize = 600
    samplerate = 10
    t = data[:,0].astype(np.int)
    y = data[:,1].astype(np.float)-data[:,2].astype(np.float)

    f = interpolate.interp1d(t,y)
    xnew = np.arange(min(t),max(t),samplerate)
    ynew = f(xnew)

    n = int(len(ynew))

    a = ynew[0:(n-1)].reshape(1,1,n-1)
    block = np.mean(a, axis=1)

    plt.figure(figsize = (10,6))
    gs1 = gridspec.GridSpec(2, 1, width_ratios=[1], height_ratios=[1,8])
    gs1.update(wspace=0.025, hspace=0.0) # set the spacing between axes. 

    ax0 = plt.subplot(gs1[0])
    ax0.pcolorfast(block, cmap='Blues_r', vmin=-25, vmax=-6)

    ax0.set_xticklabels([])
    ax0.set_xticks([])
    ax0.set_yticklabels([])
    ax0.set_yticks([])
    plt.title('Cloud Cover', size=20)

    ax1 = plt.subplot(gs1[1])
    ax1.plot(t, y, color='black')

    ax1.set_xlim(min(t), max(t))
    ax1.set_xlabel('Local Time', size=15)
    ax1.set_ylabel('Sky Temp minus Ground Temp (*C)', size=15)

    plt.savefig('test.png', dpi=300)

def timed_rotating_log(path, message):

    logger = logging.getLogger("Rotating Log")
    logger.setLevel(logging.INFO)
 
    handler = TimedRotatingFileHandler(path,
                                       when="s",
                                       interval=20,
                                       backupCount=5)
    logger.addHandler(handler)
 
    #for i in range(6):
    logger.info(message)
    #print(i)
    time.sleep(1)

if __name__ == "__main__":

    # Setup logging
    log_file = "current.log"

    logger = logging.getLogger("Rotating Log")
    logger.setLevel(logging.INFO)
 
    handler = TimedRotatingFileHandler(log_file,
                                       when="s",
                                       interval=20,
                                       backupCount=5)
    logger.addHandler(handler)

    # Setup remote cloud monitor
    address= ( b'10.0.20.10', 8888) #define server IP and port
    client_socket =socket(AF_INET, SOCK_DGRAM) #Set up the Socket
    client_socket.settimeout(2) #Only wait 2 seconds for a response

    cnt = 0

    while(1):
        req_data = b'All' # Request all data

        # client_socket.sendto( req_data, address) # Send the data request

        try:
            client_socket.sendto( req_data, address) # Send the data request
            rec_data, addr = client_socket.recvfrom(2048) # Read the response from arduino
            write_buffer = str(int(time.time())) + ' ' + str(rec_data, 'utf-8') # Format string with Unix time and rec_data
            value_array = np.append(value_array, [write_buffer.split()], axis=0) # Append data to an array for plotting
            print('Time: ' + str(value_array[cnt,0]) + '   Sky T: ' + str(value_array[cnt,1]) + '   Gnd T: ' + str(value_array[cnt,2]) + ' Delta T: ' + str(value_array[cnt,1]-value_array[cnt,2]))
            writer.writerow(write_buffer.split()) # Append data to csv file
            logger.info(write_buffer)
            
            cnt += 1

            if cnt % 6 == 0:
                fancyPlot(value_array)
                #plotData(value_array)
        except:
            pass

        time.sleep(10) #delay before sending next command

    f.close()