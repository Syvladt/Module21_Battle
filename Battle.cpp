#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

struct player
{
	std::string name;
	int health;
	int armor;
	int damage;
	int x;
	int y;
};

// Функция вывода состояния боя на экран
void showField(char kulikovo[][20])
{
	bool flagenemies = false;
	std::cout << "\x1B[2J\x1B[H";
	// Ось Y. 0 - левый нижний угол
	for (int i = 19; i >= 0; --i)
	{
		// Ось Х. Левый нижний угол
		for (int j = 0; j < 20; ++j)
		{
			std::cout << kulikovo[j][i] << ' ';
		}
		std::cout << '\n';
	}
}

// Функция инициализации данных об игроках. Почему я выделил HERO?
// Исторически сложилось.
void initField(player& hero, player enemies[], char kulikovo[][20])
{
	std::string adversary[] = {"Koschey", "Gorynych", "Chernomor", "Yaga", "Shapoklyak"};
	std::cout << "Enter name Hero: ";
	std::cin >> hero.name;
	std::cout << "Enter health value (50 - 150): ";
	std::cin >> hero.health;

	// Подготовка игрового поля
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			kulikovo[i][j] = '.';
		}
	}
	// Ввод данных о HERO
	while (hero.health < 50 || hero.health > 150)
	{
		std::cout << "Value of health incorrect. Repeat enter, please.\n";
		std::cin >> hero.health;
	}
	std::cout << "Enter armor value (0 - 50): ";
	std::cin >> hero.armor;
	while (hero.armor < 0 || hero.armor > 50)
	{
		std::cout << "Value of armor incorrect. Repeat enter, please.\n";
		std::cin >> hero.armor;
	}
	std::cout << "Enter damage value (15 - 30): ";
	std::cin >> hero.damage;
	while (hero.damage < 15 || hero.damage > 30)
	{
		std::cout << "Value of damage incorrect. Repeat enter, please.\n";
		std::cin >> hero.damage;
	}
	std::cout << "Enter coordinates values (0 - 20)\nX: ";
	std::cin >> hero.x;
	while (hero.x < 0 || hero.x > 20)
	{
		std::cout << "Value of X incorrect. Repeat enter, please.\n";
		std::cin >> hero.x;
	}
	std::cout << "Y: ";
	std::cin >> hero.y;
	while (hero.y < 0 || hero.y > 20)
	{
		std::cout << "Value of Y incorrect. Repeat enter, please.\n";
		std::cin >> hero.y;
	}
	kulikovo[hero.x][hero.y] = 'H';
	// Генерация данных о врагах
	for (int i = 0; i < 5; i++)
	{
		enemies[i].name = adversary[i];
		enemies[i].health = (std::rand() % 101) + 50;
		enemies[i].armor = std::rand() % 51;
		enemies[i].damage = (std::rand() % 16) + 15;
		enemies[i].x = std::rand() % 20;
		enemies[i].y = std::rand() % 20;
		kulikovo[enemies[i].x][enemies[i].y] = 'E';
	}
}
// Функция определения врага, ближайшего к HERO.
// Возвращается индекс врага, ближайшего к HERO.
int nearestEnemy(player& hero, player enemies[])
{
	int result = -1, distance, minDistance = 100;
	for (int i = 0; i < 5; ++i)
	{
		if (enemies[i].health > 0)
		{
			distance = abs(sqrt(pow((hero.x - enemies[i].x), 2) + pow((hero.y - enemies[i].y), 2)));
			if (minDistance > distance)
			{
				minDistance = distance;
				result = i;
			}
		}
	}
	return result;
}

std::string stroke(player& hero, player enemies[], int x, int y, char kulikovo[][20])
{
	int i;
	std::string message = "\n";
	for (i = 0; i < 5; ++i)
	{
		if (enemies[i].x == x && enemies[i].y == y)
		{
			break;
		}
	}
	enemies[i].armor -= hero.damage;
	if (enemies[i].armor < 0)
	{
		enemies[i].health += enemies[i].armor;
		enemies[i].armor = 0;
	}
	if (enemies[i].health <= 0)
	{
		kulikovo[enemies[i].x][enemies[i].y] = '.';
		message = enemies[i].name + " is dead.\n";
	}
	else
		message = enemies[i].name + " took damage " + std::to_string(hero.damage) + " units. Health is " + std::to_string(enemies[i].health) + " units\n";
	return message;
}

std::string moveEnemies(player& hero, player enemies[], char kulikovo[][20])
{	// Функция перемещение врогов. Алгоритм такой: координаты HERO и ENEMY стараются сблизится,
	// т.е. выбирается ближайший к HERO враг и начинет сближение с HERO по оси Х, потом по оси Y.
	int pointer = nearestEnemy(hero, enemies); // Выбираем номер врага для перемещения
	std::string message = "\n";
	if (pointer >= 0)
	{
		if ((enemies[pointer].x < hero.x) && (enemies[pointer].x + 1 < 20) && (kulikovo[enemies[pointer].x + 1][enemies[pointer].y] == '.'))
		{// Если координата Х ближайшего врага меньше координаты Х HERO и ячейка поля справа от врага свободна,
		 // То враг занимает эту свободную ячейку.
			kulikovo[enemies[pointer].x][enemies[pointer].y] = '.';
			enemies[pointer].x++;
			kulikovo[enemies[pointer].x][enemies[pointer].y] = 'E';
		}
		else if ((enemies[pointer].x > hero.x) && (enemies[pointer].x - 1 != 0) && (kulikovo[enemies[pointer].x - 1][enemies[pointer].y] == '.'))
		{
			kulikovo[enemies[pointer].x][enemies[pointer].y] = '.';
			enemies[pointer].x--;
			kulikovo[enemies[pointer].x][enemies[pointer].y] = 'E';
		}
		else if ((enemies[pointer].y < hero.y) && (enemies[pointer].y + 1 < 20) && (kulikovo[enemies[pointer].x][enemies[pointer].y + 1] == '.'))
		{
			kulikovo[enemies[pointer].x][enemies[pointer].y] = '.';
			enemies[pointer].y++;
			kulikovo[enemies[pointer].x][enemies[pointer].y] = 'E';
		}
		else if ((enemies[pointer].y > hero.y) && (enemies[pointer].y - 1 != 0) && (kulikovo[enemies[pointer].x][enemies[pointer].y - 1] == '.'))
		{
			kulikovo[enemies[pointer].x][enemies[pointer].y] = '.';
			enemies[pointer].y--;
			kulikovo[enemies[pointer].x][enemies[pointer].y] = 'E';
		}
	}
	else
		message = "!!!THE HERO WON!!!\n";
	return message;
}

std::string saveGame(player & hero, player enemies[])
{	// Функция сохранения игры. Сохраняем в текстовый файл. Имена персонажей текстовые, поэтому
	// остальные параметры тоже храним в текстовом виде. Хотя можно было бы заморочится и с бинарным.
	std::ofstream cfg;
	std::string message;
	cfg.open("Battle.cfg");
	if (cfg.is_open())
	{
		cfg << hero.name << " " << std::to_string(hero.health) << " " << std::to_string(hero.armor) << " " <<\
			std::to_string(hero.damage) << " " << std::to_string(hero.x) << " " << std::to_string(hero.y) << " " << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			cfg << enemies[i].name << " " << std::to_string(enemies[i].health) << " " << std::to_string(enemies[i].armor) << \
				" " << std::to_string(enemies[i].damage) << " " << std::to_string(enemies[i].x) << " " << std::to_string(enemies[i].y) << " " << std::endl;
		}
		message = "Game saved successfuly\n";
		cfg.close();
	}
	else
		message = "Error saving. Game not saved.\n";
	return message;
}

std::string loadGame(player& hero, player enemies[])
{	// Функция загрузки данных об игроках из файла. Т.к. все данные текстовые, то их
	// приходится преобразовывать из текста в int
	std::ifstream cfg;
	std::string message;
	cfg.open("Battle.cfg");
	if (cfg.is_open())
	{
		cfg >> hero.name;
		cfg >> message;
		hero.health = stoi(message);
		cfg >> message;
		hero.armor = stoi(message);
		cfg >> message;
		hero.damage = stoi(message);
		cfg >> message;
		hero.x = stoi(message);
		cfg >> message;
		hero.y = stoi(message);
		for (int i = 0; i < 5; ++i)
		{
			cfg >> enemies[i].name;
			cfg >> message;
			enemies[i].health = stoi(message);
			cfg >> message;
			enemies[i].armor = stoi(message);
			cfg >> message;
			enemies[i].damage = stoi(message);
			cfg >> message;
			enemies[i].x = stoi(message);
			cfg >> message;
			enemies[i].y = stoi(message);
		}
		cfg.close();
		message = "Game loaded succesfuly.\n";
	}
	else
		message = "Error reading. Game not loaded.\n";
	return message;
}

void helpMe()
{  // Отображает подсказку по программе
	std::cout << "\x1B[2J\x1B[H";
	std::cout << "U/u/Key <Up> - moves the HERO up\nD/d/Key <Down> - moves the HERO down\n" << \
		"R/r/Key <Right> - moves HERO right\nL/l/Key <Left> - moves HERO left\nX - exit\n" << \
		"F2 - Save game\nF3 - Load game\n";
	system("pause");
}

void refreshField(player& hero, player enemies[], char kulikovo[][20])
{	// Очищаем поле
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 20; ++j)
			kulikovo[i][j] = '.';
	// Расставляем игроков
	kulikovo[hero.x][hero.y] = 'H';
	for (int i = 0; i < 5; ++i)
		kulikovo[enemies[i].x][enemies[i].y] = 'E';
}

int main()
{  // Главная функция отображает поле, анализирует нажатия клавиш и управляет HERO.
	int ch = 0;
	player hero;
	player enemies[5];
	char kulikovo [20][20] = {};
	std::string message = "\n", temp;
	
	initField(hero, enemies, kulikovo);
	while (ch != 135 && ch != 120)
	{
		showField(kulikovo);
		std::cout << message << "Press F1 for help.\nEnter command : ";
		ch = _getch(); ch = _getch();
		if (ch == 72 || ch == 179 || ch == 117)
		{	// Если при движении по оси Y ВВЕРХ встречается враг или край поля - останавливаемся.
			if(hero.y + 1 < 20)
			{
				if (kulikovo[hero.x][hero.y + 1] == '.')
				{
					kulikovo[hero.x][hero.y] = '.';
					hero.y++;
					kulikovo[hero.x][hero.y] = 'H';
				}
				else
					// Hero атакует врага с юга
					temp = stroke(hero, enemies, hero.x, hero.y + 1, kulikovo);
			}
		}
		else if (ch == 80 || ch == 178 || ch == 100)
		{	// Если при движении по оси Y ВНИЗ встречается враг или край поля - останавливаемся.
			if(hero.y - 1 >= 0)
			{
				if (kulikovo[hero.x][hero.y - 1] == '.')
				{
					kulikovo[hero.x][hero.y] = '.';
					hero.y--;
					kulikovo[hero.x][hero.y] = 'H';
				}
				else
					//HERO атакует врага с севера
					temp = stroke(hero, enemies, hero.x, hero.y - 1, kulikovo);
			}
		}
		else if (ch == 75 || ch == 180 || ch == 108)
		{	// Если при движении по оси Х НАЛЕВО встречается враг или край поля - останавливаемся.
			if (hero.x - 1 >= 0)
			{
				if(kulikovo[hero.x - 1][hero.y] == '.')
				{
					kulikovo[hero.x][hero.y] = '.';
					hero.x--;
					kulikovo[hero.x][hero.y] = 'H';
				}
				else
					//HERO атакует врага справа
					temp = stroke(hero, enemies, hero.x - 1, hero.y, kulikovo);
			}
		}
		else if (ch == 77 || ch == 186 || ch == 114)
		{	// Если при движении по оси Х НАПРАВО встречается враг или край поля - останавливаемся.
			if (hero.x + 1 < 20)
			{
				if (kulikovo[hero.x + 1][hero.y] == '.')
				{
					kulikovo[hero.x][hero.y] = '.';
					hero.x++;
					kulikovo[hero.x][hero.y] = 'H';
				}
				else
					//HERO атакует врага слева
					temp = stroke(hero, enemies, hero.x + 1, hero.y, kulikovo);
			}
		}
		else if (ch == 60)
		{	// Сохраняем игру
			temp = saveGame(hero, enemies);
			refreshField(hero, enemies, kulikovo);
		}
		else if (ch == 61)
		{	// Загружаем игру
			temp = loadGame(hero, enemies);
			refreshField(hero, enemies, kulikovo);
		}
		else if (ch == 59)
		{	// Страница помощи
			helpMe();
		}
		else
		{
			temp = "Code of pressed key: " + std::to_string(ch) + "\n";
		}
		showField(kulikovo);
		message = moveEnemies(hero, enemies, kulikovo);
		if (message == "\n")
			message = temp;
		showField(kulikovo);
	}
	std::cout << "bye bye\n";
	return 0;
}