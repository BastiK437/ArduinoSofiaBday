
// defines
/*
 * RGB 1: R=6,  G=5,  B=3
 * RGB 2: R=11, G=10, B=9
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
#define TIMERTHRESHOLD 100
#define BRIGHTNESSTHRESHOLD 100

// uncomment to debug with serial port
#define DEBUG 


// global vars
bool button = true;
bool button_old = false;
int state = 0;

// timer vars
int timerCnt = 0;
bool timerRunning = false;

// factor and values for both RGBs
int red_value = 0;
int green_value = 0;
int blue_value = 0;
double brightnessFactor = 1.0;
int brightnessCnt = 0;

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
  pinMode(BUTTON_PORT,  INPUT);

  // debugging
#ifdef DEBUG
  Serial.begin(4800);
#endif 
  Serial.println("begin program");
}

void loop() {
  //button=digitalRead(BUTTON_PORT);

  // press button
  if( button == true  && 
      button != button_old 
    ) {     
    button_old = button;
    state++;
    setState();

    startTimer();
  }

  // release button
  if( button == false  && 
      button != button_old 
    ) {     
    button_old = button;
    
    // if timer threshold is negative change state, else the brightness got adjusted
    if( !getTimerThreshold() ) {
      setState();
    }
    stopTimer();
  }

  if( getTimerThreshold() ) {
    adjustBrightness();
  }

  Serial.println(timerCnt);

  incrementTimer();
}

void setState() {
    switch(state) {
      case 0: 
        red_value = 255 * brightnessFactor;
        green_value = 0 * brightnessFactor;
        blue_value = 90 * brightnessFactor;
#ifdef DEBUG
        Serial.println("set color pink");
#endif
        break;
      case 1: 
        red_value = 255 * brightnessFactor;
        green_value = 0 * brightnessFactor;
        blue_value = 0 * brightnessFactor;
#ifdef DEBUG
        Serial.println("set color red");
#endif        
        break;
      case 2: 
        red_value = 0 * brightnessFactor;
        green_value = 255 * brightnessFactor;
        blue_value = 255 * brightnessFactor;
#ifdef DEBUG
        Serial.println("set color turquoise");
#endif        
        break;
      case 3: 
        red_value = 255 * brightnessFactor;
        green_value = 255 * brightnessFactor;
        blue_value = 255 * brightnessFactor;
#ifdef DEBUG
        Serial.println("set color white");
#endif
        state = 0;
        break;
    }

    // write to new values to ports
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
  timerRunning = false;
}

void incrementTimer() {
  if(timerRunning) {
    timerCnt++;
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
  brightnessCnt++;

  if(brightnessCnt % BRIGHTNESSTHRESHOLD) {
    brightnessFactor += 1.0;
    if(brightnessFactor > 1.0) {
      brightnessFactor = 0.0;
    }
  }
}