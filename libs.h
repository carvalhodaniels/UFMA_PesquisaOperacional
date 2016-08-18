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
    L� o arquivo e coloca nas estruturas
*/
void DadosEntrada(int *****, int ****, int ***, int *, int *, int *, int*);

/*
    Cria estrutura Carga Hor�ria
*/
int **criarCH(int **, int , int );

/*
    Cria estrutura Prefer�ncia
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
    Imprime a Solu��o Atual
*/
void ImprimeSol(int ****, int , int , int , int );

/*
    Cria o arquivo simplex.pl a ser lido e resolvido pelo Simplex
    A sa�da fica no arquivo sol.txt
*/
void criaSimplex(int ****, int ***, int **, int , int , int , int );

/*
    Cria uma nova inst�ncia
*/
void criarInstancia(void);
