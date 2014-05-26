#include "ShaderParameter.h"

ShaderParameter::ShaderParameter(std::string variableName, ParameterType type, void *value) {
    this->parameterName = variableName;
    this->parameterType = type;
    this->value = value;
    this->valueSize = 1;
}

ShaderParameter::~ShaderParameter() {
    delete value;
    value = nullptr;
}

void ShaderParameter::setValue(void *value) {
    if (this->value != nullptr) delete this->value;
    this->value = value;
    valueChanged = true;
}