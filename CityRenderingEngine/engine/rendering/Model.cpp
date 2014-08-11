#include "Model.h"

const int Model::meshTriangleName = 100;

Model::Model(void) : Resource() {
    vertexes = nullptr;
    uv_maps = nullptr;
    normals = nullptr;
    indexes = nullptr;
    material = nullptr;
    shader = nullptr;
    fileName = "";
    numVertices = 0;
    numIndexes = 0;
    for (int i = 0; i < MAX_BUFFER; i++) {
        bufferObjects[i] = 0;
    }
}

Model::Model(std::string fileName, int name) : Resource(name) {
    vertexes = nullptr;
    uv_maps = nullptr;
    normals = nullptr;
    indexes = nullptr;
    material = nullptr;
    shader = nullptr;
    this->fileName = fileName;
    numVertices = 0;
    numIndexes = 0;
    for (int i = 0; i < MAX_BUFFER; i++) {
        bufferObjects[i] = 0;
    }
}

Model::Model(const Model &copy) : Resource(copy) {
    vertexes = copy.vertexes;
    uv_maps = copy.uv_maps;
    normals = copy.normals;
    indexes = copy.indexes;
    numVertices = copy.numVertices;
    numIndexes = copy.numIndexes;
    material = copy.material;
    shader = copy.shader;
    fileName = copy.fileName;
    for (int i = 0; i < MAX_BUFFER; i++) {
        this->bufferObjects[i] = copy.bufferObjects[i];
    }
}

Model::~Model(void) {
    material = nullptr;
    shader = nullptr;
    loaded = false;
    valid = false;
    numVertices = -1;
}

void Model::draw() {
    if (!loaded) load(); // If it's not yet loaded, try to load it
    if (loaded && valid) { // Check it again in case there's a proble loading the Model
        glBindVertexArray(vao);
        if (material != nullptr && material->getTexture() != nullptr) {
            if (!material->getTexture()->isLoaded())
                material->getTexture()->load();
            if (material->getTexture()->isValid()) {
                GLuint program = Naquadah::getRenderer()->getCurrentShader()->getShaderProgram();
                material->getTexture()->bindTexture(program, TEXTURE0);
            }
        }
        if (bufferObjects[INDEX_BUFFER]) {
            glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
        }
        Renderer::logOpenGLError("MODEL_DRAW");
        glBindVertexArray(0);
    }
}

Model *Model::generateTriangle() {
    Model *m = new Model();
    m->numVertices = 3;
    m->numIndexes = 3;
    m->vertexes = new Vector3[m->numVertices];
    m->uv_maps = new Vector2[m->numVertices];
    m->indexes = new unsigned int[m->numVertices];
    m->name = Model::meshTriangleName;

    m->vertexes[0] = Vector3(0.0f, 1.0f, 0);
    m->vertexes[1] = Vector3(1.0f, -1.0f, 0);
    m->vertexes[2] = Vector3(-1.0f, -1.0f, 0);
    m->uv_maps[0] = Vector2(0.5f, 0.5f);
    m->uv_maps[1] = Vector2(1.0f, 1.0f);
    m->uv_maps[2] = Vector2(0, 1.0f);
    m->indexes[0] = 0;
    m->indexes[1] = 1;
    m->indexes[2] = 2;

    m->material = new Material(MATERIAL_BASIC, 1.0f, Colour::WHITE, Colour::WHITE, Colour::WHITE, nullptr);
    ResourcesManager::addResource(m->material, true);
    m->material->addUser();

    m->generateNormals();
    m->bufferData();
    return m;
}

Model* Model::generateQuad() {
    Model *m = new Model();
    m->numVertices = 6;
    m->numIndexes = 0;
    m->vertexes = new Vector3[m->numVertices];
    m->uv_maps = new Vector2[m->numVertices];
    m->name = MODEL_QUAD;

    m->vertexes[0] = Vector3(-1.0f, 1.0f, 0);
    m->vertexes[1] = Vector3(1.0f, 1.0f, 0);
    m->vertexes[2] = Vector3(1.0f, -1.0f, 0);
    m->vertexes[3] = Vector3(-1.0f, 1.0f, 0);
    m->vertexes[4] = Vector3(1.0f, -1.0f, 0);
    m->vertexes[5] = Vector3(-1.0f, -1.0f, 0);

    m->uv_maps[0] = Vector2(0, 0);
    m->uv_maps[1] = Vector2(1.0f, 0);
    m->uv_maps[2] = Vector2(1.0f, 1.0f);
    m->uv_maps[3] = Vector2(0, 0);
    m->uv_maps[4] = Vector2(1.0f, 1.0f);
    m->uv_maps[5] = Vector2(0, 1.0f);

    m->material = new Material(MATERIAL_BASIC, 1.0f, Colour::WHITE, Colour::WHITE, Colour::WHITE, nullptr);
    ResourcesManager::addResource(m->material, true);
    m->material->addUser();

    m->generateNormals();
    return m;
}

void Model::generateNormals() {
    if (normals == nullptr) {
    	normals = new Vector3[numVertices];
    }
    for (int i = 0; i < numVertices; i += 3) {
    	Vector3 a = vertexes[i];
    	Vector3 b = vertexes[i + 1];
    	Vector3 c = vertexes[i + 2];

    	Vector3 normal = Vector3::cross(b - a, c - a);
    	normal.normalise();
        //normal.invert();

        normals[i] = Vector3(normal);
    	normals[i + 1] = Vector3(normal);
    	normals[i + 2] = Vector3(normal);
    }
}

void Model::bufferData() {
     // No point in buffering the data twice. Also, it shouldn't buffer the model if it doesn't have vertexes!
    if (!loaded && vertexes != nullptr) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Buffer vertexes
        glGenBuffers(1, &bufferObjects[VERTEX_BUFFER]);
        glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_BUFFER]);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &vertexes[0], GL_STATIC_DRAW);
        glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(VERTEX_BUFFER);

        // Buffer uv_maps (optional)
        if (uv_maps != nullptr) {
            glGenBuffers(1, &bufferObjects[UV_MAP_BUFFER]);
            glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[UV_MAP_BUFFER]);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), &uv_maps[0], GL_STATIC_DRAW);
            glVertexAttribPointer(UV_MAP_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(UV_MAP_BUFFER);
        }

        // Buffer normals (optional)
        if (normals != nullptr) {
            glGenBuffers(1, &bufferObjects[NORMAL_BUFFER]);
            glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_BUFFER]);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);
            glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(NORMAL_BUFFER);
        }

        // Buffer indexes
        if (indexes != nullptr) {
            glGenBuffers(1, &bufferObjects[INDEX_BUFFER]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_BUFFER]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(GLuint), &indexes[0], GL_STATIC_DRAW);
            glVertexAttribPointer(INDEX_BUFFER, 1, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(INDEX_BUFFER);
        }

        glBindVertexArray(0);

        // Only set the resource as loaded once it has actually been loaded to the GPU
        loaded = true;
        valid = true;
    }
}

void Model::load() {
    if (!loaded) {
        if (fileName == "") {
            // It's not a Model from a file, so let's just buffer its data
            bufferData();
            return;
        }
        std::vector<std::string> lines = FileIO::readTextFile(fileName);
        std::vector<Material*> materials;

        if (lines.size() < 2) {
            // We probably couldn't load the file, or the file is invalid.
            loaded = true;
            valid = false;
            return;
        }

        // To store the vertexes, uvmaps and normals.
        std::vector<Vector3> vecVertices;
        std::vector<Vector2> vecUvMaps;
        std::vector<Vector3> vecNormals;

        /* To map the above vertexes, uvmaps and normals to all the faces. Basically .obj files will have the minimum
         * number of vertex data, and the last lines will be the faces, with each face mapping 3 of the above to a
         * face. As we're not using indexes here, we need to use there vectors below to duplicate and map the data.
         * For example, a cube.obj will have 8 vertexes, and 12 triangle faces, with each face having 3 vertexes. So
         * we have in total 24 vertexes that need to be mapped and copied from the original 8 vertexes (from above).
         */
        std::vector<Vector3> vecCorrectVertices;
        std::vector<Vector2> vecCorrectUvMaps;
        std::vector<Vector3> vecCorrectNormals;

        // Go through all of the elements of coord, and decide what kind of element is that
        for (unsigned i = 0; i < lines.size(); i++) {
            std::string line = lines[i];
            if (line.size() < 2) continue;
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
                    vecVertices.emplace_back(Vector3(x, y, z));
                } else if (secondChar == 'n') {
                    // It's a normal
                    float x, y, z;
                    sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                    vecNormals.emplace_back(Vector3(x, y, z));
                } else if (secondChar == 't') {
                    // It's a texture coordinate
                    float x, y;
                    sscanf(line.c_str(), "vt %f %f", &x, &y);
                    vecUvMaps.emplace_back(Vector2(x, y));
                }
                break;
            case 'f':
                /*
                 * It's a face
                 *
                 * The viXYZ variables represent vertex indexes, the niXYZ represent
                 * normal indexes and tiXYZ, texture mapping indexes
                 */
                int viX, viY, viZ, niX, niY, niZ, tiX, tiY, tiZ;

                if (count(line.begin(), line.end(), ' ') == 3) {
                    // It's a triangle face
                    if (line.find("//") != std::string::npos) {
                        // If it's a face with vertexes and normals
                        sscanf(line.c_str(),"f %d//%d %d//%d %d//%d", &viX ,&niX, &viY, &niY, &viZ, &niZ);
                        vecCorrectNormals.emplace_back(vecNormals.at(niX - 1));
                        vecCorrectNormals.emplace_back(vecNormals.at(niY - 1));
                        vecCorrectNormals.emplace_back(vecNormals.at(niZ - 1));
                    } else if (line.find("/") != std::string::npos) {
                        if (count(line.begin(), line.end(), '/') == 6) {
                            // If it's a face with vertexes, uv_maps and normals
                            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &viX, &tiX, &niX, &viY, &tiY, &niY, &viZ, &tiZ, &niZ);
                            vecCorrectUvMaps.emplace_back(Vector2(vecUvMaps.at(tiX - 1)));
                            vecCorrectUvMaps.emplace_back(Vector2(vecUvMaps.at(tiY - 1)));
                            vecCorrectUvMaps.emplace_back(Vector2(vecUvMaps.at(tiZ - 1)));
                            vecCorrectNormals.emplace_back(vecNormals.at(niX - 1));
                            vecCorrectNormals.emplace_back(vecNormals.at(niY - 1));
                            vecCorrectNormals.emplace_back(vecNormals.at(niZ - 1));
                        } else {
                            // If it's a face with vertexes and uv_maps
                            sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &viX, &tiX, &viY, &tiY, &viZ, &tiZ);
                            vecCorrectUvMaps.emplace_back(vecUvMaps.at(tiX - 1));
                            vecCorrectUvMaps.emplace_back(vecUvMaps.at(tiY - 1));
                            vecCorrectUvMaps.emplace_back(vecUvMaps.at(tiZ - 1));
                        }
                    } else {
                        // If it's a face with only vertexes
                        sscanf(line.c_str(), "f %d %d %d", &viX, &viY, &viZ);
                    }
                    vecCorrectVertices.emplace_back(vecVertices.at(viX - 1));
                    vecCorrectVertices.emplace_back(vecVertices.at(viY - 1));
                    vecCorrectVertices.emplace_back(vecVertices.at(viZ - 1));
                }
                break;
            case 'u':
                if (secondChar == 's') {
                    // It's a call to use a new material. If this Model already has a material, this will override it.
                    char matName[200];
                    sscanf(line.c_str(), "%*s %s", matName);
                    for (unsigned i = 0; i < materials.size(); i++) {
                        if (std::string(matName).compare(materials.at(i)->getMaterialName()) == 0) {
                            // If we find the material in the materials list, use it instead of the current one
                            material = materials.at(i);
                        }
                    }
                }
                break;
            case 'm':
                if (secondChar == 't') {
                    // It's a call to load new materials from a .mtl file
                    char matFileName[200];
                    sscanf(line.c_str(), "%*s %s", matFileName);
                    // TODO: change the call below to use the model's path, not this hardcoded path
                    std::string matFileString = "resources/meshes/" + std::string(matFileName);
                    std::vector<Material*> newMaterials = Material::loadMaterialsFromFile(matFileString);
                    materials.insert(materials.end(), newMaterials.begin(), newMaterials.end());
                }
            }
        }
        
        // Now we can put all this temporary data into the Model and buffer it.
        numVertices = (int) vecCorrectVertices.size();
        int numUvMaps = (int) vecCorrectUvMaps.size();
        int numNormals = (int) vecCorrectNormals.size();

        this->vertexes = new Vector3[numVertices];
        this->uv_maps = new Vector2[numVertices];
        this->normals = new Vector3[numVertices];
        for (int i = 0; i < numVertices; i++) {
            vertexes[i] = Vector3(vecCorrectVertices.at(i));
            if (numUvMaps == numVertices)
                uv_maps[i] = Vector2(vecCorrectUvMaps.at(i));
            if (numNormals == numVertices)
                normals[i] = Vector3(vecCorrectNormals.at(i));
        }

        if (numVertices < 1) {
            // If we don't have at least a point, this is an invalid model.
            valid = false;
        } else {
            this->bufferData();
        }

        // Now we discard all the materials that were loaded but we won't use.
        for (unsigned i = 0; i < materials.size(); i++) {
            if (materials.at(i) != this->material) {
                ResourcesManager::releaseResource(materials.at(i)->getName());
            }
        }
        if (this->material != nullptr)
            this->material->addUser();
    }
    loaded = true;
}

void Model::unload() {
    if (loaded && glIsVertexArray(vao) == GL_TRUE) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(MAX_BUFFER, bufferObjects);
        loaded = false;
        vao = 0;
    }
    if (vertexes != nullptr) {
        if (material != nullptr) {
            ResourcesManager::releaseResource(material->getName());
            material = nullptr;
        }
        if (shader != nullptr) {
            ResourcesManager::releaseResource(shader->getName());
            shader = nullptr;
        }
        delete[] vertexes; // Not really deleting and freeing the memory?!
        delete[] uv_maps;
        delete[] normals;
        delete[] indexes;
        vertexes = nullptr;
        uv_maps = nullptr;
        normals = nullptr;
        indexes = nullptr;
        valid = false;
    }
}

void Model::initializePrimitiveMeshes() {
    //Model *triangle = Model::generateTriangle();
    Model *quad = Model::generateQuad();
    
    //ResourcesManager::addResource(triangle);
    ResourcesManager::addResource(quad, true);
    //ResourcesManager::addResource(uiQuad, true);
}

Model *Model::getTriangleMesh() {
    return (Model*) ResourcesManager::getResource(Model::meshTriangleName);
}

Model *Model::getQuadMesh(int name) {
    Model *quad = (Model*) ResourcesManager::getResource(name);
    if (quad == nullptr) {
        quad = Model::generateQuad();
        quad->name = name;
        ResourcesManager::addResource(quad, false);
    }
    return quad;
}

Model *Model::getOrCreate(int name, const std::string &fileName, bool preLoad) {
    if (ResourcesManager::resourceExists(name)) {
        return (Model*) ResourcesManager::getResource(name);
    } else {
        Model *newModel = new Model(fileName, name);
        ResourcesManager::addResource(newModel, preLoad);
        return newModel;
    }
}

Model *Model::getOrCreate(int name, const std::vector<Vector3> &vertices, const std::vector<Vector2> &uv_maps,
    const Colour &colour, Texture *texture, bool preLoad) {
    Model *m = (Model*) ResourcesManager::getResource(name);
    if (m != nullptr) {
        return m;
    } else {
        m = new Model();
        m->numVertices = (int) vertices.size();
        m->vertexes = new Vector3[m->numVertices];
        m->uv_maps = new Vector2[m->numVertices];
        m->name = name;
        Material *material = new Material(ResourcesManager::generateNextName(), 1.0f, colour, colour, colour, texture);
        ResourcesManager::addResource(material, true);
        m->setMaterial(material);

        for (int i = 0; i < m->numVertices; i++) {
            m->vertexes[i] = vertices.at(i);
            m->uv_maps[i] = uv_maps.at(i);
        }
        m->generateNormals();
        ResourcesManager::addResource(m, preLoad);
        return m;
    }
}

void Model::setTexture(Texture *texture) {
    if (material != nullptr) {
        material->setTexture(texture);
    }
}

void Model::setMaterial(Material *material) {
    if (this->material != nullptr && this->material != material) {
        ResourcesManager::releaseResource(this->material->getName());
    }
    this->material = material;
    if (this->material != nullptr) {
        this->material->addUser();
    }
}