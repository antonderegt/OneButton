#include <VirtualWire.h>
#include <NewRemoteTransmitter.h>

const int numberButtons = 4;

int buttonPin[numberButtons] = {3,4,5,6};
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
  
    // Initialise the IO and ISR
  vw_set_ptt_inverted(true);  // Required for DR3100
  vw_setup(2000);	      // Bits per sec
}

void loop() {
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
