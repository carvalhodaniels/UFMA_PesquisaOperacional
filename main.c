#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libs.h"

#define TRUE 1
#define FALSE 0
#define rCandidatos 0.2
#define nTestes 50
#define horarioDiario 10

//  Inicia o GRASP
void GRASP(int *****, int ****, int ***, int *, int *, int *, int *);

//  Acha uma Solução Gulosa Aleatória
void SolGulosaAleatoria(int *****, int ***, int **, int , int , int , int );

//  Constroi a Lista Restrita de Candidatos
void ConsLRC(int ****, int ***, Candidato LRC[], int , int , int , int , int );

//  Verifica se o passo é viável
int eViavel(int ****, Candidato , int , int **, int , int , int , int );

void BuscaLocal(void);
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
    int i, j, k, l, fObjetivo = 0;

    srand(time(NULL));
    GRASP(&Aula, &Pref, &cargaHoraria, &nProfs, &nTurmas, &nDias, &nHorarios);

    printf("Professores: %d\n", nProfs);
    printf("Turmas: %d\n", nTurmas);
    printf("Dias: %d\n", nDias);
    printf("Horarios: %d\n", nHorarios);


    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    if(Aula[i][j][k][l] == 1)
                        fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
    printf("\nFuncao Objetivo: %d", fObjetivo);


    desalocar(&Aula, &Pref, &cargaHoraria, nProfs, nTurmas, nDias);
    return TRUE;
}

void GRASP(int *****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    DadosEntrada(Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
    do{
        SolGulosaAleatoria(Aula, *Pref, *cargaHoraria, *nProfs, *nTurmas, *nDias, *nHorarios);
        //ImprimeSol(*Aula, *nProfs, *nTurmas, *nDias, *nHorarios);
        BuscaLocal();
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
            for(turma = 0; turma < nTurmas; turma++)
                if(eViavel(*Aula, s, turma, cargaHoraria, nProfs, nTurmas, nDias, nHorarios)){
                    (*Aula)[s.prof][turma][s.dia][s.horario] = 1;
                    break;
                }
        }
        j++;
    }while(j < nTestes);                            //Quantas vezes quer-se verificar?
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

int eViavel(int ****Aula, Candidato c, int turma, int **cargaHoraria, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i, j, k, l, contCH = 0, contHD = 0;
    //Restrições de Turma e Professor
    for(i = 0; i < nProfs; i++)
        if(Aula[i][turma][c.dia][c.horario] == 1)
            return FALSE;

    //Restrição de Carga Horária
    for(j = 0; j < nDias; j++)
        for(k = 0; k < nHorarios; k++)
            if(Aula[c.prof][turma][j][k] == 1)
                contCH++;
    if(contCH >= cargaHoraria[c.prof][turma])
        return FALSE;

    //Restrição de Horário Diário
    for(k = 0; k < nTurmas; k++)
        for(l = 0; l < nHorarios; l++)
            if(Aula[c.prof][k][c.dia][l] == 1)
                contHD++;
    if(contHD >= horarioDiario)
        return FALSE;

    return TRUE;
}

void BuscaLocal(void){
}

void AtualizaSol(void){
}
