#-----------------------------------------------------------------------------------------------------------------------#
#Bibliotecas utilizadas
import threading
import random
import time
import queue
import datetime
#-----------------------------------------------------------------------------------------------------------------------#
#Definición de los departamentos
departamentos = {
    "Departamento_de_Matematicas": {"capacidad_fila": 20, "duracion_consulta": 9, "capacidad_departamento": 10},
    "Departamento_de_Informatica": {"capacidad_fila": 8, "duracion_consulta": 5, "capacidad_departamento": 2},
    "Departamento_de_Fisica": {"capacidad_fila": 15, "duracion_consulta": 7, "capacidad_departamento": 5},
    "Departamento_de_Quimica": {"capacidad_fila": 6, "duracion_consulta": 4, "capacidad_departamento": 3},
    "DEFIDER": {"capacidad_fila": 6, "duracion_consulta": 1, "capacidad_departamento": 5},
    "Departamento_de_Mecanica": {"capacidad_fila": 9, "duracion_consulta": 4, "capacidad_departamento": 4},
    "Departamento_de_Minas": {"capacidad_fila": 7, "duracion_consulta": 5, "capacidad_departamento": 2}
}
#-----------------------------------------------------------------------------------------------------------------------#
#Creación de semáforos para cada uno de los departamentos basados en su capacidad
semaforos = {depto: threading.Semaphore(departamentos[depto]["capacidad_departamento"]) for depto in departamentos}
#-----------------------------------------------------------------------------------------------------------------------#
#Creación de colas de prioridad para cada departamento 
colas = {depto: queue.PriorityQueue(departamentos[depto]["capacidad_fila"]) for depto in departamentos}
#-----------------------------------------------------------------------------------------------------------------------#
#Candado para la sincronización de escritura
candado = threading.Lock()
#-----------------------------------------------------------------------------------------------------------------------#
#Funcion utilizadas por las hebras
def persona(id):
    #-------------------------------------------------------------------------------------------------------------------#
    #                                                                          Selección de dos departamentos aleatorios#
    deptos = random.sample(list(departamentos.keys()), 2)                                                               #
    # Registra la entrada a la universidad y los departamentos a los que irá                                            #
    entradas = []                                                                                                       #
    entradas.append(f"Persona{id}, {datetime.datetime.now().strftime('%H:%M:%S.%f')[:-1]}")                             #
    #-------------------------------------------------------------------------------------------------------------------#
    for i, depto in enumerate(deptos):
        #---------------------------------------------------------------------------------------------------------------#
        #                                                                       Espera hasta que haya espacio en la fila#
        while True:                                                                                                     #
            if not colas[depto].full():                                                                                 #
                break                                                                                                   #
        #---------------------------------------------------------------------------------------------------------------#
        #                              Registra la entrada a la fila y entra a la cola con la hora actual como prioridad#
        entradas.append(f"{depto}, {datetime.datetime.now().strftime('%H:%M:%S.%f')[:-1]}")                             #
        colas[depto].put((time.time(), id))                                                                             #
        #---------------------------------------------------------------------------------------------------------------#
        #                                                               Espera hasta que haya espacio en el departamento#
        with semaforos[depto]:
            #-----------------------------------------------------------------------------------------------------------#
            #                                                                                      Entra al departamento#
            tiempo_entrada = datetime.datetime.now().strftime('%H:%M:%S.%f')[:-1]
            time.sleep(departamentos[depto]["duracion_consulta"])
            candado.acquire() 
            try:
                with open(f"{depto}.txt", "a") as file:
                    file.write(f"Persona{id}, {entradas[-1].split(', ')[-1]}, {tiempo_entrada}, {i+1}\n")
            finally:
                candado.release()
            #-----------------------------------------------------------------------------------------------------------#
        #---------------------------------------------------------------------------------------------------------------#
        #                                                                                          Sale del departamento#
        colas[depto].get()                                                                                              #
        #---------------------------------------------------------------------------------------------------------------#
    #-------------------------------------------------------------------------------------------------------------------#
    #                                                                               Registro en el Patio de las Lamparas#
    candado.acquire()                                                                                                   #
    try:                                                                                                                #
        with open("PdLamparas.txt", "a") as file:                                                                       #       
            file.write(', '.join(entradas) + "\n")                                                                      #
    finally:                                                                                                            #
        candado.release()                                                                                               #
    #-------------------------------------------------------------------------------------------------------------------#
    #                                                                                           Salida de la universidad#
    candado.acquire()                                                                                                   #
    try:                                                                                                                #
        with open("Salida.txt", "a") as file:                                                                           #
            file.write(f"Persona{id}, {datetime.datetime.now().strftime('%H:%M:%S.%f')[:-1]}\n")                        #
    finally:                                                                                                            #
        candado.release()                                                                                               #
    #-------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#                                                                      Se generan las hebras y estas ejecutan la funcion#
for i in range(500):
    threading.Thread(target=persona, args=(i,)).start()
#-----------------------------------------------------------------------------------------------------------------------#