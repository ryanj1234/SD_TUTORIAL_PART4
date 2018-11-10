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
    uint8_t res[5], buf[512], token;
    uint32_t addr = 0x00000000;

    // initialize UART
    UART_init();

    // initialize SPI
    SPI_init(SPI_MASTER | SPI_FOSC_128 | SPI_MODE_0);

    // initialize sd card
    if(SD_init() != SD_SUCCESS)
    {
        UART_pputs("Error initializaing SD CARD\r\n");
    }
    else
    {
        UART_pputs("SD Card initialized\r\n");

        // read sector 0
        UART_pputs("\r\nReading sector: 0x");
        UART_puthex8((uint8_t)(addr >> 24));
        UART_puthex8((uint8_t)(addr >> 16));
        UART_puthex8((uint8_t)(addr >> 8));
        UART_puthex8((uint8_t)addr);
        res[0] = SD_readSingleBlock(addr, buf, &token);
        UART_pputs("\r\nResponse:\r\n");
        SD_printR1(res[0]);

        // if no error, print buffer
        if((res[0] == 0x00) && (token == SD_START_TOKEN))
            SD_printBuf(buf);
        // else if error token received, print
        else if(!(token & 0xF0))
        {
            UART_pputs("Error token:\r\n");
            SD_printDataErrToken(token);
        }

        // update address to 0x00000100
        addr = 0x00000100;

        // fill buffer with 0x55
        for(uint16_t i = 0; i < 512; i++) buf[i] = 0x55;

        UART_pputs("Writing 0x55 to sector: 0x");
        UART_puthex8((uint8_t)(addr >> 24));
        UART_puthex8((uint8_t)(addr >> 16));
        UART_puthex8((uint8_t)(addr >> 8));
        UART_puthex8((uint8_t)addr);

        // write data to sector
        res[0] = SD_writeSingleBlock(addr, buf, &token);

        UART_pputs("\r\nResponse:\r\n");
        SD_printR1(res[0]);

        // if no errors writing
        if(res[0] == 0x00)
        {
            if(token == SD_DATA_ACCEPTED)
                UART_pputs("Write successful\r\n");
        }
    }

    while(1) ;
}
