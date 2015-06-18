#include <VirtualWire.h>
#include <NewRemoteTransmitter.h>

const int numberButtons = 4;
const int stateOnePin = 6;
const int stateTwoPin = 4;
const int turnOff = 0;

int stateOne = 0;
int stateTwo = 0;
int stateThree = 0;
int whatToSwitch = 0;

int buttonPin[numberButtons] = {9,3,7,5};
int buttonState[numberButtons];
int lastButtonState[numberButtons];
int lastDebounceTime[numberButtons];
int kakuState[numberButtons];
 
long debounceDelay = 50;    // the debounce time; increase if the output flickers

char *msg;

// Create a transmitter on address 123, using digital pin 12 to transmit, 
// with a period duration of 260ms (default), repeating the transmitted
// code 2^3=8 times.
NewRemoteTransmitter transmitter(15986790, 12, 260, 3);

void setup() {
  buttonSetup();
  pinMode(stateOnePin, INPUT);
  pinMode(stateTwoPin, INPUT);
  Serial.begin(9600);
  
    // Initialise the IO and ISR
  vw_set_ptt_inverted(true);  // Required for DR3100
  vw_setup(2000);	      // Bits per sec
}

void loop() {
  whatToSwitch = checkWeirdSwitch();
  switch(whatToSwitch){
    case 1:
    Serial.println("One");
      buttonHandler();
      break;
    case 2:
    Serial.println("Two");
      ledHandler();
      break;
    case 3:
    Serial.println("Three");
      turnEverythingOff();
      break;
    default:
      buttonHandler();
  }
}

void buttonHandler(){
  // read the state of the switch into a local variable:
  for (int x = 0; x < numberButtons; x++){
    int reading = digitalRead(buttonPin[x]);
  
    if (reading != lastButtonState[x]) {
      // reset the debouncing timer
      lastDebounceTime[x] = millis();
    } 
    
    if ((millis() - lastDebounceTime[x]) > debounceDelay) {
      if (reading != buttonState[x]) {
        buttonState[x] = reading;
  
        // only toggle the LED if the new button state is HIGH
        if (buttonState[x] == HIGH) {
          kakuState[x] = !kakuState[x];
          transmitter.sendUnit(x, kakuState[x]);
        }
      }
    }
    
    lastButtonState[x] = reading;
  }
}

void ledHandler(){
  // turn LED strip on or off.
}

void turnEverythingOff(){
  // read the state of the switch into a local variable:
  for (int x = 0; x < numberButtons; x++){
    int reading = digitalRead(buttonPin[x]);
  
    if (reading != lastButtonState[x]) {
      // reset the debouncing timer
      lastDebounceTime[x] = millis();
    } 
    
    if ((millis() - lastDebounceTime[x]) > debounceDelay) {
      if (reading != buttonState[x]) {
        buttonState[x] = reading;
  
        // only toggle the LED if the new button state is HIGH
        if (buttonState[x] == HIGH) {
          for (int j = 0; j < numberButtons; j++){
            transmitter.sendUnit(j, turnOff);
            // delay ?
          }
          // @TODO Turn LED strip off
        }
      }
    }
    
    lastButtonState[x] = reading;
  }
}

int checkWeirdSwitch(){
  if(digitalRead(stateOnePin) && !stateOne){
    stateOne = 1;
    stateTwo = 0;
    stateThree = 0;
    Serial.println("StateOne");
    return 1;
  } else if(digitalRead(stateTwoPin) && !stateTwo){
    stateOne = 0;
    stateTwo = 1;
    stateThree = 0;
    Serial.println("StateTwo");
    return 2;
  } else if(!digitalRead(stateOnePin) && !digitalRead(stateTwoPin) && !stateThree){
    stateOne = 0;
    stateTwo = 0;
    stateThree = 1;
    Serial.println("StateThree");
    return 3;
  }
  Serial.println("Stop");
}

void buttonSetup()
{
  for (int x = 0; x < numberButtons; x++)
  {
    lastButtonState[x] = LOW;
    lastDebounceTime[x] = 0;
    kakuState[x] = 0;
    pinMode(buttonPin[x], INPUT); //_PULLDOWN);
  }
}
