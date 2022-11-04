/*
*   CSCE2110.001 Project 1 SimCity
*   Group 4
*
*   Members: Kevin Pham, Suraj Varne Sheela, Divyam Jain
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "regions.h"
using namespace std;

/* Function Signatures */
void DisplayMenu(int[3], vector<vector<Region*>>, vector<Person*>, vector<Good*>);
bool ExecuteMenu(char, int(&)[3], vector<vector<Region*>>&, vector<Person*>&, vector<Good*>&);
void UpdateSimulation(vector<vector<Region*>>&, vector<Person*>&, vector<Good*>&);


/* Main Function */
int main(){
    
    /* Config File Reading (Author: Kevin, Divyam, and Suraj) */
    string single_line;
    string data;
    string buffer;
    ifstream configFile;
    vector<vector<Region*>> regionMap;          //Create region map
    vector<Person*> availableWorkers;               //And its subjects
    vector<Good*> availableGoods;
    int simulProperties[3] = {1, 0, 0};         //Holds the number of iterations, max iterations, and refresh rate

    cout << "Enter the file name of the configuration file" << endl;            //Redirection in command line does not allow for easy access to istream manipulation, so this will have to do.
    getline(cin, buffer);
    configFile.open(buffer);

    if (configFile){                            //Check if config opens; Extract and output data

        stringstream ss;

        cout << endl;
        getline(configFile, data);                              //Get Region File
        buffer = data.substr(data.find(":") + 1);
        cout << "Region file name: " << buffer << endl;

        getline(configFile, data);                              //Get Max Time Steps
        ss.str(data.substr(data.find(":") + 1));
        ss >> simulProperties[1];
        cout << "Time Limit: " << ss.str() << endl;

        getline(configFile, data);                              //Get Refresh rate
        ss.clear();
        ss.str(data.substr(data.find(":") + 1));
        ss >> simulProperties[2];
        cout << "Refresh Rate: " << ss.str() << endl << endl;

    }
    else{               //Failed file open ends the program

        configFile.close();
        cout << "Unable to open configuration file. \n";
        return 0;

    }
    configFile.close();


    /* Region File Reading (Author: Kevin, Divyam, and Suraj) */
    fstream regionFile(buffer);
    if (regionFile){    //Check if region file opens

        int yCount = 0;
        while (getline(regionFile, single_line)){       //Populate region map

            vector<Region*> row;
            stringstream str (single_line);
            string row_text;
            int xCount = 0;

            while (getline(str, row_text, ',')){                //Translate the character in the region file to a region object
                                                                        //xCount and yCount are used to track x and y coordinates for storage (analyzing)
                switch(row_text[0]){
                    case 'R':
                        row.push_back(new Residential(xCount, yCount));
                        break;
                    case 'I':
                        row.push_back(new Industrial(xCount, yCount));
                        break;
                    case 'C':
                        row.push_back(new Commercial(xCount, yCount));
                        break;
                    case 'P':
                        row.push_back(new Powerplant(xCount, yCount));
                        break;
                    case 'T':
                        row.push_back(new Powerline(xCount, yCount));
                        break;
                    case '-':
                        row.push_back(new Road(xCount, yCount));
                        break;
                    case '#':
                        row.push_back(new PowerlineRoad(xCount, yCount));
                        break;
                    default:
                        row.push_back(new Region(xCount, yCount));
                        break;
                }
                xCount++;
            } 
            regionMap.push_back(row);
            yCount++;

        }

    }
    else{                   //Failed file open ends the program

        regionFile.close();
        cout << "Error reading the region file." << endl;   
        return 0;

    }
    regionFile.close();


    /* Simulation Begin (Author: Kevin) */
    char menuDecision;
    DisplayMenu(simulProperties, regionMap, availableWorkers, availableGoods);
    while(cin >> menuDecision && menuDecision != 'q') {                                 //Constant loop of character input until quit

        while(!ExecuteMenu(menuDecision, simulProperties, regionMap, availableWorkers, availableGoods)){                            //Read Documentation on ExecuteMenu() for details
            cin >> menuDecision;                                                                //Constant loop of valid character input until valid, otherwise output the menu again
        }
        cout << endl << endl;
        
        DisplayMenu(simulProperties, regionMap, availableWorkers, availableGoods);

    }
    cout << "Quitting Program...\n";

    return 0;
}


/* Function Definitions */
/*
* DisplayMenu() will output entire region map and action menu, prompting the user to enter a character
* As well as that, all properties pertaining to the whole of the region map will be displayed
* Author: Kevin
*
* TODO: Clear the console when finished with character input processing for cleaner display, then output menu
*/
void DisplayMenu(int simulationProperties[3], vector<vector<Region*>> regionMap, vector<Person*> availableWorkers, vector<Good*> availableGoods){

    int totalPopul = 0;
    int totalPollu = 0;

    if(simulationProperties[0] >= simulationProperties[1]){ cout << "---------------FINAL STATE---------------" << endl; }
    cout << "---------------------REGION MAP---------------------\n";
    for(vector<Region*> row : regionMap){
        for(Region* region : row){
            cout << region->GetRegionLetter() << " ";
            totalPopul += region->GetPopulation();
            totalPollu += region->GetPollution();
        }
        cout << endl;
    }

    cout << left;
    cout << "-----------------------------------\n";  
    cout << "| Total Population = " << setw(13) << totalPopul << "|\n";
    cout << "| Total Pollution = " << setw(14) << totalPollu << "|\n";
    cout << "| Current Step = " << setw(8) << simulationProperties[0] << "/" << setw(8) << simulationProperties[1] << "|\n";
    cout << "| Refresh Rate = " << setw(17) << simulationProperties[2] << "|\n";
    cout << "| Available Workers = " << setw(12) << availableWorkers.size() << "|\n";
    cout << "| Available Goods = " << setw(14) << availableGoods.size() << "|\n";
    cout << "-----------------------------------\n";
    cout << "| f - Time Step Forward           |\n";
    cout << "| t - Step X Amount of Times      |\n";
    cout << "| a - Analyze Region (x,y coords) |\n";
    cout << "| r - Change Refresh Rate         |\n";
    cout << "| m - Change Maximum Simul Steps  |\n";
    cout << "| q - Quit                        |\n";
    cout << "-----------------------------------\n";
    if(simulationProperties[0] >= simulationProperties[1]){ cout << "---------------FINAL STATE---------------" << endl; }
    cout << "(Enter the character of the action you would like to take): \n";

}
/*
* ExecuteMenu() requires the user input and pass by reference of the entire region map, taking actions to change the map as the user has chosen.
*   return true if user input is successfully processed, false if otherwise
* Author: Kevin
*
*/
bool ExecuteMenu(char menuOption, int (&simulationProperties)[3], vector<vector<Region*>> &regionMap, vector<Person*> &availableWorkers, vector<Good*> &availableGoods){

    /* Decision Tree */
    switch(menuOption){

            /* Time Step forward in the simulation */
        case 'f':
        {

            if(simulationProperties[0] >= simulationProperties[1]){                     //Catch end of simulation; Disable when true
                cout << "(End of Simulation Detected; Function Disabled. Enter a different character):" << endl;
                return false;
            }

            for(int iter = 0; iter < simulationProperties[2]; iter++){                  //Call UpdateSimulation to process a step, accounting for refresh rate
                UpdateSimulation(regionMap, availableWorkers, availableGoods);
                simulationProperties[0]++;
            }
            return true;
            break;
        }
            /* Time Step forward X amount of times (Requires User Input) */
        case 't':
        {        

            if(simulationProperties[0] >= simulationProperties[1]){                     //Catch end of simulation; Disable when true
                cout << "(End of Simulation Detected; Function Disabled. Enter a different character):" << endl;
                return false;
            }

            cout << "(How many steps forward? Enter a number):" << endl;                //Prompt
            string userIn;
            stringstream ss;
            int steps;
            cin.ignore();

            while(getline(cin, userIn)){                                                    //Collect valid number of steps, reprompt if invalid
                ss.clear();
                ss.str(userIn);
                if(ss >> steps && steps > -1){ break; }  

                cout << "(Invalid input. Only values above 0. Retry):" << endl;
            }

            cout << "Iterating " << steps << " times" << endl;
            for(int i = 0; i < steps * simulationProperties[2]; i++){                       //Iterate X amount of times * refresh rate
                UpdateSimulation(regionMap, availableWorkers, availableGoods);
                simulationProperties[0]++;
            }
            return true;
            break;
        }
            /* Analyze a specific region given an X and Y coordinate (Requires User Input) */
        case 'a':
        {

            cout << "(Maximum X coordinate: " << regionMap.size() - 1 << ", Maximum Y coordinate: " << regionMap[0].size() - 1 << ")" << endl;
            cout << "(Enter the x coordinate of the region you wish to analyze, where (0, 0) is the top left region.):" << endl;
            string userIn;
            int xCoord;
            stringstream ss;                                                            //Use string stream for parsing integers from input, allowing for reprompting when invalid 
            cin.ignore();

            while(getline(cin, userIn)){
                ss.clear();
                ss.str(userIn);
                if(ss >> xCoord && xCoord > -1 && xCoord < regionMap.size()){ break; }          //Make sure valid coordinate values

                cout << "(Invalid input. Only values between 0 and " << regionMap.size() - 1 << " are allowed. Retry):" << endl;
            }

            cout << "(Enter the y coordinate of the region you wish to analyze):" << endl;
            int yCoord;

            while(getline(cin, userIn)){
                ss.clear();
                ss.str(userIn);
                if(ss >> yCoord && yCoord > -1 && yCoord < regionMap.size()){ break; }

                cout << "(Invalid input. Only values between 0 and " << regionMap[0].size() - 1 << " are allowed. Retry):" << endl;
            }

            regionMap.at(yCoord).at(xCoord)->toString();

            return true;
            break;
        }
            /* Change Refresh Rate */
        case 'r':
        {

            cout << "Not Implemented" << endl;
            return true;
            break;
        }
            /* Change Maximum Steps */
        case 'm':
        {

            cout << "Not Implemented" << endl;
            return true;
            break;
        }
            /* Invalid Input */
        default:
            cout << "Invalid character input\n";
            break;

    }
    return false;
    
}
/*
* UpdateSimulation() will iterate through the entire region map, calling CheckUpdate() on each region and collecting
* a list of pointers to regions where CheckUpdate() results true. All regions in this list will grow by one point as needed
*
* A list of pointers to Person and Good structs is sent through to CheckUpdate() to provide available workers and goods for Industrial and Commercial growth conditions, which are stored outside the region map
*
* Pollution requires adjacency collection beyond immediate vicinity at industrial population 3, therefore, CheckUpdate(@adjacencies) may not be the best solution for handling such functionality. Therefore, using the
* x and y coordinates of Industrial districts, the pollution will be handled after the full region map CheckUpdate() iteration. Logically speaking, the growth of an industrial zone triggers a change in pollution,
* so it is appropriate to trigger it during the update list iteration for better runtime efficiency. 
*
* Author: Kevin
*/
void UpdateSimulation(vector<vector<Region*>>& regionMap, vector<Person*> &availableWorkers, vector<Good*> &availableGoods){

    vector<Region*> updateList;

        /* Full Region Map Iteration */
    for(int row = 0; row < regionMap.size(); row++){
        for(int col = 0; col < regionMap[row].size(); col++){

            if(regionMap[row][col]->GetRegionLetter() == " ") { continue; }                 //Skip empty regions

            vector<Region*> adjacent;

            for(int rowDeviate = -1; rowDeviate <= 1; rowDeviate++){                //Adjacencies to Current Region iteration for collection into CheckUpdate()
                for(int colDeviate = -1; colDeviate <= 1; colDeviate++){

                    if(rowDeviate == 0 && colDeviate == 0){ continue; }                     //Do not collect adjacency with itself

                    try{                                                                    //Add to adjacency list (while avoiding out of bounds errors)
                        adjacent.push_back(regionMap.at(row + rowDeviate).at(col + colDeviate));
                    }catch(out_of_range e){
                        //Out of bounds catching during adjacency collection
                    }
                }
            }

            if(regionMap[row][col]->CheckUpdate(adjacent, availableWorkers, availableGoods)){                   //After checking the list, if growth conditions are satisfied
                updateList.push_back(regionMap[row][col]);                                                              //Store its pointer for later updating
            }

        }
    }

    for(Region* update : updateList){                           //Finally, grow the eligible regions + Pollution functionality

        update->SetPopulation(update->GetPopulation() + 1);


        if(dynamic_cast<Industrial*>(update) != nullptr){                   //Pollution handling (as pollution is only affected by the growth of an industrial zone)

            vector<Region*> pollutionInfluence;                                     //Similar strategy as before; collect affected adjacencies then update them, however, there is no need for checking valid regions through CheckUpdate()

            int influenceRange = update->GetPopulation() - 1;                       //Depending on the population, we can use the population number to determine the size of the square of pollution influence (i.e., population of 1 gives a 1x1 square, 2 gives a 3x3 square, 3 gives a 5x5 square)
                                                                                            //Basically if population was represented as n, the dimensions would be (2n-1)x(2n-1) except for when n=1
            int rowDeviate = -1 * influenceRange;                                               //For a visual representation, imagine squares of larger and larger dimensions stacked on top of one another.
            int colDeviate = -1 * influenceRange;                                               //As the industrial population grows, we add another square of relevant size

            do{                                                                     //In order to account the range, we use a loop nested in a loop both bounded by the deviation range, using the deviation values to collect adjacencies with (x,y) coordinates
                                                                                            //Using a for loop, however, does not account for when population, or n, equals 1, so we will use a do{}while() to account for it.
                do{
                    try{
                        pollutionInfluence.push_back(regionMap.at(update->GetYCoord() + rowDeviate).at(update->GetXCoord() + colDeviate));
                    }catch(out_of_range e){
                        //Out of bounds catching during collection
                    }

                    colDeviate++;
                }while(colDeviate <= influenceRange);

                colDeviate = -1 * influenceRange;
                rowDeviate++;
            }while(rowDeviate <= influenceRange);

            for(Region* polluted : pollutionInfluence){                             //Increase pollution of affected region within the square of influence by 1
                polluted->SetPollution(polluted->GetPollution() + 1);
            }
            
        }

    }

}