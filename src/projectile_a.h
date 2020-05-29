#include "main.h"

#ifndef PROJECTILE_A_H
#define PROJECTILE_A_H


class Projectile_a {
public:
    Projectile_a() {}
    Projectile_a(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(double grav);
    double speed;
    double y_speed;
private:
    VAO *object;
};

#endif // BALL_H
