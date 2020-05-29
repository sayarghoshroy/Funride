#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(double x, double y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int lat, int vert, double grav);
    double speed;
    double x_speed;
    double y_speed;
private:
    VAO *object;
};

#endif // BALL_H
