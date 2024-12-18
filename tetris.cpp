#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <chrono>
using namespace std;

void showConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void goToXY(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

const int width = 10;
const int height = 20;

class Detail {
    int posX = width / 2;
    int posY = 0;
    string sprite = 
        "0000"
        "0000"
        "0000"
        "0000";

public:
    Detail(const string& _sprite) {
        sprite = _sprite;
    }

    const string& getSprite() const {
        return sprite;
    }

    int x() const {
        return posX;
    }

    int y() const {
        return posY;
    }

    void draw() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                goToXY(1+ posX + j, 1 + posY + i);
                char sym = sprite[i * 4 + j];

                if (sym == '#') {
                    cout << sym;
                }
            }
        }
    }

    void clear() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; j++) {
                goToXY(1 + posX + j, 1 + posY + i);
                char sym = sprite[i * 4 + j];

                if (sym == '#') {
                    cout << ' ';
                }
            }
        }
    }

    void moveDown() {
        posY += 1;
    }

    void moveLeft() {
        posX -= 1;
    }

    void moveRight() {
        posX += 1;
    }
};

class Map {
    string map = string(height * width, ' ');

public:
    void drawFrame() {
        for (int h = 0; h < height + 2; ++h) {
            for (int w = 0; w < width + 2; ++w) {
                goToXY(w, h);
                if (h == 0 || w == 0 || h == height + 1 || w == width + 1) {
                    cout << 'x';
                }
            }
        }
    }

    bool collision(const Detail& detail) {
        const string& sprite = detail.getSprite();
        const int detX = detail.x();
        const int detY = detail.y();

        for (int i = 0; i < 4; ++i) {
            int botPos = 0;
            for (int j = 0; j < 4; ++j) {
                if (sprite[j * 4 + i] == '#') {
                    botPos = detY + j + 1;
                }
            }

            if (botPos * width + i + detX >= map.size() || map[botPos * width + i + detX] != ' ') {
                for (int k = 0; k < 4; ++k) {
                    for (int j = 0; j < 4; ++j) {
                        if (sprite[j * 4 + k] == '#') {
                            map[(detY + j) * width + k + detX] = '#';
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }
};

vector<Detail> details = {
    Detail(
        "##00"
        "##00"
        "0000"
        "0000"
    )
};

int main(){
    Map map;
    showConsoleCursor(false);
    bool isPlaying = true;
    map.drawFrame();

    chrono::time_point<chrono::system_clock> startTime = chrono::system_clock::now();
    chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();

    do {
        Detail curDetail = details[0];
        bool stand = false;

        while (!stand) {
            endTime = chrono::system_clock::now();
            chrono::duration<double> diff = endTime - startTime;

            stand = map.collision(curDetail);

            if (diff.count() > 0.2 && !stand) {
                curDetail.clear();
                curDetail.moveDown();

                if (GetKeyState('A') & 0x8000) {
                    curDetail.moveLeft();
                }

                if (GetKeyState('D') & 0x8000) {
                    curDetail.moveRight();
                }


                curDetail.draw();
                startTime = chrono::system_clock::now();
            }
        }
    } while (isPlaying);
}

