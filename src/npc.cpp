#include "../include/npc.hpp"
#include <ostream>
#include <string>
#include <limits>

NPC::NPC(NpcType t, int _x, int _y, const std::string& _name) 
    : type(t), x(_x), y(_y), name(_name) 
{
    if (name.empty()) {
        name = "Unnamed";
    }
}

NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> x;
    is >> y;
    is >> std::ws;
    std::getline(is, name); 

    if (name.empty()) {
        name = "Unnamed";
    }
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer)
{
   observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win)
{
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    if (std::pow(x - other->x, 2) + std::pow(y - other->y, 2) <= std::pow(distance, 2))
        return true;
    else
        return false;
}


void NPC::save(std::ostream &os)
{
    os << x << std::endl;
    os << y << std::endl;
    os << name << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << npc.name << " {x:" << npc.x << ", y:" << npc.y << "}";
    return os;
}