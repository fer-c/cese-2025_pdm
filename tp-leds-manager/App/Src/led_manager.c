/*
 * led_manager.c
 *
 *  Created on: Apr 21, 2025
 *      Author: fcoronel
 */

#include "led_manager.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

/**
 * @brief  Inicializa el manager de LEDs, imprimiendo en el LCD y UART la configuración inicial.
 * @param  manager: Estructura del manager de LEDs.
 * @return None
 */
static void _stateInit(led_manager_t *manager);

void ledManagerInit(led_manager_t *manager,
                    led_matrix_t *led_matrix,
                    lcd_t *lcd,
                    console_t *console)
{
    assert_param(manager != NULL);
    assert_param(led_matrix != NULL);
    assert_param(lcd != NULL);
    assert_param(console != NULL);

    manager->led_matrix = led_matrix;  // Inicializa la matriz de LEDs
    manager->lcd = lcd;  // Inicializa el LCD
    manager->console = console;  // Inicializa el UART
    manager->state = MANAGER_STATE_INIT;  // Inicializa el estado del manager
    manager->duration = T_100MS;

    lcdInitialize(manager->lcd);  // Inicializa el LCD

    _stateInit(manager);
}

void ledManagerUpdate(led_manager_t *manager)
{
    static uint8_t cmd = 0;
    static char text[50];

    assert_param(manager != NULL);

    console_status_t status = consoleReadCommand(manager->console, &cmd);  // Lee el comando del UART

    if (status == CONSOLE_STATUS_OK && cmd != NO_CMD)
    {
        switch (cmd)
        {
            case 'h':
            case 'H':
                consoleClearScreen(manager->console);  // Borra la pantalla
                consoleSendConfiguration(manager->console);  // Envía la configuración del UART
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                manager->duration = (cmd - '0') * T_100MS;
                sprintf(text, "Delay: %lu ms", manager->duration);
                lcdPrintLine(manager->lcd, LCD_LINEA_DELAY, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                break;
            case '0':
                strcpy(text, "Delay: 1 s");
                manager->duration = T_1S;
                lcdPrintLine(manager->lcd, LCD_LINEA_DELAY, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                break;
            case 'A':
                strcpy((char*) text, "Superior: AUTO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        | MANAGER_STATE_SUP_AUTO_VALUE;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_AUTO);
                break;
            case 'a':
                strcpy((char*) text, "Inferior: AUTO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        | MANAGER_STATE_INF_AUTO_VALUE;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_AUTO);
                break;
            case 'R':
                strcpy((char*) text, "Superior: ROJO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_RED);
                break;
            case 'r':
                strcpy((char*) text, "Inferior: ROJO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_RED);
                break;
            case 'G':
                strcpy((char*) text, "Superior: VERDE");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_GREEN);
                break;
            case 'g':
                strcpy((char*) text, "Inferior: VERDE");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_GREEN);
                break;
            case 'B':
                strcpy((char*) text, "Superior: AZUL");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_BLUE);
                break;
            case 'b':
                strcpy((char*) text, "Inferior: AZUL");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_BLUE);
                break;
            case 'Y':
                strcpy((char*) text, "Superior: AMARILLO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_YELLOW);
                break;
            case 'y':
                strcpy((char*) text, "Inferior: AMARILLO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_YELLOW);
                break;
            case 'M':
                strcpy((char*) text, "Superior: MAGENTA");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_MAGENTA);
                break;
            case 'm':
                strcpy((char*) text, "Inferior: MAGENTA");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_MAGENTA);
                break;
            case 'C':
                strcpy((char*) text, "Superior: CYAN");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_CYAN);
                break;
            case 'c':
                strcpy((char*) text, "Inferior: CYAN");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_CYAN);
                break;
            case 'W':
                strcpy((char*) text, "Superior: BLANCO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_WHITE);
                break;
            case 'w':
                strcpy((char*) text, "Inferior: BLANCO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_WHITE);
                break;
            case 'O':
                strcpy((char*) text, "Superior: APAGADO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_SUP_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_OFF);
                break;
            case 'o':
                strcpy((char*) text, "Inferior: APAGADO");
                manager->state = (manager->state & MANAGER_STATE_INIT_RESET)
                        & MANAGER_STATE_INF_AUTO_RESET;
                lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) text);
                consolePrintLine(manager->console, (uint8_t*) text);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_OFF);
                break;

            default:
                cmd = NO_CMD;  // Comando no reconocido
        }
        if (cmd != NO_CMD)
        {
            lcdSetCursorOff(manager->lcd);
            lcdSetPosition(manager->lcd, LCD_LINEA_INFO, 9);
            lcdPrintValue(manager->lcd, cmd);
        }
        cmd = NO_CMD;  // Limpiar el comando
    }

    if (delayRead(&manager->delay))
    {
        switch (manager->state)
        {
            case MANAGER_STATE_INIT:
            case MANAGER_STATE_SUP_COLOR_INF_COLOR:
                break;
            case MANAGER_STATE_SUP_AUTO_INF_AUTO:
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_AUTO);
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_AUTO);
                break;
            case MANAGER_STATE_SUP_COLOR_INF_AUTO:
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_INF, PIXEL_AUTO);
                break;
            case MANAGER_STATE_SUP_AUTO_INF_COLOR:
                ledMatrixSetPixel(manager->led_matrix, LED_LINEA_SUP, PIXEL_AUTO);
                break;
            default:
                _stateInit(manager);
                break;
        }

        ledMatrixToggle(manager->led_matrix);
        delayWrite(&manager->delay, manager->duration);  // Cambia la duración del delay
    }
}

// Funciones auxiliares

static void _stateInit(led_manager_t *manager)
{
    delayInit(&manager->delay, manager->duration);
    ledMatrixInitialize(manager->led_matrix);
    lcdPrintLine(manager->lcd, LCD_LINEA_DELAY, (uint8_t*) "Delay: 100 ms");
    lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_SUP, (uint8_t*) "Superior: APAGADO");
    lcdPrintLine(manager->lcd, LCD_LINEA_COLOR_INF, (uint8_t*) "Inferior: APAGADO");
    lcdPrintLine(manager->lcd, LCD_LINEA_INFO, (uint8_t*) "Comando: ");
    lcdSetPosition(manager->lcd, LCD_LINEA_INFO, 9);
    lcdSetCursorOn(manager->lcd);
    manager->pixel_sup = PIXEL_OFF;
    manager->pixel_inf = PIXEL_OFF;
}
