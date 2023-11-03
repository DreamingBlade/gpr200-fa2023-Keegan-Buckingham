#include "procGen.h"

namespace kmb {

	ew::MeshData createSphere(float radius, int numSegments) 
	{
		ew::MeshData sphere;

		return sphere;

	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData cylinder;
		//y = 0 is centered
		float topY = height / 2;
		float bottomY = -topY;

		//Top center
		ew::Vertex topVertex;
		topVertex.pos = ew::Vec3(0, topY, 0);
		cylinder.vertices.push_back(topVertex);

		//Top ring
		float thetaStep = 2 * 3.1415 / numSegments;

		for (i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.x = cos(theta) * radius;
			v.z = sin(theta) * radius;
			v.y = topY;

			v.pos = ew::Vec3(v.x,v.z,v.y);
			
			cylinder.push_back(v);
		}

		//Bottom ring
		for (i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.x = cos(theta) * radius;
			v.z = sin(theta) * radius;
			v.y = bottomY;

			v.pos = ew::Vec3(v.x, v.z, v.y);

			cylinder.push_back(v);
		}

		//Bottom center
		ew::Vertex bottomVertex;
		bottomVertex.pos = ew::Vec3(0, bottomY, 0);
		cylinder.vertices.push_back(bottomVertex);

		//Top Cap
		int start = 1;
		int center = 0;

		for (int i = 0; i <= numSegments; i++)
		{
			cylinder.indices.push_back(start+i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
		}

		//Bottom Cap
		int start = 1;
		int center = 0;

		for (int = 0; i <= numSegments; i++)
		{
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
		}

		//Sides

		return cylinder;
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
