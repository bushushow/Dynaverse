#include "GameWorld.h"
#include <unordered_map>

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);

}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}


bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    if (realmShaper == nullptr || isle == nullptr) {
        return false;
    }

    int playerDepth = shaperTree.getDepth(realmShaper);
    if (playerDepth == -1) {
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return false;
    }

    int isleDepth = mapTree.getIsleDepth(isle);
    if (isleDepth == -1) {
        return false;
    }

    int totalTreeHeight = std::ceil(std::log2(shaperTree.getSize() + 1));

    int totalMapDepth = mapTree.getDepth();

    int minMapDepth = static_cast<int>(totalMapDepth * (static_cast<double>(playerDepth) / totalTreeHeight));

    if (isleDepth >= minMapDepth) {
        return true;
    }

    return false;
}




std::unordered_map<RealmShaper*, Isle*> shaperLocations;

void GameWorld::exploreArea(RealmShaper* realmShaper, Isle* isle) {
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName()
                  << " does not have access to explore area " << isle->getName() << std::endl;
        return;
    }

    if (shaperLocations.find(realmShaper) != shaperLocations.end()) {
        Isle* previousIsle = shaperLocations[realmShaper];
        if (previousIsle != nullptr && previousIsle != isle) {
            previousIsle->decreaseShaperCount();
        }
    }

    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;

    realmShaper->collectItem(isle->getItem());
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is "
              << realmShaper->getEnergyLevel() << std::endl;

    if (isle->increaseShaperCount()) {
        std::cout << "[Overcrowding] " << isle->getName()
                  << " self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);
    }

    shaperLocations[realmShaper] = isle;
}


void GameWorld::craft(RealmShaper *shaper, const std::string &isleName) {
    if (!shaper->hasEnoughEnergy()) {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
        return;
    }

    Isle *newIsle = new Isle(isleName);

    std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
    shaper->loseEnergy();

    mapTree.insert(newIsle);

    std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
}


void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}


void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    std::ifstream accessFile(accessLogs);
    if (!accessFile.is_open()) {
        return;
    }

    std::ifstream duelFile(duelLogs);
    if (!duelFile.is_open()) {
        return;
    }

    std::string line;
    if (std::getline(accessFile, line)) {
    }

    if (std::getline(duelFile, line)) {
    }
    int access = 0;


    while (std::getline(accessFile, line)) {
        std::istringstream iss(line);
        std::string playerName, isleName;
        iss >> playerName >> isleName;

        RealmShaper* player = shaperTree.findPlayer(playerName);
        Isle* isle = mapTree.findIsle(isleName);

        if (player == nullptr) {
            continue;
        }

        if (isle == nullptr) {
            craft(player, isleName);
        } else {
            exploreArea(player, isle);
        }

        access++;

        if (access % 5 == 0) {
            std::string duelLine;
            if (std::getline(duelFile, duelLine)) {
                std::istringstream duelStream(duelLine);
                std::string challengerName, result;
                duelStream >> challengerName >> result;
                int intResult = std::stoi(result);
                RealmShaper* challenger = shaperTree.findPlayer(challengerName);
                if (challenger == nullptr) {
                    continue;
                }
                shaperTree.duel(challenger, intResult);
            }
        }

        displayGameState();
    }

    while (std::getline(duelFile, line)) {
        std::istringstream duelStream(line);
        std::string challengerName, result;
        duelStream >> challengerName >> result;

        int intResult = std::stoi(result);
        RealmShaper* challenger = shaperTree.findPlayer(challengerName);
        if (challenger == nullptr) {
            continue;
        }

        shaperTree.duel(challenger, intResult);

        displayGameState();
    }


    accessFile.close();
    duelFile.close();
}


void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);

}