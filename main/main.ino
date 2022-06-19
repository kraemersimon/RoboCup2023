int a = 1;
void setup() {
  Serial.begin(9600);
}
void loop() {
  if (a < 100) {
    a = a + 1;
    Serial.println(a);
  }
}
