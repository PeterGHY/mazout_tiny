#define EICRA MCUCR         // names seems to be different ....
#define EIMSK GIMSK


//  SONAR_TRIGGER OUTPUT
#define SONAR_TRIGGER_DDR DDRB
#define SONAR_TRIGGER_PORT PORTB
#define SONAR_TRIGGER_PIN PB1  //PB1 pin 3

#define SONAR_TRIGGER_OUTPUT_MODE() SONAR_TRIGGER_DDR |= (1<<SONAR_TRIGGER_PIN)         // set as output

#define SONAR_TRIGGER_LOW() SONAR_TRIGGER_PORT &= ~(1<<SONAR_TRIGGER_PIN)
#define SONAR_TRIGGER_HIGH() SONAR_TRIGGER_PORT |= (1<<SONAR_TRIGGER_PIN)

// SONAR ECHO INPUT
#define SONAR_ECHO_DDR DDRB
#define SONAR_ECHO_PORT PORTB
#define SONAR_ECHO_PIN PB2      // PB2 pin 5
#define SONAR_ECHO_INPUT_MODE() SONAR_ECHO_DDR &= ~(1 << SONAR_ECHO_PIN)                 // set as input
#define SONAR_ECHO_PULL_UP() SONAR_ECHO_PORT |= (1 << SONAR_ECHO_PIN)                    // pull-up

volatile uint32_t running;
volatile unsigned char up;
volatile uint32_t distance;
volatile uint32_t timerCounter;

void srf04_init();
void sonar();
