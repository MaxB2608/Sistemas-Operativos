READ ME

Maximiliano Bardi, RUT: 21.030.899-7, ROL: 202173510-0
Bastian Ortega, RUT: 21.266.187-2, ROL: 202173547-K

Los archivos se llaman TAREA_SO.c y mejor.c, donde el primero es el código principal y el segundo es la versión mejorada para responder la pregunta del informe.

El makefile con la instrucción "make TAREA_SO" compila TAREA_SO y genera un ejecutable llamado "programa", el cual es ejecutado por el propio makefile.
Para el caso de mejor.c, se debe poner en la terminal "make mejor" y genera un ejecutable llamado "programa2", el cual es ejecutado por el propio makefile.
La instrucción "make clean" elimina los ejecutables programa, programa2, además de los directorios horizontal y vertical (y los directorios adentro de estos), por lo que es recomendable tener un respaldo de los txt, puesto
que al ejecutar el código y luego usar clean, al eliminarse los directorios, también se eliminan los txt que se encuentran en ellos.

Los archivos txt probados son los que se encuentran en "Archivo pruebas" en Aula. Debido al aviso de posibles archivos no funcionales, se realizaron a mano archivos txt exactamente iguales, tanto en el nombre como el contenido.

IMPORTANTE: El código está estructurado de forma que analiza o utiliza los archivos txt que se encuentran en la misma carpeta que los códigos, es decir, para que funcione deben tener los códigos C y los archivos txt en la misma
carpeta, esto se puede ver en el main, donde la dirección que se utiliza es ".", o sea, donde se encuentra el código mismo.

IMPORTANTE 2: Como el código lee todos los archivos de extensión txt que se encuentran en la misma carpeta donde está el código mismo, TAL VEZ accidentalmente considere el README y genere un error. Esto no debería de ser posible
puesto que el main deja claro que solo se ejecuta con archivos txt que no se llamen "README.txt", pero, en caso de fallar, mover o eliminar el README, básicamente que no esté en la misma carpeta, aunque, como decimos, no debería
pasar, es por si acaso.
