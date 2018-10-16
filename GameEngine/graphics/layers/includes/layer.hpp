#pragma once

#include "../../includes/renderer2d.hpp"
#include "../../includes/renderable2d.hpp"

namespace GameBoi { namespace graphics {

	class Layer {
	
	protected:
		Renderer2D* m_Renderer;
		std::vector<Renderable2D*> m_Renderables;
		Shader*		m_Shader;
		glm::mat4	m_ProjectionMatrix;

	protected:
		Layer(Renderer2D* renderer, Shader* shader, glm::mat4 projectionMatrix);
	public:
		virtual	~Layer();
		virtual	void	add(Renderable2D* renderable);
		virtual	void	render();
	};
}}