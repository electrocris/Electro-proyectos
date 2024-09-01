from machine import Pin, PWM
import time

# Configuración de los pines PWM
pwm_pins = [PWM(Pin(18)), PWM(Pin(19)), PWM(Pin(20))]

# Configuración de la frecuencia del PWM (por ejemplo, 1 kHz)
for pwm in pwm_pins:
    pwm.freq(1000)

# Función para ajustar el brillo del LED
def set_brightness(r, g, b):
    # Como es un LED de ánodo común, hay que invertir los valores
    pwm_pins[0].duty_u16(65535 - int(r * 65535))
    pwm_pins[1].duty_u16(65535 - int(g * 65535))
    pwm_pins[2].duty_u16(65535 - int(b * 65535))

# Ciclo principal para recorrer el espectro RGB
try:
    step = 1024  # Incremento del valor PWM por cada iteración
    delay = 0.01  # Tiempo de espera entre cada cambio de color (10 ms)

    while True:
        # Subir rojo, mantener verde y azul apagados
        for r in range(0, 65536, step):
            set_brightness(r / 65535, 0, 0)
            time.sleep(delay)

        # Subir verde, mantener rojo en máximo
        for g in range(0, 65536, step):
            set_brightness(1, g / 65535, 0)
            time.sleep(delay)

        # Bajar rojo, mantener verde en máximo
        for r in range(0, 65536, step):
            set_brightness(1 - r / 65535, 1, 0)
            time.sleep(delay)

        # Subir azul, mantener verde en máximo
        for b in range(0, 65536, step):
            set_brightness(0, 1, b / 65535)
            time.sleep(delay)

        # Bajar verde, mantener azul en máximo
        for g in range(0, 65536, step):
            set_brightness(0, 1 - g / 65535, 1)
            time.sleep(delay)

        # Subir rojo, mantener azul en máximo
        for r in range(0, 65536, step):
            set_brightness(r / 65535, 0, 1)
            time.sleep(delay)

        # Bajar azul, mantener rojo en máximo
        for b in range(0, 65536, step):
            set_brightness(1, 0, 1 - b / 65535)
            time.sleep(delay)

except KeyboardInterrupt:
    # Apagar el LED al interrumpir
    set_brightness(0, 0, 0)