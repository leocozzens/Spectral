#ifndef RENDER_H
#define RENDER_H

typedef struct _DrawDetails DrawDetails;
typedef struct _DrawList    DrawList;
typedef struct _VertexList  VertexList;
typedef struct _ElemList    ElemList;

DrawDetails render_establish_mesh(VertexList vList, ElemList eList);
void render_delete_mesh(DrawList dList);
void render_draw(DrawList dList);

#endif