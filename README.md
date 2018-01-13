# ArduinoNintendoController
The aim of this project is to create a bluetooth game controller using an Arduino Uno. 
The game controller should work in a Nintendo emulator.

Part 1: Set up the arduino

	Hardware required: 
		- Arduino UNO 
		- 6 push buttons
		- HC-05 bluetooth module
		- 6 pieces of 10 kohm rezistors
		- wires

	Set up the buttons on a breadboard.
	Set up the bluetooth module. I used the Software Serial library to move my RX and TX pins to pin 11 and 10 on the arduino.
	
	The working principle is easy, just send a predefined sequence of characters through bluetooth. Each button has an unique meaning.
	The data sent through bluetooth will be intercepted and translated by the SerialPortMonitor.c program.
	
Part 2: Set up the host pc
	The host pc should run the program SerialPortMonitor.c with an argument, the argument should be the bluetooth COM port.
	The program sends back the character '1' and the led on the board will be lit. Meaning that the connection has been established.
	Next the program waits and listens on the port for the character combinations, whenever one of the combinations is detected,
an according keystroke is fired. 

Part 3: Start using it with Nintendo emulator
	Start up the emulator. ( I use SNES - just search google for it and download )
	Once the emulator started go to INPUT -> INPUT configuration... -> push the buttons on the arduino to set them accordingly.

Part 4: Enjoy!