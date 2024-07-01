#pragma once
struct MobDistance
{
    Mob* mobPtr;
    int distance;
    MobDistance() = default;
    MobDistance(Mob* m, int i)
    {
	  mobPtr = m;
	  distance = i;
    }
};

struct lessThanKey
{
    inline bool operator() (const MobDistance& md1, const MobDistance& md2)
    {
	  return (md1.distance < md2.distance);
    }
};

class TabTarget
{
public:
    std::vector<MobDistance> distanceList;
    int currentTargetIndex;
    int cycleCounter;

    void tabTarget();
    void reset();

private:
    void updateDistanceList();
};
