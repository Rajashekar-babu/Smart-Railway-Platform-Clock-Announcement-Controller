#include <LPC21xx.H>

#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"
#include "train.h"
#include "KPM.h"
#include "types.h"


#define EINT0_VIC_CHNO    14

#define NORMAL_MODE       0
#define ADMIN_MODE        1

#define ORANGE_LED   (1 << 19)
#define GREEN_LED    (1 << 20)
#define RED_LED      (1 << 21)

#define ALL_LEDS     (ORANGE_LED | GREEN_LED | RED_LED)


int CalculateDOW(int day, int month, int year);
void eint0_isr(void) __irq;


// Admin mode flag
volatile unsigned char admin_mode = NORMAL_MODE;


// Days of week
char week[][4] =
{
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};


// Train database
TrainInfo_t TrainDB[TOTAL_TRAINS] =
{
	 //train number, train name                  , destination     , arrival , travel t, departure , updated arrival time, platform, delay
    {11019       , "Kornak Express"            , "Machilipatnam" , 3, 40   , 5, 0    , 5, 2      , 3, 40               , 1       , 0},
    {17222       , "Kakinada Port Express"     , "Kakinada"      , 0, 0    , 2, 0    , 5, 40     , 0, 0                , 2       , 0},
    {12806       , "Visakhapatnam Vande Bharat", "Visakhapatnam" , 0, 0    , 3, 0    , 16, 55    , 0, 0                , 3       , 0}
};


int main()
{
    int index;
    int train_no;
    int i;

    int hr;
    int min;
    int sec;

    int totalMinutes;
    int platform;

    int previousDOM;
    int previousMONTH;
    int previousYEAR;

    char key;

	
		IODIR0 |= ALL_LEDS;
			IOCLR0 = ALL_LEDS;

    // Stop RTC before configuration
    CCR = 1 << 1;


    // RTC prescaler values
    PREINT = 456;
    PREFRAC = 25024;


    // Start RTC
    CCR = 1 << 0;


    // Initial RTC time
    HOUR = 23;
    MIN  = 58;
    SEC  = 55;


    // Initial RTC date
    YEAR = 2026;


    // Calculate day of week
    DOW = CalculateDOW(DOM, MONTH, YEAR);


    // Store current date
    previousDOM   = DOM;
    previousMONTH = MONTH;
    previousYEAR  = YEAR;


    // Initialize LCD
    InitLCD();


    // Initialize keypad
    Init_KPM();
		
		
		// LED pins as GPIO outputs
		IODIR0 |= ALL_LEDS;
		
		// All LEDs OFF initially
		IOCLR0 = ALL_LEDS;


    // Configure LED pins as GPIO outputs
    IODIR0 |= (1 << 19) | (1 << 20) | (1 << 21);


    // Turn all LEDs OFF
    IOCLR0 = (1 << 19) | (1 << 20) | (1 << 21);


    // EINT0 - P0.1

    PINSEL0 &= ~(3 << 2);
    PINSEL0 |=  (3 << 2);


    // Clear external interrupt flag
    EXTINT = 1 << 0;


    // Configure EINT0 as edge triggered
    EXTMODE |= 1 << 0;


    // Falling edge
    EXTPOLAR &= ~(1 << 0);


    // Configure EINT0 as IRQ
    VICIntSelect &= ~(1 << EINT0_VIC_CHNO);


    // Configure VIC vector
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr0 = (unsigned long)eint0_isr;


    // Enable EINT0 interrupt
    VICIntEnable |= (1 << EINT0_VIC_CHNO);


    while(1)
    {

        // Reset train data when a new date starts
        if(DOM != previousDOM ||
           MONTH != previousMONTH ||
           YEAR != previousYEAR)
        {

            for(i = 0; i < TOTAL_TRAINS; i++)
            {

                // Restore original arrival time
                TrainDB[i].updatedArrivalHour =
                    TrainDB[i].arrivalHour;

                TrainDB[i].updatedArrivalMinute =
                    TrainDB[i].arrivalMinute;


                // Calculate original departure time
                totalMinutes =
                    (TrainDB[i].arrivalHour * 60)
                    + TrainDB[i].arrivalMinute;


                totalMinutes +=
                    (TrainDB[i].travelHour * 60)
                    + TrainDB[i].travelMinute;


                TrainDB[i].departureHour =
                    (totalMinutes / 60) % 24;


                TrainDB[i].departureMinute =
                    totalMinutes % 60;
            }


            // Update day of week
            DOW = CalculateDOW(DOM, MONTH, YEAR);


            // Store current date
            previousDOM   = DOM;
            previousMONTH = MONTH;
            previousYEAR  = YEAR;
        }


        // ADMIN MODE
        if(admin_mode == ADMIN_MODE)
        {
					
					IOSET0 = RED_LED;
					IOCLR0 = ORANGE_LED | GREEN_LED;
					

            CmdLCD(CLEAR_LCD);

            StrLCD("1.TRAIN 2.DATE");

            CmdLCD(GOTO_LINE2_POS0);

            StrLCD("3.TIME  4.EXIT");


            key = keyscan();


            switch(key)
            {

                // TRAIN EDIT
                case '1':

                    CmdLCD(CLEAR_LCD);

                    StrLCD("EDIT TRAIN No:");

                    CmdLCD(GOTO_LINE2_POS0);

                    train_no = ReadNum();


                    index = -1;


                    // Search train number
                    for(i = 0; i < TOTAL_TRAINS; i++)
                    {
                        if(TrainDB[i].trainNumber == train_no)
                        {
                            index = i;
                            break;
                        }
                    }


                    // Train not found
                    if(index == -1)
                    {
                        CmdLCD(CLEAR_LCD);

                        StrLCD("TRAIN NOT FOUND");

                        delay_ms(1000);
                    }


                    // Train found
                    else
                    {

                        // Enter arrival hour
                        CmdLCD(CLEAR_LCD);

                        StrLCD("ENTER HOURS:");

                        CmdLCD(GOTO_LINE2_POS0);

                        hr = ReadNum();


                        if(hr >= 0 && hr < 24)
                        {

                            // Enter arrival minute
                            CmdLCD(CLEAR_LCD);

                            StrLCD("ENTER MINUTES:");

                            CmdLCD(GOTO_LINE2_POS0);

                            min = ReadNum();


                            if(min >= 0 && min < 60)
                            {

                                // Update arrival time
                                TrainDB[index].updatedArrivalHour = hr;

                                TrainDB[index].updatedArrivalMinute = min;


                                // Calculate new departure time
                                totalMinutes =
                                    (hr * 60) + min;


                                totalMinutes +=
                                    (TrainDB[index].travelHour * 60)
                                    + TrainDB[index].travelMinute;


                                TrainDB[index].departureHour =
                                    (totalMinutes / 60) % 24;


                                TrainDB[index].departureMinute =
                                    totalMinutes % 60;


                                // Display departure time
                                CmdLCD(CLEAR_LCD);

                                StrLCD("DEPARTURE:");

                                CmdLCD(GOTO_LINE2_POS0);


                                CharLCD(
                                    (TrainDB[index].departureHour / 10)
                                    + '0'
                                );

                                CharLCD(
                                    (TrainDB[index].departureHour % 10)
                                    + '0'
                                );

                                CharLCD(':');


                                CharLCD(
                                    (TrainDB[index].departureMinute / 10)
                                    + '0'
                                );

                                CharLCD(
                                    (TrainDB[index].departureMinute % 10)
                                    + '0'
                                );


                                delay_ms(1500);


                                // Arrival time updated
                                CmdLCD(CLEAR_LCD);

                                StrLCD("TIME UPDATED");

                                delay_ms(1000);


                                // Enter platform
                                CmdLCD(CLEAR_LCD);

                                StrLCD("ENTER PLATFORM:");

                                CmdLCD(GOTO_LINE2_POS0);

                                platform = ReadNum();


                                if(platform >= 1 && platform <= 3)
                                {

                                    TrainDB[index].platform =
                                        platform;


                                    CmdLCD(CLEAR_LCD);

                                    StrLCD("PLATFORM UPDATED");

                                    delay_ms(1000);
                                }

                                else
                                {

                                    CmdLCD(CLEAR_LCD);

                                    StrLCD("INVALID PLATFORM");

                                    delay_ms(1000);
                                }

                            }

                            else
                            {

                                CmdLCD(CLEAR_LCD);

                                StrLCD("INVALID MINUTE");

                                delay_ms(1000);
                            }
                        }

                        else
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("INVALID HOUR");

                            delay_ms(1000);
                        }
                    }

                    break;


                // DATE EDIT
                case '2':

                    CmdLCD(CLEAR_LCD);

                    StrLCD("ENTER DAY:");

                    CmdLCD(GOTO_LINE2_POS0);

                    DOM = ReadNum();


                    CmdLCD(CLEAR_LCD);

                    StrLCD("ENTER MONTH:");

                    CmdLCD(GOTO_LINE2_POS0);

                    MONTH = ReadNum();


                    CmdLCD(CLEAR_LCD);

                    StrLCD("ENTER YEAR:");

                    CmdLCD(GOTO_LINE2_POS0);

                    YEAR = ReadNum();


                    // Validate month
                    if(MONTH < 1 || MONTH > 12)
                    {

                        CmdLCD(CLEAR_LCD);

                        StrLCD("INVALID MONTH");

                        delay_ms(1000);

                        break;
                    }


                    // Validate February
                    if(MONTH == 2)
                    {

                        if(DOM < 1 || DOM > 29)
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("INVALID DATE");

                            delay_ms(1000);

                            break;
                        }


                        // Check leap year
                        if(DOM == 29)
                        {

                            if((YEAR % 400 != 0) &&
                               ((YEAR % 100 == 0) ||
                                (YEAR % 4 != 0)))
                            {

                                CmdLCD(CLEAR_LCD);

                                StrLCD("INVALID DATE");

                                delay_ms(1000);

                                break;
                            }
                        }
                    }


                    // Validate 30-day months
                    else if(MONTH == 4 ||
                            MONTH == 6 ||
                            MONTH == 9 ||
                            MONTH == 11)
                    {

                        if(DOM < 1 || DOM > 30)
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("INVALID DATE");

                            delay_ms(1000);

                            break;
                        }
                    }


                    // Validate 31-day months
                    else
                    {

                        if(DOM < 1 || DOM > 31)
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("INVALID DATE");

                            delay_ms(1000);

                            break;
                        }
                    }


                    // Calculate day of week
                    DOW = CalculateDOW(DOM, MONTH, YEAR);


                    // Store updated date
                    previousDOM   = DOM;
                    previousMONTH = MONTH;
                    previousYEAR  = YEAR;


                    CmdLCD(CLEAR_LCD);

                    StrLCD("DATE UPDATED");

                    delay_ms(1000);

                    break;


                // TIME EDIT
                case '3':

                    CmdLCD(CLEAR_LCD);

                    StrLCD("ENTER HOURS:");

                    CmdLCD(GOTO_LINE2_POS0);

                    hr = ReadNum();


                    if(hr >= 0 && hr < 24)
                    {

                        CmdLCD(CLEAR_LCD);

                        StrLCD("ENTER MINUTES:");

                        CmdLCD(GOTO_LINE2_POS0);

                        min = ReadNum();


                        if(min >= 0 && min < 60)
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("ENTER SECONDS:");

                            CmdLCD(GOTO_LINE2_POS0);

                            sec = ReadNum();


                            if(sec >= 0 && sec < 60)
                            {

                                // Stop RTC
                                CCR = 1 << 1;


                                // Update time
                                HOUR = hr;
                                MIN  = min;
                                SEC  = sec;


                                // Start RTC
                                CCR = 1 << 0;


                                CmdLCD(CLEAR_LCD);

                                StrLCD("TIME UPDATED");

                                delay_ms(1000);
                            }

                            else
                            {

                                CmdLCD(CLEAR_LCD);

                                StrLCD("INVALID SECOND");

                                delay_ms(1000);
                            }
                        }

                        else
                        {

                            CmdLCD(CLEAR_LCD);

                            StrLCD("INVALID MINUTE");

                            delay_ms(1000);
                        }
                    }

                    else
                    {

                        CmdLCD(CLEAR_LCD);

                        StrLCD("INVALID HOUR");

                        delay_ms(1000);
                    }

                    break;


                // EXIT ADMIN MODE
                case '4':

                    admin_mode = NORMAL_MODE;

                    CmdLCD(CLEAR_LCD);

                    break;
            }
        }


        // NORMAL MODE
        if(admin_mode == NORMAL_MODE)
        {

					IOSET0 = ORANGE_LED;
					IOCLR0 = GREEN_LED | RED_LED;
          
					// Display time
            CmdLCD(GOTO_LINE1_POS0);


            CharLCD((HOUR / 10) + '0');
            CharLCD((HOUR % 10) + '0');

            CharLCD(':');


            CharLCD((MIN / 10) + '0');
            CharLCD((MIN % 10) + '0');

            CharLCD(':');


            CharLCD((SEC / 10) + '0');
            CharLCD((SEC % 10) + '0');


            StrLCD("  ");


            // Display day
            CmdLCD(GOTO_LINE1_POS0 + 10);

            StrLCD(week[DOW]);

            StrLCD("   ");


            // Display date
            CmdLCD(GOTO_LINE2_POS0);


            CharLCD((DOM / 10) + '0');
            CharLCD((DOM % 10) + '0');

            CharLCD('/');


            CharLCD((MONTH / 10) + '0');
            CharLCD((MONTH % 10) + '0');

            CharLCD('/');


            U32LCD(YEAR);

            StrLCD("      ");
        }


        // TRAIN ANNOUNCEMENT
        for(i = 0; i < TOTAL_TRAINS; i++)
				{
						if(IsTrainUpcoming(i, HOUR, MIN))
						{
								IOSET0 = GREEN_LED;
								IOCLR0 = ORANGE_LED | RED_LED;

								DisplayTrain(i);

								IOCLR0 = GREEN_LED;
						}
				}
    }
}


// EINT0 interrupt service routine
void eint0_isr(void) __irq
{
    admin_mode = ADMIN_MODE;


    // Clear EINT0 interrupt
    EXTINT = 1 << 0;


    // End interrupt
    VICVectAddr = 0;
}


// Calculate day of week
int CalculateDOW(int day, int month, int year)
{
    int t[] =
    {
        0, 3, 2, 5, 0, 3,
        5, 1, 4, 6, 2, 4
    };


    if(month < 3)
        year = year - 1;


    return
        (year + year / 4 - year / 100 + year / 400
        + t[month - 1] + day) % 7;
}
