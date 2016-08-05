#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define nProfs 2
#define nTurmas 2
#define nDias 5
#define nHorarios 5
#define nCandidatos 3

typedef struct candidato{
    int prof;
    int dia;
    int horario;
}Candidato;

void GRASP(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]);
void DadosEntrada(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]);
void SolGulosaAleatoria(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]);
void ConsLRC(int Pref[nProfs][nDias][nHorarios], Candidato LRC[nCandidatos], int Prof);
int eViavel(Candidato c);
void BuscaLocal(void);
void AtualizaSol(void);
void LimpaSol(int Aula[nProfs][nTurmas][nDias][nHorarios]);

int main(){
    printf("------------------------------------------------\n");
    printf("-------------- Bem vindo ao GRASP --------------\n");
    printf("------------ Autor: Daniel Carvalho ------------\n");
    printf("------------------------------------------------\n");

    int Aula[nProfs][nTurmas][nDias][nHorarios];        //Matriz aula do professor i,para a turma j,no dia k no horário l
    int Pref[nProfs][nDias][nHorarios];                 // Matriz de preferência do professor em dia e horário

    srand(time(NULL));
    GRASP(Aula, Pref);

    return TRUE;
}

void GRASP(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]){
    int i,j,k,l;
    DadosEntrada(Aula, Pref);
    do{
        SolGulosaAleatoria(Aula, Pref);
        for(i = 0; i < nProfs; i++){
            printf("\nProfessor %d:\n",i);
            printf("   S T Q Q S\n");
            for(j = 0; j < nTurmas; j++){
                printf("T%d:\n",j);
                for(l = 0; l < nHorarios; l++){
                    printf("%d: ",l);
                    for(k = 0; k < nDias; k++)
                        printf("%d ", Aula[i][j][k][l]);
                    printf("\n");
                }
            }
        }
        BuscaLocal();
        AtualizaSol();
    }while(FALSE);
    return;
}

void DadosEntrada(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]){
    int i, j, k;
    LimpaSol(Aula);
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nDias; j++)
            for(k = 0; k < nHorarios; k++)
                Pref[i][j][k] = rand()%5;
}

void SolGulosaAleatoria(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios]){
    int i, j = 0;
    Candidato s, LRC[nCandidatos];
    for(i = 0; i < nCandidatos; i++){
        LRC[i].prof = 0;
        LRC[i].dia = 0;
        LRC[i].horario = 0;
    }
    LimpaSol(Aula);
    do{
        for(i = 0; i < nProfs; i++){
            ConsLRC(Pref, LRC, i);                   //Constroi a lista restrita de candidatos para cada professor
            s = LRC[rand()%nCandidatos];
            //printf("\n%d %d %d\n",s.prof, s.dia, s.horario);
            if(eViavel(s))
                Aula[s.prof][i][s.dia][s.horario] = 1;
            j++;
        }
    }while(j < 10);
    return;
}

void ConsLRC(int Pref[nProfs][nDias][nHorarios], Candidato LRC[nCandidatos], int Prof){
    int i, j, k, J, K, max = 0;
    int pref[nProfs][nDias][nHorarios];

    for(i = 0; i < nProfs; i++)                     //Copia a mariz de preferência
        for(j = 0; j < nDias; j++)
            for(k = 0; k < nHorarios; k++)
                pref[i][j][k] = Pref[i][j][k];

    for(i = 0; i < nCandidatos; i++){
        LRC[i].prof = Prof;
        for(j = 0; j < nDias; j++)
            for(k = 0; k < nHorarios; k++)
                if(pref[Prof][j][k] >= max){         //Escolhe o melhor elemento da matriz
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

int eViavel(Candidato c){
    return TRUE;
}

void BuscaLocal(void){
}

void AtualizaSol(void){
}

void LimpaSol(int Aula[nProfs][nTurmas][nDias][nHorarios]){
    int i,j,k,l;
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    Aula[i][j][k][l] = 0;
    return;
}
