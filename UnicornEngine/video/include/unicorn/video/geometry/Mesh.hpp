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
				glm::vec2 pos;
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
				UNICORN_EXPORT std::uint32_t VerticesSize() const;
                UNICORN_EXPORT std::uint32_t IndicesSize() const;
				wink::signal<wink::slot<void()>> DataUpdated;
				void SetDrawData(const std::vector<Vertex>& newVertices, const std::vector<uint16_t>& indices);
                
                std::vector<Vertex> m_vertices;
                std::vector<uint16_t> m_indices;
			private:
				glm::mat4 m_model;
			};
		}
	}
}

#endif // UNICORN_VIDEO_GEOMETRY_MESH_HPP