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
    printf(ANSI_COLOR_RED);
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
        printf("\nL'alumne a modificar no s'ha trobat. ");
    else if(error == 'R')
        printf("\nHi ha hagut un error al procesar la modificacio del registre.\n\n");
    else if(error == 'M')
        printf("\nNumero de matricula ja registrat, utilitzi la opcio per a modificar alta.\n\n");
    else if(error == 'N')
        printf("\nError, el camp no pot contenir lletres.\n\n");
    else if(error == 'e')
        printf("\nError, el numero de matricula de l'alumne que voleu modificar no esta assignat.\n\n");
    else if(error == 'E')
        printf("\nSiusplau, introdueixi un numero de matricula existent.\n\n");


    printf(ANSI_COLOR_RESET);
    getchar();
}

void MostraSucces(char * succes)
{
    printf(ANSI_COLOR_GREEN);
    printf("%s", succes);
    printf(ANSI_COLOR_RESET);
}

void AfegirRegistreAlum(char * path, t_alum registre)
{
    FILE * fitxer;
    t_alum reg_temp, reg_a_ordenar[1000];
    int siono, repetit, existeix, num_linies = 0, i;
    char continuar;
    do
    {
        Neteja('P');
        /** lectura del fitxer **/
        fitxer = fopen(path, "r");

        //contem les linies i llegim el contingut guardantlo en un vector
        while(fread(&reg_temp, sizeof(reg_temp), 1, fitxer) > 0)
        {
            reg_a_ordenar[num_linies] = reg_temp;
            num_linies++;
        }
        fclose(fitxer);

        /** captura de dades **/
        // Demanem al usuari les dades del alumne
        do
        {
            existeix = 0;
            printf("Digues matricula alumne (3 digits): ");
            scanf("%d%*c", &registre.ANT_NMAT);
            // comprovem que sigui de 3 digits
            if (!(registre.ANT_NMAT >= 0) || !(registre.ANT_NMAT < 1000))
                MostraError('n');

            //comprovem que no existeixi ja
            for (i = 0; i < num_linies; i++)
            {
                if (registre.ANT_NMAT == reg_a_ordenar[i].ANT_NMAT)
                {
                    existeix = 1;
                    MostraError('M');
                }
            }
        } while (!(registre.ANT_NMAT >= 0) || !(registre.ANT_NMAT < 1000) || existeix);

        printf("Digues nom alumne: ");
        scanf("%[^\n]%*c", registre.ANT_NOMB);
        do
        {
            printf("Digues si s'ha efectuat reserva (0 o 1): ");
            scanf("%d%*c", &siono);
            if (siono == 0 || siono == 1)
                registre.ANT_RESERV = siono;
            else
                MostraError('I');

        } while (siono != 0 && siono != 1);

        printf("Digues adresa alumne: ");
        scanf("%[^\n]%*c", registre.ANT_DIR);
        printf("Digues numero de faltes: ");
        scanf("%d%*c", &registre.ANT_NFALT);
        printf("Digues numero de suspeses: ");
        scanf("%d%*c", &registre.ANT_NSUSP);

        //afegim el registre al nostre vector
        reg_a_ordenar[num_linies] = registre;
        num_linies++;

        /** ordenacio del fitxer **/
        //ordenem el vector que conte tots els registres
        OrdenaAlum(reg_a_ordenar, num_linies);

        /** escriptura del fitxer **/
        fitxer = fopen(path, "w");
        for (i = 0; i < num_linies; i++)
            fwrite(&reg_a_ordenar[i], sizeof(t_alum), 1, fitxer);

        fclose(fitxer);

        MostraSucces("\nDades introduides correctament!\n\n");
        getchar();

        printf("Voleu continuar introduint dades? (S/N): ");
        scanf("%c%*c", &continuar);
    } while (continuar == 'S' || continuar == 's');
}

void AfegirRegistreNouAlum(char * path, t_movi registre)
{
    FILE * fitxer, * fitxer2;
    int num_linies = 0, num_linies2 = 0, i, existeix;
    char continuar;
    t_movi reg_temp, reg_a_ordenar[1000];
    t_alum reg_temp2, reg_comprovar[1000];
    do
    {
        Neteja('P');
        /** lectura del fitxer **/
        fitxer = fopen(path, "r");

        //contem les linies i llegim el contingut guardantlo en vectors
        while(fread(&reg_temp, sizeof(reg_temp), 1, fitxer) > 0)
        {
            reg_a_ordenar[num_linies] = reg_temp;
            num_linies++;
        }
        fclose(fitxer);

        fitxer2 = fopen(ANT_ALUM_FILE, "r");
        while(fread(&reg_temp2, sizeof(reg_temp2), 1, fitxer2) > 0)
        {
            reg_comprovar[num_linies2] = reg_temp2;
            num_linies2++;
        }
        fclose(fitxer2);


        /** captura de dades **/
        // Demanem al usuari les dades del alumne a donar d'alta
        do
        {
            printf("Digues matricula alumne (3 digits): ");
            scanf("%d%*c", &registre.MV_NMAT);
            if (!(registre.MV_NMAT >= 0) || !(registre.MV_NMAT < 1000))
                MostraError('n');

        } while (!(registre.MV_NMAT >= 0) || !(registre.MV_NMAT < 1000));

        printf("Digues nom alumne: ");
        scanf("%[^\n]%*c", registre.MV_NOMB);
        printf("Digues adresa alumne: ");
        scanf("%[^\n]%*c", registre.MV_DIR);
        do
        {
            printf("Digues si es una alta o una modificacio (A/M): ");
            scanf("%c%*c", &registre.MV_TIP);
            if (registre.MV_TIP != 'M' && registre.MV_TIP != 'A')
                MostraError('I');

        } while(registre.MV_TIP != 'M' && registre.MV_TIP != 'A');

        //en cas de que sigui una modificacio, comprovem que tenim el num ja assignat
        if(registre.MV_TIP = 'M')
        {
            existeix = 0;
            for (i = 0; i < num_linies2; i++)
            {
                if (registre.MV_NMAT == reg_comprovar[i].ANT_NMAT)
                    existeix = 1;
            }

            if(!existeix)
            {
                int tria;
                do
                {
                    MostraError('e');
                    printf("Que voleu fer?\n");
                    printf("[1] Modificar la matricula.\n");
                    printf("[2] Donar d'alta ĺ'alumne.\n");
                    printf("[3] Cap, sortir.\n");
                    printf("Tria: ");
                    scanf("%d%*c", &tria);

                    switch(tria)
                    {
                        case 1:
                            do
                            {
                                existeix = 0;
                                printf("Digues matricula alumne (3 digits): ");
                                scanf("%d%*c", &registre.MV_NMAT);
                                if (!(registre.MV_NMAT >= 0) || !(registre.MV_NMAT < 1000))
                                    MostraError('n');

                                for (i = 0; i < num_linies2; i++)
                                {
                                    if (registre.MV_NMAT == reg_comprovar[i].ANT_NMAT)
                                    {
                                        existeix = 1;
                                    }
                                }
                                if (!existeix)
                                    MostraError('E');
                            } while (!(registre.MV_NMAT >= 0) || !(registre.MV_NMAT < 1000) || !existeix);
                            
                            break;

                        case 2:
                            registre.MV_TIP = 'A';
                            printf("\nEs donara d'alta l'alumne.\n");    
                            break;
                        case 3:
                            return;
                            break;
                        default:
                            MostraError('o');
                    }
                } while (tria != 1 && tria != 2 && tria != 3);
            }
        }

        //afegim el registre al vector que conte tota la informacio
        reg_a_ordenar[num_linies] = registre;
        num_linies++;

        /** ordenacio del fitxer **/
        //ordenem el vector que conte tots els registres
        OrdenaNouAlum(reg_a_ordenar, num_linies);

        /** escriptura del fitxer **/
        fitxer = fopen(path, "w");
        for (i = 0; i < num_linies; i++)
            fwrite(&reg_a_ordenar[i], sizeof(t_movi), 1, fitxer);

        fclose(fitxer);

        MostraSucces("\nDades introduides correctament!\n\n");
        getchar();

        printf("Voleu continuar introduint dades? (S/N): ");
        scanf("%c%*c", &continuar);
    } while (continuar == 'S' || continuar == 's');
}

void MostraBaixaAlum(t_alum registre, char * motiu)
{
    printf("\nL'alumne %s amb matricula %03d es donat de baixa per %s. \n", registre.ANT_NOMB, registre.ANT_NMAT, motiu);
}

void MostraRegistres(char tipus)
{
    FILE * f1, * f2, * f3;
    t_alum alumne;
    t_movi nou_alumne;
    int error_fitxer_a = 0, error_fitxer_na = 0, error_fitxer_nfg = 0;

    if(fopen(ANT_ALUM_FILE, "r") != NULL) f1 = fopen(ANT_ALUM_FILE, "r");
    else error_fitxer_a = 1;

    if(fopen(FICH_MOVI_FILE, "r") != NULL) f2 = fopen(FICH_MOVI_FILE, "r");
    else error_fitxer_na = 1;

    if(fopen(FICH_ALUM_FILE, "r") != NULL) f3 = fopen(FICH_ALUM_FILE, "r");
    else error_fitxer_nfg = 1;

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
    Neteja('P');
    FILE * fitxer_alumnes, * fitxer_nous_alumnes, * nou_fitxer;
    int error_fitxer_a = 0, error_fitxer_na = 0, error_proces = 0, nou_fitxer_tancat = 0;
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
        if(fread(&nou_alumne, sizeof(nou_alumne), 1, fitxer_nous_alumnes) > 0)
        {
            rewind(fitxer_nous_alumnes);

            while(fread(&nou_alumne, sizeof(nou_alumne), 1, fitxer_nous_alumnes) > 0)
            {
                if(!nou_fitxer_tancat) fclose(nou_fitxer);
                nou_fitxer_tancat = 1;
                if (nou_alumne.MV_TIP == 'A')
                { //si es una alta
                    nou_fitxer = fopen(FICH_ALUM_FILE, "a");
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
                    int alumneTrobat = 0;
                    FILE * temp = fopen("temp/modificacio_reg.tmp", "w");
                    nou_fitxer = fopen(FICH_ALUM_FILE, "r");
                    nou_fitxer_tancat = 0;

                    if (fread(&alumne, sizeof(alumne), 1, nou_fitxer) > 0)
                    {
                        rewind(nou_fitxer);
                        while(fread(&alumne, sizeof(alumne), 1, nou_fitxer) > 0)
                        {
                            if (alumne.ANT_NMAT == nou_alumne.MV_NMAT)
                            {
                                strcpy(alumne.ANT_DIR, nou_alumne.MV_DIR);
                                alumneTrobat = 1;
                            }
                            fwrite(&alumne, sizeof(t_alum), 1, temp);
                        }
                    }
                    else //si no hi ha cap registre en el fitxer nou
                        MostraError('r');

                    fclose(temp);
                    if(!nou_fitxer_tancat) fclose(nou_fitxer);
                    nou_fitxer_tancat = 1;

                    if (alumneTrobat)
                    {
                        /* si hem trobat alumne, per tant em modificat, copiem les noves dades del fitxer temporal */
                        nou_fitxer = fopen(FICH_ALUM_FILE, "w");
                        nou_fitxer_tancat = 0;
                        temp = fopen("temp/modificacio_reg.tmp", "r"); 

                        if (fread(&alumne, sizeof(alumne), 1, temp) > 0)
                        {
                            rewind(temp);
                            while(fread(&alumne, sizeof(alumne), 1, temp) > 0)
                                fwrite(&alumne, sizeof(t_alum), 1, nou_fitxer);
                        }
                        else
                            MostraError('R');

                        fclose(temp);
                        if(!nou_fitxer_tancat) fclose(nou_fitxer);
                        nou_fitxer_tancat = 1;

                    } //si no hem trobat alumne
                    else
                    {
                        printf("\n");
                        MostraError('t');
                        printf("Matricula alumne proporcionada: %03d. Nom: %s", nou_alumne.MV_NMAT, nou_alumne.MV_NOMB);
                    }
                }
            } // endwhile
        }
        else
        {
            MostraError('r');
            error_proces = 1;
        }

        fclose(fitxer_nous_alumnes);
    }

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
    printf("--------------------\n   Menu Principal \n--------------------\n\n");
    printf(" [1] Introduir alumnes ja registrats\n");
    printf(" [2] Alta / Modificacio Alumne\n");
    printf(" [3] Procesa els registres\n");
    printf(" [4] Mostrar Registres\n");
    printf(" [5] Sortir\n");
    printf("\nDigues que vols fer: ");
    scanf("%d%*c", &tria);

    switch(tria)
    {
        case 1:
            AfegirRegistreAlum(ANT_ALUM_FILE, vAlum);
            break;
        case 2:
            AfegirRegistreNouAlum(FICH_MOVI_FILE, nAlum);
            break;
        case 3:
            processament = ProcesaDades();
            if (processament)
            {
                MostraSucces("\n\n\nDades introduides correctament!\n\n");
                getchar();
            }
            else
            {
                MostraError('p');
            }
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

void OrdenaAlum(t_alum * reg_per_ordenar, int num_linies)
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

void OrdenaNouAlum(t_movi * reg_per_ordenar, int num_linies)
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
    printf("Matricula alumne (3 digits): %03d\n", alumne.ANT_NMAT);
    printf("Nom alumne: %s\n",  alumne.ANT_NOMB);
    printf("Ha efectuat reserva: %d\n", alumne.ANT_RESERV);
    printf("Adresa alumne: %s\n", alumne.ANT_DIR);
    printf("Numero de faltes: %d\n", alumne.ANT_NFALT);
    printf("Numero de suspeses: %d\n", alumne.ANT_NSUSP);
    printf("-----------------------------------------\n");
}

void MostraNouAlumne(t_movi nou_alumne)
{
    printf("Matricula alumne (3 digits): %03d\n", nou_alumne.MV_NMAT);
    printf("Nom alumne: %s\n", nou_alumne.MV_NOMB);
    printf("Adresa alumne: %s\n", nou_alumne.MV_DIR);
    printf("Modificacio / Alta: %c\n", nou_alumne.MV_TIP);
    printf("-----------------------------------------\n");
}