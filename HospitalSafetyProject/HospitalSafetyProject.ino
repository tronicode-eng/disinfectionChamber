

/*-------------READ THIS-------------
  when push button set, if button clicked value of input should be zero
*/


const int buzzerPin = 3;

int buttonClickIndicatingLight = 3; // choose the pin for the LED
int entranceButton = 7;   // This pin is declared for entance button
int entranceButtonStatus = 0;     // this is used as flag to check wheather push button is pushed or not

//This is for Valve
const int motorSwitch = 12; //Declare and initialized circulatin valve. This is working when no one is in the room
const int sprayValve = 4; //Declare and initialized spray valve. This is working when someone is in the room

//This is for indicatig lights
const int entranceOkLight = 5; //this are used for indicating of working of circulatingValve
const int roomLight = 11; //this are used for indicating of working of sprayValve

const int feedBackPin = 10; // this is used for get feedback of motor

//---------------Time Adjustments----------------
//all times are measured from milliseconds

//** unsigned integer used because delay function maximum delay is 32000 ms if it is a signed integer
unsigned const int readyTime = 5000; //This time is used to be ready to enter
unsigned const int sprayTime = 10000; //this time is used to spray
unsigned const int motorInitializeTime = 5000;
unsigned const int personExitTime = 5000; //this time varaibe is used to give a time for exiting
const double maximumNoWorkingTime = 120000; // system waiting time for shutdowning
unsigned const int motorFeedbackDelayTime = 2000; //this variable delay motor if wrong input is coming

double  startTime;
double endTime;

//-----------------------------------------------
boolean motorIsWorking = false;
boolean flag = false;
boolean motorDelayWithFeedback = true;

int buttonClickedCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonClickIndicatingLight, OUTPUT);  // declare LED as output
  pinMode(entranceButton, INPUT);    // declare pushbutton as input
  pinMode(feedBackPin, INPUT); //declare

  //This is for valves
  pinMode(motorSwitch, OUTPUT); //Set circulating valve pin as output
  pinMode(entranceOkLight, OUTPUT); //Set circulating valve light pin as output
  pinMode(sprayValve, OUTPUT); //Set spray valve pin as output
  pinMode(roomLight, OUTPUT); //Set spray valve light pin as output
  digitalWrite(sprayValve, !LOW); //Set value of spray valve as 0, because if it is one from the start that will be a problem
  digitalWrite(roomLight, !LOW); //Set value of spray valve light as 0
  digitalWrite(entranceOkLight, !LOW);
  digitalWrite(motorSwitch, LOW);

  //switchOnMotor();

  startTime = millis();
  motorDelayWithFeedback = true;
}

void loop() {
  if (digitalRead(feedBackPin) == HIGH && motorDelayWithFeedback == true) {
    delay(motorFeedbackDelayTime);
    motorDelayWithFeedback = false;
  }

  entranceButtonStatus = digitalRead(entranceButton);  // read input value
  //Serial.println(entranceButtonStatus);


  if (entranceButtonStatus == LOW) {         // check if the input is HIGH (button released)
    digitalWrite(buttonClickIndicatingLight, LOW);  // turn LED OFF
    buttonClickedCount = 0;
  } else {
    buttonClickedCount += 1;
  }

  if (buttonClickedCount >= 5 ) {
    buttonClickedCount = 0;

    motorDelayWithFeedback = true;

    buzzerTone();
    if (motorIsWorking == false && digitalRead(feedBackPin) == HIGH) { //motor has physically Off
      switchOnMotor(true);
    }
    else if (motorIsWorking == false && digitalRead(feedBackPin) == LOW) { //motor has physically on
      switchOnMotor(false);
    }

    digitalWrite(buttonClickIndicatingLight, HIGH);  // turn LED ON
    startTime = millis();
    sprayProcess();

  }

  endTime = millis();

  if ((endTime - startTime >= maximumNoWorkingTime) && motorIsWorking == true) {
    switchOffMotor();
  }

  //Serial.print(maximumNoWorkingTime);
  //Serial.print(" , ");
  //Serial.println(endTime - startTime);


}
void buzzerTone() {
  pinMode(buzzerPin, OUTPUT);
  tone(buzzerPin, 50);
  delay(50);
  noTone(buzzerPin);
}

void switchOnMotor(boolean withInit) {
  motorIsWorking = true;
  digitalWrite(roomLight, !HIGH);
  digitalWrite(motorSwitch, HIGH); //Set value of circulating valve as 1, because it should start and work from the begining
  Serial.println("Motor has started");
  if (withInit) { //wait if the motor has stopped
    delay(motorInitializeTime);
  }
  Serial.println("Motor is properly working");
}
void switchOffMotor() {
  motorIsWorking = false;
  digitalWrite(roomLight, !LOW);
  digitalWrite(motorSwitch, LOW); //Set value of circulating valve as 1, because it should start and work from the begining
  Serial.println("Motor has switcheOff");
}

void sprayProcess() {
  Serial.println("spray process function started-------------------------------------");
  Serial.println("ready time started");
  digitalWrite(entranceOkLight, !HIGH);
  delay(readyTime);
  digitalWrite(entranceOkLight, !LOW);
  Serial.println("spray process started");
  digitalWrite(sprayValve, !HIGH);
  delay(sprayTime);
  digitalWrite(sprayValve, !LOW);
  Serial.println("spray process finished");
  Serial.println("exit time started");
  delay(personExitTime);
  Serial.println("exit time finished");
  Serial.println("spray process function finished-------------------------------------");
}
