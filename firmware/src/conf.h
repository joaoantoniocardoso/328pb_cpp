/**
 * @file conf.h
 *
 * @defgroup CONF Configurations 
 *
 * @brief General configuration of the system.
 *
 */

#ifndef CONF_H
//#define CONF_H

#include "new.h"

// CONFIGURACOES DE COMPILACAO
//#define DEBUG_ON
//#define VERBOSE_ON
//#define VERBOSE_ON_CAN_APP
//#define VERBOSE_ON_MACHINE
//#define VERBOSE_ON_ADC
//#define VERBOSE_ON_PWM
#define VERBOSE_ON_INIT
//#define VERBOSE_ON_ERROR

// TESTS
//#define TEST_PWM

// MODULES ACTIVATION
#define USART_ON
//#define CAN_ON
//#define CAN_DEPENDENT
//#define ADC_ON
//#define MACHINE_ON
//#define PWM_ON
//#define LED_ON
//#define WATCHDOG_ON
//#define SLEEP_ON

 

// PWM DEFINITIONS
#ifdef PWM_ON
// WARNING: DEFINITIONS FOR TEST THE CONVERTER WITH FIXED DUTY CYCLE!!!
//#define CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
//#define CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE 0.60*(PWM_TOP)
//#define PWM_TEST 
 
#define INITIAL_D                   0               //!< float value from 0 to 1
#define PWM_FREQUENCY               100000          //! frequency in Hz
#define PWM_D_DELTA                 1               //!< amount to increase (may interfer on threshholds)
#define PWM_D_MAX_DELTA             1               //!< clock divisor
#define PWM_D_MIN                   0.01*(PWM_TOP)  //!< minimum D
#define PWM_D_MAX                   0.9*(PWM_TOP)   //!< maximum D
#define PWM_D_MIN_THRESHHOLD        PWM_D_MIN       //!< minimum D threshhold
#define PWM_D_MAX_THRESHHOLD        PWM_D_MAX       //!< maximum D threshhold
#define PWM_D_LIN_MULT              1               //!< this is A for D = (D*A) >> B
#define PWM_D_LIN_DIV               1               //!< this is B for D = (D*A) >> B
#ifdef CONVERTER_TEST_WITH_FIXED_DUTYCYCLE
#define PWM_INITIAL_D               CONVERTER_TEST_WITH_FIXED_DUTYCYCLE_DT_VALUE
#else
#define PWM_INITIAL_D               0.6f*(PWM_TOP)
#endif

// number of checks before reset the pwm fault counter.
#define CHECKS_BEFORE_RESET_FAULT_COUNTER 100
// maximum of consecutive faults before state an error
#define FAULT_COUNT_LIMIT           50
#endif

// INPUT PINS DEFINITIONS
/*#define     BatOverVoltageInterrupt_PORT PORTD	// <--------- ADICIONADO
#define     BatOverVoltageInterrupt_PIN  PIND
#define     BatOverVoltageInterrupt_DDR  DDRD
#define     BatOverVoltageInterrupt      PD2

#define     Enable_PORT             PORTD	// <------------- ADICIONADO
#define     Enable_PIN              PIND
#define     Enable_DDR              DDRD
#define     Enable                  PD3
*/

// OUTPUT PINS DEFINITIONS
#ifdef PWM_ON
#define     PWM_PORT                PORTB
#define     PWM_PIN                 PINB
#define     PWM_DDR                 DDRB
#define     PWM                     PB1
#endif 

#ifdef LED_ON
#define     LED_PORT                PORTD
#define     LED_PIN                 PIND
#define     LED_DDR                 DDRD
#define     LED                     PD7
#define     cpl_led()               cpl_bit(LED_PORT, LED)
#define     set_led()               set_bit(LED_PORT, LED)
#define     clr_led()               clr_bit(LED_PORT, LED)
#else
#define     cpl_led()               
#define     set_led()               
#define     clr_led()               
#endif

// CANBUS DEFINITONS
// ----------------------------------------------------------------------------
/* Global settings for building the can-lib and application program.
 *
 * The following two #defines must be set identically for the can-lib and
 * your application program. They control the underlying CAN struct. If the
 * settings disagree, the underlying CAN struct will be broken, with
 * unpredictable results.
 * If can.h detects that any of the #defines is not defined, it will set them
 * to the default values shown here, so it is in your own interest to have a
 * consistent setting. Ommiting the #defines in both can-lib and application
 * program will apply the defaults in a consistent way too.
 *
 * Select if you want to use 29 bit identifiers.
 */
#define	SUPPORT_EXTENDED_CANID	0

/* Select if you want to use timestamps.
 * Timestamps are sourced from a register internal to the AT90CAN.
 * Selecting them on any other controller will have no effect, they will
 * be 0 all the time.
 */
#define	SUPPORT_TIMESTAMPS		0



#endif /* ifndef CONF_H */
