#include "Days.h"
#include "Point3D.h"
#include <regex>
#include <vector>

using namespace std;

//on x=10..12,y=10..12,z=10..12
const regex RX_COMMAND(R"((\w+) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))");

struct Cuboid
{
	Cuboid(bool _isOn, int64_t _sign, const Point3D& _cornerMin, const Point3D& _cornerMax)
		: isOn(_isOn), sign(_sign), cornerMin(_cornerMin), cornerMax(_cornerMax)
	{
	}
	
	Cuboid()
		: Cuboid(false, 1, {}, {})
	{
	}

	bool IsWithinBounds(int64_t bounds) const
	{
		return cornerMin.x >= -bounds && cornerMax.x <= bounds
			&& cornerMin.y >= -bounds && cornerMax.y <= bounds
			&& cornerMin.z >= -bounds && cornerMax.y <= bounds;
	}

	int64_t GetVolume() const
	{
		return (sign
			* (cornerMax.x - cornerMin.x + 1)
			* (cornerMax.y - cornerMin.y + 1)
			* (cornerMax.z - cornerMin.z + 1)
		);
	}

	Cuboid Intersect(const Cuboid& other) const
	{
		const Point3D intersectMin = Point3D(
			max(cornerMin.x, other.cornerMin.x),
			max(cornerMin.y, other.cornerMin.y),
			max(cornerMin.z, other.cornerMin.z)
		);
		const Point3D intersectMax = Point3D(
			max(min(cornerMax.x, other.cornerMax.x), intersectMin.x - 1),
			max(min(cornerMax.y, other.cornerMax.y), intersectMin.y - 1),
			max(min(cornerMax.z, other.cornerMax.z), intersectMin.z - 1)
		);
		return Cuboid(
			true,
			-1 * sign * other.sign,
			intersectMin,
			intersectMax
		);
	}

	bool isOn;
	int64_t sign;
	Point3D cornerMin;
	Point3D cornerMax;
};

template <>
void Run<Day22>(Part part, istream& is, ostream& os)
{
	vector<Cuboid> initialCuboids = {};
	
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_COMMAND);
		initialCuboids.push_back(Cuboid(
			match.str(1) == "on",
			1,
			Point3D(stoi(match.str(2)), stoi(match.str(4)), stoi(match.str(6))),
			Point3D(stoi(match.str(3)), stoi(match.str(5)), stoi(match.str(7)))
		));
	}

	int64_t cubeCount = 0;
	vector<Cuboid> cuboids = {};
    for (const Cuboid& initialCuboid : initialCuboids) 
    {
		if (part == Part02 || initialCuboid.IsWithinBounds(50))
		{
			// Just maintain a collection of cuboids, of increasing numbers of intersected initial cuboids
			// Sign changes on each new intersection. The logic is that the volume of:
			// - A and B combined is vol(A) + vol(B) - vol(A|B). 
			// - A, B and C combined is vol(A) + vol(B) - vol(A|B) + vol(C) - vol(A|C) - vol(B|C) + vol(A|B|C). 
			// - ...
			const size_t cuboidCount = cuboids.size();
			for (size_t i = 0; i < cuboidCount; ++i)
			{
				const Cuboid intersectingCuboid = cuboids[i].Intersect(initialCuboid);
				int64_t intersectVolume = intersectingCuboid.GetVolume();
				if (intersectVolume != 0)
				{
					cubeCount += intersectVolume;
					cuboids.push_back(intersectingCuboid);
				}
			}
 
			// Any intersection which started from a negative volume makes no contribution to the total volume. Taking
			// out the whole chain of intersection is just a case of not placing the original negative volume in the 
			// collection.
			if (initialCuboid.isOn)
			{
				cubeCount += initialCuboid.GetVolume();
				cuboids.push_back(initialCuboid);
			}
		}
    }
	os << "Total cubes: " << cubeCount << endl;
}