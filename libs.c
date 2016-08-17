#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs.h"

/*
    Lê o arquivo e coloca nas estruturas
*/
void DadosEntrada(int *****Aula, int ****Pref, int ***cargaHoraria, int *nProfs, int *nTurmas, int *nDias, int *nHorarios){
    int nProf = 0, nTurma = 0, nDia = 0, nHorario = 0;
    char buf[20], cH[20], *pCH, pref[20], *pPref;
    FILE *file;

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
    (*cargaHoraria) = criarCH(*cargaHoraria, nProf, nTurma);

    (*Pref) = criarPref(*Pref, nProf, nDia, nHorario);

    (*Aula) = criarAula(*Aula, nProf, nTurma, nDia, nHorario);

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
    Cria estrutura Carga Horária
*/
int **criarCH(int **cargaHoraria, int nProf, int nTurma){
    int i;
    cargaHoraria = (int **)malloc(nProf * sizeof(int *));
    if(cargaHoraria != NULL){
        for(i = 0; i < nProf; i++){
            cargaHoraria[i] = (int *)malloc(nTurma * sizeof(int));
            if(cargaHoraria[i] == NULL)
                return NULL;
        }
        return cargaHoraria;
    }
    return NULL;
}

/*
    Cria estrutura Preferência
*/
int ***criarPref(int ***Pref, int nProf, int nDia, int nHorario){
    int i, j;
    Pref = (int ***)malloc(nProf * sizeof(int **));
    if(Pref != NULL){
        for(i = 0; i < nProf; i++){
            Pref[i] = (int **)malloc(nDia * sizeof(int *));
            if(Pref[i] != NULL){
                for(j = 0; j < nDia; j++){
                    Pref[i][j] = (int *)malloc(nHorario * sizeof(int));
                    if(Pref[i][j] == NULL)
                        return NULL;
                }
            }else
                return NULL;
        }
        return Pref;
    }
    return NULL;
}

/*
    Cria estrutura Aula
*/
int ****criarAula(int ****Aula, int nProf, int nTurma, int nDia, int nHorario){
    int i, j, k;
    Aula = (int ****)malloc(nProf * sizeof(int ***));
    if(Aula != NULL){
        for(i = 0; i < nProf; i++){
            Aula[i] = (int ***)malloc(nTurma * sizeof(int **));
            if(Aula[i] != NULL){
                for(j = 0; j < nTurma; j++){
                    Aula[i][j] = (int **)malloc(nDia * sizeof(int *));
                    if(Aula[i][j] != NULL){
                        for(k = 0; k < nDia; k++){
                            Aula[i][j][k] = (int *)calloc(nHorario, sizeof(int));
                            if(Aula[i][j][k] == NULL)
                                return NULL;
                        }
                    }else
                        return NULL;
                }
            }else
                return NULL;
        }
        return Aula;
    }
    return NULL;
}

/*
    Desaloca Estruturas
*/
void desalocar(int *****Aula, int ****Pref, int ***cargaHoraria, int nProfs, int nTurmas, int nDias){
    int i, j, k;
    for(i = 0; i < nProfs; i++)
        free((*cargaHoraria)[i]);
    free(*cargaHoraria);
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++)
            free((*Pref)[i][j]);
        free((*Pref)[i]);
    }
    free(*Pref);
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++)
                free((*Aula)[i][j][k]);
            free((*Aula)[i][j]);
        }
        free((*Aula)[i]);
    }
    free(*Aula);
}

/*
    Imprime a Solução Atual
*/
void ImprimeSol(int ****Aula, int nProfs, int nTurmas, int nDias, int nHorarios){
    int i,j,k,l;
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
}

void criaSimplex(int ****Aula, int ***Pref, int **cargaHoraria,int nProfs, int nTurmas, int nDias, int nHorarios){
    FILE *fileO;
    int i, j, k, l;

    fileO = fopen("simplex.txt","w");

    if (fileO == NULL){
        printf("\nErro ao abrir o arquivo.\n");
        exit(0);
    }

    fprintf(fileO, "Max ");
    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                fprintf(fileO, "(");
                for(l = 0; l < nTurmas-1; l++){
                    fprintf(fileO, "x%d%d%d%d + ", i, l, j, k);
                }
                fprintf(fileO, "x%d%d%d%d", i, l, j, k);
                if((i == nProfs-1) && (j == nDias-1) && (k == nHorarios-1) &&(l == nTurmas-1))
                    fprintf(fileO, ")*%d", Pref[i][j][k]);
                else
                    fprintf(fileO, ")*%d + ", Pref[i][j][k]);
            }
        }
    }

    fprintf(fileO, "\n\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                fprintf(fileO, "(");
                for(l = 0; l < nTurmas-1; l++){
                    fprintf(fileO, "x%d%d%d%d + ", i, l, j, k);
                }
                fprintf(fileO, "x%d%d%d%d", i, l, j, k);
                fprintf(fileO, ") <= 1\n");
            }
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nTurmas; i++){
        for(j = 0; j < nDias; j++){
            for(k = 0; k < nHorarios; k++){
                fprintf(fileO, "(");
                for(l = 0; l < nProfs-1; l++){
                    fprintf(fileO, "x%d%d%d%d + ", l, i, j, k);
                }
                fprintf(fileO, "x%d%d%d%d", l, i, j, k);
                fprintf(fileO, ") <= 1\n");
            }
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            fprintf(fileO, "(");
            for(k = 0; k < nDias; k++){
                for(l = 0; l < nHorarios; l++){
                    if((k == nDias-1) &&(l == nHorarios-1))
                        break;
                    fprintf(fileO, "x%d%d%d%d + ", i, j, k, l);
                }
            }
            fprintf(fileO, "x%d%d%d%d) ", i, j, k, l);
            fprintf(fileO, "<= ");
            fprintf(fileO, "%d\n", cargaHoraria[i][j]);
        }
    }

    fprintf(fileO, "\n");

    for(i = 0; i < nProfs; i++){
        for(j = 0; j < nTurmas; j++){
            for(k = 0; k < nDias; k++){
                fprintf(fileO, "(");
                for(l = 0; l < nHorarios-1; l++){
                    fprintf(fileO, "x%d%d%d%d + ", i, j, k, l);
                }
                fprintf(fileO, "x%d%d%d%d", i, j, k, l);
                fprintf(fileO, ") <= 4\n");
            }
        }
    }

    fclose(fileO);
}
