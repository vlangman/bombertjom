#pragma once

#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"
#include <map>

class ResourceManager
{
private: 
    std::map <std::string, GameBoi::graphics::Shader *> m_shaders;
    std::map <std::string, GameBoi::graphics::Model *> m_models;
public:
    void loadModel(std::string modelName, std::string modelPath);
    void loadShader(std::string shaderName, std::string vertPath, std::string fragPath);

    float getScaling(void);

    GameBoi::graphics::Model *getModel(std::string modelName);
    GameBoi::graphics::Shader *getShader(std::string shaderName);
};