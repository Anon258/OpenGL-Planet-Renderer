#ifndef SphereGeneration_hpp
#define SphereGeneration_hpp

#include <GL/glew.h>
#include <vector>
#include <cmath>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Sphere{
public:
    int stack, sector;
    float radius;
    std::vector<float> positions;
    //std::vector<float> normal;
    std::vector<int> indices;
    
    float sectorStep, stackStep;
    float sectorAngle, stackAngle;
    
    int positionsSize, indicesSize;
    
    Sphere();
};

#endif /* SphereGeneration_hpp */
