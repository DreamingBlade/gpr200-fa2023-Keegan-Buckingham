#include "procGen.h"

namespace kmb {

	ew::MeshData createSphere(float radius, int numSegments) 
	{

	}
	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{

	}
	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData plane;
		//Getting plane vertices
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				ew::Vertex v;
				v.pos = ew::Vec3(width * (col / subdivisions), 0, -height * (row / subdivisions));
				plane.vertices.push_back(v);
			}
		}
		//Getting plane Indices
		int columns = subdivisions + 1;
		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
				int start = row * columns + col;
				//Bottom right triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + 1);
				plane.indices.push_back(start + columns + 1);
				//Top left triangle
				plane.indices.push_back(start + columns + 1);
				plane.indices.push_back(start + columns);
				plane.indices.push_back(start);
			}
		}

		return plane;
	}

}