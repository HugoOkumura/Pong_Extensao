#include <TVout.h>
#include <fontALL.h>
#include <video_gen.h>

#define MENU 1
#define JOGO 2
#define VITORIA 3
#define TREINO 4
#define TREINO_FIM 5
#define VEL_X 2
#define VEL_Y 2

TVout TV;
int x = 0;
uint8_t circle_x;
uint8_t circle_y;
int dir_x;      // 1=direita, -1=esquerda
int dir_y = 0;  // 1=cima, 0=reto, -1=baixo
int p1_cima, p1_baixo, p2_cima, p2_baixo;
int p1_pontuacao = 0, p2_pontuacao = 0;
unsigned long clique = 0;
unsigned long clique_2 = 0;
char last_state = (1 << PD0);
char last_state_2 = (1 << PD1);
uint8_t estado;
uint8_t selecao;
uint8_t jogador_vitorioso;
char leitura;
char controle;

void setup() {
  TV.begin(_NTSC, 128, 72);
  TV.begin(NTSC);
  circle_x = TV.hres() / 2;
  circle_y = TV.vres() / 2;
  DDRD &= ~(1 << PD0);  // PD6 entrada
  PORTD |= (1 << PD0);  // PD6 pull-up
  // Aleatoriza a 'seed'
  srand(analogRead(A0) + analogRead(A1));
  estado = MENU;
  selecao = JOGO;

  // Decide qual direção ele vai ir no começo da partida.
  if ((rand() % 2) == 0) {
    dir_x = 1;    // Direita -> Esquerda
  } else {
    dir_x = -1;   // Direita <- Esquerda
  }
}

void menu();
void jogo();
void treino();
void vitoria();
void treino_fim();

void loop() {
  srand(analogRead(A0) + analogRead(A1));

  if (estado == MENU) {
    menu();
  }
  if (estado == JOGO) {
    jogo();
  }
  if (estado == TREINO) {
    treino();
  }
  if (estado == VITORIA) {
    vitoria();
  }
  if (estado == TREINO_FIM) {
    treino_fim();
  }

  TV.delay_frame(3);
  TV.clear_screen();
}

void menu() {
  TV.draw_rect(0, 0, 122, 88, WHITE);
  TV.select_font(font6x8);
  TV.print(42, 10, "P O N G");

  TV.select_font(font4x6);
  TV.print(55, 50, "PVP");
  TV.print(55, 60, "TREINO");
  if (selecao == JOGO) // Seta para mostrar opção selecionada
  {
    TV.print(50, 50, ">");
  } else {
    TV.print(50, 60, ">");
  }

  leitura = PIND & (1 << PD0);
  controle = PIND & (1 << PD1);


  if (leitura != last_state && (millis() - clique) > 1) // Botão para escolher opção selecionada
  {
    clique = millis();
    if (leitura == 0) 
    {
      estado = selecao;
    }
  }
  last_state = leitura;

  if (controle != last_state_2 && (millis() - clique_2) > 1)  // Botão para alterar opção selecionada
  {
    clique_2 = millis();

    if (controle == 0) {
      if (selecao == JOGO) 
      {
        selecao = TREINO;
      } else {
        selecao = JOGO;
      }
    }
  }

  last_state_2 = controle;
}

void move_jogador_1()
{
  x = analogRead(A0);                           // Lê valor do potenciometro
  x = map(x, 0, 1023, 20, 80);                  // O tranforma em um valor entre 20 e 80
  p1_cima = x + 12;
  p1_baixo = x - 12;                            // Utiliza o valor conseguido com a leitura para determinar a posição do jogador
  TV.draw_line(5, p1_baixo, 5, p1_cima,WHITE);  // Desenha o personagem do jogador
}

void move_jogador_2()
{
  x = analogRead(A1);                               // Lê valor do potenciometro
  x = map(x, 0, 1023, 20, 80);                      // O tranforma em um valor entre 20 e 80
  p2_cima = x + 12;
  p2_baixo = x - 12;                                // Utiliza o valor conseguido com a leitura para determinar a posição do jogador
  TV.draw_line(117, p2_baixo, 117, p2_cima,WHITE);  // Desenha o personagem do jogador
}

void chute_jogador_1()
{
  dir_x = 1;
  if (circle_y <= p1_cima && circle_y > p1_baixo + 16)        // bateu em cima
  {
    dir_y = 1;
  }
  if (circle_y <= p1_cima - 8 && circle_y > p1_baixo + 8)     // bateu no meio
  {
    dir_y = 0;
  }
  if (circle_y <= p1_cima - 16 && circle_y >= p1_baixo)       // bateu em baixo
  {
    dir_y = -1;
  }
}

void chute_jogador_2()
{
  dir_x = -1;
  if (circle_y <= p2_cima && circle_y > p2_baixo + 16)    // bateu em cima
  {
    dir_y = 1;
  }
  if (circle_y <= p2_cima - 8 && circle_y > p2_baixo + 8)    // bateu no meio
  {
    dir_y = 0;
  }
  if (circle_y <= p2_cima - 16 && circle_y >= p2_baixo)    // bateu em baixo
  {
    dir_y = -1;
  }
}

void jogo()
{
  TV.draw_circle(circle_x, circle_y, 2, WHITE, WHITE);  // Desenha a bola
  TV.draw_rect(0, 6, 122, 88, WHITE);                   // Desenha o mapa
  TV.select_font(font4x6);                              // Seleciona fonte para escrever pontuação
  TV.print(8, 0, p1_pontuacao);                         // Escreve a pontuação do jogador 1
  TV.print(108, 0, p2_pontuacao);                       // Escreve a pontuação do jogador 2

  move_jogador_1();
  move_jogador_2();

  circle_x = circle_x + dir_x * VEL_X;
  circle_y = circle_y + dir_y * VEL_Y;  // Calcula a posição da bola

  if ((circle_x == 8 || circle_x == 7) && (circle_y <= p1_cima && circle_y >= p1_baixo))  // Colisão no jogador 1
  {
    chute_jogador_1();
  }
  if ((circle_x == 115 || circle_x == 114) && (circle_y <= p2_cima && circle_y >= p2_baixo))  // Colisão no jogador 2
  {
    chute_jogador_2();
  }

  if (circle_y <= 8)  // colisão em cima
  {
    dir_y = -1;
  }

  if (circle_y >= 91)  // colisão em baixo
  {
    dir_y = 1;
  }

  if (circle_x >= 120)  // Bateu no gol atrás do jogador 2
  {
    dir_x = -1;
    dir_y = 0;
    p1_pontuacao++;  // Aumenta pontuação do jogador 1
    if (p1_pontuacao == 5)  // Verifica vencedor
    {
      estado = VITORIA;
      jogador_vitorioso = 1;
    }
    circle_x = TV.hres() / 2;   // Posiciona bola no centro do mapa
    circle_y = TV.vres() / 2;
  }

  if (circle_x <= 2)  // Bateu no gol atrás do jogador 1
  {
    dir_x = 1;
    dir_y = 0;
    p2_pontuacao++;  // Aumenta pontuação do jogador 2
    if (p2_pontuacao == 5)  // Verifica vencedor
    {
      estado = VITORIA;
      jogador_vitorioso = 2;
    }
    circle_x = TV.hres() / 2;   // Posiciona bola no centro do mapa
    circle_y = TV.vres() / 2;
  }
}

void vitoria() {
  TV.draw_rect(0, 0, 122, 88, WHITE);

  TV.select_font(font6x8);
  TV.print(42, 10, "P O N G");

  if (jogador_vitorioso == 1) {
    TV.println(12, TV.vres() / 2, "JOGADOR 1 VENCEU");
  } else {
    TV.println(12, TV.vres() / 2, "JOGADOR 2 VENCEU");
  }
  TV.select_font(font4x6);
  TV.print(2, (TV.vres() / 2) + 20, "Pressione o botao para o menu.");

  leitura = PIND & (1 << PD0);
  if (leitura != last_state && (millis() - clique) > 1) {
    clique = millis();

    if (leitura == 0) {
      estado = MENU;
      p1_pontuacao = 0;
      p2_pontuacao = 0;
    }
  }
  last_state = leitura;
}

void treino() {

  TV.draw_circle(circle_x, circle_y, 2, WHITE, WHITE);  // Desenha a bola
  TV.draw_rect(0, 6, 122, 88, WHITE);                   // Desenha o mapa

  move_jogador_1();

  circle_x = circle_x + dir_x * VEL_X;
  circle_y = circle_y + dir_y * VEL_Y;

  if ((circle_x == 8 || circle_x == 7) && (circle_y <= p1_cima && circle_y >= p1_baixo))  // Colisão no jogador 1
  {
    chute_jogador_1();
  }

  if (circle_y <= 8)  // colisão em cima
  {
    dir_y = -1;
  }

  if (circle_y >= 91)  // colisão em baixo
  {
    dir_y = 1;
  }
  
  if (circle_x >= 120)  // Bateu na direita
  {
    dir_x = -1;
    switch(rand()%3)    // Gera numero aleatorio e o usa para dicidir qual será a direção no eixo Y
    {
      case 0:
      dir_y = 1;
      break;
      case 1:
      dir_y = 0;
      break;
      case 2:
      dir_y = -1;
      break;
    }
  }
  // Bateu na esquerda
  if (circle_x <= 2) {
    estado = TREINO_FIM;
  }
}
void treino_fim() {
  TV.draw_rect(0, 0, 122, 88, WHITE);
  TV.select_font(font6x8);

  TV.print(42, 10, "P O N G");
  TV.println(23, TV.vres() / 2, "FIM DE TREINO");
  TV.select_font(font4x6);
  TV.print(2, (TV.vres() / 2) + 20, "Pressione o botao para o menu.");

  leitura = PIND & (1 << PD0);
  if (leitura != last_state && (millis() - clique) > 1) {
    clique = millis();

    if (leitura == 0) {
      estado = MENU;
      p1_pontuacao = 0;
      p2_pontuacao = 0;
    }
  }
  last_state = leitura;
}
