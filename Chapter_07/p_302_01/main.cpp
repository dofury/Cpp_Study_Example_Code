#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <SFML/System/Time.hpp>
#include <SFML/Config.hpp>
#include <SFML/System/Export.hpp>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;

class LunarLander
{		
public:
	float x, y;
	float velocity;
	float fuel;
	string status;
	Texture t1, t2;
	Sprite spaceship, burst;
	Font font;
	Text text;
	bool flag;	// ���� �÷���
	LunarLander(float h, float v, float f)
	{
		flag = false;
		x = 500.f;
		y = h;
		velocity = v;
		fuel = f;
		t1.loadFromFile("images/spaceship.png");
		t2.loadFromFile("images/burst.png");
		spaceship.setTexture(t1);
		spaceship.setOrigin(spaceship.getTexture()->getSize().x / 2.f , spaceship.getTexture()->getSize().y / 2.f);
		spaceship.setPosition(x, y);
		burst.setTexture(t2);
		burst.setOrigin(burst.getTexture()->getSize().x / 2.f, burst.getTexture()->getSize().y / 2.f);
		burst.setPosition(spaceship.getPosition().x, spaceship.getPosition().y + 60.f);

		if (!font.loadFromFile("fonts/OpenSans-Bold.ttf"))
		{
			cout << "��Ʈ ������ ������ �� ����!" << endl;
		}
		text.setFont(font);
	}
	void update(float amount, float dt)
	{
		if (fuel <= 0)
		{
			fuel = 0;
			amount = 0;
		}
		fuel -= amount;
		velocity += -amount + 0.8f * dt * 3.3f;	// ��ŸŸ�� ���ؼ� ��� ��ǻ�� ������ ��� ����
		if(!flag) y += velocity;	// �߷�
		spaceship.setPosition(x, y);
		status = "Watch out. If the speed is below -5 or above 5, the ship will break!\nheight: " + to_string(y) +"\nspeed: " + to_string(velocity) + "\nfuel: " + to_string(fuel);
	}
	void draw(RenderWindow& window)
	{
		window.draw(spaceship);
		window.draw(burst);
		text.setString(status);
		text.setCharacterSize(35);
		text.setPosition(10, 100);
		window.draw(text);
	}
};

int main()
{
	RenderWindow window(VideoMode(1920, 1080), "LUNAR LANDER");
	window.setFramerateLimit(60);
	Clock dtClock;	// ��ŸŸ��
	float dt;	// ��ŸŸ��
	Texture t, t2;	// ���� �� �ؽ���
	Sprite background, land;	// ���� �� ��������Ʈ
	t.loadFromFile("images/background.png");
	background.setTexture(t);
	t2.loadFromFile("images/land.png");
	land.setTexture(t2);
	land.setPosition(1920 / 2, 700);
	LunarLander lander(300.0, 1.0, 100.0);		
	int score = 0;
	bool flag1 = false;	// ���� �÷���
	bool flag_faile = false;	// ���� �÷���
	Text end_text;
	end_text.setFont(lander.font);
	end_text.setString("Your Spaceship is safe!!\n         Youre Score: " + to_string(score));
	end_text.setCharacterSize(75);

	FloatRect textRect = end_text.getGlobalBounds();	// ���� �߾� ������ ���� �簢 rect ����
	end_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
	end_text.setPosition(3000.f, 3000.f);

	while (window.isOpen())
	{
		dt = dtClock.restart().asSeconds();
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{ 
			lander.update(1.0f, dt);
			if (lander.fuel > 0)
			{
				lander.burst.setPosition(lander.spaceship.getPosition().x, lander.spaceship.getPosition().y + 60);
			}
		}
		else
		{
			lander.update(0.0, dt);
			lander.burst.setPosition(3000.f, 3000.f);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) && !flag1)
			lander.x += 35.f * dt;
		if (Keyboard::isKeyPressed(Keyboard::Left) && !flag1)
			lander.x -= 35.f * dt;

		if (land.getGlobalBounds().intersects(lander.spaceship.getGlobalBounds()) &&
			lander.y > 710.f && lander.y < 740.f && lander.velocity > -5.f && lander.velocity < 5.f)
		{
			if (!flag_faile)
			{
				// ���� �÷��װ� �ƴҽÿ��� �¸� ���� ������ �̰� ���ϸ� ������ �ӷ��� 0�̶� ���� ǥ�� ���ٰ� ���� ǥ�ö�
				score = (int)(lander.fuel + 100 - lander.velocity) * 15;
				end_text.setString("Your Spaceship is safe!!\n         Youre Score: " + to_string(score));
				end_text.setPosition(1920 / 2.f, 1080 / 2.f - 200.f);
				flag1 = true;	
			}
			//cout << "���� ����" << endl;
		}
		if (lander.spaceship.getPosition().y >= 1100 || 
			(land.getGlobalBounds().intersects(lander.spaceship.getGlobalBounds()) &&
			(lander.y > 710.f && lander.y < 740.f ) && (lander.velocity < -5.f || lander.velocity > 5.f)))
		{
			score = (int)(lander.fuel + 100 - lander.velocity);
			end_text.setString("Your spaceship crashed...\n         Youre Score: " + to_string(score));
			end_text.setPosition(1920 / 2.f, 1080 / 2.f - 200.f);
			flag1 = true;
			flag_faile = true;
			lander.flag = true;
		}
		if (flag1)
		{
			lander.x = lander.spaceship.getPosition().x;
			lander.y = lander.spaceship.getPosition().y;
			lander.velocity = 0;	// ���� �÷��׸� ����
		}
		window.clear();
		window.draw(background);
		window.draw(land);
		lander.draw(window);
		window.draw(end_text);
		window.display();
		Sleep(50);			
	}

	return 0;
}
