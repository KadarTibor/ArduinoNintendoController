#include <SoftwareSerial.h>// import the serial library

SoftwareSerial bluetoothSerial(10, 11); // RX, TX
int ledPin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer

int button1Pin = 2;
int button2Pin = 3;
int button3Pin = 4;
int button4Pin = 5;
int button5Pin = 6;
int button6Pin = 7;

int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  //bluetoothSerial.begin(9600);
  //bluetoothSerial.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(ledPin,OUTPUT);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);
  pinMode(button5Pin, INPUT);
  pinMode(button6Pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (bluetoothSerial.available()){
    BluetoothData=bluetoothSerial.read();
    if(BluetoothData=='1'){   // if number 1 pressed ....
      digitalWrite(ledpin,1);
      bluetoothSerial.println("LED  On D13 ON ! ");
    }
    if (BluetoothData=='0'){// if number 0 pressed ....
      digitalWrite(ledpin,0);
      bluetoothSerial.println("LED  On D13 Off ! ");
    }
  }
  */
  checkButtonPressed();
  delay(100);// prepare for next data ...
}

void checkButtonPressed(){
  buttonState = digitalRead(button1Pin) | digitalRead(button2Pin) | digitalRead(button3Pin) | digitalRead(button4Pin)
                | digitalRead(button5Pin) | digitalRead(button6Pin);

  if(buttonState == HIGH){
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }                
}

