//@Autor: Ednilson Brito Lopes
#include <Servo.h>  //Inclui a biblioteca de manipulação de servos
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Definindo os pinos
#define trigPin A0 //Pino TRIG do sensor no pino analógico A0
#define echoPin A1 //Pino ECHO do sensor no pino analógico A1
#define enA 2 //Pino que mantem a velocidade do motorA
#define in1 3 //Pino do motor A
#define in2 4 //Pino do motor A
#define in3 5 //Pino do motor B
#define in4 6 //Pino do motor B
#define enB 7 //Pino que mantem a velocidade do motorB

//Pinos utilizados para conexao do modulo GY-NEO6MV2
static const int RXPin = 11, TXPin = 12;

//Objeto TinyGPS++
TinyGPSPlus gps;

//Conexao serial do modulo GPS
SoftwareSerial Serial_GPS(RXPin, TXPin);

int tempoGirar = 1;//esse é o tempo para o robô girar em 45º com uma bateria de 9v.
int distanciaObstaculo = 37; //distância para o robô parar e recalcular o melhor caminho
int velocidadeMotores = 130; // velocidade que os motores funcionarão na bateria 9v. Para a bateria 9v a velocidade 80 é ideal

Servo servo_ultra_sonico; // nomeando o servo motor
//variáveis  para o sensor ultrassonico
long duracao;
long distancia_cm = 0;
int minimumRange = 5; //tempo de resposta do sensor
int maximumRange = 200;

void setup() {
  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados
  //Baud rate Modulo GPS
  Serial_GPS.begin(9600);
  servo_ultra_sonico.attach(8);  // Define o mini servo motor ligado no pino digital 8.
  pinMode(trigPin, OUTPUT); //define o pino TRIG como saída
  pinMode(echoPin, INPUT);  //define o pino ECHO como entrada
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  analogWrite(enA, velocidadeMotores); //Define a velocidade dos motores
  analogWrite(enB, velocidadeMotores);
  servo_ultra_sonico.write(90);   // O servo do sensor se inicia a 90 graus (meio)
  //Inicia Motor A parado
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  //Inicia Motor B parado
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);

}

void loop() {
  pensar(); //inicia a função pensar
}

// Função para chamar outras funções e definir o que o robô fará
void pensar() {
  displayInfo();
  reposicionaServoSonar(); //Coloca o servo para olhar a frente
  int distancia = lerSonar(); // Ler o sensor de distância
  Serial.print("distancia em cm: ");
  Serial.println(distancia);   // Exibe no serial monitor
  if (distancia > distanciaObstaculo) {  // Se a distância for maior que 20 cm
    rotacao_Frente(); //robô anda para frente
  } else {
    rotacao_Parado();  //para o robô
    posicionaCarroMelhorCaminho(); //calcula o melhor caminho
    pensar();
  }
}

// Função que faz o robô virar à direita, http://loja.seurobo.com.br/
void rotacao_Direita()
{
  delay(100);
  // O robô dá uma ré para não colidir ao girar
  //Gira o Motor A no sentido horario
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //Gira o Motor B no sentido horario
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(100);
  Serial.println(" Para a Direita ");
  //Motor A parado
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(tempoGirar);
}

// Função que faz o robô virar à esquerda
void rotacao_Esquerda()
{
  delay(100);
  // O robô dá uma ré para não colidir ao girar
  //Gira o Motor A no sentido horario
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //Gira o Motor B no sentido horario
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(100);
  Serial.println(" Para a esquerda ");
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Motor B Parado
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  delay(tempoGirar);
}

// Função para fazer o robô andar para frente
void rotacao_Frente()
{
  Serial.println("Motor: Frente ");
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(50);
}

// Função para fazer o carro parar
void rotacao_Parado()
{
  Serial.println(" Motor: Parar ");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}

// Função que faz o robô andar para trás e emite som quando ele dá ré
void rotacao_Re()
{
  Serial.println("Motor: ré ");
  for (int i = 0; i <= 3; i++) {
    delay(100);
    //Gira o Motor A no sentido horario
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    //Gira o Motor B no sentido horario
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(200);
  }
  rotacao_Parado();
}

// Função para colocar o carrinho na melhor distância, isto é, girá-lo para a melhor distância
void posicionaCarroMelhorCaminho() {
  char melhorDist = calculaMelhorDistancia();
  Serial.print("melhor Distancia em cm: ");
  Serial.println(melhorDist);
  if (melhorDist == 'c') {
    pensar();
  } else if (melhorDist == 'd') {
    rotacao_Direita();
  } else if (melhorDist == 'e') {
    rotacao_Esquerda();
  } else {
    rotacao_Re();
  }
  reposicionaServoSonar();
}

// Função para captar as distâncias lidas e calcular a melhor distância. Acesse: Seu Robô http://seurobo.com.br/
char calculaMelhorDistancia() {
  int esquerda = calcularDistanciaEsquerda();
  int centro = calcularDistanciaCentro();
  int direita = calcularDistanciaDireita();
  reposicionaServoSonar();
  int maiorDistancia = 0;
  char melhorDistancia = '0';

  if (centro > direita && centro > esquerda) {
    melhorDistancia = 'c';
    maiorDistancia = centro;
  } else if (direita > centro && direita > esquerda) {
    melhorDistancia = 'd';
    maiorDistancia = direita;
  } else if (esquerda > centro && esquerda > direita) {
    melhorDistancia = 'e';
    maiorDistancia = esquerda;
  }
  if (maiorDistancia <= distanciaObstaculo) { //distância limite para parar o robô
    rotacao_Re();
    posicionaCarroMelhorCaminho();
  }
  reposicionaServoSonar();
  return melhorDistancia;
}

// Função para calcular a distância da direita
int calcularDistanciaDireita() {
  servo_ultra_sonico.write(0);
  delay(200);
  int leituraDoSonar = lerSonar();
  delay(500);
  leituraDoSonar = lerSonar();
  delay(500);
  Serial.print("Distancia da Direita: ");
  Serial.println(leituraDoSonar);
  return leituraDoSonar;
}

// Função para calcular a distância do centro
int calcularDistanciaCentro() {
  servo_ultra_sonico.write(90);
  delay(20);
  int leituraDoSonar = lerSonar();  // Ler sensor de distância
  delay(500);
  leituraDoSonar = lerSonar();
  delay(500);
  Serial.print("Distancia do Centro: "); // Exibe no serial
  Serial.println(leituraDoSonar);
  return leituraDoSonar;       // Retorna a distância
}

// Função para calcular a distância da esquerda
int calcularDistanciaEsquerda() {
  servo_ultra_sonico.write(180);
  delay(200);
  int leituraDoSonar = lerSonar();
  delay(500);
  leituraDoSonar = lerSonar();
  delay(500);
  Serial.print("Distancia Esquerda: ");
  Serial.println(leituraDoSonar);
  return leituraDoSonar;
}

// Função para ler e calcular a distância do sensor ultrassônico
int lerSonar() {
  digitalWrite(trigPin, LOW); //não envia som
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //envia som
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); //não envia o som e espera o retorno do som enviado
  duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
  distancia_cm = duracao / 56; //Calcula a distância
  delay(30);
  return distancia_cm;             // Retorna a distância
}

// Função para deixar o sensor "olho" do robô no centro
void reposicionaServoSonar() {
  servo_ultra_sonico.write(90);
  delay(200);
}

void displayInfo()
{

  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6); //latitude
    Serial.print(";");
    Serial.print(gps.location.lng(), 6); //longitude
    Serial.print(";");
  }
  else
  {
    Serial.print("INVALID"); //latitude
    Serial.print(";");
    Serial.print("INVALID"); //longitude
    Serial.print(";");
  }

  if (gps.date.isValid())
  {
    Serial.print(gps.date.day()); //dia
    Serial.print("/");
    Serial.print(gps.date.month()); //mes
    Serial.print("/");
    Serial.print(gps.date.year()); //ano
    Serial.print(";");
  }
  else
  {
    Serial.print("INVALID");
    Serial.print(";");
  }

  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour()); //hora
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute()); //minuto
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second()); //segundo
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.print(gps.time.centisecond());
    Serial.print(";");
  }
  else
  {
    Serial.print("INVALID");
    Serial.print(";");
  }
  Serial.println();
}
