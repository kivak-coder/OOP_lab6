#pragma once
#include "npc.hpp"

struct Pegasus : public NPC
{
    Pegasus(int x, int y);
    Pegasus(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;

    bool fight(std::shared_ptr<Dragon> other) override;
    bool fight(std::shared_ptr<Knight> other) override;
    bool fight(std::shared_ptr<Pegasus> other) override;

    bool accept(std::shared_ptr<NPC> other) override;

    friend std::ostream &operator<<(std::ostream &os, Pegasus &knight);
};