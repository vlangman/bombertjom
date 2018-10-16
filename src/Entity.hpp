#pragma once
#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"
#include <vector>


enum ENTITY_TYPE 
{
    BOX_ENTITY, 
    PLAYER_ENTITY
};

class Entity 
{
private:
    GameBoi::graphics::Shader *m_shader;
    GameBoi::graphics::Model *m_model;

    glm::vec3 mPosition;
    glm::vec3 mScale;
    float mRotation;
public:
    ENTITY_TYPE type;
    Entity();

    //void setShader(std::string vertPath, std::string fragPath);
    //void setModel(std::string modelPath);

    void setShader(GameBoi::graphics::Shader *shader);
    void setModel(GameBoi::graphics::Model *model);

    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);

    void rotate(float rot);

    glm::vec3 getPosition();

    void render( glm::mat4 &projection,  glm::mat4 &view);

    GameBoi::graphics::Shader *getShader();
    GameBoi::graphics::Model *getModel();

};