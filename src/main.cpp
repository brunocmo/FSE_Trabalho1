#include <iostream>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"

int main() {

  struct bme280_dev dev;

  struct identifier id;

  int8_t rslt = BME280_OK;

  char filePath[] = "/dev/i2c-1";

  if ((id.fd = open(filePath, O_RDWR)) < 0)
  {
      fprintf(stderr, "Failed to open the i2c bus %s\n", filePath);
      exit(1);
  }

  id.dev_addr = BME280_I2C_ADDR_PRIM;

  if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
  {
      fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
      exit(1);
  }

  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_us = user_delay_us;

  dev.intf_ptr = &id;

  /* Initialize the bme280 */
  rslt = bme280_init(&dev);
  if (rslt != BME280_OK)
  {
      fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
      exit(1);
  }

  rslt = stream_sensor_data_forced_mode(&dev);
  if (rslt != BME280_OK)
  {
      fprintf(stderr, "Failed to stream sensor data (code %+d).\n", rslt);
      exit(1);
  }

  CommsProtocol * teste = new CommsProtocol();

  if ( teste->solicitarTemperaturaInterna() ) {
    return EXIT_FAILURE;
  }

  sleep(2);

  if ( teste->solicitarTemperaturaPotenciometro() ) {
    return EXIT_FAILURE;
  }

  sleep(2);

  if ( teste->lerComandosDoUsuario() ) {
    return EXIT_FAILURE;
  }



  delete(teste);

  return EXIT_SUCCESS;
}
