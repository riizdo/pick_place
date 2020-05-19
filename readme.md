-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
----------------------------------PICK & PLACE---------------------------------
---------------------------------arduino proyect-------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

starting proyect

developing serial_lib
	serial protocol for setter comunication
		SOH-NºSTATION-COMMAND-STX-NºDATA-DATA*-ETX-CHECKSUM
	response
		STX-NºSTATION-ERROR CODE-ETX-CHECKSUM
	serial protocol for getter comunication
		SOH-NºSTATION-COMMAND-STX-NºDATA-ETX-CHECKSUM
	response
		STX-NºSTATION-ERROR CODE-DATA*-ETX-CHECKSUM

developing motion_lib:
	step pins and direction pins of motors are:
	first motor (axis x) step pin = 2, direction pin = 22
	second motor(axis y) step pin = 3, derection pin = 23
	third motor (axis *) step pin = 4, direction pin = 24
	fourth motor(axis *) step pin = 5, direction pin = 25
	fifth motor (axis *) step pin = 6, direction pin = 26
	sixth motor (axis *) step pin = 7, direction pin = 27
	seventh motor(axis*) step pin = 8, direction pin = 28
	eighth motor(axis *) step pin = 9, direction pin = 29
	nineth motor(axis *) step pin =10, direction pin = 30
	tenth motor (axis *) step pin =11, direction pin = 31
	eleventh motor(axis*)step pin =12, direction pin = 32
	twelfth motor(axis*) step pin =13, direction pin = 33
	
	in pointList, the first axis is x and the second axis is y
