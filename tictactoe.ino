/* TIC TAC TOE - Jogo da Velha
 * 
 * Codigo para um jogo da velha iterativo usando o Arduino UNO
 * e ShiftRegisters para um total de 18 pinos de saída e 9 de entrada,
 * totalizando 27 pinos de IO. Utilizado para a avaliação prática da
 * disciplina de Microcontroladores da Anhanguera UNIAN - ABC no 2sem
 * de 2018.
 * 
 * Autores:
 *   Grupo 1 - Anhanguera 2sem 2018
 *   Integrantes:
 *     Fernando Aranão
 * 
 *   Orientador: Prof. Enrico Davini Neto
 *     email: enrico.neto@kroton.com.br
 *   
 * Licença: Este código é de Domínio Público sobre a licença Creative Commons
 */


//funcao que grava os LEDs usando o shiftOut para os bits 0-7 e
//digitalWrite para o bit mais significativo (bit 8 - MSB)
void gravarLED(int valor, int dataPin, int clockPin, int lastBitPin){
  int valorByte = valor & 0xFF; //extrai os 8 primeiros bits do valor
  int bitMSB = valor >> 8; //extrai o bit mais significativo
  digitalWrite(A5, LOW); //coloca latch em low
  shiftOut(dataPin, clockPin, MSBFIRST, valorByte);
  digitalWrite(A5, HIGH); //coloca latch em high
  digitalWrite(lastBitPin, bitMSB);
}

//grava os LEDS vermelhos, definindo os pinos abaixo para o shiftregister:
//8: pino de dados
//9: pino de clock
//10: pino do bit mais significativo (MSB)
void gravarVerm(int valor){
  gravarLED(valor, 8, 9, 10);
}

//grava os LEDS verdes, definindo os pinos abaixo para o shiftregister:
//11: pino de dados
//12: pino de clock
//13: pino do bit mais significativo (MSB)
void gravarVerde(int valor){
  gravarLED(valor, 11, 12, 13);
}

int lerBotoes(){
  //primeiro le-se o botão A0, ref ao bit mais significativo
  //do botao 8 e o move para a posicao certa
  int valorLido = digitalRead(A0) << 8;
  //move o valor de PORTD para o restante dos bits
  valorLido = valorLido | PIND; 
  return valorLido;
}

//variaveis
int accVermelho = 0; //acumulador que guarda o estado do jogador vermelho
int accVerde = 0;  //acumulador que guarda o estado do jogador verde
int acc = 0; //acumulador que guarda o estado dos dois jogadores
int vezJogador = 0; //variavel para alternar as vezes dos jogadores
                    //0: Jogador vermelho
                    //1: Jogador verde

void atualizaAccs(int valor){
  if((acc & valor) != valor){ //verifica se a casa esta ocupada
    acc = acc | valor; //ocupa a casa
    if(vezJogador == 0){
      accVermelho = accVermelho | valor;
      gravarVerm(accVermelho);
      vezJogador = 1; //altera a vez
    } else{
      accVerde = accVerde | valor;
      gravarVerde(accVerde);
      vezJogador = 0; //altera a vez
    }
  }
}

//verifica se alguem ganhou baseado no valor do acc
int verificaVencedor(int valorAcc){ 
  if(valorAcc == 0x1C0 || valorAcc == 0x038 || valorAcc == 0x007 || //verifica linhas horizontais
     valorAcc == 0x124 || valorAcc == 0x092 || valorAcc == 0x049 || //verifica linhas verticais
     valorAcc == 0x111 || valorAcc == 0x054){ //verifica diagonais
       return HIGH;
     } else{
       return LOW;
     }
}

//pisca os LEDs 10 vezes. 0: Verm, 1: Verde e 3: Os dois
void piscarLEDs(int LED){ 
  int i;
  int valor = 0x1FF;
  for(i = 0; i<10; i++){
    if(LED == 0)
      gravarVerm(valor);
    else if(LED == 1)
      gravarVerde(valor);
    else{
      gravarVerm(valor);
      gravarVerde(valor);
    }
    delay(500);
    valor = valor ^ 0x1FF; //inverte os valores
  }
}

void finalizaEReseta(){
    while(lerBotoes() == 0x1FF); //enquanto nenhum botão for apertado, aguarda

    //quando algum botao for apertado, reseta as variaveis e grava na saida
    acc = 0;
    accVermelho = 0;
    accVerde = 0;
    gravarVerm(accVermelho);
    gravarVerde(accVerde);
  
    while(lerBotoes() != 0x1FF); //aguarda soltar o botao
  }

void setup() {
 DDRB = 0xFF; //coloca os pinos 8-13 como saida

 PORTD = 0xFF; //habilita os resistores pull-up dos pinos 0-7
 DDRD = 0x00; //e coloca os pinos 0-7 como entrada p/ os botoes

 pinMode(A0, INPUT_PULLUP); //coloca o pino A0 como entrada, ref. ao botao 8
 pinMode(A5, OUTPUT);
}

void loop(){
  int statusBotao = lerBotoes();

  if(statusBotao != 0x1FF){ //se algum botão foi apertado
    
     //verifica qual botão foi apertado
     switch(statusBotao){
      case 0x1FE: //botao 0 apertado
        atualizaAccs(0x01);
        while(digitalRead(0) == LOW); //aguarda soltar botao
        break;
      case 0x1FD: //botao 1 apertado
        atualizaAccs(0x02);
        while(digitalRead(1) == LOW); //aguarda soltar botao
        break;
      case 0x1FB: //botao 2 apertado
        atualizaAccs(0x04);
        while(digitalRead(2) == LOW); //aguarda soltar botao
        break;
      case 0x1F7: //botao 3 apertado
        atualizaAccs(0x08);
        while(digitalRead(3) == LOW); //aguarda soltar botao
        break;
      case 0x1EF: //botao 4 apertado
        atualizaAccs(0x10);
        while(digitalRead(4) == LOW); //aguarda soltar botao
        break;
      case 0x1DF: //botao 5 apertado
        atualizaAccs(0x20);
        while(digitalRead(5) == LOW); //aguarda soltar botao
        break;
      case 0x1BF: //botao 6 apertado
        atualizaAccs(0x40);
        while(digitalRead(6) == LOW); //aguarda soltar botao
        break;
      case 0x17F: //botao 7 apertado
        atualizaAccs(0x80);
        while(digitalRead(7) == LOW); //aguarda soltar botao
        break;
      case 0x0FF: //botao 8 apertado
        atualizaAccs(0x100);
        while(digitalRead(A0) == LOW); //aguarda soltar botao
        break;
    }

    //verifica se alguem ganhou
    if(verificaVencedor(accVermelho) == HIGH){ //verifica se vermelhor ganhou
      gravarVerde(0x00);
      piscarLEDs(0);
      gravarVerm(0x1FF); //liga todos os LEDs vermelhos
      finalizaEReseta();
    } else if(verificaVencedor(accVerde) == HIGH){ //verifica se o verde ganhou
      gravarVerm(0x00);
      piscarLEDs(1);
      gravarVerde(0x1FF); //liga todos os LEDs verdes
      finalizaEReseta();
    } else if(acc == 0x1FF){ //se ninguem ganhou e o acumulador está cheio -> EMPATE
      piscarLEDs(3);
      gravarVerm(0x1FF); //liga todos os LEDs vermelhos
      gravarVerde(0x1FF); //liga todos os LEDs verdes
      finalizaEReseta();
    }
  }
}


