#include <iostream>
#include <memory>
#include <vector>

class World;

class Player : public std::enable_shared_from_this<Player>
{
public:
    Player(std::string name) : name(name) 
    {
        std::cout << "Player " << name << " created.\n";
    }

    ~Player() 
    {
        std::cout << "Player " << name << " destroyed.\n";
    }

    void joinWorld(std::shared_ptr<World> world);

    std::string name;
};

class World {
public:
    void addPlayer(std::shared_ptr<Player> player) 
    {
        players.push_back(player);
    }

    void printPlayers() 
    {
        for (const auto& player : players) 
        {
            if (auto p = player.lock())
            {
                std::cout << "Player in world: " << p->name << "\n";
            }
            else 
            {
                std::cout << "A player has been removed.\n";
            }
        }
    }

private:
    std::vector<std::weak_ptr<Player>> players;
};

void Player::joinWorld(std::shared_ptr<World> world) 
{
    world->addPlayer(shared_from_this());
}

int main() {
    std::shared_ptr<World> world = std::make_shared<World>();

    {
        std::shared_ptr<Player> player1 = std::make_shared<Player>("Alice");
        player1->joinWorld(world);

        std::shared_ptr<Player> player2 = std::make_shared<Player>("Bob");
        player2->joinWorld(world);

        world->printPlayers();
    }

    world->printPlayers();

    return 0;
}