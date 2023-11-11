// External libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Local headers
#include <data.h>

#define UNBIND_VERTEX_ARRAY glBindVertexArray(0)

DrawDetails render_establish_mesh(VertexList vList, ElemList eList) {
    GLuint VAO;
    GLuint objectBuffers[2]; // VBO and EBO

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, objectBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, objectBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vList.count, vList.list, GL_STATIC_DRAW);
    glVertexAttribPointer(0, VERTEX_ELEM, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectBuffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * eList.count, eList.list, GL_STATIC_DRAW);

    UNBIND_VERTEX_ARRAY;
    glDeleteBuffers(2, objectBuffers);

    DrawDetails details = { VAO, vList.count };
    return details;
}

void render_delete_mesh(DrawList dList) {
    for(unsigned int i = 0; i < dList.count; i++) {
        glDeleteVertexArrays(1, &dList.list[i].VAO);
    }
}

void render_draw(DrawList dList) {
    for(unsigned int i = 0; i < dList.count; i++) {
        glBindVertexArray(dList.list[i].VAO);
        glDrawElements(GL_TRIANGLES, dList.list[i].numElems, GL_UNSIGNED_INT, NULL);
    }
    UNBIND_VERTEX_ARRAY;
}