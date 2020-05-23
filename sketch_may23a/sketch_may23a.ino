
// defines
/*
 * RGB 1: R=d6,  G=d5,  B=d3
 * RGB 2: R=d11, G=d10, B=d9
 */

// RGB 1 ports
#define RED_ONE 6
#define GREEN_ONE 5
#define BLUE_ONE 3

// RGB 2 ports
#define RED_TWO 11
#define GREEN_TWO 10
#define BLUE_TWO 9

// other defines
#define BUTTON_PORT 7
#define TIMERTHRESHOLD 32000

// global vars
// button counter
bool button = true;
bool button_old = false;
int buttonCnt = 0;        // debounce counter

// color state
int state = 0;

// timer vars
int timerCnt = 0;
bool timerRunning = false;

// factor and values for both RGBs
int red_value = 0;
int green_value = 0;
int blue_value = 0;

// brightness vars
double brightnessFactor = 1.0;
int brightnessCnt = 0;
int brightnessDirection = 1;

void setup() {
  // RGB 1 pins as output
  pinMode(RED_ONE,      OUTPUT);
  pinMode(GREEN_ONE,    OUTPUT);
  pinMode(BLUE_ONE,     OUTPUT);
  
  // RGB 2 pins as output
  pinMode(RED_TWO,      OUTPUT);
  pinMode(GREEN_TWO,    OUTPUT);
  pinMode(BLUE_TWO,     OUTPUT);

  // other port settings
  pinMode(BUTTON_PORT, INPUT_PULLUP);
}

void loop() {
  button=digitalRead(BUTTON_PORT);

  // press button
  if( button == false  && 
      button != button_old &&
      buttonCnt > 500
    ) {     
    button_old = button;
    buttonCnt = 0;

    startTimer();
  }

  // release button
  if( button == true  && 
      button != button_old &&
      buttonCnt > 500
    ) {     
    button_old = button;
    buttonCnt = 0;

    // if timer threshold is negative change state, else the brightness got adjusted
    if( !getTimerThreshold() ) {
      state++;
      if(state > 3) {
        state = 0;
      }
      setState();
    }
    stopTimer();
  }

  if( getTimerThreshold() ) {
    adjustBrightness();
  }

  incrementTimer();
  incrementButtonCnt();
}

void setState() {
    switch(state) {
      case 0: 
        red_value = 255 * brightnessFactor;
        green_value = 0 * brightnessFactor;
        blue_value = 90 * brightnessFactor;
        break;
      case 1: 
        red_value = 255 * brightnessFactor;
        green_value = 0 * brightnessFactor;
        blue_value = 0 * brightnessFactor;
        break;
      case 2: 
        red_value = 0 * brightnessFactor;
        green_value = 255 * brightnessFactor;
        blue_value = 255 * brightnessFactor;
        break;
      case 3: 
        red_value = 255 * brightnessFactor;
        green_value = 255 * brightnessFactor;
        blue_value = 255 * brightnessFactor;
        break;
    }

    // write new values to ports
    analogWrite(RED_ONE,    red_value);
    analogWrite(GREEN_ONE,  green_value);
    analogWrite(BLUE_ONE,   blue_value);
    analogWrite(RED_TWO,    red_value);
    analogWrite(GREEN_TWO,  green_value);
    analogWrite(BLUE_TWO,   blue_value);
}

void startTimer() {
  timerCnt = 0;
  timerRunning = true;
}

void stopTimer() {
  timerCnt = 0;
  timerRunning = false;
}

void incrementTimer() {
  if(timerRunning) {
    if(timerCnt <= TIMERTHRESHOLD ) {
      timerCnt++;
    }
  }
}

void incrementButtonCnt() {
  buttonCnt++;
  if(buttonCnt < 0) {
    buttonCnt = 0;
  }
}

boolean getTimerThreshold() {
  if( timerRunning &&
      timerCnt > TIMERTHRESHOLD) {
    return true;
  }
  return false;
}

void adjustBrightness() {
  if(brightnessDirection == 1) {
    brightnessFactor += 0.01;
  } else {
    brightnessFactor -= 0.01;
  }
  if(brightnessFactor > 1.0) {
    brightnessDirection = 0;
    brightnessFactor = 1.0;
  } else if( brightnessFactor < 0) {
    brightnessDirection = 1;
    brightnessFactor = 0;
  }

  setState();
}