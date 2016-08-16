/*
    Estrutura de Candidatos
*/
typedef struct candidato{
    int prof;
    int turma;
    int dia;
    int horario;
}Candidato;

/*
    Lê o arquivo e coloca nas estruturas
*/
void DadosEntrada(int *****, int ****, int ***, int *, int *, int *, int*);

/*
    Cria estrutura Carga Horária
*/
int **criarCH(int **, int , int );

/*
    Cria estrutura Preferência
*/
int ***criarPref(int ***, int , int , int );

/*
    Cria estrutura Aula
*/
int ****criarAula(int ****, int , int , int , int );

/*
    Desaloca Estruturas
*/
void desalocar(int *****, int ****, int ***, int , int , int );

/*
    Imprime a Solução Atual
*/
void ImprimeSol(int ****, int , int , int , int );

/*
    Cria arquivo auxiliar para o Simplex
*/
void criaSimplex();
