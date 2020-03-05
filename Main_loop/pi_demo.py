import serial
import time
from parse import parse

if __name__ == "__main__":

	name_list = ["motor", "sensor", "stepper"]
	usb_id(name_list=name_list, usb_prefix="dev/ttyUSB", baud_rate=9600, timeout=1)

	finished = False
	while not finished:
		finished = take_command(ports)
		
	finish()
	exit()


# ID the serial ports that appear the same
def usb_id(name_list=[], usb_prefix="dev/ttyUSB", baud_rate=9600, timeout=1):
	# Set timeout while waiting for each port's response

	num_ports = len(name_list)

	ports = {key:value in zip(name_list,[None]*num_ports)}

	for j in range(num_ports):
		try:
			port_name = "{}{}".format(usb_prefix,j)
			ser = serial.Serial(port_name, baud_rate)
			ser.reset_input_buffer()

			ser.send(b'id')
			handshake_done = False

			start = time.time()
			while True:
				if time.time()-start > timeout:
					print("USB{} handshake timed out".format(j))
					break
				if ser.in_waiting > 0:
					line = ser.readline().decode("utf-8").rstrip("\r\n")
					for n in name_list:
						if line == n:
							ports[n] = ser
							handshake_done = True
							break
			if not handshake_done:
				print("USB{} name not found".format(j))

		except serial.SerialException:
			print("Can not open USB{}".format(j))
	
	return ports

def take_command(ports):

	finished = False

	cmd = input("Give me a command\n")
		
	if cmd == "motor_forward":
		ports["motor"].send(b'1')
		print("Motor forward\n")
	elif cmd == "motor_brake":
		ports["motor"].send(b'2')
		print("Motor brake\n")
	elif cmd == "motor_backward":
		ports["motor"].send(b'3')
		print("Motor backward\n")
	elif cmd == "sensor_gps":
		ports["sensor"].send(b'1')
		print("Get GPS")
	elif cmd == "sensor_imu":
		ports["sensors"].send(b'2')
		print("Get IMU")
	elif cmd == "stepper_f10":
		#ports["stepper"].send(b'f10')
		print("Stepper forward")
	elif cmd == "stepper_b10":
		#ports["stepper"].send(b'b10')
		print("Stepper backward")
	elif cmd == "done":
		finished = True
	else:
		print("{} is not a valid command".format(cmd))

	return finished

# Send commands to stop motions at each subsystem
def finish(ports):
	ports["motor"].send(b'3')

