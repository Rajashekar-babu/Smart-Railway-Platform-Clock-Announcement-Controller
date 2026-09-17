#define TOTAL_TRAINS 3

typedef struct
{
    unsigned int trainNumber;
    char trainName[30];
    char destination[20];

    unsigned char arrivalHour;
    unsigned char arrivalMinute;

    unsigned char travelHour;
    unsigned char travelMinute;

    unsigned char departureHour;
    unsigned char departureMinute;

    unsigned char updatedArrivalHour;
    unsigned char updatedArrivalMinute;

    unsigned char platform;
    unsigned char delayMinutes;

} TrainInfo_t;


// TrainDB is created in main.c
extern TrainInfo_t TrainDB[TOTAL_TRAINS];


// Train functions
int IsTrainUpcoming(int index, int currentHour, int currentMinute);
void DisplayTrain(int index);
void ScrollTrainName(int index);
