#include "../includes/group.hpp"

namespace GameBoi { namespace graphics {

	Group::Group(const glm::mat4& transform) : m_TransformationMatrix(transform) {

	}

	void	Group::Submit(Renderer2D* renderer) const  {

		renderer->push(m_TransformationMatrix);
		for (const Renderable2D* renderable : m_Renderables)
			renderable->Submit(renderer);
		
		renderer->pop();
	}
	void	Group::add(Renderable2D* renderable) {
		m_Renderables.push_back(renderable);
	}

}}

