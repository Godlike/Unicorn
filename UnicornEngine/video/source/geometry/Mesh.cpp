/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/geometry/Mesh.hpp>

namespace unicorn
{
	namespace video
	{
		namespace geometry
		{
			Mesh::Mesh() : m_model(glm::mat4())
			{
			}

			Mesh::~Mesh() {}

			std::uint32_t Mesh::VerticesSize() const
			{
				return static_cast<uint32_t>(m_vertices.size());
			}

			std::uint32_t Mesh::IndicesSize() const
			{
				return static_cast<uint32_t>(m_indices.size());
			}

			void Mesh::SetDrawData(const std::vector<Vertex>& newVertices, const std::vector<uint16_t>& indices)
			{				
				m_vertices = newVertices;
				m_indices = indices;
				DataUpdated.emit();
			}
		}
	}
}