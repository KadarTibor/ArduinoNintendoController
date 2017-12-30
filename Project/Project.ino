#include <SoftwareSerial.h>// import the serial library

SoftwareSerial bluetoothSerial(10, 11); // RX, TX
int ledPin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer

int buttonPin[] = {2,3,4,5,6,7};
String buttonMessage[] = {"left","up","down","right","select","back"};
unsigned long lastSampleTime[] = {0,0,0,0,0,0}; 
int buttonPreviousState[] = {0,0,0,0,0,0};
int buttonState[] = {0,0,0,0,0,0};

unsigned long currentTime = 0;

void setup() {
  bluetoothSerial.begin(9600);
  //bluetoothSerial.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledPin,OUTPUT);
  for(int i = 0; i < 6; i++){
    pinMode(buttonPin[i], INPUT);
  }
  check_connection();
}

void loop() { 
  handle_button_press();
}

void check_connection(){
  while(!bluetoothSerial.available());
  BluetoothData=bluetoothSerial.read();
  if(BluetoothData=='1'){   // if number 1 pressed ....
      digitalWrite(ledPin,HIGH);
      bluetoothSerial.println("Connected");
  }
}
void handle_button_press(){
      for(int i = 0; i < 6; i++){
        currentTime = millis();
        if(currentTime - lastSampleTime[i] >= 50){
            buttonState[i] = digitalRead(buttonPin[i]);
            if(buttonState[i] == 1 && buttonPreviousState[i] == 0){
              bluetoothSerial.println(buttonMessage[i]);
            }
            buttonPreviousState[i] = buttonState[i];
            lastSampleTime[i] = currentTime;
        }
      }
}
