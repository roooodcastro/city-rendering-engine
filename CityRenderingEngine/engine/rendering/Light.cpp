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
            shader->addShaderParameter(new ShaderParameter("lightSource.position", PARAMETER_VECTOR_3, &position));
            shader->addShaderParameter(new ShaderParameter("lightSource.direction", PARAMETER_VECTOR_3, &direction));
            shader->addShaderParameter(new ShaderParameter("lightSource.colour", 
                PARAMETER_VECTOR_3, new Vector3(colour.getColourVec3())));
            shader->addShaderParameter(new ShaderParameter("lightSource.intensity", PARAMETER_FLOAT, &intensity));
            shader->addShaderParameter(new ShaderParameter("lightSource.radius", PARAMETER_FLOAT, &radius));
            shader->addShaderParameter(new ShaderParameter("lightSource.type", PARAMETER_INT, &type));
        } else {
            parameterPos->setValue(&position, false);
            shader->getShaderParameter("lightSource.direction")->setValue(&direction, false);
            shader->getShaderParameter("lightSource.colour")->setValue(new Vector3(colour.getColourVec3()), true);
            shader->getShaderParameter("lightSource.radius")->setValue(&radius, false);
            shader->getShaderParameter("lightSource.intensity")->setValue(&intensity, false);
            shader->getShaderParameter("lightSource.type")->setValue(&type, false);
        }
    }
}