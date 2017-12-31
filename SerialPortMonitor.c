// SerialPortMonitor.cpp : Defines the entry point for the console application.
//


#include <windows.h>
#include <stdio.h>

#define LEFT "left"
#define RIGHT "right"
#define UP "up"
#define DOWN "down"
#define SELECT "select"
#define BACK "back"

HANDLE hcomm;

char serial_buffer[25]; 

void open_port() {
	TCHAR *pcCommPort = TEXT("COM4");
	hcomm = CreateFile(pcCommPort,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	
	if (hcomm == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND) {
			printf("\nError: \nThe system cannot find the file specified \n");  //error code 0x02
		}
		else if (err == ERROR_INVALID_NAME) {
			printf("\nError: \n 'filename, directory name, or volume label syntax is incorrect'\n");  //error code 0x7B
		}
		else
		{
			printf("\nHandle creation error code: %x\n", err);
		}
		puts("\t...CreateFile returned an invalid handle value");
		exit(0);
	}
	else {
		printf("SUCCESS: Opened serial port.");
	}
}

void configure_port() {
	DCB dcb_serial_params = { 0 };
	dcb_serial_params.DCBlength = sizeof(dcb_serial_params);

	if (GetCommState(hcomm, &dcb_serial_params) == 0) {
		printf("ERROR: Could not retrieve Comm state");
	}
	
	//  Print the DCB 
	printf("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n",
		dcb_serial_params.BaudRate,
		dcb_serial_params.ByteSize,
		dcb_serial_params.Parity,
		dcb_serial_params.StopBits);
	

	dcb_serial_params.BaudRate = CBR_9600;   // Setting BaudRate = 9600
	dcb_serial_params.ByteSize = 8;          // Setting ByteSize = 8
	dcb_serial_params.StopBits = ONESTOPBIT;		   // Setting StopBits = 0
	dcb_serial_params.Parity   = NOPARITY;   // Setting Parity = None
	
	if (SetCommState(hcomm, &dcb_serial_params) == 0) {
		printf("ERROR: Could not set the ports state");
	}

	printf("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n",
		dcb_serial_params.BaudRate,
		dcb_serial_params.ByteSize,
		dcb_serial_params.Parity,
		dcb_serial_params.StopBits);
}

void set_timeouts() {
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50; 
	timeouts.ReadTotalTimeoutConstant = 50; 
	timeouts.ReadTotalTimeoutMultiplier = 10; 
	timeouts.WriteTotalTimeoutConstant = 50; 
	timeouts.WriteTotalTimeoutMultiplier = 10; 

	if (SetCommTimeouts(hcomm, &timeouts) == 0) {
		printf("ERROR: Could not set the timeouts");
	}
}

void write_to_port() {
	char buffer[] = "1";
	DWORD dNoOFBytestoWrite = sizeof(buffer);   
	DWORD dNoOfBytesWritten = 0;					
	
	int fsuccess = WriteFile(hcomm,				
			  buffer,				 
			  1,	
			  &dNoOfBytesWritten,	
			  NULL);
	if (fsuccess == 0) {
		printf("ERROR: Could not write to the port");
	}
}

void generate_keystroke(char c) {
	INPUT ip;
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	switch (c) {
		case 'A': {
			ip.ki.wVk = 0x41; //code for A
			break;
		}
		case 'W': {
			ip.ki.wVk = 0x57; //code for W
			break;
		}
		case 'S': {
			ip.ki.wVk = 0x53; //code for S
			break;
		}
		case 'D': {
			ip.ki.wVk = 0x44; //code for D
			break;
		}
		case 'J': {
			ip.ki.wVk = 0x4A; //code for J
			break;
		}
		case 'K': {
			ip.ki.wVk = 0x4B; //code for K
			break;
		}
	}
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(50); //delay needed in order to trigger the keystroke even, otherwise it is released too soon
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}


void read_from_port() {

	char temp_char; 
	DWORD NoBytesRead;
	int i = 0;
	int status = 0;
	do
	{
		status = ReadFile(hcomm,            //Handle of the Serial port
						  &temp_char,        //Temporary character
						  sizeof(temp_char), //Size of temp_char
						  &NoBytesRead,     //Number of bytes read
						  NULL);
		if (status != 0 && NoBytesRead > 0 ) {
			serial_buffer[i] = temp_char;// Store temp_char into buffer
			i++;
		}
	}
	while (NoBytesRead > 0);

	
	
	if (i > 0) {
		char *received_info = (char *)malloc((i - 1) * sizeof(char));
		received_info[i - 2] = '\0';
		strncpy(received_info, serial_buffer, i - 2);
		if (strcmp(received_info, LEFT) == 0) {
			generate_keystroke('A');
		}
		if (strcmp(received_info, UP) == 0) {
			generate_keystroke('W');
		}
		if (strcmp(received_info, DOWN) == 0) {
			generate_keystroke('S');
		}
		if (strcmp(received_info, RIGHT) == 0) {
			generate_keystroke('D');
		}
		if (strcmp(received_info, SELECT) == 0) {
			generate_keystroke('J');
		}
		if (strcmp(received_info, BACK) == 0) {
			generate_keystroke('K');
		}
		
		printf("Read data: %s", serial_buffer);
		memset(serial_buffer, 0, sizeof(serial_buffer));
	}
	
}

void wait_comm_event() {
	SetCommMask(hcomm, EV_RXCHAR);
	DWORD dwEventMask;
	WaitCommEvent(hcomm, &dwEventMask, NULL);
}


int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("ERROR: Bad nr of arguments given to the program");
	}
	else {
	
		open_port();
		configure_port();
		set_timeouts();
		write_to_port(); //send character 1 back to confirm connection
		while (1) {
			wait_comm_event();
			read_from_port();
		}
	}
	return 0;
}

