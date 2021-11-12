// Datastructures.cc
//
// Student name: Eero Tarri
// Student email: eero.tarri@tuni.fi
// Student number: H283568

#include "datastructures.hh"

#include <random>

#include <cmath>

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
//    std::unordered_map<TownID, Town*>::iterator it = towns_.begin();
//    while ( it != towns_.end() ) {
//        delete it->second;

//        it++;
//    }
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
    std::vector<TownID> ret = {};

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
        ++it;
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
    std::vector<TownID> towns = all_towns();
    if (towns.empty()) {return {};}
    std::sort( towns.begin(), towns.end(),
               [this](TownID i, TownID j){ return towns_.at(i)->name_ < towns_.at(j)->name_; });

    return towns;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> towns = all_towns(); // O(N)
    if (towns.empty()) {return {};}
    std::sort( towns.begin(), towns.end(),
               [this](TownID i, TownID j){return distance_(towns_.at(i)->location_, {0,0}) <
                distance_(towns_.at(j)->location_, {0,0});});

    return towns;
}

TownID Datastructures::min_distance()
{

    std::vector<TownID> towns = all_towns();

    if (towns.empty())
    {return {NO_TOWNID};}

    auto it = std::min_element(towns.begin(), towns.end(),
                               [this](TownID i, TownID j)
                               { return distance_(towns_.at(i)->location_, {0,0})
                               < distance_(towns_.at(j)->location_, {0,0}); } );
    return *it;
}

TownID Datastructures::max_distance()
{
    std::vector<TownID> towns = all_towns();

    if (towns.empty())
    {return {NO_TOWNID};}

    auto it = std::max_element(towns.begin(), towns.end(),
                               [this](TownID i, TownID j)
                               { return distance_(towns_.at(i)->location_, {0,0})
                               < distance_(towns_.at(j)->location_, {0,0}); } );
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
        return v;
    }
    return {NO_TOWNID};
}

bool Datastructures::remove_town(TownID id)
{
    if (!town_exist_(id)) {return false;}

    auto vassals = towns_.at(id)->town_vassals;

    if (towns_.at(id)->masterid_ != NO_TOWNID ) {

        auto& master_vassals = towns_.at(towns_.at(id)->masterid_)->town_vassals;
        master_vassals.erase(std::remove(master_vassals.begin(), master_vassals.end(), id), master_vassals.end());

        for (auto& vassal : vassals) { // O(N)
            master_vassals.push_back(vassal);
            towns_.at(vassal)->masterid_ = towns_.at(id)->masterid_;
        }
    } else {
        for (auto& vassal : vassals) {// O(N)
            towns_.at(vassal)->masterid_ = NO_TOWNID;
        }
    }

    towns_.erase(id);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> towns = all_towns();
    if (towns.empty())
    {return {};}
    std::sort( towns.begin(), towns.end(),
               [this, coord](TownID i, TownID j){return distance_(towns_.at(i)->location_, coord) <
                distance_(towns_.at(j)->location_, coord);});

    return towns;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id) // O(N)
{
    if (!town_exist_(id)) {return {NO_TOWNID};} // O(N)

    std::vector<TownID> path = {id};

    std::vector<TownID> longest_path = {};

    for (auto &vassal : towns_.at(id)->town_vassals) { // O(N*log(N))
        std::vector<TownID> vassal_path = longest_vassal_path(vassal);
        if (vassal_path.size() > longest_path.size()) {longest_path = vassal_path;}
    }

    for (auto& item : longest_path) { // O(N)
        path.push_back(item);
    }


    return path;
}

int Datastructures::total_net_tax(TownID id)
{
    int sum = recursive_tax_(id);
    if (towns_.at(id)->masterid_ != NO_TOWNID) {
        sum *= 0.9;
    }
    return sum;
}

bool Datastructures::town_exist_(TownID town) //N
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

int Datastructures::recursive_tax_(TownID id)
{
    if (towns_.at(id)->town_vassals.empty()) { return towns_.at(id)->tax_; }

    int sum = towns_.at(id)->tax_;
    for (TownID& vassal : towns_.at(id)->town_vassals) {
        sum += std::floor(0.1 * recursive_tax_(vassal));
    }

    return sum;
}

