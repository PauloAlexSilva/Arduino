#include <idDHT11.h>
#include <LiquidCrystal_I2C.h>

//Definições

LiquidCrystal_I2C lcd(0x3F, 16, 2);

bool leituraSensor;
bool leituraAnterior;

int humidade;
int totalRegas;

int idDHT11pin = 2;       //Porta Digital do Arduino onde o Sinal do Sensor DHT esta conectado
int idDHT11intNumber = 0; //Número da interrupção respectiva à porta definida no parametro anterior (veja tabela acima)

void dht11_wrapper();     // Declaração da funcão de controle da interrupção. 
void loopDHT();           // Atualiza a leitura do sensor

idDHT11 DHT11(idDHT11pin, idDHT11intNumber, dht11_wrapper);   //Instanciação do Objeto de Controle

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

//Variaveis que irao conter os valores lidos no Sensor DHT11
float temperaturaC;
float temperaturaF;
float temperaturaK;
float umidade;
float dewPoint;
float dewPointSlow;

void loop() {

  // Função sensor DHT11
  loopDHT();

  // Converte valor sensor em %
  humidade = analogRead(leituraSensor);
  humidade = map(humidade, 1023, 0, 0, 100);
  
  lcd.clear();  // Limpa o display
  lcd.print("Humidade do Solo");
  lcd.setCursor(0,1);  // Posiciona o cursor na primeira coluna da segunda linha
  lcd.print(humidade);
  lcd.print("%");
  delay(2000);

  Serial.print("Valor Sensor: ");
  Serial.println(analogRead(leituraSensor));
  
  Serial.print("Valor Convertido: ");
  Serial.println(humidade);
  
  Serial.print("Temperatura Ar: ");
  Serial.println(temperaturaC);
  
  Serial.print("Humidade Ar: ");
  Serial.println(umidade);
 
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

void dht11_wrapper() {
  DHT11.isrCallback();
}

void loopDHT() {
#define tempoLeitura 1000
static unsigned long delayLeitura = millis() + tempoLeitura + 1;
static bool request = false;

  if ((millis() - delayLeitura) > tempoLeitura) { 
      if (!request) {
         DHT11.acquire(); 
         request = true;
      }
  }

  if (request && !DHT11.acquiring()) {
    request = false;

    int result = DHT11.getStatus();
      
    switch (result)
    {
    case IDDHTLIB_OK: 
        Serial.println("Leitura OK"); 
        break;
    case IDDHTLIB_ERROR_CHECKSUM: 
        Serial.println("Erro\n\r\tErro Checksum"); 
        break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT: 
        Serial.println("Erro\n\r\tISR Time out"); 
        break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
        Serial.println("Erro\n\r\tResponse time out"); 
        break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT: 
        Serial.println("Erro\n\r\tData time out erro"); 
        break;
    case IDDHTLIB_ERROR_ACQUIRING: 
        Serial.println("Erro\n\r\tAcquiring"); 
        break;
    case IDDHTLIB_ERROR_DELTA: 
        Serial.println("Erro\n\r\tDelta time to small"); 
        break;
    case IDDHTLIB_ERROR_NOTSTARTED: 
        Serial.println("Erro\n\r\tNao iniciado"); 
        break;
    default: 
        Serial.println("Erro Desconhecido"); 
        break;
    }
    
    float valor = DHT11.getCelsius();
     
    if (!isnan(valor)) {             
      temperaturaC = valor;
    }

    valor = DHT11.getHumidity();
    if (!isnan(valor)) {      
      umidade = valor;
    }  

    valor = DHT11.getFahrenheit();
    if (!isnan(valor)) {      
      temperaturaF = valor;
    }  

    valor = DHT11.getKelvin();
    if (!isnan(valor)) {      
      temperaturaK = valor;
    }  

    valor = DHT11.getDewPoint();
    if (!isnan(valor)) {      
      dewPoint = valor;
    }  
   
    valor = DHT11.getDewPointSlow();
    if (!isnan(valor)) {      
      dewPointSlow = valor;
    } 

    delayLeitura = millis();     
  }
}
