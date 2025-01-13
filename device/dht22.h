/*
    https://cdn.sparkfun.com/assets/f/7/d/9/c/DHT22.pdf
*/

#include <unistd.h>

#define DHT22_PIN 18
#define DHT_START_SIGNAL_MS 2


struct Dht22Data {
    uint16_t RH_int;
    uint16_t RH_dec;
    uint16_t T_int;
    uint16_t T_dec;
    uint16_t checksum;
}

bool readDht22Data(struct Data * data) {
    gpioSetPullUpDown(DHT22_PIN, PI_PUD_UP);
    gpioSetMode(DHT22_PIN, PI_OUTPUT);

    // send start signal
    gpioWrite(DHT22_PIN, PI_LOW);
    usleep(DHT_START_SIGNAL_MS * 1000);

    gpioSetMode(DHT22_PIN, PI_INPUT);

    // wait for data
    while (true) {

    }
}
