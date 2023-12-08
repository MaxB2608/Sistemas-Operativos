README
Maximiliano Bardi, RUT: 21.030.899-7, ROL: 202173510-0
Bastian Ortega, RUT: 21.266.187-2, ROL: 202173547-K

El makefile con la instrucción "make Magic_maze" compila mm y genera un ejecutable llamado "programa", el cual es ejecutado por el propio makefile.
La instrucción "make clean" elimina el ejecutable programa.

ÍNDICE DE NUESTRO CÓDIGO:
Nuestro código presenta muchas líneas, por lo que se hará un pequeño índice aproximado de lo que está en cada sección.

1-1400 se encuentran las funciones utilizadas en la tarea, incluyendo bibliotecas y definiciones

1400-1650 se genera lo necesario para empezar el juego, cosas como el mapa o los jugadores

1650 - 2000 está todo lo relacionado a la lógica del juego, incluyendo el uso de pipes y forks

CONSIDERACIONES UTILIZADAS EN LA TAREA

- Se considera que, a la hora de pedir ingresar una dirección, esta siempre será en minúscula, es decir: arriba, abajo, izquierda y derecha. Cualquier error podría alterar gravemente el funcionamiento, puesto que, no se alcanzó a
sobreescribir el código para evitarlos
- SOBRE "B": Debido a la naturaleza del código, para activar la carta el jugador que tiene la carta tiene que estar "encima" de B, la posibilidad de usar la carta aparece cuando esto pasa. Se pedirá al usuario adónde quiere expandir
el mapa, por lo que se espera que este indique apropiadamente la dirección para seguir la naturaleza del juego.
- SOBRE "E": Para activar E (y que salga la opción), el jugador que tiene la carta debe estar "al lado" de dicha E (da igual si a la izquierda, derecha, arriba o abajo). Al activarse, la E se "transforma" en un 0, como si hubiese
destrozado la puerta.
- SOBRE "bc": las cámaras aparecen en minúscula y estos se eliminan al pasar por encima, si el jugador en un turno se pone encima y en el siguiente se mueve, donde estaba la cámara habrá un 0 (la destruyó).
- SOBRE "bt": Aparecen en minúscula, si el jugador en un turno se pone encima, el turno aumenta 5 unidades, sólo en caso de que la cantidad de cámaras es menor a 2.
- SOBRE "bn": Aparecen en minúscula, si el jugador en un turno se pone encima, el turno disminuye 3 unidades, independientemente de la cantidad de cámaras.
- SOBRE "btp": Aparecen en minúscula, si el jugador en un turno se pone encima, se teletransporta a otro btp, esto es para todos, no hay un tp especial para J1 o J2 o J3 o J4. En caso de haber 2 o más btp, se dará opciones para
escoger, esta elección estará basada en la ubicación. Si no se entiende el último punto: básicamente saldrá en pantalla opciones enumeradas y uno tendrá que elegir la opción insertando el número (si la opción 1 le gusta pues marque
1 y Enter). Cada opción muestra la columna y fila para que el usuario sepa lo que ingresa. Ejemplo: 1) Columna 45, Fila 45.

*SOBRE el Mapa: 
- la función "crear_mapa_inicial" y la línea 1486 contiene strings que corresponden al nombre del archivo. Uno de nosotros no usó máquina virtual y usa wsl, y tuvo que poner la dirección del archivo (donde se encuentra (ej: C:\Users\Admin\Downloads))
De todos modos, todos los elementos que cuentan con este problema tienen un comentario que dice "IMPORTANTE". Se entregará como funciona en la VM pero dejamos instancia de esto.
- Se van a generar 5 archivos txt, donde 4 representan los mapas que se pueden utilizar por extensión (cuales se pueden usar para la derecha, izquierda, etc.) y un txt llamado "mapa" el cual representa el mapa inicial.
Dicho mapa es una matriz de 85x85 rellena de "-", de esta forma, se "manipula" una matriz de tamaño constante      

SOBRE LOS ARCHIVOS: En la tarea se asumió que los archivos se llaman: Inicio.txt, tablero1.txt, tablero2.txt, tablero3.txt, tablero4.txt, tablero5.txt, tablero6.txt, tablero7.txt y tablero8.txt.
