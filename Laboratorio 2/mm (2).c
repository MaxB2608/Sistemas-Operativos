//--------------------------------------------------------------------------//
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>       
#include <string.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
//--------------------------------------------------------------------------//
#define SIZE 85//Tamano de la matriz cuadrada, deberia ser 85 al final del codigo
#define MAX_LENGTH 50 //Tamano maximo de una palabra, por si acaso 50
//--------------------------------------------------------------------------//
void direccion(const char *nombreArchivo, char dir[]) {
    if (getcwd(dir, 256) == NULL) {
        perror("getcwd() error");
        free(dir); // Libera la memoria antes de retornar NULL
        return ;
    }

    // Concatena un separador de directorio y el nombre del archivo
    strcat(dir, "/");
    strcat(dir, nombreArchivo);
}
//Funcion para mover jugadores (sujeto a cambios)
void movePlayer(char* filename, char* player, char* direction, int steps, char* previous, char* current) {
    FILE *file = fopen(filename, "r");
    char matrix[SIZE][SIZE][MAX_LENGTH];
    int i, j;
    int player_i = -1, player_j = -1;

    for(i = 0; i < SIZE; i++) {	// Leer la matriz del archivo
        for(j = 0; j < SIZE; j++) {
            fscanf(file, "%s", matrix[i][j]);
            if(strcmp(matrix[i][j], player) == 0) {
                player_i = i;
                player_j = j;
            }
        }
    }
    fclose(file);

    strcpy(matrix[player_i][player_j], current); // Coloca current en la posición del jugador antes de moverse

    if(strcmp(direction, "arriba") == 0 && player_i - steps >= 0) {	    // Realizar el movimiento
        for(i = 1; i <= steps; i++) {
            if(strcmp(matrix[player_i - i][player_j], "/") == 0 || matrix[player_i - i][player_j][0] == 'J' || strcmp(matrix[player_i - i][player_j], "E") == 0 || strcmp(matrix[player_i - i][player_j], "-") == 0) {
                steps = i - 1;
                break;
            }
        }
        strcpy(previous, matrix[player_i - steps][player_j]);
        strcpy(matrix[player_i - steps][player_j], player);
    } else if(strcmp(direction, "abajo") == 0 && player_i + steps < SIZE) {
        for(i = 1; i <= steps; i++) {
            if(strcmp(matrix[player_i + i][player_j], "/") == 0 || matrix[player_i + i][player_j][0] == 'J' || strcmp(matrix[player_i + i][player_j], "E") == 0 || strcmp(matrix[player_i + i][player_j], "-") == 0) {
                steps = i - 1;
                break;
            }
        }
        strcpy(previous, matrix[player_i + steps][player_j]);
        strcpy(matrix[player_i + steps][player_j], player);
    } else if(strcmp(direction, "izquierda") == 0 && player_j - steps >= 0) {
        for(i = 1; i <= steps; i++) {
            if(strcmp(matrix[player_i][player_j - i], "/") == 0 || matrix[player_i][player_j - i][0] == 'J' || strcmp(matrix[player_i][player_j - i], "E") == 0 || strcmp(matrix[player_i][player_j - i], "-") == 0) {
                steps = i - 1;
                break;
            }
        }
        strcpy(previous, matrix[player_i][player_j - steps]);
        strcpy(matrix[player_i][player_j - steps], player);
    } else if(strcmp(direction, "derecha") == 0 && player_j + steps < SIZE) {
        for(i = 1; i <= steps; i++) {
            if(strcmp(matrix[player_i][player_j + i], "/") == 0 || matrix[player_i][player_j + i][0] == 'J' || strcmp(matrix[player_i][player_j + i], "E") == 0 || strcmp(matrix[player_i][player_j + i], "-") == 0) {
                steps = i - 1;
                break;
            }
        }
        strcpy(previous, matrix[player_i][player_j + steps]);
        strcpy(matrix[player_i][player_j + steps], player);
    }

    file = fopen(filename, "w");		 // Sobreescribir el archivo con la nueva matriz
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            fprintf(file, "%s", matrix[i][j]);
            if (j != SIZE - 1) {
                fprintf(file, " ");
            }
        }
        if (i != SIZE - 1) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

//Funcion para verificar si un jugador se encuentra cerca de una e
int jugadore(char* filename, char* player) {
    FILE *file = fopen(filename, "r");
    char matrix[SIZE][SIZE][MAX_LENGTH];
    int i, j;

    for(i = 0; i < SIZE; i++) { //Lee el archivo 
        for(j = 0; j < SIZE; j++) {
            fscanf(file, "%s", matrix[i][j]);
        }
    }

    fclose(file);

    for(i = 0; i < SIZE; i++) { 	    // Verifica al jugador seleccionado
        for(j = 0; j < SIZE; j++) {
            if(strcmp(matrix[i][j], player) == 0) {
                if((i > 0 && strcmp(matrix[i-1][j], "E") == 0) || 
                   (i < SIZE-1 && strcmp(matrix[i+1][j], "E") == 0) ||
                   (j > 0 && strcmp(matrix[i][j-1], "E") == 0) ||
                   (j < SIZE-1 && strcmp(matrix[i][j+1], "E") == 0)) {
                    return 10;
                } else {
                    return 11;
                }
            }
        }
    }

    return -1; //Caso si el jugador no se encuentra en el tablero
}
//Funcion para eliminar la E cerca de un jugador
void elimina_e(char* filename, char* player) {
    FILE *file = fopen(filename, "r");
    char matrix[SIZE][SIZE][MAX_LENGTH];
    int i, j;

    // Leer el archivo y almacenar en la matriz
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            fscanf(file, "%s", matrix[i][j]);
        }
    }
    fclose(file);

    // Buscar el jugador y reemplazar la "E" adyacente por "0"
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            if(strcmp(matrix[i][j], player) == 0) {
                if(i > 0 && strcmp(matrix[i-1][j], "E") == 0) {
                    strcpy(matrix[i-1][j], "0");
                } else if(i < SIZE-1 && strcmp(matrix[i+1][j], "E") == 0) {
                    strcpy(matrix[i+1][j], "0");
                } else if(j > 0 && strcmp(matrix[i][j-1], "E") == 0) {
                    strcpy(matrix[i][j-1], "0");
                } else if(j < SIZE-1 && strcmp(matrix[i][j+1], "E") == 0) {
                    strcpy(matrix[i][j+1], "0");
                }
            }
        }
    }

    // Abrir el archivo en modo de escritura para sobrescribirlo
    file = fopen(filename, "w");

    // Escribir la matriz actualizada en el archivo
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            if (j != SIZE - 1) {
                fprintf(file, "%s ", matrix[i][j]);
            } else {
                fprintf(file, "%s", matrix[i][j]);
            }
        }
        if (i != SIZE - 1) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}
//Funcion para contar la cantidad de veces que se repite una palabra en un archivo
int contar_palabras(char *nombreArchivo, char *palabra) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return -1;
    }

    char buffer[1024];
    int contador = 0;

    while (fscanf(archivo, " %1023s", buffer) == 1) {
        if (strcmp(buffer, palabra) == 0)
            contador++;
    }

    fclose(archivo);

    return contador;
}
//Funcion para teletransportar
void teletransporte(char *filename, char *word) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char board[SIZE][SIZE][MAX_LENGTH];
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            fscanf(file, "%s", board[i][j]);
        }
    }
    fclose(file);

    int wordRow = -1, wordCol = -1;
    int btpRow[SIZE*SIZE], btpCol[SIZE*SIZE], btpCount = 0;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (strcmp(board[i][j], word) == 0) {
                wordRow = i;
                wordCol = j;
            } else if (strcmp(board[i][j], "btp") == 0) {
                btpRow[btpCount] = i;
                btpCol[btpCount] = j;
                btpCount++;
            }
        }
    }

    if (wordRow != -1 && wordCol != -1 && btpCount > 0) {
        int choice = 0;
        if (btpCount > 1) {
            printf("Se encontraron múltiples 'btp'. Por favor selecciona uno:\n");
            for (i = 0; i < btpCount; i++) {
                printf("%d: btp en la fila %d, columna %d\n", i+1, btpRow[i]+1, btpCol[i]+1);
            }
            scanf("%d", &choice);
            choice--; // Ajusta para el índice base cero
        }

        file = fopen(filename, "w");
        if (file == NULL) {
            printf("No se pudo abrir el archivo.\n");
            return;
        }

        strcpy(board[btpRow[choice]][btpCol[choice]], word);
        strcpy(board[wordRow][wordCol], "btp");

        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                fprintf(file, "%s", board[i][j]);
                if (j < SIZE - 1) {
                    fprintf(file, " ");
                }
            }
            if (i < SIZE - 1) {
                fprintf(file, "\n");
            }
        }
        fclose(file);
    } else {
        printf("No se encontró la palabra o 'btp'.\n");
    }
}
//Funcion para asignar los roles
void shuffle(char *arreglo[], int n) {
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            char *t = arreglo[j];
            arreglo[j] = arreglo[i];
            arreglo[i] = t;
        }
    }
}

void asignarRoles(char *jugadores[], char *roles[], char *resultados[][4], int n) {

    shuffle(jugadores, n);
    shuffle(roles, n);

    for (int i = 0; i < n; i++) {
        resultados[i][0] = jugadores[i];
        resultados[i][1] = roles[i];
        resultados[i][2] = "0";
        resultados[i][3] = "0";
    }
}
//Funcion de extension lo cual se complementa con otra funcion , recibe un numero que es el numero del tablero el cual debe retornar
char** extension(int num,char**tablero1,char**tablero2,char**tablero3,char**tablero4,char**tablero5,char**tablero6,char**tablero7,char**tablero8){
    if(num==1){
        return tablero1;
    }else if(num==2){
        return tablero2;
    }else if(num==3){
        return tablero3;
    }else if(num==4){
        return tablero4;
    }else if(num==5){
        return tablero5;
    }else if(num==6){
        return tablero6;
    }else if(num==7){
        return tablero7;
    }else if(num==8){
        return tablero8;
    }else if(num==0){
        printf("No existen mas tableros en esta direccion\n");
        return NULL;
    }
    return 0;
}
//Funcion para leer el archivo y retorna un array con los elementos
char** leerArchivo(const char *nombreArchivo, int *numElementos) {
    FILE *archivo;
    archivo = fopen(nombreArchivo, "r"); // Abrir el archivo en modo lectura

    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    // Definir un tamaño máximo para cada elemento
    char elemento[3];
    
    // Definir un número máximo de elementos
    char **array = (char **)malloc(7225 * sizeof(char *));
    int contador = 0;

    if (array == NULL) {
        perror("Error al asignar memoria para el array");
        return NULL;
    }

    // Leer el archivo y guardar cada elemento en el array
    while (fscanf(archivo, "%s", elemento) == 1) {
        // Reservar memoria para almacenar el elemento
        array[contador] = strdup(elemento);

        if (array[contador] == NULL) {
            perror("Error al asignar memoria para un elemento");
            fclose(archivo);
            for (int i = 0; i < contador; i++) {
                free(array[i]);
            }
            free(array);
            return NULL;
        }
        contador++;
        if (contador >= 7225) {
            break;
        }
    }
    
    fclose(archivo); 

    *numElementos = contador; 

    return array; 
}
//Funcion para leer la lista de mapas , retorna un array con los elementos que seran numeros
int *leer_lista_mapas(char* nombre,int numeros[8]){
    FILE * archivo=fopen(nombre,"r");
    for (int i = 0; i < 8; i++) {
        if (fscanf(archivo, "%d", &numeros[i]) != 1) {
            perror("Error al leer el número");
            break; // Error al leer el número
        }
    }
    fclose(archivo);
    return numeros;
}

//IMPORTANTE: MODIFICAR DIRRECION DEL ARCHIVO INICIO.TXT PARA CADA QUIEN EJECUTE EL CODIGO
//Funcion para crear el mapa inicial el cual será un archivo, ademas en el centro de éste se inicializa el "Inicio.txt"
void crear_mapa_inicial (char *nombre) {
    FILE *archivo;
    archivo = fopen(nombre, "w"); // Abre un archivo en modo escritura

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
    }
    const char *mapa[7225];
    int i = 0;

    while (i < 7225) {
        mapa[i] = "-";
        i++;
    }
    // Escribir el contenido de mapa en el archivo
    for (i = 0; i < 7225; i++) {
        fprintf(archivo, "%s", mapa[i]);
        if (i % 85 == 84) {
            fprintf(archivo, "\n"); // Agrega un salto de línea cada 85 elementos
        } else {
            fprintf(archivo, " "); // Agrega un espacio entre elementos
        }
    }
    fclose(archivo); 
    int numElementos;
    char **inicio = leerArchivo("Inicio.txt", &numElementos);  //MODIFICAR DIRECCION DEL ARCHIVO SEGUN QUIEN LO USE        
    char** map=leerArchivo(nombre,&numElementos);
    int a=0;
    int i_1=0;
    int posicion=3440;
    int posicion_ini=0;
    while(a<5){
         while(i_1<5){
            char* variable=inicio[posicion_ini];
            map[posicion]=variable; 
            posicion_ini++;
            i_1++;
            posicion++;
        }
        i_1=0;
        a++;
        posicion+=80;
    }
    archivo=fopen(nombre,"w");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escritura.\n");
    }
    for (int j = 0; j < 7225; j++) {
        fprintf(archivo, "%s ", map[j]);
        if (j % 85 == 84) {
            fprintf(archivo, "\n"); 
        }
    }
    fclose(archivo);
    printf("El archivo se ha creado correctamente.\n");
}
//Funcion que crea archivo con los 8 posibles tableros es decir del 1 al 8 es su contenido
void crear_lista_mapas(char *nombre){
    FILE * archivo;
    archivo=fopen(nombre,"w");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
    }

    for (int i = 1; i <= 8; i++) {
        fprintf(archivo, "%d",i);
        if(i<8){
            fprintf(archivo," ");
        }
    }
    fclose(archivo);
}
//Funcion que modifica los tableros es decir genera los trofeos y las casillas especiales
void tableros(char** tablero1,char**tablero2,char**tablero3,char**tablero4,char**tablero5,char**tablero6,char**tablero7,char**tablero8){
    int mapas_t[8]={1,2,3,4,5,6,7,8};
    int num_map;
    int flag=1;
    int num_aleatorio;
    int random;
    int posicion_elim;
    int num_rand;
    srand(time(NULL));
    //---------------------------------------- TROFEO PARA J1-----------------------------------
    while(flag==1){
        num_aleatorio=rand()%8;
        if(mapas_t[num_aleatorio]!=0){
            flag=0;
        }
    }
    num_map=mapas_t[num_aleatorio];
    posicion_elim=0;
    for(int c=0;c<8;c++){
        if (mapas_t[c]==num_map){
            posicion_elim=c;
            mapas_t[posicion_elim]=0;
            break;
        }
    }
        //--------------------------------casos segun que mapa fuera------------------------------
    if(num_map==1){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero1[random],"0")==0){
            strcpy(tablero1[random],"1");
            flag=0;
        }
    }    
    }else if(num_map==2){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero2[random],"0")==0){
            strcpy(tablero2[random],"1");
            flag=0;
        }
    }
    }else if(num_map==3){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero3[random],"0")==0){
            strcpy(tablero3[random],"1");
            flag=0;
        }
    }
    }else if(num_map==4){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero4[random],"0")==0){
            strcpy(tablero4[random],"1");
            flag=0;
        }
    }
    }else if(num_map==5){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero5[random],"0")==0){
            strcpy(tablero5[random],"1");
            flag=0;
        }
    }
    }else if(num_map==6){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp (tablero6[random],"0")==0){
            strcpy(tablero6[random],"1");
            flag=0;
        }
    }
    }else if(num_map==7){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero7[random],"0")==0){
            strcpy(tablero7[random],"1");
            flag=0;
        }
    }
    }else if(num_map==8){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero8[random],"0")==0){
            strcpy(tablero8[random],"1");
            flag=0;
        }
    }
    }
    //---------------------------------------- TROFEO PARA J2-----------------------------------
    flag=1;
    while(flag==1){
        num_aleatorio=rand()%8;
        if(mapas_t[num_aleatorio]!=0){
            flag=0;
        }
    }
    num_map=mapas_t[num_aleatorio];
    posicion_elim=0;
    for(int c=0;c<8;c++){
        if (mapas_t[c]==num_map){
            posicion_elim=c;
            mapas_t[posicion_elim]=0;
            break;
        }
    }
        //--------------------------------casos segun que mapa fuera------------------------------
    if(num_map==1){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero1[random],"0")==0){
            strcpy(tablero1[random],"2");
            flag=0;
        }
    }    
    }else if(num_map==2){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero2[random],"0")==0){
            strcpy(tablero2[random],"2");
            flag=0;
        }
    }
    }else if(num_map==3){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero3[random],"0")==0){
            strcpy(tablero3[random],"2");
            flag=0;
        }
    }
    }else if(num_map==4){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero4[random],"0")==0){
            strcpy(tablero4[random],"2");
            flag=0;
        }
    }
    }else if(num_map==5){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero5[random],"0")==0){
            strcpy(tablero5[random],"2");
            flag=0;
        }
    }
    }else if(num_map==6){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp (tablero6[random],"0")==0){
            strcpy(tablero6[random],"2");
            flag=0;
        }
    }
    }else if(num_map==7){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero7[random],"0")==0){
            strcpy(tablero7[random],"2");
            flag=0;
        }
    }
    }else if(num_map==8){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero8[random],"0")==0){
            strcpy(tablero8[random],"2");
            flag=0;
        }
    }
    }
    //---------------------------------------- TROFEO PARA J3-----------------------------------
    flag=1;
    while(flag==1){
        num_aleatorio=rand()%8;
        //printf("Indice 3: %d\n",num_aleatorio);
        if(mapas_t[num_aleatorio]!=0){
            flag=0;
        }
    }
    num_map=mapas_t[num_aleatorio];
    posicion_elim=0;
    for(int c=0;c<8;c++){
        if (mapas_t[c]==num_map){
            posicion_elim=c;
            mapas_t[posicion_elim]=0;
            break;
        }
    }
        //--------------------------------casos segun que mapa fuera------------------------------
    if(num_map==1){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero1[random],"0")==0){
            strcpy(tablero1[random],"3");
            flag=0;
        }
    }    
    }else if(num_map==2){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero2[random],"0")==0){
            strcpy(tablero2[random],"3");
            flag=0;
        }
    }
    }else if(num_map==3){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero3[random],"0")==0){
            strcpy(tablero3[random],"3");
            flag=0;
        }
    }
    }else if(num_map==4){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero4[random],"0")==0){
            strcpy(tablero4[random],"3");
            flag=0;
        }
    }
    }else if(num_map==5){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero5[random],"0")==0){
            strcpy(tablero5[random],"3");
            flag=0;
        }
    }
    }else if(num_map==6){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp (tablero6[random],"0")==0){
            strcpy(tablero6[random],"3");
            flag=0;
        }
    }
    }else if(num_map==7){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero7[random],"0")==0){
            strcpy(tablero7[random],"3");
            flag=0;
        }
    }
    }else if(num_map==8){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero8[random],"0")==0){
            strcpy(tablero8[random],"3");
            flag=0;
        }
    }
    }
    //---------------------------------------- TROFEO PARA J4-----------------------------------
    flag=1;
    while(flag==1){
        num_aleatorio=rand()%8;
        if(mapas_t[num_aleatorio]!=0){
                flag=0;
        }
    }
    num_map=mapas_t[num_aleatorio];
     posicion_elim=0;
    for(int c=0;c<8;c++){
        if (mapas_t[c]==num_map){
            posicion_elim=c;
            mapas_t[posicion_elim]=0;
            break;
        }
    }
        //--------------------------------casos segun que mapa fuera------------------------------
    if(num_map==1){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero1[random],"0")==0){
            strcpy(tablero1[random],"4");
            flag=0;
        }
    }    
    }else if(num_map==2){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero2[random],"0")==0){
            strcpy(tablero2[random],"4");
            flag=0;
        }
    }
    }else if(num_map==3){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero3[random],"0")==0){
            strcpy(tablero4[random],"4");
            flag=0;
        }
    }
    }else if(num_map==4){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero4[random],"0")==0){
            strcpy(tablero4[random],"4");
            flag=0;
        }
    }
    }else if(num_map==5){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp(tablero5[random],"0")==0){
            strcpy(tablero5[random],"4");
            flag=0;
        }
    }
    }else if(num_map==6){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp (tablero6[random],"0")==0){
            strcpy(tablero6[random],"4");
            flag=0;
        }
    }
    }else if(num_map==7){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero7[random],"0")==0){
            strcpy(tablero7[random],"4");
            flag=0;
        }
    }
    }else if(num_map==8){
    flag=1;
    while(flag==1){
        random=rand()%25;
        if(strcmp( tablero8[random],"0")==0){
            strcpy(tablero8[random],"4");
            flag=0;
        }
    }
    }
    //----------------------------QUEDAN 4 TABLEROS RESTANTES QUE PUEDEN TENER 1 DE LOS 4 TIPO DE CASILLAS Bc, Bt, Bn, Btp
    
    //---------------------------- 5to caso de tablero------------------------------------
    srand(time(NULL));
    int ciclo=0;
    while(ciclo<4){
        num_rand=rand()%4;
        //0   -> Bc
        //1   -> Bt
        //2   -> Bn
        //3   -> Btp
        flag=1;
        while(flag==1){
            num_aleatorio=rand()%8;
            //printf("Indice 5: %d\n",num_aleatorio);
            if(mapas_t[num_aleatorio]!=0){
                    flag=0;
            }
        }
        num_map=mapas_t[num_aleatorio];
        posicion_elim=0;
        for(int c=0;c<8;c++){
            if (mapas_t[c]==num_map){
                posicion_elim=c;
                mapas_t[posicion_elim]=0;
                break;
            }
        }


        if(num_map==1){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero1[random],"0")==0){
                    strcpy(tablero1[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero1[random],"0")==0){
                    strcpy(tablero1[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero1[random],"0")==0){
                    strcpy(tablero1[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero1[random],"0")==0){
                    strcpy(tablero1[random],"btp");
                    flag=0;
                }
            }
        }    
        }else if(num_map==2){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero2[random],"0")==0){
                    strcpy(tablero2[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero2[random],"0")==0){
                    strcpy(tablero2[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero2[random],"0")==0){
                    strcpy(tablero2[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero2[random],"0")==0){
                    strcpy(tablero2[random],"btp");
                    flag=0;
                }
            }
        }   
        }else if(num_map==3){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero3[random],"0")==0){
                    strcpy(tablero3[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero3[random],"0")==0){
                    strcpy(tablero3[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero3[random],"0")==0){
                    strcpy(tablero3[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero3[random],"0")==0){
                    strcpy(tablero3[random],"btp");
                    flag=0;
                }
            }
        }  
        }else if(num_map==4){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero4[random],"0")==0){
                    strcpy(tablero4[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero4[random],"0")==0){
                    strcpy(tablero4[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero4[random],"0")==0){
                    strcpy(tablero4[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero4[random],"0")==0){
                    strcpy(tablero4[random],"btp");
                    flag=0;
                }
            }
        }  
        }else if(num_map==5){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero5[random],"0")==0){
                    strcpy(tablero5[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero5[random],"0")==0){
                    strcpy(tablero5[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero5[random],"0")==0){
                    strcpy(tablero5[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero5[random],"0")==0){
                    strcpy(tablero5[random],"btp");
                    flag=0;
                }
            }
        } 
        }else if(num_map==6){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero6[random],"0")==0){
                    strcpy(tablero6[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero6[random],"0")==0){
                    strcpy(tablero6[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero6[random],"0")==0){
                    strcpy(tablero6[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero6[random],"0")==0){
                    strcpy(tablero6[random],"btp");
                    flag=0;
                }
            }
        }   
        }else if(num_map==7){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero7[random],"0")==0){
                    strcpy(tablero7[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero7[random],"0")==0){
                    strcpy(tablero7[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero7[random],"0")==0){
                    strcpy(tablero7[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero7[random],"0")==0){
                    strcpy(tablero7[random],"btp");
                    flag=0;
                }
            }
        }  
        }else if(num_map==8){
        flag=1;
        while(flag==1){
            random=rand()%25;
            if(num_rand==0){
                if(strcmp(tablero8[random],"0")==0){
                    strcpy(tablero8[random],"bc");
                    flag=0;
                }
            }else if(num_rand==1){
                if(strcmp(tablero8[random],"0")==0){
                    strcpy(tablero8[random],"bt");
                    flag=0;
                }
            }else if(num_rand==2){
                if(strcmp(tablero8[random],"0")==0){
                    strcpy(tablero8[random],"bn");
                    flag=0;
                }
            }else if(num_rand==3){
                if(strcmp(tablero8[random],"0")==0){
                    strcpy(tablero8[random],"btp");
                    flag=0;
                }
            }
        } 
        }
        ciclo++;
        }

}
//Imprime el mapa
void imprimir_mapa(char *nombre) {
    int numElementos;
    char **mapa=leerArchivo(nombre,&numElementos);
    int longitud=7225;
    int a = 0;
    int aux = 1;

    while (a < longitud) {
        if (aux % 85 == 0) {
            printf("%s  \n", mapa[a]);
            aux++;
            a++;
        } else {
            printf("%s ", mapa[a]);
            a++;
            aux++;
        }
    }
}
//Actualiza el mapa el cual usa el archivo que se creo antes el cual es mapa.txt
void actualizar_mapa_txt(char *nombre,char** map){
    FILE *archivo;
    archivo=fopen(nombre,"w");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escritura.\n");
    }
    for (int j = 0; j < 7225; j++) {
        fprintf(archivo, "%s ", map[j]);
        if (j % 85 == 84) {
            fprintf(archivo, "\n"); 
        }
    }

    fclose(archivo); 
}
//Actualiza el archivo que contenia los tableros es decir los numeros del 1 al 8 y convierte en 0 aquel que ya se usó
void actualizar_lista_mapas(char* nombre,int numero_reem){
    FILE *archivo = fopen(nombre, "r+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return; 
    }

    int numeros[8];
    int i = 0;
    while (i < 8 && fscanf(archivo, "%d", &numeros[i]) == 1) {
        i++;
    }
    rewind(archivo);
    for (int j = 0; j < i; j++) {
        if (numeros[j] == numero_reem) {
            numeros[j] = 0;
        }
    }
    for (int j = 0; j < i; j++) {
        fprintf(archivo, "%d", numeros[j]);
        if (j < i - 1) {
            fprintf(archivo, " "); 
        }
    }
    fclose(archivo);
}
//Permite hacer random el sorteo de los tableros al momento de expandir segun la direccion 
int map(int movimiento,int arreglo[]){
    int num=0;
    int flag=1;
    int num_random;
    int emergencia=0;
    srand(time(NULL));
    if(movimiento==1){                      //ARRIBA
        int mapas_arriba[8];
        leer_lista_mapas("mapas_arriba.txt",mapas_arriba);
        while (flag==1){
            num_random=rand()%5;
            int mapa=mapas_arriba[num_random];
            for(int i=0;i<8;i++){
                if(arreglo[i]==mapa){
                    num=mapa;
                    flag=0;
                    break;
                }
            }emergencia++;
            if(emergencia==20){
                flag=0;
            }
        }
    }else if(movimiento==2){                //ABAJO 
        int mapas_abajo[8];
        leer_lista_mapas("mapas_abajo.txt",mapas_abajo);
        while (flag==1){
            num_random=rand()%5;
            int mapa=mapas_abajo[num_random];
            for(int i=0;i<8;i++){
                if(arreglo[i]==mapa){
                    num=mapa;
                    flag=0;
                    break;
                }
            }emergencia++;
            if(emergencia==20){
                flag=0;
            }
        }
    }else if(movimiento==3){                //IZQUIERDA
        int mapas_izquierda[8];
        leer_lista_mapas("mapas_izquierda.txt",mapas_izquierda);
        while (flag==1){
            num_random=rand()%5;
            int mapa=mapas_izquierda[num_random];
            for(int i=0;i<8;i++){
                if(arreglo[i]==mapa){
                    num=mapa;
                    flag=0;
                    break;
                }
            }emergencia++;
            if(emergencia==20){
                flag=0;
            }
        }
    }else if(movimiento==4){                //DERECHA
        int mapas_derecha[8];
        leer_lista_mapas("mapas_derecha.txt",mapas_derecha);
        while (flag==1){
            num_random=rand()%5;
            int mapa=mapas_derecha[num_random];
            for(int i=0;i<8;i++){
                if(arreglo[i]==mapa){
                    num=mapa;
                    flag=0;
                    break;
                }
            }emergencia++;
            if(emergencia==20){
                flag=0;
            }
        }
    }
    return num;
}
//Funcion encargada de hacer la expansion del mapa.
void ampliar(int j,char* mapa[],int carta_B,int mapas[],char** tablero1,char**tablero2,char**tablero3,char**tablero4,char**tablero5,char**tablero6,char**tablero7,char**tablero8, char direction[]){
    if(carta_B==1 ){
        if(strcmp(direction,"arriba")==0){         //ARRIBA
            int aux=map(1,mapas);
            actualizar_lista_mapas("lista_mapas.txt",aux);
            char** mapa_extension=extension(aux,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);  //cambiar aux por el mapa que se quiera
            int i_2=0;
            int i_3=0;
            int coord=j-427;
            int coord_ini=0;
            if(mapa_extension!=NULL){
                while(i_2<5){
                    while(i_3<5){
                        char* variable_2=mapa_extension[coord_ini];
                        mapa[coord]=variable_2;
                        coord_ini++;
                        coord++;
                        i_3++;
                    }
                    i_2++;
                    i_3=0;
                    coord+=80;
                }
            }
        }else if(strcmp(direction,"abajo")==0){      //ABAJO
            int aux=map(2,mapas);
            actualizar_lista_mapas("lista_mapas.txt",aux);
            char** mapa_extension=extension(aux,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);//cambiar aux por el mapa que se quiera
            int i_2=0;
            int i_3=0;
            int coord=j+83;
            int coord_ini=0;
            if(mapa_extension!=NULL){
                while(i_2<5){
                    while(i_3<5){
                        char* variable_2=mapa_extension[coord_ini];
                        mapa[coord]=variable_2;
                        coord_ini++;
                        coord++;
                        i_3++;
                    }
                    i_2++;
                    i_3=0;
                    coord+=80;
                }
            }
        }else if(strcmp(direction,"izquierda")==0){      //IZQUIERDA
            int aux=map(3,mapas);
            actualizar_lista_mapas("lista_mapas.txt",aux);
            char** mapa_extension=extension(aux,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8); //cambiar aux por el mapa que se quiera
            int i_2=0;
            int i_3=0;
            int coord=j-175;
            int coord_ini=0;
            if(mapa_extension!=NULL){
                while(i_2<5){
                    while(i_3<5){
                        char* variable_2=mapa_extension[coord_ini];
                        mapa[coord]=variable_2;
                        coord_ini++;
                        coord++;
                        i_3++;
                    }
                    i_2++;
                    i_3=0;
                    coord+=80;
                }
            }
        }else if(strcmp(direction,"derecha")==0){       //DERECHA
            int aux=map(4,mapas);
            actualizar_lista_mapas("lista_mapas.txt",aux);
            char** mapa_extension=extension(aux,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);  //cambiar aux por el mapa que se quiera
            int i_2=0;
            int i_3=0;
            int coord=j-169;
            int coord_ini=0;
            if (mapa_extension!=NULL){
                while(i_2<5){
                    while(i_3<5){
                        char* variable_2=mapa_extension[coord_ini];
                        mapa[coord]=variable_2;
                        coord_ini++;
                        coord++;
                        i_3++;
                    }
                    i_2++;
                    i_3=0;
                    coord+=80;
                }
            }
        }
        actualizar_mapa_txt("mapa.txt",mapa);
    }
}
//Struct de los jugadores
typedef struct {
	char nombre[MAX_LENGTH];
	char rol[MAX_LENGTH];
	char paso[MAX_LENGTH];
	char tesoro[MAX_LENGTH];
} jugador;
//Verifica si los jugadores estàn en el inicio y si tienen los tesoros
int verficacion_final(int j1, int j2, int j3,int j4, char j1_[], char j2_[], char j3_[], char j4_[]){
    int coordenadas[25]={3440,3441,3442,3443,3444,3525,3526,3527,3528,3529,3610,3611,3612,3613,3614,3695,3696,3697,3698,3699,3780,3781,3782,3783,3784};
    int flag_j1 = 0;
    int flag_j2 = 0;
    int flag_j3 = 0;
    int flag_j4 = 0;
    for (int i=0;i<25;i++){
        if(coordenadas[i]==j1){
            flag_j1=1;
        }if(coordenadas[i]==j2){
            flag_j2=1;
        }if(coordenadas[i]==j3){
            flag_j3=1;
        }if(coordenadas[i]==j4){
            flag_j4=1;
        }
    }
    
    if(flag_j1 == 1 && flag_j2 == 1 && flag_j3 == 1 && flag_j4 == 1 && strcmp(j1_,"1")==0 && strcmp(j2_,"2")==0 && strcmp(j3_,"3")==0 && strcmp(j4_,"4")==0){
        return 1;           //Si retorna 1 es porque el juego debe finalizar y ganar
    }else{
        return 0;           
    }
}

//Funcion donde se mandan las instrucciones desde J1
void usuario(int *fd, jugador j) {
    printf("\nLa carta del %s es: %s\n",j.nombre, j.rol);
    char current[MAX_LENGTH];
    strcpy(current,j.paso);
	int cerca = jugadore("mapa.txt", j.nombre);
	int decision;
	printf("\n%s: ¿Que quieres que haga?\n1) Moverse\n", j.nombre);
	if(strcmp(j.rol, "busca")==0 && strcmp(current,"B")==0){			//Printea en caso de que J sea buscador
		printf("2) Usar:\n/////\n/   /\n/ B /\n/   /\n/////\n");
	}
	else if(strcmp(j.rol, "puerta")==0 && cerca == 10){ //Printea en caso de que J sea portero
		printf("2) Usar:\n/////\n/   /\n/ E /\n/   /\n/////\n");
	}			
	scanf("%d", &decision);

	if (decision == 1){ 			//Elige moverse
		int steps; 					//Cantidad de pasos a realizar
		char direction[MAX_LENGTH]; //Direccion del movimiento (izquierda|derecha|arriba|abajo)
		printf("En que direccion? ");
		scanf("%s", direction);			//Le pide al usuario los parametros necesarios
		printf("Cuantos pasos? ");
		scanf("%d", &steps);
		//-----------------------------//
		close(fd[0]);
		write(fd[1], &decision, sizeof(decision));
		write(fd[1], &steps, sizeof(steps));		
		write(fd[1], direction, sizeof(direction));			
		close(fd[1]);

	}else{ //Elige utilizar la carta
		if(strcmp(j.rol, "busca") == 0){
			int steps = 1; 				//Pasos no se usa pero es para no estropear el formato
			char direction[MAX_LENGTH]; 
			printf("En que direccion? "); //Donde se extiende el mapa (por lo que dijo Basty)
			scanf("%s", direction);	
			//-----------------------------//
			close(fd[0]);
			write(fd[1], &decision, sizeof(decision));
			write(fd[1], &steps, sizeof(steps));		
			write(fd[1], direction, sizeof(direction));			
			close(fd[1]);
		}
		else{
			int steps = 1; 						//
			char direction[MAX_LENGTH] = "xd\n"; 
			//-----------------------------//
			close(fd[0]);
			write(fd[1], &decision, sizeof(decision));
			write(fd[1], &steps, sizeof(steps));		
			write(fd[1], direction, sizeof(direction));			
			close(fd[1]);
		}
	}
}
//Funcion donde papa actualiza los structs
void papa(int *fd, jugador* j){
	close(fd[1]);
	int decision;
	char debajo[MAX_LENGTH];					//Recibe lo que mando el hijo
	read(fd[0], &decision, sizeof(decision));
	read(fd[0], debajo, sizeof(debajo));
	if (decision == 1){					
		strcpy(j->paso,debajo); //Actualiza el struct al realizar un movimiento
	}
	close(fd[0]);
}
//Funcion donde bot ejecuta su parte
void bot(int *fd, int *fd2, jugador j,int coord,char leer_map[],int mapas[],char** tablero1,char** tablero2,char** tablero3,char** tablero4,char** tablero5,char** tablero6,char** tablero7,char** tablero8){
	close(fd[1]);
	char* texto = "mapa.txt";
	char debajo[MAX_LENGTH];		
	char current [MAX_LENGTH]; //Elemento actualmente debajo del J

	int decision;
	int steps; 
	char direction[MAX_LENGTH];
	read(fd[0], &decision, sizeof(decision));
	read(fd[0], &steps, sizeof(steps));			//Lee instrucciones
	read(fd[0], direction, sizeof(direction));
	if (decision == 1){					//Ejecuta el movimiento
		strcpy(current, j.paso);
		movePlayer(texto,j.nombre, direction, steps, debajo, current);				
	}else{
		if(strcmp(j.rol,"busca") == 0){
            if(strcmp(j.nombre,"J2")==0){
                ampliar(coord,leer_map,1,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8,direction);
            }else if(strcmp(j.nombre,"J3")==0){
                ampliar(coord,leer_map,1,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8,direction);
            }else if(strcmp(j.nombre,"J4")==0){
                ampliar(coord,leer_map,1,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8,direction);
            }
			printf("MAPA AMPLIADO\n");	//Se activa buscar si es que lo tiene
		}else{
			elimina_e("mapa.txt", j.nombre); //Se activa puerta si es que lo tiene
			printf("\nPUERTA ROTA\n");	
		}
	}
	close(fd[0]); 
	close(fd2[0]); 
	write(fd2[1], &decision, sizeof(decision));	
	write(fd2[1], debajo, sizeof(debajo));		//Manda informacion al papa
	close(fd2[1]); 							  			
}
//----------------------- MAIN -------------------------------------------//
int main() {
	//----------- variables para iniciar el juego -------------------------------------------//
	srand(time(0));
    char *jugadores[] = {"J1", "J2", "J3", "J4"};
    char *roles[] = {"puerta", "puerta", "busca", "busca"};		//Aqui se hace el sorteo de roles
    char *tablaj[4][4];
    asignarRoles(jugadores, roles, tablaj, 4);

	jugador j1;
	jugador j2;
	jugador j3;		//Se generan los structs
	jugador j4;

	for (int x = 0; x < 4; x++){			//Se asigna de manera correspondiente los datos
		if(strcmp(tablaj[x][0], "J1") == 0){
			strcpy(j1.nombre, tablaj[x][0]);
			strcpy(j1.rol, tablaj[x][1]);
			strcpy(j1.paso, tablaj[x][2]);
			strcpy(j1.tesoro, tablaj[x][3]);
		}
		else if(strcmp(tablaj[x][0], "J2") == 0){
			strcpy(j2.nombre, tablaj[x][0]);
			strcpy(j2.rol, tablaj[x][1]);
			strcpy(j2.paso, tablaj[x][2]);
			strcpy(j2.tesoro, tablaj[x][3]);
		}
		else if(strcmp(tablaj[x][0], "J3") == 0){
			strcpy(j3.nombre, tablaj[x][0]);
			strcpy(j3.rol, tablaj[x][1]);
			strcpy(j3.paso, tablaj[x][2]);
			strcpy(j3.tesoro, tablaj[x][3]);
		}
		else if(strcmp(tablaj[x][0], "J4") == 0){
			strcpy(j4.nombre, tablaj[x][0]);
			strcpy(j4.rol, tablaj[x][1]);
			strcpy(j4.paso, tablaj[x][2]);
			strcpy(j4.tesoro, tablaj[x][3]);
		}						
	}

    //-------------------------------------------------------------------------//
    //IMPORTANTE: MODIFICAR DIRRECION DEL ARCHIVO INICIO.TXT PARA CADA QUIEN EJECUTE EL CODIGO (ej direccion utilizada en wsl: "/home/basty/code/Helloword/C/tablero4.txt")
    int numElementos;
    char **tablero1 = leerArchivo("tablero1.txt", &numElementos);
    char **tablero2 = leerArchivo("tablero2.txt", &numElementos);
    char **tablero3 = leerArchivo("tablero3.txt", &numElementos);
    char **tablero4 = leerArchivo("tablero4.txt", &numElementos);
    char **tablero5 = leerArchivo("tablero5.txt", &numElementos);
    char **tablero6 = leerArchivo("tablero6.txt", &numElementos);
    char **tablero7 = leerArchivo("tablero7.txt", &numElementos);
    char **tablero8 = leerArchivo("tablero8.txt", &numElementos);
    tableros(tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);
//------------------------------------------------------------------------------
    FILE* archivo_1;
    archivo_1=fopen("mapas_arriba.txt","w");
    FILE* archivo_2;
    archivo_2=fopen("mapas_abajo.txt","w");
    FILE* archivo_3;
    archivo_3=fopen("mapas_izquierda.txt","w");
    FILE* archivo_4;
    archivo_4=fopen("mapas_derecha.txt","w");

    if (strcmp(tablero1[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",1);
        fprintf(archivo_1," ");
    }if(strcmp(tablero1[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",1);
        fprintf(archivo_2," ");
    }if(strcmp(tablero1[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",1);
        fprintf(archivo_3," ");
    }if(strcmp(tablero1[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",1);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero2[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",2);
        fprintf(archivo_1," ");
    }if(strcmp(tablero2[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",2);
        fprintf(archivo_2," ");
    }if(strcmp(tablero2[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",2);
        fprintf(archivo_3," ");
    }if(strcmp(tablero2[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",2);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero3[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",3);
        fprintf(archivo_1," ");
    }if(strcmp(tablero3[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",3);
        fprintf(archivo_2," ");
    }if(strcmp(tablero3[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",3);
        fprintf(archivo_3," ");
    }if(strcmp(tablero3[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",3);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero4[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",4);
        fprintf(archivo_1," ");
    }if(strcmp(tablero4[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",4);
        fprintf(archivo_2," ");
    }if(strcmp(tablero4[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",4);
        fprintf(archivo_3," ");
    }if(strcmp(tablero4[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",4);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero5[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",5);
        fprintf(archivo_1," ");
    }if(strcmp(tablero5[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",5);
        fprintf(archivo_2," ");
    }if(strcmp(tablero5[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",5);
        fprintf(archivo_3," ");
    }if(strcmp(tablero5[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",5);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero6[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",6);
        fprintf(archivo_1," ");
    }if(strcmp(tablero6[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",6);
        fprintf(archivo_2," ");
    }if(strcmp(tablero6[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",6);
        fprintf(archivo_3," ");
    }if(strcmp(tablero6[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",6);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero7[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",7);
        fprintf(archivo_1," ");
    }if(strcmp(tablero7[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",7);
        fprintf(archivo_2," ");
    }if(strcmp(tablero7[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",7);
        fprintf(archivo_3," ");
    }if(strcmp(tablero7[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",7);
        fprintf(archivo_4," ");
    }

    if (strcmp(tablero8[22],"B")==0) {     //ARRIBA
        fprintf(archivo_1,"%d",8);
        fprintf(archivo_1," ");
    }if(strcmp(tablero8[2],"B")==0) {   //ABAJO
        fprintf(archivo_2,"%d",8);
        fprintf(archivo_2," ");
    }if(strcmp(tablero8[14],"B")==0) {   // IZQUIERDA
        fprintf(archivo_3,"%d",8);
        fprintf(archivo_3," ");
    }if(strcmp(tablero8[10],"B")==0) {   //DERECHA
        fprintf(archivo_4,"%d",8);
        fprintf(archivo_4," ");
    }


    fclose(archivo_1);
    fclose(archivo_2);
    fclose(archivo_3);
    fclose(archivo_4);

//--------------------------------------------------------------------------------------------
    char mapa[]="mapa.txt"; //Se crea napa.txt, archivo que representa el tablero
    char lista_mapas[]="lista_mapas.txt";
    crear_mapa_inicial(mapa);
    crear_lista_mapas(lista_mapas);
    //----------------------------------------------------------------------------------------------------------------------------------
    char ** leer_map=leerArchivo(mapa,&numElementos);
    int mapas[8];
    leer_lista_mapas(lista_mapas,mapas);
    //
    //----------------------------------------------------------------------------------------------------------------------------------
    //Buscamos las posiciones de J1,J2,J3,J4
    int b=0;
    int j1_coord;
    int j2_coord;
    int j3_coord;
    int j4_coord;
    while(b<7225){
        if(strcmp(leer_map[b],"J1")==0){
            j1_coord=b;
        }else if(strcmp(leer_map[b],"J2")==0){
            j2_coord=b;
        }else if (strcmp(leer_map[b],"J3")==0){
            j3_coord=b;
        }else if(strcmp(leer_map[b],"J4")==0){
            j4_coord=b;
        }
        b++;
    }
    //-------------------------------------------------------------------------//
	printf("BIENVENID@ A MAGIC MAZE\n\nPor favor, lea el README antes de empezar\nLas instrucciones se encuentran ahi :)\n");
	int turnos = 15;
	while (turnos > 0 ){
        imprimir_mapa("mapa.txt");
		int camaras = contar_palabras("mapa.txt", "bc");
		int portales = contar_palabras("mapa.txt", "btp");
		printf("\nTurno actual: %d\n\n", turnos);
		//----------- Genera pipes -------------------------------------------//
		int fdu1[2], fdu2[2], fdu3[2], fdup[2], fd1p[2], fd2p[2], fd3p[2];

		pipe(fdup); //Desde J1 a padre
		pipe(fdu1); //Desde J1 a J2
		pipe(fdu2); //Desde J1 a J3
		pipe(fdu3); //Desde J1 a J4
		pipe(fd1p); //Desde J2 a padre
		pipe(fd2p); //Desde J3 a padre
		pipe(fd3p); //Desde J4 a padre
		//-------------- Genera pids-----------------------------------------//    
		pid_t pid_usuario, pid_bot1, pid_bot2, pid_bot3;
		//-------------- Genera al J1 (se llama usuario porque no es bot) ----------//    
		pid_usuario = fork();
		//-------------- Hace lo que debe hacer -----------------------------------------//    		
		if (pid_usuario == 0){
            char current[MAX_LENGTH];
            strcpy(current,j1.paso);
            printf("\nJ1 tiene la carta: %s\n",j1.rol);
			int cerca = jugadore("mapa.txt", "J1");
			int decision;
			printf("\nJ1: ¿Que quieres hacer?\n1) Moverse\n");
			if(strcmp(j1.rol, "busca")==0 && strcmp(current, "B") == 0){			//Printea en caso de que J1 sea busca
				printf("2) Usar:\n/////\n/   /\n/ B /\n/   /\n/////\n");
			}
			else if(strcmp(j1.rol, "puerta")==0 && cerca == 10){ //Printea en caso de que J1 sea puerta
				printf("2) Usar:\n/////\n/   /\n/ E /\n/   /\n/////\n");
			}			
			scanf("%d", &decision);

			if (decision == 1){ 			//Elige moverse
				char debajo[MAX_LENGTH];	//Lo que esta debajo de J1 despues de moverse
				int steps; 					//Cantidad de pasos a realizar
				char direction[MAX_LENGTH]; //Direccion del movimiento (izquierda|derecha|arriba|abajo)
				char current [MAX_LENGTH]; //Elemento actualmente debajo de J1
				strcpy(current, j1.paso);
				char* texto = "mapa.txt"; 	// Texto en el cual se va a mover
				printf("En que direccion? ");
				scanf("%s", direction);			//Le pide al usuario los parametros necesarios
				printf("Cuantos pasos? ");
				scanf("%d", &steps);
				//-----------------------------//
				movePlayer(texto,j1.nombre, direction, steps, debajo, current); //Realiza el movimiento
				//-----------------------------//
				close(fdup[0]);
				write(fdup[1], &decision, sizeof(decision));	//Le manda a papa los datos
				write(fdup[1], debajo, sizeof(debajo));			//para actualizar el struct
				close(fdup[1]);

			}else{ //Elige utilizar la carta
				if(strcmp(j1.rol, "busca") == 0){
					printf("ACTIVASTE LA CARTA BUSCAR\n");
                    leer_map=leerArchivo(mapa,&numElementos);
                    leer_lista_mapas(lista_mapas,mapas);
                    b=0;
                    while(b<7225){
                    if(strcmp(leer_map[b],"J1")==0){
                        j1_coord=b;
				    }
                    b++;
                    }
                    char donde[MAX_LENGTH];
            		printf("En que direccion? "); //Donde se extiende el mapa (por lo que dijo Basty)
			        scanf("%s", donde);
                    ampliar(j1_coord,leer_map,1,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8,donde);
				}else{
					elimina_e("mapa.txt", j1.nombre); //Se activa puerta si es que lo tiene
				}
				char debajo[MAX_LENGTH] = "xd\n";
				close(fdup[0]); 
				write(fdup[1], &decision, sizeof(decision));	//Le manda a papa datos inservibles
				write(fdup[1], debajo, sizeof(debajo));		 	//Se hace para mantener el formato
				close(fdup[1]); 							 	//de llegada para el papa 
			}
			usuario(fdu1, j2); //Le manda al J2 que hacer
			usuario(fdu2, j3); //Le manda al J3 que hacer
			usuario(fdu3, j4); //Le manda al J4 que hacer
			return 0;
			
		}
		waitpid(pid_usuario, NULL, 0); //El papa espera a que finalice el hijo
		//-------------- Papa recibe datos de J1 -----------------------------------------//
		papa(fdup, &j1);
		//---------- Se ejecutan casillas especiales en caso de que J1 este sobre una -------//
		if (strcmp(j1.paso, "1") == 0){
			strcpy(j1.paso, "0");
            strcpy(j1.tesoro, "1");
		}
		if (strcmp(j1.paso, "bc") == 0){
			strcpy(j1.paso, "0");
		}
		else if(camaras < 2 && strcmp(j1.paso, "bt") == 0){
			turnos = turnos + 5;
			printf("\nEl J1 consigue 5 turnos extra para todos:D\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j1.paso, "0");			
		}
		else if(strcmp(j1.paso, "bn") == 0){
			turnos = turnos - 3;
			printf("\nJ1 acaba de perder 3 turnos D:\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j1.paso, "0");			
		}
		else if(portales >= 2 && strcmp(j1.paso, "btp") == 0){
			printf("Para J1:\n");
			teletransporte("mapa.txt", j1.nombre);
		}
		printf("\nEl jugador %s esta encima de %s y el tesoro es %s\n",j1.nombre, j1.paso, j1.tesoro);
		//-------------- Genera al J2 -----------------------------------------//
		pid_bot1 = fork();
		//-------------- Hace lo que debe hacer -----------------------------------------//
		if (pid_bot1 == 0) {
            leer_map=leerArchivo(mapa,&numElementos);
            leer_lista_mapas(lista_mapas,mapas);
            b=0;
            while(b<7225){
                if(strcmp(leer_map[b],"J2")==0){
                    j2_coord=b;
                    //printf("Jugador 1: %d\n",j1_coord);
                }
                b++;
            }
			bot(fdu1, fd1p, j2,j2_coord,leer_map,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);
			return 0;
		}
		waitpid(pid_bot1, NULL, 0); //El papa espera a que finalice el hijo
		//-------------- Papa recibe datos de J2 -----------------------------------------//
		papa(fd1p, &j2);
		//---------- Se ejecutan casillas especiales en caso de que J2 este sobre una -------//
		if (strcmp(j2.paso, "2") == 0){
			strcpy(j2.paso, "0");
            strcpy(j2.tesoro, "2");
		}		
        if (strcmp(j2.paso, "bc") == 0){
			strcpy(j2.paso, "0");
		}
		else if(camaras < 2 && strcmp(j2.paso, "bt") == 0){
			turnos = turnos + 5;
			printf("\nEl J2 consigue 5 turnos extra para todos:D\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j2.paso, "0");			
		}
		else if(strcmp(j2.paso, "bn") == 0){
			turnos = turnos - 3;
			printf("\nJ2 acaba de perder 3 turnos D:\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j2.paso, "0");			
		}
		else if(portales >= 2 && strcmp(j2.paso, "btp") == 0){
			printf("Para J2:\n");			
			teletransporte("mapa.txt", j2.nombre);
		}
		printf("\nEl jugador %s esta encima de %s y el tesoro es %s\n",j2.nombre, j2.paso, j2.tesoro);
		//-------------- Genera al J3 -----------------------------------------//
		pid_bot2 = fork();
		//-------------- Hace lo que debe hacer -----------------------------------------//
		if (pid_bot2 == 0) {
            leer_map=leerArchivo(mapa,&numElementos);
            leer_lista_mapas(lista_mapas,mapas);
            b=0;
            while(b<7225){
                if(strcmp(leer_map[b],"J3")==0){
                    j3_coord=b;
                    //printf("Jugador 1: %d\n",j1_coord);
                }
                b++;
            }
			bot(fdu2, fd2p, j3,j3_coord,leer_map,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);
			return 0;
		}
		waitpid(pid_bot2, NULL, 0); //El papa espera a que finalice el hijo
		//-------------- Papa recibe datos de J3-----------------------------------------//
		papa(fd2p, &j3);
		//---------- Se ejecutan casillas especiales en caso de que J3 este sobre una -------//
		if (strcmp(j3.paso, "3") == 0){
			strcpy(j3.paso, "0");
            strcpy(j3.tesoro, "3");
		}

        if (strcmp(j3.paso, "bc") == 0){
			strcpy(j3.paso, "0");
		}
		else if(camaras < 2 && strcmp(j3.paso, "bt") == 0){
			turnos = turnos + 5;
			printf("\nEl J3 consigue 5 turnos extra para todos:D\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j3.paso, "0");			
		}
		else if(strcmp(j3.paso, "bn") == 0){
			turnos = turnos - 3;
			printf("\nJ3 acaba de perder 3 turnos D:\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j3.paso, "0");			
		}
		else if(portales >= 2 && strcmp(j3.paso, "btp") == 0){
			printf("Para J3:\n");
			teletransporte("mapa.txt", j3.nombre);
		}
		printf("\nEl jugador %s esta encima de %s y el tesoro es %s\n",j3.nombre, j3.paso, j3.tesoro);
		//-------------- Genera al J4 -----------------------------------------//
		pid_bot3 = fork();
		//-------------- Hace lo que debe hacer -----------------------------------------//
		if (pid_bot3 == 0) {
            leer_map=leerArchivo(mapa,&numElementos);
            leer_lista_mapas(lista_mapas,mapas);
            b=0;
            while(b<7225){
                if(strcmp(leer_map[b],"J4")==0){
                    j4_coord=b;
                    //printf("Jugador 1: %d\n",j1_coord);
                }
                b++;
            }
			bot(fdu3, fd3p, j4,j4_coord,leer_map,mapas,tablero1,tablero2,tablero3,tablero4,tablero5,tablero6,tablero7,tablero8);
			return 0;
		}
		waitpid(pid_bot3, NULL, 0); // Esperamos a que bot termine
		//-------------- Papa recibe datos de J4-----------------------------------------//
		papa(fd3p, &j4);
		//---------- Se ejecutan casillas especiales en caso de que J3 este sobre una -------//
		if (strcmp(j4.paso, "4") == 0){
			strcpy(j4.paso, "0");
            strcpy(j4.tesoro, "4");
		}		
        if (strcmp(j4.paso, "bc") == 0){
			strcpy(j4.paso, "0");
		}
		else if(camaras < 2 && strcmp(j4.paso, "bt") == 0){
			turnos = turnos + 5;
			printf("\nEl J3 consigue 5 turnos extra para todos:D\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j4.paso, "0");			
		}
		else if(strcmp(j4.paso, "bn") == 0){
			turnos = turnos - 3;
			printf("\nJ3 acaba de perder 3 turnos D:\n");
			printf("Les quedan %d turnos\n", turnos);
			strcpy(j4.paso, "0");			
		}
		else if(portales >= 2 && strcmp(j4.paso, "btp") == 0){
			printf("Para J4:\n");
			teletransporte("mapa.txt", j4.nombre);
		}
		printf("\nEl jugador %s esta encima de %s y el tesoro es %s\n",j4.nombre, j4.paso, j4.tesoro);
		///-------------- Cierre de pipes-----------------------------------------//
		close(fdu1[0]);
		close(fdu1[1]);
		close(fdu2[0]);
		close(fdu2[1]);
		close(fdu3[0]);
		close(fdu3[1]);
		close(fdup[0]);
		close(fdup[1]);
		//-------------- Se resta 1 turno -----------------------------------------//
		
        turnos = turnos - 1;
        leer_map=leerArchivo(mapa,&numElementos);
        leer_lista_mapas(lista_mapas,mapas);
        b=0;
        while(b<7225){
            if(strcmp(leer_map[b],"J4")==0){
                j4_coord=b;
            }else if(strcmp(leer_map[b],"J1")==0){
                j1_coord=b;
            }else if(strcmp(leer_map[b],"J2")==0){
                j2_coord=b;
            }else if(strcmp(leer_map[b],"J3")==0){
                j3_coord=b;
            }
            b++;
        }
        char tesoro1_f[MAX_LENGTH];
        char tesoro2_f[MAX_LENGTH];
        char tesoro3_f[MAX_LENGTH];
        char tesoro4_f[MAX_LENGTH];
        strcpy(tesoro1_f,j1.tesoro);
        strcpy(tesoro2_f,j2.tesoro);
        strcpy(tesoro3_f,j3.tesoro);
        strcpy(tesoro4_f,j4.tesoro);
        if(verficacion_final(j1_coord,j2_coord,j3_coord,j4_coord,tesoro1_f,tesoro2_f,tesoro3_f,tesoro4_f)==1){
            printf("\nFELICIDADES GANARON\n");
            turnos =0;
            break;
        }
	}
    printf("\nJUEGO TERMINADO\n");
	return 0;
}