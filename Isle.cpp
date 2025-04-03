#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}

const std::string& Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = false;

    if (shaperCount < capacity) {
        shaperCount++;
    } else {
        isFull = true;
    }

    return isFull;
}


bool Isle::decreaseShaperCount()
{
    bool isEmpty = false;

    if (shaperCount > 0) {
        shaperCount--;
    }

    if (shaperCount <= 0) {
        isEmpty = true;
    }

    return isEmpty;
}


bool Isle::operator==(const Isle &other) const
{
    return this->name == other.name;
}

bool Isle::operator<(const Isle &other) const
{
    return this->name < other.name;
}

bool Isle::operator>(const Isle &other) const
{
    return this->name > other.name;
}

std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;

    std::ifstream file(filename);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        Isle* isle = new Isle(line);
        isles.push_back(isle);
    }

    file.close();

    return isles;
}


std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os <<  p.name );
    if (p.item == GOLDIUM)
        return (os  << p.name );
    if (p.item == AMAZONITE)
        return (os  << p.name );
    return (os << p.name);
}