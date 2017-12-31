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

char SerialBuffer[25]; 

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
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	GetCommState(hcomm, &dcbSerialParams);
	
	//  Print some of the DCB structure values
	printf("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n",
		dcbSerialParams.BaudRate,
		dcbSerialParams.ByteSize,
		dcbSerialParams.Parity,
		dcbSerialParams.StopBits);
	

	dcbSerialParams.BaudRate = CBR_9600;   // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;          // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;		   // Setting StopBits = 0
	dcbSerialParams.Parity   = NOPARITY;   // Setting Parity = None
	
	SetCommState(hcomm, &dcbSerialParams);

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	
	SetCommTimeouts(hcomm,&timeouts);
}

void write_to_port() {
	char lpBuffer[] = "1";
	DWORD dNoOFBytestoWrite = sizeof(lpBuffer);     // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	
	int fsuccess = WriteFile(hcomm,				 // Handle to the Serial port
			  lpBuffer,				 // Data to be written to the port
			  1,	 //No of bytes to write
			  &dNoOfBytesWritten,	 //Bytes written
			  NULL);
	if (fsuccess == 0) {
		printf("bad reading");
	}
	else {
		printf("good reading");
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
	Sleep(100);
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}


void read_from_port() {

	char TempChar; 
	DWORD NoBytesRead;
	int i = 0;
	int status = 0;
	do
	{
		status = ReadFile(hcomm,            //Handle of the Serial port
						  &TempChar,        //Temporary character
						  sizeof(TempChar), //Size of TempChar
						  &NoBytesRead,     //Number of bytes read
						  NULL);
		if (status != 0 && NoBytesRead > 0 ) {
			SerialBuffer[i] = TempChar;// Store Tempchar into buffer
			i++;
		}
	}
	while (NoBytesRead > 0);

	
	
	if (i > 0) {
		char *received_info = (char *)malloc((i - 1) * sizeof(char));
		received_info[i - 2] = '\0';
		strncpy(received_info, SerialBuffer, i - 2);
		//strncmp(LEFT, SerialBuffer, i - 2);
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
	}
	printf("Read data: %s",SerialBuffer);
	memset(SerialBuffer, 0, sizeof(SerialBuffer));
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
		write_to_port(); //send character 1 back to confirm connection
		while (1) {
			wait_comm_event();
			read_from_port();
		}
	}
    
	return 0;
}

