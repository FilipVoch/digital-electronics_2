# Projekt 

### Team members

* David Hromas
* Filip Voch
* Richard Ladislav

### Table of contents


* [Hardware description](#hardware)
* [Software description](#software)
* [Video](#video)
* [References](#references)





## Hardware description
![Arduino Uno](https://github.com/davidhro/digital-electronics_2/blob/main/Project_1/pictures/Arduino_uno_pinout.png)

### PS2 Joy Stick
Stick je prstom ovládaný modul pozostávajúci z dvoch potenciometrov o veľkosti 10kΩ a jedného tlačidla. Potenciometre indikujú súradnice smeru do ktorého je páčka vychýlená a tlačidlo vysiela low signál
V stave pokoja, keď nie je s Joy Stickom manipulované nadobúda výstupné napätie X i Y hodnotu 2,5V a následným pohybom meníme rozsah odporu potenciometrov v rozsahu 0Ω až 10kΩ, čo mení výstupné napätie modulu v rozmedzí 0V až 5V podľa smeru výchylky. Toto napätie je následne Arduinom interpretované ako celočíselná hodnota pohybujúca sa medi  0 až 1023.

![your figure](https://github.com/davidhro/digital-electronics_2/blob/main/Project_1/pictures/Joystick_pinout.png)
![JoyStickSmerz](https://user-images.githubusercontent.com/99683944/205034556-a86ae40a-1e32-45dd-8896-14e67f004670.png)
![joystickDobre](https://user-images.githubusercontent.com/99683944/205034636-049df2af-aab6-4d67-9917-56e2f35a5b18.png)

### LCD display

![your figure](https://github.com/davidhro/digital-electronics_2/blob/main/Project_1/pictures/LCD_pinout.png)
![LCD_pinout](https://user-images.githubusercontent.com/99683944/205039905-d8dde4cb-79b3-4df0-93b8-fbf0b47b5101.png)


### Rotačný enkodér
Rotačný enkodér je typ pozičného senzoru, používaný na detekciu pohybu na základe zmeny uhlu pri jeho otáčaní. Jeho veľkou výhodou oproti bežnému rotačnému potenciometru založenom na princípe zmeny odporu je fakt, že dokáže zaznamenať tridsať zmien polohy na otáčku, pričom jeho rozsah nie je obmedzený na 360 stupňov, ale je možné ho otáčať v podstate neobmedzene. Toto je realizované vďaka dômyselnému systému založenom na rotačnom disku obsahujúcom rovnomerne rozložené kontaktné plochy pripojené na spoločný pin C a dva separátne piny A a B.
![emnkoder princip](https://user-images.githubusercontent.com/99683944/205701989-74b76092-c505-4c1f-9460-7877281fd28f.png)
Enkodér generuje dva obdĺžnikové signály nazývané clock (pin CLK) a data (pin DT), ktoré sú generované vždy keď je uzavretý kontakt s kontaktnou plochou. To znamená, že na samotnú detekciu rotácie enkodéru stačí, jeden zo signálov, no v praktickom využití skoro vždy potrebujeme určiť i smer rotácie. Princíp na ktorom, je toto určovanie založené vychádza z faktu, že pri otáčaní senzoru v smere hodinových ručičiek predbieha clock signál data signál o fázu 90 stupňov a naopak.
![encoderSignaly](https://user-images.githubusercontent.com/99683944/205702402-56fd4a53-ed80-4a50-87b9-a4a589fb50a1.png)
Čip ATmega328P, ktorým je doska Arduina UNO osadená následne túto zmenu fáze deteguje a podľa toho rozhoduje o inkrempentácii, či dekrementácii premennej. Tieto vlastnosti robia z rotačného enkodéru ideálnu súčiastku pre projekty, v ktorých je potreba jednoduchá orientácia v menu, ovládanie servomotorov a iné aplikácie, kde 360 stupňový rozsah rotačných potenciometrov nevystačí. 
![encoder](https://user-images.githubusercontent.com/99683944/205036515-bb7ead21-4897-4920-b1be-1d6879a7a050.png)

### Finálny pinout projektu

![projekt1_finalPinou](https://user-images.githubusercontent.com/99683944/205702651-b4dd97b1-2299-4d3a-82a2-d01c4f0b3459.png)


## Software description

### Funkce TimerOVF_vect
 ```c
   static uint8_t channel = 0;
   if (channel == 0)
   {
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    channel = 1;
   }
   else
   {
    ADMUX &= ~((1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    ADMUX |= (1 << MUX0);
    channel = 0;
   }
    
   // Start ADC conversion
    ADCSRA |= (1 << ADSC);

   ```
![flowchartADMUX](https://user-images.githubusercontent.com/99683944/205927797-109ba625-1778-44eb-b8ba-52f494f3f20b.png)

### Funkce ADC_vect
 ```c
    static uint8_t channel = 0;
    uint16_t value;
   
    char string[4];  // String for converted numbers by itoa()
   
    //Cursors, defined as volatile variable
   
    if (channel == 0) //osa x
    {
     value = ADC;
     if (value > 600)
     {
     CursorX++;
     if (CursorX == 2)
     {
       CursorX = 3;
     }
     else if (CursorX == 5)
     {
       CursorX = 6;
     }
     else if (CursorX > 6)
     {
       CursorX = 0;
     }
         
     lcd_gotoxy(CursorX, CursorY);
     }
    else if (value < 400)
    {
     CursorX--;
     if (CursorX == 5)
     {
       CursorX = 4;
     }
     else if (CursorX == 2)
     {
       CursorX = 1;
     }
     else if (CursorX < 0)
     {
       CursorX = 6;
     }
     lcd_gotoxy(CursorX, CursorY);
    }
    else
    {
     lcd_gotoxy(CursorX, CursorY);
    }
     channel = 1;
   }
   else if (channel == 1) //osa y
   {
    value = ADC;
     if (value > 600)
     {
      CursorY != CursorY;
      lcd_gotoxy(CursorX, CursorY);
      }
     else if (value < 400)
     {
     CursorY != CursorY;
     lcd_gotoxy(CursorX, CursorY);
     }
     else
     {
     lcd_gotoxy(CursorX, CursorY);
     }
     channel = 0;
   }
   ```
![flowchartJoystick](https://github.com/davidhro/digital-electronics_2/blob/main/Project_1/pictures/flowchart_adc.png)

### Funkce SetTimer
```c
   if (buttonVal == 0)
    {
       if (CursorX = 0 && CursorY = 0 )
      {
        minutes = minutes + 10;
       
      }
       
       else if (CursorX = 1 && CursorY = 0 )
      {
        minutes ++;
       
      } 
       else if (CursorX = 3 && CursorY = 0 )
      {
        seconds = seconds + 10;
       
      } 
       else if (CursorX = 4 && CursorY = 0 )
      {
        seconds ++;
       
      } 
    }
```
![flowchartSetTimer](https://user-images.githubusercontent.com/99683944/205927833-6f11ed96-a3ac-4330-87e5-f5cb927f9fc3.png)





## Video


## References

1. http://www.handsontec.com/dataspecs/accessory/PS2-Joystick.pdf
2. https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/
3. https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
4. https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf
5. https://app.code2flow.com/
