#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
    printf( "\n Spaceship constructor");
    ObjectModel::getBuffers()->Create();
    ObjectBoundingBox = ObjectModel::getBuffers()->getBufferBoundingBox();

    // automatically calculatd in scaleBB
    /*xLength = ObjectModel::ObjectBoundingBox.Xmax - ObjectModel::ObjectBoundingBox.Xmin;
    yLength = ObjectModel::ObjectBoundingBox.Ymax - ObjectModel::ObjectBoundingBox.Ymin;
    zLength = ObjectModel::ObjectBoundingBox.Zmax - ObjectModel::ObjectBoundingBox.Zmin;*/

    ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);

    Center.x = 0;
    Center.y = 0;
    Center.z = 0;

// Fuel Calculation
    Fuel = 10000;
    Velocity = 0;

}
Spaceship::~Spaceship()
{}

