#include <VirtualWire.h>
#include <NewRemoteTransmitter.h>

const int numberButtons = 4;
const int stateOnePin = 4;
const int stateTwoPin = 6;
const int turnOff = 0;

int whatToSwitch = 0;
int ledState = 0;

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

    // Initialise the IO and ISR
  //  vw_set_ptt_inverted(true);  // Required for DR3100
  vw_setup(2000);	      // Bits per sec
}

void loop() {
  whatToSwitch = checkWeirdSwitch();
  switch(whatToSwitch){
    case 1:
      buttonHandler();
      break;
    case 2:
      ledHandler();
      break;
    case 3:
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
  
        // Only toggle kakuState and send it over 433MHz if the button state is HIGH
        if (buttonState[x] == HIGH) {
          kakuState[x] = !kakuState[x];
          transmitter.sendUnit(x, kakuState[x]);
        }
      }
    }

    // Remember the state of the button
    lastButtonState[x] = reading;
  }
}

void ledHandler(){
  // Needs to be refactored, this block is the same as the block above
  for (int x = 0; x < numberButtons; x++){
    int reading = digitalRead(buttonPin[x]);
  
    if (reading != lastButtonState[x]) {
      lastDebounceTime[x] = millis();
    } 
    
    if ((millis() - lastDebounceTime[x]) > debounceDelay) {
      if (reading != buttonState[x]) {
        buttonState[x] = reading;
  
        if (buttonState[x] == HIGH) {
          switchLed(x);
        }
      }
    }
    
    lastButtonState[x] = reading;
  }
}

void switchLed(int x){
  switch(x){
    case 0:
      msg = "brightnessMin";
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(200);
      break;
    case 2:
      msg = "brightnessPlus";
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(200);
      break;
    case 3:
      // switch color
      
        msg = "off";
      
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(200);
      break;
    case 1:
//      if (ledState){
//        msg = "on";
//      }
//      else {
//        msg = "off";
//      }

      msg = "on";
      
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(200);
      break;
  }
}

void turnEverythingOff(){
  // Also needs to be refactored
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
          }
          // Turn off LED strip
          msg = "off";
          vw_send((uint8_t *)msg, strlen(msg));
          vw_wait_tx(); // Wait until the whole message is gone
          delay(200);
        }
      }
    }
    
    lastButtonState[x] = reading;
  }
}

// Check in which of three possible state the switch is in
int checkWeirdSwitch(){
  if(digitalRead(stateOnePin)){
    return 1;
  } else if(digitalRead(stateTwoPin)){
    return 2;
  } else if(!digitalRead(stateOnePin) && !digitalRead(stateTwoPin)){
    return 3;
  } 
}

void buttonSetup()
{
  // Setup for numberButtons buttons
  for (int x = 0; x < numberButtons; x++)
  {
    lastButtonState[x] = LOW;
    lastDebounceTime[x] = 0;
    kakuState[x] = 0;
    pinMode(buttonPin[x], INPUT); //_PULLUP); Hardware pull down is used
  }
}
