#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "libs.h"

#define TRUE 1
#define FALSE 0
#define rCandidatos 0.2
#define nTestes 100
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
void ConsLRC(int ****, int ***, Candidato LRC[], int , int , int , int , int );

//  Verifica se o passo é viável
int eViavel(int ****, Candidato , int **, int , int , int , int );

// Imprime o valor da função objetivo
void fObjetivo(int ****, int ***, int , int , int , int );

// Realiza o Recozimento Simulado
void Simulated_Annealing(int ****, int ***, int **, int , int , int , int );

// Busca uma solução vizinha
int ****BuscaVizinho(int ****, int ** , int , int , int , int );

// Retorna o valor da função objetivo
int ValFunc_Obj(int ****, int ***, int , int , int , int );

int ****TrocaSol(int ****, int ****, int , int , int , int );

void AtualizaSol(void);

int main(){
    printf("------------------------------------------------\n");
    printf("-------------- Bem vindo ao GRASP --------------\n");
    printf("-------------------- Autores: ------------------\n");
    printf("---------- Daniel Carvalho e Laudelino ---------\n");
    printf("------------------------------------------------\n");

    int ****Aula = NULL;                    //Matriz aula do professor i,para a turma j,no dia k no horário l
    int ***Pref = NULL;                     //Matriz de preferência do professor em dia e horário
    int **cargaHoraria = NULL;                                 //Matriz de Carga Horária
    int nProfs, nTurmas, nDias, nHorarios;

    srand(time(NULL));
    GRASP(&Aula, &Pref, &cargaHoraria, &nProfs, &nTurmas, &nDias, &nHorarios);

    printf("\nProfessores: %d\n", nProfs);
    printf("Turmas: %d\n", nTurmas);
    printf("Dias: %d\n", nDias);
    printf("Horarios: %d\n", nHorarios);

    //fObjetivo(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);

    criaSimplex(Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
    desalocar(&Aula, &Pref, &cargaHoraria, nProfs, nTurmas, nDias);
    return TRUE;
}

void GRASP(int *****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    DadosEntrada(Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
    do{
        SolGulosaAleatoria(Aula, *Pref, *cargaHoraria, *nProfs, *nTurmas, *nDias, *nHorarios);
        //ImprimeSol(*Aula, *nProfs, *nTurmas, *nDias, *nHorarios);
        Simulated_Annealing(*Aula, *Pref, *cargaHoraria, *nProfs, *nTurmas, *nDias, *nHorarios);
        AtualizaSol();
    }while(FALSE);
    return;
}

void SolGulosaAleatoria(int *****Aula, int ***Pref, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j = 0, turma;
    int nCandidatos = (nDias * nHorarios) * rCandidatos;
    Candidato s, LRC[nCandidatos];
    printf("Candidatos: %d\n", nCandidatos);
    for(i = 0; i < nCandidatos; i++){               //Zeramos a lista LRC
        LRC[i].prof = 0;
        LRC[i].dia = 0;
        LRC[i].horario = 0;
    }
    //LimpaSol(Aula);
    do{
        for(i = 0; i < nProfs; i++){
            //Constroi a lista restrita de candidatos para cada professor
            ConsLRC(*Aula, Pref, LRC, nCandidatos, i, nProfs, nDias, nHorarios);
            //Escolhe um Candidato da LRC aleatoriamente
            s = LRC[rand()%nCandidatos];
            //Verifica se é viável
            for(turma = 0; turma < nTurmas; turma++){
                s.turma = turma;
                if(eViavel(*Aula, s, cargaHoraria, nProfs, nTurmas, nDias, nHorarios)){
                    (*Aula)[s.prof][turma][s.dia][s.horario] = 1;
                    break;
                }
            }
        }
        j++;
    }while(j < nTestes);                            //Quantas vezes quer-se verificar?
    printf("\nGuloso:\n");
    fObjetivo(*Aula, Pref, nProfs, nTurmas, nDias, nHorarios);
    return;
}

void ConsLRC(int ****Aula, int ***Pref, Candidato LRC[], int nCandidatos, int Prof, int nProfs, int nDias, int nHorarios){
    int i, j, k, J, K, max = 0;
    int pref[nProfs][nDias][nHorarios];

    //Cria uma cópia da mariz de preferência
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nDias; j++)
            for(k = 0; k < nHorarios; k++)
                pref[i][j][k] = Pref[i][j][k];

    for(i = 0; i < nCandidatos; i++){
        LRC[i].prof = Prof;
        for(j = 0; j < nDias; j++)
            for(k = 0; k < nHorarios; k++)
                //Escolhe o melhor elemento da matriz que já não tiver sido escolhido
                if((pref[Prof][j][k] >= max) && (Aula[Prof][0][j][k] != 1)&& (Aula[Prof][1][j][k] != 1)){
                    max = pref[Prof][j][k];
                    J = j;
                    K = k;
                }
        max = 0;
        LRC[i].dia = J;
        LRC[i].horario = K;
        pref[Prof][J][K] = -1;                      //Retira o elemento da matriz
    }
    return;
}

int eViavel(int ****Aula, Candidato c, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, contCH = 0, contHD = 0;//, contTurmas = 0;
    //int turmas[nTurmas];
    //for(i = 0; i< nTurmas; i++)
    //    turmas[i] = 0;

    // Restrições de Turma e Professor
    for(i = 0; i < nProfs; i++)
        if(Aula[i][c.turma][c.dia][c.horario] == 1)
            return FALSE;

    // Restrição de Carga Horária
    for(j = 0; j < nDias; j++)
        for(k = 0; k < nHorarios; k++)
            if(Aula[c.prof][c.turma][j][k] == 1)
                contCH++;
    if(contCH >= cargaHoraria[c.prof][c.turma])
        return FALSE;

    // Restrição de Horário Diário
    for(l = 0; l < nHorarios; l++)
        if(Aula[c.prof][c.turma][c.dia][l] == 1)
            contHD++;
    if(contHD >= horarioDiario)
        return FALSE;

    // Restrição de Turmas Mínimas
    //for(i = 0; i < nTurmas; i++)
    //    for(j = 0; j < nDias; j++){
    //        for(k = 0; k < nHorarios; k++){
    //            if((Aula[c.prof][i][j][k] == 1) && (turmas[i] == 0)){
    //                contTurmas++;
    //                turmas[i] = 1;
    //            }
    //        }
    //    }
    //if(contTurmas > maxTurmas)
    //    return FALSE;

    return TRUE;
}

void fObjetivo(int ****Aula, int ***Pref, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, fObjetivo = 0;
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    if(Aula[i][j][k][l] == 1)
                        fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
    printf("Funcao Objetivo: %d\n", fObjetivo);
}

void AtualizaSol(void){
}

void Simulated_Annealing(int ****Aula, int ***Pref, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    int IterT;
    double Te = T;
    int delta = 0;
    float ax = alpha;
    double prob;
    int ****Aulaviz = NULL;
    Aulaviz = criarAula(Aulaviz, nProfs, nTurmas, nDias, nHorarios);
    Aulaviz = TrocaSol(Aulaviz, Aula, nProfs, nTurmas, nDias, nHorarios);
    while(Te>Tmin)
    {
        IterT = 0;
        while(IterT < nTestes)
        {
            Aulaviz = BuscaVizinho(Aulaviz, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
            IterT = IterT + 1;
            delta = ValFunc_Obj(Aulaviz, Pref, nProfs, nTurmas, nDias, nHorarios) - ValFunc_Obj(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);
            prob = pow(2.71828,(delta/Te));
            if((delta > 0) || ((delta < 0) && (prob >= (rand()%101)/100.0))){
                Aula = TrocaSol(Aula, Aulaviz, nProfs, nTurmas, nDias, nHorarios);
            }
        }
        Te = Te * ax;
        break;
    }
    printf("Simulated Anneling:\n");
    fObjetivo(Aula, Pref, nProfs, nTurmas, nDias, nHorarios);
    return;
}

// Retorna uma Solução Vizinha Randomico Viável
int ****BuscaVizinho(int ****Aula, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    Candidato C;
    C.prof = rand()%nProfs;
    C.turma = rand()%nTurmas;
    C.dia = rand()%nDias;
    C.horario = rand()%nHorarios;

    // Checa a viabilidade desse Candidato
    if(eViavel(Aula, C, cargaHoraria, nProfs, nTurmas, nDias, nHorarios)){
        if(Aula[C.prof][C.turma][C.dia][C.horario] == 0){
            Aula[C.prof][C.turma][C.dia][C.horario] = 1;
        }else{
            Aula[C.prof][C.turma][C.dia][C.horario] = 0;
        }
    }

    return Aula;
}

int ValFunc_Obj(int ****Aula, int ***Pref, int nProfs, int nTurmas, int nDias, int nHorarios)
{
    int i, j, k, l, fObjetivo = 0;
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    if(Aula[i][j][k][l] == 1)
                        fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
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
