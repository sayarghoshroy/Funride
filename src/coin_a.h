#include "main.h"

#ifndef COIN_A_H
#define COIN_A_H


class Coin_a {
public:
    Coin_a() {}
    Coin_a(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
