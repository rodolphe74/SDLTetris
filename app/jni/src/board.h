//
// Created by rodolphe on 12/09/18.
//

#ifndef SDLTETRIS_BOARD_H
#define SDLTETRIS_BOARD_H

#include <random>
#include <vector>

enum animation { none, scrollDown, rotate, scrollRight, scrollLeft, scrollLine, gameOver };

class board {

private:
    int border;
    int currentShape;
    int currentShapeY;
    int currentShapeX;
    int formerShapeX;
    int formerShapeY;
    int currentShapeRotation;
    int squareLength;
    int rowToDelete;

    SDL_Texture *debugMessageTexture;
    SDL_Texture *gameOverMessageTexture;
    SDL_Texture *mainTexture;
    SDL_Texture *currentShapeTexture;
    SDL_Texture *currentBoardMinusFirstLineTexture;

    SDL_Texture *levelTextureLabel;
    SDL_Texture *linesTextureLabel;
    SDL_Texture *scoreTextureLabel;
    SDL_Rect levelRectLabel;
    SDL_Rect linesRectLabel;
    SDL_Rect scoreRectLabel;
    SDL_Texture *levelTexture;
    SDL_Texture *linesTexture;
    SDL_Texture *scoreTexture;
    SDL_Rect levelRect;
    SDL_Rect linesRect;
    SDL_Rect scoreRect;
    int level = 1234567;
    char sLevel[20];
    std::string levelString;
    int lines = 0;
    char sLine[20];
    std::string lineString;
    int score = 0;
    char sScore[20];
    std::string scoreString;
    int linesDeleted = 0;

    SDL_Rect currentShapeCoordinates;
    SDL_Rect currentBoardMinusFirstLineRect;
    float currentShapeRotateAngle;
    int currentShapeScrollDownYCoordinate;
    int currentShapeScrollLeftXCoordinate;
    int currentShapeScrollRightXCoordinate;
    int currentLineScrollYCoordinate;
    int gameOverDetection;
    int lastCurrentShapeY, lastCurrentShapeX;
    SDL_Rect debugMessageRect;
    SDL_Rect gameOverMessageRect;
    std::string debugMessageString;

    Mix_Chunk **sounds;
    int playSounds;

    SDL_Texture **images;

    SDL_Renderer *renderer;
    SDL_Rect screenRect;
    SDL_Window *window;

    TTF_Font *font;
    TTF_Font *bigFont;

    void copyCurrentShapeToGrid();
    void deleteCurrentShapeFromGrid();

public:

    int grid[26][10];
    animation animate;

    board() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 22; j++) {
                grid[j][i] = 0;
            }
        }

        border = 30;
        // int x = rand() % 7;

        // Random first shape
        std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6};
        std::random_device random_dev;
        std::mt19937 generator(random_dev());
        std::shuffle(vec.begin(), vec.end(), generator);
        int x = vec.at(rand() % 7);
//        int x = 0;

        currentShape = x;
        currentShapeY = /*21*/22;
        currentShapeX = 5;
        formerShapeY = currentShapeY;
        formerShapeX = currentShapeX;
        currentShapeRotation = 0;
        animate = none;
        currentShapeRotateAngle = 0;
        currentShapeScrollDownYCoordinate = 0;
        gameOverDetection = 0;
        lastCurrentShapeY = -1;
        lastCurrentShapeX = -1;
        gameOverMessageTexture = NULL;
        playSounds = 1;
        scoreTextureLabel = NULL;
        linesTextureLabel = NULL;
        levelTextureLabel = NULL;

        // DEBUG
//        for (int j = 0; j < 9; j++) {
//            grid[0][j] = 2;
//            grid[1][j] = 3;
//            grid[2][j] = 3;
//            grid[3][j] = 3;
//        }
        ////////

    }

    ~board() {
        SDL_DestroyTexture(levelTextureLabel);
        SDL_DestroyTexture(linesTextureLabel);
        SDL_DestroyTexture(scoreTextureLabel);
    }

    void setFont(TTF_Font *font);

    void setBigFont(TTF_Font *bigFont);

    void setSounds(Mix_Chunk **sounds);

    void setImages(SDL_Texture **images);

    void setRenderer(SDL_Renderer *renderer);

    void setScreenRect(SDL_Rect screenRect);

    void setMainTexture(SDL_Texture *texture);

    void setWindow(SDL_Window *window);

    void setLinesDeleted(int val) {
        linesDeleted = val;
    }

    int getLinesDeleted() {
        return linesDeleted;
    }

    void setScore(int val) {
        score = val;
    }

    int getScore() {
        return score;
    }

    void setPlaySounds(int v);

    void playWithShape(int idx);

    void renderSquare(SDL_Renderer *renderer, const SDL_Rect &squareRect, const SDL_Color &color);

    int scrollDownCurrentShape();

    int moveCurrentShapeRight();

    int moveCurrentShapeLeft();

    int rotateCurrentShape();

    SDL_Rect getCurrentShapeRect();

    SDL_Rect getCurrentBoardMinusFirstLineRect();

    bool getTextureFromText(SDL_Renderer *renderer, std::string text, TTF_Font *font, SDL_Color &color, SDL_Texture *&texture, SDL_Rect &textureSize);

    SDL_Texture *createTextureFromRenderer(SDL_Renderer *renderer, SDL_Rect rect);

    void render(/*SDL_Renderer *gRenderer,*/ /*SDL_Rect screenSize,*/ /*TTF_Font *font*/);

    void checkLines();

    int checkGameOver();

    void deleteLine(int row);

    void drawCurrentShape();
};


#endif //SDLTETRIS_BOARD_H
