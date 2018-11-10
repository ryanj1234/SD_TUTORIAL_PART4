#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "spi.h"
#include "sdcard.h"
#include "sdprint.h"

#define BAUD_RATE 57600

int main(void)
{
    // array to hold responses
    uint8_t res[5], sdBuf[512], error;

    // initialize UART
    UART_init();

    // initialize SPI
    SPI_init(SPI_MASTER | SPI_FOSC_128 | SPI_MODE_0);

    _delay_ms(1000);

    // initialize sd card
    if(SD_init() != SD_SUCCESS)
    {
        UART_pputs("Error initializaing SD CARD\r\n");
    }
    else
    {
        uint32_t myAddr = 0x7fffffff;
        UART_pputs("SD Card initialized\r\n");

        res[0] = SD_readSingleBlock(myAddr, sdBuf, &error);
        UART_pputs("CMD17 Response:\r\n");
        SD_printR1(res[0]);

        UART_pputs("Error token:\r\n");
        SD_printDataResToken(error);

        UART_pputs("Status:\r\n");
        SD_sendStatus(res);
        SD_printR2(res);

        if(error == SD_READ_NO_ERROR)
        {
            UART_pputs("Sector ");
            UART_puthex8((uint8_t)myAddr);
            UART_pputs(":\r\n");
            if(res[0] == 0x00) {SD_printBuf(sdBuf); UART_pputs("\r\n"); }
            else UART_pputs("\tRead Error\r\n");

            SD_sendStatus(res);
            SD_printR2(res);
        }

        /*
        for(uint16_t i = 0; i < SD_BLOCK_LEN; i++) sdBuf[i] = 0x55;

        UART_pputs("Writing sector...\r\n");
        SD_writeSingleBlock(myAddr, sdBuf, res);
        UART_pputs("Response:\r\n");
        SD_printR1(res[0]);
        UART_pputs("Token:\r\n");
        SD_printDataResponseToken(res[1]);
        _delay_ms(10);

        UART_pputs("Status Register:\r\n");
        SD_sendStatus(res);
        SD_printR2(res);

        UART_pputs("Reading back sector:\r\n");
        res[0] = SD_readSingleBlock(myAddr, sdBuf);
        if(res[0] == 0x00) { UART_pputs("\t"); for(uint16_t i = 0; i < SD_BLOCK_LEN; i++) UART_puthex8(sdBuf[i]); UART_pputs("\r\n"); }
        else UART_pputs("\tRead Error\r\n");
        */

        /*if(res[0] == 0x00)
        {
            UART_pputs("Reading CID:\r\n");
            res[0] =  SD_sendCID(sdBuf);
            if(res[0] == 0x00)
            {
                SD_printCSD(sdBuf);
            }
        }
        else
        {
            UART_pputs("Res0: ");
            UART_puthex8(res[0]);
            UART_puts("\r\n");
        }*/
    }

    while(1) ;
}
