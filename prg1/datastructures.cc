// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <QDebug>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
    clear_all();
}

unsigned int Datastructures::town_count()
{
    return towns_.size();
}

void Datastructures::clear_all()
{
    towns_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    bool already_exists = town_exist_(id);

    if (!already_exists) {
        Town* new_town = new Town{id, name, coord, tax};
        towns_.insert(std::pair<TownID, Town*>( id, new_town ));
    }
    return !already_exists;
}

Name Datastructures::get_town_name(TownID id)
{
    if (town_exist_(id))
    {return towns_.at(id)->name_;}
    return NO_NAME;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if (town_exist_(id))
    {return towns_.at(id)->location_;}
    return NO_COORD;
}

int Datastructures::get_town_tax(TownID id)
{
    if (town_exist_(id))
    {return towns_.at(id)->tax_;}
    return NO_VALUE;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> ret;

    for(auto it = towns_.begin(); it != towns_.end(); ++it) {
        ret.push_back(it->first);
    }

    return ret;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> ret = {};
    std::unordered_map<TownID, Town*>::iterator it = towns_.begin();

    while (it != towns_.end()) {
        if (it->second->name_ == name) {
            ret.push_back(it->first);
        }
    }

    return ret;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if (town_exist_(id)) {
        towns_.at(id)->name_ = newname;
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> towns = all_towns(); // O(N)
    std::sort( towns.begin(), towns.end() ); // O(N*log(N))

    return towns;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> towns = all_towns(); // O(N)
    std::sort( towns.begin(), towns.end(),
               [this](TownID i, TownID j){return distance_(towns_.at(i)->location_, {0,0}) <
                distance_(towns_.at(j)->location_, {0,0});});

    return towns;
}

TownID Datastructures::min_distance() // O(N)
{

    std::vector<TownID> towns = all_towns(); // O(N)

    auto it = std::min_element(towns.begin(), towns.end(),
                               [this](TownID i, TownID j)
                               { return distance_(towns_.at(i)->location_, {0,0})
                               < distance_(towns_.at(j)->location_, {0,0}); } );  // O(N)
    return *it;
}

TownID Datastructures::max_distance()
{
    std::vector<TownID> towns = all_towns(); // O(N)

    auto it = std::max_element(towns.begin(), towns.end(),
                               [this](TownID i, TownID j)
                               { return distance_(towns_.at(i)->location_, {0,0})
                               < distance_(towns_.at(j)->location_, {0,0}); } );  // O(N)
    return *it;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if (town_exist_(vassalid) && town_exist_(masterid)) {
        auto v = towns_.at(masterid)->town_vassals;
        if (std::find(v.begin(), v.end(), vassalid) == v.end()) {
            towns_.at(masterid)->town_vassals.push_back(vassalid);
            towns_.at(vassalid)->masterid_ = masterid;
            return true;
        }
    }
    return false;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if (town_exist_(id)) {
        return towns_.at(id)->town_vassals;
    }
    return {NO_TOWNID};
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    if (town_exist_(id)) {
        TownID vassal = id;
        std::vector<TownID> v = {vassal};
        while (towns_.at(vassal)->masterid_ != NO_TOWNID) {
            v.push_back(towns_.at(vassal)->masterid_);
            vassal = towns_.at(vassal)->masterid_;
        }
    }
    return {NO_TOWNID};
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}

bool Datastructures::town_exist_(TownID town) //log(n)
{
    if (towns_.find(town) == towns_.end())
    {return false;}
    return true;
}

int Datastructures::distance_(Coord location1, Coord location2) // O(1)
{
    int x1 = location1.x;
    int y1 = location1.y;
    int x2 = location2.x;
    int y2 = location2.y;
    return floor( sqrt( pow((x1-x2), 2) + pow((y1-y2), 2) ) );
}
