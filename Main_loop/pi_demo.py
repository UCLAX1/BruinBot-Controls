import serial
import time

# ID the serial ports that appear the same
def usb_id(name_list=[], usb_prefix="/dev/ttyACM", baud_rate=9600, timeout=1):
	# Set timeout while waiting for each port's response

	num_ports = len(name_list)
	ports = {}

	for name in name_list:
		ports[name] = None

	connections = 0

	for j in range(10):
		if connections >= num_ports:
			break
		try:
			port_name = "{}{}".format(usb_prefix,j)
			ser = serial.Serial(port_name, baud_rate)
			ser.reset_input_buffer()
			ser.reset_output_buffer()
			connections += 1

			# Opening serial port reboots Arduino, wait a bit before trying to talk
			# time.sleep(2)
			
			# ser.write(b'i')

			handshake_done = False
			start = time.time()
			while True:
				if time.time()-start > timeout:
					print("USB{} handshake timed out".format(j))
					break
				line = ser.readline().decode("utf-8").rstrip("\r\n")
				if line != "":
					for n in name_list:
						if line == n:
							ports[n] = serial.Serial(port_name, baud_rate)
							ser.write(b'd')
							ser.close()
							print("USB{} is {}".format(j,n))
							handshake_done = True
					if not handshake_done:
						print("USB{} did not have a matching name".format(j))
					break

		except serial.SerialException:
			print("Can not open USB{}".format(j))
	
	return ports

def take_command(ports, timeout=1):
	print("")

	finished = False

	msg = input("Give me a command\n")
	parsed = msg.split(" ")
	cmd = parsed[0]
	if len(parsed) == 2:
		arg = parsed[1]
	else:
		arg = 0
		
	if cmd == "motor_forward":
		ports["motor"].write("<forward,{}>".format(arg).encode('utf-8'))

	elif cmd == "motor_backward":
		ports["motor"].write("<backward,{}>".format(arg).encode('utf-8'))

	elif cmd == "motor_brake":
		ports["motor"].write("<brake,{}>".format(arg).encode('utf-8'))

	elif cmd == "sensors_GPS":
		ports["sensors"].write("<GPS>".encode('utf-8'))
		start = time.time()
		while True:
			if time.time()-start > timeout:
				print("Getting GPS timed out".format(j))
				break
			if ports["sensors"].in_waiting > 0:
				line = ports["sensors"].readline().decode("utf-8").rstrip("\r\n")
				print(line)
				break

	elif cmd == "sensors_ACC":
		ports["sensors"].write("<ACC>".encode('utf-8'))
		start = time.time()
		while True:
			if time.time()-start > timeout:
				print("Getting ACC timed out".format(j))
				break
			if ports["sensors"].in_waiting > 0:
				line = ports["sensors"].readline().decode("utf-8").rstrip("\r\n")
				print(line)
				break

	elif cmd == "stepper_forward":
		ports["stepper"].write("<forward,{}>".format(arg).encode('utf-8'))
		print("Stepper forward")
	elif cmd == "stepper_backward":
		ports["stepper"].write("<backward,{}>".format(arg).encode('utf-8'))
		print("Stepper backward")
	elif cmd == "stepper_brake":
		ports["stepper"].write("<brake,{}>".format(arg).encode('utf-8'))
		print("Stepper backward")

	elif cmd == "done":
		finished = True
	else:
		print("{} is not a valid command".format(cmd))

	return finished

# write commands to stop motions at each subsystem
def finish(ports):
	ports["motor"].write("<brake,0>".encode("utf-8"))

if __name__ == "__main__":

	name_list = ["motor", "sensors"]
	ports = usb_id(name_list=name_list, usb_prefix="/dev/ttyACM", baud_rate=9600, timeout=1)

	finished = False
	while not finished:
		finished = take_command(ports)
		
	finish(ports)
	exit()

