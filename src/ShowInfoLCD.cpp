#include "../inc/ShowInfoLCD.hpp"

ShowInfoLCD::ShowInfoLCD() {
    std::strcpy( mensagemAcima16, "                " );
    std::strcpy( mensagemAbaixo16, "                " );
    fd = wiringPiI2CSetup(I2C_ADDR);
}

ShowInfoLCD::~ShowInfoLCD() {}

void ShowInfoLCD::mostratMensagem() {
    if (std::wiringPiSetup () == -1) exit (1);

    //printf("fd = %d ", fd);

    lcd_init(); // setup LCD

    lcdLoc(LINE1);
    typeln(get_mensagemAcima16());
    lcdLoc(LINE2);
    typeln(get_mensagemAbaixo16());
}

// float to string
void ShowInfoLCD::typeFloat(float myFloat)   {
  sprintf(this->mensagemAbaixo16, "%2.4f",  myFloat);
}

// int to string
void ShowInfoLCD::typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ShowInfoLCD::ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void ShowInfoLCD::lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void ShowInfoLCD::typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void ShowInfoLCD::typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void ShowInfoLCD::lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void ShowInfoLCD::lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void ShowInfoLCD::lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

char *ShowInfoLCD::get_mensagemAcima16() {
    return this->mensagemAcima16;
}

void ShowInfoLCD::set_mensagemAcima16( char *mensagem ){
    std::strcpy( mensagemAcima16, mensagem );
}

char *ShowInfoLCD::get_mensagemAbaixo16() {
    return this->mensagemAbaixo16;
}

void ShowInfoLCD::set_mensagemAbaixo16( char *mensagem ){
    std::strcpy( mensagemAbaixo16, mensagem );
}
