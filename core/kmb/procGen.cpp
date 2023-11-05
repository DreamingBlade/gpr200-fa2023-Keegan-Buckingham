#include "procGen.h"

namespace kmb {

	ew::MeshData createSphere(float radius, int numSegments) 
	{
		ew::MeshData sphere;

		float thetaStep = (2 * 3.1415) / numSegments;
		float phiStep = 3.1415 / numSegments;

		for (int row = 0; row <= numSegments; row++)
		{
			//First and last row converge at poles
			float phi = row * phiStep;

			//Duplicate column for each row
			for (int col = 0; col <= numSegments; col++)
			{
				float theta = col * thetaStep;

				ew::Vertex v;

				v.pos = ew::Vec3(radius * cos(theta) * sin(phi), radius * cos(phi), radius * sin(theta) * sin(phi));

				sphere.vertices.push_back(v);
			}
		}

		int poleStart = 0;
		int sideStart = numSegments + 1;

		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(poleStart + i);
			sphere.indices.push_back(sideStart + i + 1);
		}

		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int row = 1; row < numSegments - 1; row++)
		{
			for (int col = 0; col < numSegments; col++)
			{
				int start = row * columns + col;

				//Triangle 1
				sphere.indices.push_back(start);
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columns);

				//Triangle 2
				sphere.indices.push_back(start + columns);
				sphere.indices.push_back(start + columns + 1);
				sphere.indices.push_back(start + 1);
			}
		}
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

		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;

			v.pos = ew::Vec3(cos(theta) * radius, topY, sin(theta) * radius);
			
			cylinder.vertices.push_back(v);
		}

		//Bottom ring
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;

			v.pos = ew::Vec3(cos(theta) * radius, bottomY, sin(theta) * radius);

			cylinder.vertices.push_back(v);
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
		start = numSegments + 1;
		center = cylinder.vertices.size() - 1;

		for (int i = 0; i <= numSegments; i++)
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
