#include <SoftwareSerial.h>
#define BASE_CYCLE 3
#define BUZZER_FREQ 440.f
#define N_COMMANDS 6
SoftwareSerial *sserial = NULL;
boolean connected = false;

int buttons[N_COMMANDS] = {6,7,8,9,10,11};
struct Led
{
  int pin;
  bool isOn;
  int cyclesLeft;

  void set_on()
  {
    digitalWrite(pin, HIGH);
    isOn = true;
    cyclesLeft = BASE_CYCLE;
  }

  void set_off()
  {
    digitalWrite(pin, LOW);
    isOn = false;
    cyclesLeft = 0;
  }
};

struct Buzzer
{
  int pin;
  bool isOn;
  int cyclesLeft;

  void set_on()
  {
    tone(pin, BUZZER_FREQ);
    isOn = true;
    cyclesLeft = BASE_CYCLE;
  }

  void set_off()
  {
    noTone(pin);
    isOn = false;
    cyclesLeft = 0;
  }
};

Led red;
Led green;
Buzzer buzzer;
int button_alim = 5;
int cur_command;

void SerialParser(void) {
  char readChar[64];
  Serial.readBytesUntil(33,readChar,64);
  String read_ = String(readChar);
  int idx = read_.indexOf('$');
  String cmd = read_.substring(1,idx);
  
  if (cmd == "rl")
  {
    if (!red.isOn)
    {
      red.set_on();
    }
  }

  else if (cmd == "gl")
  {
    if (!green.isOn)
    {
      green.set_on();
    }
  }

  else if (cmd == "tn")
  {
    if (!buzzer.isOn)
    {
      buzzer.set_on();
    }
  }

  else if (cmd == "cm")
  {
    Serial.print('[');
    Serial.print(cur_command);
    Serial.print(']');
  }

  else if (cmd == "rs")
  {
    cur_command = N_COMMANDS;
  }
}

void check_mode()
{
  //cur_command = N_COMMANDS; //corresponds to error command
  for (int i = 0; i < N_COMMANDS; i++)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      cur_command = i;
    }
  }
}

void setup()  {
  red.pin = 2;
  green.pin = 3;
  red.isOn = false;
  green.isOn = false;
  red.cyclesLeft = 0;
  green.cyclesLeft = 0;

  buzzer.pin = 4;
  buzzer.isOn = false;
  buzzer.cyclesLeft = 0;

  pinMode(red.pin, OUTPUT);
  pinMode(green.pin, OUTPUT);
  pinMode(buzzer.pin, OUTPUT);
  pinMode(button_alim, OUTPUT);
  digitalWrite(button_alim, HIGH);

  Serial.setTimeout(200);

  for (int pin : buttons)
  {
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
  }
  Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  Serial.println("connected");
}

void loop() {
  check_mode();
  SerialParser();

  if (red.isOn)
  {
    red.cyclesLeft--;
    if (red.cyclesLeft <= 0)
    {
      red.set_off();
    }
  }

  if (green.isOn)
  {
    green.cyclesLeft--;
    if (green.cyclesLeft <= 0)
    {
      green.set_off();
    }
  }

  if (buzzer.isOn)
  {
    buzzer.cyclesLeft--;
    if (buzzer.cyclesLeft <= 0)
    {
      buzzer.set_off();
    }
  }
  delay(5);
}