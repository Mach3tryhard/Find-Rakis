#include "GUI.h"
#include "Bullet.h"
#include "Pair.h"
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "SpaceShip.h"
#include "SolarSystem.h"

sf::Text& GUI::getText() {
    return this->debugText;
}
void GUI::UpdateGUI(SpaceShip& player,SolarSystem solar_system) {
    std::string posText ="position: x: " + std::to_string(player.getPhysics().getPosition().x)
    +" y: " + std::to_string(player.getPhysics().getPosition().y)
    + "\ndirection: "+ std::to_string(player.getShape().getRotation().asDegrees())
    + "\nvelocity: x " + std::to_string(player.getPhysics().getVelocity().x)
    +"y: " + std::to_string(player.getPhysics().getVelocity().y)
    + "\nacceleration: x " + std::to_string(player.getPhysics().getAcceleration().x)
    +" y: " + std::to_string(player.getPhysics().getAcceleration().y)
    +"\nSolar System-> x: " + std::to_string(solar_system.getPhysics().getPosition().x)
    +" y: " + std::to_string(solar_system.getPhysics().getPosition().y);
    debugText.setString(posText);
}