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
 * It contains vectors with the vertexes, texture mapping, normal data and the indexes. The indexes tell OpenGL which
 * vertexes make up a triangle, so we can have less vertex, uv_map and normal information uploaded to the GPU, taking
 * less space in its memory, and having on;y a big array of floats, which is much less memory intensive.
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

enum ModelBuffer {
    VERTEX_BUFFER, UV_MAP_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, MAX_BUFFER
};

class Model : public Resource {
public:
    Model(void);
    Model(const Model &copy);
    Model(std::string fileName, int name);
    virtual ~Model(void);

    virtual void draw();

    /* This method should load the specific resource into memory */
    virtual void load();

    /* This method should destroy the resource, unloading and releasing it from memory */
    virtual void unload();

    /* Sets the texture to the Material of this Model. */
    void setTexture(Texture *texture);

    void setMaterial(Material *material);

    /*
     * Functions to create some primitive models/meshes.
     * This is similar to the Graphics coursework functions,
     * and creates a point, a triangle and a quad mesh, with
     * the resource names "MESH_POINT", "MESH_TRI", and "MESH_QUAD".
     */
    static void initializePrimitiveMeshes();

    /*
     * Methods to retrieve the primitive meshes. This creates the mesh AND adds it to the ResourcesManager. If the mesh
     * already exists in the manager, it just retrieves it, instead of duplicating it. This does not buffer the mesh.
     */
    static Model *getTriangleMesh();
    static Model *getQuadMesh(int name);

    /*
     * This function tries to retrieve the requested model from the resource manager, and if it fails, creates and
     * returns a new model. The bool preLoad determines if the Model should be also loaded in this call, if set to
     * false it will still need to be loaded before it can be used.
     */
    static Model *getOrCreate(int name, const std::string &fileName, bool preLoad);

    /*
     * Tries to retrieve a Model from ResourcesManager with the provided name. If there's no loaded Model with this
     * name, it'll create, load and add a new Model to ResourcesManager using the provided vertices. This should only
     * be used to create 2D models, as the texture coordinates will be dynamically calculated and may not be correct
     * for more complex 3D shapes. The bool preLoad determines if the Model should be also loaded in this call, if set
     * to false it will still need to be loaded before it can be used.
     */
    static Model *getOrCreate(int name, const std::vector<Vector3> &vertices, const std::vector<Vector2> &uv_maps,
        const Colour &colour, Texture *texture, bool preLoad);

    static const int meshTriangleName;

protected:

    void bufferData();
    void generateNormals();

    static Model *generateTriangle();
    static Model *generateQuad();

    /* OpenGL identifiers for the buffer objects of this Model. */
    GLuint vao; // Vertex Array Object - VAO
    GLuint bufferObjects[MAX_BUFFER]; // The Vertex Buffer Objects - VBOs

    /* The total number of verteces, uv_maps and normals of this Model. */
    int numVertices;

    /* The total number of indexes of this model. This is the true number of vertexes of the Model. */
    int numIndexes;

    /* The fileName for the .obj file. If this model wasn't loaded from an .obj file, this will be null.*/
    std::string fileName;

    /* The arrays containing the vertex, uv_map and normal informations. */
    Vector3 *vertexes;
    Vector2 *uv_maps;
    Vector3 *normals;

    /* The array containing the indexes information, to map the vertexes, uv_maps and normals to OpenGL. */
    unsigned int *indexes;

    /* The Shader to be used by this model. */
    Shader *shader;

    /* The Material used in this model. */
    Material *material;
};