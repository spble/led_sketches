import socket
from artnet import packet
import serial
import struct
import select
import threading

UDP_IP = "127.0.0.1"
UDP_PORT = 6454
SERIAL_DEVICE = "/dev/ttyACM0"

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
sock.setblocking(1)
sock.settimeout(0.1)

ser = serial.Serial(SERIAL_DEVICE, 50000, timeout=0.3, write_timeout=5.0)

print "Listening on %s:%s for Artnet, sending to: %s" % (UDP_IP, UDP_PORT, ser.name)

START_UNIVERSE = 1
MAX_UNIVERSES = 1
universes = [None,] * MAX_UNIVERSES
universes_received = 0
NUM_PIXELS = 4800;

while ser.is_open:
    """
    ser.write(struct.pack("B", 255))
    for i in xrange(4800):
        ser.write(struct.pack("B", 1))
        ser.write(struct.pack("B", 30))
        ser.write(struct.pack("B", 16))
    ser.flush()
    continue
    """
    r, _, _ = select.select([sock], [], [])
    if r:
        # ready to receive
        data, addr = sock.recvfrom(540) # buffer size is 1024 bytes
    else:
        print "No data"

    p = packet.ArtNetPacket.decode(addr, data)
    universes[p.universe - START_UNIVERSE] = p.framedata
    universes_received += 1
    if universes_received == MAX_UNIVERSES:
        if ser.writable():
            print "Can't write!"
        else:
            ser.write(struct.pack("B", 255))
        for universe in universes:
            print universe
            for data in universe:
                if ser.writable():
                    print "Can't write!"
                else:
                    written = ser.write(struct.pack("B", data))
        if ser.writable():
            print "Can't write!"
        else:
            ser.write(struct.pack("B", 255))
        universes = [None,] * MAX_UNIVERSES
        universes_received = 0
print "Serial closed."



