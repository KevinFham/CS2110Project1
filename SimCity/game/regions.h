/*
* Header file defining the signatures for all region classes in the simulation
*/
#ifndef REGIONS_H
#define REGIONS_H

#include <vector>
#include "citysubjects.h"

/* Parent Region Class */
class Region{
public:
    Region();
    Region(int,int);
    void SetPollution(int);
    int GetPollution();
    void SetPopulation(int);
    int GetPopulation();
    std::string GetRegionLetter();
    int GetXCoord();
    int GetYCoord();
    virtual bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    virtual void toString();
protected:
    int pollution;
    int population;
    int xCoord, yCoord;
    std::string regionLetter;
};


/* Residential Region Class */
class Residential : public Region{
public:
    Residential(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
private:
    std::vector<Person*> residents;
};


/* Industrial Region Class */
class Industrial : public Region{
public:
    Industrial(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
private:
    std::vector<Person*> workers;
    std::vector<Good*> goods;
};


/* Commercial Region Class */
class Commercial : public Region{
public:
    Commercial(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
private:
    std::vector<Person*> workers;
    std::vector<Good*> goods;
};


/* Powerplant Region Class */
class Powerplant : public Region{
public:
    Powerplant(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
};


/* Powerline Region Class */
class Powerline : public Region{
public:
    Powerline(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    bool GetPowered();
    void TogglePowered();
    void toString();
private:
    bool isPowered;
};


/* Road Region Class */
class Road : public Region{
public:
    Road(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
};


/* PowerlineRoad Region Class */
class PowerlineRoad : public Region{
public:
    PowerlineRoad(int,int);
    bool CheckUpdate(std::vector<Region*>&, std::vector<Person*>&, std::vector<Good*>&);
    void toString();
};

#endif
