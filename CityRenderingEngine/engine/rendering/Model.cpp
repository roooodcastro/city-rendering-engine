#include "Model.h"

const char *Model::meshTriangleName = "MESH_TRI";
const char *Model::meshQuadName = "MESH_QUAD";

Face::Face(const Face &copy) {
    this->material = new Material(*(copy.material));
    for (int i = 0; i < (3); i++) {
        this->vertexIndexes[i] = copy.vertexIndexes[i];
        this->texCoords[i] = copy.texCoords[i];
        this->normals[i] = copy.normals[i];
    }
}

Face::Face(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, Material *material) {
    this->vertexIndexes[0] = v1;
    this->vertexIndexes[1] = v2;
    this->vertexIndexes[2] = v3;
    this->texCoords[0] = t1;
    this->texCoords[1] = t2;
    this->texCoords[2] = t3;
    this->normals[0] = n1;
    this->normals[1] = n2;
    this->normals[2] = n3;
    this->material = material;
}

Face::~Face() {
    material = nullptr;
}

Model::Model(void) : Resource() {
    vertices = NULL;
    textureCoords = NULL;
    colours = NULL;
    numVertices = 0;
    normals = NULL;
    fileName = NULL;
    faces = NULL;
}

Model::Model(const char *fileName, const char *name) : Resource(name) {
    vertices = NULL;
    textureCoords = NULL;
    colours = NULL;
    numVertices = 0;
    normals = NULL;
    this->fileName = fileName;
    faces = NULL;
}

Model::Model(const Model &copy) : Resource(copy) {
    // TODO
    vertices = NULL;
    textureCoords = NULL;
    colours = NULL;
    numVertices = 0;
    normals = NULL;
    fileName = copy.fileName;
    faces = new std::vector<Face*>();
}

Model::~Model(void) {
    /* We don't delete the filename in the unload() method
     * Because we may want to load the model again, in which
     * case we'll need the filename */
    delete fileName;
}

void Model::draw() {
    // Will only try to render if the model is loaded to the GPU
    if (loaded) {
        glBindVertexArray(vao);
        //GLuint program = Naquadah::getInstance()->getDefaultShader()->getShaderProgram();
        int vertexOffset = 0;
        int vertexCount = 0;
        int currentTexId = -1;
        for (unsigned i = 0; i < faces->size(); i++) {
            Face *face = (*faces)[i];
            vertexCount += 3;
            if (face->material->getTexture() && face->material->getTexture()->isTextureValid()) {
                if (face->material->getTexture()->getTextureId() != currentTexId) {
                    //face->material->getTexture()->bindTexture(program, TEXTURE0);
                    if (currentTexId >= 0) {
                        glDrawArrays(GL_TRIANGLES, vertexOffset, vertexCount);
                        vertexOffset = vertexCount;
                        vertexCount = 0;
                    }
                    currentTexId = face->material->getTexture()->getTextureId();
                }
            }
        }
        glDrawArrays(GL_TRIANGLES, vertexOffset, vertexCount);
        Renderer::logOpenGLError("MODEL_DRAW");
        glBindVertexArray(0);
    }
}

Model *Model::generateTriangle() {
    Model *m = new Model();
    m->numVertices = 3;
    m->vertices = new Vector3[m->numVertices];
    m->colours = new Vector4[m->numVertices];
    m->textureCoords = new Vector2[m->numVertices];
    m->name = Model::meshTriangleName;
    m->faces = new std::vector<Face*>();

    m->vertices[0] = Vector3(0.0f, 1.0f, 0);
    m->vertices[1] = Vector3(1.0f, -1.0f, 0);
    m->vertices[2] = Vector3(-1.0f, -1.0f, 0);
    m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
    m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
    m->textureCoords[0] = Vector2(0.5f, 0.5f);
    m->textureCoords[1] = Vector2(1.0f, 1.0f);
    m->textureCoords[2] = Vector2(0, 1.0f);
    Face *face = new Face(1, 2, 3, 1, 2, 3, 0, 0, 0, new Material());
    //face->material->setTexture(Texture());
    m->faces->emplace_back(face);
    m->bufferData();
    return m;
}

Model* Model::generateQuad() {
    Model *m = new Model();
    m->numVertices = 6;
    m->vertices = new Vector3[m->numVertices];
    m->colours = new Vector4[m->numVertices];
    m->textureCoords = new Vector2[m->numVertices];
    m->faces = new std::vector<Face*>();
    m->name = Model::meshQuadName;

    m->vertices[0] = Vector3(-1.0f, 1.0f, 0);
    m->vertices[1] = Vector3(1.0f, 1.0f, 0);
    m->vertices[2] = Vector3(1.0f, -1.0f, 0);
    m->vertices[3] = Vector3(-1.0f, 1.0f, 0);
    m->vertices[4] = Vector3(1.0f, -1.0f, 0);
    m->vertices[5] = Vector3(-1.0f, -1.0f, 0);
    m->colours[0] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m->colours[1] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m->colours[2] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m->colours[3] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m->colours[4] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m->colours[5] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    m->textureCoords[0] = Vector2(0, 0);
    m->textureCoords[1] = Vector2(1.0f, 0);
    m->textureCoords[2] = Vector2(1.0f, 1.0f);
    m->textureCoords[3] = Vector2(0, 0);
    m->textureCoords[4] = Vector2(1.0f, 1.0f);
    m->textureCoords[5] = Vector2(0, 1.0f);

    Face *face1 = new Face(1, 2, 3, 1, 2, 3, 0, 0, 0, new Material());
    Face *face2 = new Face(4, 5, 6, 4, 5, 6, 0, 0, 0, new Material());
    /* We purposely set invalid textures here, so when this model renders,
     * it won't bind any texture. This specific mesh can't bind a texture
     * on its own because these primitives will be used primarily to render
     * interface items, such as texts and buttons that have other ways of
     * binding its textures. */
    //face1->material->setTexture(Texture());
    //face2->material->setTexture(Texture());
    m->faces->emplace_back(face1);
    m->faces->emplace_back(face2);

    m->bufferData();
    return m;
}

void Model::generateNormals() {
    //if (type == GL_TRIANGLES) {
    //	if (normals == NULL) {
    //		normals = new Vector3[numVertices];
    //	}
    //	for (int i = 0; i < numVertices; i += 3) {
    //		Vector3 &a = vertices[i];
    //		Vector3 &b = vertices[i + 1];
    //		Vector3 &c = vertices[i + 2];

    //		Vector3 normal = Vector3::cross(b - a, c - a);
    //		normal.normalise();

    //		normals[i] = normal;
    //		normals[i + 1] = normal;
    //		normals[i + 2] = normal;
    //	}
    //}
}

void Model::bufferData() {
    if (!loaded) { // No point in buffering the data twice
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Now we can finally buffer the whole thing to the driver
        int index = 0; // Vertex
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);

        index = 1; // Colour (diffuse colour and alpha of the material)
        glGenBuffers(1, &colVbo);
        glBindBuffer(GL_ARRAY_BUFFER, colVbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), &colours[0], GL_STATIC_DRAW);
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);

        if (textureCoords != NULL) {
            index = 2; // Texture map
            glGenBuffers(1, &texVbo);
            glBindBuffer(GL_ARRAY_BUFFER, texVbo);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), &textureCoords[0], GL_STATIC_DRAW);
            glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(index);
        }

        if (normals != NULL) {
            index = 3; // Normal
            glGenBuffers(1, &norVbo);
            glBindBuffer(GL_ARRAY_BUFFER, norVbo);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(index);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        loaded = true;
    }
}

void Model::load() {
    if (!loaded) {
        std::vector<std::string*> lines;
        std::vector<Vector3*> vecVertices;
        std::vector<Face*> vecFaces;
        std::vector<Vector3*> vecNormals;
        std::vector<Vector2*> vecTexCoords;
        std::ifstream in(fileName);
        std::vector<Material*> materials;
        Material *currentMaterial = new Material();

        if (!in.is_open()) {
            std::cout << "Wavefront (.obj) file couldn't be opened!" << std::endl;
            return;
        }

        char lineBuffer[256];
        //read in every line to coord
    
        while(!in.eof()) {
            in.getline(lineBuffer, 256);
            lines.emplace_back(new std::string(lineBuffer));
        }

        //go through all of the elements of coord, and decide what kind of element is that
        for (unsigned i = 0; i < lines.size(); i++) {
            std::string line = *(lines[i]);
            if (line.size() < 2) {
                continue;
            }
            // Trim the whitespaces at the beginning of the line, if present
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            char firstChar = line[0];
            char secondChar = line[1];

            switch (firstChar) {
            case '#':
                // It's a comment, don't do anything with it
                break;
            case 'v':
                if (secondChar == ' ') {
                    // It's a vertex
                    float x, y, z;
                    sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                    vecVertices.emplace_back(new Vector3(x, y, z));
                } else if (secondChar == 'n') {
                    // It's a normal
                    float x, y, z;
                    sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                    vecNormals.emplace_back(new Vector3(x, y, z));
                } else if (secondChar == 't') {
                    // It's a texture coordinate
                    float x, y;
                    sscanf(line.c_str(), "vt %f %f", &x, &y);
                    vecTexCoords.emplace_back(new Vector2(x, y));
                }
                break;
            case 'f':
                /*
                    * It's a face
                    *
                    * The vi variables represent vertex indexes, the ni represent
                    * normal indexes and ti, texture mapping indexes
                    */
                int viX, viY, viZ, niX, niY, niZ, tiX, tiY, tiZ;

                if (count(line.begin(), line.end(), ' ') == 4) {
                    //// It's a quadrilateral face
                    //if (line.find("//") != std::string::npos) {
                    //    // If there's a normal vector index
                    //    sscanf(line.c_str(),"f %d//%d %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ, &viW, &niW);
                    //    vecFaces.emplace_back(new Face(viX, viY, viZ, viW, 0, 0, 0, 0, niX, niY, niZ, niW, *currentMaterial));
                    //} else if (line.find("/") != std::string::npos) {
                    //    if (count(line.begin(), line.end(), '/') == 8) {
                    //        // If there's both texture coordinates and normal vector
                    //        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ, &viW, &tiW, &niW);
                    //        vecFaces.emplace_back(new Face(viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, niX, niY, niZ, niW, *currentMaterial));
                    //    } else {
                    //        // If there's texture coordinate indexes
                    //        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ, &viW, &tiW);
                    //        vecFaces.emplace_back(new Face(viX, viY, viZ, viW, tiX, tiY, tiZ, tiW, 0, 0, 0, 0, *currentMaterial));
                    //    }
                    //} else {
                    //    // If we just have vertices, no normals or texture coordinates
                    //    sscanf(line.c_str(), "f %d %d %d %d", &viX, &viY, &viZ, &viW);
                    //    vecFaces.emplace_back(new Face(viX, viY, viZ, viW, 0, 0, 0, 0, 0, 0, 0, 0, *currentMaterial));
                    //}
                } else {
                    // It's a triangle face
                    if (line.find("//") != std::string::npos) {
                        // If there's a normal vector index
                        sscanf(line.c_str(),"f %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ);
                        vecFaces.emplace_back(new Face(viX, viY, viZ, 0, 0, 0, niX, niY, niZ, currentMaterial));
                    } else if (line.find("/") != std::string::npos) {
                        if (count(line.begin(), line.end(), '/') == 6) {
                            // If there's both texture coordinates and normal vector
                            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ);
                            vecFaces.emplace_back(new Face(viX, viY, viZ, tiX, tiY, tiZ, niX, niY, niZ, currentMaterial));
                        } else {
                            // If there's texture coordinate indexes
                            sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ);
                            vecFaces.emplace_back(new Face(viX, viY, viZ, tiX, tiY, tiZ, 0, 0, 0, currentMaterial));
                        }
                    } else {
                        // If we just have vertices, no normals or texture coordinates
                        sscanf(line.c_str(), "f %d %d %d", &viX, &viY, &viZ);
                        vecFaces.emplace_back(new Face(viX, viY, viZ, 0, 0, 0, 0, 0, 0, currentMaterial));
                    }
                }
                break;
            case 'u':
                if (secondChar == 's') {
                    // It's a call to use a new material
                    char matName[200];
                    sscanf(line.c_str(), "%*s %s", matName);
                    for (unsigned i = 0; i < materials.size(); i++) {
                        Material material = *(materials[i]);
                        if (((std::string) matName).compare(material.getName()) == 0) {
                            // If we find our material in the materials list, use it
                            currentMaterial = materials[i];
                            // If we don't find it, keep the current material, something has gone wrong!
                        }
                    }
                }
                break;
            case 'm':
                if (secondChar == 't') {
                    // It's a call to load new materials from a .mtl file
                    char matFileName[200];
                    sscanf(line.c_str(), "%*s %s", matFileName);
                    std::vector<Material*> newMaterials = Material::loadMaterialsFromFile((((std::string) "resources/models/") + ((std::string) matFileName)).c_str());
                    materials.insert(materials.end(), newMaterials.begin(), newMaterials.end());
                }
            }
        }

        // If we got this far, make our model and buffer the data
        this->faces = new std::vector<Face*>(vecFaces);

        // First we have to count the total number of vertices
        int numTotalVertices = 0;
        for (unsigned i = 0; i < vecFaces.size(); i++) {
            numTotalVertices += 3;
        }
        this->numVertices = numTotalVertices;
        // Now we fill in the ordered vectors with all vertices
        int vertexOffset = 0;
        this->vertices = new Vector3[numTotalVertices];
        this->colours = new Vector4[numTotalVertices];
        this->textureCoords = new Vector2[numTotalVertices];
        this->normals = new Vector3[numTotalVertices];
        for (unsigned i = 0; i < vecFaces.size(); i++) {
            Face *face = vecFaces[i];
            int numVertices = 3;
        
            for (int j = 0; j < numVertices; j++) {
                this->vertices[vertexOffset + j] = *(vecVertices[face->vertexIndexes[j] - 1]);
                Colour colour = face->material->getDiffuse();
                this->colours[vertexOffset + j] = Vector4(colour.red, colour.green, colour.blue, face->material->getAlpha());
                if (face->texCoords[j] == 0) {
                    this->textureCoords[vertexOffset + j] = Vector2();
                } else {
                    this->textureCoords[vertexOffset + j] = *(vecTexCoords[face->texCoords[j] - 1]);
                }
                if (face->normals[j] == 0) {
                    this->normals[vertexOffset + j] = Vector3();
                } else {
                    this->normals[vertexOffset + j] = *(vecNormals[face->normals[j] - 1]);
                }
            }
            vertexOffset += numVertices;
        }

        this->bufferData();

        // Now we clean up the mess
        for (unsigned i = 0; i < lines.size(); i++)
            delete lines[i];
        for (unsigned i = 0; i < vecNormals.size(); i++)
            delete vecNormals[i];
        for (unsigned i = 0; i < vecVertices.size(); i++)
            delete vecVertices[i];
        for (unsigned i = 0; i < vecTexCoords.size(); i++)
            delete vecTexCoords[i];
        //for (unsigned i = 0; i < materials.size(); i++)
            //delete materials[i];
        //delete currentMaterial;
    }
}

void Model::unload() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &posVbo);
    glDeleteBuffers(1, &colVbo);
    glDeleteBuffers(1, &texVbo);
    glDeleteBuffers(1, &norVbo);
    delete[] vertices;
    delete[] colours;
    delete[] textureCoords;
    delete[] normals;
    faces->clear();
    delete faces;
    loaded = false;
}

void Model::initializePrimitiveMeshes() {
    //Model *triangle = Model::generateTriangle();
    //Model *quad = Model::generateQuad();
    //ResourcesManager::addResource(triangle);
    //ResourcesManager::addResource(quad);
}

Model *Model::getTriangleMesh() {
    return (Model*) ResourcesManager::getResource(Model::meshTriangleName);
}

Model *Model::getQuadMesh() {
    return (Model*) ResourcesManager::getResource(Model::meshQuadName);
}

Model *Model::getOrCreate(const char *name, const char *fileName) {
    if (ResourcesManager::resourceExists(name)) {
        return (Model*) ResourcesManager::getResource(name);
    } else {
        Model *newModel = new Model(fileName, name);
        ResourcesManager::addResource(newModel);
        return newModel;
    }
}