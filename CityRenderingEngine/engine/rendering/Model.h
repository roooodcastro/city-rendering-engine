/*
 * Author: Rodrigo Castro Azevedo and TheCPlusPlusGuy (http://www.youtube.com/user/thecplusplusguy)
 * Date: 17/05/2014
 *
 * Description: This class stores all the vertex information necessary to render a complete 3D model, with UV mapping,
 * normals, materials and everything else. It supports loading of Wavefront files (.obj), but currently only those that
 * consists of only triangles. It will just ignore all quads that might be in the file, because for some reason quads
 * don't work. The object file loader is an adaptation of the object loader from thecplusplusguy, a Youtube channel
 * that has some nice tutorials on SDL and OpenGL, but it was 99% rewritten because of the massive lack of readability
 * of his code.
 *
 * It contains vectors with the vertex, texture mapping and normal data, and a vector of faces to index each vertex to
 * its correct face. A Face is a structure that holds the 3 indices for the vertex, texture and normal vectors. It also
 * specifies the material that the face is using. The Model must also contain a Shader that will be used to render it.
 *
 */

#pragma once
#pragma warning(disable : 4996) // Annoying warning about sscanf

#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"
#include "../Naquadah.h"
#include "Material.h"
#include "Colour.h"
#include "Shader.h"
#include "../ResourcesManager.h"

class Naquadah;
class Material;
class Texture;
class Shader;

/*
 * Face stores the attributes of a face, and is used to correctly order and render every vertex, uv map, material
 * and normal.
 */
struct Face {
    int vertexIndexes[3]; // Index of the vertices that make up this face, starting from 1
    int texCoords[3]; // Indexes of the texture coordinates of this face, starting from 1
    int normals[3]; // Indexes of the texture coordinates of this face, starting from 1
    Material *material; // Material used in this face
    Face(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, Material *material);
    Face(const Face &copy);
    ~Face(void);
};

class Model : public Resource {
public:
    Model(void);
    Model(const Model &copy);
    Model(const char *fileName, const char *name);
    virtual ~Model(void);

    virtual void draw();

    /* This method should load the specific resource into memory */
    virtual void load();

    /* This method should destroy the resource, unloading and releasing it from memory */
    virtual void unload();

    /* Sets texture to ALL materials of ALL faces of this Model. */
    void setTexture(Texture *texture);

    /*
     * Functions to create some primitive models/meshes.
     * This is similar to the Graphics coursework functions,
     * and creates a point, a triangle and a quad mesh, with
     * the resource names "MESH_POINT", "MESH_TRI", and "MESH_QUAD".
     */
    static void initializePrimitiveMeshes();

    /* Methods to retrieve the primitive meshes */
    static Model *getTriangleMesh();
    static Model *getQuadMesh();

    /*
     * This function tries to retrieve the requested model from the
     * resource manager, and if it fails, creates and returns a new model.
     */
    static Model *getOrCreate(const char *name, const char *fileName);

    /*
     * Tries to retrieve a Model from ResourcesManager with the provided name. If there's no loaded Model with this
     * name, it'll create, load and add a new Model to ResourcesManager using the provided vertices. This should only
     * be used to create 2D models, as the texture coordinates will be dynamically calculated and may not be correct
     * for more complex 3D shapes.
     */
    static Model *getOrCreate(std::string name, std::vector<Vector3> vertices, Colour colour, Texture *texture);

    static const char *meshTriangleName;
    static const char *meshQuadName;

protected:

    void bufferData();
    void generateNormals();

    static Model *generateTriangle();
    static Model *generateQuad();

    GLuint vao;
    GLuint posVbo;
    GLuint colVbo;
    GLuint texVbo;
    GLuint norVbo;

    int numVertices;
    const char *fileName;

    Vector3 *vertices;
    Vector4 *colours;
    Vector2 *textureCoords;
    Vector3 *normals;
    std::vector<Face*> *faces;

    /* The Shader to be used by this model. */
    Shader *shader;

    /* Stores the materials used in this model. */
    std::vector<Material*> *materials;
};