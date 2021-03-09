#include <dht.h>
#include <LiquidCrystal_I2C.h>

//Definições
LiquidCrystal_I2C lcd(0x3F, 16, 2);

bool leituraSensor;

int humidade;
int totalRegas;

dht DHT;
#define DHT11_PIN 2

void setup() {

  lcd.init();  // Inicia a comunicação com o display
  lcd.backlight();  // Liga a luz do display
  lcd.print("      BEM");
  lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
  lcd.print("     VINDO");
  delay(4000);
  
  Serial.begin(9600); //Enviar e receber dados em 9600 baud
  
  //Sensor


  //Atuador
  pinMode(8, OUTPUT);

  //LEDs
  pinMode(5, OUTPUT);  //Vermelho
  pinMode(6, OUTPUT);  //Amarelo
  pinMode(7, OUTPUT);  //Verde

}



void loop() {

  DHT.read11(DHT11_PIN);
  delay(100);//Devido a um Bug no DHTLib tem que ter um pause extra

  // Converte valor sensor em %
  humidade = analogRead(leituraSensor);
  humidade = map(humidade, 1023, 0, 0, 100);
  
  int temperaturaCasa = DHT.temperature;
  int humidadeCasa = DHT.humidity;

  // Temperatura do Ar
  lcd.clear();  // Limpa o display
  lcd.print("Temp Ar: ");
  lcd.print(temperaturaCasa);
  lcd.print(" C");
  lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
  
  // Humidade do Ar
  lcd.print("Humi Ar: ");
  lcd.print(humidadeCasa);
  lcd.print(" %");
  delay(3000);
  
  // Humidade do Solo
  lcd.clear();  // Limpa o display
  lcd.print("Humi Solo: ");
  lcd.print(humidade);
  lcd.print(" %");
  lcd.setCursor(0,1); 
  
  // solo umido
  lcd.print("Num Regas: ");
  lcd.print(totalRegas);
  delay(3000);
  
  // Consola Arduino
  Serial.print("Valor Sensor: ");
  Serial.println(analogRead(leituraSensor));
 
  Serial.print("Valor Convertido: ");
  Serial.println(humidade);

  Serial.print("Temperatura: ");  
  Serial.println(DHT.temperature);
  
  Serial.print("Humidade: ");
  Serial.println(DHT.humidity);
  
  //Solo seco
  if (humidade < 50) {

    digitalWrite(5, HIGH);  //Vermelho
    digitalWrite(7, LOW);  //Verde

    lcd.clear();  // Limpa o display
    lcd.print("Solo Seco");
    lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
    lcd.print("A ligar rega");
    delay(2000);
    
    while (humidade < 50) {
      
      totalRegas ++;
      
      lcd.clear();
      lcd.print("Rega ligada");
      lcd.setCursor(0,1);  
      lcd.print("Humi Solo: ");
      lcd.print(humidade);
      lcd.print(" %");
      
      //digitalWrite(5, LOW);  //Vermelho
      digitalWrite(6, HIGH);  //Amarelo
    
      digitalWrite(8, HIGH);  //Rele
      delay(5000);
      digitalWrite(8, LOW); //Rele
      delay(1000);

      // Converte valor sensor em %
      humidade = analogRead(leituraSensor);
      humidade = map(humidade, 1023, 0, 0, 100);
    }
    digitalWrite(6, LOW);  //Amarelo

  } else {
  
    digitalWrite(5, LOW);  //Vermelho
    digitalWrite(7, HIGH); //Verde
  }
}
