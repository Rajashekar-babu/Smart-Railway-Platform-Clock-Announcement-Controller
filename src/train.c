#include "train.h"

TrainInfo_t TrainDB[TOTAL_TRAINS] =
{
    {
        11019,
        "Kornak Express",
        "Machilipatnam",

        0, 2,       // arrival
        0, 7,       // departure

        0, 2,       // updated arrival
        0, 7,       // updated departure

        1,          // platform
        0           // delay
    },

    {
        17222,
        "Kakinada Port Express",
        "Kakinada",

        3, 40,
        3, 45,

        3, 40,
        3, 45,

        2,
        0
    },

    {
        12806,
        "Visakhapatnam Vande Bharat",
        "Visakhapatnam",

        13, 55,
        14, 0,

        13, 55,
        14, 0,

        3,
        0
    }
};