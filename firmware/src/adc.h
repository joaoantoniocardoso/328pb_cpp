/**
 * @file adc.h
 *
 * @defgroup ADC    ADC Module
 *
 * @brief This module implements a simple ADC using a state machine to mux
 * between the adc channels.
 *
 */

#ifndef _ADC_H_
#define _ADC_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "conf.h"
#include "dbg_vrb.h"
#include "usart.h"
#include "../lib/bit_utils.h"
#include "../lib/cbuf.h"
//#include "../lib/log2.h"

#define ADC_LAST_CHANNEL 3                  //*< quantity of channels used
//#define ADC_8BITS
//
#define cbuf_adc0_SIZE          64      // size of buffers
#define cbuf_adc0_SIZE_2        6       // size of buffers in 2^n
#define cbuf_adc1_SIZE          64      // size of buffers
#define cbuf_adc1_SIZE_2        6       // size of buffers in 2^n
#define cbuf_adc2_SIZE          64      // size of buffers
#define cbuf_adc2_SIZE_2        6       // size of buffers in 2^n

class Adc
{
    Usart *usart;

    enum channels_t : volatile uint8_t{ 
		ADC0, ADC1 ,ADC2, ADC3, ADC4, ADC5, ADC6, ADC7 
    }channel = ADC0;

    Adc& operator = (Adc& other) = delete;
    Adc(const Adc& other) = delete;
    Adc() = default;

    uint8_t select_channel(Adc::channels_t __ch);

	// MOVING AVERAGE BELOW //
	void init_buffers(void);

	volatile struct cbuf_adc0{
		uint8_t     m_getIdx;
		uint8_t     m_putIdx;
        #ifdef ADC_8BITS
	        uint8_t     m_entry[cbuf_adc0_SIZE];
        #else
		    uint16_t    m_entry[cbuf_adc0_SIZE];
        #endif

	} cbuf_adc0;
    #ifdef ADC_8BITS
	    uint8_t ma_adc0(void);
	    uint8_t avg_adc0;
    #else
	    uint16_t ma_adc0(void);
	    uint16_t avg_adc0;
    #endif

	volatile struct cbuf_adc1{
		uint8_t     m_getIdx;
		uint8_t     m_putIdx;
        #ifdef ADC_8BITS
		    uint8_t     m_entry[cbuf_adc1_SIZE];
        #else
		    uint16_t    m_entry[cbuf_adc1_SIZE];
        #endif
	} cbuf_adc1; 
    #ifdef ADC_8BITS
        uint8_t ma_adc1(void);
        uint8_t avg_adc1;
    #else
        uint16_t ma_adc1(void);
        uint16_t avg_adc1;
    #endif

	volatile struct cbuf_adc2{
		uint8_t     m_getIdx;
		uint8_t     m_putIdx;
        #ifdef ADC_8BITS
		    uint8_t     m_entry[cbuf_adc2_SIZE];
        #else
		    uint16_t    m_entry[cbuf_adc2_SIZE];
        #endif
	} cbuf_adc2;  
    #ifdef ADC_8BITS
	    uint8_t ma_adc2(void);
	    uint8_t avg_adc2;
    #else
	    uint16_t ma_adc2(void);
	    uint16_t avg_adc2;
    #endif

public:

    static Adc& self(){
        static Adc self;
        return self;
    }

    ~Adc() = default;

    void run();

    void init(void);
    void next_channel(void);

    void isr_handler(void);
 
};
     

#endif /* ifndef _ADC_H_ */
