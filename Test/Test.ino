
bool leituraSensor;
bool leituraAnterior;

void setup() {

  Serial.begin(9600); //Enviar e receber dados em 9600 baud
  
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

  
  
  Serial.println(analogRead(leituraSensor));

  if (analogRead(leituraSensor) >  700) {

    //Solo seco
    digitalWrite(4, HIGH);  //Vermelho
    digitalWrite(7, LOW);  //Verde

    delay(1000);
    
    while (analogRead(leituraSensor) > 700) {

      digitalWrite(4, LOW);  //Vermelho
      digitalWrite(6, HIGH);  //Amarelo
    
      digitalWrite(12, HIGH);  //Rele
      delay(5000);
      digitalWrite(12, LOW); //Rele

      delay(1000);
    }
    digitalWrite(6, LOW);  //Amarelo

  } else {

    // solo umido
    digitalWrite(4, LOW);  //Vermelho
    digitalWrite(7, HIGH); //Verde

  }

}
