const int motor1_forward=2;
const int motor2_forward=4;
const int motor3_forward=7;
const int motor4_forward=8;
const int DATA0 = 10;
const int DATA1 = 11;

void setup() {
  pinMode(motor1_forward, OUTPUT);
  pinMode(motor2_forward, OUTPUT);
  pinMode(motor3_forward, OUTPUT);
  pinMode(motor4_forward, OUTPUT);
  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  Serial.begin(9600);
}


void forward() {
  digitalWrite(motor1_forward,1);
  digitalWrite(motor2_forward,1);
  digitalWrite(motor3_forward,1);
  digitalWrite(motor4_forward,1);
}


void halt() {
  digitalWrite(motor1_forward,0);
  digitalWrite(motor2_forward,0);
  digitalWrite(motor3_forward,0);
  digitalWrite(motor4_forward,0);
}


void right() {
  digitalWrite(motor1_forward,0);
  digitalWrite(motor2_forward,0);
  digitalWrite(motor3_forward,1);
  digitalWrite(motor4_forward,1);
}

void left() {
  digitalWrite(motor1_forward,1);
  digitalWrite(motor2_forward,1);
  digitalWrite(motor3_forward,0);
  digitalWrite(motor4_forward,0);
}

void loop() {
  if (digitalRead(DATA0) == HIGH && digitalRead(DATA1) == HIGH)
    forward();
  else if (digitalRead(DATA0) == HIGH && digitalRead(DATA1) == LOW)
    right();
  else if (digitalRead(DATA0) == LOW && digitalRead(DATA1) == HIGH)
    left();
  else
    halt();
}
