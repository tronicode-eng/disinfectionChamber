void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(10, INPUT);
  digitalWrite(12, LOW);
}

void loop() {
  Serial.println(digitalRead(10) == LOW);

}
