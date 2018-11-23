/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, standard IO, and, strings
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <deque>
#include <random>
#include <vector>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include "board.h"
#include "button.h"


// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Screen dimensions
SDL_Rect gScreenRect = {0, 0, 600, 600};

// Queue holding input events
// std::queue<SDL_Event> gEventsQueue;
std::deque<SDL_Event> gEventsDeque;

SDL_Texture *gMainTexture;

TTF_Font *gFont;
TTF_Font *gBigFont;
TTF_Font *gArrows;

Mix_Music *gMusic;
Mix_Chunk *gRotateLeft;
Mix_Chunk *gRotateRight;
Mix_Chunk *gChirp;
Mix_Chunk *gHit;
Mix_Chunk *gJump;
Mix_Chunk *gZap;
Mix_Chunk *gNoise;
Mix_Chunk **sounds;

SDL_Surface *gTetrisLogo;
SDL_Texture **images;
/*
SDL_Texture *gFramerateTexture;
SDL_Rect gFramerateSize;
*/

/*
Uint32 gTimerStart, gTimerCurrent;
Uint32 gFrameCount = 0;
std::stringstream gTimeText;
*/

SDL_Event gEvent;
int gCanSlideOrRotate = 0;
int gCurrentTimerSpeed = 700;
int gLevelTimerSpeed = 700;
int gPlaySounds = 1;
int gLastAnim = none;


button *up, *down, *left, *right;
button *gMusicButton, *gSoundsButton, *gQuitButton, *gPauseButton;
std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
board gBoard;

uint32_t gScrollDownEventType = SDL_RegisterEvents(1);


bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else if( TTF_Init() == -1 ) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            SDL_Log("Warning: Linear texture filtering not enabled!");
        }

        // Get device display mode
        SDL_DisplayMode displayMode;
        if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
            gScreenRect.w = displayMode.w;
            gScreenRect.h = displayMode.h;
        }

        // Load fonts
        gFont = TTF_OpenFont( "AtariST8x16SystemFont.ttf", 44 );
        if( gFont == NULL ) {
            printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
            return success;
        }

        gBigFont = TTF_OpenFont( "AtariST8x16SystemFont.ttf", 50 );
        if( gBigFont == NULL ) {
            printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
            return success;
        }

        gArrows = TTF_OpenFont( "PixArrows.ttf", 60 );
        if( gArrows == NULL ) {
            printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
            return success;
        }


        // Load music
        gMusic = Mix_LoadMUS( "maintheme.ogg" );
        if( gMusic == NULL ) {
            printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
            success = false;
        }

        // Load sound effects
        gRotateLeft = Mix_LoadWAV("rotateleft.wav");
        gRotateRight = Mix_LoadWAV("rotateright.wav");
        gChirp = Mix_LoadWAV("chirp.wav");
        gHit = Mix_LoadWAV("hit.wav");
        gJump = Mix_LoadWAV("jump.wav");
        gZap = Mix_LoadWAV("zap.wav");
        gNoise = Mix_LoadWAV("noise.wav");
        sounds = (Mix_Chunk **) malloc(7 * sizeof(Mix_Chunk *));
        sounds[0] = gRotateLeft;
        sounds[1] = gRotateRight;
        sounds[2] = gChirp;
        sounds[3] = gHit;
        sounds[4] = gJump;
        sounds[5] = gZap;
        sounds[6] = gNoise;
        gBoard.setSounds(sounds);

        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, gScreenRect.w,
                                   gScreenRect.h, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(
                            gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                SDL_Log("Renderer could not be created! SDL Error: %s\n",
                        SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                gMainTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, gScreenRect.w, gScreenRect.h);

                // Load images
                gTetrisLogo = IMG_Load("logo_tetris.jpg");
                images = (SDL_Texture **) malloc(1 * sizeof(SDL_Texture *));
                images[0] = SDL_CreateTextureFromSurface(gRenderer, gTetrisLogo);
                gBoard.setImages(images);
                SDL_FreeSurface(gTetrisLogo);
            }
        }
    }

    return success;
}




void close() {

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    //Free global font
    TTF_CloseFont(gFont);
    TTF_CloseFont(gBigFont);
    TTF_CloseFont(gArrows);
    gFont = NULL;

    // Free music
    Mix_FreeMusic(gMusic);
    Mix_FreeChunk(gRotateLeft);
    Mix_FreeChunk(gRotateRight);
    Mix_FreeChunk(gChirp);
    Mix_FreeChunk(gHit);
    Mix_FreeChunk(gJump);
    Mix_FreeChunk(gZap);
    free(sounds);
    SDL_DestroyTexture(images[0]);
    free(images);

    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}



Uint32 callbackTimerScrollDownCurrentShape( Uint32 interval, void* param ) {

    // Add a scroll down event in the queue
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = gScrollDownEventType;
    event.user.code = 1;
    event.user.data1 = 0;
    event.user.data2 = 0;

    // prioritize scrolldown event by putting it at the front of the queue
    gEventsDeque.push_front(event);

    return gCurrentTimerSpeed;
}

int doShapeFall() {
    if (gPlaySounds) {
        Mix_PlayChannel(-1, sounds[5], 0);
    }
    gCurrentTimerSpeed = 50;
    gCanSlideOrRotate = 0;
    return 0;
}

int shapeLeft() {
    gBoard.moveCurrentShapeLeft();
    gCanSlideOrRotate = 0;
    return 0;
}

int shapeRight() {
    gBoard.moveCurrentShapeRight();
    gCanSlideOrRotate = 0;
    return 0;
}

int rotateShape() {
    gBoard.rotateCurrentShape();
    gCanSlideOrRotate = 0;
    return 0;
}

int toggleMusic() {
    if (gMusicButton->isPushed()) {
        gMusicButton->setPushed(0);
        Mix_PauseMusic();
    } else {
        gMusicButton->setPushed(1);
        Mix_ResumeMusic();
    }
    return 0;
}

int toggleSounds() {
    if (gSoundsButton->isPushed()) {
        gSoundsButton->setPushed(0);
        gPlaySounds = 0;
        gBoard.setPlaySounds(gPlaySounds);
    } else {
        gSoundsButton->setPushed(1);
        Mix_Resume(-1);
        gPlaySounds = 1;
        gBoard.setPlaySounds(gPlaySounds);
    }
    return 0;
}

void handleInput() {

    if (gEvent.type != SDL_FINGERUP && gEvent.type != SDL_FINGERDOWN && gEvent.type != SDL_FINGERMOTION) {
        return;
    }

    gEventsDeque.push_back(gEvent);
}


void doMoves() {


    if (gBoard.animate != none) {
        return;
    }

    // process event queue
    if (!gEventsDeque.empty()) {
        SDL_Event queueEvent = gEventsDeque.front();
        gEventsDeque.pop_front();

        gMusicButton->handle_events(queueEvent, gScreenRect, toggleMusic);
        gSoundsButton->handle_events(queueEvent, gScreenRect, toggleSounds);




        if (queueEvent.type == gScrollDownEventType) {
            // manage shape scroll with a specific event
            int collision = gBoard.scrollDownCurrentShape();
            if (collision) {

                // empty queue
                while (!gEventsDeque.empty()) {
                    gEventsDeque.pop_front();
                }

                // Random next Shape
                // int x = rand() % 7;


                std::random_device random_dev;
                std::mt19937 generator(random_dev());
                std::shuffle(vec.begin(), vec.end(), generator);
                int x = vec.at(rand() % 7);

                gBoard.playWithShape(x);

                gBoard.setLinesDeleted(0);

                gBoard.checkLines();
                gCurrentTimerSpeed = gLevelTimerSpeed;


            }
        } else {
            // user input event
            left->handle_events(queueEvent, gScreenRect, shapeLeft);
            right->handle_events(queueEvent, gScreenRect, shapeRight);
            up->handle_events(queueEvent, gScreenRect, rotateShape);
            down->handle_events(queueEvent, gScreenRect, doShapeFall);

            if (queueEvent.type == SDL_FINGERDOWN) {
                gCanSlideOrRotate = 1;
            }

            if (gCanSlideOrRotate && queueEvent.type == SDL_FINGERMOTION) {
                if (queueEvent.tfinger.dx >= .05) {
                    shapeRight();
                } else if (queueEvent.tfinger.dx <= -.05) {
                    shapeLeft();
                } else if (queueEvent.tfinger.dy >= .05) {
                    doShapeFall();
                } else if (queueEvent.tfinger.dy <= -.05) {
                    rotateShape();
                }
            }
        }



        // Update score after scrollDown events (which means that lines where deleted)
        if (gLastAnim == scrollLine && gBoard.animate != scrollLine) {
            // Check how many line where deleted and compute score
            printf("last anim:%d", gBoard.animate);

            int multiplier = 0;
            switch (gBoard.getLinesDeleted()) {
                case 1 :
                    multiplier = 1;
                    break;
                case 2 :
                    multiplier = 4;
                    break;
                case 3 :
                    multiplier = 16;
                    break;
                case 4 :
                    multiplier = 64;
                    break;
                default:
                    multiplier = 0;
            }
            int points = 1000 * multiplier;
            gBoard.setScore(gBoard.getScore() + points);
            gBoard.setLinesDeleted(0);

            // update Level and Speed
            gBoard.setLevel(gBoard.getLines() / 10);
            gLevelTimerSpeed = gLevelTimerSpeed - (gBoard.getLevel() * 50);
            if (gLevelTimerSpeed <= 200) {
                gLevelTimerSpeed = 200;
            }
        }
        gLastAnim = gBoard.animate;
    }
}


void gameLoop() {

    SDL_Delay(1000);

    gBoard.setScreenRect(gScreenRect);
    gBoard.setRenderer(gRenderer);
    gBoard.setMainTexture(gMainTexture);
    gBoard.setWindow(gWindow);
    gBoard.setFont(gFont);
    gBoard.setBigFont(gBigFont);

    SDL_TimerID timerScrollDownCurrentShape = SDL_AddTimer(gCurrentTimerSpeed, callbackTimerScrollDownCurrentShape, (void *) "1 seconds waited!");

    left = new button(gScreenRect.w / 2 - 100 - 40, gScreenRect.h - 160, 80, 80, gArrows, "|");
    right = new button(gScreenRect.w / 2 + 100 - 40, gScreenRect.h - 160, 80, 80, gArrows, "{");
    up = new button(gScreenRect.w / 2 - 40, gScreenRect.h - 240, 80, 80, gArrows, "}");
    down = new button(gScreenRect.w / 2 - 40, gScreenRect.h - 90, 80, 80, gArrows, "~");

    gMusicButton = new button((gScreenRect.w / 3) * 2 + 45, 1300, 140, 80, gFont, "Music");
    gMusicButton->setPushed(1);

    gSoundsButton = new button((gScreenRect.w / 3) * 2 + 210, 1300, 140, 80, gFont, "Sounds");
    gSoundsButton->setPushed(1);

    if(Mix_PlayingMusic() == 0) {
        Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
        Mix_PlayMusic(gMusic, -1);
    }

    int quit = 0;
    while (!quit) {

        // events
        while (SDL_PollEvent(&gEvent)) {
            handleInput();
            if (gEvent.type == SDL_QUIT)
                quit = 1;
        }

        // process events
        doMoves();

        // rendering
        SDL_SetRenderTarget(gRenderer, gMainTexture);
        SDL_SetRenderDrawColor(gRenderer, 0x35, 0x35, 0x35, 0x00);

        SDL_RenderClear(gRenderer);
        gBoard.render(/*gRenderer, */ /*gScreenRect,*/ /*gFont*/);

        left->show(gRenderer);
        right->show(gRenderer);
        up->show(gRenderer);
        down->show(gRenderer);

        gMusicButton->show(gRenderer);
        gSoundsButton->show(gRenderer);

        SDL_SetRenderTarget(gRenderer, NULL);
        SDL_RenderCopy(gRenderer, gMainTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);


        if (gBoard.checkGameOver()) {
            quit = 1;
        }

        // wait vsync
    }
    SDL_RemoveTimer(timerScrollDownCurrentShape);

    // Do a game over animation
    gBoard.animate = gameOver;
    while (1) {
        SDL_SetRenderTarget(gRenderer, gMainTexture);
        SDL_SetRenderDrawColor(gRenderer, 0x35, 0x35, 0x35, 0x00);
        SDL_RenderClear(gRenderer);
        gBoard.render();
        SDL_SetRenderTarget(gRenderer, NULL);
        SDL_RenderCopy(gRenderer, gMainTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
    }

    // Free resources and close SDL
    close();
}

int main(int argc, char *args[]) {
    // Start up SDL and create window
    if (!init()) {
        SDL_Log("Failed to initialize!\n");
        return -1;
    } else {
        gameLoop();

        return 0;
    }
}
