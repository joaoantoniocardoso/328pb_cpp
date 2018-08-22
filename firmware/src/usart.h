/**
 * @file usart.h
 *
 * @defgroup USART USART Module
 *
 * @brief Simple (asynchrounous) usart using pooling method, with basic functions used for debug
 * purpose.
 *
 */

#ifndef USART_H
#define USART_H

#ifndef F_CPU
#define F_CPU 16000000UL    //define a frequencia do microcontrolador - 16MHz
#endif /* ifndef F_CPU */

#include <avr/io.h>
#include "../lib/bit_utils.h"
#include "conf.h"

#ifndef USART_BAUD
#define USART_BAUD 57600
#endif /* ifndef USART_BAUD */
#define MYUBRR F_CPU/16/USART_BAUD-1

#define   USART_HAS_DATA   bit_is_set(UCSR0A, RXC0)
#define   USART_READY      bit_is_set(UCSR0A, UDRE0)

class Usart
{
    Usart& operator = (Usart other) = delete;
    Usart(const Usart& other) = delete;
    Usart() = default;

public:

    static Usart& self(){
        static Usart self;
        return self;
    }
    ~Usart() = default;

    void init(uint16_t, uint8_t, uint8_t);

    void send_char(char data);
    void send(uint16_t num);
    void send(uint32_t num);
    void send(uint8_t *b, uint8_t lenght);
    void send(const char *s);

    char receive(void);

};

#endif
