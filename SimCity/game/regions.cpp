/*
* Code file defining the functions for all region classes in the simulation
*/
#include <iostream>
#include <iomanip>
#include <string>
#include "regions.h"
#include "citysubjects.h"

/* 
* Parent Region Class 

* (Base class in which everything other region derives from)
* (By itself, the base class acts as an empty region in the simulation)
* (The Region class's functions act as an access point to each and every derived class under a single type)
*/
Region::Region(){
    pollution = 0;
    population = 0;
    regionLetter = " ";
    xCoord = 0;
    yCoord = 0;
}
Region::Region(int x, int y){
    pollution = 0;
    population = 0;
    regionLetter = " ";
    xCoord = x;
    yCoord = y;
}
void Region::SetPollution(int newPollution){ pollution = newPollution; }
int Region::GetPollution() { return pollution; }
void Region::SetPopulation(int newPopulation){ population = newPopulation; }
int Region::GetPopulation() { return population; }
int Region::GetXCoord(){ return xCoord; }
int Region::GetYCoord(){ return yCoord; }
std::string Region::GetRegionLetter() {
    if(population > 0 && regionLetter != " "){
        return "" + std::to_string(population);
    }
    return this->regionLetter;
}
        /* Virtual Function for Derived Class Access; Serves no purpose by itself, however, is the driving force of all other regions */
bool Region::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods){
    std::cout << "If you are accessing this function, then you did something wrong" << std::endl;
    return false;
}
        /* Virtual Function for detailed info about a region; overridden by derived class toString() functions */
void Region::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Empty Region   (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
}



/* 
* Residential Region Class 
*
* (The Residential zone's zole purpose is to provide workers as it grows in population)
* (CheckUpdate() gathers @adjacencies and scans for potential growth. When conditions are met, @availableWorkers is altered)
*/
Residential::Residential(int x, int y){ xCoord = x; yCoord = y; regionLetter = "R"; }

        /* Main Driver of Residential Zone Growth */
bool Residential::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 
    int count = 0;
        //Increase population based on surrounding neighbors
    for(Region* region : adjacencies){

        if(population == 0 && dynamic_cast<Powerline*>(region) != nullptr){                     //Check for powerline and population 1 at population == 0 are separate
            
            residents.push_back(new Person(this));
            availableWorkers.push_back(residents.back());
            return true;
        }
        else if(population == 0 && region->GetPopulation() >= 1){
            
            residents.push_back(new Person(this));
            availableWorkers.push_back(residents.back());
            return true;
        }
        else if(population == 1 && region->GetPopulation() >= 1){
            
            count += 1;
            if(count >= 2){
                residents.push_back(new Person(this));
                availableWorkers.push_back(residents.back());
                return true; 
            }
        }
        else if(population == 2 && region->GetPopulation() >= 2){
            
            count += 1;
            if (count >= 4){
                residents.push_back(new Person(this));
                availableWorkers.push_back(residents.back());
                return true; 
            }
        }
        else if(population == 3 &&  region->GetPopulation() >= 3){
            
            count += 1;
            if(count >= 6){
                residents.push_back(new Person(this));
                availableWorkers.push_back(residents.back());
                return true; 
            }
        }
        else if(population == 4 && region->GetPopulation() >= 4){
            
            count += 1;
            if(count >= 8){
                residents.push_back(new Person(this));
                availableWorkers.push_back(residents.back());
                return true; 
            }
        }

    }

    return false;
}
void Residential::toString(){
    std::cout << "---------------------------------\n";
    std::cout << std::right;
    std::cout << "| Residential      (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "---------------------------------\n";
    std::cout << std::left;
    std::cout << "| Population = " << std::setw(17) << population << "|\n";
    std::cout << "| Pollution = " << std::setw(18) << pollution << "|\n";
    std::cout << "---------------------------------\n";
    std::cout << "| Residents:                    |\n";
    std::cout << std::right;

    for(Person* resident : residents){
        if(resident->getWorkLocation() != nullptr){
            std::cout << "| Resident working at (" << std::setw(3) << resident->getWorkLocation()->GetXCoord() << "," << std::setw(3) << resident->getWorkLocation()->GetYCoord() << ") |\n";
        }
        else{
            std::cout << "| Unemployed Resident           |\n";
        }
    }
    std::cout << "---------------------------------\n";
}


/* 
* Industrial Region Class 
*
* (The Industrial zone has the most active role in the simulation; it grows due to adjacency conditions/available workers, produces pollution, and provides goods to the Commercial Region)
* (CheckUpdate() gathers @adjacencies to scan for half the satisfactory growth conditions. The other half is from scanning from @availableWorkers. When growth conditions are met, @availableGoods is altered)
* (Pollution functionality is handled outside the Industrial Region object, via the region map iteration TODO:)
*/
Industrial::Industrial(int x, int y){ xCoord = x; yCoord = y; regionLetter = "I"; }

        /* Main Driver of Industrial Growth */
bool Industrial::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 
    int count = 0;
        //Increase population based on surrounding neighbors AND available workers
    for(Region* region : adjacencies){
    
        if(population == 0 && dynamic_cast<Powerline*>(region) != nullptr && availableWorkers.size() >= 2){         //Check for powerline and population of >=1 are separate

            for(int i = 0; i < 2; i++){                                                                                     //Two workers required for the industrial to grow once
                availableWorkers.back()->setWorkLocation(this);                                                                 //Set worker work location to self, as well as store the pointer to the worker in @workers, popping the newly acquired worker off @available workers
                workers.push_back(availableWorkers.back());
                availableWorkers.pop_back();
            }
            goods.push_back(new Good(this));                                                                                //One good per industrial zone growth
            availableGoods.push_back(goods.back());

            return true;
        }
        if(population == 0 && region->GetPopulation() >= 1 && availableWorkers.size() >= 2){

            for(int i = 0; i < 2; i++){
                availableWorkers.back()->setWorkLocation(this);
                workers.push_back(availableWorkers.back());
                availableWorkers.pop_back();
            }
            goods.push_back(new Good(this));
            availableGoods.push_back(goods.back());

            return true;
        }
        else if(population == 1 && region->GetPopulation() >= 1 && availableWorkers.size() >= 2){

            count += 1;
            if(count >= 2){ 

                for(int i = 0; i < 2; i++){
                    availableWorkers.back()->setWorkLocation(this);
                    workers.push_back(availableWorkers.back());
                    availableWorkers.pop_back();
                }
                goods.push_back(new Good(this));
                availableGoods.push_back(goods.back());

                return true; 
            }
        }
        else if(population == 2 && region->GetPopulation() >= 2 && availableWorkers.size() >= 2){

            count += 1;
            if(count >= 4){ 

                for(int i = 0; i < 2; i++){
                    availableWorkers.back()->setWorkLocation(this);
                    workers.push_back(availableWorkers.back());
                    availableWorkers.pop_back();
                }
                goods.push_back(new Good(this));
                availableGoods.push_back(goods.back());

                return true; 
            }
        }
        
    }

    return false;
}
void Industrial::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Industrial     (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Population = " << std::setw(15) << population << "|\n";
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
    std::cout << "| Workers:                    |\n";
    std::cout << std::right;
    
    for(Person* worker : workers){
        std::cout << "| Worker living at (" << std::setw(3) << worker->getResidence()->GetXCoord() << "," << std::setw(3) << worker->getResidence()->GetYCoord() << ")  |\n";
    }

    std::cout << "-------------------------------\n";
    std::cout << "| Goods:                      |\n";

    for(Good* good : goods){
        if(good->getVendor() != nullptr){
            std::cout << "| Good sold at (" << std::setw(3) << good->getVendor()->GetXCoord() << "," << std::setw(3) << good->getVendor()->GetYCoord() << ")      |\n";
        }
        else{
            std::cout << "| Unsold Good                 |\n";
        }
    }
    std::cout << "-------------------------------\n";
}


/* 
* Commercial Region Class 
*
* (The Commercial zone does not provide resources to the simulation and simply grows. A commercial region will grow only when provided workers and goods to sell)
* (CheckUpdate() gathers @adjacencies to scan for a portion of the satisfactory growth conditions. @availableWorkers and @availableGoods are scanned as well. There are no other changes beyond population growth) 
*/
Commercial::Commercial(int x, int y){ xCoord = x; yCoord = y; regionLetter = "C"; }

        /* Main Driver of Commercial Growth */
bool Commercial::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 
    int count = 0;
        //Increase population based on surrounding neighbors, available workers, AND available goods
    for(Region* region : adjacencies){

        if(population == 0 && dynamic_cast<Powerline*>(region) != nullptr && availableWorkers.size() >= 1 && availableGoods.size() >= 1){   //Check Powerline and population >=1 nearby are separate at population == 0

            availableWorkers.back()->setWorkLocation(this);                                                                                         //One person and one good is required to grow the commercial district
            workers.push_back(availableWorkers.back());
            availableWorkers.pop_back();

            availableGoods.back()->setVendor(this);
            goods.push_back(availableGoods.back());
            availableGoods.pop_back();

            return true; 
        }
        else if(population == 0 && region->GetPopulation() >= 1 && availableWorkers.size() >= 1 && availableGoods.size() >= 1){

            availableWorkers.back()->setWorkLocation(this);
            workers.push_back(availableWorkers.back());
            availableWorkers.pop_back();

            availableGoods.back()->setVendor(this);
            goods.push_back(availableGoods.back());
            availableGoods.pop_back();

            return true;
        }
        else if(population == 1 && region->GetPopulation() >= 1 && availableWorkers.size() >= 1 && availableGoods.size() >= 1){

            count += 1;
            if(count >= 2){

                availableWorkers.back()->setWorkLocation(this);
                workers.push_back(availableWorkers.back());
                availableWorkers.pop_back();

                availableGoods.back()->setVendor(this);
                goods.push_back(availableGoods.back());
                availableGoods.pop_back();

                return true;
            }
        }

    }

    return false;
}
void Commercial::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Commercial     (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Population = " << std::setw(15) << population << "|\n";
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
    std::cout << "| Workers:                    |\n";
    std::cout << std::right;

    for(Person* worker : workers){
        std::cout << "| Worker living at (" << std::setw(3) << worker->getResidence()->GetXCoord() << "," << std::setw(3) << worker->getResidence()->GetYCoord() << ")  |\n";
    }

    std::cout << "-------------------------------\n";
    std::cout << "| Goods:                      |\n";

    for(Good* good : goods){
        std::cout << "| Good made at (" << std::setw(3) << good->getSource()->GetXCoord() << "," << std::setw(3) << good->getSource()->GetYCoord() << ")      |\n";
    }
    std::cout << "-------------------------------\n";
}


/* 
* Powerplant Region Class 
*
* (The Powerplant currently has no implementation)
* (A placeholder class has been created for possible future implementation)
*/
Powerplant::Powerplant(int x, int y){ xCoord = x; yCoord = y; regionLetter = "P"; }
bool Powerplant::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods){

    return false;
}
void Powerplant::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Powerplant     (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
}


/* 
* Powerline Region Class 
*
* (The Powerline has unrequested implementation)
* (It was originally thought that a powerline was only capable of powering a single region on its own, however, that implementation is obsolete)
* (In case the guidelines requires so, the implementation has stayed for such a possibility)
*/
Powerline::Powerline(int x, int y){ xCoord = x; yCoord = y; regionLetter = "T"; isPowered = true;}
bool Powerline::GetPowered() { return isPowered; }
void Powerline::TogglePowered(){ isPowered = !isPowered; }
bool Powerline::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 

    return false;
}
void Powerline::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Powerline      (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
}


/* 
* Road Region Class 
*
* (The Road currently has no implementation)
* (A placeholder class has been created for possible future implementation)
*/
Road::Road(int x, int y){ xCoord = x; yCoord = y; regionLetter = "-"; }
bool Road::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 

    return false;
}
void Road::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| Road           (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
}


/* 
* PowerlineRoad Region Class 
*
* (The PowerlineRoad currently has no implementation)
* (A placeholder class has been created for possible future implementation)
*/
PowerlineRoad::PowerlineRoad(int x, int y){ xCoord = x; yCoord = y; regionLetter = "#"; }
bool PowerlineRoad::CheckUpdate(std::vector<Region*>& adjacencies, std::vector<Person*>& availableWorkers, std::vector<Good*>& availableGoods) { 

    return false;
}
void PowerlineRoad::toString(){
    std::cout << "-------------------------------\n";
    std::cout << std::right;
    std::cout << "| PowerlineRoad  (" << std::setw(4) << xCoord << ", " << std::setw(4) << yCoord << ") |\n";
    std::cout << "-------------------------------\n";
    std::cout << std::left;
    std::cout << "| Pollution = " << std::setw(16) << pollution << "|\n";
    std::cout << "-------------------------------\n";
}
