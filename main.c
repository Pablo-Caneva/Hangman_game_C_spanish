#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

void cargar_palabras();
void cargar_ranking();
void menu();
void errores(int cant_error);
void victoria(int x);
int comienzojuego();
int juego(int n);
int ingresar_nombre();
void ordenar_puntajes(int maximo_logrado);
void mostrar_ranking();
void escribir_ranking();

struct palabras
{
    int indice;
    char wrd[16];
};

struct palabras lista_palabras[50];

struct puntajes
{
    int max;
    char jugador[31];
};

struct puntajes ranking_puntajes[10];

char nombre[31];

int main()
{
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    cargar_palabras();
    cargar_ranking();
    int eleccion_menu, bandera_ingreso=0, puntaje;
    do
    {
        system("cls");
        menu();
        scanf("%d", &eleccion_menu);
        fflush(stdin);
        switch(eleccion_menu)
        {
            case 1:
                bandera_ingreso = ingresar_nombre();
                break;
            case 2:
                {
                    if (bandera_ingreso==0)
                    {
                        printf ("DEBE INGRESAR UN NOMBRE PRIMERO.\nPRESIONE ENTER PARA CONTINUAR...\n");
                        getchar();
                        break;
                    }
                    else
                    {
                        puntaje = comienzojuego();
                        if (puntaje>=0)
                            ordenar_puntajes(puntaje);
                        break;
                    }
                }
            case 3:
                mostrar_ranking();
                break;
            case 4:
                escribir_ranking();
                exit(1);
                break;
        }
    }
    while (eleccion_menu!=4);
    return 0;
}

int ingresar_nombre()
{
    int bandera_nombre=1;
    int k;
    do
    {
        k=0;
        system("cls");
        if (bandera_nombre==0)
        {
            printf("LAS LETRAS DEBEN SER TODAS MAYUSCULAS.\n");
        }
        bandera_nombre=1;
        printf("INGRESE SU NOMBRE, UTILIZANDO SOLO MAYUSCULAS.\n");
        gets(nombre);
        while (nombre[k]!='\0')
        {
            if ((nombre[k]<65)||(nombre[k]>90))
                bandera_nombre=0;
            k++;
        }
    }
    while (bandera_nombre==0);
    return 1;
}

int comienzojuego()
{
    int suma_puntos=0, puntos, nro;
    do
    {
        srand(time(0));
        nro = rand()%50;
        puntos = juego(nro);
        suma_puntos = suma_puntos + puntos;
    }
    while (puntos>0);
    if (puntos==0)
        return suma_puntos;
    else
        return -1;
}

int juego (int n)
{
    int err=0, letra_correcta, puntos=100, acumulados=0;
    int l = strlen(lista_palabras[n].wrd);
    char control[16];
    strcpy (control, lista_palabras[n].wrd);
    char letra_ingresada, opcion;
    for (int i=0; i<l; i++)
    {
        control[i]='_';
    }
    do
    {
        system("cls");
        letra_correcta=0;
        for (int i=0; i<l; i++)
        {
            printf("%c ", control[i]);
        }
        printf("\n\n");
        errores(err);
        do
        {
            printf("\nINGRESE UNA LETRA MAYUSCULA:\n");
            scanf("%c", &letra_ingresada);
            fflush(stdin);
        }
        while ((letra_ingresada<65)||(letra_ingresada>90));
        for (int i=0; i<l; i++)
        {
            if (letra_ingresada==lista_palabras[n].wrd[i])
            {
                control[i]=lista_palabras[n].wrd[i];
                letra_correcta=1;
            }
        }
        if (letra_correcta==1)
            printf("LA LETRA INGRESADA ES CORRECTA.\nPRESIONE ENTER PARA CONTINUAR.\n");
        else
        {
            printf("LA LETRA INGRESADA NO ES CORRECTA.\nPRESIONE ENTER PARA CONTINUAR.\n");
            err++;
            puntos=puntos-10;
        }
        if (strcmp(control, lista_palabras[n].wrd)==0)
        {
            acumulados=acumulados+puntos;
            system("cls");
            for (int i=0; i<l; i++)
            {
                printf("%c ", lista_palabras[n].wrd[i]);
            }
            printf("\n");
            victoria(puntos);
            do
            {
                printf("DESEA JUGAR NUEVAMENTE? (S/N)\n");
                scanf("%c", &opcion);
                fflush(stdin);
                if (opcion=='S')
                    return puntos;
                if (opcion=='N')
                    return -1;
            }
            while ((opcion!='S')||(opcion!='N'));
        }
        getchar();
    }
    while (err!=10);
    system("cls");
    printf("LA PALABRA CORRECTA ERA:\n");
    for (int i=0; i<l; i++)
    {
        printf("%c ", lista_palabras[n].wrd[i]);
    }
    printf("\n");
    errores(err);
    getchar();
    return puntos;

}

void cargar_palabras()
{
    int i = 0;
    FILE *archivo_palabras;
    archivo_palabras = fopen("lista.txt", "r");
    if (archivo_palabras==NULL)
    {
        printf("Error al cargar el archivo de palabras. Solucione el error y abra el programa nuevamente. \n");
        exit(1);
    }
    else
    {
        while (!feof(archivo_palabras))
        {
            fscanf(archivo_palabras, "%d ", &lista_palabras[i].indice);
            fflush(stdin);
            fgets(lista_palabras[i].wrd, 16, archivo_palabras);
            fflush(stdin);
            strtok(lista_palabras[i].wrd, "\n");
            i++;
        }
    }
    fclose(archivo_palabras);
}

void cargar_ranking()
{
    int i = 0;
    FILE *archivo_ranking;
    archivo_ranking = fopen("rank.txt", "r");
    if (archivo_ranking==NULL)
    {
        printf("Error al cargar el archivo de puntajes. Solucione el error y abra el programa nuevamente. \n");
        exit(1);
    }
    else
    {
        while (!feof(archivo_ranking))
        {
            fscanf(archivo_ranking, "%d ", &ranking_puntajes[i].max);
            fflush(stdin);
            fgets(ranking_puntajes[i].jugador, 31, archivo_ranking);
            fflush(stdin);
            strtok(ranking_puntajes[i].jugador, "\n");
            i++;
        }
    }
    fclose(archivo_ranking);
}

void menu()
{
    system("cls");
    printf("--------------------------AHORCADO--------------------------\n\n");
    printf("____________________________________________________________\n");
    printf("|/   \\                                                 /  \\|\n");
    printf("|    |             1.- INGRESAR NOMBRE                 |   |\n");
    printf("|  (x_x)           2.- COMENZAR A JUGAR              (x_x) |\n");
    printf("|   /|\\            3.- VER RANKING                    /|\\  |\n");
    printf("|   / \\            4.- SALIR                          / \\  |\n");
    printf("|__________________________________________________________|\n\n");
    printf("-----------------------INSTRUCCIONES:-----------------------\n");
    printf("- DEBE INGRESAR UN NOMBRE ANTES DE COMENZAR A JUGAR.       -\n");
    printf("- UTILICE SOLAMENTE LETRAS MAYUSCULAS.                     -\n");
    printf("- RECUERDE APRETAR ENTER LUEGO DE INGRESAR UNA LETRA.      -\n");
    printf("- LA CANTIDAD MAXIMA DE ERRORES ES DE 10.                  -\n");
    printf("- LAS PALABRAS NO CONTIENEN LA LETRA %c                     -\n", 165);
    printf("- ELIJA UN NRO DEL 1 AL 4 Y PRESIONE ENTER PARA CONTINUAR  -\n");
    printf("------------------------------------------------------------\n");
}

void errores(int cant_error)
{
    switch(cant_error)
    {
        case 0:
            printf("Errores cometidos: 0\n\n");
            printf("     _______\n");
            printf("     |/     \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 1:
            printf("Errores cometidos: 1\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 2:
            printf("Errores cometidos: 2\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |     O\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 3:
            printf("Errores cometidos: 3\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 4:
            printf("Errores cometidos: 4\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |     |\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 5:
            printf("Errores cometidos: 5\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |     |\n");
            printf("     |     |\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 6:
            printf("Errores cometidos: 6\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |    /|\n");
            printf("     |     |\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 7:
            printf("Errores cometidos: 7\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |    /|\\\n");
            printf("     |     |\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 8:
            printf("Errores cometidos: 8\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |    /|\\\n");
            printf("     |     |\n");
            printf("     |    / \n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 9:
            printf("Errores cometidos: 9\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |    (_)\n");
            printf("     |    /|\\\n");
            printf("     |     |\n");
            printf("     |    / \\\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
        case 10:
            printf("Errores cometidos: 10\n\a");
            printf("No ha logrado adivinar la palabra. Mejor suerte la proxima!\n\n");
            printf("     _______\n");
            printf("     |/   \\|\n");
            printf("     |     |\n");
            printf("     |   (x_x)\n");
            printf("     |    /|\\\n");
            printf("     |     |\n");
            printf("     |    / \\\n");
            printf("     |      \n");
            printf("     |      \n");
            printf("_____|______\n");
            break;
    }
}

void victoria(int x)
{
    printf("Felicitaciones, ha adivinado la palabra\n");
    printf("Puntos obtenidos en esta palabra: %d\n\n", x);
    printf("     _______\n");
    printf("     |/   \\|\n");
    printf("     |     |\n");
    printf("     |     O\n");
    printf("     |      \n");
    printf("     |      \n");
    printf("     |  \\(_)/\n");
    printf("     |    | \n");
    printf("     |    | \n");
    printf("_____|___/_\\\n");
}

void ordenar_puntajes(int maximo_logrado)
{
    int band=0;
    for (int i=0; i<10; i++)
    {
        if (strcmp(nombre, ranking_puntajes[i].jugador)==0)
            {
                if (maximo_logrado > ranking_puntajes[i].max)
                {
                    for (int k=i; k<10; k++)
                    {
                        ranking_puntajes[k].max = ranking_puntajes[k+1].max;
                        strcpy(ranking_puntajes[k].jugador, ranking_puntajes[k+1].jugador);
                    }
                    break;
                }
                else
                {
                    band=1;
                    break;
                }
            }
    }
    if (band==0)
    {
        for (int i=0; i<10; i++)
        {
            if (maximo_logrado >= ranking_puntajes[i].max)
            {
                for (int j=9; j>i; j--)
                {
                    ranking_puntajes[j].max = ranking_puntajes[j-1].max;
                    strcpy(ranking_puntajes[j].jugador, ranking_puntajes[j-1].jugador);
                }
                ranking_puntajes[i].max=maximo_logrado;
                strcpy(ranking_puntajes[i].jugador, nombre);
                break;
            }
        }
    }
}

void mostrar_ranking()
{
    system("cls");
    printf("_____________________________________________________________\n");
    printf("|/  \\|                 ________________                |/  \\|\n");
    printf("|    |                |@@@@|     |####|                |    |\n");
    printf("|    |                |@@@@|     |####|                |    |\n");
    printf("|    O                |@@@@|     |####|                O    |\n");
    printf("|                     \\@@@@|     |####/                     |\n");
    printf("|                      \\@@@|     |###/                      |\n");
    printf("|                       \\@@|_____|##/                       |\n");
    printf("|                            (0)                            |\n");
    printf("|                        .-'''''-.                          |\n");
    printf("|                      .'  * * *  '.                        |\n");
    printf("|                     :  *        * :                       |\n");
    printf("|                    : *           * :                      |\n");
    printf("|                    : R A N K I N G :                      |\n");
    printf("|                    : *           * :                      |\n");
    printf("|                     :  *       *  :                       |\n");
    printf("|  \\(_)/               '.  * * *  .'                 \\(_)/  |\n");
    printf("|    |                   '-.....-'                     |    |\n");
    printf("|___/_\\_______________________________________________/_\\___|\n\n");
    for (int i=0; i<10; i++)
    {
        printf("| %d.- %s %d\n", i+1, ranking_puntajes[i].jugador, ranking_puntajes[i].max);
    }
    printf("------------------------------------------------------------\n");
    printf("---------------PRESIONE ENTER PARA CONTINUAR.---------------\n");
    getchar();
}

void escribir_ranking()
{
    FILE *ranking_cierre;
    ranking_cierre=fopen("rank.txt", "w");
    for (int i=0; i<10; i++)
    {
        fprintf(ranking_cierre, "%d %s\n", ranking_puntajes[i].max, ranking_puntajes[i].jugador);
    }
    fclose(ranking_cierre);
}
