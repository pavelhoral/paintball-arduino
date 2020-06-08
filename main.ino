#include <TM1637Display.h>

//~ PIN Constants

const int PIN_CLK = 9; // CLK pin connection to the display
const int PIN_DIO = 8; // DIO pin connection to the display

const int PIN_BTN1 = 13;
const int PIN_BTN2 = 12;
const int PIN_BTN3 = 11;

//~ Display Constants

const int BRIGHTNESS = 3; // Display brightness (0 - 7)

//~ Game Constants

const int COUNTDOWN_SECS = 5;

const int STATE_NONE = 0;
const int STATE_WAITING = 1;
const int STATE_TICKING = 2;
const int STATE_WINNER = 3;

//~ Runtime Variables

int GameState = STATE_NONE;
unsigned long CountdownStart = 0;
int DisplayNumber = 0;
int TeamNumber = 0;
int WinnerPosition = 0;

TM1637Display display(PIN_CLK, PIN_DIO);  //set up the 4-Digit Display.

void setup()
{
  // Set button pin modes
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);
  // Set display brightness
  display.setBrightness(BRIGHTNESS, true);
}

// Read active team number
int readTeam()
{
   if (digitalRead(PIN_BTN1) == LOW) {
     return 1;
   } else if (digitalRead(PIN_BTN2) == LOW) {
     return 2;
   } else if (digitalRead(PIN_BTN3) == LOW) {
     return 3;
   }
   return 0;
}

// Handle STATE_NONE
void handleNone()
{
  display.clear();
  GameState = STATE_WAITING;
}

// Handle STATE_WAITING
void handleWaiting()
{
  int activeTeam = readTeam();
  if (activeTeam > 0) {
    GameState = STATE_TICKING;
    CountdownStart = millis();
    TeamNumber = activeTeam;
  }
}

// Handle STATE_TICKING
void handleTicking()
{
  int activeTeam = readTeam();
  if (activeTeam != TeamNumber) {
    GameState = STATE_NONE;
    return;
  }
  int remaining = COUNTDOWN_SECS - (millis() - CountdownStart) / 1000L;
  if (remaining <= 0) {
    GameState = STATE_WINNER;
  } else if (DisplayNumber != remaining) {
    DisplayNumber = remaining;
    display.showNumberDecEx((remaining / 60 * 100) + remaining % 60, 0b01000000, true);
  }
}

// Handle STATE_WINNER
void handleWinner()
{
  int digitPosition = (millis() / 250) % 4;
  if (digitPosition == WinnerPosition) {
    return;
  }
  WinnerPosition = digitPosition;
  uint8_t data[] = { 0, 0, 0, 0 };
  data[digitPosition] = display.encodeDigit(TeamNumber);
  display.setSegments(data);
}

void loop()
{
  switch (GameState)
  {
  case STATE_NONE:
    handleNone();
  case STATE_WAITING:
    handleWaiting();
    break;
  case STATE_TICKING:
    handleTicking();
    break;
  case STATE_WINNER:
    handleWinner();
    break;
  default:
    break;
  }
  delay(100);
}