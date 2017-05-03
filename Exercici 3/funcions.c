#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcions.h"

void Neteja(char opcio)
{
    switch(opcio)
    {
        case 'L':
            printf("\r%c[K", 27);
            break;
        case 'P':
            printf("%c[2J", 27);
            printf("%c[H", 27);
            break;
    }
}

void MostraError(char error)
{
    if(error == 'I')
        printf("\nError, la dada introduida es incorecte.\n\n");
    else if(error == 'o')
        printf("\nOpcio incorrecte.\n\n");
    else if(error == 'F')
        printf("\nEl fitxer que voleu consultar no ha estat creat!\n\n");
    else if(error == 'r')
        printf("\nEl fitxer no conte cap registre.\n\n");
    else if(error == 'n')
        printf("\nLa matricula es incorrecte, ha de contenir 3 digits.\n\n");
    else if(error == 'p')
        printf("\nError en el processament de les dades.\n\n");
    else if(error == 't')
        printf("\nL'alumne a modificar no s'ha trobat.\n\n");
    else if(error == 'R')
        printf("\nHi ha hagut un error al procesar la modificacio del registre.\n\n");



    getchar();
}


t_alum AltaAlumne(t_alum alumne)
{
    Neteja('P');
    int siono;
    do
    {
        printf("Digues matricula alumne (3 digits): ");
        scanf("%d%*c", &alumne.ANT_NMAT);
        if (!(alumne.ANT_NMAT >= 0) || !(alumne.ANT_NMAT < 1000))
            MostraError('n');

    } while (!(alumne.ANT_NMAT >= 0) || !(alumne.ANT_NMAT < 1000));

    printf("Digues nom alumne: ");
    scanf("%[^\n]%*c", alumne.ANT_NOMB);

    do
    {
        printf("Digues si s'ha efectuat reserva (0 o 1): ");
        scanf("%d%*c", &siono);
        if (siono == 0 || siono == 1)
            alumne.ANT_RESERV = siono;
        else
            MostraError('I');

    } while (siono != 0 && siono != 1);

    printf("Digues adresa alumne: ");
    scanf("%[^\n]%*c", alumne.ANT_DIR);
    printf("Digues numero de faltes: ");
    scanf("%d%*c", &alumne.ANT_NFALT);
    printf("Digues numero de suspeses: ");
    scanf("%d%*c", &alumne.ANT_NSUSP);
    return alumne;
}

t_movi AltaNouAlumne(t_movi nou_alumne)
{
    Neteja('P');
    do
    {
        printf("Digues matricula alumne (3 digits): ");
        scanf("%d%*c", &nou_alumne.MV_NMAT);
        if (!(nou_alumne.MV_NMAT >= 0) || !(nou_alumne.MV_NMAT < 1000))
            MostraError('n');

    } while (!(nou_alumne.MV_NMAT >= 0) || !(nou_alumne.MV_NMAT < 1000));

    printf("Digues matricula alumne (3 digits): ");
    printf("Digues nom alumne: ");
    scanf("%[^\n]%*c", nou_alumne.MV_NOMB);
    printf("Digues adresa alumne: ");
    scanf("%[^\n]%*c", nou_alumne.MV_DIR);
    do
    {
        printf("Digues si es una alta o una modificacio (A/M): ");
        scanf("%c%*c", &nou_alumne.MV_TIP);
        if (nou_alumne.MV_TIP != 'M' && nou_alumne.MV_TIP != 'A')
            MostraError('I');

    } while(nou_alumne.MV_TIP != 'M' && nou_alumne.MV_TIP != 'A');

    return nou_alumne;
}


void AfegirRegistreAlum(char * path, t_alum registre)
{
    FILE * fitxer;
    FILE * temp;
    fitxer = fopen(path, "a");
    int num_linies = 0, i;
    t_alum reg_temp, reg_a_ordenar[1000];

    /** escriptura del fitxer **/
    fwrite(&registre, sizeof(t_alum), 1, fitxer);
    fclose(fitxer);

    /** ordenacio del fitxer **/
    fitxer = fopen(path, "r");

    //contem les linies, no es molt eficient pero es necessari
    while(fread(&reg_temp, sizeof(reg_temp), 1, fitxer) > 0)
    {
        reg_a_ordenar[num_linies] = reg_temp;
        num_linies++;
    }
    fclose(fitxer);

    //ordenem el vector que conte tots els registres
    OrdenaAlum(reg_a_ordenar, num_linies);

    temp = fopen("temp/registre_alum.tmp", "w");
    for (i = 0; i < num_linies; i++)
        fwrite(&reg_a_ordenar[i], sizeof(t_alum), 1, temp);

    fclose(temp);

    fitxer = fopen(path, "w");
    temp = fopen("temp/registre_alum.tmp", "r");

    while(fread(&reg_temp, sizeof(reg_temp), 1, temp) > 0)
        fwrite(&reg_temp, sizeof(t_alum), 1, fitxer);

    fclose(fitxer);
    fclose(temp);

    printf("\nDades introduides correctament!\n\n");
    getchar();
}

void AfegirRegistreNouAlum(char * path, t_movi registre)
{
    FILE * fitxer;
    FILE * temp;
    fitxer = fopen(path, "a");
    int num_linies = 0, i;
    t_movi reg_temp, reg_a_ordenar[1000];

    /** escriptura del fitxer **/
    fwrite(&registre, sizeof(t_movi), 1, fitxer);
    fclose(fitxer);

    /** ordenacio del fitxer **/
    fitxer = fopen(path, "r");

    //contem les linies, no es molt eficient pero es necessari
    while(fread(&reg_temp, sizeof(reg_temp), 1, fitxer) > 0){
        reg_a_ordenar[num_linies] = reg_temp;
        num_linies++;
    }
    fclose(fitxer);

    //ordenem el vector que conte tots els registres
    OrdenaNouAlum(reg_a_ordenar, num_linies);

    temp = fopen("temp/registre_nou_alum.tmp", "w");
    for (i = 0; i < num_linies; i++)
        fwrite(&reg_a_ordenar[i], sizeof(t_movi), 1, temp);

    fclose(temp);

    fitxer = fopen(path, "w");
    temp = fopen("temp/registre_nou_alum.tmp", "r");

    while(fread(&reg_temp, sizeof(reg_temp), 1, temp) > 0)
        fwrite(&reg_temp, sizeof(t_movi), 1, fitxer);

    fclose(fitxer);
    fclose(temp);

    printf("\nDades introduides correctament!\n\n");
    getchar();
}

void ModificarRegistre(t_alum adresa, t_movi adresaNova){
    strcpy(adresaNova.MV_DIR, adresa.ANT_DIR);
}

void MostraBaixaAlum(t_alum registre, char * motiu)
{
    printf("\nL'alumne %s amb matricula %d es donat de baixa per %s. \n", registre.ANT_NOMB, registre.ANT_NMAT, motiu);
}

void MostraRegistres(char tipus)
{
    FILE * f1;
    FILE * f2;
    FILE * f3;
    int error_fitxer_a = 0, error_fitxer_na = 0, error_fitxer_nfg = 0;

    if(fopen(ANT_ALUM_FILE, "r") != NULL) f1 = fopen(ANT_ALUM_FILE, "r");
    else error_fitxer_a = 1;

    if(fopen(FICH_MOVI_FILE, "r") != NULL) f2 = fopen(FICH_MOVI_FILE, "r");
    else error_fitxer_na = 1;

    if(fopen(FICH_ALUM_FILE, "r") != NULL) f3 = fopen(FICH_ALUM_FILE, "r");
    else error_fitxer_nfg = 1;

    t_alum alumne;
    t_movi nou_alumne;

    if (tipus == 'A')
    {
        if (!error_fitxer_a)
        {
            if(fread(&alumne, sizeof(alumne), 1, f1) > 0)
            {
                rewind(f1);
                printf("\nRegistres de: ALUMNE");
                printf("\n-----------------------------------------\n");
                while(fread(&alumne, sizeof(alumne), 1, f1) > 0)
                    MostraAlumne(alumne);
                getchar();
            }
        }
        else
            MostraError('F');
    }
    else if (tipus == 'F')
    {
        if (!error_fitxer_na)
        {
            if(fread(&nou_alumne, sizeof(nou_alumne), 1, f2) > 0)
            {
                rewind(f2);
                printf("\nRegistres de: NOU ALUMNE");
                printf("\n-----------------------------------------\n");
                while(fread(&nou_alumne, sizeof(nou_alumne), 1, f2) > 0)
                    MostraNouAlumne(nou_alumne);
                getchar();
            }
        }
        else
            MostraError('F');
    }
    else
    {
        if (!error_fitxer_nfg)
        {
            printf("dins fitxer generat\n");
            if(fread(&alumne, sizeof(t_alum), 1, f3) > 0)
            {
                rewind(f3);
                printf("\nRegistres de: NOU FITXER");
                printf("\n-----------------------------------------\n");
                while(fread(&alumne, sizeof(alumne), 1, f3) > 0)
                    MostraAlumne(alumne);
                getchar();
            } else
                MostraError('r');
        } 
        else
            MostraError('F');
    }

    if (!error_fitxer_a)
        fclose(f1);
    if (!error_fitxer_na)
        fclose(f2);
    if (!error_fitxer_nfg)
        fclose(f3);
}

int ProcesaDades()
{
    FILE * fitxer_alumnes;
    FILE * fitxer_nous_alumnes;
    FILE * nou_fitxer;
    int error_fitxer_a = 0, error_fitxer_na = 0, error_proces = 0;
    t_alum alumne;
    t_movi nou_alumne;

    if(fopen(ANT_ALUM_FILE, "r") != NULL) fitxer_alumnes = fopen(ANT_ALUM_FILE, "r");
    else error_fitxer_a = 1;

    if(fopen(FICH_MOVI_FILE, "r") != NULL) fitxer_nous_alumnes = fopen(FICH_MOVI_FILE, "r");
    else error_fitxer_na = 1;

    nou_fitxer = fopen(FICH_ALUM_FILE, "w");

    if(!error_fitxer_a)
    {
        if(fread(&alumne, sizeof(alumne), 1, fitxer_alumnes) > 0)
        {
            rewind(fitxer_alumnes);
            while(fread(&alumne, sizeof(alumne), 1, fitxer_alumnes) > 0)
            {
                if (!alumne.ANT_RESERV)
                    MostraBaixaAlum(alumne, "no fer reserva");
                else if (alumne.ANT_NFALT >= 50)
                    MostraBaixaAlum(alumne, "faltes");
                else if(alumne.ANT_NSUSP > 3)
                    MostraBaixaAlum(alumne, "assignatures suspeses");
                else
                    fwrite(&alumne, sizeof(alumne), 1, nou_fitxer);
            }
        } 
        else 
        {
            MostraError('r');
            error_proces = 1;
        }
        fclose(fitxer_alumnes);
    }
    else
    {
        MostraError('F');
        error_proces = 1;
    }

    if (!error_fitxer_na)
    {
        if(fread(&nou_alumne, sizeof(nou_alumne), 1, fitxer_alumnes) > 0)
        {
            rewind(fitxer_alumnes);
            while(fread(&nou_alumne, sizeof(nou_alumne), 1, fitxer_alumnes) > 0)
            {
                if (nou_alumne.MV_TIP == 'A')
                { //si es una alta
                    alumne.ANT_NMAT = nou_alumne.MV_NMAT;
                    strcpy(alumne.ANT_NOMB, nou_alumne.MV_NOMB);
                    strcpy(alumne.ANT_DIR, nou_alumne.MV_DIR);
                    alumne.ANT_NSUSP = 0;
                    alumne.ANT_NFALT = 0;
                    alumne.ANT_RESERV = 1;
                    fwrite(&alumne, sizeof(alumne), 1, nou_fitxer);
                }
                else //si es una modificacio
                {
                    fclose(nou_fitxer);
                    int alumneTrobat = 0;
                    FILE * temp = fopen("temp/modificacio_reg.tmp", "w");
                    nou_fitxer = fopen(FICH_ALUM_FILE, "r");
                    
                    if (fread(&alumne, sizeof(alumne), 1, nou_fitxer) > 0)
                    {
                        rewind(nou_fitxer);
                        while(fread(&alumne, sizeof(alumne), 1, nou_fitxer) > 0)
                        {
                            if (alumne.ANT_NMAT == nou_alumne.MV_NMAT)
                            {
                                ModificarRegistre(alumne, nou_alumne);
                                alumneTrobat = 1;
                            }
                            fwrite(&alumne, sizeof(t_alum), 1, temp);
                        }
                        fclose(temp);
                    } 
                    else //si no hi ha cap registre en el fitxer nou
                        MostraError('r'); 

                    fclose(nou_fitxer);

                    if (alumneTrobat)
                    { 
                        /* si hem trobat alumne, per tant em modificat, copiem les noves dades del fitxer temporal */
                        nou_fitxer = fopen(FICH_ALUM_FILE, "w");
                        temp = fopen("temp/modificacio_reg.tmp", "r");

                        if (fread(&alumne, sizeof(alumne), 1, temp) > 0)
                        {
                            rewind(temp);
                            while(fread(&alumne, sizeof(alumne), 1, temp) > 0)
                                fwrite(&alumne, sizeof(t_alum), 1, nou_fitxer);

                            fclose(temp);
                        }
                        else
                            MostraError('R');

                        fclose(nou_fitxer);
                    } //si no hem trobat alumne
                    else
                        MostraError('t');
                }
            }
        } 
        else 
        {
            MostraError('r');
            error_proces = 1;
        }

        fclose(fitxer_nous_alumnes);
    }
    

    fclose(nou_fitxer);

    if (error_proces)
        return 0;

    return 1;
}


void PintaMenu()
{   
    Neteja('P');
    t_alum vAlum;
    t_movi nAlum;
    int tria, processament;
    char tipusReg;
    printf("Menu Principal\n");
    printf("[1] Alta Alumne\n");
    printf("[2] Alta Nou Alumne\n");
    printf("[3] Procesa les dades\n");
    printf("[4] Mostrar Registres\n");
    printf("[5] Sortir\n");
    printf("Digues que vols fer: ");
    scanf("%d%*c", &tria);

    switch(tria)
    {
        case 1:
            vAlum = AltaAlumne(vAlum);
            AfegirRegistreAlum(ANT_ALUM_FILE, vAlum);
            break;
        case 2:
            nAlum = AltaNouAlumne(nAlum);
            AfegirRegistreNouAlum(FICH_MOVI_FILE, nAlum);
            break;
        case 3:
            processament = ProcesaDades();
            if (processament)
                printf("\nDades introduides correctament!\n\n");
            else
                MostraError('p');
            break;
        case 4:
            printf("\n\nQuin tipus de registre voleu veure?\n");
            printf("[A] Alumnes\n");
            printf("[F] Nous Alumnes\n");
            printf("[N] Nou Fitxer Generat\n");
            printf("[S] Cap, sortir\n");
            printf("Tria: ");
            scanf("%c%*c", &tipusReg);
            if (tipusReg == 'A' || tipusReg == 'F' || tipusReg == 'N')
                MostraRegistres(tipusReg);
            else if (tipusReg == 'S')
                break;
            else
                MostraError('o');
            break;
        case 5: 
            exit(0);
        default:
            MostraError('o');
            break;
    }

    PintaMenu();
}


void OrdenaAlum(t_alum *reg_per_ordenar, int num_linies)
{
  int i, j;
  for (i = 0; i < num_linies - 1; i++)
  {
    for (j = 0; j < num_linies - i - 1; j++)
    {
      if (reg_per_ordenar[j].ANT_NMAT > reg_per_ordenar[j + 1].ANT_NMAT)
      {
        t_alum temp;
        temp = reg_per_ordenar[j];
        reg_per_ordenar[j] = reg_per_ordenar[j + 1];
        reg_per_ordenar[j + 1] = temp;
      }
    }
  }
}


void OrdenaNouAlum(t_movi *reg_per_ordenar, int num_linies)
{
  int i, j;
  for (i = 0; i < num_linies - 1; i++)
  {
    for (j = 0; j < num_linies - i - 1; j++)
    {
      if (reg_per_ordenar[j].MV_NMAT > reg_per_ordenar[j + 1].MV_NMAT)
      {
        t_movi temp;
        temp = reg_per_ordenar[j];
        reg_per_ordenar[j] = reg_per_ordenar[j + 1];
        reg_per_ordenar[j + 1] = temp;
      }
    }
  }
}

void MostraAlumne(t_alum alumne)
{
    printf("Matricula alumne (3 digits): %3d\n", alumne.ANT_NMAT);
    printf("Nom alumne: %s\n",  alumne.ANT_NOMB);
    printf("Ha efectuat reserva: %d\n", alumne.ANT_RESERV);
    printf("Adresa alumne: %s\n", alumne.ANT_DIR);
    printf("Numero de faltes: %d\n", alumne.ANT_NFALT);
    printf("Numero de suspeses: %d\n\n", alumne.ANT_NSUSP);
}

void MostraNouAlumne(t_movi nou_alumne)
{
    printf("Matricula alumne (3 digits): %3d\n", nou_alumne.MV_NMAT);
    printf("Nom alumne: %s\n", nou_alumne.MV_NOMB);
    printf("Adresa alumne: %s\n", nou_alumne.MV_DIR);
    printf("Modificacio / Alta: %c\n\n", nou_alumne.MV_TIP);
}