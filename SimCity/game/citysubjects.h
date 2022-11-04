/*
* Header file defining the persons and goods structs of the simulation
*
* Author: Kevin
*/
#ifndef CITYSUBJECTS_H
#define CITYSUBJECTS_H

class Region;

/*
* The Person Struct objects will be stored in the Residential Zones, while the Good struct objects will be stored in the Industrial Zones
*
* Two vectors outside the region map will store pointers to available workers and goods, and will be passed into the
* Update() function of the Industrial and Commercial zones (TODO:)
*
*/
struct Person{
public:
    Person(){ residence = nullptr; workLocation = nullptr; }
    Person(Region* initresidence) { residence = initresidence; workLocation = nullptr; }
    void setResidence(Region* newResidence) { residence = newResidence; }
    void setWorkLocation(Region* newWorkLocation) { workLocation = newWorkLocation; }
    Region* getResidence() { return residence; }
    Region* getWorkLocation() { return workLocation; }
private:
    Region* residence;
    Region* workLocation;
};

struct Good{
public:
    Good(){ source = nullptr; vendor = nullptr; }
    Good(Region* initsource) { source = initsource; vendor = nullptr; }
    void setSource(Region* newSource) { source = newSource; }
    void setVendor(Region* newVendor) { vendor = newVendor; }
    Region* getSource() { return source; }
    Region* getVendor() { return vendor; }
private:
    Region* source;
    Region* vendor;
};

#endif