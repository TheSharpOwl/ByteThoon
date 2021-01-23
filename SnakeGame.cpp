#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <ctime>
#include <cstdlib>

const uint32_t Width = 100, Height = 20;
std::vector<std::pair<uint32_t, uint32_t>> bodyCoord;

void draw(uint32_t Width, uint32_t Height)
{
    for (unsigned int i = 0; i <= Height + 1; i++)
    {
        for (unsigned int j = 0; j <= Width + 1; j++)
        {
            if (i == 0 || i == Height + 1 || j == 0 || j == Width + 1)
                std::cout << "#";
            else if (i == bodyCoord[0].second && j == bodyCoord[0].first)
                std::cout << "*";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

void start()
{
    // using time as a seed for our random start point
    std::srand((unsigned int) std::time(nullptr));
    uint32_t startX = std::rand() % Width + 1;
    uint32_t startY = std::rand() % Height + 1;

    bodyCoord.push_back({startX, startY});
}
int main()
{

}