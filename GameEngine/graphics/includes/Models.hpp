#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <vector>

#include "Mesh.hpp"

namespace GameBoi { namespace graphics {

	class Model 
	{
	public:
		/*  Model Data  */
		std::vector<Textures> textures_loaded;
		std::vector<Mesh> meshes;
		bool gammaCorrection;
		glm::vec3 msizeMax;
		/*  Functions   */
		Model(std::string path, bool gamma = false);
		void		NewPostionAndScale(const glm::vec3 & position, const glm::vec3 & scale, const float & degrees);
		void		Position(const glm::vec3& position);
		void		Scale(const glm::vec3& scale);
		void		Rotate(const float& degrees);
		void		DrawAndSet(const Shader & shader, const std::string & name);
		glm::vec3	getMaxModelScaling(void);
		glm::vec3 getSize();
		
	private:
		std::string	directory;
		glm::mat4	_Matrix;
		glm::vec3	mSize;

		/*  Functions   */
		void	Draw(Shader shader);
		void	ResetMatrix();
		void	loadModel(std::string const path);
		void	processNode(aiNode *node, const aiScene *scene);
		Mesh	processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Textures> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	};
}}