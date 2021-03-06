
bool leituraSensor;
bool leituraAnterior;

void setup() {

  //Sensor
  pinMode(8, INPUT);

  //Atuador
  pinMode(12, OUTPUT);

  //LEDs
  pinMode(5, OUTPUT);  //Vermelho
  pinMode(6, OUTPUT);  //Amarelo
  pinMode(7, OUTPUT);  //Verde

}

void loop() {

  leituraSensor = digitalRead(9);

  if (leituraSensor == HIGH) {

    //Solo seco
    digitalWrite(5, HIGH);  //Vermelho
    digitalWrite(7, LOW);  //Verde

  } else {

    // solo umido
    digitalWrite(5, LOW);  //Vermelho
    digitalWrite(7, HIGH);

  }

  //Acção solo seco
  if (leituraSensor && !leituraAnterior) {

    delay(5000);
    digitalWrite(5, LOW);  //Vermelho
    digitalWrite(6, HIGH);  //Amarelo

    while (digitalRead(8)) {
      digitalWrite(12, HIGH);  //Rele
      delay(1000);
      digitalWrite(12, LOW); //Rele

      delay(5000);
    }
    digitalWrite(6, LOW);  //Amarelo
  }

  leituraAnterior = leituraSensor;
}
