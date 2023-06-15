#define n_vals 4
uint8_t vals[n_vals];
uint8_t comp = A0;

uint8_t mux_ctr[4] = {7,6,5,4};
uint8_t comp_alim = 11;

uint8_t sensors[4] = {0,1,6,7};

uint8_t bin_0[16] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
uint8_t bin_1[16] = {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1};
uint8_t bin_2[16] = {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1};
uint8_t bin_3[16] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};

int val;

void setup() {
  pinMode(comp_alim, OUTPUT);
  digitalWrite(comp_alim, HIGH);

  for (int i = 0; i < 4; i++)
  {
    pinMode(mux_ctr[i], OUTPUT);
    digitalWrite(mux_ctr[i], LOW);
  }

  pinMode(comp, INPUT);

  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
}

void loop() {

  for (int i = 0; i < n_vals; i++)
  {
    digitalWrite(mux_ctr[0], bin_0[sensors[i]]);
    digitalWrite(mux_ctr[1], bin_1[sensors[i]]);
    digitalWrite(mux_ctr[2], bin_2[sensors[i]]);
    digitalWrite(mux_ctr[3], bin_3[sensors[i]]);

    delay(5);
    Serial.print('[');
    Serial.print(i);
    Serial.print('%');
    Serial.print(analogRead(comp));
    Serial.print(']');
    Serial.print('\n');
    digitalWrite(mux_ctr[0], LOW);
    digitalWrite(mux_ctr[1], LOW);
    digitalWrite(mux_ctr[2], LOW);
    digitalWrite(mux_ctr[3], LOW);
    delay(5);

  }
}

// void loop() {
//   for (int i = 0; i < 20; i++)
//   {
//     delay(5);
//     Serial.print('[');
//     Serial.print("0");
//     Serial.print('%');
//     Serial.print("1000");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("1");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("2");
//     Serial.print('%');
//     Serial.print("1000");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("3");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//   }
//   for (int i = 0; i < 20; i++)
//   {
//     delay(5);
//     Serial.print('[');
//     Serial.print("0");
//     Serial.print('%');
//     Serial.print("1000");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("1");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("2");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("3");
//     Serial.print('%');
//     Serial.print("1000");
//     Serial.print(']');
//   }
//   for (int i = 0; i < 300; i++)
//   {
//     delay(5);
//     Serial.print('[');
//     Serial.print("0");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("1");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("2");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//     delay(5);
//     Serial.print('[');
//     Serial.print("3");
//     Serial.print('%');
//     Serial.print("0");
//     Serial.print(']');
//   }
// }
