#include "adc.h"

/**
 * @brief initializes all adc circular buffers.
 */
void Adc::init_buffers(void)
{
    CBUF_Init(cbuf_adc0);
    CBUF_Init(cbuf_adc1);
    CBUF_Init(cbuf_adc2);
}

/**
 * @brief computes the average of a given adc channel
 *
 * Ma = (1/N)*Summation of x[i] from i=0 to N, 
 * if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
 *
 */
#ifdef ADC_8BITS
uint8_t Adc::ma_adc0(void)
#else
uint16_t Adc::ma_adc0(void)
#endif
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc0_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc0, i);
    }
    avg_adc0 = sum >> cbuf_adc0_SIZE_2;
    return avg_adc0;
}

/**
* @brief computes the average of a given adc channel
*
* Ma = (1/N)*Summation of x[i] from i=0 to N, 
* if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
*
*/
#ifdef ADC_8BITS
uint8_t Adc::ma_adc1(void)
#else
uint16_t Adc::ma_adc1(void)
#endif
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc1_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc1, i);
    }
    avg_adc1 = sum >> cbuf_adc1_SIZE_2;
    return avg_adc1;
}

/**
* @brief computes the average of a given adc channel 
*
* Ma = (1/N)*Summation of x[i] from i=0 to N, 
* if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
*
*/
#ifdef ADC_8BITS
uint8_t Adc::ma_adc2(void)
#else
uint16_t Adc::ma_adc2(void)
#endif 
{   
    uint16_t sum = 0;
    for(uint8_t i = cbuf_adc2_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc2, i);
    }
    avg_adc2 = sum >> cbuf_adc2_SIZE_2;
    return avg_adc2;
}


/**
 * @brief inicializa o ADC, configurado para conversão engatilhada com o timer0.
 */
void Adc::init(void)
{

    clr_bit(PRR0, PRADC);                           // Activates clock to adc

    init_buffers();

    // configuracao do ADC
    PORTC   =   0b00000000;                         // disables pull-up for adcs pins
    DDRC    =   0b00000000;                         // all adcs as inputs
    DIDR0   =   0b11111111;                         // ADC0 to ADC2 as adc (digital disable)

    ADMUX   =   (0 << REFS1)                        // AVcc with external capacitor at AREF pin
            | (1 << REFS0)
#ifdef ADC_8BITS
            | (1 << ADLAR);                         // ADC left adjusted -> using 8bits ADCH only
#else
            | (0 << ADLAR);                         // ADC left adjusted -> using all 10 bits
#endif

    ADCSRB  =   (0 << ADTS2)                        // Auto-trigger source: timer0 Compare Match A
            | (1 << ADTS1)
            | (1 << ADTS0);

    select_channel(ADC0);                           // Choose admux

    ADCSRA  =   (1 << ADATE)                        // ADC Auto Trigger Enable
            | (1 << ADIE)                           // ADC Interrupt Enable
            | (1 << ADEN)                           // ADC Enable
            | (1 << ADSC)                           // Do the first Start of Conversion
            | (1 << ADPS2)                          // ADC Prescaller = 128;
            | (1 << ADPS1)
            | (1 << ADPS0);



    // configuracao do Timer TC0 --> TIMER DO ADC
    clr_bit(PRR0, PRTIM0);                          // Activates clock to timer0
    //set_bit(DDRD, PD5);
    //set_bit(DDRD, PD6);
    TCCR0A  =   (1 << WGM01) | (0 << WGM00)         // Timer 0 in Mode 2 = CTC (clear on compare)
            | (0 << COM0B1) | (0 << COM0B0)         // do nothing with OC0B
            | (0 << COM0A1) | (0 << COM0A0);        // Normal port operation
    TCCR0B  =   (0 << WGM02)                        // Timer 0 in Mode 2 = CTC (clear on compare)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (0 << CS02) | (1 << CS01) | (0 << CS00); // clock enabled, prescaller = 64

	OCR0A  =    199;                                 // Valor para igualdade de comparacao A para frequencia de 5kHz
    TIMSK0 |=   (1 << OCIE0A);                      // Ativa a interrupcao na igualdade de comparação do TC0 com OCR0A

}

/**
 * @brief Changes ADC channel
 * @param __ch is the channel to be switched to
 * @return return the selected channel
 */
uint8_t Adc::select_channel(Adc::channels_t __ch)
{
    if(__ch < ADC_LAST_CHANNEL ) channel = __ch;
    else                         channel = ADC0;

    ADMUX = (ADMUX & 0xF8) | channel; // clears the bottom 3 bits before ORing
    return channel;
}
 

void Adc::next_channel(void)
{
    channel = static_cast<channels_t>( static_cast<uint8_t>(channel) +1 );
    select_channel(channel);
}

/**
 * @brief MUX do ADC
 */
void Adc::run()
{
    switch(Adc::channel){
        case ADC0:
            VERBOSE_MSG_ADC(usart->send(" \tadc0: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc0, ADCH); 
#else
            CBUF_Push(cbuf_adc0, ADC); 
#endif
            break;
        case ADC1:
            VERBOSE_MSG_ADC(usart->send(" \tadc1: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc1, ADCH); 
#else
            CBUF_Push(cbuf_adc1, ADC); 
#endif
            break;
        case ADC2:
            VERBOSE_MSG_ADC(usart->send(" \tadc2: "));
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc2, ADCH);
#else
            CBUF_Push(cbuf_adc2, ADC);
#endif
        case ADC3:
        case ADC4:
        case ADC5:
        case ADC6:
        case ADC7:
        default:
            VERBOSE_MSG_ADC(usart->send("\n"));
            break;
    }        
#ifdef ADC_8BITS
    VERBOSE_MSG_ADC(usart->send(ADCH));
#else
    VERBOSE_MSG_ADC(usart->send(ADC));
#endif
    Adc::next_channel();
}
 
ISR(ADC_vect)
{
    Adc::self().run();
}

/**
 * @brief ISR necessária para auto-trigger do ADC. Caso contrário, dispara
 * BADISR_vect.
 */
EMPTY_INTERRUPT(TIMER0_COMPA_vect);

