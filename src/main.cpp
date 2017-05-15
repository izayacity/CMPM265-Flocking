#include <SFML/Graphics.hpp>
#include "../include/Flock.h"
#include "../include/NewVector.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <array>
#include <iomanip>      // std::setprecision

int main () {
	const int gameWidth = 1280;
	const int gameHeight = 720;

	std::srand (static_cast<unsigned int>(std::time (NULL)));
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window (sf::VideoMode (gameWidth, gameHeight), "Flocking", sf::Style::Default, settings);
	sf::Clock clock;

	// load fonts
	sf::Font fontHNL;
	assert (fontHNL.loadFromFile ("static/font/HelveticaNeue Light.ttf"));

	// draw the background of text field on the top for stats
	sf::RectangleShape statsField;
	statsField.setFillColor (sf::Color::Black);
	statsField.setSize (sf::Vector2f (gameWidth, 30));
	statsField.setPosition (0, 0);

	// display the text of stats
	sf::Text statsText;
	statsText.setFont (fontHNL);
	statsText.setCharacterSize (20);
	statsText.setFillColor (sf::Color::Green);
	statsText.setPosition (5, 5);
	
	// draw the background of text field at the bottom of key commands text
	sf::RectangleShape keyField;
	keyField.setFillColor (sf::Color::Black);
	keyField.setSize (sf::Vector2f (gameWidth, 60));
	keyField.setPosition (0, gameHeight - 60);

	// display key commands text
	sf::Text keyText[2];
	sf::String msg[2] = {
		"Ctrl+C to add cohesion; Alt+C to reduce cohesion; Ctrl+A to add align; Alt+A to reduce align",
		"RMouseBtn to add boids; Alt+T to reduce boids; Ctrl+S to add separation; Alt+S to reduce separation"
	};
	int text_position = 25;

	for (int i = 0; i < 2; i++) {
		keyText[i].setString (msg[i]);
		keyText[i].setFont (fontHNL);
		keyText[i].setCharacterSize (20);
		keyText[i].setFillColor (sf::Color::Blue);
		keyText[i].setPosition (5, gameHeight - text_position);
		text_position += 30;
	}

	// create flocking boids
	Flock boids;

	while (window.isOpen ()) {
		sf::Event event;	
		sf::Vector2i mouse = sf::Mouse::getPosition (window);

		while (window.pollEvent (event)) {
			if (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
				window.close ();
			}
			
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Right) {
					boids.addBoid (sf::Vector2f (event.mouseButton.x, event.mouseButton.y));
				}
			}

			else if (event.type == sf::Event::TextEntered) {
				if (sf::Keyboard::isKeyPressed (sf::Keyboard::T) &&
				(sf::Keyboard::isKeyPressed (sf::Keyboard::LAlt) ||
				sf::Keyboard::isKeyPressed (sf::Keyboard::RAlt))) {
					boids.reduceBoid ();
				}
			}
		}
		
		// get weight of flock function
		std::stringstream stream;
		stream << std::fixed << std::setprecision (1) << boids.getWeight ()[0];
		std::string seperationStr = stream.str();
		stream.str ("");
		stream << std::fixed << std::setprecision (1) << boids.getWeight ()[1];
		std::string cohesionStr = stream.str ();
		stream.str ("");
		stream << std::fixed << std::setprecision (1) << boids.getWeight ()[2];
		std::string alignStr = stream.str ();
		stream.str ("");

		// update boids
		boids.update ();

		statsText.setString ("fps: " + std::to_string (static_cast<int>(1.f / clock.restart ().asSeconds ()))
		+ "  total: " + std::to_string (boids.getCount ())
		+ "  separation: " + seperationStr + "  cohesion: " + cohesionStr + "  align: " + alignStr);		

		window.clear ();
		window.draw (boids);
		window.draw (statsField);
		window.draw (statsText);
		window.draw (keyField);
		for (auto text : keyText) {
			window.draw (text);
		}
		window.display ();
	}

	return 0;
}
