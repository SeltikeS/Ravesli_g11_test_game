#include <iostream>
#include <string>
#include <cstdlib> // для rand() и srand()
#include <ctime> // для time()


// Generate random number between MIN and MAX 
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем генерацию значения из диапазона
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

// Parent class for all CREATUREs
class Creature
{
protected:
	std::string m_name;
	char m_symbol;
	int m_health;
	int m_damage;
	int m_gold;

public:
	Creature(std::string name, char symbol, int health, int damage, int gold) :
		m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
	{
	}

	std::string getName() { return m_name; }
	char getSymbol() { return m_symbol; }
	int getHealth() { return m_health; }
	int getDamage() { return m_damage; }
	int getGold() { return m_gold; }

	void reduceHealth(int health)
	{
		m_health -= health;
	}

	bool isDead()
	{
		return m_health <= 0;
	}

	void addGold(int gold)
	{
		m_gold += gold;
	}

};

// Create a new PLAYER
class Player : public Creature
{
private:
	int m_level;

public:
	Player(std::string name) : m_level(1), Creature(name, '@', 10, 1, 0)
	{
	}

	int getLevel() { return m_level; }

	void levelUp() { m_level += 1;  }

	bool hasWon() { return m_level == 20; }

};

// Create a new MONSTER
class Monster : public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};

	Monster(Type type) : 
		Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{
	} 

	struct MonsterData
	{
		std::string name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	static MonsterData monsterData[MAX_TYPES];

	static Monster getRandomMonster()
	{
		int number = getRandomNumber(0, MAX_TYPES-1);
		return Monster(static_cast<Type>(number));
	}
};

// Search table for all MONSTER TYPES
Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'O', 4, 2, 25 },
	{ "slime", 'S', 1, 1, 10 }
};


int main()
{
	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand(); // сбрасываем первый результат

	for (int i = 0; i < 10; ++i)
	{
		Monster m = Monster::getRandomMonster();
		std::cout << "A " << m.getName() << " (" << m.getSymbol() << ") was created.\n";
	}

//	Monster m1;
//	Monster m2;

//	std::cout << m1.monsterData[0].gold << " " << m2.monsterData[0].gold << '\n';
	

	return 0;
}