#include <TM1637Display.h>

const int CLK = 9; // CLK pin connection to the display
const int DIO = 8; // DIO pin connection to the display
const int BRIGHTNESS = 3; // Display brightness (0 - 7)

const int BTN1 = 13;
const int BTN2 = 12;
const int BTN3 = 11;

const int START_COUNTDOWN = 5;

int NumCountdown = START_COUNTDOWN;
int Team = 0;

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

void setup()
{
  // Set button pin modes
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  // Set display brightness
  display.setBrightness(BRIGHTNESS, true);

}

int readTeam()
{
   if (digitalRead(BTN1) == LOW) {
     return 1;
   } else if (digitalRead(BTN2) == LOW) {
     return 2;
   } else if (digitalRead(BTN3) == LOW) {
     return 3;
   }
   return 0;
}

void countdown()
{
  if (NumCountdown >= 0) {
    display.showNumberDecEx((NumCountdown / 60 * 100) + NumCountdown % 60, 0b01000000, true);
    NumCountdown--;
  }
}

void loop()
{
  int activeTeam = readTeam();
  if (activeTeam == 0) {
    display.showNumberDec(8888, true);
  } else if (activeTeam == Team && NumCountdown >= 0) {
    countdown();
  } else if (activeTeam != Team) {
    NumCountdown = START_COUNTDOWN;
    Team = activeTeam;
  } else if (NumCountdown < 0) {
    display.showNumberDec(activeTeam);
  }
  delay(1000);
}