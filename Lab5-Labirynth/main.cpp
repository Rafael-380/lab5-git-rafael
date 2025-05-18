#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class CustomSprite : public sf::Sprite {
public:
    void setBounds(int left, int right, int top, int bottom) {
        bound_top = top;
        bound_bottom = bottom;
        bound_right = right;
        bound_left = left;
    }

    void moveInDirection(const sf::Time &elapsed, const std::vector<sf::Sprite> &obstacles) {
        float distance = m_speed * elapsed.asSeconds();

        sf::Vector2f directions[] = {
            sf::Vector2f(0, -distance), // Up
            sf::Vector2f(0, distance),  // Down
            sf::Vector2f(-distance, 0), // Left
            sf::Vector2f(distance, 0)   // Right
        };

        sf::Keyboard::Key keys[] = {
            sf::Keyboard::Up,
            sf::Keyboard::Down,
            sf::Keyboard::Left,
            sf::Keyboard::Right
        };

        for (int i = 0; i < 4; ++i) {
            if (sf::Keyboard::isKeyPressed(keys[i])) {
                sf::FloatRect futureBounds = getGlobalBounds();
                futureBounds.left += directions[i].x;
                futureBounds.top += directions[i].y;

                // Verifica limites da janela
                if (futureBounds.left < bound_left ||
                    futureBounds.top < bound_top ||
                    futureBounds.left + futureBounds.width > bound_right ||
                    futureBounds.top + futureBounds.height > bound_bottom) {
                    continue; // Ignora movimento se sair da janela
                }

                // Verifica colisões com paredes
                bool collision = false;
                for (const auto& wall : obstacles) {
                    if (futureBounds.intersects(wall.getGlobalBounds())) {
                        collision = true;
                        break;
                    }
                }

                if (!collision) {
                    move(directions[i]);
                }
            }
        }
    }


private:
    float m_speed = 200.0f;
    int bound_top = 0;
    int bound_bottom = 0;
    int bound_left = 0;
    int bound_right = 0;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth");
    sf::Clock clock;

    // Textures
    sf::Texture guy_tex;
    if (!guy_tex.loadFromFile("guy.png")) {
        std::cerr << "Failed to load guy.png" << std::endl;
        return 1;
    }

    sf::Texture grass_tex;
    if (!grass_tex.loadFromFile("grass.png")) {
        std::cerr << "Failed to load grass.png" << std::endl;
        return 1;
    }
    grass_tex.setRepeated(true);

    sf::Texture wall_tex;
    if (!wall_tex.loadFromFile("wall.png")) {
        std::cerr << "Failed to load wall.png" << std::endl;
        return 1;
    }

    // Background
    sf::Sprite grass;
    grass.setTexture(grass_tex);
    grass.setTextureRect(sf::IntRect(0, 0, 800, 600));

    // Player
    CustomSprite guy;
    guy.setTexture(guy_tex);
    guy.setPosition(100, 100);
    guy.setBounds(0, 800, 0, 600);

    // Walls
    std::vector<sf::Sprite> walls;
    auto createWall = [&](float x, float y, float scaleX = 0.3f, float scaleY = 0.3f) {
        sf::Sprite wall;
        wall.setTexture(wall_tex);
        wall.setScale(scaleX, scaleY);
        wall.setPosition(x, y);
        walls.push_back(wall);
    };

    // Example walls
    createWall(200, 100);
    createWall(300, 100);
    createWall(400, 100);
    createWall(400, 200);
    createWall(400, 300);
    createWall(200, 300);
    createWall(200, 400);
    createWall(300, 400);
    createWall(100, 300);

    // Game loop
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);

        guy.moveInDirection(elapsed, walls);

        window.clear();
        window.draw(grass);
        window.draw(guy);
        for (const auto& wall : walls) {
            window.draw(wall);
        }
        window.display();
    }

    return 0;
}


