#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char CP[5];
    int NUM;
    char LETRAS[2];

} t_matricula;
    
typedef struct
{
    t_matricula MATRICULA;
    char PROPIETARIO[25]; 
    char DIRECCION[40];
    int ANYO_MATRIC;

} t_automobil;


char Ciutat(char *cp);
void GeneraRegistres();
void EntraRegistres();
void MostraRegistres(char registre);
void MostraCotxe(t_automobil Auto);
void PintaMenu();
void Neteja(char opcio);


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
    int tria;
    char opcio;

    printf("=== Generador de registres d'automobils ===\n");
    printf("1. Entrar Cotxes\n");
    printf("2. Generar Registres\n");
    printf("3. Mostrar Registre\n");
    printf("4. Sortir\n");
    printf("Tria: ");
    scanf("%d%*c", &tria);

    switch(tria)
    {
        case 1:
            EntraRegistres();
            break;
        case 2:
            GeneraRegistres();
            break;
        case 3:
            Neteja('P');
            printf("Quin registre voleu veure?\n");
            printf("[B] BCN\n");
            printf("[M] MADRID\n");
            printf("[T] TOTS\n");
            printf("Tria: ");
            scanf("%c%*c", &opcio);
            MostraRegistres(opcio);
            break;
        case 4:
            exit(0);
        default:
            printf("Opcio incorrecte.\n");
            break;
    }
    PintaMenu();
}

char Ciutat(char *cp)
{
    char ciutat;

    if (cp[0] == 48 && cp[1] == 56)
    {
        ciutat = 'B';
    } 
    else if (cp[0] == 50 && cp[1] == 56)
    {
        ciutat = 'M';
    }
    else
    {
        ciutat = 'N';
    }

    return ciutat;
}

void GeneraRegistres()
{
    FILE *AUTOS = fopen("Autos.dat", "r");
    FILE *A1 = fopen("Matriculats_BCN.dat", "w");
    FILE *A2 = fopen("Matriculats_MADRID.dat", "w");
    t_automobil Auto;

    if(!(fread(&Auto, sizeof(Auto), 1, AUTOS) > 0))
    {
        printf("\nError al llegir el fitxer de cotxes, no en teniu cap de registrat?\n");
        printf("Prem una tecla per continuar...");
        getchar();
    }
    else
    {
        rewind(AUTOS);
        while(fread(&Auto, sizeof(Auto), 1, AUTOS) > 0)
        {
            if(Auto.ANYO_MATRIC == 1990)
            {
                if (Ciutat(Auto.MATRICULA.CP) == 'B')
                {
                    fwrite(&Auto, sizeof(Auto), 1, A1);
                }
                else if (Ciutat(Auto.MATRICULA.CP) == 'M')
                {
                    fwrite(&Auto, sizeof(Auto), 1, A2);
                }

            }
        }
    }

    fclose(AUTOS);
    fclose(A1);
    fclose(A2);
    printf("\nRegistros generados correctamente!\n");
    printf("\nPrem una tecla per continuar...");
    getchar();
}

void EntraRegistres()
{
    Neteja('P');
    FILE *FITXER;
    t_automobil AUTOS;
    char continua = 'S';

    FITXER = fopen("Autos.dat", "a");

    while(continua == 'S')
    {
        printf("\nMatricula CP: ");
        scanf("%[^\n]%*c", AUTOS.MATRICULA.CP);
        printf("Matricula NUM: ");
        scanf("%d%*c", &AUTOS.MATRICULA.NUM);
        printf("Matricula LETRAS: ");
        scanf("%[^\n]%*c", AUTOS.MATRICULA.LETRAS);
        printf("Propietario: ");
        scanf("%[^\n]%*c", AUTOS.PROPIETARIO);
        printf("Direccion: ");
        scanf("%[^\n]%*c", AUTOS.DIRECCION);
        printf("Anyo matriculacion: ");
        scanf("%d%*c", &AUTOS.ANYO_MATRIC);

        fwrite(&AUTOS, sizeof(AUTOS), 1, FITXER);

        printf("Continuar introduciendo? (S/N): ");
        scanf("%c%*c", &continua);
    }
    fclose(FITXER);
    printf("\nRegistros entrados correctamente!\n");
    printf("\nPrem una tecla per continuar...");
    getchar();
}

void MostraRegistres(char registre)
{
    Neteja('P');
    FILE *AUTOS = fopen("Autos.dat", "r");
    FILE *A1 = fopen("Matriculats_BCN.dat", "r");
    FILE *A2 = fopen("Matriculats_MADRID.dat", "r");
    t_automobil Auto;

    if (registre == 'B')
    {   
        printf("\nRegistros de: BCN");
        printf("\n-----------------------------------------\n");
        while(fread(&Auto, sizeof(Auto), 1, A1) > 0)
            MostraCotxe(Auto);

    }
    else if(registre == 'M')
    {   
        printf("\nRegistros de: MADRID");
        printf("\n-----------------------------------------\n");
        while(fread(&Auto, sizeof(Auto), 1, A2) > 0)
            MostraCotxe(Auto);

    }
    else if(registre == 'T')
    {   
        printf("\nRegistros de: TODOS");
        printf("\n-----------------------------------------\n");
        while(fread(&Auto, sizeof(Auto), 1, AUTOS) > 0)
            MostraCotxe(Auto);

    }
    else
    {
        printf("\nOpcio incorrecte.\n");
    }
    printf("\nPrem una tecla per continuar...");
    getchar();
}

void MostraCotxe(t_automobil Auto)
{
    printf("Matricula CP: %s\n", Auto.MATRICULA.CP);
    printf("Matricula NUM: %d\n", Auto.MATRICULA.NUM);
    printf("Matricula LETRAS: %s\n", Auto.MATRICULA.LETRAS);
    printf("Propietario: %s\n", Auto.PROPIETARIO);
    printf("Direccion: %s\n", Auto.DIRECCION);
    printf("Anyo matriculacion: %d\n", Auto.ANYO_MATRIC);
    printf("-----------------------------------------\n");
}

