//SingletonInterruptionExample
class SingIntEx
{
public:
    void setPin(uint8_t pin) {
         _pin = pin;
    }

    void run() {
        if(_pin == -1) {
            return;
        }

        digitalWrite(_pin, !digitalRead(_pin));
    }
    
    static SingIntEx& self() {
        static SingIntEx self;
        return self;
    }
    ~SingIntEx() = default;

private:
    SingIntEx& operator = (SingIntEx& other) = delete;
    SingIntEx(const SingIntEx& other) = delete;
    SingIntEx() = default;

    uint8_t _pin = -1;
};

// the setup function runs once when you press reset or power the board
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    SingIntEx::self().setPin(LED_BUILTIN); // Start class


    // TIMER 1 for interrupt frequency 2 Hz:
    cli(); // stop interrupts
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    // set compare match register for 2 Hz increments
    OCR1A = 31249; // = 16000000 / (256 * 2) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 256 prescaler
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei(); // allow interrupts
};

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
    SingIntEx::self().run();
}

// the loop function runs over and over again forever
void loop() {
}