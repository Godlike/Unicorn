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
			Mesh::Mesh() : model(glm::mat4())
			{
			}

			Mesh::~Mesh() {}

			void Mesh::Updated() const
			{				
				DataUpdated.emit();
			}
		}
	}
}