#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

char* Horizontal(char letras[],char palabra[],int contador){
                                                
    for(int i=0; i<strlen(palabra);i++){    //--------------------------------
        palabra[i]=toupper(palabra[i]);     // Pone la palabra en mayuscula
    }                                       //--------------------------------
    char* resultado= strstr(letras,palabra);        
    if (resultado!=NULL){                   // Busca la palabra
        printf("Palabra encontrada.\nPalabra: %s\nOrientacion: Horizontal\n", palabra);
    }                                               
    return 0;       
};                                          //--------------------------------

char* Vertical(char letras[],char lista[], int largo,int contador) {
char lista_2[largo];                    
    for(int i=0; i<strlen(lista);i++){      //--------------------------------
        lista_2[i]=toupper(lista[i]);       // Pone la palabra en mayuscula
    }                                       //--------------------------------
    bool flag = true;
    
    int letra = 8;
    int palabra = 0;
    int comprueba = 1;
    int correcto = 1;
    int len_sopa = round(sqrt(contador - 8));
    
    while (flag) {
        if (letras[letra] == lista_2[palabra]) {
            bool flag2 = true;
            while (flag2) {
                if (letras[letra + len_sopa * comprueba] == lista_2[comprueba]) {
                    comprueba++;
                    correcto++;
                    if (correcto == sizeof(lista_2)) {
                        printf("%s","Palabra encontrada.\n");
                        printf("Palabra:%s\nOrientacion: vertical\n", lista);
                        flag2 = false;
                        flag = false;
                    }
                } else {
                    comprueba = 1;          // Busca la palabra
                    correcto = 1;
                    letra++;
                    flag2 = false;
                }
            }
        } else {
            letra++;
        }
    }
    
    return 0;
};                                         //--------------------------------

//----------------------------------------------------------------------------------------------------------------------------
// El resto del codigo no cambia, puesto que se optimizan las funciones Vertical y Horizontal.
// El unico cambio en sopa son la cantidad de parametros en las funciones ya mencionadas.
char* sopa(char datos[]){
    //------------------------------------------------------------
    char palabra[200];
    const char* corte= strchr(datos,'.');
    if(corte != NULL){
        int largo= corte - datos;
        strncpy(palabra,datos,largo);
        palabra[largo]='\0';
    }
    int largo_pal=strlen(palabra);
    //------------------------------------------------------------
    FILE *archivo;
    char caracteres;
    int contador=0;
    archivo=fopen(datos,"r");
    while ((caracteres=fgetc(archivo))!= EOF){
        if (!isspace(caracteres)){
            contador++;
        }
    }
    //------------------------------------------------------------
    fclose(archivo);
    char letras[contador];
    int indice=0;
    char caracter;
    archivo=fopen(datos,"r");
    while ((caracter=fgetc(archivo))!= EOF){
        if (!isspace(caracter)){
            letras[indice]=caracter;
            indice++;
        }
    }
    fclose(archivo);
    char orientacion =letras[0];
    //------------------------------------------------------------
    int largo_2=0;
    if(orientacion=='v'){
        int largo_2=round(sqrt(contador-8));
        //printf("\n%d",largo_2);
        clock_t start_time = clock();
        Vertical(letras,palabra,largo_pal,contador);
        clock_t end_time = clock();

        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Tiempo de demora: %f segundos\n\n", elapsed_time);
        char* archivo = datos;
        if (largo_2 == 50){
            char carpeta[20]= "vertical/50x50/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado.\n\n");
            } else{
                perror("Error al mover el archivo");
            }   
        }
        if(largo_2 == 100){
            char carpeta[20]= "vertical/100x100/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado. \n\n");
            } else{
                perror("Error al mover el archivo");
            }   
        }
        if(largo_2 == 200){
            char carpeta[20]= "vertical/200x200/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado.\n\n");
            } else{
                perror("Error al mover el archivo");
            }            
        } 
        
    //------------------------------------------------------------
    }if(orientacion=='h'){
        int largo_2=round(sqrt(contador-10));
        clock_t start_time = clock();
        Horizontal(letras,palabra, contador);
        clock_t end_time = clock();

        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Tiempo de demora: %f segundos\n\n", elapsed_time);
        char* archivo = datos;
        if (largo_2 == 50){
            char carpeta[20]= "horizontal/50x50/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado.\n\n");
            } else{
                perror("Error al mover el archivo");
            }   
        }
        if(largo_2 == 100){
            char carpeta[20]= "horizontal/100x100/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado.\n \n");
            } else{
                perror("Error al mover el archivo");
            }   
        }
        if(largo_2 == 200){
            char carpeta[20]= "horizontal/200x200/";
            strcat(carpeta, archivo);

            if (rename(archivo, carpeta) == 0){
                printf("Archivo transladado. \n\n");
            } else{
                perror("Error al mover el archivo");
            }            
        } 
    }
    //------------------------------------------------------------
}

int main(){
    //------------------------------------------------------------
    const char *comprueba = "horizontal";
    struct stat stats;
    if(stat(comprueba, &stats) == 0){
        printf("Las carpetas ya existen");
    }
    else{
        mkdir("horizontal", 0755);
        mkdir("vertical", 0755);
        chdir ("horizontal");
        mkdir("50x50", 0755);
        mkdir("100x100", 0755);
        mkdir("200x200", 0755);
        chdir("..");
        chdir ("vertical");
        mkdir("50x50", 0755);
        mkdir("100x100", 0755);
        mkdir("200x200", 0755);
        chdir("..");
    }
    //------------------------------------------------------------
    DIR *dir;
    struct dirent *ent;

    dir = opendir(".");
    if (dir == NULL){
        perror("No se pudo abrir el directorio");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL){
        if(strstr(ent->d_name,".txt") != NULL && strcmp(ent->d_name, "README.txt")){
            printf("%s\n", ent->d_name);
            sopa(ent->d_name);
        }
    }
    closedir(dir);
    return 0;
    //------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------------------------------