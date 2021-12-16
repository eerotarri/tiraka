// Datastructures.hh
//
// Student name: Eero Tarri
// Student email: eero.tarri@tuni.fi
// Student number: H283568

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <deque>
#include <queue>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Iterates through unordered_map to delete
    // pointers which is linear and then clears the datastructure with
    // unordered_map::clear which is linear
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations


    // Estimate of performance: O(N²)
    // Short rationale for estimate: Iterates through the elements of unordered_map which is linear
    // and uses unordered_map::clear which is also linear
    void clear_roads();

    // Estimate of performance: O(N³) averages theta O(N²)
    // Short rationale for estimate: Iterates through vertices then iterates through their
    // adjacent vertices aka edges. For each adjacent vertice uses unordered_set::insert which is
    // linear at worst case but averages at constant.
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: // O(N)
    // Short rationale for estimate: Uses town_exist_ and std::find which are linear
    // and unorderedset::insert which is linear at worst but averages constant
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: // O(N)
    // Short rationale for estimate: Uses town_exists_ which is linear and iterates through
    // edges of given town which is linear at worst.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: // O(N+K)
    // Short rationale for estimate: Contains DFS algorithm which has complecity of
    // amountOfVertices N plus amountOfEdges K
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: // O(N)
    // Short rationale for estimate: Uses town_exist_ and std::find which are linear
    // and unordered_set::erase which is linear at worst but averages constant
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: // O(N+K)
    // Short rationale for estimate: Contains BFS algorithm which has complecity of
    // amountOfVertices N plus amountOfEdges K
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: // O(N+K)
    // Short rationale for estimate: Contains DFS algorithm which has complecity of
    // amountOfVertices N plus amountOfEdges K
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: // O((N + K)*log(N))
    // Short rationale for estimate: Contains DIJKSTRA algoritghm which has complexity
    // of product of amountOfVertices N plus amountOfEdges K times log(N)
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:

    enum Colour {WHITE, GRAY, BLACK};

    struct Town {
        TownID id_ = NO_TOWNID;
        Name name_ = NO_NAME;
        Coord location_ = NO_COORD;
        int tax_ = 0;
        TownID  masterid_ = NO_TOWNID;
        std::vector<TownID> town_vassals = {};

        int d = NO_VALUE; // Distance
        Town* path_back = nullptr; // pi
        Colour colour = WHITE; // Colour

        std::unordered_set<TownID> neighbours_ = {};
    };

    std::unordered_map<TownID, Town*> towns_ = {};

    // Estimate of performance: O(N)
    // Short rationale for estimate: recursively iterates trough
    // subtree and performs constant operations on all of the nodes
    int recursive_tax_(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses std::find
    bool town_exist_(TownID town);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Only constant operations
    int distance_(Coord location1, Coord location2);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Checks path back which is at worst trough every vertice
    // and uses std::reverse which is linear
    std::vector<TownID> get_path_(TownID id, TownID lasttown = NO_TOWNID);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Only constant operations
    void relax(TownID u, TownID v);

};

#endif // DATASTRUCTURES_HH
