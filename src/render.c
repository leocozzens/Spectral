// C standard headers
#include <stddef.h>
// External libraries
#include <glad/glad.h>
// Local headers
#include <data.h>

#define UNBIND_VERTEX_ARRAY glBindVertexArray(0)

DrawDetails render_establish_mesh(VertexList vList, ElemList eList) {
    GLuint VAO;
    GLuint bufferObjects[2]; // VBO and EBO

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, bufferObjects);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vList.count, vList.list, GL_STATIC_DRAW);
    glVertexAttribPointer(0, POSITION_ELEM, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, pos));
    glVertexAttribPointer(1, COLOR_ELEM, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(0); // Vertices position
    glEnableVertexAttribArray(1); // Vertices color

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * eList.count, eList.list, GL_STATIC_DRAW);

    UNBIND_VERTEX_ARRAY;
    glDeleteBuffers(2, bufferObjects);

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
        glDrawElements(GL_TRIANGLE_STRIP, dList.list[i].numElems, GL_UNSIGNED_INT, NULL);
    }
    UNBIND_VERTEX_ARRAY;
}