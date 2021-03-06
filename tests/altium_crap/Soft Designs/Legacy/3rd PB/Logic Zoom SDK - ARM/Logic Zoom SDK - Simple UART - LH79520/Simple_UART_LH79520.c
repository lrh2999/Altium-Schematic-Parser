/********************************************************************\
|*
|* Version : 1.0
|*
|* Copyright : Copyright (C) 2006, Altium
|*
|* Description : This project uses the serial interface of the Logic
|*               Zoom development board to output a message and echo
|*               some text to an HyperTerminal interface for example.
|*
\********************************************************************/


#include "hardware.h"
#include <string.h>


/*
* Constants and definitions
*/
#define STR_SIZE    255


/***********************************************************************
|*
|* Function : uart_init
|*
|* Parameters :
|*
|* Returns :
|*
|* Description : Enable and initialize the UART interface.
*/

void uart_init( void )
{
    // Enable UART1 on Multiplexed Pins - p154 of LH79520 User's Guide
    IOCON_UARTMux(IOCONBase) |= ( IOCON_UARTMux_U1RxD | IOCON_UARTMux_U1TxD );

    // Activate UART1 clock - p136 of LH79520 User's Guide
    RCPC_PeriphClkCtrl1(RCPCBase) &= ~(0x1 << 2);

    // Select UART1 clock source - p140 of LH79520 User's Guide
    RCPC_PeriphClkSelect(RCPCBase) &= ~(0x1 << 2);

    // Set 1 Stop bits, 8-bit word lenght and no parity  - p350 of LH79520 User's Guide
    UART1_UARTLCR_H(UART1Base) = UART1_UARTLCR_H_FEN | UART1_UARTLCR_H_WLEN8;

    // p354
    UART1_UARTIFLS(UART1Base) = UART1_UARTIFLS_TXIFLSEL(2) | UART1_UARTIFLS_RXIFLSEL(2);

    // Clear all interrupts for UART1 - p355 of LH79520 User's Guide
    UART1_UARTICR(UART1Base) =  UART1_UARTICR_OEIC | UART1_UARTICR_BEIC | UART1_UARTICR_PEIC |
                                UART1_UARTICR_FEIC | UART1_UARTICR_RTIC | UART1_UARTICR_TXIC |
                                UART1_UARTICR_RXIC;

    // Disable all interrupt for UART1 - p355 of LH79520 User's Guide
    UART1_UARTIMSC(UART1Base) = 0;

    // Disable CTS/RTS flow control - p352 - not sure why it should be commented but it works without ???
    //UART1_UARTCR(UART1Base) = UART1_UARTCR_TXE | UART1_UARTCR_RXE | UART1_UARTCR_UARTEN;
}


/***********************************************************************
|*
|* Function : uart_read
|*
|* Parameters : buffer = string
|*              n_bytes = size of string
|*
|* Returns : bytes = number of bytes read
|*
|* Description : Read data from the UART and returns the number of
|*               characters read.
*/

unsigned int uart_read( void * buffer, unsigned int n_bytes )
{
    unsigned char *data = (unsigned char *) buffer;
    unsigned int bytes = 0;

    while ( (n_bytes > 0) && ( (UART1_UARTFR(UART1Base) & UART1_UARTFR_RXFE) == 0 ) )
    {
        // Read data from the FIFP into the buffer
        *data = UART1_UARTDR(UART1Base);  // (unsigned char)
        data++;

        // Increment data count and decrement buffer size cound
        bytes++;
        n_bytes--;
    }

    return bytes;
}


/***********************************************************************
|*
|* Function : uart_write
|*
|* Parameters : buffer = string
|*              n_bytes = size of string
|*
|* Returns : bytes = number of bytes written
|*
|* Description : Write data to the UART and returns the number of
|*               characters written.
*/

unsigned int uart_write( void * buffer, unsigned int n_bytes )
{
    unsigned char *data = (unsigned char*) buffer;
    unsigned int bytes = 0;

    while( n_bytes > 0 )
    {
        if ( (UART1_UARTFR(UART1Base) & UART1_UARTFR_TXFF) == 0 )
        {
            UART1_UARTDR(UART1Base) = *data;
            data++;

            bytes++;
            n_bytes--;
        }
    }

    return bytes;
}


/***********************************************************************
|*
|* Function : uart_write_welcome
|*
|* Parameters :
|*
|* Returns :
|*
|* Description : Ouputs instructions through the serial interface.
*/

void uart_write_welcome( void )
{
    char * line0 = "\r\n";
    char * line1 = "************************************************\r\n";
    char * line2 = "  Hello Example for Logic Zoom LH79520 Dev Kit\r\n";
    char * line3 = "\r\n";
    char * line4 = "  Version 1.0\r\n";
    char * line5 = "************************************************\r\n";
    char * line6 = " Type some text and press the Enter key to\r\n";
    char * line7 = " echo a string of 256 characters maximum\r\n";
    char * line8 = "************************************************\r\n";
    uart_write(line0, strlen(line0));
    uart_write(line1, strlen(line1));
    uart_write(line2, strlen(line2));
    uart_write(line3, strlen(line3));
    uart_write(line4, strlen(line4));
    uart_write(line5, strlen(line5));
    uart_write(line6, strlen(line6));
    uart_write(line7, strlen(line7));
    uart_write(line8, strlen(line8));
}


/***********************************************************************
|*
|* Function : main
|*
|* Parameters :
|*
|* Returns :
|*
|* Description : main function
*/

void main( void )
{
    unsigned int index = 0;
    char output[STR_SIZE];
    char buffer[10];

    uart_init();

    uart_write_welcome();

    output[0] = '\0';

    while (1)
    {
        while(uart_read(buffer, 1) == 0);
            
        buffer[1] = '\0';

        if((buffer[0] == '\r') || (index >= STR_SIZE))
        {
            uart_write("\r\n",2);
            uart_write("You typed - ", 12);
            uart_write(output, strlen(output));
            uart_write("\r\n",2);
            output[0] = '\0';
            index = 0;
        }
        else
        {
            strcat(output, buffer);
            uart_write(buffer, 1);
        }
        index++;
    }
}


