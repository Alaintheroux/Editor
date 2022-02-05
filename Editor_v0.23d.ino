// EDITOR v2.3d version for Arduino based serie of MCU V0.2 (Jan2022)
// (C)2022 ALAIN THÉROUX
// License: You can do what you want with these code but leave these three lines.

void editorMain();
void setup() {
  Serial.begin(115200);
  editorMain();
  Serial.println("returning from editor");
  delay(2000);
}
void loop(){
  editorMain();
  Serial.println("returning from editor");
  delay(2000);
}
