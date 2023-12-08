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


char* Horizontal(char doc[],char palabra[]){
    int time=0;
    int indice=0;                               //--------------------------------
    FILE* archivo;
    char caracteres;
    int contador=0;
    archivo=fopen(doc,"r");
    while ((caracteres=fgetc(archivo))!= EOF){  // Se busca el largo del archivo
        if (!isspace(caracteres)){
            contador++;
        }
    }
    fclose(archivo);                            //--------------------------------
    char letras[contador];
    char caracter;
    archivo=fopen(doc,"r");
    while ((caracter=fgetc(archivo))!= EOF){
        if (!isspace(caracter)){                // Se "juntan" los caracteres, ignorando
            letras[indice]=caracter;            // los espacios en blanco y salto de linea
            indice++;
        }
    }                                           //--------------------------------
    for(int i=0; i<strlen(palabra);i++){
        palabra[i]=toupper(palabra[i]);         // Pone la palabra en mayuscula
    }                                           //--------------------------------
    char* resultado= strstr(letras,palabra);
    if (resultado!=NULL){                       //busca la palabra
        printf("Palabra encontrada.\nPalabra: %s\nOrientacion: Horizontal\n", palabra);
    }
    return 0;
};                                              //--------------------------------

char* Vertical (char doc[],char lista[], int largo) {
    int indice=0;
    FILE *archivo;                                  //--------------------------------
    char caracteres;
    int contador=0;
    archivo=fopen(doc,"r");
    while ((caracteres=fgetc(archivo))!= EOF){
        if (!isspace(caracteres)){
            contador++;
        }
    }                                               // Lo mismo que Horizontal, buscar
    fclose(archivo);                                // el largo de la sopa, y juntar los
    char letras[contador];                          // caracteres que no sean saltos de linea
    char caracter;                                  // o espacio en blanco
    archivo=fopen(doc,"r");
    while ((caracter=fgetc(archivo))!= EOF){
        if (!isspace(caracter)){
            letras[indice]=caracter;
            indice++;
        }
    }                                               //--------------------------------
    
    // -------------------------------------------------------------------------------------- //
    char lista_2[largo];                        
    for(int i=0; i<strlen(lista);i++){              // Poner en mayuscula la palabra
        lista_2[i]=toupper(lista[i]);
    }                                               //--------------------------------
    bool flag = true;
    
    int letra = 8;
    int palabra = 0;
    int comprueba = 1;
    int correcto = 1;
    int len_sopa = round(sqrt(contador - 8));      // Busca la palabra
                                                
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
                    comprueba = 1;
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
};                                                  //--------------------------------

char* sopa(char datos[]){
    //--------------------------------------------------------------------
    char palabra[200];
    const char* corte= strchr(datos,'.');
    if(corte != NULL){
        int largo= corte - datos;           //Se extrae la palabra del nombre del archivo
        strncpy(palabra,datos,largo);
        palabra[largo]='\0';
    }
    int largo_pal=strlen(palabra);
    //--------------------------------------------------------------------
    FILE *archivo;
    char caracteres;
    int contador=0;                         // Analizamos el documento y buscamos el largo de éste
    archivo=fopen(datos,"r");
    while ((caracteres=fgetc(archivo))!= EOF){
        if (!isspace(caracteres)){
            contador++;
        }
    }
    //--------------------------------------------------------------------
    fclose(archivo);
    char letras[contador];
    int indice=0;                          // Analizamos los datos y eliminamos los espacios en blanco
    char caracter;                         // quedando todo unido 
    archivo=fopen(datos,"r");
    while ((caracter=fgetc(archivo))!= EOF){
        if (!isspace(caracter)){
            letras[indice]=caracter;
            indice++;
        }
    }
    fclose(archivo);
    //--------------------------------------------------------------------
    char orientacion =letras[0]; // Se obtiene la orientacion con el primer caracter de la Sopa
    //--------------------------------------------------------------------
    int largo_2=0;
    if(orientacion=='v'){       // Caso donde la orientacion es vertical
        int largo_2=round(sqrt(contador-8)); // Largo de la sopa
        //---------------------------------------------------
        clock_t start_time = clock();
        Vertical(datos,palabra,largo_pal);  // Se busca la palabra y se mide el tiempo
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Tiempo de demora: %f segundos\n\n", elapsed_time);
        //---------------------------------------------------
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
            char carpeta[20]= "vertical/100x100/";          // Se transfiere el archivo
            strcat(carpeta, archivo);                       // al directorio correspondiente
                                                        
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
        //---------------------------------------------------
    //--------------------------------------------------------------------
    }if(orientacion=='h'){      // Caso donde la orientacion es horizontal
        int largo_2=round(sqrt(contador-10));   // Se obtiene el largo de la sopa de letras
        //---------------------------------------------------
        clock_t start_time = clock();
        Horizontal(datos,palabra);              // Se busca la palabra y se mide el tiempo
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("Tiempo de demora: %f segundos\n\n", elapsed_time);
        //---------------------------------------------------
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
            char carpeta[20]= "horizontal/100x100/";        // Se transfiere el archivo
            strcat(carpeta, archivo);                       // al directorio correspondiente

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
        //--------------------------------------------------- 
    }
    //--------------------------------------------------------------------
}

int main(){
    //--------------------------------------------------------------------
    const char *comprueba = "horizontal";
    struct stat stats;
    if(stat(comprueba, &stats) == 0){
        printf("Las carpetas ya existen");
    }
    else{
        mkdir("horizontal", 0755);      // Se crean los directorios 
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
    //--------------------------------------------------------------------
    DIR *dir;
    struct dirent *ent;
                                    
    dir = opendir(".");                         // Se recorren todos los archivos txt
    if (dir == NULL){                           // menos el README y se ejecuta la funcion Sopa
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
    //--------------------------------------------------------------------
}