#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Defines
#define PIN_SERVO 22
#define STEP 5

// Variáveis Globais
const uint16_t duty_cycles[] = {2400, 1470, 500};
uint8_t tamanho = 0;
uint32_t servo_level = 500;
bool up_down = true;

// Prototipação das funções
void pwm_setup();
void set_servo_position(uint16_t dutyCycle);
void smooth_movement();

int main() {
    
    // Inicializações
    stdio_init_all();
    pwm_setup();
    
    while (true) {
        while(tamanho < 3){
            set_servo_position(duty_cycles[tamanho]);
            tamanho++;
        }
        if(tamanho == 3){
            smooth_movement();
        }
    }
}

void pwm_setup() {
    gpio_set_function(PIN_SERVO, GPIO_FUNC_PWM);
    uint sliceNum = pwm_gpio_to_slice_num(PIN_SERVO);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.f);
    pwm_config_set_wrap(&config, 20000);
    pwm_init(sliceNum, &config, true);
}

void set_servo_position(uint16_t dutyCycle) {
    // Seta a posição do servo motor com base no Duty Cycle
    pwm_set_gpio_level(PIN_SERVO, dutyCycle);
    sleep_ms(5000);
}

void smooth_movement() {
    // Rotina para movimentação periódica do braço do servomotor
    if (up_down) {
        servo_level += STEP;
        if (servo_level >= 2400)
            up_down = false;
    } else {
        servo_level -= STEP;
        if (servo_level <= 500)
            up_down = true;
    }
    pwm_set_gpio_level(PIN_SERVO, servo_level);
    sleep_ms(10);
}