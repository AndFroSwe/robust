#include "mbed.h"
/* mbed TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

class TextLCD : public Stream {
public:

    /** LCD panel format */
    enum LCDType {
        LCD16x2     /**< 16x2 LCD panel (default) */
        , LCD16x2B  /**< 16x2 LCD panel alternate addressing */
        , LCD20x2   /**< 20x2 LCD panel */
        , LCD20x4   /**< 20x4 LCD panel */
        , LCD16x1   /**< 16x1 LCD panel */
    };

    /** Create a TextLCD interface
     *
     * @param rs    Instruction/data control line
     * @param e     Enable line (clock)
     * @param d0-d3 Data lines
     * @param type  Sets the panel size/addressing mode (default = LCD16x2)
     */
    TextLCD(PinName rs, PinName e, PinName d0, PinName d1, PinName d2, PinName d3, LCDType type = LCD16x2);

    /** Locate to a screen column and row
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void locate(int column, int row);

    /** Clear the screen and locate to 0,0 */
    void cls();

    int rows();
    int columns();

protected:

    // Stream implementation functions
    virtual int _putc(int value);
    virtual int _getc();

    int address(int column, int row);
    void character(int column, int row, int c);
    void writeByte(int value);
    void writeCommand(int command);
    void writeData(int data);

    DigitalOut _rs, _e;
    BusOut _d;
    LCDType _type;

    int _column;
    int _row;
};



TextLCD::TextLCD(PinName rs, PinName e, PinName d0, PinName d1,
                 PinName d2, PinName d3, LCDType type) : _rs(rs),
        _e(e), _d(d0, d1, d2, d3),
        _type(type) {

    _e  = 1;
    _rs = 0;            // command mode

    wait(0.015);        // Wait 15ms to ensure powered up

    // send "Display Settings" 3 times (Only top nibble of 0x30 as we've got 4-bit bus)
    for (int i=0; i<3; i++) {
        writeByte(0x3);
        wait(0.00164);  // this command takes 1.64ms, so wait for it
    }
    writeByte(0x2);     // 4-bit mode
    wait(0.000040f);    // most instructions take 40us

    writeCommand(0x28); // Function set 001 BW N F - -
    writeCommand(0x0C);
    writeCommand(0x6);  // Cursor Direction and Display Shift : 0000 01 CD S (CD 0-left, 1-right S(hift) 0-no, 1-yes
    cls();
}

void TextLCD::character(int column, int row, int c) {
    int a = address(column, row);
    writeCommand(a);
    writeData(c);
}

void TextLCD::cls() {
    writeCommand(0x01); // cls, and set cursor to 0
    wait(0.00164f);     // This command takes 1.64 ms
    locate(0, 0);
}

void TextLCD::locate(int column, int row) {
    _column = column;
    _row = row;
}

int TextLCD::_putc(int value) {
    if (value == '\n') {
        _column = 0;
        _row++;
        if (_row >= rows()) {
            _row = 0;
        }
    } else {
        character(_column, _row, value);
        _column++;
        if (_column >= columns()) {
            _column = 0;
            _row++;
            if (_row >= rows()) {
                _row = 0;
            }
        }
    }
    return value;
}

int TextLCD::_getc() {
    return -1;
}

void TextLCD::writeByte(int value) {
    _d = value >> 4;
    wait(0.000040f); // most instructions take 40us
    _e = 0;
    wait(0.000040f);
    _e = 1;
    _d = value >> 0;
    wait(0.000040f);
    _e = 0;
    wait(0.000040f);  // most instructions take 40us
    _e = 1;
}

void TextLCD::writeCommand(int command) {
    _rs = 0;
    writeByte(command);
}

void TextLCD::writeData(int data) {
    _rs = 1;
    writeByte(data);
}

int TextLCD::address(int column, int row) {
    switch (_type) {
        case LCD20x4:
            switch (row) {
                case 0:
                    return 0x80 + column;
                case 1:
                    return 0xc0 + column;
                case 2:
                    return 0x94 + column;
                case 3:
                    return 0xd4 + column;
            }
        case LCD16x2B:
            return 0x80 + (row * 40) + column;
        case LCD16x1:
            return 0x80 + column % 8 + (column / 8)*0x40;
        case LCD16x2:
        case LCD20x2:
        default:
            return 0x80 + (row * 0x40) + column;
    }
}

int TextLCD::columns() {
    switch (_type) {
        case LCD20x4:
        case LCD20x2:
            return 20;
        case LCD16x1:
        case LCD16x2:
        case LCD16x2B:
        default:
            return 16;
    }
}

int TextLCD::rows() {
    switch (_type) {
        case LCD20x4:
            return 4;
        case LCD16x1:
            return 1;
        case LCD16x2:
        case LCD16x2B:
        case LCD20x2:
        default:
            return 2;
    }
}

#include <stdarg.h>

#define PE_IN_INIT  0
#define PE_IN_TEST  1
#define PE_OUT_INIT 2
#define PE_OUT_TEST 3

DigitalInOut pc0(p30);
DigitalInOut pc1(p29);
DigitalInOut pc2(p28);
DigitalInOut pc3(p27);


AnalogIn adc0(p15);
AnalogIn adc1(p16);
AnalogIn adc2(p17);

AnalogOut dac0(p18);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

PwmOut pwm0(p26); // p25 p26 p30
PwmOut pwm1(p25); // p25 p26 p30
PwmOut pwm2(p24); // p25 p26 p30

DigitalInOut pe0(p5);
DigitalInOut pe1(p6);
DigitalInOut pe2(p7);
DigitalInOut pe3(p8);
DigitalInOut pe4(p9);
DigitalInOut pe5(p10);
DigitalInOut pe6(p11);
DigitalInOut pe7(p12);

DigitalInOut asse(p19);

TextLCD lcd(p14, p13, p20, p21, p22, p23, TextLCD::LCD20x4); // rs, e, d4-d7


void Delay( unsigned int d );



void init_pin(DigitalInOut p, char *dir)
{
    if (dir[0] == 'o')
    {
        p.output();
    }
    else
    {
        p.input();
        p.mode(PullNone);
    }
}


void SET_BIT( DigitalInOut p )
{
    p = 1;
}


void CLR_BIT( DigitalInOut p )
{
    p = 0;
}

int GET_BIT( DigitalInOut p )
{
    Delay(10);
    return ( p );
}


void PWM0( int d )
{
    pwm0 = ( (float) d ) / 100;  
    wait(0.01);
}


void PWM1( int d )
{
    pwm1 = ( (float) d ) / 100;
    wait(0.01);
}


void PWM2( int d )
{
    pwm2 = ( (float) d ) / 100;
    wait(0.01);
}


unsigned int GET_AD(int channel)
{
    float x;
      
    switch ( channel )
    {
        case 0 :
            x = adc0.read();
        break;
          
        case 1 :
            x = adc1.read();
        break;
        
        case 2 :
            x = adc2.read();
        break;
        
        
        default :
            x = 0;
    }
    
    return ( (int)(x * 1023.0) );
}


void Delay(unsigned int d)
{
    wait_ms(d);
}


void clear_disp(void)
{
    lcd.cls();
}

void move_cursor( int r, int c )
{
    lcd.locate(c-1,r-1);
}

void dprintf(char *format, ...)
{
     char text[20];
     va_list ap;
     char *p, *sval; 
     char *cval;
     int ival;
     unsigned int uval;
     double dval;
     long int lival;

     va_start(ap, format);

     for (p = format; *p; p++)
     {
        if(*p != '%')
        {
            lcd.putc(*p);
            continue;
        }
        switch (*++p)
        {
            case 'i':
                  ival = va_arg(ap, int);
                  sprintf(text, "%i", ival);
                  lcd.puts(text);
                  break;
            case 'f':
                  dval = va_arg(ap, double);
                  sprintf(text, "%.4f", dval);
                  lcd.puts(text);
                  break;
            case 's':
                  for (sval = va_arg(ap, char *); *sval; sval++)
                    lcd.putc(*sval);
                  break;
            case 'c':
                  cval = va_arg(ap, char *);
                  lcd.putc(*cval);
                  break;
            case 'u':
                  uval = va_arg(ap, unsigned int);
                  sprintf(text, "%u", uval);
                  lcd.puts(text);
                  break;  
            case 'l':
                  lival = va_arg(ap, long int);
                  sprintf(text, "%li", lival);
                  lcd.puts(text);
                  break;
        
            case '%':
                  lcd.puts("%");
                  break;
                  
            default:
                lcd.puts("ERROR HORROR!");
                break;
        }
    }     
    va_end(ap);    
}

// Initierar startvarden, in- och utgangar.    
void init_mik(void)
{
    int tst_state, tst_adc0, tst_adc1, tst_adc2;
    char text[20];    
    unsigned int asse_cnt;
    led1 = led2 = led3 = led4 = 0;
    
    lcd.cls();
        
    pwm0.period_us(50);
    pwm0 = 0.50;
    pwm1.period_us(50);
    pwm1 = 0.50;
    pwm2.period_us(50);
    pwm2 = 0.50;
        
    // Initierar pinnar som ingångar    
    init_pin(pe0,"in");
    init_pin(pe1,"in");
    init_pin(pe2,"in");
    init_pin(pe3,"in");
    init_pin(pe4,"in");
    init_pin(pe5,"in");
    init_pin(pe6,"in");
    init_pin(pe7,"in");
    
    init_pin(pc0,"in");
    init_pin(pc1,"in");
    init_pin(pc2,"in");
    init_pin(pc3,"in");


    asse.input();
    asse.mode(PullUp);


    if ( asse == 0 )
    {
        tst_state = PE_IN_INIT;
        asse_cnt = 0;
        
        while (1)
        {      
            tst_adc0 = (int) ((GET_AD(0) * 100)/1023.0);        
            tst_adc1 = (int) ((GET_AD(1) * 100)/1023.0);//GET_AD(1) * 0.0978;     
            tst_adc2 = (int) ((GET_AD(2) * 100)/1023.0);//GET_AD(2) * 0.0978;     
                        
            switch (tst_state)
            {

                case PE_IN_INIT :
                    move_cursor(1,1);
                    dprintf("PE:76543210 PWM0:");
                    move_cursor(2,13);
                    dprintf("PWM1:");
                    move_cursor(3,1);
                    dprintf("PC:3210     PWM2:");

                    move_cursor(4,11);
                    dprintf("DAC0:");
                                                        
                    tst_state = PE_IN_TEST;
                    break;

                case PE_IN_TEST :
                    move_cursor(2,4);           

                    dprintf("%i", GET_BIT(pe7));  
                    dprintf("%i", GET_BIT(pe6));  
                    dprintf("%i", GET_BIT(pe5));  
                    dprintf("%i", GET_BIT(pe4));  
                    dprintf("%i", GET_BIT(pe3));  
                    dprintf("%i", GET_BIT(pe2));  
                    dprintf("%i", GET_BIT(pe1));  
                    dprintf("%i", GET_BIT(pe0));  
                    
                    move_cursor(4,4);    
                    dprintf("%i", GET_BIT(pc3));  
                    dprintf("%i", GET_BIT(pc2));  
                    dprintf("%i", GET_BIT(pc1));  
                    dprintf("%i", GET_BIT(pc0));  

                    break;

                case PE_OUT_INIT :

                    break;

                case PE_OUT_TEST :

                    break;

                default :
                    break;
            }

            move_cursor(1,18);       
            sprintf(text, "%3d", tst_adc0);
            lcd.puts(text);   
            
            move_cursor(2,18);       
            sprintf(text, "%3d", tst_adc1);
            lcd.puts(text);   
            
            move_cursor(3,18);       
            sprintf(text, "%3d", tst_adc2);
            lcd.puts(text);   
            
            move_cursor(4,16);       
            sprintf(text, "%4.2fV", (tst_adc0*3.3)/100);
            lcd.puts(text);   
            
            PWM0(tst_adc0);
            PWM1(tst_adc1);
            PWM2(tst_adc2);
            
            dac0.write(adc0.read());
            
            if ( asse == 0 )
            {
                if ( ++asse_cnt == 100 )
                {
                    led1=1;
                    asse_cnt = 0;
                    remove( "adc_lpc1768.bin" );
                }                
            }
            else
            {
                asse_cnt = 0;
                led1=0;
            }
        }
    }
    
}

