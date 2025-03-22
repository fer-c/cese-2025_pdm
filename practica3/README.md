# Practica 2

El enunciado del ejercicio esta [acá](enunciado.md)

## Configuración

Agregar al include path `Drivers/API/Inc`

## Documentación por Doxygen

La documentación por Doxygen se encuentra en [docs](docs/html/index.html) generada con:

```
doxygen Doxyfile
```


## Diagrama de flujo

```mermaid
flowchart TD
    A[Inicio] --> B["delayInit(delay, duration)"]
    B --> C["delayTimeMatrixInit(matrix, num_durations, durations)"]
    C --> D{"delayRead(delay)"}
    D -->|true| E["updateLedOnDelay(delay, matrix)"]
    E --> F["toggleLedState()"]
    F --> G{"checkLedStateReset()"}
    G -->|true| H["updateDelay(delay, matrix)"]
    H --> I["delayTimeMatrixRead(matrix)"]
    I -->|true| J["delayTimeMaxtrixCurrentDuration(matrix)"]
    J --> K["delayWrite(delay, newDuration)"]
    G -->|false| L[Continuar]
    D -->|false| L[Continuar]
    L --> D
```

```mermaid
flowchart TD

    subgraph updateLedOnDelay
        E1["checkLedStateReset()"]
        E2["updateDelay(delay, matrix)"]
        E3["toggleLedState()"]
        E1 -->|true| E2
        E2 --> E3
        E1 -->|false| E3
    end
```

```mermaid
flowchart TD
    subgraph updateDelay
        H1["delayTimeMatrixRead(matrix)"]
        H2{"delayTimeMatrixRead(matrix)"}
        H3["delayTimeMaxtrixCurrentDuration(matrix)"]
        H4["delayWrite(delay, newDuration)"]
        H1 --> H2
        H2 -->|true| H3
        H3 --> H4
    end
```
