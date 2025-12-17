#include "../include/pegasus.hpp"
#include "../include/dragon.hpp"
#include "../include/knight.hpp"
#include <memory>

Pegasus::Pegasus(int x, int y) : NPC(PegasusType, x, y) {}
Pegasus::Pegasus(std::istream &is) : NPC(PegasusType, is) {}

void Pegasus::print()
{
    std::cout << *this;
}

void Pegasus::save(std::ostream &os)
{
    os << PegasusType << std::endl;
    NPC::save(os);
}


bool Pegasus::fight(std::shared_ptr<Dragon> other)
{
    fight_notify(other, false);
    return false;
}

bool Pegasus::fight(std::shared_ptr<Knight> other)
{
    fight_notify(other, false);
    return false;
}

bool Pegasus::fight(std::shared_ptr<Pegasus> other)
{
    fight_notify(other, false);
    return false;
}

bool Pegasus::accept(std::shared_ptr<NPC> other) {
    return other->fight(std::static_pointer_cast<Pegasus>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Pegasus &knight)
{
    os << "pegasus: " << *static_cast<NPC *>(&knight) << std::endl;
    return os;
}