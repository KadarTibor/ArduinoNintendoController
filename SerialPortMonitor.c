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

	char *received_info = (char*)malloc((i - 3) * sizeof(char));
	strncat(received_info, SerialBuffer, i - 3);
	
	if (NoBytesRead > 0) {
		if(strcmp(SerialBuffer, ))
	}
	printf("Read data: %s",SerialBuffer);
	memset(SerialBuffer, 0, sizeof(SerialBuffer));
}

void wait_comm_event() {
	SetCommMask(hcomm, EV_RXCHAR);
	DWORD dwEventMask;
	WaitCommEvent(hcomm, &dwEventMask, NULL);
}

void generate_keystroke(char c) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;  // Specify the key as a unicode character
	ip.ki.wScan = c;					// Which keypress to simulate
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	SendInput(1, &ip, sizeof(INPUT));
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

