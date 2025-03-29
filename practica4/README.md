# Practica 4

El enunciado del ejercicio esta [acá](enunciado.md)

## Configuración

## Include Path

Agregar al include path `API/Inc`

### Pinout

1. Setear el label `LD2` para el PIN `GPIO:PA5` en `GPIO_Output`
1. Setear el label `BTN` para el PIN `GPIO:PC13` en `GPIO_EXTI13`
1. `GPO` -> `NVIC` -> EXTI Line Interrupt

![pins](../images/practica1-pin-assigments.png)

## Documentación por Doxygen

La documentación por Doxygen se encuentra en [docs](docs/html/index.html) generada con:

```
doxygen Doxyfile
```

## Diagramas

### sequencia de llamadas

```mermaid
sequenceDiagram
    participant MainProgram as ====== Main ======
    participant fsm as         ================ FSM  ===============
    participant matrix as      ====== delay_time_matrix_t ======
    participant LedDelay as    ====== delay_t ======

    MainProgram->>matrix: delayTimeMatrixInit
    MainProgram->>LedDelay: delayInit
    MainProgram->>fsm: debounceFSM_init

    loop
        MainProgram->>fsm: debounceFSM_update()
        alt 
        alt Transición BUTTON_FALLING -> BUTTON_DOWN
            fsm -->> fsm: buttonPressed()
            Note Right of fsm: btn_pressed = true
        end
        alt Transición BUTTON_RAISING -> BUTTON_UP
            fsm -->> fsm: buttonReleased()
            Note Right of fsm: 
        end
        end

        MainProgram->>LedDelay: delayRead
        LedDelay-->>MainProgram: 
        alt delayRead
            MainProgram->>fsm: readKey
            Note Right of fsm: btn_pressed = false
            fsm-->>MainProgram: 
            alt readKey == true
                MainProgram->>matrix: delayTimeMatrixNext
                matrix-->>MainProgram: newDuration
                MainProgram->>LedDelay: delayWrite(newDuration)
            end
        end
    end
```

### Diagrama de estado debounceFSM

```mermaid

stateDiagram-v2
    state BUTTON_UP
    state BUTTON_FALLING
    state BUTTON_RAISING
    state BUTTON_DOWN    

    [*] --> BUTTON_UP
    BUTTON_UP --> BUTTON_FALLING: HAL_GPIO_ReadPin == true

    BUTTON_FALLING --> BUTTON_UP: delayRead && HAL_GPIO_ReadPin == false
    BUTTON_FALLING --> BUTTON_DOWN: delayRead && HAL_GPIO_ReadPin == true

    BUTTON_DOWN --> BUTTON_RAISING: HAL_GPIO_ReadPin == false

    BUTTON_RAISING --> BUTTON_DOWN: delayRead && HAL_GPIO_ReadPin == true
    BUTTON_RAISING --> BUTTON_UP: delayRead && HAL_GPIO_ReadPin == false

```

