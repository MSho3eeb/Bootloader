## new Seq
# 2) start value then 1 second delay
# 3) send data with 0.01 second delay
# 4) send end bit with 1 second delay

import serial
import time

serial_port = 'COM8'
baud_rate = 115200

try:
    ser = serial.Serial(serial_port, baud_rate);
    print("Connected")
    
except serial.SerialException as e:
    print(f"Failed {e}")
    exit()
    

file_path = 'E:\\C_EUI\\BOOOOOTLOAADDEEERR\\examples\\test1.hex' #path

time.sleep(1)

counter = 0

try:
    with open(file_path, 'r') as file:
        lines = file.readlines()

    for line in lines:
        line = line.strip()
        val = line[2:4]

        if val.lower() == "ff":
            counter += 1

    time.sleep(2)

    print(counter)

    data_to_send = counter.to_bytes(1, byteorder='little')
    ser.write(data_to_send)
    print(data_to_send)
    
    for line in lines:
        line = line.strip()
        val = line[:4]
        
        if line:
            
            data_to_send = int(val, 16).to_bytes(1, byteorder='little')
            ser.write(data_to_send)
            #print(data_to_send)
            print(f"***********************/// {line} sent ///***********************")
            time.sleep(0.01)
            
    

except FileNotFoundError:
    print("File not Found")
    
except Exception as e:
    print(f"error: {e}")
    
ser.close()
        