/* Testando a saída serial do Arduíno
Vamos escrever números na porta serial*/
void setup(){
  Serial.begin(9600); //Enviar e receber dados em 9600 baud
  Serial.println("Gerando valores sequenciais");
}
int numero = 0;
void loop() {
  Serial.print("Valor: ");
  Serial.println(numero);
  delay(500);
  numero++;
}
