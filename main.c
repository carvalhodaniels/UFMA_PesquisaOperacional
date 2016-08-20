#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "libs.h"

#define TRUE 1
#define FALSE 0
#define rCandidatos 0.2
#define nTestes 1000
#define horarioDiario 4
#define alpha 0.8
#define T 1.0
#define Tmin 0.000001
#define maxTurmas 3
//  Inicia o GRASP
void GRASP(int *****, int ****, int ***, int *, int *, int *, int *);

//  Acha uma Solução Gulosa Aleatória
void SolGulosaAleatoria(int *****, int ***, int **, int , int , int , int );

//  Constroi a Lista Restrita de Candidatos
void ConsLRC(int ****, int ***, int **, Candidato LRC[], int , int , int , int , int , int );

//  Verifica se o passo é viável
int eViavel(int ****, Candidato , int **, int , int , int , int );

// Imprime o valor da função objetivo
void fObjetivo(int ****, int ***, int , int , int , int );

// Realiza o Recozimento Simulado
void Simulated_Annealing(int ****, int ***, int **, int , int , int , int );

// Retorna uma Solução Vizinha Randomica Viável
int ****BuscaVizinho(int ****, int ** , int , int , int , int );

// Retorna o valor da função objetivo
int ValFunc_Obj(int ****, int ***, int , int , int , int );

// Troca Soluções
int ****TrocaSol(int ****, int ****, int , int , int , int );

void AtualizaSol(int *****, int *****, int ***, int , int , int , int );
void menu(void);

int main(){
    int ****Aula = NULL;                    //Matriz aula do professor i,para a turma j,no dia k no horário l
    int ***Pref = NULL;                     //Matriz de preferência do professor em dia e horário
    int **cargaHoraria = NULL;                                 //Matriz de Carga Horária
    int nProfs, nTurmas, nDias, nHorarios, op = 0;
    srand(time(NULL));

    do{
        menu();
        scanf("%d", &op);

        switch(op){
        case 1:
            criarInstancia();
            break;
        case 2:
            GRASP(&Aula, &Pref, &cargaHoraria, &nProfs, &nTurmas, &nDias, &nHorarios);
            system("pause");
            break;
        case 3:
            if(Aula != NULL){
                criaSimplex(Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
            }else{
                printf("Voce deve executar o GRASP antes!\n");
            }
            system("pause");
            break;
        default:
            break;
        }
    }while(op >= 1 && op <= 3);

    //fObjetivo(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);

    if(Aula != NULL)
        desalocar(&Aula, &Pref, &cargaHoraria, nProfs, nTurmas, nDias);

    return TRUE;
}

void GRASP(int *****melhorSol, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    int ****Aula = NULL;
    int i, j, k, teste = 0;

    DadosEntrada(&Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
    *melhorSol = criarAula(*melhorSol, *nProfs, *nTurmas, *nDias, *nHorarios);

    do{
        SolGulosaAleatoria(&Aula, *Pref, *cargaHoraria, *nProfs, *nTurmas, *nDias, *nHorarios);
        Simulated_Annealing(Aula, *Pref, *cargaHoraria, *nProfs, *nTurmas, *nDias, *nHorarios);
        AtualizaSol(&Aula, melhorSol, *Pref, *nProfs, *nTurmas, *nDias, *nHorarios);
        teste++;
    }while(teste < 10);

    printf("Professores: %d\n", *nProfs);
    printf("Turmas: %d\n", *nTurmas);
    printf("Dias: %d\n", *nDias);
    printf("Horarios: %d\n", *nHorarios);
    printf("\nMelhor Solucao: ");
    fObjetivo(*melhorSol, *Pref, *nProfs, *nTurmas, *nDias, *nHorarios);
    printf("\n");

    for(i = 0; i < *nProfs; i++){
        for(j = 0; j < *nTurmas; j++){
            for(k = 0; k < *nDias; k++)
                free(Aula[i][j][k]);
            free(Aula[i][j]);
        }
        free(Aula[i]);
    }
    free(Aula);

    return;
}

void SolGulosaAleatoria(int *****Aula, int ***Pref, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j = 0;
    int nCandidatos = (nDias * nHorarios) * rCandidatos;
    Candidato s, LRC[nCandidatos];
    //printf("Candidatos: %d\n", nCandidatos);
    for(i = 0; i < nCandidatos; i++){               //Zeramos a lista LRC
        LRC[i].prof = 0;
        LRC[i].turma = 0;
        LRC[i].dia = 0;
        LRC[i].horario = 0;
    }
    //LimpaSol(Aula);
    do{
        for(i = 0; i < nProfs; i++){
            //Constroi a lista restrita de candidatos para cada professor
            ConsLRC(*Aula, Pref, cargaHoraria, LRC, nCandidatos, i, nProfs, nTurmas, nDias, nHorarios);

            //Escolhe um Candidato da LRC aleatoriamente
            s = LRC[rand()%nCandidatos];

            if(eViavel(*Aula, s, cargaHoraria, nProfs, nTurmas, nDias, nHorarios))
                (*Aula)[s.prof][s.turma][s.dia][s.horario] = 1;
        }
        j++;
    }while(j < nTestes);                            //Quantas vezes quer-se verificar?
    //printf("Guloso:\n");
    //fObjetivo(*Aula, Pref, nProfs, nTurmas, nDias, nHorarios);
    return;
}

void ConsLRC(int ****Aula, int ***Pref, int **cargaHoraria, Candidato LRC[], int nCandidatos, int Prof, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, J, K, L, max = 0;
    int pref[nProfs][nDias][nHorarios];
    //Candidato C;

    //Cria uma cópia da mariz de preferência
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                pref[i][j][k] = Pref[i][j][k];
            }
        }
    }

    for(i = 0; i < nCandidatos; i++){
        LRC[i].prof = Prof;
        //C.prof = Prof;
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    //C.turma = j;
                    //C.dia = k;
                    //C.horario = l;
                    //Escolhe o melhor elemento da matriz que seja viável
                    if(pref[Prof][k][l] > max){ //&& eViavel(Aula, C, cargaHoraria, nProfs, nTurmas, nDias, nHorarios)){
                        max = pref[Prof][k][l];
                        J = j;
                        K = k;
                        L = l;
                    }
                }
            }
        };
        max = 0;
        LRC[i].turma = J;
        LRC[i].dia = K;
        LRC[i].horario = L;
        pref[Prof][K][L] = -1;                      //Retira o elemento da matriz
    }
    return;
}

int eViavel(int ****Aula, Candidato c, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, contCH = 0, contHD = 0;

    // Restrição de Turma
    for(i = 0; i < nTurmas; i++){
        if(Aula[c.prof][i][c.dia][c.horario] == 1){
            return FALSE;
        }
    }

    // Restrições de Turma e Professor
    for(i = 0; i < nProfs; i++)
        if(Aula[i][c.turma][c.dia][c.horario] == 1){
            return FALSE;
        }

    // Restrição de Carga Horária
    for(j = 0; j < nDias; j++)
        for(k = 0; k < nHorarios; k++)
            if(Aula[c.prof][c.turma][j][k] == 1)
                contCH++;
    if(contCH >= cargaHoraria[c.prof][c.turma]){
        return FALSE;
    }

    // Restrição de Horário Diário
    for(l = 0; l < nHorarios; l++)
        if(Aula[c.prof][c.turma][c.dia][l] == 1)
            contHD++;
    if(contHD >= horarioDiario){
        return FALSE;
    }

    return TRUE;
}

void fObjetivo(int ****Aula, int ***Pref, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, fObjetivo = 0;
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
                }
            }
        }
    }
    printf("%d\n", fObjetivo);
}

void Simulated_Annealing(int ****Aula, int ***Pref, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    int i, j, k, IterT;
    double Te = T;
    int delta = 0;
    float ax = alpha;
    double prob;
    int ****Aulaviz = NULL;

    // Solução Vizinha
    Aulaviz = criarAula(Aulaviz, nProfs, nTurmas, nDias, nHorarios);
    Aulaviz = TrocaSol(Aulaviz, Aula, nProfs, nTurmas, nDias, nHorarios);

    // Enquanto são chegar na temperatura mínima
    while(Te>Tmin)
    {
        IterT = 0;
        // Quantas vezes deve-se realizar nesta temperatura?
        while(IterT < nTestes)
        {
            Aulaviz = BuscaVizinho(Aulaviz, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
            IterT = IterT + 1;
            delta = ValFunc_Obj(Aulaviz, Pref, nProfs, nTurmas, nDias, nHorarios) - ValFunc_Obj(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);
            // Exponencial da contante de Euler pelo delta/temperatura
            // Quanto mais alta a temperatura, mais soluções piores são aceitas
            prob = pow(2.71828,(delta/Te));
            // Se a função objetivo for melhor ou se a probabilidade for boa
            if((delta > 0) || ((delta < 0) && (prob >= (rand()%101)/100.0))){
                Aula = TrocaSol(Aula, Aulaviz, nProfs, nTurmas, nDias, nHorarios);
            }
        }
        Te = Te * ax;
        break;
    }
    //printf("Simulated Anneling:\n");
    //fObjetivo(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);

    // Sempre desalocar alocações dinâmicas
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++)
                free(Aulaviz[i][j][k]);
            free(Aulaviz[i][j]);
        }
        free(Aulaviz[i]);
    }
    free(Aulaviz);

    return;
}

int ****BuscaVizinho(int ****Aula, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    int i = 0;
    Candidato C;

    // "Um vizinho s' de uma solução s é uma solução na qual foi aplicado um
    // movimento (definido a priori) modificando a solução corrente."
    // No caso, selecionamos um elemento aleatório e trocamos seu bit
    C.prof = rand()%nProfs;
    C.turma = rand()%nTurmas;
    C.dia = rand()%nDias;
    C.horario = rand()%nHorarios;
    // Se formos retirar, não precisamos testar
    if(Aula[C.prof][C.turma][C.dia][C.horario] == 1){
        Aula[C.prof][C.turma][C.dia][C.horario] = 0;
    }else
    // Checa a viabilidade desse Candidato
    do{
        C.prof = rand()%nProfs;
        C.turma = rand()%nTurmas;
        C.dia = rand()%nDias;
        C.horario = rand()%nHorarios;
        if(eViavel(Aula, C, cargaHoraria, nProfs, nTurmas, nDias, nHorarios)){
            Aula[C.prof][C.turma][C.dia][C.horario] = 1;
            break;
        }
        i++;
    }while(i < nTestes);

    return Aula;
}

int ValFunc_Obj(int ****Aula, int ***Pref, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    int i, j, k, l, fObjetivo = 0;
    if(Aula != NULL){
        for(i = 0; i < nProfs; i++){
            for(j = 0; j < nTurmas; j++){
                for(k = 0; k < nDias; k++){
                    for(l = 0; l < nHorarios; l++){
                        if(Aula[i][j][k][l] == 1){
                            fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
                        }
                    }
                }
            }
        }
    }
    return fObjetivo;
}

int ****TrocaSol(int ****A1, int ****A2, int nProfs, int nTurmas, int  nDias, int nHorarios){
    int i, j, k, l;
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    A1[i][j][k][l] = A2[i][j][k][l];
    return A1;
}

void AtualizaSol(int *****Sol1, int *****Sol2, int ***Pref, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l;
    if(ValFunc_Obj(*Sol1, Pref, nProfs, nTurmas, nDias, nHorarios) > ValFunc_Obj(*Sol2, Pref, nProfs, nTurmas, nDias, nHorarios)){
        for(i = 0; i < nProfs; i++){
            for(j = 0; j < nTurmas; j++){
                for(k = 0; k < nDias; k++){
                    for(l = 0; l < nHorarios; l++){
                        (*Sol2)[i][j][k][l] = (*Sol1)[i][j][k][l];
                    }
                }
            }
        }
    }


    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    (*Sol1)[i][j][k][l] = 0;
                }
            }
        }
    }

    return;
}

void menu(void){
    system("cls");
    printf("------------------------------------------------\n");
    printf("-------------- Bem vindo ao GRASP --------------\n");
    printf("-------------------- Autores: ------------------\n");
    printf("---------- Daniel Carvalho e Laudelino ---------\n");
    printf("------------------------------------------------\n\n");
    printf("Escolha uma opcao pelo seu valor:\n");
    printf("1. Criar uma nova instancia de entrada\n");
    printf("2. Resolver a instancia pelo GRASP\n");
    printf("3. Resolver a instancia pelo Simplex-GLPK\n");
    printf("0. Sair\n");

    return;
}
