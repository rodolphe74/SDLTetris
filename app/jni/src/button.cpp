//
// Created by rodolphe on 19/09/18.
//

#include "button.h"


void button::handle_events(SDL_Event &event, SDL_Rect screenRect, int (*function)(void)) {
    //The mouse offsets
    int x = 0, y = 0;


    //If button was pressed
    if (event.type == SDL_FINGERDOWN) {
        //Get the finger offsets
        x = event.tfinger.x * screenRect.w;
        y = event.tfinger.y * screenRect.h;

        //If the mouse is over the button
        if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h)) {
            (*function)();
        }
    }

    //If a mouse button was released
    if (event.type == SDL_FINGERUP) {
        //Get the finger offsets
        x = event.tfinger.x * screenRect.w;
        y = event.tfinger.y * screenRect.h;

        //If the mouse is over the button
        if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h)) {
            // Nothing
        }
    }
}

void button::show(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderFillRect(renderer, &box);
    // SDL_RenderDrawRect(renderer, &box);

    if (this->font) {
        SDL_Color textColor = { 220, 0, 0, 255 };
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture != NULL) {
            SDL_Rect textureRect;
            textureRect.w = textSurface->w;
            textureRect.h = textSurface->h;
            textureRect.x = box.x + (box.w - textureRect.w) / 2;
            textureRect.y = box.y + (box.h - textureRect.h) / 2;
            SDL_RenderCopy(renderer, texture, NULL, &textureRect);
        }
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(texture);
    }

    if (isPushed()) {
        SDL_Rect pushRect = { box.x - 5, box.y - 5, box.w + 10, box.h + 10 };
        SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0x00, 0xFF);
        SDL_RenderDrawRect(renderer, &pushRect);
        pushRect = { box.x - 6, box.y - 6, box.w + 12, box.h + 12 };
        SDL_RenderDrawRect(renderer, &pushRect);
        pushRect = { box.x - 7, box.y - 7, box.w + 14, box.h + 14 };
        SDL_RenderDrawRect(renderer, &pushRect);
    }
}
