#include <stdio.h>
#include <pthread.h>

/*
* Aluno: Thiago Augusto da Silva Cortez
* Matricula: 2017005276
* Curso: Sistemas de Informação
*/

int matriz[9][9];
int linha = 0;
int coluna = 0;
int quadradosMenores = 0;

void preencherSudoku(){
    matriz[0][0] = 6;
    matriz[0][1] = 2;
    matriz[0][2] = 4;
    matriz[0][3] = 5;
    matriz[0][4] = 3;
    matriz[0][5] = 9;
    matriz[0][6] = 1;
    matriz[0][7] = 8;
    matriz[0][8] = 7;

    matriz[1][0] = 5;
    matriz[1][1] = 1;
    matriz[1][2] = 9;
    matriz[1][3] = 7;
    matriz[1][4] = 2;
    matriz[1][5] = 8;
    matriz[1][6] = 6;
    matriz[1][7] = 3;
    matriz[1][8] = 4;

    matriz[2][0] = 8;
    matriz[2][1] = 3;
    matriz[2][2] = 7;
    matriz[2][3] = 6;
    matriz[2][4] = 1;
    matriz[2][5] = 4;
    matriz[2][6] = 2;
    matriz[2][7] = 9;
    matriz[2][8] = 5;

    matriz[3][0] = 1;
    matriz[3][1] = 4;
    matriz[3][2] = 3;
    matriz[3][3] = 8;
    matriz[3][4] = 6;
    matriz[3][5] = 5;
    matriz[3][6] = 7;
    matriz[3][7] = 2;
    matriz[3][8] = 9;

    matriz[4][0] = 9;
    matriz[4][1] = 5;
    matriz[4][2] = 8;
    matriz[4][3] = 2;
    matriz[4][4] = 4;
    matriz[4][5] = 7;
    matriz[4][6] = 3;
    matriz[4][7] = 6;
    matriz[4][8] = 1;

    matriz[5][0] = 7;
    matriz[5][1] = 6;
    matriz[5][2] = 2;
    matriz[5][3] = 3;
    matriz[5][4] = 9;
    matriz[5][5] = 1;
    matriz[5][6] = 4;
    matriz[5][7] = 5;
    matriz[5][8] = 8;

    matriz[6][0] = 3;
    matriz[6][1] = 7;
    matriz[6][2] = 1;
    matriz[6][3] = 9;
    matriz[6][4] = 5;
    matriz[6][5] = 6;
    matriz[6][6] = 8;
    matriz[6][7] = 4;
    matriz[6][8] = 2;

    matriz[7][0] = 4;
    matriz[7][1] = 9;
    matriz[7][2] = 6;
    matriz[7][3] = 1;
    matriz[7][4] = 8;
    matriz[7][5] = 2;
    matriz[7][6] = 5;
    matriz[8][7] = 7;
    matriz[7][8] = 3;

    matriz[8][0] = 2;
    matriz[8][1] = 8;
    matriz[8][2] = 5;
    matriz[8][3] = 4;
    matriz[8][4] = 7;
    matriz[8][5] = 3;
    matriz[8][6] = 9;
    matriz[8][7] = 1;
    matriz[8][8] = 6;
}

void * checarLinhas(){
    int i, j, k, flag = 1;

    /*
    * O primeiro loop serve para percorrer as linhas da matriz, enquanto que
    * o segundo e o terceiro loop servem para comparar valores que ocupam a
    * mesma linha
    */
    for(i = 0; i < 9 && flag; i++)
        for(j = 0; j < 9 && flag; j++)
            for(k = 0; k < 9 && flag; k++)
                if((j != k) && (matriz[i][j] == matriz[i][k]))
                    flag = 0;

    linha = flag;
    pthread_exit(NULL); 
}

void * checarColunas(){

    /*
    * Essa função tem o funcionamento exatamente igual ao da função checarLinhas,
    * com a diferença que procura valores iguais na mesma coluna ao invés de na
    * mesma linha.
    */
    int i, j, k, flag = 1;

    for(j = 0; j < 9 && flag; j++)
        for(i = 0; i < 9 && flag; i++)
            for(k = 0; k < 9 && flag; k++)
                if((i != k) && (matriz[i][j] == matriz[k][j]))
                    flag = 0;

    coluna = flag;
    pthread_exit(NULL); 
}



/*
* Esta função serve para checar os quadrados menores do jogo de Sudoku, para
* fazer isso, recebe uma matriz 3x3, que tem o tamanho de um setor do jogo
* de Sudoku e verifica se esse mesmo é válido. Ela é alimentada pela função
* checarQuadrados()
*/
int checarMenor(int mini[3][3]){
    int i, j, num, existe = 0;

    /*
    * Os loops passam por todos os valores da matriz e sempre que encontram o
    * número "num" em um dos espaços incrementam a variável "existe" em mais 1.
    * Se "existe" for maior que 1, isto é, se ouver mais de um número "num" no mesmo
    * setor, o setor é automaticamente inválido e retorna falso.
    */
    for(num = 1; num <= 9; num++){
        for(i = 0; i < 3; i++)
            for(j = 0; j < 3; j++)
                if(mini[i][j] == num) existe++;
        if(existe > 1) return 0;
        existe = 0;
    }

    return 1;
}

/*
* Esta função serve para separar a matriz completa 9x9 em matrizes
* pequenas 3x3 e enviá-las para a função checarMenor()
*/
void * checarQuadrados(){
    int i, j, k, l, mini[3][3];

    /*
    * O for a seguir serve para separar os quadrados menores (3x3)
    * do geral (9x9). O primeiro delimita a linha, o segundo a Coluna
    * o terceiro itera sobre as linhas separadas pelo primeiro for e 
    * o quarto for itera sobre as colunas separadas pelo segundo for 
    */

    for(i = 0; i < 9; i += 3)
        for(j = 0; j < 9; j += 3){
            for(k = 0; k < 3; k++)
                for(l = 0; l < 3; l++)
                    mini[k][l] = matriz[k+i][l+j];
            if(!checarMenor(mini)){
                quadradosMenores = 0;
                pthread_exit(NULL);
                return ;
            }
        }

    quadradosMenores = 1;
    pthread_exit(NULL); 
}

int main(){
    //Inserindo os dados do sudoku
    preencherSudoku();

    //Cria-se as variáveis que auxiliarão no manutenção dos threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;


    //Crio cada thread e uso join para "unir" todos no principal, posteriormente
    pthread_create(&thread1, NULL, checarLinhas, NULL);
    pthread_join(thread1,NULL);

    pthread_create(&thread2, NULL, checarColunas, NULL);
    pthread_join(thread2,NULL);
    
    pthread_create(&thread3, NULL, checarQuadrados, NULL);
    pthread_join(thread3,NULL);
    
    /*Se as flags linha, coluna e quadradosMenores forem todos iguais à um
    * o Sudoku está escrito corretamente, mas se algum estiver com o valor 0,
    * quer dizer que em alguma parte do sudoku há um erro.
    */
    if (linha && coluna && quadradosMenores)
        printf("O Sudoku esta escrito corretamente.\n");
    else    
        printf("Ops, tem um erro no sudoku! Verifique novamente.\n");

    /*
    * Fim do Programa.
    */
    return 0;
}