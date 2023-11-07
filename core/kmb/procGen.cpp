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
				v.normal = ew::Normalize(ew::Vec3(v.pos.x, v.pos.y, v.pos.z));
				v.uv = ew::Vec2((float)(theta), (float)(phi));

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
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start);
				sphere.indices.push_back(start + columns);

				//Triangle 2
				sphere.indices.push_back(start + columns);
				sphere.indices.push_back(start + columns + 1);
				sphere.indices.push_back(start + 1);
			}
		}

		//bottom cap
		poleStart = sphere.vertices.size()-numSegments;
		sideStart = sphere.vertices.size() - (numSegments) - 1;
		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart - 1);
			sphere.indices.push_back(poleStart - i - 1);
			sphere.indices.push_back(sideStart - i- 1);
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
		topVertex.normal = ew::Normalize(ew::Vec3(topVertex.pos.x, topVertex.pos.y, topVertex.pos.z));
		cylinder.vertices.push_back(topVertex);

		//Top ring
		float thetaStep = 2 * 3.1415 / numSegments;

		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;

			v.pos = ew::Vec3(cos(theta) * radius, topY, sin(theta) * radius);

			v.normal = ew::Normalize(ew::Vec3(v.pos.x, v.pos.y, v.pos.z));
			//v.uv = ew::Vec2((float)(theta), (float)(phi));
			
			cylinder.vertices.push_back(v);
		}

		//Bottom ring
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;

			v.pos = ew::Vec3(cos(theta) * radius, bottomY, sin(theta) * radius);
			v.normal = ew::Normalize(ew::Vec3(v.pos.x, v.pos.y, v.pos.z));
			cylinder.vertices.push_back(v);
		}

		//Bottom center
		ew::Vertex bottomVertex;
		bottomVertex.pos = ew::Vec3(0, bottomY, 0);
		bottomVertex.normal = ew::Normalize(ew::Vec3(bottomVertex.pos.x, bottomVertex.pos.y, bottomVertex.pos.z));
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
		int sideStart = 1;

		int columns = numSegments + 1;

		for (int i = 0; i < columns; i++)
		{
			start = sideStart + i;
			//Triangle 1
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start);
			cylinder.indices.push_back(start + columns);

			//Triangle 2
			cylinder.indices.push_back(start + columns);
			cylinder.indices.push_back(start + columns + 1);
			cylinder.indices.push_back(start + 1);
		}

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

				ew::Vec3 normal = ew::Vec3((- 1.0 + (col / subdivisions)), -1.0 + (row / subdivisions)*2, 0);
				ew::Vec3 a = ew::Vec3(normal.z, normal.x, normal.y);
				ew::Vec3 b = ew::Cross(normal ,a);

				ew::Vertex v;
				v.pos = ew::Vec3(width * (col / subdivisions), 0, -height * (row / subdivisions));
				v.normal = ew::Vec3 (0,1,0);
				v.uv = ew::Vec2((col / subdivisions), (row / subdivisions));
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
