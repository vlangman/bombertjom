#pragma once

#include "../../includes/renderable2d.hpp"

namespace GameBoi { namespace graphics {

	class Group : public Renderable2D {
	private:
		std::vector<Renderable2D*> m_Renderables;
		glm::mat4	m_TransformationMatrix;
	public:
		Group(const glm::mat4& transform);
		void			add(Renderable2D* renderable);
		virtual void	Submit(Renderer2D* renderer) const;
	};
}}