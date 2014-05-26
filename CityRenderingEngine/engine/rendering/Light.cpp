#include "Light.h"

Light::Light(void) {
    this->radius = 1.0f;
    this->intensity = 1.0f;
    this->position = Vector3(0, 0, 0);
    this->direction = Vector3(0, 0, 0);
    this->colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);
    this->type = LIGHT_POINT;
}

Light::Light(Colour colour, Vector3 position, Vector3 direction, float intensity, float radius, LightType type) {
    this->radius = radius;
    this->intensity = intensity;
    this->position = position;
    this->direction = direction;
    this->colour = colour;
    this->type = type;
}

Light::Light(Colour colour, Vector3 position, float intensity, float radius) {
    this->radius = radius;
    this->intensity = intensity;
    this->position = position;
    this->direction = Vector3(0, 0, 0);
    this->colour = colour;
    this->type = LIGHT_POINT;
}

Light::Light(Colour colour, Vector3 position, Vector3 direction, float intensity, float radius) {
    this->radius = radius;
    this->intensity = intensity;
    this->position = position;
    this->direction = direction;
    this->colour = colour;
    this->type = LIGHT_SPOT;
}

Light::Light(Colour colour, Vector3 direction, float intensity) {
    this->radius = 1.0f;
    this->intensity = intensity;
    this->position = Vector3(0, 0, 0);
    this->direction = direction;
    this->colour = colour;
    this->type = LIGHT_DIRECTIONAL;
}

void Light::updateShaderParameters(Shader *shader) {
    if (shader != nullptr) {
        ShaderParameter *parameterPos = shader->getShaderParameter("lightSource.position");
        if (parameterPos == nullptr) {
            shader->addShaderParameter(new ShaderParameter("lightSource.position", 
                PARAMETER_VECTOR_3, new Vector3(position)));
            shader->addShaderParameter(new ShaderParameter("lightSource.direction", 
                PARAMETER_VECTOR_3, new Vector3(direction)));
            shader->addShaderParameter(new ShaderParameter("lightSource.colour", 
                PARAMETER_VECTOR_3, new Vector3(colour.getColourVec3())));
            shader->addShaderParameter(new ShaderParameter("lightSource.intensity",
                PARAMETER_FLOAT, new float(intensity)));
            shader->addShaderParameter(new ShaderParameter("lightSource.radius", PARAMETER_FLOAT, new float(radius)));
            shader->addShaderParameter(new ShaderParameter("lightSource.type", PARAMETER_INT, new int(type)));
        } else {
            parameterPos->setValue(new Vector3(position));
            shader->getShaderParameter("lightSource.direction")->setValue(new Vector3(direction));
            shader->getShaderParameter("lightSource.colour")->setValue(new Vector3(colour.getColourVec3()));
            shader->getShaderParameter("lightSource.radius")->setValue(new float(radius));
            shader->getShaderParameter("lightSource.intensity")->setValue(new float(intensity));
            shader->getShaderParameter("lightSource.type")->setValue(new int(type));
        }
    }
}