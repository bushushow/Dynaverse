#include "Map.h"
#include <stack>
int rebalanceCount;
bool initialized;



Map::Map()
{
    this->root = nullptr;
}

Map::~Map()
{
    if (root == nullptr) {
        return;
    }

    std::stack<MapNode*> stack;
    stack.push(root);

    while (!stack.empty()) {
        MapNode* current = stack.top();
        stack.pop();

        if (current->left) {
            stack.push(current->left);
        }
        if (current->right) {
            stack.push(current->right);
        }

            delete current;

    }

    root = nullptr;
}


void Map::initializeMap(std::vector<Isle*> isles) {
    initialized =false;
    for (Isle* isle : isles) {
        insert(isle);
    }
    populateWithItems();
    initialized = true;
    rebalanceCount=0;
}


MapNode *Map::rotateRight(MapNode *current)
{
    if (!current || !current->left) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}


MapNode *Map::rotateLeft(MapNode *current)
{
    if (!current || !current->right) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}


int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}


int Map::height(MapNode *node)
{
    return (node == nullptr) ? 0 : node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{

    if (node == nullptr) {

        return new MapNode(isle);
    }

    if (*isle < *(node->isle)) {
        node->left = insert(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balancingFactor = height(node->left) - height(node->right);

    if (balancingFactor > 1 && *isle < *(node->left->isle)) {
        rebalanceCount++;
        return rotateRight(node);
    }

    if (balancingFactor < -1 && *isle > *(node->right->isle)) {
        rebalanceCount++;
        return rotateLeft(node);
    }

    if (balancingFactor > 1 && *isle > *(node->left->isle)) {
        node->left = rotateLeft(node->left);
        rebalanceCount++;
        return rotateRight(node);
    }

    if (balancingFactor < -1 && *isle < *(node->right->isle)) {
        node->right = rotateRight(node->right);
        rebalanceCount++;
        return rotateLeft(node);
    }

    return node;
}

void Map::insert(Isle* isle) {
    root = insert(root, isle);

    if (initialized) {
        if (rebalanceCount % 3 == 0 && rebalanceCount>0) {
            int count = 0;
            postOrderItemDrop(root, count);
        }
        if (rebalanceCount % 3 == 0 && rebalanceCount>0) {
            int count = 0;
            preOrderItemDrop(root, count);
        }
        if (rebalanceCount % 3 == 0 && rebalanceCount>0) {
            dropItemBFS();
        }
    }
}


MapNode *Map::remove(MapNode *node, Isle *isle) {
    if (node == nullptr) {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return node;
    }

    if (*isle < *(node->isle)) {
        node->left = remove(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = remove(node->right, isle);
    } else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        } else if (node->left == nullptr) {
            MapNode *temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            MapNode *temp = node->left;
            delete node;
            return temp;
        } else {

            MapNode *inOrderPredecesor = node->left;
            while (inOrderPredecesor->right != nullptr) {
                inOrderPredecesor = inOrderPredecesor->right;
            }

            node->isle = inOrderPredecesor->isle;

            node->left = remove(node->left, inOrderPredecesor->isle);
        }
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balancingFactor = height(node->left) - height(node->right);

    if (balancingFactor > 1 && height(node->left->left) >= height(node->left->right)) {
        rebalanceCount++;
        return rotateRight(node);
    }

    if (balancingFactor < -1 && height(node->right->right) >= height(node->right->left)) {
        rebalanceCount++;
        return rotateLeft(node);
    }

    if (balancingFactor > 1 && height(node->left->left) < height(node->left->right)) {
        node->left = rotateLeft(node->left);
        rebalanceCount++;
        return rotateRight(node);
    }

    if (balancingFactor < -1 && height(node->right->right) < height(node->right->left)) {
        node->right = rotateRight(node->right);
        rebalanceCount++;
        return rotateLeft(node);
    }

    return node;
}



void Map::remove(Isle* isle) {
    root = remove(root, isle);

    if (initialized) {
        if (rebalanceCount % 3 == 0 && rebalanceCount>0) {
            int count = 0;
            postOrderItemDrop(root, count);
        }

        // Her 5 rebalancing'de EINSTEINIUM ekle
        if (rebalanceCount % 3 == 0&& rebalanceCount>0) {
            int count = 0;
            preOrderItemDrop(root, count);
        }

        // Her 3 rebalancing'de AMAZONITE ekle
        if (rebalanceCount % 3 == 0&& rebalanceCount>0) {
            dropItemBFS();
        }
    }
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if (current == nullptr) {
        return;
    }

    count++;

    if (count % 5 == 0) {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);

    preOrderItemDrop(current->right, count);
}


void Map::postOrderItemDrop(MapNode *current, int &count){


    if (current == nullptr) {
        return;
    }

    postOrderItemDrop(current->left, count);

    postOrderItemDrop(current->right, count);

    count++;

    if (count % 3 == 0) {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }


}


MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (node == nullptr) {
        return nullptr;
    }

    if (node->isle->getItem() == EMPTY) {
        return node;
    }

    MapNode *left = findFirstEmptyIsle(node->left);
    if (left != nullptr) {
        return left;
    }

    return findFirstEmptyIsle(node->right);
}


void Map::dropItemBFS() {
    if (root == nullptr) {
        return;
    }

    std::queue<MapNode*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        MapNode* current = nodes.front();

        nodes.pop();

        if (current->isle->getItem() == EMPTY) {
            current->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }

        if (current->left != nullptr) {
            nodes.push(current->left);
        }
        if (current->right != nullptr) {
            nodes.push(current->right);
        }
    }
    std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;


}


void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode* node)
{
    if (node == nullptr) {
        return -1;
    }

    return std::max(getDepth(node->left), getDepth(node->right)) + 1;
}

int Map::getDepth()
{
    return getDepth(root);
}

int getIsleRecursive(MapNode *node, Isle *isle, int currentDepth)
{
    if (node == nullptr) {
        return -1;
    }

    if (*(node->isle) == *isle) {
        return currentDepth;
    }

    if (*isle < *(node->isle)) {
        return getIsleRecursive(node->left, isle, currentDepth + 1);
    } else {
        return getIsleRecursive(node->right, isle, currentDepth + 1);
    }
}
int Map::getIsleDepth(Isle *isle)
{
    return getIsleRecursive(root, isle, 0);
}


void Map::populateWithItems()
{
    int count = 0;

    postOrderItemDrop(root, count);

    count = 0;
    preOrderItemDrop(root, count);

}


Isle *Map::findIsle(Isle isle)
{
    MapNode *current = root;

    while (current != nullptr) {
        if (*(current->isle) == isle) {
            return current->isle;
        }

        if (isle < *(current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}


Isle *Map::findIsle(std::string name)
{
    MapNode *current = root;

    while (current != nullptr) {
        if (current->isle->getName() == name) {
            return current->isle;
        }

        if (name < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}


MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;

    while (current != nullptr) {
        if (*(current->isle) == isle) {
            return current;
        }

        if (isle < *(current->isle)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}


MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;

    while (current != nullptr) {
        if (current->isle->getName() == name) {
            return current;
        }

        if (name < current->isle->getName()) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return nullptr;
}


void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    if (root == nullptr) {
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return;
    }

    std::queue<MapNode *> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        MapNode *current = nodes.front();
        nodes.pop();

        outFile << current->isle->getName() << std::endl;

        if (current->left != nullptr) {
            nodes.push(current->left);
        }
        if (current->right != nullptr) {
            nodes.push(current->right);
        }
    }

    outFile.close();
}



void Map::writeIslesToFile(const std::string &filename)
{
    if (root == nullptr) {
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return;
    }

    MapNode* current = root;
    std::vector<MapNode*> nodes;

    while (current != nullptr || !nodes.empty()) {
        while (current != nullptr) {
            nodes.push_back(current);
            current = current->left;
        }

        current = nodes.back();
        nodes.pop_back();
        outFile << current->isle->getName() << std::endl;

        current = current->right;
    }
    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;

    outFile.close();
}

