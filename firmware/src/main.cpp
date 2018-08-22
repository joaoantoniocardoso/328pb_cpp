// coding: utf-8

#include "main.h"


void init(void)
{
     #ifdef USART_ON
        usart->init(MYUBRR, 1, 1);
        VERBOSE_MSG_INIT(usart->send("\n\n\nUSART... OK!\n"));
    #endif

    _delay_ms(1000);

    #ifdef PWM_ON
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("PWM..."));
        #endif
        pwm->init();
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
        #endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("PWM... OFF!\n"));
        #endif
    #endif

    #ifdef CAN_ON
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("CAN (500kbps)..."));
		#endif
        can->init(BITRATE_500_KBPS);
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
            VERBOSE_MSG_INIT(usart->send("CAN filters..."));
		#endif
        //can_static_filter(can_filter);
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("CAN... OFF!\n"));
		#endif
    #endif

    #ifdef ADC_ON
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("ADC..."));
		#endif
        adc->init();
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("ADC... OFF!\n"));
		#endif
    #endif

    #ifdef SLEEP_ON 
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("SLEEP..."));
		#endif
        sleep->init();
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("SLEEP... OFF!\n"));
		#endif
    #endif

    #ifdef WATCHDOG_ON
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("WATCHDOG..."));
		#endif
        wdt->init();
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("WATCHDOG... OFF!\n"));
		#endif
    #endif

 	#ifdef MACHINE_ON
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("MACHINE..."));
		#endif
		machine->init();
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send(" OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("MACHINE... OFF!\n"));
		#endif
	#endif
	
    #ifdef LED_ON
        set_bit(LED_DDR, LED);                      // LED como saída
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("LED... OK!\n"));
		#endif
    #else
        #ifdef USART_ON
            VERBOSE_MSG_INIT(usart->send("LED... OFF!\n"));
		#endif
    #endif

    sei();
 
}

int main(void)
{
    init();
   
	for(;;){
		#ifdef WATCHDOG_ON
            wdt->reset();
		#endif

        #ifdef MACHINE_ON
            machine->run();
        #else
            #ifdef LED_ON
                cpl_led();
                _delay_ms(300);
                cpl_led();
                _delay_ms(100);
                cpl_led();
                _delay_ms(100);
                cpl_led();
                _delay_ms(300);
            #endif

        #ifdef PWM_ON
            #ifdef PWM_TEST
                #ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
                    pwm->set_duty_cycle(CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE);
                #else
                    static uint8_t i = 0;
                    pwm->set_duty_cycle(i++);
                    if(i>PWM_D_MAX) i = PWM_D_MIN;
                #endif
            #endif
        #endif

        #endif

		#ifdef SLEEP_ON
            sleep_mode();
		#endif
	}
}


/**
 * @brief se em debug, fica chaveando os pinos de debugs até o reset do watchdog
 */
ISR(BADISR_vect)
{
    for(;;){
        #ifdef USART_ON
            VERBOSE_MSG_ERROR(usart->send("\nFATAL ERROR: BAD ISR."));
            #ifdef WATCHDOG_ON
                VERBOSE_MSG_ERROR(usart->send("WAITING FOR WATCHDOG TO RESET...\n"));
            #endif
        #endif
        #ifdef DEBUG_ON
            DEBUG0;
            DEBUG1;
            _delay_ms(100);
        #endif
    }
}

