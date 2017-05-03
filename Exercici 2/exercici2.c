#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_REGISTRES 50

typedef struct 
{
    int any;
    int mes;
    int dia;
} t_data;

typedef struct 
{
    char titol[30];
    char autor[20];
    char editorial[15];
    int any;
    t_data data;
    int id;
    int id_prestat;
} t_llibre;

typedef struct 
{
    char nom[12];
    char cognom_1[12];
    char cognom_2[12];
    int telefon;
    char direccio[40];
    int id_alumne;
} t_alumne;


void PintaMenu();
void EntraLlibres(FILE * llibres);
void EntraAlumnes(FILE * alumnes);
void DeixarLlibre(FILE * llibres, FILE * alumnes, FILE * prestecs, FILE * temp);
void TornarLlibre(FILE * llibres, FILE * prestecs, FILE * temp);
void TriaVeureRegistres();
void MostraRegistres(char registre);
void MostraLlibre(t_llibre Llibre);
void MostraAlumne(t_alumne Alumne);
void MostraPrestecs(t_llibre Prestec);
void Neteja(char opcio);
void MostraError(char tipus);

int main()
{
    PintaMenu();
}

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

void PintaMenu()
{
    Neteja('P');
    FILE *llibres, *alumnes, *prestecs, *temp;
    int tria;

    printf("=== Gestio de prestecs de llibres ===\n");
    printf("1. Entrar Llibre/s\n");
    printf("2. Entrar Alumnes\n");
    printf("3. Prestar Llibre\n");
    printf("4. Tornar Llibre\n");
    printf("5. Veure Registres\n");
    printf("6. Sortir\n");
    printf("Tria: ");
    scanf("%d%*c", &tria);

    switch(tria)
    {
        case 1:
            EntraLlibres(llibres);
            break;
        case 2:
            EntraAlumnes(alumnes);
            break;
        case 3:
            DeixarLlibre(llibres, alumnes, prestecs, temp);
            break;
        case 4:
            TornarLlibre(llibres, prestecs, temp);
            break;
        case 5:
            TriaVeureRegistres();
            break;
        case 6:
            exit(0);
        default:
            MostraError('O');
            break;
    }
    PintaMenu();
}

void EntraLlibres(FILE * llibres)
{
    Neteja('P');
    t_llibre LLIBRE;
    char continua = 'S';

    llibres = fopen("llibres.dat", "a");

    while(continua == 'S')
    {
        printf("Titol: ");
        scanf("%[^\n]%*c", LLIBRE.titol);
        printf("Autor: ");
        scanf("%[^\n]%*c", LLIBRE.autor);
        printf("Editorial: ");
        scanf("%[^\n]%*c", LLIBRE.editorial);
        printf("Any: ");
        scanf("%d%*c", &LLIBRE.any);
        printf("IBAN (Numero ID): ");
        scanf("%d%*c", &LLIBRE.id);
        LLIBRE.id_prestat = -1;

        fwrite(&LLIBRE, sizeof(LLIBRE), 1, llibres);

        printf("\nContinuar entrant llibres? (S/N): ");
        scanf("%c%*c", &continua);
    }
    fclose(llibres);
    printf("\nLlibres entrats correctament! Prem una tecla per continuar...");
    getchar();
}

void EntraAlumnes(FILE * alumnes)
{
    Neteja('P');
    t_alumne ALUMNE;
    char continua = 'S';

    alumnes = fopen("alumnes.dat", "a");

    while(continua == 'S')
    {
        printf("Nom: ");
        scanf("%[^\n]%*c", ALUMNE.nom);
        printf("Primer Cognom: ");
        scanf("%[^\n]%*c", ALUMNE.cognom_1);
        printf("Segon Cognom: ");
        scanf("%[^\n]%*c", ALUMNE.cognom_2);
        printf("Telefon: ");
        scanf("%d%*c", &ALUMNE.telefon);
        printf("Direccio: ");
        scanf("%[^\n]%*c", ALUMNE.direccio);
        printf("ID Alumne: ");
        scanf("%d%*c", &ALUMNE.id_alumne);

        fwrite(&ALUMNE, sizeof(ALUMNE), 1, alumnes);

        printf("\nContinuar entrant alumnes? (S/N): ");
        scanf("%c%*c", &continua);
    }
    fclose(alumnes);
    printf("\nAlumnes entrats correctament! Prem una tecla per continuar...");
    getchar();
}

void DeixarLlibre(FILE * llibres, FILE * alumnes, FILE * prestecs, FILE * temp)
{
    Neteja('P');
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    t_llibre Prestec;
    t_alumne Alumne;
    int ID_llibre, ID_alumne, i = 0, exemplars[MAX_REGISTRES];

    llibres = fopen("llibres.dat", "r");
    alumnes = fopen("alumnes.dat", "r");
    prestecs = fopen("prestecs.dat", "a");
    temp = fopen("temp/tmp.dat", "w");

    printf("Llibres disponibles:\n");
    if(fread(&Prestec, sizeof(Prestec), 1, llibres) > 0)
    {
        rewind(llibres);
        while(fread(&Prestec, sizeof(Prestec), 1, llibres) > 0)
        {
            if (Prestec.id_prestat == -1)
                printf("%s - %d; ", Prestec.titol, Prestec.id);
            
            fwrite(&Prestec, sizeof(Prestec), 1, temp);
            exemplars[i] = Prestec.id;
            k++;
        }
        exemplars[i] = '\0';
        printf("\n\nQuin llibre vols prestar? \n(Escriu ID, -1 si no vols prestar): ");
        scanf("%d%*c", &ID_llibre);

        int existent = 0;
        i = 0;
        while (i < MAX_REGISTRES && exemplars[i] != '\0')
        {   
            if (ID_llibre == exemplars[i])
                existent = 1;
            i++;
        }

        if (ID_llibre >= 0 && existent)
        {
            printf("\nAlumnes:\n");
            if (fread(&Alumne, sizeof(Alumne), 1, alumnes) > 0)
            {
                rewind(alumnes);
                while(fread(&Alumne, sizeof(Alumne), 1, alumnes) > 0)
                    printf("%s - %d; ", Alumne.nom, Alumne.id_alumne);
                
                printf("\n\nA quin alumne el vols prestar? (Escriu ID): ");
                scanf("%d%*c", &ID_alumne);

                fclose(llibres);
                fclose(temp);
                llibres = fopen("llibres.dat", "w");
                temp = fopen("temp/tmp.dat", "r");
                
                if (fread(&Prestec, sizeof(Prestec), 1, temp) > 0)
                {
                    rewind(temp);
                    while(fread(&Prestec, sizeof(Prestec), 1, temp) > 0)
                    {
                        if (Prestec.id == ID_llibre)
                        {
                            Prestec.id_prestat = ID_alumne;
                            Prestec.data.dia = tm.tm_mday;
                            Prestec.data.mes = tm.tm_mon + 1;
                            Prestec.data.any = tm.tm_year - 100;
                            fwrite(&Prestec, sizeof(Prestec), 1, prestecs);
                            fwrite(&Prestec, sizeof(Prestec), 1, llibres);
                            printf("Heu prestat el llibre %s correctament. Prem una tecla per continuar...", Prestec.titol);
                        } 
                        else 
                        {
                            fwrite(&Prestec, sizeof(Prestec), 1, llibres);
                        }
                    }
                }
                else
                {
                    MostraError('F');
                }
            }
            else
            {
                MostraError('L');
            }
        }
        else
        {
            if (ID_llibre < 0)
                MostraError('C');
            else
                MostraError('I');
        }
    }
    else
    {
        MostraError('L');
    }
    fclose(temp);
    fclose(llibres);
    fclose(alumnes);
    fclose(prestecs);
    getchar();
}

void TornarLlibre(FILE * llibres, FILE * prestecs, FILE * temp)
{
    Neteja('P');
    t_llibre Prestec;
    int ID_llibre, ID_alumne, teLlibres = 0;

    prestecs = fopen("prestecs.dat", "r");
    temp = fopen("temp/tmp2.dat", "w");

    printf("Identifica't.\n(Escriu el teu ID, -1 si no vols tornar llibre): ");
    scanf("%d%*c", &ID_alumne);

    if (ID_alumne >= 0)
    {
        printf("\nLlibres en possesio:\n");
        if (fread(&Prestec, sizeof(Prestec), 1, prestecs) > 0)
        {
            rewind(prestecs);
            while(fread(&Prestec, sizeof(Prestec), 1, prestecs) > 0)
            {
                if (Prestec.id_prestat == ID_alumne)
                {
                    teLlibres = 1;
                    printf("%s - %d; ", Prestec.titol, Prestec.id);
                    fwrite(&Prestec, sizeof(Prestec), 1, temp);
                }
                else
                {
                    fwrite(&Prestec, sizeof(Prestec), 1, temp);
                }
            }
            if(teLlibres)
            {
                printf("\nQuin llibre dels que tens vols tornar? (ID llibre): ");
                scanf("%d%*c", &ID_llibre);

                fclose(prestecs);
                fclose(temp);

                prestecs = fopen("prestecs.dat", "w");
                temp = fopen("temp/tmp2.dat", "r");

                if (fread(&Prestec, sizeof(Prestec), 1, temp) > 0)
                {
                    rewind(temp);
                    while(fread(&Prestec, sizeof(Prestec), 1, temp) > 0)
                    {
                        if (Prestec.id != ID_llibre)
                            fwrite(&Prestec, sizeof(Prestec), 1, prestecs);
                        else
                            printf("Heu tornal el llibre %s correctament. Prem una tecla per continuar...", Prestec.titol);
                    }
                    fclose(temp);
                    
                    llibres = fopen("llibres.dat", "r");
                    temp = fopen("temp/tmp2.dat", "w");

                    if (fread(&Prestec, sizeof(Prestec), 1, llibres) > 0)
                    {
                        rewind(llibres);
                        while(fread(&Prestec, sizeof(Prestec), 1, llibres) > 0)
                            fwrite(&Prestec, sizeof(Prestec), 1, temp);
                        
                        fclose(temp);
                        fclose(llibres);
                        llibres = fopen("llibres.dat", "w");
                        temp = fopen("temp/tmp2.dat", "r");

                        while(fread(&Prestec, sizeof(Prestec), 1, temp) > 0)
                        {
                            if (Prestec.id != ID_llibre)
                            {
                                fwrite(&Prestec, sizeof(Prestec), 1, llibres);
                            }
                            else
                            {
                                Prestec.id_prestat = -1;
                                fwrite(&Prestec, sizeof(Prestec), 1, llibres);
                            }
                        }
                    }
                    else
                    {
                        MostraError('F');
                    }

                }
                else
                {
                    MostraError('T');
                    getchar();
                }
            }
            else
            {
                printf("L'ID introduit no te en possesio cap llibre. Prem una tecla per continuar...");
            }
        }
        else
        {
            MostraError('L');
        }
    }
    else
    {
        MostraError('C');
    }
    fclose(temp);
    fclose(prestecs);
    fclose(llibres);
    getchar();

}

void TriaVeureRegistres()
{
    Neteja('P');
    char opcio, opcio_correcte;

    do
    {
        printf("Quin registre voleu veure?\n");
        printf("[L] Llibres\n");
        printf("[A] Alumnes\n");
        printf("[P] Prestecs\n");
        printf("Tria: ");
        scanf("%c%*c", &opcio);
        
        opcio_correcte = 1;

        switch(opcio)
        {
            case 76:
                MostraRegistres('L');
                break;
            case 65:
                MostraRegistres('A');
                break;
            case 80:
                MostraRegistres('P');
                break;
            default:
                printf("La opcio escollida es incorrecte.\n");
                opcio_correcte = 0;
                break;
        }
    } while (!opcio_correcte);

}

void MostraRegistres(char registre)
{
    Neteja('P');
    FILE *PRESTECS = fopen("prestecs.dat", "r");
    FILE *ALUMNES = fopen("alumnes.dat", "r");
    FILE *LLIBRES = fopen("llibres.dat", "r");
    t_llibre Llibre;
    t_alumne Alumne;
    t_llibre Prestec;

    if (registre == 'L')
    {   
        printf("\nRegistres de: LLIBRES");
        printf("\n-----------------------------------------\n");
        while(fread(&Llibre, sizeof(Llibre), 1, LLIBRES) > 0)
            MostraLlibre(Llibre);
    }
    else if(registre == 'A')
    {   
        printf("\nRegistres de: ALUMNES");
        printf("\n-----------------------------------------\n");
        while(fread(&Alumne, sizeof(Alumne), 1, ALUMNES) > 0)
            MostraAlumne(Alumne);
    }
    else if(registre == 'P')
    {   
        printf("\nRegistres de: PRESTECS");
        printf("\n-----------------------------------------\n");
        while(fread(&Prestec, sizeof(Prestec), 1, PRESTECS) > 0)
            MostraPrestecs(Prestec);
    }
    else
    {
        MostraError('O');
    }
    fclose(PRESTECS);
    fclose(ALUMNES);
    fclose(LLIBRES);
    printf("\nPrem una tecla per continuar...");
    getchar();
}

void MostraLlibre(t_llibre Llibre)
{
    printf("Titol: %s\n", Llibre.titol);
    printf("Autor: %s\n", Llibre.autor);
    printf("Editorial: %s\n", Llibre.editorial);
    printf("Any: %d\n", Llibre.any);
    printf("IBAN (Numero ID): %d\n", Llibre.id);
    printf("-----------------------------------------\n");
}

void MostraAlumne(t_alumne Alumne)
{
    printf("Nom: %s\n", Alumne.nom);
    printf("Primer Cognom: %s\n", Alumne.cognom_1);
    printf("Segon Cognom: %s\n", Alumne.cognom_2);
    printf("Telefon: %d\n", Alumne.telefon);
    printf("Direccio: %s\n", Alumne.direccio);
    printf("ID Alumne: %d\n", Alumne.id_alumne);
    printf("-----------------------------------------\n");
}

void MostraPrestecs(t_llibre Prestec)
{
    printf("Titol: %s\n", Prestec.titol);
    printf("Autor: %s\n", Prestec.autor);
    printf("Editorial: %s\n", Prestec.editorial);
    printf("Any: %d\n", Prestec.any);
    printf("IBAN (Numero ID): %d\n", Prestec.id);
    printf("Data de prestec: %d/%02d/%d\n", Prestec.data.dia, Prestec.data.mes, Prestec.data.any);
    printf("Prestat a: %d (ID Alumne)\n", Prestec.id_prestat);
    printf("-----------------------------------------\n");
}

void MostraError(char tipus)
{
    if (tipus == 'L')
    {
        printf("\n[SISTEMA] Error al llegir el fitxer, es possible que no tinguem cap registre \nassociat a la consulta que voleu.\nPrem una tecla per continuar...");
    }
    else if(tipus == 'C')
    {
        printf("\nOperacio cancelada. Prem una tecla per continuar...");
    }
    else if(tipus == 'T')
    {
        printf("Error al en la transaccio. Prem una tecla per continuar...");
    }
    else if (tipus == 'O')
    {
        printf("\nOpcio incorrecte.\n");
    }
    else if(tipus == 'F')
    {
        printf("\n[SISTEMA] Error amb el fitxer. Prem una tecla per continuar...");
    }
    else if(tipus == 'I')
    {
        printf("\nError, el ID introduit no coincideix amb cap ID dels exemplars existents.\nPrem una tecla per continuar...");
    }
}