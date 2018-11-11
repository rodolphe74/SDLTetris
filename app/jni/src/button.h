//
// Created by rodolphe on 19/09/18.
//

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#ifndef SDLTETRIS_BUTTON_H
#define SDLTETRIS_BUTTON_H


class button {
private:
    // The attributes of the button
    SDL_Rect box;

    // The part of the button sprite sheet that will be shown
    SDL_Rect* clip;

    // Font
    TTF_Font *font = NULL;

    // Text
    std::string text;

    int pushed = 0;

public:
    //Initialize the variables
    button(int x, int y, int w, int h) {
        //Set the button's attributes
        box.x = x;
        box.y = y;
        box.w = w;
        box.h = h;
        this->font = NULL;
        text = "";
        pushed = 0;
    }

    button(int x, int y, int w, int h, TTF_Font *font, std::string text) {
        box.x = x;
        box.y = y;
        box.w = w;
        box.h = h;
        this->font = font;
        this->text = text;
        pushed = 0;
    }

    //Handles events and set the button's sprite region
    void handle_events(SDL_Event &event, SDL_Rect screenRect, int (*function)(void));

    //Shows the button on the screen
    void show(SDL_Renderer *renderer);

    void setPushed(int v) {
        pushed = v;
    }

    int isPushed() {
        return pushed;
    }
};


#endif //SDLTETRIS_BUTTON_H
