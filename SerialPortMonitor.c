// SerialPortMonitor.cpp : Defines the entry point for the console application.
//


#include <windows.h>
#include <stdio.h>


HANDLE hcomm;

DCB dcbSerialParams = { 0 };

char SerialBuffer[25]; 

void open_port(char port_name[]) {
	
	hcomm = CreateFile( port_name,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);
	if (hcomm == INVALID_HANDLE_VALUE) {
		printf("ERROR : Could not open port");
		exit(0);
	}
	else {
		pritnf("SUCCESS: Opened serial port for reading");
	}
}

void configure_port() {
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	GetCommState(hcomm, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_9600;   // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;          // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT; // Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;     // Setting Parity = None
	
	SetCommState(hcomm, &dcbSerialParams);
}

void write_to_port() {
	char lpBuffer[] = "1";
	DWORD dNoOFBytestoWrite = sizeof(lpBuffer);     // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	
	WriteFile(hcomm,				 // Handle to the Serial port
			  lpBuffer,				 // Data to be written to the port
			  dNoOFBytestoWrite,	 //No of bytes to write
			  &dNoOfBytesWritten,	 //Bytes written
			  NULL);
}

void read_from_port() {

	char TempChar; 
	DWORD NoBytesRead;
	int i = 0;
	
	do
	{
		ReadFile(hcomm,            //Handle of the Serial port
				 &TempChar,        //Temporary character
				 sizeof(TempChar), //Size of TempChar
				 &NoBytesRead,     //Number of bytes read
				 NULL);

		SerialBuffer[i] = TempChar;// Store Tempchar into buffer
		i++;
	}
	while (NoBytesRead > 0);
	
	printf("Read data: %s ",SerialBuffer);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("ERROR: Bad nr of arguments given to the program");
	}
	else {
		open_port(argv[1]);
		configure_port();
		write_to_port(); //send character 1 back to confirm connection
		while (1) {
			read_from_port();
		}

	}
    
	return 0;
}

