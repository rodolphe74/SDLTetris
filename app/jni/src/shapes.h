//
// Created by rodolphe on 12/09/18.
//

#ifndef SDLTETRIS_SHAPES_H
#define SDLTETRIS_SHAPES_H

#include <algorithm>
#include <SDL.h>

const SDL_Color gShapesColor[7] = { {0, 127, 4, 255},
                                    {246, 159, 0, 255},
                                    {163, 44, 38, 255},
                                    {254, 1, 0, 255},
                                    {5, 253, 255, 255},
                                    {6, 4, 237, 255},
                                    {250, 175, 190, 255} };

const float gRotationCenters[7][4][2] = { { { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
										  { { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
                                          { { 0.375, 0.375 }, { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.625, 0.625 } },
										  { { 0.500, 0.500 }, { 0.500, 0.500 }, { 0.500, 0.500 }, { 0.500, 0.500 } },
                                          { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 } },
                                          { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.625, 0.625 }, { 0.375, 0.375 } },
                                          { { 0.375, 0.375 }, { 0.625, 0.625 }, { 0.375, 0.375 }, { 0.625, 0.625 } } };

const int gShapesArray[7][4][16] = {
        {
                {       0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0
                },
                {       0, 0, 0, 0,
                        1, 1, 1, 1,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0
                },
                {       0, 0, 0, 0,
                        1, 1, 1, 1,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                }
        },
        {
                {       0, 1, 0, 0,
                        0, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 1, 0, 0,
                        1, 0, 0, 0,
                        1, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0
                }
        },
        {
                {       0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0
                },
                {       0, 0, 0, 0,
                        1, 1, 1, 0,
                        1, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 0, 1, 0,
                        1, 1, 1, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                }
        },
        {
                {       1, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                }
        },
        {
                {       0, 1, 1, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 1, 0,
                        1, 1, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       1, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                }
        },
        {
                {       0, 0, 0, 0,
                        1, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        1, 1, 1, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0
                }
        },
        {
                {
                        0, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        1, 1, 0, 0,
                        1, 0, 0, 0,
                        0, 0, 0, 0
                },
                {       0, 0, 0, 0,
                        1, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0
                },
                {       0, 1, 0, 0,
                        1, 1, 0, 0,
                        1, 0, 0, 0,
                        0, 0, 0, 0
                }
        }
};

class shapes {

public:

    shapes() {
    }

};


#endif //SDLTETRIS_SHAPES_H
