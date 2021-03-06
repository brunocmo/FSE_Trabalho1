
/*
*
* by Lewis Loflin www.bristolwatch.com lewis@bvu.net
* http://www.bristolwatch.com/rpi/i2clcd.htm
* Using wiringPi by Gordon Henderson
*
*
* Port over lcd_i2c.py to C and added improvements.
* Supports 16x2 and 20x4 screens.
* This was to learn now the I2C lcd displays operate.
* There is no warrenty of any kind use at your own risk.
*
*/

#ifndef SHOWINFOLCD_HPP
#define SHOWINFOLCD_HPP

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

// Define some device parameters
#define I2C_ADDR   0x27 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0x04 // Enable bit

class ShowInfoLCD {

    private:
        char mensagemAcima16[16];
        char mensagemAbaixo16[16];

    public:

        ShowInfoLCD();
        ~ShowInfoLCD();

        void lcd_init(void);
        void lcd_byte(int bits, int mode);
        void lcd_toggle_enable(int bits);

        // added by Lewis
        void typeInt(int i);
        void typeFloat(float myFloat);
        void lcdLoc(int line); //move cursor
        void ClrLcd(void); // clr LCD return home
        void typeln(const char *s);
        void typeChar(char val);
        int fd;  // seen by all subroutines

        void mostrarMensagem();

        char *get_mensagemAcima16();
        void set_mensagemAcima16( char *mensagem );

        char *get_mensagemAbaixo16();
        void set_mensagemAbaixo16( char *mensagem );


};
 

#endif