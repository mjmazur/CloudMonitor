import logging
import time
 
from logging.handlers import TimedRotatingFileHandler
 
#----------------------------------------------------------------------
def create_timed_rotating_log(path, message):
    """"""
    print('Dude')
    logger = logging.getLogger("Rotating Log")
    logger.setLevel(logging.INFO)
 
    handler = TimedRotatingFileHandler(path,
                                       when="s",
                                       interval=10,
                                       backupCount=5)
    logger.addHandler(handler)
 
    #for i in range(6):
    logger.info(message)
    #print(i)
    time.sleep(1)
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    log_file = "timed_test.log"
    print('Hiya')
    create_timed_rotating_log(log_file, 'Fuck Me')