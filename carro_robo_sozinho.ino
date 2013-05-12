/*
  Programa para o carro robo andar sozinho.
  O carro pode ser comprado em:
  http://dx.com/p/arduino-compatible-bluetooth-controlled-robot-car-kits-146418
*/
#include <Servo.h>

#define trigPin 13
#define echoPin 12
#define servoPin 9

int ENA=5;
int IN1=2; // Gira as rodas da direita para traz
int IN2=3; // Gira as rodas da direita para frente
int ENB=6;
int IN3=4; // Gira as rodas da esquerda para traz
int IN4=7; // Gira as rodas da esquerda para frente

/*
 Servo motor que fica na frente do carrinho e controla a direcao do ultrasonico.
*/
Servo cabeca;

// Inicializa os componentes do carrinho
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  cabeca.attach(servoPin);
  Serial.begin (9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  
  cabeca.write(90);
}

// Movimenta o carrinho para frente
void paraFrente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(100);
}

// Movimenta o carrinho para traz
void paraTraz() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(100);
}

// Movimenta o carrinho para esquerda
void paraEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(400);
}

// Movimenta o carrinho para direita
void paraDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(400);
}

// Para o carrinho
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/*
  Utilizando o ultrasonico que esta na frente do carrinho, verifica a distancia de algo
  na frente do carro.
*/
int obterDistancia(int qtd) {
  // Configura e obtem a distancia
  int duracao = 0, distancia = 0;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duracao = pulseIn(echoPin, HIGH);
  distancia = (duracao / 2) / 29.1;
  if (distancia <= 0 || distancia >= 200) {
    // Se nao conseguir obter a distancia, verifica mais uma vez.
    if(qtd < 3) { // Tenta verificar no maximo 3 vezes a distancia em uma direcao.
      distancia = obterDistancia(qtd++);
    } else {
      distancia = 0;
    }
  }
  
  // retorna a distancia em centimetros
  return distancia;
}

/*
  Esta funcao escolhe qual a melhor direcao que o carro deve seguir. Utilizando o servo motor localizado
  na frente do carrinho, verifica a distancia da direita e da esquerda, a direcao que tiver mais espaco
  sera a direcao retornada.
*/
void escolherDirecao() {
  int direita = 0, esquerda = 0;
  int distancia = obterDistancia(1);
  /*
    Se algo estiver a menos de 15cm da frente do carrinho entao para o carro e verifica 
    qual direcao (direita ou esquerda) ele deve ir.
  */
  if(distancia < 15) {
    parar();
    cabeca.write(0);
    direita = obterDistancia(1);
    delay(500);
    
    cabeca.write(180);
    esquerda = obterDistancia(1);
    delay(500);
    
    if(direita >= esquerda) {
      paraDireita();
    } else if (esquerda > direita) {
      paraEsquerda();
    }
    
    cabeca.write(90);
  }
}

void loop() {
  //Vira o carro para a direcao que houver mais espaco
  escolherDirecao();
  //Faz o carro seguir em frente.
  paraFrente();
}
