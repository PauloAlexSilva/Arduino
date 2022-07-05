#include <dht.h>
#include <LiquidCrystal_I2C.h>

//Definições
  LiquidCrystal_I2C lcd(0x3F, 16, 2);

//Humidade solo
  //bool leituraSensor;
  //int humidade;
  const int AirValue = 620;   //you need to replace this value with Value_1
  const int WaterValue = 310;  //you need to replace this value with Value_2
  int soilMoistureValue = 0;
  int soilmoisturepercent =0;

int totalRegas;

dht DHT;
#define DHT11_PIN 2

//Set the float sensor to pin 4
#define Float_Switch 4

void(* resetFunc) (void) = 0;

void setup() {

  lcd.init();  // Inicia a comunicação com o display
  lcd.backlight();  // Liga a luz do display
  lcd.print("      BEM");
  lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
  lcd.print("     VINDO");
  delay(4000);
  
  Serial.begin(9600); //Enviar e receber dados em 9600 baud
  
  //Atuador
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  //LEDs
  pinMode(5, OUTPUT);  //Vermelho
  pinMode(6, OUTPUT);  //Azul
  pinMode(7, OUTPUT);  //Verde

  pinMode(Float_Switch, INPUT_PULLUP);
}

void loop() {

  DHT.read11(DHT11_PIN);
  delay(100);//Devido a um Bug no DHTLib tem que ter um pause extra

  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent >= 100)
  {
    soilmoisturepercent = 100;
  }
  else if(soilmoisturepercent <=0)
  {
    soilmoisturepercent = 0;
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    soilmoisturepercent;
  }

  //Leitura de dar de e humidade e temperatura do ar
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
    //lcd.print(humidade);
    lcd.print(soilmoisturepercent);
    lcd.print(" %");
    lcd.setCursor(0,1); 
  
  // Numero de Regas
    lcd.print("Num Regas: ");
    lcd.print(totalRegas);
    delay(3000);
  
  // Consola Arduino
    Serial.print("Humidade Solo - Valor Sensor: ");
    //Serial.println(analogRead(leituraSensor));
    Serial.println(analogRead(A0));
   
    Serial.print("Humidade Solo - Valor Convertido: ");
    Serial.println(soilmoisturepercent);
    
    Serial.print("Temperatura Ar: ");  
    Serial.println(DHT.temperature);
    
    Serial.print("Humidade Ar: ");
    Serial.println(DHT.humidity);

    Serial.print("Nível de água: ");
    Serial.println(digitalRead(Float_Switch));
    
  //Se o nível de água OK pode regar
  if(digitalRead(Float_Switch) == HIGH)
  {
    digitalWrite(6, HIGH); //Turn LED on

    digitalWrite(5, HIGH);  //Vermelho
    digitalWrite(7, LOW);  //Verde

    lcd.clear();  // Limpa o display
    lcd.print("Solo Seco");
    lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
    lcd.print("A ligar rega");
    delay(1000);
          
    totalRegas ++;
      
    lcd.clear();
    lcd.print("Rega ligada");
    lcd.setCursor(0,1);  
    lcd.print("Humi Solo: ");
    lcd.print(soilmoisturepercent);
    lcd.print(" %");

    //Bomba 1(Ervas)
    digitalWrite(8, HIGH);  //Rele
    //40 segundos a bomba a trabalhar
    delay(40000);
    digitalWrite(8, LOW); //Rele

    //Delay entre Bombas
    delay(4000);
    
    //Bomba 2 (Tomateiros)
    digitalWrite(9, HIGH);  //Rele
    //3.33 minutos a bomba a trabalhar
    delay(200000); 
    digitalWrite(9, LOW); //Rele
    
    delay(1000);
   
    digitalWrite(5, LOW);  //Vermelho
    digitalWrite(7, HIGH); //Verde

    // Humidade do Solo
    lcd.clear();  // Limpa o display
    lcd.print("Humi Solo: ");
    lcd.print(soilmoisturepercent);
    lcd.print(" %");
    lcd.setCursor(0,1); 
  
    // Numero de regas
    lcd.print("Num Regas: ");
    lcd.print(totalRegas);
    delay(3000);

    //Rega de 8 em 8 horas
    delay(28800000);
    
    if(totalRegas > 12)//reset do arduino ao fim de 4 dias
    {
      resetFunc(); //chamada reset
    }
  }
  
  //Sem água - Parar bomba
  else
  {
    digitalWrite(6, LOW); //Turn LED off
    lcd.clear();  // Limpa o display
    lcd.print("    SEM AGUA   ");
    lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
    lcd.print("Humi Solo: ");
    lcd.print(soilmoisturepercent);
    lcd.print(" %");
    delay(5000);
  }
}
