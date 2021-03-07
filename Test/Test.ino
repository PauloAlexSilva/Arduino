//Import Bibliotecas

#include <LiquidCrystal_I2C.h>

//Definições

LiquidCrystal_I2C lcd(0x3F, 16, 2);

bool leituraSensor;
bool leituraAnterior;

int humidade;
int totalRegas;

void setup() {

  lcd.init();  // Inicia a comunicação com o display
  lcd.backlight();  // Liga a luz do display
  lcd.print("Bem Vindo!");
  delay(3000);
  
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

  // Converte valor sensor em %
  humidade = analogRead(leituraSensor);
  humidade = map(humidade, 1023, 0, 0, 100);
  
  lcd.clear();  // Limpa o display
  lcd.print("Humidade do Solo");
  lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
  lcd.print(humidade);
  lcd.print("%");
  delay(2000);

  Serial.println("Valor Sensor: ");
  Serial.println(analogRead(leituraSensor));
  Serial.println("Valor Convertido: ");
  Serial.println(humidade);

  //Solo seco
  if (humidade < 50) {

    digitalWrite(5, HIGH);  //Vermelho
    digitalWrite(7, LOW);  //Verde

    lcd.clear();  // Limpa o display
    lcd.print("Solo Seco!");
    lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
    lcd.print("A ligar a rega.");
    delay(2000);
    
    while (humidade < 50) {
      
      totalRegas ++;
      
      lcd.clear();
      lcd.print("Rega ligada");
      lcd.setCursor(0,1);  
      lcd.print("Humidade: ");
      lcd.print(humidade);
      lcd.print("%");
      
      //digitalWrite(5, LOW);  //Vermelho
      digitalWrite(6, HIGH);  //Amarelo
    
      digitalWrite(12, HIGH);  //Rele
      delay(5000);
      digitalWrite(12, LOW); //Rele
      delay(1000);

      // Converte valor sensor em %
      humidade = analogRead(leituraSensor);
      humidade = map(humidade, 1023, 0, 0, 100);
    }
    digitalWrite(6, LOW);  //Amarelo

  } else {

    // solo umido
    lcd.clear();  // Limpa o display
    lcd.print("Solo Humido");
    lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
    lcd.print("Total Regas:");
    lcd.print(totalRegas);
    delay(2000);
    
    digitalWrite(5, LOW);  //Vermelho
    digitalWrite(7, HIGH); //Verde
  }
}
