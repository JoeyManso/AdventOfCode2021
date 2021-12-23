#include "Days.h"
#include "AdventUtils.h"
#include "Point3D.h"
#include <regex>
#include <set>
#include <vector>

using namespace std;

const regex RX_SCANNER(R"(--- scanner (\d+) ---)");
const regex RX_COORD(R"((-?\d+),(-?\d+),(-?\d+))");

using RelativePoints = vector<Point3D>;

const set<Point3D> ROTATION_OFFSETS = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 2},
    {0, 0, 3},
    {0, 1, 0},
    {0, 1, 1},
    {0, 1, 2},
    {0, 1, 3},
    {0, 2, 0},
    {0, 2, 1},
    {0, 2, 2},
    {0, 2, 3},
    {0, 3, 0},
    {0, 3, 1},
    {0, 3, 2},
    {0, 3, 3},
    {1, 0, 0},
    {1, 0, 1},
    {1, 0, 2},
    {1, 0, 3},
    {1, 2, 0},
    {1, 2, 1},
    {1, 2, 2},
    {1, 2, 3},
};

Point3D GetRotated(const Point3D& pBase, const Point3D& rot)
{
    Point3D p = pBase;
    for (int i = 0; i < rot.x; ++i)       
    {
        // About x
        auto y = p.y; 
        p.y = -p.z;
        p.z = y;
    }

    for (int i = 0; i < rot.y; ++i)
    {
        // About y
        auto x = p.x; 
        p.x = -p.z;
        p.z = x;
    }

    for (int i = 0; i < rot.z; ++i)
    {
        // About z
        auto x = p.x; 
        p.x = -p.y;
        p.y = x;
    }
    return p;
}

struct Scanner
{
	Scanner()
		: Scanner(0)
	{
	}

	Scanner(size_t idx)
		: scannerIdx(idx), relativePointsIdx(-1), relativePointsOffset(Vector3D(0,0,0)), relativePointsArray({}), relativeScanner(nullptr)
	{
	}

    bool HasSolved() const
    {
        return relativeScanner != nullptr;
    }

	void AddPoint(const Point3D& p)
	{
        size_t i = 0;
		for (const Point3D& rotationOffset : ROTATION_OFFSETS)
        {
            if (relativePointsArray.size() <= i)
            {
                relativePointsArray.push_back({});
            }
            relativePointsArray[i].push_back(GetRotated(p, rotationOffset));
            ++i;
        }
	}

    vector<Point3D> GetBeaconPoints() const
    {
        vector<Point3D> beaconPoints = {};
        const Vector3D globalOffset = GetGlobalOffset();
        for (const Point3D& relativePoint : relativePointsArray[relativePointsIdx])
        {
            beaconPoints.push_back(relativePoint + globalOffset);
        }
        return beaconPoints;
    }

    Vector3D GetGlobalOffset() const
    {
        if (scannerIdx == 0)
        {
            return Vector3D();
        }
        return relativePointsOffset + relativeScanner->GetGlobalOffset();
    }

    // Index of this scanner via Input
	size_t scannerIdx;
    // Index of "correct" relative points, found via overlap check
    size_t relativePointsIdx;
    // Offset to relative points, calculated against relative scanner
    Vector3D relativePointsOffset;
    // Array of all points in each orientation
	vector<RelativePoints> relativePointsArray;
    // Scanner used as a reference to find the points in this scanner
    const Scanner* relativeScanner;
};

bool CheckOverlap(const RelativePoints& relativePointsA, const RelativePoints& relativePointsB, const Vector3D& offset)
{
    size_t overlapCount = 0;
    for (const Point3D& pointB : relativePointsB)
    {
        if (contains(relativePointsA, pointB + offset))
        {
            ++overlapCount;
            if (overlapCount >= 12)
            {
                return true;
            }
        }
    }
    return false;
}

bool CheckOverlap(Scanner& scannerA, Scanner& scannerB)
{
    for (size_t i = 0; i < scannerA.relativePointsArray.size(); ++i)
    {
        // Skip if the scanner was solved and this is the wrong idx
        if (!scannerA.HasSolved() || scannerA.relativePointsIdx == i)
        {
            for (size_t j = 0; j < scannerB.relativePointsArray.size(); ++j)
            {
                // Skip if the scanner was solved and this is the wrong idx
                if (!scannerB.HasSolved() || scannerB.relativePointsIdx == j)
                {
                    const RelativePoints& relativePointsA = scannerA.relativePointsArray[i];
                    const RelativePoints& relativePointsB = scannerB.relativePointsArray[j];
                    for (const Point3D& pointA : relativePointsA)
                    {
                        for (const Point3D& pointB : relativePointsB)
                        {
                            // Apply this offset to the points of B and check for overlap in A
                            const Vector3D offset = pointA - pointB;
                            if (CheckOverlap(relativePointsA, relativePointsB, offset))
                            {
                                if (!scannerA.HasSolved())
                                {
                                    scannerA.relativePointsIdx = i;
                                    scannerA.relativePointsOffset = Vector3D() - offset;
                                    scannerA.relativeScanner = &scannerB;
                                }
                                
                                if (!scannerB.HasSolved())
								{
									scannerB.relativePointsIdx = j;
									scannerB.relativePointsOffset = offset;
                                    scannerB.relativeScanner = &scannerA;
								}
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

template <>
void Run<Day19>(Part part, istream& is, ostream& os)
{
	string arg;
	
	vector<Scanner> scanners;
	while(getline(is, arg))
	{
		smatch match;
		if (regex_search(arg, match, RX_SCANNER))
		{
			scanners.push_back(Scanner(stoi(match.str(1))));
		}
		else if (regex_search(arg, match, RX_COORD))
		{
			Point3D p = Point3D(stoi(match.str(1)), stoi(match.str(2)), stoi(match.str(3)));
			scanners.back().AddPoint(p);
		}
	}
	
    set<size_t> solvedScannerIndices = {};
    while(solvedScannerIndices.size() < scanners.size())
	{
		for (size_t i = 0; i < scanners.size(); ++i)
		{
			for (size_t j = i + 1; j < scanners.size(); ++j)
			{
				// Ensure scanners will chain back to the starting scanner as the base reference point
				// Skip check if both scanners are already solved
				if (
					    (i == 0 || scanners[i].HasSolved() || scanners[j].HasSolved()) 
                        && !(scanners[i].HasSolved() && scanners[j].HasSolved())
                        && CheckOverlap(scanners[i], scanners[j])
					)
				{
                    solvedScannerIndices.emplace(i);
                    solvedScannerIndices.emplace(j);
				}
			}
		}
	}

	if(part == Part01)
	{
        set<Point3D> beaconPoints = {};
        for (const Scanner& scanner : scanners)
        {
            for (const Point3D& p : scanner.GetBeaconPoints())
            {
                beaconPoints.emplace(p);
            }
        }

        //for (const Point3D& beaconPoint : beaconPoints)
        //{
        //    os << beaconPoint << endl;
        //}
        os << "Total Beacons: " << beaconPoints.size() << endl;
	}
	else if(part == Part02)
	{
        int maxDist = 0;
        for (size_t i = 0; i < scanners.size(); ++i)
		{
			for (size_t j = i + 1; j < scanners.size(); ++j)
			{
                maxDist = max(maxDist, Vector3D::Manhattan(scanners[i].GetGlobalOffset(), scanners[j].GetGlobalOffset()));
			}
		}
        cout << "Max Dist: " << maxDist << endl;
	}
}