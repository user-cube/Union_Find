#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/**
 * Comprimento da tabela.
 */
#define W 50

/**
 * Altura da tabela. 
 */
#define H 35

/**
 * Nós de execução.
 */
#define EXEC 1e6

/**
 * Array com pixeis da imagem.
 * 0 - Pixel preto
 * 1 - Pixel branco
 *
 * A inicialização é feita (por defeito) a 0
 */
int image[H][W];

/**
 * Array unidimensional de representantesde cada pixel.
 */
int array[W*H];

/**
 * Tamanho do conjunto com essa raíz.
 * Usado para manter os unions equilibrados.
 */
int size[W*H]; 

/**
 * transforma coordenadas 2D num indíce de um array unidimensional.
 */int getArray( int x, int y ) {
	return array[x*W+y];
}

/**
 * Imprime o array de representantes na consola.
 */
void show(int arr[])
{
	printf("[");
	for(int i = 0; i < H*W; i++)
		printf("%d ", arr[i]);
	printf("]\n");
}


/**
 * Gera a estrutura dos arrays para matlab.
 * Estes arrays são guardados em dois ficheiros:
 * 
 * matrixA.txt -
 * matrizB.txt -
 *
 *Após tudo gerado será tudo processado em Matlab / Octave.
 */
void showImage(int arr[H][W], int a)
{
	if (a == 1){
		FILE *fp = fopen("matrixA.txt","w");
		if (fp == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		for(int i = 0; i < H; i++) {
			for(int j = 0; j < W; j++)
				fprintf(fp, "%d ", arr[i][j]);

			if(i==H-1)
				fprintf(fp, "\n");
			else
				fprintf(fp,"\n");
		}
	}
	if (a == 2){
		FILE *fb = fopen("matrixB.txt","w");
		if (fb == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		for(int i = 0; i < H; i++) {
			for(int j = 0; j < W; j++)
				fprintf(fb, "%d ", arr[i][j]);

			if(i==H-1)
				fprintf(fb, "\n");
			else
				fprintf(fb,"\n");
		}
	}	
}


/**
 *                                  CONECTIVITY USING UNION-FINDING
 */

/**
 * Inicializa os representantes de todos os píxeis 
 * como eles prórpios e a imagem a zeros.
 */
void init()
{
	for(int i = 0;i < H; i++) {
		for(int j = 0; j < W; j++) {
			image[i][j] = 0;
			array[i*W+j] = i*W+j;
			size[i*W+j] = 1;
		}
	}

}

/**
 * Função que encontra a raíz de um píxel e comprime 
 * o caminho definindo a raíz do conjunto como um 
 * novo representante.
 */
int find(int i)
{
	if(array[i] != i)
		array[i] = find(array[i]);

	return array[i];
}

/**
 * Procura a raíz de 2 pixeis e une os conjuntos tendo 
 * em conta o peso de cada um, unindo sempre o maior 
 * ao menor para obter finds mais rápidos.
 */

void uni(int x, int y)
{
	int raiz_x = find(x);
	int raiz_y = find(y);
	if(size[raiz_x] > size[raiz_y]) {
		array[raiz_y] = raiz_x;
		size[raiz_x] += size[raiz_y];
	}

	else{
		array[raiz_x] = raiz_y;
		size[raiz_y] += size[raiz_x];
	}
}

/**
 * Função que verifica a vizinhança de um píxel 
 * tendo sempre em conta os limites da imagem.
 */
void adjacent(int x, int y)
{
	/**
     * Verificar de baixo.
     */
    if(x > 0 && image[x-1][y] != 0) 
		uni(getArray(x-1, y), getArray(x,y));
    /**
     * Verificar pixel de cima.
     */
	if(x < H-1 && image[x+1][y] != 0) 
		uni(getArray(x+1, y), getArray(x,y));
    /**
     * Verificar pixel à esquerda.
     */
	if(y > 0 && image[x][y-1] != 0) 
		uni(getArray(x, y-1), getArray(x,y));
    /**
     * Verificar pixel à direita.
     */
	if(y < W-1 && image[x][y+1] != 0)
		uni(getArray(x, y+1), getArray(x,y));
}

/**
 * Verifica se existe algum conjunto que esteja 
 * ao mesmo tempo na 1ª e na última linha, 
 * caso exista um conjunto nessa situação quer
 * dizer que há pelo menos um caminho entre o topo 
 * e o fundo.
 */
int check(void)
{
	for(int i = 0; i < W; i++) {
		for(int j = (H-1)*W; j < H*W; j++) {
			if (find(i) == find(j) ) {
				return find(i);
			}
		}
	}
	return 0;
}

/**
 * Desenha o caminho na imagem.
 * Este desenho será processado em matlab com o
 * ficheiro resuults.m.
 */
void draw(int h) {
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++)
			if (find(getArray(i,j)) == h)
			image[i][j] = 2;
	}
}

/**
 * Main function
 */
int main(int argc,char **argv)
{
    /**
     * Seed para obter sempre o mesmo resultado.
     */
	srand(80013);
    /**
     * Declarações.
     */
	int x, 
        y,
        a,
        cont, 
        unicos,
        total = 0,
        resolvido, 
        total_gerados = 0;

	/**
     * Ciclo principal do programa.
     */
    for(int i = 0; i < EXEC; i++) {
		// inicializar os arrays.
        init();
    	unicos = 0;
		resolvido = 0;
		cont = 0;
		/**
         * Enquanto que não houver um caminho entre o topo e o fundo.
         */
        while(resolvido == 0) { 
			cont++;
            /**
             * Gerar novas coordenadas.
             */
			x = rand() % H;
			y = rand() % W;
            /* Se esse ponto ainda não tinha sido gerado */
			if (image[x][y] == 0) { 
				image[x][y] = 1;
				adjacent(x, y);
				unicos++;
				resolvido = check();
			}
		}
		total += unicos;
		total_gerados += cont;
	}
	double media_gerados = total_gerados/EXEC;
	double media = total/EXEC;
	double percentagem = media/(W*H)*100;
	
	a = 1;
	show(array);
	showImage(image, a);
	draw(resolvido);
	a = 2;
	showImage(image, a);
	printf("Média de pontos gerados por imagem:%lf\nMédia pontos preenchidos por imagem:%lf\nPercentagem de pontos a branco:%lf\n",media_gerados, media, percentagem);
}

