#include "Material.h"

Material::Material(void) : Resource() {
    diffuse = Colour::WHITE;
    ambient = Colour::WHITE;
    specular = Colour::WHITE;
    texture = nullptr;
    alpha = 1.0f;
    ns = 1.0f;
    ni = 0;
    illum = 0;
    filename = "";
    materialName = "";
}

Material::Material(const Material &copy) : Resource(copy) {
    this->diffuse = copy.diffuse;
    this->ambient = copy.ambient;
    this->specular = copy.specular;
    this->texture = copy.texture;
    this->alpha = copy.alpha;
    this->ns = copy.ns;
    this->ni = copy.ni;
    this->illum = copy.illum;
    this->filename = std::string(copy.filename);
    this->materialName = std::string(copy.materialName);
}

Material::Material(int name, float shininess, Colour diffuse, Colour ambient, Colour specular,
    Texture *texture) : Resource(name) {
    this->diffuse = diffuse;
    this->ambient = ambient;
    this->specular = specular;
    this->texture = nullptr;
    this->setTexture(texture);
    this->alpha = diffuse.alpha;
    this->ns = shininess;
    this->ni = 0;
    this->illum = 0;
    this->filename = "";
    this->materialName = "";
}

Material::~Material(void) {
    texture = nullptr;
}

void Material::unload() {
    // The only thing to unload is basically the texture, so we just remove it from the resources manager.
    if (texture != nullptr) {
        ResourcesManager::releaseResource(texture->getName());
        texture = nullptr;
    }
}

Material &Material::operator=(const Material &other) {
    this->diffuse = other.diffuse;
    this->ambient = other.ambient;
    this->specular = other.specular;
    this->texture = other.texture;
    this->name = other.name;
    this->alpha = other.alpha;
    this->ns = other.ns;
    this->ni = other.ni;
    this->illum = other.illum;
    this->filename = std::string(other.filename);
    this->materialName = std::string(other.materialName);
    return *this;
}

void Material::setTexture(Texture *texture) {
    if (this->texture != nullptr && this->texture != texture) {
        ResourcesManager::releaseResource(texture->getName());
    }
    this->texture = texture;
    if (this->texture != nullptr) {
        this->texture->addUser();
    }
}

std::vector<Material*> Material::loadMaterialsFromFile(const std::string &filename) {
    std::vector<std::string> lines = FileIO::readTextFile(filename);
    std::vector<Material*> materials = std::vector<Material*>();
    if (lines.size() > 0) {
        Material *currentMaterial = nullptr;
        // Start reading through the lines
        for (unsigned i = 0; i < lines.size(); i++) {
            std::string line = lines[i];
            if (line.size() < 2) continue; // Make sure it's not an empty line

            // Trim the whitespaces at the beginning of the line, if present
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            char firstChar = line[0];
            char secondChar = line[1];

            switch (firstChar) {
            case 'n':
                // Creates a new material and sets it as the current material, pushing the current one to the vector
                char nameC[256];
                sscanf(line.c_str(),"%*s %s", &nameC);
                if (currentMaterial != nullptr) {
                    currentMaterial->loaded = true;
                    ResourcesManager::addResource(currentMaterial, true);
                    materials.emplace_back(currentMaterial);
                }
                currentMaterial = new Material();
                currentMaterial->materialName = std::string(nameC);
                break;
            case 'N':
                // Sets the Ni or Ns of the current material
                if (currentMaterial != nullptr) {
                    if (secondChar == 'i') {
                        sscanf_s(line.c_str(), "Ni %f", &(currentMaterial->ni));
                    } else if (secondChar == 's') {
                        sscanf_s(line.c_str(), "Ns %f", &(currentMaterial->ns));
                    }
                }
                break;
            case 'd':
                // Sets the alpha value of the current material
                if (currentMaterial != nullptr) {
                    sscanf_s(line.c_str(), "d %f", &(currentMaterial->alpha));
                }
                break;
            case 'K':
                // Sets the light colours of the current material
                if (currentMaterial != nullptr) {
                    if (secondChar == 'd') {
                        float r, g, b;
                        sscanf_s(line.c_str(), "Kd %f %f %f", &r, &g, &b);
                        currentMaterial->diffuse = Colour(r, g, b, 1.0f);
                    } else if (secondChar == 'a') {
                        float r, g, b;
                        sscanf_s(line.c_str(), "Ka %f %f %f", &r, &g, &b);
                        currentMaterial->ambient = Colour(r, g, b, 1.0f);
                    } else {
                        float r, g, b;
                        sscanf_s(line.c_str(), "Ks %f %f %f", &r, &g, &b);
                        currentMaterial->specular = Colour(r, g, b, 1.0f);
                    }
                }
                break;
            case 'i':
                // Sets the illumination value of the current material
                if (currentMaterial != nullptr) {
                    sscanf_s(line.c_str(), "illum %d", &(currentMaterial->illum));
                }
                break;
            case 'm':
                // Maps a texture to the current material
                if (currentMaterial != nullptr) {
                    char texFilename[150];
                    sscanf(line.c_str(), "map_Kd %s", &texFilename);
                    // TODO: think of a way to store the int name and associate it with the 6 textures in runtime
                    currentMaterial->setTexture(Texture::getOrCreate(ResourcesManager::generateNextName(),
                        std::string(texFilename), false));
                }
                break;
            }
        }
        if (currentMaterial != nullptr) {
            // Adds the last material to the vector
            currentMaterial->loaded = true;
            currentMaterial->valid = true;
            materials.emplace_back(currentMaterial);
        }
    }
    return materials;
}

bool Material::fileHasMaterial(const std::string &filename, const std::string &materialName) {
    std::vector<std::string> lines = FileIO::readTextFile(filename);
    if (lines.size() == 0) {
        return false; // Material file couldn't be opened
    }
    bool hasMaterial = false;
    for (unsigned i = 0; i < lines.size(); i++) {
        std::string line = lines[i];
        if (line.size() < 2) continue; // Make sure it's not an empty line

        // Trim the whitespaces at the beginning of the line, if present
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        char firstChar = line[0];

        if (firstChar == 'n') {
            char name[256];
            sscanf(line.c_str(),"%*s %s", &name);
            hasMaterial = std::string(name).compare(materialName) == 0;
            if (hasMaterial) return true; // If material is found, no need to finish reading the file
        }
    }
    return hasMaterial;
}