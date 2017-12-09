#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Diffuse.bmp")
{
   printf( "\n Spaceship constructor");
	ObjectModel::getBuffers()->Create();
	// scale spaceship 
	ObjectModel::scaleObject(0.25f, 0.25f, 0.25f);
	//ObjectModel::setTexture("Diffusion.bmp");

}
Spaceship::~Spaceship()
{}

