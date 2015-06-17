const int button1Pin = 3;
const int button2Pin = 4;
const int button3Pin = 5;
const int button4Pin = 6;

const int ledPin = 7;

int ledState = HIGH;         // the current state of the output pin

int button1State;             // the current reading from the input pin
int lastButton1State = LOW;   // the previous reading from the input pin
int button2State;             
int lastButton2State = LOW;   
int button3State;             
int lastButton3State = LOW;   
int button4State;             
int lastButton4State = LOW;   


// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long lastDebounceTime2 = 0;  
long lastDebounceTime3 = 0;  
long lastDebounceTime4 = 0;  
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);
  
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading1 = digitalRead(button1Pin);
  int reading2 = digitalRead(button2Pin);
  int reading3 = digitalRead(button3Pin);
  int reading4 = digitalRead(button4Pin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading1 != lastButton1State) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  } 
  if (reading2 != lastButton2State) {
    lastDebounceTime2 = millis();
  } 
  if (reading3 != lastButton3State) {
    lastDebounceTime3 = millis();
  } 
  if (reading4 != lastButton4State) {
    lastDebounceTime4 = millis();
  } 
  
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading1 != button1State) {
      button1State = reading1;

      // only toggle the LED if the new button state is HIGH
      if (button1State == HIGH) {
        ledState = !ledState;
        Serial.println("Button 1");
      }
    }
  }
    if ((millis() - lastDebounceTime2) > debounceDelay) {
      if (reading2 != button2State) {
        button2State = reading2;
  
        if (button2State == HIGH) {
          ledState = !ledState;
          Serial.println("Button 2");
        }
      }
    }
    if ((millis() - lastDebounceTime3) > debounceDelay) {
      if (reading3 != button3State) {
        button3State = reading3;
        
        if (button3State == HIGH) {
          ledState = !ledState;
          Serial.println("Button 3");
        }
      }
    }
    if ((millis() - lastDebounceTime4) > debounceDelay) {
      if (reading4 != button4State) {
        button4State = reading4;
  
        if (button4State == HIGH) {
          ledState = !ledState;
          Serial.println("Button 4");
        }
      }
    }
  
  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButton1State = reading1;
  lastButton2State = reading2;
  lastButton3State = reading3;
  lastButton4State = reading4;
}
