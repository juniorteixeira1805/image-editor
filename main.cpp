#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

struct pixel{
	 int r;
	 int g;
	 int b;
};

struct Img{
	char tipo[3];
	 int valorMax;
	 int nLinhas;
	 int nColunas;
	pixel conteudo[1024][1024];
};

void menuPrincipal(int &opt);
void solariza(Img& img_in, Img& img_out, long long int limiar);
void separa_canais(Img& img_in, Img& img_r, Img& img_g, Img& img_b);
void compt_neg(Img& img_in, Img& img_out);
void aumenta_tamanho(Img img_in, Img& img_out);
void equaliza_histograma(Img img_in, Img& img_out);
int abre_img(char nome[], Img& img);
void salva_img(char nome[], Img& img);
void proc_img(char salvar, char nome[], int limiar, int &opt, Img& img_in, Img& img_out, Img& img_r, Img& img_g, Img& img_b, Img masc);
void roll_img();

void gerar_masc(Img& img_in, Img img_out, Img& masc);

int main(){
  int opt = 0, limiar = 0;
  char nomeE[100];
  char nomeS[100];
  char continua = 's', salvar = 's';
  Img entrada, saida, canalR, canalG, canalB, masc;

  while(continua == 's'){
    menuPrincipal(opt);

    if(opt == 0){
        break;
    }

    system("cls");

    roll_img();

    cout << "Informe o nome da imagem: ";
    cin >> nomeE;

    system("cls");

    if(abre_img(nomeE, entrada) != 2){
        break;
    }

    proc_img(salvar, nomeS, limiar, opt, entrada, saida, canalR, canalB, canalG, masc);

    system("cls");

    cout << "Deseja continuar? (s para sim e n�o para n) ";
    cin >> continua;
    while(continua != 's' && continua != 'n'){
      system("cls");
      cout << "opcao invalida! (s para sim e n�o para n) ";
      cin >> continua;
    }
    system("cls");
  }

	return 0;
}

int abre_img(char nome[], Img& img){
  if ( (img.tipo[0] == 'P' && img.tipo[1] == '3') ){
    cout << "imagem nao e do tipo 'P3' " << endl;
    return 0;
  }
  ifstream arquivo;
  arquivo.open(nome);
  if(arquivo.is_open()){
    arquivo >> img.tipo[0] >> img.tipo[1];
    img.tipo[2] = '\0';
    arquivo >> img.nColunas;
    arquivo >> img.nLinhas;
    arquivo >> img.valorMax;
    for( int i = 0; i < img.nLinhas; i++){
      for( int j = 0; j < img.nColunas; j++){
        arquivo >> img.conteudo[i][j].r;
        arquivo >> img.conteudo[i][j].g;
        arquivo >> img.conteudo[i][j].b;
      }
    }
    arquivo.close();
    return 2;
  } else {
    cout << "Erro ao abrir o arquivo!!!" << endl;
    return 1;
  }
}

void salva_img(char nome[], Img& img){
  ofstream arquivo;
  arquivo.open(nome);
  if(arquivo.is_open()){
    arquivo << img.tipo[0] << img.tipo[1] << endl;
    arquivo << img.nColunas << " ";
    arquivo << img.nLinhas << endl;
    arquivo << img.valorMax << endl;
    for( int i = 0; i < img.nLinhas; i++){
      for( int j = 0; j < img.nColunas; j++){
        arquivo << img.conteudo[i][j].r << " ";
        arquivo << img.conteudo[i][j].g << " ";
        arquivo << img.conteudo[i][j].b << " ";
      }
      cout << endl;
    }
    arquivo.close();
  } else {
    cout << "Erro criar o arquivo!!!" << endl;
    return;
  }
}


void menuPrincipal(int &opt){
	cout << "MENU" << endl << endl << endl;
	cout << "1 - Solarizar" << endl << endl;
	cout << "2 - Separar Canais" << endl << endl;
	cout << "3 - Aumentar Tamanho" << endl << endl;
	cout << "4 - Equalizar Histograma" << endl << endl;
  cout << "5 - computar negativo" << endl << endl;
	cout << "0 - Sair" << endl;
	cout << "Digite um numero referente a uma opcao: ";
	cin >> opt;
	while (opt < 0 || opt > 5){
            system("cls");
    cout << "opcao invalida! Digite um numero referente a uma opcao: " << endl << endl;
    	cout << "MENU" << endl << endl << endl;
	cout << "1 - Solarizar" << endl << endl;
	cout << "2 - Separar Canais" << endl << endl;
	cout << "3 - Aumentar Tamanho" << endl << endl;
	cout << "4 - Equalizar Histograma" << endl << endl;
  cout << "5 - computar negativo" << endl << endl;
	cout << "0 - Sair" << endl;
	cin >> opt;
	}
}

void proc_img(char salvar, char nome[], int limiar, int &opt, Img& img_in, Img& img_out, Img& img_r, Img& img_g, Img& img_b, Img masc){
  	switch(opt){
		case 1:

      solariza(img_in, img_out, limiar);
      cout << "imagem processada. " << endl;

      cout << "voce deseja salvar: (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
        cout << "opaos invalida! (s para sim e n para nao)";
        cin >> salvar;
      }

      if(salvar == 's'){
        cout <<"Informe nome da nova imagem: ";
        cin >> nome;
        salva_img(nome, img_out);
      }

      system("cls");
      cout << "Imagem salva." << endl;

      cout << "gerar mascara?  (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
        cout << "opaos invalida! (s para sim e n para nao)";
        cin >> salvar;
      }
      if(salvar == 's'){
        cout <<"Informe o nome da mascara: ";
        cin >> nome;
        gerar_masc(img_in, img_out, masc);
        salva_img(nome, masc);
      }
      system("cls");
      cout << "mascara processada. " << endl;
		break;

		case 2:
      separa_canais(img_in, img_r, img_g, img_b);
      cout << "imagem processada. " << endl;

      cout << "voce deseja salvar: (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
          cout << "opaos invalida! (s para sim e n para nao)";
          cin >> salvar;
      }
      if(salvar == 's'){
        cout <<"Informe o nome do canal r: ";
        cin >> nome;
        salva_img(nome, img_r);
        system("cls");

        cout <<"Informe o nome do canal g: ";
        cin >> nome;
        salva_img(nome, img_g);
        system("cls");

        cout <<"Informe o nome do canal b: ";
        cin >> nome;
        salva_img(nome, img_b);
        system("cls");
        cout <<"Canais separados " << endl;
      }
		break;

		case 3:
      aumenta_tamanho(img_in, img_out);
      cout << "imagem processada. " << endl;

      cout << "voce deseja salvar: (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
        cout << "opaos invalida! (s para sim e n para nao)";
        cin >> salvar;
      }
      if(salvar == 's'){
        cout <<"Informe o nome da nova imagem: ";
        cin >> nome;
        salva_img(nome, img_out);
      }

		break;

		case 4:
      equaliza_histograma(img_in, img_out);
      cout << "imagem processada. " << endl;

      cout << "voce deseja salvar: (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
        cout << "opaos invalida! (s para sim e n para nao)";
        cin >> salvar;
      }
      if(salvar == 's'){
        cout <<"Informe nome da nova imagem: ";
        cin >> nome;
        salva_img(nome, img_out);
      }
		break;

    case 5:
      compt_neg(img_in, img_out);
      cout << "voce deseja salvar? (s para sim e n para nao)";
      cin >> salvar;
      while(salvar != 's' && salvar != 'n'){
        cout << "opaos invalida! (s para sim e n para nao)";
        cin >> salvar;
      }
      if(salvar == 's'){
        cout <<"Informe o nome da nova imagem: ";
        cin >> nome;
        salva_img(nome, img_out);
      }
      break;

		case 0:
			return;
		default:
			cout << "Opcao invalida!" << endl;
			menuPrincipal(opt);
	}
}

void solariza(Img& img_in, Img& img_out, long long int limiar){

  img_out = img_in;

  for(int i = 0; i < img_out.nLinhas; i++){
    for(int j = 0; j < img_out.nColunas; j++){
      limiar += img_in.conteudo[i][j].r;
    }
  }

  limiar = limiar / (img_out.nLinhas*img_out.nColunas);

  for( int i = 0; i < img_out.nLinhas; i++){
    for( int j = 0; j < img_out.nColunas; j++){
      if(img_out.conteudo[i][j].r < limiar){
        img_out.conteudo[i][j].r = 255 - img_in.conteudo[i][j].r;
      }
      else{
        img_out.conteudo[i][j].r =  img_in.conteudo[i][j].r;
      }
    }
  }

  limiar = 0;

  for(int i = 0; i < img_out.nLinhas; i++){
    for(int j = 0; j < img_out.nColunas; j++){
      limiar += img_in.conteudo[i][j].g;
    }
  }

  limiar = limiar / (img_out.nLinhas*img_out.nColunas);

  for( int i = 0; i < img_out.nLinhas; i++){
    for( int j = 0; j < img_out.nColunas; j++){
      if(img_out.conteudo[i][j].g < limiar){
        img_out.conteudo[i][j].g = 255 - img_in.conteudo[i][j].g;
      }
      else{
        img_out.conteudo[i][j].g =  img_in.conteudo[i][j].g;
      }
    }
  }

  limiar = 0;

  for(int i = 0; i < img_out.nLinhas; i++){
    for(int j = 0; j < img_out.nColunas; j++){
      limiar += img_in.conteudo[i][j].b;
    }
  }

  limiar = limiar / (img_out.nLinhas*img_out.nColunas);

  for( int i = 0; i < img_out.nLinhas; i++){
    for( int j = 0; j < img_out.nColunas; j++){
      if(img_out.conteudo[i][j].b < limiar){
        img_out.conteudo[i][j].b = 255 - img_in.conteudo[i][j].b;
      }
      else{
        img_out.conteudo[i][j].b =  img_in.conteudo[i][j].b;
      }
    }
  }
}

void compt_neg(Img& img_in, Img& img_out){
  img_out.tipo[0] = img_in.tipo[0];
  img_out.tipo[1] = img_in.tipo[1];
  img_out.tipo[2] = img_in.tipo[2];
  img_out.nColunas = img_in.nColunas;
  img_out.nLinhas = img_in.nLinhas;
  img_out.valorMax = img_in.valorMax;
  for(int i = 0; i < img_in.nLinhas; i++){
    for(int j = 0; j < img_in.nColunas; j++){
      img_out.conteudo[i][j].r = 255 - img_in.conteudo[i][j].r;
      img_out.conteudo[i][j].g = 255 - img_in.conteudo[i][j].g;
      img_out.conteudo[i][j].b = 255 - img_in.conteudo[i][j].b;
    }
  }
}

void separa_canais(Img& img_in, Img& img_r, Img& img_g, Img& img_b){
  img_r = img_in; img_g = img_in; img_b = img_in;

  for(int i = 0; i < img_in.nLinhas; i++){
      for(int j = 0; j < img_in.nColunas; j++){
        img_r.conteudo[i][j].g = 0;
        img_r.conteudo[i][j].b = 0;
    }
  }
  for(int i = 0; i < img_in.nLinhas; i++){
    for(int j = 0; j < img_in.nColunas; j++){
      img_g.conteudo[i][j].r = 0;
      img_g.conteudo[i][j].b = 0;
    }
  }
  for(int i = 0; i < img_in.nLinhas; i++){
    for(int j = 0; j < img_in.nColunas; j++){
      img_b.conteudo[i][j].r = 0;
      img_b.conteudo[i][j].g = 0;
    }
  }
}

void roll_img(){
  cout << "lista de imagens: " << endl << endl << endl;
    system("dir *.ppm");
}
void equaliza_histograma(Img img_in, Img& img_out){
  int contr[257], contg[257], contb[257], aux = 0;
  unsigned long long int somatorio = 0;
  float aux2 = ((float)255/((float)img_in.nLinhas*(float)img_in.nColunas));
  img_out = img_in;

  for(int i = 0; i < 257; i++){
    contr[i] = 0;
    contg[i] = 0;
    contb[i] = 0;
  }

  for(int i = 0; i < img_in.nLinhas; i++){
    for(int j = 0; j < img_in.nColunas; j++){
      aux = img_in.conteudo[i][j].r;
      contr[aux] += 1;
      aux = img_in.conteudo[i][j].g;
      contg[aux] += 1;
      aux = img_in.conteudo[i][j].b;
      contb[aux] += 1;
    }
  }

  for(int i = 0; i < img_in.nLinhas; i++){
    for(int j = 0; j < img_in.nColunas; j++){
        aux = img_in.conteudo[i][j].r;
        for(int k = 0; k < aux; k++){
          somatorio += contr[k];
        }
        img_out.conteudo[i][j].r = somatorio*aux2;
        somatorio = 0;

        aux = img_in.conteudo[i][j].g;
        for(int k = 0; k < aux; k++){
          somatorio += contg[k];
        }
        img_out.conteudo[i][j].g = somatorio*aux2;
        somatorio = 0;

        aux = img_in.conteudo[i][j].b;
        for(int k = 0; k < aux; k++){
          somatorio += contb[k];
        }
        img_out.conteudo[i][j].b = somatorio*aux2;
        somatorio = 0;
    }
  }
}

void aumenta_tamanho(Img img_in, Img& img_out){
  img_out.tipo[0] = img_in.tipo[0];
  img_out.tipo[1] = img_in.tipo[1];
  img_out.tipo[2] = img_in.tipo[2];
  img_out.nColunas = (img_in.nColunas *2) - 1;
  img_out.nLinhas = (img_in.nLinhas *2) - 1;
  img_out.valorMax = img_in.valorMax;

  for(int i = 0; i < img_out.nColunas; i++){
    for(int j = 0; j < img_out.nLinhas; j++){
      if(i % 2 == 0 || j % 2 == 0){
        img_out.conteudo[i][j].r = img_in.conteudo[i][j].r; //Esta despejando todos os pixels da matrix anterios
        img_out.conteudo[i][j].g = img_in.conteudo[i][j].g;// na matriz aumentada. ok!
        img_out.conteudo[i][j].b = img_in.conteudo[i][j].b;
      }
    }
  }

  for(int i = 0; i < img_out.nColunas; i++){
    for(int j = 0; j < img_out.nLinhas; j++){
      if( (i == 0 && j % 2 == 1) || (i == (img_out.nLinhas - 1) && j % 2 == 1) ){
        img_out.conteudo[i][j].r = (img_out.conteudo[i][j-1].r + img_out.conteudo[i][j+1].r)/2;
        img_out.conteudo[i][j].g = (img_out.conteudo[i][j-1].g + img_out.conteudo[i][j+1].g)/2;   // aqui est� preenchendo as bordas superior e inferior
        img_out.conteudo[i][j].b = (img_out.conteudo[i][j-1].b + img_out.conteudo[i][j+1].b)/2;
      }
    }
  }

  for(int i = 0; i < img_out.nColunas; i++){
    for(int j = 0; j < img_out.nLinhas; j++){
      if( (j == 0 && i % 2 == 1) || (j == img_out.nColunas - 1 && i % 2 == 1) ){
        img_out.conteudo[i][j].r = (img_out.conteudo[i+1][j].r + img_out.conteudo[i-1][j].r)/2;
        img_out.conteudo[i][j].g = (img_out.conteudo[i+1][j].g + img_out.conteudo[i-1][j].g)/2;   // aqui est� preenchendo as bordas laterais
        img_out.conteudo[i][j].b = (img_out.conteudo[i+1][j].b + img_out.conteudo[i-1][j].b)/2;   // a ordem da matriz sempre vai ser impar.
      }
    }
  }

  for(int i = 0; i < img_out.nColunas; i++){
    for(int j = 0; j < img_out.nLinhas; j++){
      if( i % 2 == 1 && j % 2 == 1){
              // essa parate vai preencher todas indices "i" impar e "j" impar (nao preciso delimitar at� a penultima linha e coluna)
              // pois por a ordem ser impar, as bordas sempre serao par.
        img_out.conteudo[i][j].r = (img_out.conteudo[i+1][j+1].r + img_out.conteudo[i-1][j-1].r + img_out.conteudo[i+1][j-1].r + img_out.conteudo[i-1][j+1].r)/4;
        img_out.conteudo[i][j].g = (img_out.conteudo[i+1][j+1].g + img_out.conteudo[i-1][j-1].g + img_out.conteudo[i+1][j-1].g + img_out.conteudo[i-1][j+1].g)/4;
        img_out.conteudo[i][j].b = (img_out.conteudo[i+1][j+1].b + img_out.conteudo[i-1][j-1].b + img_out.conteudo[i+1][j-1].b + img_out.conteudo[i-1][j+1].b)/4;
      }
    }
  }

  for(int i = 1; i < img_out.nColunas; i++){
    for(int j = 1; j < img_out.nLinhas; j++){
        if(i % 2 == 1 && j % 2 == 0 && i != 0 && i != img_out.nLinhas-1 && j != 0 && j != img_out.nColunas-1){
        img_out.conteudo[i][j].r = (img_out.conteudo[i+1][j].r + img_out.conteudo[i-1][j].r + img_out.conteudo[i][j-1].r + img_out.conteudo[i][j+1].r)/4;
        img_out.conteudo[i][j].g = (img_out.conteudo[i+1][j].g + img_out.conteudo[i-1][j].g + img_out.conteudo[i][j-1].g + img_out.conteudo[i][j+1].g)/4;
        img_out.conteudo[i][j].b = (img_out.conteudo[i+1][j].b + img_out.conteudo[i-1][j].b + img_out.conteudo[i][j-1].b + img_out.conteudo[i][j+1].b)/4;
        }
        else if(i % 2 == 0 && j % 2 == 1 && i != 0 && i != img_out.nLinhas-1 && j != 0 && j != img_out.nColunas-1){
        img_out.conteudo[i][j].r = (img_out.conteudo[i+1][j].r + img_out.conteudo[i-1][j].r + img_out.conteudo[i][j-1].r + img_out.conteudo[i][j+1].r)/4;
        img_out.conteudo[i][j].g = (img_out.conteudo[i+1][j].g + img_out.conteudo[i-1][j].g + img_out.conteudo[i][j-1].g + img_out.conteudo[i][j+1].g)/4;
        img_out.conteudo[i][j].b = (img_out.conteudo[i+1][j].b + img_out.conteudo[i-1][j].b + img_out.conteudo[i][j-1].b + img_out.conteudo[i][j+1].b)/4;
        }
    }
  }
}

void gerar_masc(Img& img_in, Img img_out, Img& masc){
  masc = img_in;

  for (int i = 0; i < img_out.nLinhas; i++){
    for (int j = 0; j < img_out.nColunas; j++){
      if(j < (img_out.nColunas-1)/2){
        masc.conteudo[i][j].r = 0;
        masc.conteudo[i][j].g = 0;
        masc.conteudo[i][j].b = 0;     //MASCARA CRIADA
      }
      else{
        masc.conteudo[i][j].r = 255;
        masc.conteudo[i][j].g = 255;
        masc.conteudo[i][j].b = 255;
      }
    }
  }

  for (int i = 0; i < img_out.nLinhas; i++){
    for (int j = 0; j < img_out.nColunas; j++){
      if(masc.conteudo[i][j].r == 0 || masc.conteudo[i][j].g == 0 || masc.conteudo[i][j].b == 0){
        masc.conteudo[i][j].r = img_in.conteudo[i][j].r;
        masc.conteudo[i][j].g = img_in.conteudo[i][j].g;
        masc.conteudo[i][j].b = img_in.conteudo[i][j].b;
      }
      else if(masc.conteudo[i][j].r == 255 || masc.conteudo[i][j].g == 255 || masc.conteudo[i][j].b == 255){
        masc.conteudo[i][j].r = img_out.conteudo[i][j].r;
        masc.conteudo[i][j].g = img_out.conteudo[i][j].g;
        masc.conteudo[i][j].b = img_out.conteudo[i][j].b;
      }
    }
  }
}