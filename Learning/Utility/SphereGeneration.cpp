#include "SphereGeneration.hpp"

Sphere::Sphere(){
    stack = 150;
    sector = 150;
    radius = 1.0f;
    
    sectorStep = (float) 2*M_PI/sector;
    stackStep = (float) M_PI/stack;
    
    
    for(int i = 0; i<=stack; i++){
        stackAngle = (float) M_PI/2 - i*stackStep;
        float xy = radius * (float) cos(stackAngle);
        float z = radius * (float) sin(stackAngle);
        
        for(int j = 0; j<=sector; j++){
            sectorAngle = j*sectorStep;
            float x = xy * (float) cos(sectorAngle);
            float y = xy * (float) sin(sectorAngle);
            
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
            
            float nx = x/radius;
            float ny = y/radius;
            float nz = z/radius;
            positions.push_back(nx);
            positions.push_back(ny);
            positions.push_back(nz);
        }
    }
    
    int k1, k2; //to mark 2 adjacent stacks;
    
    for(int i = 0; i< stack; i++){
        k1 = i*(stack + 1);
        k2 = k1 + stack + 1; //stack just below in same sector;
        
        // We now create 2 triangles here
        for(int j = 0; j< sector; j++){
            
            // 2 triangles per sector excluding first and last stacks, also, not that even though top and bottom are a single point, we still have 25 of them, so we can proceed normally for the single triangle as well
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            // k1+1 => k2 => k2+1
            if(i != (stack-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
            
            k1++;
            k2++;
        }
    }
    
    indicesSize = indices.size();
    positionsSize = positions.size();
}
