import serial
import time

s = serial.Serial('/dev/cu.usbserial-0001', 115200, timeout=1)
time.sleep(2)
s.reset_input_buffer()

with open('soak/recording.bin', 'wb') as f:
    end = time.time() + 30
    while time.time() < end:
        f.write(s.read(1024))

s.close()