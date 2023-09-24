import serial
import time

serial_port = 'COM10'
baud_rate = 115200

try:
    ser = serial.Serial(serial_port, baud_rate);
    print("Connected")
    
except serial.SerialException as e:
    print(f"Failed {e}")
    exit()
    

file_path = 'E:\\C_EUI\\BOOOOOTLOAADDEEERR\\examples\\test.hex' #path

time.sleep(1)

try:
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    for line in lines:
        line = line.strip()
        val = line[:4]
        
        if line:
            
            data_to_send = int(val, 16).to_bytes(1, byteorder='little')
            ser.write(data_to_send)
            #print(data_to_send)
            print(f"***********************/// {line} sent ///***********************")
            time.sleep(0.01)
            
    data_to_send = int("0xaa", 16).to_bytes(1, byteorder='little')
    ser.write(data_to_send)

except FileNotFoundError:
    print("File not Found")
    
except Exception as e:
    print(f"error: {e}")
    
ser.close()
        