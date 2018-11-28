//@Autor: Ednilson Brito Lopes

//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>

//Define os pinos para o trigger e echo
#define trigger 8
#define echo 9
//Deine os pinos para a Ponte H
#define in1 4
#define in2 5
#define in3 6
#define in4 7
#define velocidadeA 3
#define velocidadeB 2

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(trigger, echo);

//Define as variaveis globais
float distancia = 0.0; //Distância em centímetros.

boolean flgDirecao = true; //Flag de direção do carro| TRUE = Frente, FALSE = Ré
float distanciaMinima = 15.0;
float distanciaMedia = 80.0;
float distanciaAlta = 200.0;

int velocidadeBaixa = 80;
int velocidadeMedia = 150;
int velocidadeAlta = 230;

int velocidadeBaixaB = 85;
int velocidadeMediaB = 155;
int velocidadeAltaB = 235;

void setup()
{
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  //Define os pinos da ponte H como saída.
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(velocidadeA, OUTPUT);
  pinMode(velocidadeB, OUTPUT);
}

void loop()
{
  Serial.print("Distancia segura? = ");
  Serial.println(isDistanciaSegura());
  verificaDirecao();
  verificaVelocidade();
  //  if (isDistanciaSegura())
  //  {
  //Gira o Motor A no sentido anti-horario
  //   digitalWrite(in1, LOW);
  //   digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  //   digitalWrite(in3, LOW);
  //   digitalWrite(in4, HIGH);
  //  }
  // else
  // {
  //Gira o Motor A no sentido horario
  //   digitalWrite(in1, HIGH);
  //   digitalWrite(in2, LOW);
  //Gira o Motor B no sentido horario
  //   digitalWrite(in3, HIGH);
  //   digitalWrite(in4, LOW);
}

void verificaDirecao()
{
  if (isDistanciaSegura())
  {
    flgDirecao = true;
  }
  else
  {
    flgDirecao = false;
  }
}

void verificaVelocidade()
{
  if (flgDirecao)
  {
    if (distancia > distanciaAlta)
    {
      velocidadeAltaCarro();
    }
    else if (distancia > distanciaMedia)
    {
      velocidadeMediaCarro();
    }
    else if (distancia > distanciaMinima)
    {
      velocidadeBaixaCarro();
    }
  }
  else
  {
    andaParaTras();
    virar();
  }
}

void velocidadeAltaCarro()
{
  //Define a velocidade alta
  analogWrite(velocidadeA, velocidadeAlta);
  analogWrite(velocidadeB, velocidadeAltaB);
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void velocidadeMediaCarro()
{
  //Define a velocidade média
  analogWrite(velocidadeA, velocidadeMedia);
  analogWrite(velocidadeB, velocidadeMediaB);
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void velocidadeBaixaCarro()
{
  //Define a velocidade baixa
  analogWrite(velocidadeA, velocidadeBaixa);
  analogWrite(velocidadeB, velocidadeBaixaB);
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido anti-horario
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void andaParaTras()
{
  if (!flgDirecao)
  {
    //Define a velocidade média
    analogWrite(velocidadeA, velocidadeMedia);
    analogWrite(velocidadeB, velocidadeMediaB);

    //Gira o Motor A no sentido horario
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    //Gira o Motor B no sentido horario
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(700);
    flgDirecao = true;
  }
}
void virar()
{
  //Define a velocidade média
    analogWrite(velocidadeA, velocidadeBaixa);
    analogWrite(velocidadeB, velocidadeBaixaB);
  //Gira o Motor A no sentido anti-horario
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //Gira o Motor B no sentido horario
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(800);
}

//Valida se a distancia é uma distancia segura.
boolean isDistanciaSegura()
{
  boolean b = false;
  long microsec = ultrasonic.timing();
  distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (flgDirecao)
  {
    if (distancia > distanciaMinima)
    {
      b = true;
    }
  }
  return b;
}
