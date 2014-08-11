/*
 * Authors: Rodrigo Castro Azevedo
 * Date: 25/05/2014
 *
 * Description: This class encapsulates a single Shader Parameter, a variable that needs to be sent to the current
 * Shader. It exists to facilitate the management of these variables, and provide a safe environment to correctly feed
 * these variables with data and upload it to OpenGL.
 */

#pragma once

#include <string>

/* This tells which type is this parameter. This should be used to know how to cast value to a usable type. */
enum ParameterType {
    PARAMETER_INT,
    PARAMETER_FLOAT,
    PARAMETER_DOUBLE,
    PARAMETER_VECTOR_2,
    PARAMETER_VECTOR_3,
    PARAMETER_VECTOR_4,
    PARAMETER_MATRIX_3,
    PARAMETER_MATRIX_4
};

class ShaderParameter {
public:

    /* The name of the parameter (variable) inside the Shader. */
    std::string parameterName;

    /*
     * The type of this parameter. This can be changed at will and does not change the behaviour of this class, but
     * should be used to know how to cast value to a usable type and which OpenGL method to call to upload the value.
     */
    ParameterType parameterType;

    /* The size of the parameter, in case it's an array. Defaults to 1. */
    int valueSize;

    ShaderParameter(std::string variableName, ParameterType type, void *value);
    ~ShaderParameter(void);

    /*
     * Sets a new value for this Shader Parameter, changing the flag valueChanged to true. This will destroy the
     * previous value, so any value being entered here should be copied before. The deletePrevious bool, if set to
     * true, deletes the previous value from memory, if there was any.
     */
    void setValue(void *value, bool deletePrevious);

    /* Returns the current value of this Shader Parameter. */
    void *getValue() { return value; }

    void setValueChanged(bool valueChanged) { this->valueChanged = valueChanged; }
    bool hasValueChanged() { return valueChanged; }

protected:

    /* The value that this parameter holds. It can be set to any type that is supported by GLSL. */
    void *value;

    /*
     * This bool tells if the value of the ShaderParameter has changed since the last time it was uploaded to OpenGL.
     * This is set to false everytime it's uploaded, and if the value changes, it's set to true. When the Shader gets
     * the value to upload it, it first checks if this bool was set to true, if not, there's no reason to upload the
     * same value again.
     */
    bool valueChanged;
};