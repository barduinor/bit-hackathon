void readButA(){
// read the state of the switch into a local variable:
  int reading = digitalRead(butA);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != butALastState) {
    // reset the debouncing timer
    butALastDebounceTime = millis();
  }

  if ((millis() - butALastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != butAState) {
      butAState = reading;

      // only toggle the LED if the new button state is HIGH
      if (butAState == LOW) {
        // send the message:
        sendButA();
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  butALastState = reading;  
}

void readButB(){
// read the state of the switch into a local variable:
  int reading = digitalRead(butB);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != butBLastState) {
    // reset the debouncing timer
    butBLastDebounceTime = millis();
  }

  if ((millis() - butBLastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != butBState) {
      butBState = reading;

      // only toggle the LED if the new button state is HIGH
      if (butBState == LOW) {
        // send the message:
        sendButB();
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  butBLastState = reading;  
}
