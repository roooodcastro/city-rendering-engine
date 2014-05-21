/*
 * Authors: TheCPlusPlusGuy (http://www.youtube.com/user/thecplusplusguy) and Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: This class reads a .mtl file (Wavefront obj material file) and stores its attributes. The material
 * described here should be used with the models to correctly render them, using different textures and light
 * properties, etc. To be valid and work, a Material must have either a diffuse colour or a texture, otherwise the
 * fragment shader won't know how to shade the pixels and it won't render anything. The Model class will actually
 * prevent faces with invalid materials to be rendered, so a potential error won't occur.
 *
 * Note: This class cannot correctly load materials which contain more than one texture map. It will
 * only load the texture map associated with the diffuse light (map_Kd)
 */

#pragma once

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "Texture.h"
#include "Colour.h"
#include "../ResourcesManager.h"
#include "../input/FileIO.h"

class Texture;

class Material : public Resource {
public:

    Material(void);
    Material(const Material &copy);
    Material(const char* name, float alpha, float ns, float ni, Colour &diffuse, Colour &ambient, Colour &specular,
        int illum, Texture *texture);
    ~Material(void);

    /*
     * This function should load the Material, but as the same material file can have multiple materials, loading of
     * materials can only be done in batch, which defeats the purpose of load(). This function here does nothing.
     */
    virtual void load() {}

    /* This function will unload the material from memory. */
    virtual void unload();

    /*
     * Loads one or more materials from a .mtl file. It returns a vector of materials because a single material file
     * can define more than one material.
     */
    static std::vector<Material*> loadMaterialsFromFile(const char *filename);

    /* Checks if a .mtl file defines a material with the name specified. */
    static bool fileHasMaterial(const char *filename, const char *materialName);

    /* General getters and setters */
    void setDiffuse(Colour &diffuse) { this->diffuse = diffuse; }
    Colour getDiffuse() { return diffuse; }
    void setAmbient(Colour &ambient) { this->ambient = ambient; }
    Colour getAmbient() { return ambient; }
    void setSpecular(Colour &specular) { this->specular = specular; }
    Colour getSpecular() { return specular; }
    void setTexture(Texture *texture) { this->texture = texture; }
    Texture *getTexture() { return texture; }
    void setAlpha(float alpha) { this->alpha = alpha; }
    float getAlpha() { return alpha; }
    void setNs(float ns) { this->ns = ns; }
    float getNs() { return ns; }
    void setNi(float ni) { this->ni = ni; }
    float getNi() { return ni; }
    void setIllum(int illum) { this->illum = illum; }
    int getIllum() { return illum; }

    Material &operator=(const Material &other);

protected:

    std::string filename; // File name of this material
    float alpha; // Transparency of this material
    float ns; // Weight of the specular light (the bigger the shinier)
    float ni; // Optical Density (Index of Refraction)
    Colour specular; // Colour of the specular light
    Colour ambient; // Colour of the ambient light
    Colour diffuse; // Colour of the diffuse light
    int illum; // Illumination model, not used
    Texture *texture; // The texture used for this material
};