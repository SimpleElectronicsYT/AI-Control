// Defining the input pins for the ESP32 DevKitC
const int sw1 = 19;
const int sw2 = 21;
const int sw3 = 22;
const int sw4 = 23;
const int sw5 = 14;
const int sw6 = 16;
const int sw7 = 17;
const int sw8 = 18;

// Defining the output pins for the ESP32 DevKitC
const int out1 = 12;
const int out2 = 27;
const int out3 = 26;
const int out4 = 25;
const int out5 = 33;
const int out6 = 32;
// Screwed up here, these two pins cannot be used as outputs - sorry.
const int out7 = 35;
const int out8 = 34;

void setup() {

  // Setting up the input pins to use the built-in pullup resistors
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  pinMode(sw5, INPUT_PULLUP);
  pinMode(sw6, INPUT_PULLUP);
  pinMode(sw7, INPUT_PULLUP);
  pinMode(sw8, INPUT_PULLUP);

  // Setting up the output pins and set them to a known state (low)
  pinMode(out1, OUTPUT);
  digitalWrite(out1, LOW);
  pinMode(out2, OUTPUT);
  digitalWrite(out2, LOW);
  pinMode(out3, OUTPUT);
  digitalWrite(out3, LOW);
  pinMode(out4, OUTPUT);
  digitalWrite(out4, LOW);
  pinMode(out5, OUTPUT);
  digitalWrite(out5, LOW);
  pinMode(out6, OUTPUT);
  digitalWrite(out6, LOW);
  pinMode(out7, OUTPUT);
  digitalWrite(out7, LOW);
  pinMode(out8, OUTPUT);
  digitalWrite(out8, LOW);

}

void loop() {
  // Loop though every pin and activate the corresponding output
  if (digitalRead(sw1) == LOW) {
    digitalWrite(out1, HIGH);
  }
  else {
    digitalWrite(out1, LOW);
  }
  if (digitalRead(sw2) == LOW) {
    digitalWrite(out2, HIGH);
  }
  else {
    digitalWrite(out2, LOW);
  }
  if (digitalRead(sw3) == LOW) {
    digitalWrite(out3, HIGH);
  }
  else {
    digitalWrite(out3, LOW);
  }
  if (digitalRead(sw4) == LOW) {
    digitalWrite(out4, HIGH);
  }
  else {
    digitalWrite(out4, LOW);
  }
  if (digitalRead(sw5) == LOW) {
    digitalWrite(out5, HIGH);
  }
  else {
    digitalWrite(out5, LOW);
  }
  if (digitalRead(sw6) == LOW) {
    digitalWrite(out6, HIGH);
  }
  else {
    digitalWrite(out6, LOW);
  }
  if (digitalRead(sw7) == LOW) {
    digitalWrite(out7, HIGH);
  }
  else {
    digitalWrite(out7, LOW);
  }
  if (digitalRead(sw8) == LOW) {
    digitalWrite(out8, HIGH);
  }
  else {
    digitalWrite(out8, LOW);
  }
}
