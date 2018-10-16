#pragma once

#include "layer.hpp"
#include "../../includes/batchedrenderer2D.hpp"

namespace GameBoi { namespace graphics {

	class TileLayer : public Layer {
	
	public:
		TileLayer(Shader* shader);
		~TileLayer();
	};
}}