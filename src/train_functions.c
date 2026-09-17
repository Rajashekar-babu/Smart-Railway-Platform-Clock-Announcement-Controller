#include <LPC21xx.h>
#include "train.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"

void DisplayTrain(int index);
void ScrollTrainName(int index);
int IsTrainUpcoming(int index, int currentHour, int currentMinute);

extern volatile unsigned char admin_mode;


// Checks whether the train will arrive exactly one minute from now
int IsTrainUpcoming(int index, int currentHour, int currentMinute)
{
    int currentTime;
    int nextTime;
    int trainTime;

    currentTime = currentHour * 60 + currentMinute;

    nextTime = (currentTime + 1) % 1440;

    trainTime = TrainDB[index].updatedArrivalHour * 60
              + TrainDB[index].updatedArrivalMinute;

    if(trainTime == nextTime)
        return 1;

    return 0;
}


// Displays train number, platform and arrival time
void DisplayTrain(int index)
{
    if(index < 0 || index >= TOTAL_TRAINS)
        return;

    CmdLCD(CLEAR_LCD);

    // Line 1

    U32LCD(TrainDB[index].trainNumber);

    CharLCD(' ');

    CharLCD('P');
    CharLCD(TrainDB[index].platform + '0');

    CharLCD(' ');

    CharLCD('A');
    CharLCD(':');

    CharLCD((TrainDB[index].updatedArrivalHour / 10) + '0');
    CharLCD((TrainDB[index].updatedArrivalHour % 10) + '0');

    CharLCD(':');

    CharLCD((TrainDB[index].updatedArrivalMinute / 10) + '0');
    CharLCD((TrainDB[index].updatedArrivalMinute % 10) + '0');

    // Line 2

    CmdLCD(GOTO_LINE2_POS0);

    ScrollTrainName(index);

    CmdLCD(CLEAR_LCD);
}


// Scrolls the train name on line 2
// Departure time remains fixed on the right side
void ScrollTrainName(int index)
{
    int j;
    int k;
    int len;
    char text[60];

    // Create train name - destination
    len = 0;

    for(j = 0; TrainDB[index].trainName[j] != '\0'; j++)
    {
        text[len++] = TrainDB[index].trainName[j];
    }

    text[len++] = ' ';
    text[len++] = '-';
    text[len++] = ' ';

    for(j = 0; TrainDB[index].destination[j] != '\0'; j++)
    {
        text[len++] = TrainDB[index].destination[j];
    }

    text[len] = '\0';


    // Scroll train name and destination
    for(j = 0; j < len; j++)
    {
        CmdLCD(GOTO_LINE2_POS0);

        // Display 8 characters of train information
        for(k = 0; k < 8; k++)
        {
            if((j + k) < len)
                CharLCD(text[j + k]);
            else
                CharLCD(' ');
        }

        // Display departure time
        CharLCD(' ');
        CharLCD('D');
        CharLCD(':');

        CharLCD((TrainDB[index].departureHour / 10) + '0');
        CharLCD((TrainDB[index].departureHour % 10) + '0');

        CharLCD(':');

        CharLCD((TrainDB[index].departureMinute / 10) + '0');
        CharLCD((TrainDB[index].departureMinute % 10) + '0');

        delay_ms(300);
    }
}
