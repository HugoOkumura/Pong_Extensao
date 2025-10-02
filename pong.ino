#include <TVout.h>
#include <video_gen.h>
#include <fontALL.h>

#define MENU 1
#define JOGO 2
#define VITORIA 3

TVout TV;
int x = 0;
uint8_t circle_x;
uint8_t circle_y;
int dir_x; //1=direita, -1=esquerda
int dir_y = 0; //1=cima, 0=reto, -1=baixo
int p1_cima, p1_baixo, p2_cima, p2_baixo;
int p1_pontuacao=0, p2_pontuacao=0;
unsigned long clique = 0;
char last_state = (1<<PD0);
uint8_t estado;
uint8_t jogador_vitorioso;
char leitura;

void setup() {

  // TV.begin(_PAL, 640, 360);
//  TV.begin(_NTSC,128,72);
   TV.begin(NTSC);
   circle_x = TV.hres()/2;
   circle_y = TV.vres()/2;
   DDRD &= ~(1<<PD0); // PD6 entrada
   PORTD |= (1<<PD0); // PD6 pull-up
   //Aleatoriza a 'seed'
   srand(analogRead(A0) + analogRead(A1));
   estado = MENU;

   //Decide qual direção ele vai ir no começo da partida.
   if((rand() % 100) >= 50){
    dir_x = 1; // Esquerda -> Direita
   }
   else{
    dir_x = -1; // Direita -> Esquerda
   }
//  TV.begin(_PAL,128,72);
  // TV.begin(_PAL);
  //Serial.begin(9600);
}

void loop() {
  if(estado == MENU){
    menu();
  }
  else if(estado == JOGO){
    jogo();
  }else if(estado == VITORIA){
    vitoria();
  }

  TV.delay_frame(7);  
  TV.clear_screen();

}

void menu(){
    TV.draw_rect(0, 0, 122, 88, WHITE);
    TV.select_font(font6x8);
    TV.print(45, 10, "P O N G");
    
    TV.select_font(font4x6);
    TV.print(50,50, "INICIAR");

    leitura = PIND & (1<<PD0);
  
    if (leitura!=last_state && (millis()-clique)>1) {
      clique = millis();
 
      if (leitura == 0){

          estado = JOGO;
      }
    }
  
  last_state = leitura;

}

void jogo(){
  // TV.select_font(font8x8);
  // Serial.println(TV.hres());
  // Serial.printlnS(TV.vres());
  // TV.fill(WHITE);
  
  TV.draw_circle(circle_x, circle_y, 2, WHITE, WHITE);
 
//  for(int i = 2; i < TV.hres()-4; i+=2){
//  TV.draw_circle(i, TV.vres()/2, 2, WHITE,WHITE);
//  TV.select_font(font4x6);
//  TV.print(0,0,i);
//  delay(50);
//  TV.clear_screen();
//  }
//  for(int j = TV.hres()-4; j > 2; j-=2){
//  TV.draw_circle(j, TV.vres()/2, 2, WHITE,WHITE);
//  TV.print(0,0,j);
//  delay(50);
//  TV.clear_screen();
//  }
  TV.draw_rect(0, 6, 122, 88, WHITE);

  x = analogRead(A0);
  TV.select_font(font4x6);
  // TV.print(8,0,x);
  x = map(x, 0, 1023, 20, 80);

  // TV.print(8,6,x);
  TV.print(8,0, p1_pontuacao);
  p1_cima = x+12;
  p1_baixo = x-12;
  TV.draw_line(5, p1_baixo, 5, p1_cima, WHITE);
  // TV.set_pixel(x, TV.vres()/2 - 10, WHITE);
  // TV.set_pixel(5, x, WHITE);

  x = analogRead(A1);
  TV.select_font(font4x6);
  // TV.println(108,0,x);
  // x = map(x, 0, 1023, 12, 110);
  x = map(x, 0, 1023, 20, 80);
  // TV.println(108,6,x);
  //TV.println(108,0,x);
  TV.print(108,0, p2_pontuacao);
  // TV.set_pixel(x, TV.vres()/2 + 10, WHITE);
  // TV.set_pixel(117, x, WHITE);
  p2_cima = x+12;
  p2_baixo = x-12;
  TV.draw_line(117, p2_baixo, 117, p2_cima, WHITE);

  //Comentado 18/09/2025
  //TV.print(4,72,"Resolucao: ");
  //x = TV.hres();
  //TV.print(x);
  //TV.print("x");
  //x = TV.vres();
  //TV.print(x);
 // delay(15);

  circle_x = circle_x + dir_x*2;
  circle_y = circle_y + dir_y;

  //colisão da bolinha
  //batendo no P1 ou na quina
  if(circle_x == 8 || circle_x == 7){
  //bateu no meio
  if(circle_y <= p1_cima-6 && circle_y >= p1_baixo+6){
    dir_y = 0;
    dir_x *= -1;
  }
  //bateu em cima
  else if(circle_y <= p1_cima && circle_y >= p1_cima-5){
    dir_y= 1;
    dir_x *= -1;
  }
  //bateu em baixo
  else if(circle_y >= p1_baixo && circle_y <= p1_baixo+5){
    dir_y= -1;
    dir_x *= -1;
  }
  }
  //batendo no P2 ou na
  if(circle_x == 115 || circle_x ==114){
  //bateu no meio
  if(circle_y <= p2_cima-6 && circle_y >= p2_baixo+6){
    dir_y = 0;
    dir_x *= -1;
  }
  //bateu em cima
  else if(circle_y <= p2_cima && circle_y >= p2_cima-5){
    dir_y= 1;
    dir_x *= -1;
  }
  //bateu em baixo
  else if(circle_y >= p2_baixo && circle_y <= p2_baixo+5){
    dir_y= -1;
    dir_x *= -1;
  }
  }

  //colisão em cima
  if(circle_y == 8){
  dir_y *= -1;
  }
 
  //colisão em baixo
  if(circle_y == 91){
  dir_y *= -1;
  }

  //Bateu na direita
  if(circle_x >= 120){
  dir_x *= -1;
  dir_y= 0;
  p1_pontuacao += 1; //Aumenta pontuação do Player 1

  if(p1_pontuacao == 5){

    estado = VITORIA;

    jogador_vitorioso = 1;
  }

  //Inicializa a posição da bola
  circle_x = TV.hres()/2;
  circle_y = TV.vres()/2;
  }
  //Bateu na esquerda
  else if(circle_x <= 2){
  dir_x *= -1;
  dir_y= 0;
  p2_pontuacao += 1; //Aumenta pontuação do Player 2

  if(p2_pontuacao == 5){

    estado = VITORIA;

    jogador_vitorioso = 2;
  }

  //Inicializa a posição da bola
  circle_x = TV.hres()/2;
  circle_y = TV.vres()/2;
  }

}

void vitoria(){  
    TV.draw_rect(0, 0, 122, 88, WHITE);
    TV.select_font(font6x8);
    
    TV.print(45, 10, "P O N G");
    
    if(jogador_vitorioso == 1){
      TV.println(12, TV.vres()/2, "JOGADOR 1 VENCEU");
    }
    else{
      TV.println(12, TV.vres()/2, "JOGADOR 2 VENCEU");
    }
    TV.select_font(font4x6);
    TV.print(2,(TV.vres()/2)+20, "Pressione o botao para o menu.");

    leitura = PIND & (1<<PD0);
    if (leitura!=last_state && (millis()-clique)>1) {
      clique = millis();
 
      if (leitura == 0){
          estado = MENU;
          p1_pontuacao = 0;
          p2_pontuacao = 0;
      }
    }
    last_state = leitura;
}
