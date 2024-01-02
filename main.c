#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * Imprime uma ajuda explicando como o programa deve ser usado.
 * @param program_name Nome do arquivo executável.
 */
void print_usage(char *program_name)
{
  printf("Forma de usar:\n");
  printf("    %s <operacao>\n", program_name);
  printf("Onde <operacao> pode ser:\n");
  printf("Outra forma de usar após compilar o código:\n");
  printf("    %s operacao\n", program_name);
  printf("Aparecerá uma mensagem 'Digite o nome da Imagem:' informe o nome da imagem em arquivo PPM.\n");
  printf("    gray: para criar uma versão em escala de cinza da imagem original.\n");
  printf("    enlarge: para ampliar em 2x a imagem original.\n");
  printf("    reduce: para reduzir em 2x a imagem original.\n");
  printf("    rotate: para rotacionar 90 graus (sentido horário) a imagem original.\n");
  printf("    sharp: para aplicar um filtro de sharpening na imagem original.\n");
  printf("    blur: para aplicar um filtro de blurring na imagem original.\n");
  printf("\n");
  printf("A imagem original será lida da entrada-padrão e a imagem transformada será enviada para a saída-padrão.\n\n");
}

// Define uma estrutura para armazenar as informações de cada pixel da imagem
typedef struct
{
  int r, g, b;
} Pixel;

/*
Define uma estrutura para representar a imagem. Contém o tipo da imagem (PPM),
o número de linhas e colunas da imagem, a cor máxima e um ponteiro
para a matriz de pixels
*/
typedef struct
{
  char tipo[3];
  int coluna, linha;
  int corMaxima;
  Pixel **pixels;
} Imagem;
/* Função para criar uma matriz alocada dinâmicamente, ela recebe como parâmetro o ponteiro da variável do
tipo Imagem para manipulação dos pixels.*/
void criarMatriz(Imagem *img)
{
  // Aloca memória para as linhas da matriz
  img->pixels = (Pixel **)malloc(img->linha * sizeof(Pixel *));

  // Aloca memória para as colunas da matriz
  for (int i = 0; i < img->linha; i++)
    img->pixels[i] = (Pixel *)malloc(img->coluna * sizeof(Pixel));
}
/* Função para ler imagem, ela recebe como parâmetro o ponteiro ds variável do tipo Imagem da main para fazer
 a leiturado arquivo PPM.*/
void lerImagem(Imagem *img)
{
  FILE *arquivo;
  char nome_imagem[50];

  // Solicita o nome do arquivo da imagem para o usuário
  printf("Digite o nome da Imagem: ");
  scanf("%s", nome_imagem);

  // Abre o arquivo para leitura
  arquivo = fopen(nome_imagem, "r");

  // Verifica se o arquivo foi aberto
  if (arquivo == NULL)
  {
    printf("Erro ao abrir a Imagem\n");
    exit(1);
  }

  // Lê os dados do tipo, da quantidade de colunas e linhas e da cor máxima
  fscanf(arquivo, "%s", img->tipo);
  fscanf(arquivo, "%d %d", &img->coluna, &img->linha);
  fscanf(arquivo, "%d", &img->corMaxima);

  // Cria a matriz de pixels com base na quantidade de colunas e linhas
  criarMatriz(img);

  // Lê os valores dos pixels da imagem do arquivo
  for (int i = 0; i < img->linha; i++)
    for (int j = 0; j < img->coluna; j++)
      fscanf(arquivo, "%d %d %d", &img->pixels[i][j].r, &img->pixels[i][j].g, &img->pixels[i][j].b);

  // Fecha o arquivo após a leitura
  fclose(arquivo);
}
/* Função para salvar a imagem, ela recebe como parâmetro os ponteiros das variáveis do tipo char para salvar de acordo com
o efeito e do tipo Imagem para receber o endereço da imagem já manipuladda.*/
void saveImagem(const char *operation, Imagem *img)
{
  FILE *novo_arquivo;

  // Abre ou cira um arquivo conforme o nome da operação realizada e adicona na pasta imgs_saidas.
  if (strcmp(operation, "gray") == 0)
    novo_arquivo = fopen("imgs_saidas/gray.ppm", "w");

  else if (strcmp(operation, "enlarge") == 0)
    novo_arquivo = fopen("imgs_saidas/enlarge.ppm", "w");

  else if (strcmp(operation, "reduce") == 0)
    novo_arquivo = fopen("imgs_saidas/reduce.ppm", "w");

  else if (strcmp(operation, "rotate") == 0)
    novo_arquivo = fopen("imgs_saidas/rotate.ppm", "w");

  else if (strcmp(operation, "sharp") == 0)
    novo_arquivo = fopen("imgs_saidas/sharp.ppm", "w");

  else if (strcmp(operation, "blur") == 0)
    novo_arquivo = fopen("imgs_saidas/blur.ppm", "w");

  /*
  É adicionado ao novo arquivo, após as modificações na imagem,
  o tipo da imagem (PPM), a quantidade de colunas e linhas e a
  cor máxima.
  */
  fprintf(novo_arquivo, "%s\n", img->tipo);
  fprintf(novo_arquivo, "%d %d\n", img->coluna, img->linha);
  fprintf(novo_arquivo, "%d\n", img->corMaxima);

  // Adiciona ao novo arquivo os pixels da nova imagem
  for (int i = 0; i < img->linha; i++)
      for (int j = 0; j < img->coluna; j++)
        fprintf(novo_arquivo, "%d %d %d ", img->pixels[i][j].r, img->pixels[i][j].g, img->pixels[i][j].b);
      fprintf(novo_arquivo, "\n");

    // Fecha o novo arquivo
    fclose(novo_arquivo);

    // Libera a memória alocada
    free(img->pixels);
}
// Função para aplicar a escala cinza na imagem, ela recebe como parâmetro o ponteiro da variável Imagem já lida do arquivo.
void imgGray(Imagem *img)
{
  // A variável media irá armazenar a media dos pixels (r, g, b)
  int media;
  for (int i = 0; i < img->linha; i++)
  {
    for (int j = 0; j < img->coluna; j++)
    {
      // Calcula a média dos valores r, g, b do pixel atual.
      media = (img->pixels[i][j].r + img->pixels[i][j].g + img->pixels[i][j].b) / 3;

      // Define o valores r, g, b de acordo com a média, convertendo para uma imagem cinza
      img->pixels[i][j].r = media;
      img->pixels[i][j].g = media;
      img->pixels[i][j].b = media;
    }
  }
}
// Função para ampliar a imagem, ela recebe como parametro o ponteiro da variável Imagem já lida do arquivo.
void *imgEnlarge(Imagem *img){
  // Cria um novo arquivo para escrita do efeito enlarge.
  FILE *arquivo = fopen("imgs_saidas/enlarge.ppm", "w");

  // Variável para acrescentar novas linhas, e fazer a média dos pixels.
  int novaLinhaR, novaLinhaG, novaLinhaB;
  int mediaR, mediaG, mediaB;

  // imprime no arquivo de saída o cabeçalho da imagem PPM. 
  fprintf(arquivo, "%s\n", img->tipo);
  fprintf(arquivo, "%d %d\n%d\n", img->coluna*2-1, img->linha*2-1, img->corMaxima);

  // Loop para percorrer a matriz de pixels da imagem original.
  for(int i=0; i<img->linha; i++){
    // Verifica se não é a última linha
    if(i != img->linha - 1){
      for(int j=0; j<img->coluna-1; j++){
        // Imprime os valores RGB do pixel atual e a média calculada no arquivo de saída.
        mediaR = (img->pixels[i][j].r + img->pixels[i][j+1].r) / 2;
        mediaG = (img->pixels[i][j].g + img->pixels[i][j+1].g) / 2;
        mediaB = (img->pixels[i][j].b + img->pixels[i][j+1].b) / 2;

        // É gravado no arquivo os pixels da coluna original e os pixels da nova coluna
        fprintf(arquivo, "%d %d %d ", img->pixels[i][j].r, img->pixels[i][j].g, img->pixels[i][j].b);
        fprintf(arquivo, "%d %d %d ", mediaR, mediaG, mediaB);

        // Verifica se é o último pixel da linha e imprime seus valores RGB no arquivo.

        if(j == img->coluna-2){
        fprintf(arquivo, "%d %d %d ", img->pixels[i][j+1].r, img->pixels[i][j+1].g, img->pixels[i][j+1].b);
        fprintf(arquivo, "\n");
        }
    }

    // Loop para criar novas linhas duplicando a linha atual e fazendo a média com a próxima linha.
    for(int k=0; k<img->coluna-1; k++){
      // Calcula a média dos valores R, G e B entre o pixel atual e o correspondente na próxima linha.
      novaLinhaR = (img->pixels[i][k].r + img->pixels[i+1][k].r) / 2;
      novaLinhaG = (img->pixels[i][k].g + img->pixels[i+1][k].g) / 2;
      novaLinhaB = (img->pixels[i][k].b + img->pixels[i+1][k].b) / 2;

       // Retira a media dos pixels da nova linha[i][k] com a nova linha[i][k+1] + nova linha[i+1][k+1].

      mediaR = (novaLinhaR + (img->pixels[i][k+1].r + img->pixels[i+1][k+1].r)/2) / 2;
      mediaG = (novaLinhaG + (img->pixels[i][k+1].g + img->pixels[i+1][k+1].g)/2) / 2;
      mediaB = (novaLinhaB + (img->pixels[i][k+1].b + img->pixels[i+1][k+1].b)/2) / 2;

      // Imprime os valores RGB dos pixels da nova linha e a média calculada no arquivo de saída.

      fprintf(arquivo, "%d %d %d ", novaLinhaR, novaLinhaG, novaLinhaB);
      fprintf(arquivo, "%d %d %d ", mediaR, mediaG, mediaB);

      // Verifica se é o último pixel da linha e imprime seus valores RGB no arquivo.

      if(k == img->coluna-2){
        fprintf(arquivo, "%d %d %d ", novaLinhaR, novaLinhaG, novaLinhaB);
        fprintf(arquivo, "\n");
        }
      }
    } 
    else{
      // Laço para preencher a última linha
      for(int l=0; l<img->coluna-1; l++){
        mediaR = (img->pixels[i][l].r + img->pixels[i][l+1].r) / 2;
        mediaG = (img->pixels[i][l].g + img->pixels[i][l+1].g) / 2;
        mediaB = (img->pixels[i][l].b + img->pixels[i][l+1].b) / 2;

        // É gravado no arquivo os pixels da coluna original e os pixels da nova coluna

        fprintf(arquivo, "%d %d %d ", img->pixels[i][l].r, img->pixels[i][l].g, img->pixels[i][l].b);
        fprintf(arquivo, "%d %d %d ", mediaR, mediaG, mediaB);

        // Verifica se é o último pixel da linha e imprime seus valores RGB no arquivo.
        
        if(l == img->coluna-2){
          fprintf(arquivo, "%d %d %d ", img->pixels[i][l+1].r, img->pixels[i][l+1].g, img->pixels[i][l+1].b);
          fprintf(arquivo, "\n");
          }
      }
    }
  }
}
// Função para reduzir a imagem, ela recebe como parâmetro o ponteiro da variável Imagem já lida do arquivo.
void imgReduce(Imagem *img)
{ 
  // cria uma variável imagem_aux do tipo imagem, para auxiliar na manipulação da imagem.
  Imagem imagem_aux;

  // Atribui valores de linha e coluna da variável com os valores da linha e coluna divido por 2 da imagem passada por parâmetro. 
  imagem_aux.linha = img->linha / 2;
  imagem_aux.coluna = img->coluna / 2;

  // Cria a matriz de pixels com base na quantidade de colunas e linhas.
  criarMatriz(&imagem_aux);

  // Laço para atribuir os valores dos pixels da imagem_aux com a media dos pixels da imagem passada por parâmetro para reduzir a imagem. 
  for (int i = 0, k = 1; i < imagem_aux.linha && k < img->linha; i++, k += 2)
    for (int j = 0, l = 1; j < imagem_aux.coluna && l < img->coluna; j++, l += 2)
    {
      imagem_aux.pixels[i][j].r = (img->pixels[k - 1][l - 1].r + img->pixels[k - 1][l].r + img->pixels[k][l - 1].r + img->pixels[k][l].r) / 4;
      imagem_aux.pixels[i][j].g = (img->pixels[k - 1][l - 1].g + img->pixels[k - 1][l].g + img->pixels[k][l - 1].g + img->pixels[k][l].g) / 4;
      imagem_aux.pixels[i][j].b = (img->pixels[k - 1][l - 1].b + img->pixels[k - 1][l].b + img->pixels[k][l - 1].b + img->pixels[k][l].b) / 4;
    }
  // Libera a memória alocada.
  free(img->pixels);

  // Atribui os valores da imagem_aux para os valores da imagem passada por parâmetro.
  img->linha = imagem_aux.linha;
  img->coluna = imagem_aux.coluna;
  img->pixels = imagem_aux.pixels;
}
/* Função para rotacionar a imagem em 90º para direita, ela recebe como parâmetro o ponteiro da variável Imagem 
já lida do arquivo.*/
void imgRotate(Imagem *img)
{
  // Cria uma variável imagem_aux do tipo Imagem, para auxiliar na manipulação da imagem.
  Imagem imagem_aux;

  // Atribui valores de linha e coluna da variável com invertidos da linha e coluna da imagem passada por parâmetro.
  imagem_aux.linha = img->coluna;
  imagem_aux.coluna = img->linha;

  // Cria a matriz de pixels com base na quantidade de colunas e linhas.
  criarMatriz(&imagem_aux);

  // Laço para atribuir os valores dos pixels da imagem passada para a imagem_aux rotacionando 90º para a esquerda.
  for(int j = img->coluna - 1, k = 0; k < img->linha && j >= 0; j--, k++){
		for(int i = 0; i < img->linha; i++){
			imagem_aux.pixels[k][i] = img->pixels[i][j];
		}
	}
  // Cria uma nova variável do tipo Imagem para auxiliar na manipulação da imagem_aux. 
  Imagem imagem_aux2;

  // Atribui os valores de linha e coluna da imagem_aux para imagem_aux2.
  imagem_aux2.linha = imagem_aux.linha;
  imagem_aux2.coluna = imagem_aux.coluna;

  // Cria a matriz de pixels com base na quantidade de colunas e linhas.
  criarMatriz(&imagem_aux2);

  // Laço para fazer a matriz transposta dos pixels da imagem_aux para imagem_aux2.
  for(int i = 0; i < imagem_aux.linha; i++){
    for(int j = 0; j < imagem_aux.coluna; j++){
      imagem_aux2.pixels[i][j].r = imagem_aux.pixels[i][imagem_aux.coluna - j].r;
      imagem_aux2.pixels[i][j].g = imagem_aux.pixels[i][imagem_aux.coluna - j].g;
      imagem_aux2.pixels[i][j].b = imagem_aux.pixels[i][imagem_aux.coluna - j].b;
    }
  }
  // Libera memória alocada.
	free(img->pixels);

  // Atribui os valores da imagem_aux2 para imagem passada por parâmetro.
  img->pixels = imagem_aux2.pixels;
  img->coluna = imagem_aux2.coluna;
  img->linha = imagem_aux2.linha;
}
/* Função para aplicar o filtro Sharp, ela recebe como parâmetro o ponteiro da variável Imagem ja lida do arquivo.*/
void imgSharp(Imagem *img)
{
  // Cria os valores de controle de laço.
  int i = 0, j = 0;
  int k, l;

  // Cria a matrizSharp do tipo inteiro para aplicar o efeito.
  int matrizSharp[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

  // Cria uma variável do tipo pixel para auxiliar a manipulação dos pixels da imagem passada por parâmetro.
  Pixel somapixels = {0, 0, 0};

  // Cria uma variável imagem_aux para auxiliar a manipulação da imagem.
  Imagem imagem_aux;

  // Atribuindo os valores da linha e coluna da imagem recebida por parâmetro para imagem_aux.
  imagem_aux.linha = img->linha;
  imagem_aux.coluna = img->coluna;

  // Cria a matriz de pixels com base na quantidade de colunas e linhas.
  criarMatriz(&imagem_aux);

  // Laço externo para verificar os valores do pixels caso tenha ultrapassado os valores limites.
  do
  {
    k = 0;
    l = 0;
    // Laço interno para atribuir soma dos valores incrementados multipicado pela matrizSharp para aplicar o efeito.
    do{
      somapixels.r += img->pixels[i + k][j + l].r * matrizSharp[k][l];
      somapixels.g += img->pixels[i + k][j + l].g * matrizSharp[k][l];
      somapixels.b += img->pixels[i + k][j + l].b * matrizSharp[k][l];
      if(l == 2){
        l = 0;
        k++;
      }
      else
        l++;
    } while( k < 3);
    if(somapixels.r > 255)
      somapixels.r = img->corMaxima;
    if(somapixels.g > 255)
      somapixels.g = img->corMaxima;
    if(somapixels.b > 255)
      somapixels.b = img->corMaxima;
    if(somapixels.r < 0)
      somapixels.r = 0;
    if(somapixels.g < 0)
      somapixels.g = 0;
    if(somapixels.b < 0)
      somapixels.b = 0;
    imagem_aux.pixels[i + 1][j + 1] = somapixels;
    somapixels.r = 0;
    somapixels.g = 0;
    somapixels.b = 0;
    if(j == img->coluna -3){
      j = 0;
      i++;
    }
    else
      j++;
  } while (i < img->linha -2);

  // Laço para atribur o restantes dos valores do pixels para imagem_aux.
  for(i = 0; i < img->linha; i++){
    for(j = 0; j < img->coluna; j++){
      if(i == 0 || i == img->linha - 1 || j == 0 || j == img->coluna -1){
        imagem_aux.pixels[i][j] = img->pixels[i][j];
      }
    }
  }

  // Libera memória alocada.
  free(img->pixels);

  // Atribui os valores da imagem_aux para imagem passada por parâmetro.
  img->linha = imagem_aux.linha;
  img->coluna = imagem_aux.coluna;
  img->pixels = imagem_aux.pixels;
}
/* Função para aplicar o filtro Blur, ela recebe como parâmetro o ponteiro da variável Imagem ja lida do arquivo.*/
void imgBlur(Imagem *img)
{
  // Cria os valores de controle de laço.
  int i = 0, j = 0;
  int k, l;
  
  // Cria a matrizBlur do tipo inteiro para aplicar o efeito.
  int matrizBlur[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

  // Cria uma variável do tipo pixel para auxiliar a manipulação dos pixels da imagem passada por parâmetro.
  Pixel somapixels = {0, 0, 0};
  // Cria uma variável imagem_aux para auxiliar a manipulação da imagem.
  Imagem imagem_aux;

  // Atribuindo os valores da linha e coluna da imagem recebida por parâmetro para imagem_aux.
  imagem_aux.linha = img->linha;
  imagem_aux.coluna = img->coluna;

  // Cria a matriz de pixels com base na quantidade de colunas e linhas.
  criarMatriz(&imagem_aux);

  // Laço externo para verificar os valores do pixels caso tenha ultrapassado os valores limites.
  do
  {
    k = 0;
    l = 0;
    // Laço interno para atribuir soma dos valores incrementados multipicado pela matrizSharp para aplicar o efeito.
    do{
      somapixels.r += img->pixels[i + k][j + l].r * matrizBlur[k][l];
      somapixels.g += img->pixels[i + k][j + l].g * matrizBlur[k][l];
      somapixels.b += img->pixels[i + k][j + l].b * matrizBlur[k][l];
      if(l == 2){
        l = 0;
        k++;
      }
      else
        l++;
    } while( k < 3);
    somapixels.r /= 9;
    somapixels.g /= 9;
    somapixels.b /= 9;
    if(somapixels.r > 255)
      somapixels.r = img->corMaxima;
    if(somapixels.g > 255)
      somapixels.g = img->corMaxima;
    if(somapixels.b > 255)
      somapixels.b = img->corMaxima;
    if(somapixels.r < 0)
      somapixels.r = 0;
    if(somapixels.g < 0)
      somapixels.g = 0;
    if(somapixels.b < 0)
      somapixels.b = 0;
    imagem_aux.pixels[i + 1][j + 1] = somapixels;
    somapixels.r = 0;
    somapixels.g = 0;
    somapixels.b = 0;
    if(j == img->coluna -3){
      j = 0;
      i++;
    }
    else
      j++;
  } while (i < img->linha -2);

  // Laço para atribur o restantes dos valores do pixels para imagem_aux.
  for(i = 0; i < img->linha; i++){
    for(j = 0; j < img->coluna; j++){
      if(i == 0 || i == img->linha - 1 || j == 0 || j == img->coluna -1){
        imagem_aux.pixels[i][j] = img->pixels[i][j];
      }
    }
  }

  // Libera memória alocada.
  free(img->pixels);

  // Atribui os valores da imagem_aux para imagem passada por parâmetro.
  img->linha = imagem_aux.linha;
  img->coluna = imagem_aux.coluna;
  img->pixels = imagem_aux.pixels;
}
// Função principal, ela recebe os argumentos para manipulação.
int main(int argc, char *argv[])
{
  // Verifica se o número de argumentos é válido
  if (argc != 2)
  {
    // Se não for, imprime uma mensagem de ajuda
    print_usage(argv[0]);
  }
  else
  {
    // Se for válido, a operação é passada como argumento
    char *operation = argv[1];

    // Verifica a operação passada e executa a função correpondente
    if (strcmp(operation, "gray") == 0)
    {
      Imagem imagem;
      lerImagem(&imagem);
      imgGray(&imagem);
      saveImagem(operation, &imagem);
    }
    else if (strcmp(operation, "enlarge") == 0)
    {
      Imagem imagem;
      lerImagem(&imagem);
      imgEnlarge(&imagem);
    }
    else if (strcmp(operation, "reduce") == 0)
    {
      Imagem imagem;
      lerImagem(&imagem);
      imgReduce(&imagem);
      saveImagem(operation, &imagem);
    }
    else if (strcmp(operation, "rotate") == 0)
    {
      Imagem imagem;
      lerImagem(&imagem);
      imgRotate(&imagem);
      saveImagem(operation, &imagem);
    }
    else if (strcmp(operation, "sharp") == 0){
      Imagem imagem;
      lerImagem(&imagem);
      imgSharp(&imagem);
      saveImagem(operation, &imagem);
    }
    else if (strcmp(operation, "blur") == 0) {
      Imagem imagem;
      lerImagem(&imagem);
      imgBlur(&imagem);
      saveImagem(operation, &imagem);
    }
    else
      // Caso a operação seja inválida, retorna uma mensagem de erro.
      printf("Operação inválida!\n");
  }

  return 0;
}

/*Referências:
https://github.com/AndersonSMed/Aplicador-de-filtros-em-imagens-PPM/tree/master;
Essa referência nos ajudou a como organizar as funções e de como aplicar o filtros Sharp e Blur.
As demais já haviam sido implementadas.
*/