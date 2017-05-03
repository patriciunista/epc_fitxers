#include <stdio.h>

#ifndef FUNC_H_
#define FUNC_H_

#define MAX_CARACTERS 30
#define MAX_DIGITS 3
#define ANT_ALUM_FILE "registres/ANT_ALUM.dat" // ordenat per el num matricula
#define FICH_MOVI_FILE "registres/FICH_MOVI.dat" // ordenat per el mv_nmat
#define FICH_ALUM_FILE "registres/FICH_ALUM.dat" // ordenat per el alu_nmat

typedef struct // Definicio del registre fitxer ANT_ALUM
{
    int ANT_NMAT;//[MAX_DIGITS];
    char ANT_NOMB[MAX_CARACTERS];
    int ANT_RESERV;
    char ANT_DIR[MAX_CARACTERS];
    int ANT_NFALT;
    int ANT_NSUSP;
} t_alum;


typedef struct // Definicio del registre fitxer FICH_MOVI
{
    int MV_NMAT;//[MAX_DIGITS];
    char MV_NOMB[MAX_CARACTERS];
    char MV_DIR[MAX_CARACTERS];
    char MV_TIP;
} t_movi;


void Neteja(char opcio);
t_alum AltaAlumne(t_alum alumne);
t_movi AltaNouAlumne(t_movi nou_alumne);
void AfegirRegistre();
void ModificarRegistre(t_alum adresa, t_movi adresaNova);
void MostraErrors(char error);
void MostraBaixaAlum(t_alum registre, char * motiu);
void MostraRegistres(char tipus);
void PintaMenu();
void MostraAlumne(t_alum alumne);
void MostraNouAlumne(t_movi nou_alumne);
void OrdenaAlum(t_alum *reg_per_ordenar, int num_linies);
void OrdenaNouAlum(t_movi *reg_per_ordenar, int num_linies);
int ProcesaDades();

#endif
