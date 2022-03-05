#ifndef TEMPERATURESTATUS_HPP
#define TEMPERATURESTATUS_HPP

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "bme280.h"

struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};

/* Function that creates a mandatory delay required in some of the APIs. */
void user_delay_us(uint32_t period, void *intf_ptr);
/* Function for print the temperature, humidity and pressure data. */
void print_sensor_data(struct bme280_data *comp_data);
/* Function for reading the sensor's registers through I2C bus. */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
/* Function for writing the sensor's registers through I2C bus. */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
/* Function reads temperature, humidity and pressure data in forced mode. */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

class TemperatureStatus {

private:
    double temperatura;

public: 

    struct bme280_dev dev;

    struct identifier id;

    int8_t rslt = BME280_OK;

    TemperatureStatus();
    ~TemperatureStatus();

    bool init();

    double get_temperatura();
    int8_t set_temperature( struct bme280_dev *dev);

    float get_temperaturaEmFloat();

};



#endif