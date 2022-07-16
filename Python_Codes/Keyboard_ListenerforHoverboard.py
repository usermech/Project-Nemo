#!/usr/bin/env python
import sys
import termios
import contextlib
import serial
import time
arduino = serial.Serial(port = '/dev/ttyUSB0',baudrate = 115200, timeout=0.1)

@contextlib.contextmanager
def raw_mode(file):
    old_attrs = termios.tcgetattr(file.fileno())
    new_attrs = old_attrs[:]
    new_attrs[3] = new_attrs[3] & ~(termios.ECHO | termios.ICANON)
    try:
        termios.tcsetattr(file.fileno(), termios.TCSADRAIN, new_attrs)
        yield
    finally:
        termios.tcsetattr(file.fileno(), termios.TCSADRAIN, old_attrs)

def serial_write(ch):
    global arduino
    arduino.write(bytes(ch, 'utf-8'))
    time.sleep(0.05)
    


def main():
      
    print( 'exit with ^C or ^D')
    with raw_mode(sys.stdin):
        try:
            while True:
                ch = sys.stdin.read(1)
                if not ch or ch == chr(4):
                    break
                serial_write(ch)             
                # print (ch)
        except (KeyboardInterrupt, EOFError):
            pass


if __name__ == '__main__':
    main()
