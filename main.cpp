#include "include/npc.hpp"
#include "include/dragon.hpp"
#include "include/knight.hpp"
#include "include/pegasus.hpp"
#include <fstream>
#include <ostream>
#include <string>

// Text Observer
class TextObserver : public IFightObserver
{
private:
    TextObserver(){};

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::cout << std::endl
                      << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
        }
    }
};

class FileObserver : public IFightObserver
{
private:

    FileObserver(){};
    std::ofstream log_file;

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {

        if (!log_file.is_open()) {
            log_file.open("log.txt", std::ios::app);
        }

        if (win)
        {
            log_file << std::endl << "Murder --------" << std::endl;            
            log_file << "Attacker: " << attacker->name 
                     << " at (" << attacker->x << ", " << attacker->y << ")" << std::endl;
            log_file << "Defender: " << defender->name 
                     << " at (" << defender->x << ", " << defender->y << ")" << std::endl;
            
        }
        log_file.flush();
    }
};




// Фабрики -----------------------------------
std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        switch ((int)type)
        {
        case DragonType:
            result = std::make_shared<Dragon>(is);
            break;
        case KnightType:
            result = std::make_shared<Knight>(is);
            break;
        case PegasusType:
            result = std::make_shared<Pegasus>(is);
            break;
        }
    }
    else {
        std::cerr << "unexpected NPC type:" << type << std::endl;
        return nullptr;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}


std::string generate_dragon_name(int index) {
    static const std::vector<std::string> prefixes = {"Fire", "Ice", "Storm", "Shadow", "Ancient", 
                                                      "Golden", "Crystal", "Obsidian", "Thunder", "Night"};

    return prefixes[index % prefixes.size()];
}

std::string generate_knight_name(int index) {
    static const std::vector<std::string> first_names = {"Arthur", "Lancelot", "Galahad", "Percival", 
                                                         "Gawain", "Tristan", "Bedivere", "Kay", 
                                                         "Gareth", "Bors", "Ector", "Mordred"};

    return first_names[index % first_names.size()];
}

std::string generate_pegasus_name(int index) {
    static const std::vector<std::string> names = {"Skydancer", "Cloudchaser", "Stormrider", "Moonbeam",
                                                   "Starlight", "Windwhisper", "Sunflare", "Rainbowmane",
                                                   "Thunderhoof", "Silverwing", "Dreamweaver", "Aurora"};
    return names[index % names.size()];
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y)
{
    std::cout << "Type:" << (int)type << std::endl;
    std::shared_ptr<NPC> result;
    std::string name;
    static int index = 0;
    switch (type)
    {
    case DragonType:
        name = generate_dragon_name(index);
        result = std::make_shared<Dragon>(x, y, name);
        break;
    case KnightType:
        name = generate_knight_name(index + 1);
        result = std::make_shared<Knight>(x, y, name);
        break;
    case PegasusType:
        name = generate_pegasus_name(index + 2);
        result = std::make_shared<Pegasus>(x, y, name);
        break;
    default:
        return nullptr;
        break;
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    ++index;

    return result;
}

// save array to file
void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename); 
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush(); 
    fs.close(); 
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename); 
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int i = 0; i < count; ++i) {
            auto npc = factory(is);
            if (!npc) {
                std::cerr << "Failed to load npc" << std::endl;
            }
            result.insert(npc);
        }
        is.close();
   
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

// print to screen
std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array) {
        if (n) {
            n->print();
        }
    }
    return os;
}


// ВНИМАНИЕ: метод осуществляющий сражение написан неправильно!
// Переделайте его на использование паттерна Visitor
// То есть внутри цикла вместо кучи условий должно быть:
//
// success = defender->accept(attacker);
//
// В NPC методы типа is_dragon - станут не нужны    

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && (attacker->is_close(defender, distance)))
            {
                bool success{false};
                success = defender->accept(attacker);
                if (success)
                    dead_list.insert(defender);
            }

    return dead_list;
}

int main()
{
    set_t array; // монстры

    // Гененрируем начальное распределение монстров
    std::cout << "Generating ..." << std::endl;
    for (size_t i = 0; i < 20; ++i)
        array.insert(factory(NpcType(std::rand() % 3 + 1),
                             std::rand() % 500,
                             std::rand() % 500));
    std::cout << "Saving ..." << std::endl;

    save(array, "npc.txt");

    std::cout << "Loading ..." << std::endl;
    array = load("npc.txt");

    std::cout << "Fighting ..." << std::endl
              << array;

    for (size_t distance = 20; (distance <= 500) && !array.empty(); distance += 10)
    {
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list)
            array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "distance: " << distance << std::endl
                  << "killed: " << dead_list.size() << std::endl 
                  << std::endl;

    }

    std::cout << "Survivors:\n" << array;

    return 0;
}