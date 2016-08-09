#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
//#define nProfs 2
//#define nTurmas 2
//#define nDias 5
//#define nHorarios 5
#define nCandidatos 3
#define nTestes 50
#define horarioDiario 3

typedef struct candidato{
    int prof;
    int dia;
    int horario;
}Candidato;

void GRASP(int ****, int ****, int ***, int *, int *, int *, int *);
void DadosEntrada(int ****, int ****, int ***, int *, int *, int *, int*);
//void SolGulosaAleatoria(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios], int cargaHoraria[nProfs][nTurmas]);
//void ConsLRC(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios], Candidato LRC[nCandidatos], int Prof);
//int eViavel(int Aula[nProfs][nTurmas][nDias][nHorarios], Candidato c, int turma, int cargaHoraria[nProfs][nTurmas]);
void BuscaLocal(void);
void AtualizaSol(void);
//void LimpaSol(int Aula[nProfs][nTurmas][nDias][nHorarios]);

int main(){
    printf("------------------------------------------------\n");
    printf("-------------- Bem vindo ao GRASP --------------\n");
    printf("-------------------- Autores: ------------------\n");
    printf("---------- Daniel Carvalho e Laudelino ---------\n");
    printf("------------------------------------------------\n");

    int ****Aula = NULL;        //Matriz aula do professor i,para a turma j,no dia k no horário l
    int ***Pref = NULL;                 //Matriz de preferência do professor em dia e horário
    int **cargaHoraria = NULL;                                 //Matriz de Carga Horária
    int nProfs, nTurmas, nDias, nHorarios;
    int i, j, k;//, l, fObjetivo = 0;

    srand(time(NULL));
    GRASP(Aula, &Pref, &cargaHoraria, &nProfs, &nTurmas, &nDias, &nHorarios);

    printf("Professores: %d\n", nProfs);
    printf("Turmas: %d\n", nTurmas);
    printf("Dias: %d\n", nDias);
    printf("Horarios: %d\n", nHorarios);
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++)
            printf("%d ", cargaHoraria[i][j]);
        printf("\n");
    }
    for(i = 0; i < nProfs; i++){
        printf("\n");
        for(j = 0; j < nHorarios; j++){
            for(k = 0; k < nDias; k++)
                printf("%d ", Pref[i][k][j]);
            printf("\n");
        }
    }
    /*
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    if(Aula[i][j][k][l] == 1)
                        fObjetivo+= Aula[i][j][k][l] * Pref[i][k][l];
    printf("\nFuncao Objetivo: %d", fObjetivo);
    */

    for(i = 0; i < nProfs;i++)
        free(cargaHoraria[i]);
    free(cargaHoraria);
    for(i = 0; i < nProfs;i++){
        //for(j = 0; j < nDias;i++)
        //    free(Pref[i][j]);
        free(Pref[i]);
    }
    free(Pref);
    return TRUE;
}

void GRASP(int ****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    int i,j,k,l;
    DadosEntrada(Aula, Pref, cargaHoraria, nProfs, nTurmas, nDias, nHorarios);
    return;
    do{
        //SolGulosaAleatoria(Aula, Pref, cargaHoraria);
        for(i = 0; i < *nProfs; i++){
            printf("\nProfessor %d:\n",i);
            printf("   S T Q Q S\n");
            for(j = 0; j < *nTurmas; j++){
                printf("T%d:\n",j);
                for(l = 0; l < *nHorarios; l++){
                    printf("%d: ",l);
                    for(k = 0; k < *nDias; k++)
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

void DadosEntrada(int ****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    int i, j, nProf = 0, nTurma = 0, nDia = 0, nHorario = 0;
    char buf[20], cH[20], *pCH, pref[20], *pPref;
    FILE *file;
    //LimpaSol(Aula);

    file = fopen("entrada.txt","r");
    if (file == NULL){
        printf("\nErro ao abrir o arquivo.\n");
        exit(0);
    }
    while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Carga Horaria:\n") != 0){
            fseek (file, -(strlen(buf)+1), SEEK_CUR);
            break;
        }
        fgets(cH, 50, file);
        nProf++;
        nTurma = 0;
        pCH = strtok(cH," ");
        while (pCH != NULL){
            pCH = strtok(NULL, " ");
            nTurma++;
        }
    }
     while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Preferencia:\n") != 0){
            fseek (file, -strlen(buf)+1, SEEK_CUR);
            break;
        }
        fgets(pref, 50, file);
        nHorario++;
        nDia = 0;
        pPref = strtok(pref, " ");
        while (pPref != NULL){
            pPref = strtok(NULL, " ");
            nDia++;
        }
    }

    *nProfs = nProf;
    *nTurmas = nTurma;
    *nDias = nDia;
    *nHorarios = nHorario;

    //Alocações aqui
    (*cargaHoraria) = (int **)malloc(nProf * sizeof(int *));
    if((*cargaHoraria) != NULL){
        for(i = 0; i < nProf; i++){
            fflush(file);
            (*cargaHoraria)[i] = (int *)malloc(nTurma * sizeof(int));
            if((*cargaHoraria)[i] == NULL){
                printf("Falha na alocacao\n");
                exit(0);
            }
        }
    }else{
        printf("Falha na alocacao\n");
        exit(0);
    }
    (*Pref) = (int ***)malloc(nProf * sizeof(int **));
    if((*Pref) != NULL){
        for(i = 0; i < nProf; i++){
            (*Pref)[i] = (int **)malloc(nDia * sizeof(int *));
            if((*Pref)[i] != NULL){
                for(j = 0; j < nDia; j++){
                    (*Pref)[i][j] = (int *)malloc(nHorario * sizeof(int));
                    if((*Pref)[i][j] == NULL){
                        printf("Falha na alocacao\n");
                        exit(0);
                    }
                }
            }else{
                printf("Falha na alocacao\n");
                exit(0);
            }
        }
    }else{
        printf("Falha na alocacao\n");
        exit(0);
    }

    rewind(file);
    nProf = nTurma = 0;
    while (!feof(file)){
        fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Carga Horaria:\n") != 0){
            fseek (file, -(strlen(buf)+1), SEEK_CUR);
            break;
        }
        fgets(cH, 50, file);
        nProf++;
        nTurma = 0;
        pCH = strtok(cH," ");
        while (pCH != NULL){
            nTurma++;
            (*cargaHoraria)[nProf-1][nTurma-1] = atoi(pCH);
            pCH = strtok (NULL, " ");
        }
    }
    nHorario = 0;
    nProf = 0;
    fscanf(file, "%[A-Z a-z :\n]s", buf);
        if(strcmp(buf, "Preferencia:\n") != 0){
            fseek (file, -(strlen(buf)+1), SEEK_CUR);
            //break;
        }
    while (!feof(file)){
        fgets(pref, 50, file);
        if(strcmp(pref, "\n") == 0){
            fgets(pref, 50, file);
            printf("\n");
            nHorario = 0;
            nProf++;
        }
        nHorario++;
        nDia = 0;
        pPref = strtok(pref, " ");
        while (pPref != NULL){
            nDia++;
            (*Pref)[nProf][nDia-1][nHorario-1] = atoi(pPref);
            pPref = strtok (NULL, " ");
        }
    }
    fclose(file);
}
/*
void SolGulosaAleatoria(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios], int cargaHoraria[nProfs][nTurmas]){
    int i, j = 0, turma;
    Candidato s, LRC[nCandidatos];
    for(i = 0; i < nCandidatos; i++){               //Zeramos a lista LRC
        LRC[i].prof = 0;
        LRC[i].dia = 0;
        LRC[i].horario = 0;
    }
    LimpaSol(Aula);
    do{
        for(i = 0; i < nProfs; i++){
            ConsLRC(Aula, Pref, LRC, i);                   //Constroi a lista restrita de candidatos para cada professor
            s = LRC[rand()%nCandidatos];
            //printf("\n%d %d %d\n",s.prof, s.dia, s.horario);
            for(turma = 0; turma < nTurmas; turma++)
                if(eViavel(Aula, s, turma, cargaHoraria)){
                    Aula[s.prof][turma][s.dia][s.horario] = 1;
                    break;
                }
        }
        j++;
    }while(j < nTestes);             //Quantas vezes quer-se verificar?
    return;
}

void ConsLRC(int Aula[nProfs][nTurmas][nDias][nHorarios], int Pref[nProfs][nDias][nHorarios], Candidato LRC[nCandidatos], int Prof){
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
                if((pref[Prof][j][k] >= max) && (Aula[Prof][0][j][k] != 1)&& (Aula[Prof][1][j][k] != 1)){         //Escolhe o melhor elemento da matriz
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

int eViavel(int Aula[nProfs][nTurmas][nDias][nHorarios], Candidato c, int turma, int cargaHoraria[nProfs][nTurmas]){
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

void LimpaSol(int Aula[nProfs][nTurmas][nDias][nHorarios]){
    int i, j, k, l;
    for(i = 0; i < nProfs; i++)
        for(j = 0; j < nTurmas; j++)
            for(k = 0; k < nDias; k++)
                for(l = 0; l < nHorarios; l++)
                    Aula[i][j][k][l] = 0;
    return;
}
*/
