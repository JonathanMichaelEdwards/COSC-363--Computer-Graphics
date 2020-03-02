#ifndef LOAD_MESH_H
#define LOAD_MESH_H


class loadMesh
{
private:
    /* data */
public:
    // loadMesh(/* args */);
    // ~loadMesh();
    float *x, *y, *z;					//vertex coordinates
    int *nv, *t1, *t2, *t3, *t4;		//number of vertices and vertex indices of each face
    int nvert, nface;					//total number of vertices and faces
    float angleX = 10.0,  angleY = -20;	//Rotation angles about x, y axes
    float xmin, xmax, ymin, ymax;		//min, max values of  object coordinates
};

// loadMesh::loadMesh(/* args */)
// {
// }

// loadMesh::~loadMesh()
// {
// }


#endif  // LOAD_MESH_H