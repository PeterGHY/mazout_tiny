// LED
#define LED_NUMMER PB0
#define LED_DDRB DDRB
#define LED_PORT PORTB

#define LED_DDRB_OUTPUT_MODE() LED_DDRB |= (1<<LED_NUMMER)    // set as output

#define LED_HIGH() LED_PORT |= (1<<LED_NUMMER)
#define LED_LOW() LED_PORT &= ~(1<<LED_NUMMER)

int main(void);
