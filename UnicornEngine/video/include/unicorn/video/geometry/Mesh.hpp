/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_GEOMETRY_MESH_HPP
#define UNICORN_VIDEO_GEOMETRY_MESH_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <wink/signal.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace unicorn
{
	namespace video
	{
		namespace geometry
		{
			struct Vertex {
				glm::vec3 pos;
				glm::vec3 color;
			};

			/**
			 * \brief 
			 */
			class Mesh
			{
			public:
				UNICORN_EXPORT Mesh();
				UNICORN_EXPORT ~Mesh();
				void Updated() const;
                wink::signal<wink::slot<void()>> DataUpdated;
                
                std::vector<Vertex> vertices;
                std::vector<uint16_t> indices;
                glm::mat4 model;
			};
		}
	}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_HPP