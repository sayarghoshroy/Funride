#include "boom.h"
#include "main.h"
#include "bits/stdc++.h"

Boom::Boom(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->y_speed = -0.025;
    this->speed = -0.1;
    acceleration = 0.0008;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.25f, 0.0f,
        0.25f, 0.25f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.0f, 0.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Boom::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boom::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boom::tick() {
	this->speed += acceleration;
    this->position.x += this->speed;
    this->position.y += (this->y_speed * (1 - abs(this->speed/-0.09)));
}