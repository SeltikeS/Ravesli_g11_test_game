#include <iostream>
#include <string>
#include <cstdlib> // для rand() и srand()
#include <ctime> // для time()
 
// Генерируем рандомное число между min и max
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
 
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
 
	char getSymbol() { return m_symbol; }
	const std::string& getName() { return m_name; }
	bool isDead() { return m_health <= 0; }
	int getGold() { return m_gold; }
	void addGold(int gold) { m_gold += gold;  }
	void reduceHealth(int health) { m_health -= health;  }
	int getHealth() { return m_health; }
	int getDamage() { return m_damage;  }
};
 
class Player : public Creature
{
	int m_level = 1;
 
public:
	Player(std::string name)
		: Creature(name, '@', 10, 1, 0)
	{
	}
 
	void levelUp()
	{
		++m_level;
		++m_damage;
	}
 
	int getLevel() { return m_level; }
	bool hasWon() { return m_level >= 20; }
};
 
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
 
	struct MonsterData
	{
		const char* name;
		char symbol;
		int health;
		int damage;
		int gold;
	};
 
	static MonsterData monsterData[MAX_TYPES];
 
	Monster(Type type)
		: Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{
	}
 
	static Monster getRandomMonster()
	{
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}
};
 
Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};

class GameLogic
{
private:
    enum FightStatus
    {
        RUN,
        FIGHT,
        FIGHT_STATUS_NUMBER
    };
    
    enum GameStatus
    {
        WON,
        LOSE,
        NEXT_FIGHT,
        GAME_STATUS_NUMBER
    };
    
    FightStatus fight_status;
    GameStatus game_status;
    
public:
    GameLogic()
    {
        startGame();
    }
    
    void startGame()
    {
        std::cout << "Enter your name: ";
        std::string playerName;
        std::cin >> playerName;
        Player p(playerName);
        std::cout << "Welcome, " << p.getName() << '\n';
        
        game_status = NEXT_FIGHT;
        
        do
        {
            Monster m = Monster::getRandomMonster();
            fightMonster(m, p);
        }
        while(game_status == NEXT_FIGHT);
        
        if(game_status == WON)
        std::cout << p.getName() << ", you win. You have " <<
            p.getGold() << " gold.\n";
            
    }
    
    void fightMonster(Monster& m, Player& p)
    {
        std::cout << "You have encountered a " << 
            m.getName() << " (" << m.getSymbol() << ").\n";
        
        fight_status = request(m, p);
        
        while((fight_status == FIGHT)&&(game_status == NEXT_FIGHT))
        {
            attackMonster(m, p);
        }
            
    }
    
    FightStatus request(Monster& m, Player& p)
    {
        char ch;
        
        std::cout << "(R)un or (F)ight?: ";
        std::cin >> ch;
            
        switch(ch)
        {
            case 'r':
                fight_status = RUN;
                break;
            case 'f':
                fight_status = FIGHT;
                break;
            default:
                fight_status = request(m, p);
                break;
        }
            
        if(fight_status == RUN)
        {
            int num = getRandomNumber(RUN, FIGHT);
            fight_status = static_cast<FightStatus>(num);
            if(fight_status == FIGHT)
            {
                attackPlayer(m, p);
            }
        }
            
        return fight_status;
        
    }
    
    void attackMonster(Monster& m, Player& p)
    {
        std::cout << "You hit " << m.getName() << 
            " for " << p.getDamage() << " damage.\n";
        m.reduceHealth(p.getDamage());
        
        if(m.isDead())
        {
            p.levelUp();
            p.addGold(m.getGold());
            std::cout << "You are now at level " << 
                p.getLevel() << ".\nYou found " << 
                m.getGold() << " gold.\n";
            
            fight_status = RUN;
            
            if(p.hasWon())
            {
                game_status = WON;
            }
            
        }
        else 
                attackPlayer(m, p);
    }
    
    void attackPlayer(Monster& m, Player& p)
    {
        std::cout << "The " << m.getName() <<
            " hit you for " << m.getDamage() << ".\n";
        
        p.reduceHealth(m.getDamage());
        std::cout << "Your health " << p.getHealth() << ".\n";
        
        if(p.isDead())
        {
            std::cout << "You died at level " << p.getLevel() <<
                " and with " << p.getGold() << ".\n" <<
                "Too bad you can't take it with you!\n";
            game_status = LOSE;
        }
        else
            fight_status = request(m, p);
    }
    
};
 
int main()
{
	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand(); // сбрасываем первый результат
 
	GameLogic g;
 
	return 0;
}
