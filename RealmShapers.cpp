#include "RealmShapers.h"
#include <cmath>
#include <queue>
#include <fstream>
#include <iostream>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (RealmShaper *shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
    return static_cast<int>(realmShapers.size());
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    return index >= 0 && index < realmShapers.size();
}

void ShaperTree::insert(RealmShaper *shaper)
{
    if (realmShapers.empty()) {
        realmShapers.push_back(shaper);
    }
    else {
        realmShapers.push_back(shaper);
    }
}

int ShaperTree::remove(RealmShaper *shaper)
{
    int removingIndex = findIndex(shaper);
    if (removingIndex == -1) {
        return -1;
    }

    for (int i = removingIndex; i < realmShapers.size() - 1; ++i) {
        realmShapers[i] = realmShapers[i + 1];
    }

    realmShapers.pop_back();

    return removingIndex;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == *shaper) {
            return i;
        }
    }

    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int index = findIndex(shaper);

    if (index == -1) {
        return -1;
    }

    return static_cast<int>(std::log2(index + 1));
}


int ShaperTree::getDepth()
{
    if (realmShapers.empty()) {
        return 0;
    }

    int nodeSize = static_cast<int>(realmShapers.size());

    return static_cast<int>(std::log2(nodeSize));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    int challengerIndex = findIndex(challenger);
    if (challengerIndex == -1) {
        return *challenger;
    }

    int opponentIndex = (challengerIndex - 1) / 2;
    if (!isValidIndex(opponentIndex)) {
        return *challenger;
    }

    RealmShaper *opponent = realmShapers[opponentIndex];
    if (result==1) {
        challenger->gainHonour();
        opponent->loseHonour();
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: " << challenger->getName() <<"-"<< challenger->getHonour() <<" "<< opponent->getName()<< "-"<< opponent->getHonour()<< std::endl;

        replace(challenger,opponent);


        return *challenger;

    } else {
        opponent->gainHonour();
        challenger->loseHonour();
        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: " << challenger->getName() <<"-"<< challenger->getHonour()<<" "<<opponent->getName()<<"-"<<opponent->getHonour() << std::endl;

        if (challenger->getHonour() < 0) {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;

            for (int i = challengerIndex; i < realmShapers.size() - 1; ++i) {
                realmShapers[i] = realmShapers[i + 1];
            }

            // Son elemanı çıkar
            realmShapers.pop_back();
        }

        return *opponent;
    }

}



RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index <= 0 || index >= realmShapers.size()) {
        return nullptr;
    }

    int parent = (index - 1) / 2;
    return realmShapers[parent];
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    int lowPlayer = findIndex(player_low);
    int highPlayer = findIndex(player_high);

    if (lowPlayer != -1 && highPlayer != -1) {

        realmShapers[lowPlayer] = player_high;
        realmShapers[highPlayer] = player_low;
    }
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *found = nullptr;

    for (RealmShaper *currentShaper : realmShapers)
    {
        if (*currentShaper == shaper)
        {
            found = currentShaper;
            break;
        }
    }

    return found;
}

RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *found = nullptr;

    for (RealmShaper *currentShaper : realmShapers)
    {
        if (currentShaper->getName() == name)
        {
            found = currentShaper;
            break;
        }
    }

    return found;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> inOrderVector;

    if (!isValidIndex(index)) {
        return inOrderVector;
    }

    int leftChild = 2 * index + 1;
    std::vector<std::string> left = inOrderTraversal(leftChild);
    inOrderVector.insert(inOrderVector.end(), left.begin(), left.end());

    inOrderVector.push_back(realmShapers[index]->getName());

    int rightChild = 2 * index + 2;
    std::vector<std::string> right = inOrderTraversal(rightChild);
    inOrderVector.insert(inOrderVector.end(), right.begin(), right.end());

    return inOrderVector;
}



std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> preOrderVector;

    if (!isValidIndex(index)) {
        return preOrderVector;
    }

    preOrderVector.push_back(realmShapers[index]->getName());

    int leftChild = 2 * index + 1;
    std::vector<std::string> left = preOrderTraversal(leftChild);
    preOrderVector.insert(preOrderVector.end(), left.begin(), left.end());

    int rightChild = 2 * index + 2;
    std::vector<std::string> right = preOrderTraversal(rightChild);
    preOrderVector.insert(preOrderVector.end(), right.begin(), right.end());

    return preOrderVector;
}


std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> postOrderVector;

    if (!isValidIndex(index)) {
        return postOrderVector;
    }

    int leftChild = 2 * index + 1;
    std::vector<std::string> left = postOrderTraversal(leftChild);
    postOrderVector.insert(postOrderVector.end(), left.begin(), left.end());

    int rightChild = 2 * index + 2;
    std::vector<std::string> right = postOrderTraversal(rightChild);
    postOrderVector.insert(postOrderVector.end(), right.begin(), right.end());

    postOrderVector.push_back(realmShapers[index]->getName());

    return postOrderVector;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index)) {
        return;
    }

    outFile << realmShapers[index]->getName() << std::endl;

    int leftChild = 2 * index + 1;
    preOrderTraversal(leftChild, outFile);

    int rightChild = 2 * index + 2;
    preOrderTraversal(rightChild, outFile);
}


void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    if (realmShapers.empty()) {
        return;
    }

    std::queue<int> nodes;
    nodes.push(0);

    while (!nodes.empty()) {
        int current = nodes.front();
        nodes.pop();

        outFile << realmShapers[current]->getName() << std::endl;

        int leftChild = 2 * current + 1;
        if (isValidIndex(leftChild)) {
            nodes.push(leftChild);
        }

        int rightChild = 2 * current + 2;
        if (isValidIndex(rightChild)) {
            nodes.push(rightChild);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}


void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}


void ShaperTree::writeShapersToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    for (auto shaper : realmShapers) {
        outFile << shaper->getName() << std::endl;
    }
    //std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    outFile.close();

}

void ShaperTree::writeToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    preOrderTraversal(0, outFile);
    //std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    outFile.close();

}
