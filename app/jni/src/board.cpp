//
// Created by rodolphe on 12/09/18.
//

#include <SDL.h>
#include <string.h>
#include <string>
#include <sstream>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <algorithm>

#include "board.h"
#include "shapes.h"
// #include "button.h"

void board::playWithShape(int idx) {

    // Melt current shape with grid
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == -1) {
                // grid[i][j] = 1;
                grid[i][j] = currentShape + 1;  // use the color index of the current shape
            }
        }
    }

    currentShape = idx;
    currentShapeY = /*21*/22;
    currentShapeX = 5;
    formerShapeX = 5;
    formerShapeY = /*21*/22;
    currentShapeRotation = 0;

}

void board::copyCurrentShapeToGrid() {
    const int *shape = gShapesArray[currentShape][currentShapeRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[squarecount] == 1) {
                grid[currentShapeY + i][currentShapeX + j] = -1;
            }
            squarecount ++;
        }
    }
}

void board::deleteCurrentShapeFromGrid() {
    const int *shape = gShapesArray[currentShape][currentShapeRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shape[squarecount] == 1) {
                grid[currentShapeY + i][currentShapeX /*formerShapeX*/ + j] = 0;
            }
            squarecount ++;
        }
    }
}


bool board::getTextureFromText(SDL_Renderer *renderer, std::string text, TTF_Font *font, SDL_Color &color, SDL_Texture *&texture, SDL_Rect &textureSize) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface != NULL) {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            //Get image dimensions
            textureSize.w = textSurface->w;
            textureSize.h = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    } else {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    return texture != NULL;
}

SDL_Texture *board::createTextureFromRenderer(SDL_Renderer *renderer, SDL_Rect rect) {
    int pitch = sizeof(Uint32) * rect.w;
    Uint32 *pixels = (Uint32 *) malloc(pitch * rect.h);
    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, rect.w, rect.h, 32, pitch, SDL_PIXELFORMAT_RGBA8888);
    /*
    SDL_LockSurface(surface);
    surface->pixels = pixels;
    SDL_UnlockSurface(surface);
     */
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    free(pixels);

    return t;
}

void board::renderSquare(SDL_Renderer *renderer, const SDL_Rect &squareRect, const SDL_Color &color) {

    int lightestR = (int) (color.r + 255 * .5f) > 255 ? 255 : (int) (color.r + 255 * .5f);
    int lightestB = (int) (color.b + 255 * .5f) > 255 ? 255 : (int) (color.b + 255 * .5f);
    int lightestG = (int) (color.g + 255 * .5f) > 255 ? 255 : (int) (color.g + 255 * .5f);

    int lighterR = (int) (color.r + 255 * .25f) > 255 ? 255 : (int) (color.r + 255 * .25f);
    int lighterB = (int) (color.b + 255 * .25f) > 255 ? 255 : (int) (color.b + 255 * .25f);
    int lighterG = (int) (color.g + 255 * .25f) > 255 ? 255 : (int) (color.g + 255 * .25f);

    SDL_SetRenderDrawColor(renderer, color.r,
                           color.g,
                           color.b,
                           color.a);
    SDL_RenderFillRect(renderer, &squareRect);


    SDL_Rect firstSquareInside = { squareRect.x + 5, squareRect.y + 5, squareRect.w - 10, squareRect.h - 10 };
    SDL_SetRenderDrawColor(renderer, lightestR,
                           lightestG,
                           lightestB,
                           255);
    SDL_RenderFillRect(renderer, &firstSquareInside);

    SDL_Rect secondSquareInside = { squareRect.x + 10, squareRect.y + 10, squareRect.w - 20, squareRect.h - 20 };
    SDL_SetRenderDrawColor(renderer, lighterR,
                           lighterG,
                           lighterB,
                           255);
    SDL_RenderFillRect(renderer, &secondSquareInside);

}


void board::render(/*TTF_Font *font*/) {

    squareLength = ((screenRect.w / 3) * 2) / 10;
    SDL_Rect shapeSquareSize = { 0, 10, squareLength, squareLength };

    // draw board
    SDL_Rect screenFillRect = {border, border, shapeSquareSize.w * 10 /*+ border*/, shapeSquareSize.h * 22 + border};
    SDL_SetRenderDrawColor(renderer, 0x22, 0x22, 0x22, 0x00);
    SDL_RenderFillRect(renderer, &screenFillRect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);

    // logo
    SDL_Rect logoTetrisRect = {screenFillRect.w + 35, screenFillRect.h - 150, 320, 80};
    SDL_RenderCopy(renderer, images[0], NULL, &logoTetrisRect);

    // Level, lines, score
    SDL_Color white = { 255, 255, 255, 255 };
    if (levelTextureLabel == NULL && getTextureFromText(renderer, "level", bigFont, white, levelTextureLabel, levelRectLabel)) {
        levelRectLabel.x = screenFillRect.w + 40;
        levelRectLabel.y = 350;
    }
    SDL_RenderCopy(renderer, levelTextureLabel, NULL, &levelRectLabel);
    if (linesTextureLabel == NULL && getTextureFromText(renderer, "lines", bigFont, white, linesTextureLabel, linesRectLabel)) {
        linesRectLabel.x = screenFillRect.w + 40;
        linesRectLabel.y = 450;
    }
    SDL_RenderCopy(renderer, linesTextureLabel, NULL, &linesRectLabel);
    if (scoreTextureLabel == NULL && getTextureFromText(renderer, "score", bigFont, white, scoreTextureLabel, scoreRectLabel)) {
        scoreRectLabel.x = screenFillRect.w + 40;
        scoreRectLabel.y = 550;
    }
    SDL_RenderCopy(renderer, scoreTextureLabel, NULL, &scoreRectLabel);

    SDL_Color red = { 255, 0, 0, 255 };
    sprintf(sLevel, "%d", level);
    levelString = sLevel;
    if (getTextureFromText(renderer, levelString, bigFont, red, levelTexture, levelRect)) {
        levelRect.x = screenFillRect.w + 183;
        levelRect.y = 350;
        SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);
        SDL_DestroyTexture(levelTexture);
    }

    sprintf(sLine, "%d", lines);
    lineString = sLine;
    if (getTextureFromText(renderer, lineString, bigFont, red, linesTexture, linesRect)) {
        linesRect.x = screenFillRect.w + 183;
        linesRect.y = 450;
        SDL_RenderCopy(renderer, linesTexture, NULL, &linesRect);
        SDL_DestroyTexture(linesTexture);
    }

    sprintf(sScore, "%d", score);
    scoreString = sScore;
    if (getTextureFromText(renderer, scoreString, bigFont, red, scoreTexture, scoreRect)) {
        scoreRect.x = screenFillRect.w + 183;
        scoreRect.y = 550;
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_DestroyTexture(scoreTexture);
    }


    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == -1 && animate == none) {
                // the current moving shape is drawn with the currentShape indexed color
                // and only when not in animation mode
                SDL_Rect square = {(j * squareLength) + border, (i * squareLength) + border,
                                   squareLength, squareLength};
                renderSquare(renderer, square, gShapesColor[currentShape]);

            } /*else*/ if (grid[i][j] > 0) {
                // the grid is drawn with the saved indexed color
                SDL_Color color = gShapesColor[grid[i][j] - 1];
                SDL_Rect square = {(j * squareLength) + border, (i * squareLength) + border,
                                   squareLength, squareLength};
                renderSquare(renderer, square, color);
            }
        }
    }


    if (animate == gameOver) {
        SDL_Color textColor = { 255, 128, 128, 255 };
        if (gameOverMessageTexture == NULL && getTextureFromText(renderer, "GAME OVER", bigFont, textColor, gameOverMessageTexture, gameOverMessageRect)) {
            gameOverMessageRect.x = screenRect.w;
            gameOverMessageRect.y = screenRect.h / 2;
            SDL_RenderCopy(renderer, gameOverMessageTexture, NULL, &gameOverMessageRect);
            // SDL_DestroyTexture(debugMessageTexture);
        } else {
            gameOverMessageRect.x -= 5;
            SDL_RenderCopy(renderer, gameOverMessageTexture, NULL, &gameOverMessageRect);

            if (gameOverMessageRect.x == screenRect.x / 2) {
                animate = none;
                SDL_DestroyTexture(gameOverMessageTexture);
                gameOverMessageTexture = NULL;
            }
        }
    }

    if (animate == scrollDown) {

        // Just draw current shape before scrolling it
        if (currentShapeScrollDownYCoordinate == 0) {
            currentShapeCoordinates = getCurrentShapeRect();
            drawCurrentShape();
            // Create a texture from it
            currentShapeTexture = createTextureFromRenderer(renderer, currentShapeCoordinates);
        }

        SDL_Rect scrollCoordinate = {currentShapeCoordinates.x, currentShapeCoordinates.y - currentShapeScrollDownYCoordinate, currentShapeCoordinates.w, currentShapeCoordinates.h};
        SDL_RenderCopy(renderer, currentShapeTexture, NULL, &scrollCoordinate);
        currentShapeScrollDownYCoordinate += (squareLength / 5);

        if (currentShapeScrollDownYCoordinate >= squareLength) {
            // Scroll down
            SDL_DestroyTexture(currentShapeTexture);
            animate = none;
            formerShapeY = currentShapeY;
            deleteCurrentShapeFromGrid();
            currentShapeY --;
            copyCurrentShapeToGrid();
        }
    }

    if (animate == scrollLine) {

        // get the board texture before scrolling it
        if (currentLineScrollYCoordinate == 0) {
            currentBoardMinusFirstLineRect = getCurrentBoardMinusFirstLineRect();
            currentBoardMinusFirstLineTexture = createTextureFromRenderer(renderer, currentBoardMinusFirstLineRect);
        }

        SDL_Rect scrollCoordinate = {currentBoardMinusFirstLineRect.x, currentBoardMinusFirstLineRect.y - currentLineScrollYCoordinate, currentBoardMinusFirstLineRect.w, currentBoardMinusFirstLineRect.h};
        SDL_RenderCopy(renderer, currentShapeTexture, NULL, &scrollCoordinate);
        currentLineScrollYCoordinate += (squareLength / 5);

        if (currentLineScrollYCoordinate >= squareLength) {
            // Scroll down
            SDL_DestroyTexture(currentBoardMinusFirstLineTexture);
            animate = none;
            if (playSounds) {
                Mix_PlayChannel(-1, sounds[6], 0);
            }
            int i = rowToDelete + 1;
            while (i < 22) {

                for (int j = 0; j < 10; j++) {
                    grid[i - 1][j] = grid[i][j];
                }

                i++;
            }
            checkLines();

        }

    }


    if (animate == scrollLeft) {
        // Just draw current shape before scrolling it
        if (currentShapeScrollLeftXCoordinate == 0) {
            currentShapeCoordinates = getCurrentShapeRect();
            drawCurrentShape();
            // Create a texture from it
            currentShapeTexture = createTextureFromRenderer(renderer, currentShapeCoordinates);
        }

        SDL_Rect scrollCoordinate = {currentShapeCoordinates.x - currentShapeScrollLeftXCoordinate, currentShapeCoordinates.y /*- currentShapeScrollDownYCoordinate*/, currentShapeCoordinates.w, currentShapeCoordinates.h};
        SDL_RenderCopy(renderer, currentShapeTexture, NULL, &scrollCoordinate);
        currentShapeScrollLeftXCoordinate += (squareLength / 5);

        if (currentShapeScrollLeftXCoordinate >= squareLength) {
            // Scroll left
            SDL_DestroyTexture(currentShapeTexture);
            animate = none;
            formerShapeX = currentShapeX;
            deleteCurrentShapeFromGrid();
            currentShapeX --;
            copyCurrentShapeToGrid();
        }

    }


    if (animate == scrollRight) {
        // Just draw current shape before scrolling it
        if (currentShapeScrollRightXCoordinate == 0) {
            currentShapeCoordinates = getCurrentShapeRect();
            drawCurrentShape();
            // Create a texture from it
            currentShapeTexture = createTextureFromRenderer(renderer, currentShapeCoordinates);
        }

        SDL_Rect scrollCoordinate = {currentShapeCoordinates.x + currentShapeScrollRightXCoordinate, currentShapeCoordinates.y /*- currentShapeScrollDownYCoordinate*/, currentShapeCoordinates.w, currentShapeCoordinates.h};
        SDL_RenderCopy(renderer, currentShapeTexture, NULL, &scrollCoordinate);
        currentShapeScrollRightXCoordinate += (squareLength / 5);

        if (currentShapeScrollRightXCoordinate >= squareLength) {
            // Scroll left
            SDL_DestroyTexture(currentShapeTexture);
            animate = none;
            formerShapeX = currentShapeX;
            deleteCurrentShapeFromGrid();
            currentShapeX ++;
            copyCurrentShapeToGrid();
        }

    }


    // rotation animation
    if (animate == rotate) {
        // Just draw current shape before rotating it
        if (currentShapeRotateAngle == 0) {
            currentShapeCoordinates = getCurrentShapeRect();
            drawCurrentShape();
            // Create a texture from it
            currentShapeTexture = createTextureFromRenderer(renderer, currentShapeCoordinates);
        }

        SDL_Point rotationCenter = {(int) (currentShapeCoordinates.w *  gRotationCenters[currentShape][currentShapeRotation][0]   ), (int) (currentShapeCoordinates.h * gRotationCenters[currentShape][currentShapeRotation][1])};
        SDL_RenderCopyEx(renderer, currentShapeTexture, NULL, &currentShapeCoordinates, currentShapeRotateAngle,
                         &rotationCenter, SDL_FLIP_NONE);

        // DEBUG
        // SDL_Rect r = {screenFillRect.w + 30, 200, currentShapeCoordinates.w, currentShapeCoordinates.h};
        // SDL_RenderCopyEx(renderer, currentShapeTexture, NULL, &r, 0, NULL, SDL_FLIP_NONE);
        // SDL_RenderDrawRect(renderer, &r);
        ////////

        currentShapeRotateAngle += 5;
        if (currentShapeRotateAngle == 90) {
            // End of the animation, update the current shape and grid state
            SDL_DestroyTexture(currentShapeTexture);
            animate = none;
            deleteCurrentShapeFromGrid();
            currentShapeRotation = (currentShapeRotation + 1) % 4;
            copyCurrentShapeToGrid();
        }
    }

    // some debug informations
    /*
    SDL_Color textColor = { 200, 200, 200, 255 };
    if (getTextureFromText(renderer, debugMessageString, font, textColor, debugMessageTexture, debugMessageRect)) {
        debugMessageRect.x = screenFillRect.w + 30;
        debugMessageRect.y = 50;
        SDL_RenderCopy(renderer, debugMessageTexture, NULL, &debugMessageRect);
        SDL_DestroyTexture(debugMessageTexture);
    }
     */

}

void board::drawCurrentShape() {
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == -1) {
                // the current moving shape is drawn with the currentShape indexed color
                SDL_Rect square = {(j * squareLength) + border, (i * squareLength) + border,
                                   squareLength, squareLength};

                /*
                SDL_SetRenderDrawColor(renderer, gShapesColor[currentShape].r,
                                       gShapesColor[currentShape].g,
                                       gShapesColor[currentShape].b,
                                       gShapesColor[currentShape].a);
                SDL_RenderFillRect(renderer, &square);
                 */

                renderSquare(renderer, square, gShapesColor[currentShape]);
            }
        }
    }
}

int board::scrollDownCurrentShape() {

    // endGame detection
    if (lastCurrentShapeY == currentShapeY && lastCurrentShapeX == currentShapeX) {
        gameOverDetection = 1;
    } else {
        gameOverDetection = 0;
    }

    // wait for animation before scrolling down
    if (animate != none) {
        return 0;
    }

    // Detect further collision and not return 0 when collide
    int nextY = currentShapeY - 1;
    lastCurrentShapeY = currentShapeY;
    lastCurrentShapeX = currentShapeX;
    const int *shape = gShapesArray[currentShape][currentShapeRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j ++) {

            // collide with another shape
            if (shape[squarecount] == 1 && grid[nextY /*currentShapeY*/ + i][currentShapeX + j] > 0) {
                return 1;
            }

            // reach the bottom
            char s [20];
            sprintf(s, "%d %d %d", i, currentShapeY, currentShape);
            debugMessageString = s;
            if (shape[squarecount] == 1 && (currentShapeY + i == 0)) {
                return 1;
            }

            squarecount ++;
        }
    }

    // Scroll down
    animate = scrollDown;
    currentShapeScrollDownYCoordinate = 0;

    return 0;
}

int board::moveCurrentShapeLeft() {

    // wait for animation before scrolling down
    if (animate != none) {
        return 0;
    }

    // Check left border of the shape
    const int *shape = gShapesArray[currentShape][currentShapeRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0;  j < 4; j++) {
            if (shape[squarecount] == 1 && (j + currentShapeX - 1 < 0)) {
                return 0;
            } else if (shape[squarecount] == 1 && (grid[currentShapeY + i][currentShapeX - 1 + j] > 0)) {
                return 0;
            }
            squarecount ++;
        }
    }

    // Scroll left
    animate = scrollLeft;
    currentShapeScrollLeftXCoordinate = 0;

    return 0;
}


int board::moveCurrentShapeRight() {

    // Check right border of the shape
    const int *shape = gShapesArray[currentShape][currentShapeRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0;  j < 4; j++) {
            if (shape[squarecount] == 1 && (j + currentShapeX + 1 > 9)) {
                return 0;
            } else if (shape[squarecount] == 1 && (grid[currentShapeY + i][currentShapeX + 1 + j] > 0)) {
                return 0;
            }
            squarecount ++;
        }
    }

    // scroll right
    animate = scrollRight;
    currentShapeScrollRightXCoordinate = 0;

    return 0;
}

int board::rotateCurrentShape() {

    // before rotation, test if the rotated shape goes out of the screen
    // or collide with another shape
    int nextRotation = (currentShapeRotation + 1) % 4;
    const int *shape = gShapesArray[currentShape][nextRotation];
    int squarecount = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0;  j < 4; j++) {
            if (shape[squarecount] == 1 && (j + currentShapeX > 9)) {
                return 0;
            } else if (shape[squarecount] == 1 && (j + currentShapeX < 0)) {
                return 0;
            } else if (shape[squarecount] == 1 && (grid[currentShapeY + i][currentShapeX + j] > 0)) {
                return 0;
            }
            squarecount ++;
        }
    }

    // then rotation
    if (playSounds) {
        Mix_PlayChannel(-1, sounds[0], 0);
    }


    animate = rotate;
    currentShapeRotateAngle = 0;

    return 0;
}

SDL_Rect board::getCurrentShapeRect() {
    // find current shape coordinate
    int stop = 0;
    int mini = 0, maxi = 0;
    int minj = 10, maxj = 0;

    // first, we find the lower shape coordinate
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == -1) {
                mini = i;
                // minj = j;
                stop = 1;
                break;
            }
        }
        if (stop) break;
    }
    // then continue to find the the other coordinates
    for (int i = mini; i < 22; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == -1) {
                if (i > maxi) {
                    maxi = i;
                }
                if (j > maxj) {
                    maxj = j;
                }
                if (j < minj) {
                    minj = j;
                }
            }
        }
    }
    SDL_Rect shapeCoordinates = {minj * squareLength + border, mini * squareLength + border, (maxj - minj + 1) * squareLength, (maxi - mini + 1) * squareLength};
    return shapeCoordinates;
}


SDL_Rect board::getCurrentBoardMinusFirstLineRect() {
    squareLength = ((screenRect.w / 3) * 2) / 10;
    SDL_Rect shapeSquareSize = { 0, 10, squareLength, squareLength };
    SDL_Rect screenFillRect = {border, border + (shapeSquareSize.h * rowToDelete + 1), shapeSquareSize.w * 10 + border, shapeSquareSize.h * 21 + border};
    return  screenFillRect;
}

void board::setFont(TTF_Font *font) {
    this->font = font;
}

void board::setBigFont(TTF_Font *bigFont) {
    this->bigFont = bigFont;
}

void board::setSounds(Mix_Chunk **sounds) {
    this->sounds = sounds;
}

void board::setImages(SDL_Texture **images) {
    this->images = images;
}

void board::setRenderer(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

void board::setScreenRect(SDL_Rect screenRect) {
    this->screenRect = screenRect;
}

void board::setMainTexture(SDL_Texture *texture) {
    this->mainTexture = texture;
}

void board::setWindow(SDL_Window *window) {
    this->window = window;
}

void board::setPlaySounds(int v) {
    this->playSounds = v;
}

void board::checkLines() {
    for (int i = 0; i < 22; i++) {
        int cpt = 0;
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] > 0) {
                cpt++;
            }
        }
        if (cpt == 10) {
            deleteLine(i);
            linesDeleted ++;
            // checkLines();
            return;
        }
    }
}

int board::checkGameOver() {
    return gameOverDetection;
}

void board::deleteLine(int row) {

    if (animate != none) {
        return;
    }

    // Scroll left
    animate = scrollLine;
    currentLineScrollYCoordinate = 0;

    rowToDelete = row;

    lines++;

}