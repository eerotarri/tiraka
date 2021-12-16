// Datastructures.cc
//
// Student name: Eero Tarri
// Student email: eero.tarri@tuni.fi
// Student number: H283568

#include "datastructures.hh"

#include <random>

#include <cmath>

const int INF = std::numeric_limits<int>::max(); // Maximum value for edge lenghts

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
    auto u_map_iter = towns_.begin();

    while (u_map_iter != towns_.end()) {
        delete u_map_iter->second;
        u_map_iter++;
    }
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
    std::vector<TownID> towns = all_towns();
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

        for (auto& vassal : vassals) {
            master_vassals.push_back(vassal);
            towns_.at(vassal)->masterid_ = towns_.at(id)->masterid_;
        }
    } else {
        for (auto& vassal : vassals) {
            towns_.at(vassal)->masterid_ = NO_TOWNID;
        }
    }

    delete towns_.at(id);
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

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if (!town_exist_(id)) {return {NO_TOWNID};}

    std::vector<TownID> path = {id};

    std::vector<TownID> longest_path = {};

    for (auto &vassal : towns_.at(id)->town_vassals) {
        std::vector<TownID> vassal_path = longest_vassal_path(vassal);
        if (vassal_path.size() > longest_path.size()) {longest_path = vassal_path;}
    }

    for (auto& item : longest_path) {
        path.push_back(item);
    }


    return path;
}

int Datastructures::total_net_tax(TownID id)
{
    if (!town_exist_(id)) {return NO_VALUE;}

    float sum = recursive_tax_(id);
    if (towns_.at(id)->masterid_ != NO_TOWNID) {
        sum = round(sum*0.9);
    }
    return int(sum);
}

int Datastructures::recursive_tax_(TownID id)
{
    if (towns_.at(id)->town_vassals.empty()) { return towns_.at(id)->tax_; }

    int sum = towns_.at(id)->tax_;
    for (TownID& vassal : towns_.at(id)->town_vassals) {
        sum += 0.1 * recursive_tax_(vassal);
    }

    return sum;
}

bool Datastructures::town_exist_(TownID id)
{
    if (towns_.find(id) == towns_.end()) {return false;}
    return true;
}

int Datastructures::distance_(Coord location1, Coord location2)
{
    int x1 = location1.x;
    int y1 = location1.y;
    int x2 = location2.x;
    int y2 = location2.y;
    return floor( sqrt( pow((x1-x2), 2) + pow((y1-y2), 2) ) );
}

std::vector<TownID> Datastructures::get_path_(TownID id, TownID lasttown)
{
    std::vector<TownID> ret;
    if (lasttown == NO_TOWNID) {
        ret = {id};
    } else {
        ret = {lasttown, id};
    }


    Town* town = towns_.at(id);

    while (town->path_back != nullptr) {

        town = town->path_back;
        ret.push_back(town->id_);
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

void Datastructures::relax(TownID u, TownID v)
{
    int cost = distance_(towns_.at(u)->location_, towns_.at(v)->location_);
    if (towns_.at(v)->d > towns_.at(u)->d + cost
            || towns_.at(v)->d == -1) {
        towns_.at(v)->d = towns_.at(u)->d + cost;
        towns_.at(v)->path_back = towns_.at(u);
    }
}


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    auto it = towns_.begin();
    while (it != towns_.end()) {
        it->second->neighbours_.clear();
        it++;
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::set<std::pair<TownID, TownID>> edges = {};

    auto it = towns_.begin();

    while (it != towns_.end()) {
        for (auto i : it->second->neighbours_) {

            std::pair<TownID, TownID> pair;
            if (it->first < i) {
                pair = {it->first, i};
            } else {
                pair = {i, it->first};
            }
            edges.insert(pair);
        }
        it++;
    }

    std::vector<std::pair<TownID, TownID>> ret(edges.begin(), edges.end());

    return ret;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if (!town_exist_(town1) || !town_exist_(town2)) {
        return false;
    }

    if (find(towns_.at(town1)->neighbours_.begin(), towns_.at(town1)->neighbours_.end(), town2)
            != towns_.at(town1)->neighbours_.end()) {
        return false;
    }

    towns_.at(town1)->neighbours_.insert(town2);
    towns_.at(town2)->neighbours_.insert(town1);

    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    if (!town_exist_(id)) {
        return {NO_TOWNID};
    }

    std::vector<TownID> roads(towns_.at(id)->neighbours_.begin(), towns_.at(id)->neighbours_.end());

    return roads;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    if (!town_exist_(fromid) || !town_exist_(toid)) {
        return {NO_TOWNID};
    }

    auto it = towns_.begin();
    while (it != towns_.end()) {
        it->second->colour = WHITE;
        it->second->path_back = nullptr;
        it++;
    }

    std::vector<TownID> S = {};

    S.push_back(fromid);

    while (!S.empty()) {
        TownID u = S.back();
        S.pop_back();

        if (towns_.at(u)->colour == WHITE) {
            towns_.at(u)->colour = GRAY;
            S.push_back(u);
            for (auto&& v : towns_.at(u)->neighbours_) {
                if (towns_.at(v)->colour == WHITE) {
                    S.push_back(v);
                    towns_.at(v)->path_back = towns_.at(u);
                    if (v == toid) {
                        return get_path_(v);
                    }
                }
            }
        } else {
            towns_.at(u)->colour = BLACK;
        }
    }
    return {};
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    if (!town_exist_(town1) || !town_exist_(town2)) {
        return false;
    }
    auto it1 = find(towns_.at(town1)->neighbours_.begin(), towns_.at(town1)->neighbours_.end(), town2);
    auto it2 = find(towns_.at(town2)->neighbours_.begin(), towns_.at(town2)->neighbours_.end(), town1);

    if (it1 == towns_.at(town1)->neighbours_.end() || it2 == towns_.at(town2)->neighbours_.end()) {
        return false;
    }
    towns_.at(town1)->neighbours_.erase(it1);
    towns_.at(town2)->neighbours_.erase(it2);
    return true;

}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    if (!town_exist_(fromid) || !town_exist_(toid)) {
        return {NO_TOWNID};
    }

    auto it = towns_.begin();
    while (it != towns_.end()) {
        it->second->colour = WHITE;
        it->second->path_back = nullptr;
        it->second->d = -1;
        it++;
    }

    std::deque<TownID> Q = {};

    towns_.at(fromid)->colour = GRAY;
    towns_.at(fromid)->d = 0;

    Q.push_back(fromid);

    while (!Q.empty()) {
        TownID u = Q.front();
        Q.pop_front();

        for (auto&& v : towns_.at(u)->neighbours_) {
            if (towns_.at(v)->colour == WHITE) {
                towns_.at(v)->colour = GRAY;
                towns_.at(v)->d = towns_.at(u)->d + 1;
                towns_.at(v)->path_back = towns_.at(u);
                Q.push_back(v);
                if (v == toid) {
                    return get_path_(v);
                }
            }
        }
        towns_.at(u)->colour = BLACK;
    }

    return {NO_TOWNID};
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    if (!town_exist_(startid)) {
        return {NO_TOWNID};
    }

    auto it = towns_.begin();
    while (it != towns_.end()) {
        it->second->colour = WHITE;
        it->second->path_back = nullptr;
        it++;
    }

    std::vector<TownID> S = {};

    S.push_back(startid);

    while (!S.empty()) {
        TownID u = S.back();
        S.pop_back();

        if (towns_.at(u)->colour == WHITE) {
            towns_.at(u)->colour = GRAY;
            S.push_back(u);
            for (auto&& v : towns_.at(u)->neighbours_) {
                if (towns_.at(v)->colour == WHITE) {
                    S.push_back(v);
                    towns_.at(v)->path_back = towns_.at(u);
                } else if (towns_.at(v)->colour == GRAY && towns_.at(v) != towns_.at(u)->path_back) {
                    return get_path_(u, v);
                }
            }
        } else {
            towns_.at(u)->colour = BLACK;
        }
    }
    return {};
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    if (!town_exist_(fromid) || !town_exist_(toid)) {
        return {NO_TOWNID};
    }

    auto it = towns_.begin();
    while (it != towns_.end()) {
        it->second->colour = WHITE;
        it->second->path_back = nullptr;
        it->second->d = INF;
        it++;
    }

    const Coord dest = towns_.at(toid)->location_;

    auto cmp = [this, toid, dest](TownID left, TownID right) {
        auto dist_left = distance_(towns_.at(left)->location_, dest) + towns_.at(left)->d;
        auto dist_right = distance_(towns_.at(right)->location_, dest) + towns_.at(right)->d;
        return dist_left > dist_right;
    };
    std::priority_queue<TownID, std::vector<TownID>, decltype (cmp)> Q(cmp);

    towns_.at(fromid)->colour = GRAY;
    towns_.at(fromid)->d = 0;

    Q.push(fromid);

    while (!Q.empty()) {
        TownID u = Q.top();
        Q.pop();
        for (auto&& v : towns_.at(u)->neighbours_) {
            relax(u, v);
            if (towns_.at(v)->colour == WHITE) {
                towns_.at(v)->colour = GRAY;
                Q.push(v);
            } else {

            }
        }
        towns_.at(u)->colour = BLACK;
    }

    if (towns_.at(toid)->d != INF) {
        return get_path_(toid);
    }

    return {};
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
