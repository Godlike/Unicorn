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
			Mesh::Mesh(): m_isVerticesSizeChanged(false)
			{
			}

			Mesh::~Mesh() {}

			std::uint32_t Mesh::VerticesSize() const
			{
				return static_cast<uint32_t>(m_vertices.size());
			}

			bool Mesh::IsSizeChanged() const 
			{
				return m_isVerticesSizeChanged;
			}

			void Mesh::SetVertices(const std::vector<Vertex>& newVertices)
			{
				if (newVertices.size() != m_vertices.size())
					m_isVerticesSizeChanged = true;
				m_vertices = newVertices;
				DataUpdated.emit();
			}
		}
	}
}