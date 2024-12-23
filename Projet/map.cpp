#include "map.h"

int carte[32][32] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2},
    {3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3},
    {4, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},
    {4, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4},

};