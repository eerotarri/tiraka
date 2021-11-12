// Datastructures.hh
//
// Student name: Eero Tarri
// Student email: eero.tarri@tuni.fi
// Student number: H283568

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <map>
#include <vector>
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

    // Estimate of performance: O(1)
    // Short rationale for estimate: only uses .size() which is constant
    unsigned int town_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate:only uses .clear() which is linear
    void clear_all();

    // Estimate of performance: O(log(N))
    // Short rationale for estimate: Funtion uses constant orders and couple of
    // logarithmic functions.
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist which is O(N)
    Name get_town_name(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist which is O(N)
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist which is O(N)
    int get_town_tax(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: For-loops as linear operation where N is
    // size of unordered_map towns_
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: While-loops as linear operation where N is
    // size of unordered_map towns_
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist which is O(N)
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: Uses std::sort which is approximately N*log(N)
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: Uses std::sort which is approximately N*log(N)
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Gets all_towns (O(N)) and min_element (O(N) separately)
    TownID min_distance();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Gets all_towns (O(N)) and max_element (O(N) separately)
    TownID max_distance();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist and find separately.
    // Both are O(N)
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist which is O(N)
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist, which is linear
    // and while loop, which is linear in worst case but averages less.
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses town_exist, which is linear
    // and for loops which are linear
    bool remove_town(TownID id);

    // Estimate of performance: O(N*log(N))
    // Short rationale for estimate: Uses std::sort which is approximately N*log(N)
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Iterates trough all vassal towns indirectly under
    // id which is N times at worst.
    int total_net_tax(TownID id);

private:

    struct Town {
        TownID id_ = NO_TOWNID;
        Name name_ = NO_NAME;
        Coord location_ = NO_COORD;
        int tax_ = 0;
        TownID  masterid_ = NO_TOWNID;
        std::vector<TownID> town_vassals = {};
    };

    std::unordered_map<TownID, Town*> towns_ = {};

    // Estimate of performance: O(N)
    // Short rationale for estimate: Uses std::find
    bool town_exist_(TownID town);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Only constant operations
    int distance_(Coord location1, Coord location2);

    // Estimate of performance: O(N)
    // Short rationale for estimate: recursively iterates trough
    // subtree and performs constant operations on all of the nodes
    int recursive_tax_(TownID id);
};

#endif // DATASTRUCTURES_HH
