# Enunciado

## Objetivo:

Implementar un módulo de software para trabajar con retardos no bloqueantes a partir
 de las funciones creadas en la práctica 2.

## Configuración

1. Setear el label `LD2` para el PIN `GPIO:PA5` en `GPIO_Output`
1. Setear el label `BTN` para el PIN `GPIO:PC13` en `GPIO_EXTI13`
1. `GPO` -> `NVIC` -> EXTI Line Interrupt

![pins](../images/practica1-pin-assigments.png)

### Punto 1:

Implementar las funciones auxiliares necesarias para usar retardos no bloqueantes en un archivo
 fuente main.c con su correspondiente archivo de cabecera main.h.
En main.h se deben ubicar los prototipos de las siguientes funciones y las declaraciones

```c
typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile?
typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile?
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
```

En main.c se deben ubicar la implementación de todas las funciones:

Consideraciones para la implementación:

1. `delayInit` debe cargar el valor de duración del retardo en la estructura, en el campo correspondiente.
 No debe iniciar el conteo del retardo. Debe inicializar el flag `running` en `false`.
2. `delayRead` debe verificar el estado del flag `running`.
    * `false`, tomar marca de tiempo y cambiar `running` a `true`
    * `true`, hacer la cuenta para saber si el tiempo del retardo se cumplió o no:

            marca de tiempo actual - marca de tiempo inicial es mayor o igual a duración del retardo?

    * devolver un valor booleano que indique si el tiempo se cumplió o no.
    * Cuando el tiempo se cumple se debe cambiar el flag `running` a `false`.
3. `delayWrite` debe permitir cambiar el tiempo de duración de un delay existente

### Punto 2:

Implementar un programa que utilice retardos no bloqueantes y haga titilar en forma periódica un led
 de la placa NUCLEO-F4xx de acuerdo a una secuencia predeterminada:

```c
const uint32_t TIEMPOS[] = {500, 100, 100, 1000};
```

Utilizar la función delayWrite y una única variable tipo delay_t para cambiar el tiempo de encendido del led.
NOTA: los tiempos indicados son de encendido y el led debe trabajar con duty = 50%

### Punto 3

Implementar la siguiente función auxiliar pública en API_delay.c

```c
bool_t delayIsRunning(delay_t * delay);
```

Esta función debe devolver una copia del valor del campo running de la estructura delay_t

Utilizar esta función en el código implementado para el punto dos para verificar que el
delay no esté corriendo antes de cambiar su valor con delayWrite.
