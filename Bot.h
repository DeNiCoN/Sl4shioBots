#include <memory>

class Bot;

class Behavior
{

};


using Bot_ptr = std::shared_ptr<Bot>;

class Bot
{
public:
	Bot(std::string name) : name(name) {}
	std::string getName() const { return name; }
	unsigned int getExp() const { return exp; }
	unsigned int getLvl() const { return lvl; }

private:
	std::string name;
	unsigned int exp;
	unsigned int lvl;
	Behavior behavior;
};