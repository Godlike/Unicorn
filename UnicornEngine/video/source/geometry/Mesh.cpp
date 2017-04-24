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
			Mesh::Mesh() {}

			Mesh::~Mesh() {}

		    std::uint32_t Mesh::VerticesSize() const
		    {
		        return static_cast<uint32_t>(m_vertices.size());
		    }
		}
	}
}