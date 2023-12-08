README
Maximiliano Bardi, RUT: 21.030.899-7, ROL: 202173510-0
Bastian Ortega, RUT: 21.266.187-2, ROL: 202173547-K

Sobre el Makefile
El Makefile está hecho de manera que los códigos se ejecuten por separado, debido a esto, si se quiere ejecutar las 3 funciones se deben utilizar los 3 comandos. A continuación se presentarán los comandos a escribir dependiendo del código:
* Para ejecutar la función sin hebras ni forks: make runSopa
* Para ejecutar la función con forks: make runSopaForks
* Para ejecutar la función con hebras: make runSopaThreads
* Para "limpiar" los ejecutables: make clean 

Sobre el Excel:
*En caso de verse "raro" el Excel al descargarlo, acá está el link por si acaso: https://docs.google.com/spreadsheets/d/1oQa6LpySu57Dacj-BYnS0wC5lG-nASITcwweWjrps20/edit?usp=sharing 

Consideraciones:
* Los tiempos que demora en buscar no son constantes, estos cambian con cada ejecucion, debido a que no controlamos que proceso o hebra toma el procesador, por lo que podria tomarlo al inicio tanto como al final resultando tiempos variados.
* Se hizo uso de maquina virtual para programar en linux.
