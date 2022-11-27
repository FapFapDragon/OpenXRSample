#pragma once
#ifndef SQUARE_H
#define SQUARE_H


#include "shader.hpp"
#include "glm.hpp"
#include "gtx/quaternion.hpp"

class Square
{
private:

	Shader* shader;
	uint32_t program;

    //Values needed for Model Matrix
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat rotation = glm::quat(0, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);

    glm::mat4 model_matrix = glm::mat4(1);

    //Vertices to make up square, for this program not bothering with index buffer objects
    const float vertices[108] = {-1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, // triangle 2 : end
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    GLuint vao;
    GLuint vbo;

    GLuint mvp_location;

public:
    /*
     Constructor: Run when square is created
     inputs:     The shader program for this square
     returns:    None
    */
    Square(Shader* shader);

    Square();

    void initVBO();

    /*
     initVBO:    Initialize the Vertex Array Object
     inputs:     None
     returns:    None
    */
    void initVAO();


    /*
        initVBO:    Draw the Square
        inputs:     View Projection matrix for Camera
        returns:    None
       */
    void draw(glm::mat4 vp_matrix);
};

#endif 