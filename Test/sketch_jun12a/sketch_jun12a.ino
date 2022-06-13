
//LEDs
  int LED = 5;  //Vermelho
  
//Set the float sensor to pin 4
#define Float_Switch 4

void setup() {
    pinMode(LED, OUTPUT);

    pinMode(Float_Switch, INPUT_PULLUP);
  }

void loop() {
  if(digitalRead(Float_Switch) == HIGH)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}
