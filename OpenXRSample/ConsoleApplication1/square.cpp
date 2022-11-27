#include "square.hpp"
#include "GL/glew.h"

/*
    Constructor: Run when square is created
    inputs:     The shader program for this square
    returns:    None
   */
Square::Square(Shader* shader)
{
    initVAO();
    initVBO();
    this->shader = shader;
    return;
}

/*
 Constructor: Run when Square is created, uses default shader since none is provided
 inputs:     None
 returns:    None
*/
Square::Square()
{
    initVAO();
    initVBO();
    
    this->shader = Shader::default_shader;
    GLuint program = this->shader->getProgram();
    this->mvp_location = glGetUniformLocation(program, "mvp");

    //Create the model matrix
    model_matrix = glm::mat4(1.0f);

    glm::mat4 rot = glm::toMat4(this->rotation);
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), this->position);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
    model_matrix = trans * rot * scale;
}

void Square::initVBO()
{
    glGenBuffers(1, &(this->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/*
 initVBO:    Initialize the Vertex Array Object
 inputs:     None
 returns:    None
*/
void Square::initVAO()
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
}


/*
    initVBO:    Draw the Square
    inputs:     View Projection matrix for Camera
    returns:    None
   */
void Square::draw(glm::mat4 vp_matrix)
{
    //Calculate out the Model View Projection Matrix
    /*Matrix4f mvp = new Matrix4f();
    vp_matrix.mul(model_matrix, mvp);
    */
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &vp_matrix[0][0]);
    //Draw the Square
    uint32_t program = this->shader->getProgram();
    glUseProgram(program);
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
    glDisableVertexAttribArray(0);
}
