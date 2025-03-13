# Practica 1

Practica base para probar la configuración del entorno de desarrollo SMT32CubeIDE.

## Objetivo:

Familiarizarse con el entorno de trabajo STM32CubeIDE + NUCLEO-F446RE + firmware. 

### Punto 1:

Implementar un programa que haga parpadear el led de la placa NUCLEO-F446RE LD2.

El led debe permanecer encendido 200 ms con duty cycle 50%.

### Punto 2:

Utilizar el pulsador (B1) para controlar el tiempo de encendido.  Cada vez que se presiona el pulsador el tiempo de encendido debe alternar entre 200 ms y 500 ms.

## Consideraciones

Las multiples implmentaciones se seleccionan activando el código en cada una de las versiones

- `practica1_punto1.c`: Implementación del punto 1  
Para activarla asegurarse que este definida la macro `PRACTICA1_PUNTO1` en el tope del archivo

- `practica1_punto2_censo.c`: Implmentación del punto 2 censando el estado del botón después de cada ciclo.  
Para activarla asegurarse que este definida la macro `PRACTICA1_PUNTO2_CENSO` en el tope del archivo


- `practica1_punto2_inter.c`: Implmentación del punto 2 mediante la interrupción generada por el botón al activarse.  
Para activarla asegurarse que este definida la macro `PRACTICA1_PUNTO2_INTER` en el tope del archivo

## Configuración

1. Setear el label `LD2` para el PIN `GPIO:PA5` en `GPIO_Output`
1. Setear el label `BTN` para el PIN `GPIO:PC13` en `GPIO_EXTI13`
1. `GPO` -> `NVIC` -> EXTI Line Interrupt

![pins](../images/practica1-pin-assigments.png)
