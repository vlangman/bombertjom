#include "ResourceManager.hpp"

void ResourceManager::loadModel(std::string modelName, std::string modelPath)
{
    m_models[modelName] = new GameBoi::graphics::Model(modelPath);
}
void ResourceManager::loadShader(std::string shaderName, std::string vertPath, std::string fragPath)
{
    m_shaders[shaderName] = new GameBoi::graphics::Shader(vertPath, fragPath);
}

GameBoi::graphics::Model *ResourceManager::getModel(std::string modelName)
{
    return m_models[modelName];
}

GameBoi::graphics::Shader *ResourceManager::getShader(std::string shaderName)
{
    return m_shaders[shaderName];
}

float ResourceManager::getScaling(void){
	float max_x = 0.0f;
	// Create a map iterator and point to beginning of map
	std::map<std::string, GameBoi::graphics::Model *>::iterator it = m_models.begin();
 
	// Iterate over the map using c++11 range based for loop
	for (std::pair<std::string, GameBoi::graphics::Model *> element : m_models) {
		// Accessing VALUE from element.
		if (element.second->getMaxModelScaling().x > max_x){
			max_x = element.second->getMaxModelScaling().z;
		}
	}


	return max_x;
}