#include "Entity.hpp"

Entity::Entity()
{
    m_shader = NULL;
    m_model = NULL;
    type = BOX_ENTITY;
    mRotation = 0;
}

/*void Entity::setShader(std::string vertPath, std::string fragPath)
{
    if (m_shader != NULL)
        delete m_shader;

    m_shader = new GameBoi::graphics::Shader(vertPath, fragPath);
    m_shader->use();
}
void Entity::setModel(std::string modelPath)
{
    if (m_model != NULL)
        delete m_model;
    
    m_model = new GameBoi::graphics::Model(modelPath);
}*/

glm::vec3 Entity::getPosition()
{
    return mPosition;
}

void Entity::setShader(GameBoi::graphics::Shader *shader)
{
    m_shader = shader;
    m_shader->use();
}
void Entity::setModel(GameBoi::graphics::Model *model)
{
    m_model = model;
}

void Entity::setPosition(float x, float y, float z)
{
    mPosition = glm::vec3(x, y, z);
}
void Entity::setScale(float x, float y, float z)
{
    mScale = glm::vec3(x, y, z);
}

void Entity::render( glm::mat4 &projection,  glm::mat4 &view)
{
    m_shader->setMat4("projection", projection);
    m_shader->setMat4("view", view);
    
        // calculate the model matrix for each object and pass it to shader before drawing
    m_model->NewPostionAndScale(mPosition, mScale, mRotation);
    m_model->DrawAndSet(*m_shader, "model");   
}

void Entity::rotate(float rot)
{
    mRotation = rot;
}

GameBoi::graphics::Shader * Entity::getShader()
{
    return m_shader;
}
GameBoi::graphics::Model * Entity::getModel()
{
    return m_model;
}