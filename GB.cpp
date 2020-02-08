// Tymoteusz Chatys 2018
// All the simulations are described in detail in the PDF attached.

#include <iostream>
#include <fstream>
#include <vector> // Includes other source file into current source file
#include <iomanip>
#include <string>
#include <windows.h>
#include <sstream>
#include <cmath>
#include <tuple>
#include <algorithm>

using namespace std; //enables us to remove std:: from standard functions
//Shorter Definitions
typedef vector < string > is;
typedef vector < double > id;
typedef vector < vector < string > > iis;
typedef vector < vector < double > > iid;
typedef tuple <double, double, double, double, string, string, id, id> tup;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//Colours
#define GREEN_F		(SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN))  //sets the console colour to green
#define RED_F		(SetConsoleTextAttribute(hConsole, FOREGROUND_RED))  //sets the console colour to red
#define BLUE_F		(SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE)) //sets the console colour to blue
#define WHITE_F		(SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)) //defines the colour white
#define pow2(x)		((x)*(x))
//Global variables
const double PI = 2 * acos(0.0); //pi
const int R = 6378137; //according to WGS-84
int SIZE_ROW;
int NOT = 0;
int CUS = 0;
string PROGRAMEND; //variable used to end the program
iis DATA;
id LATITUDE, LONGITUDE, POPULATION;
//Function definitions
tup simulation3p1(iis x, id lat, id lon, double step, int locations);
id convertToDouble(iis x, int j);
id populationtoDouble(iis x);
iis readFile();
int checkforint();
int simulation1p1(iis x, id lat, id lon);
int simulation1p2(iis x, id lat, id lon);
int simulation2p1(iis x, id lat, id lon);
int simulation2p2(iis x, id lat, id lon);
int simulation3ps();
tuple <double, id, id> simulation3p2(iis x, id lat, id lon, double best);
double dCalculation(double lat2, double lat1, double lon2, double lon1);
double randomCoords(int x);

int main() {
	int answer,answer2;
	//seed for the random number generator
	srand(time(NULL));
	GREEN_F;
	printf("!--GB PLACES AMAZINGNATOR--!\n\n\n"); //Welcome message
	DATA = readFile(); //reads the file and extracts information to DATA
	LATITUDE = convertToDouble(DATA, 3); //extracts latitudes from DATA
	LONGITUDE = convertToDouble(DATA, 4); //extracts longitudes from DATA
	POPULATION = populationtoDouble(DATA); //extracts population from DATA
	SIZE_ROW = DATA.size(); //sets the global constant
	do {
		cout << "If you want to see the log of the program enter '1'\n";
		cin >> answer;
	} while (checkforint() == 0); //checks if input given by user is a number
	if (answer == 1) NOT = 1; //if 1 is inputted, notifications will be displayed
	do {
		cout << "\nIf you want to use custom settings enter '1'\n";
		cin >> answer2; 
	} while (checkforint() == 0);
	if (answer2 == 1) CUS = 1; //if 1 is inputted, user will be asked if he wants to change some settings.
	//Calls the functions of the various simulations
	simulation1p1(DATA, LATITUDE, LONGITUDE);
	simulation1p2(DATA, LATITUDE, LONGITUDE);
	simulation2p1(DATA, LATITUDE, LONGITUDE);
	simulation2p2(DATA, LATITUDE, LONGITUDE);
	simulation3ps();
	cout << "\n\n Finished Simulations \n";
	cin >> PROGRAMEND; //awaits user input to end the program
	exit(1);
	return 0;
}

iis readFile() {
	iis data; //defines a 2D vector 

	ifstream file("GBPlaces.csv"); //reads a file

	if (file.is_open()) {
		string aLine;
		getline(file, aLine);
		while (!file.eof()) {
			//reads a line by line
			//hold the read in line
			getline(file, aLine); //reads line from file into aLine
			if (aLine.length() > 0) {
				int comma_pos;
				int a = 1;
				double number;
				string numberstr;
				is temp;

				while (a == 1) {
					if (aLine.find(',', 0) != -1) { //if it finds that the next comma position is larger than the previous comma
						comma_pos = aLine.find(',', 0); // finds where comma starting from the position of the last comma
						numberstr = aLine.substr(0, comma_pos); //extracts the value between two commas
						number = atof(numberstr.c_str()); // converts from a string
						temp.push_back(numberstr); // added the value to the temporary vector
						WHITE_F;
						//cout << numberstr; //prints out the number to the user.
						aLine = aLine.substr(comma_pos + 1, aLine.length() - comma_pos);
						GREEN_F;
						//printf(",");
					}
					else {
						numberstr = aLine;
						number = atof(numberstr.c_str());
						temp.push_back(numberstr);
						WHITE_F;
						//cout << numberstr;
						a = 0; //else set a=0 which tells the program it has reached the end of the line and to end the while loop
					}

				}
				//printf("\n");

				data.push_back(temp); //pushes back the row into the data 2D vector
			}
		}


		file.close();
		//If file is open successfully and the program successfully created the vectors. The user is notified that his data will be loaded
	}
	else {
		cout << "Unable to open the file! \n"; //notified that the program is unable to open
		//make program stay open unless something is typed
		printf("");
		cin >> PROGRAMEND; //random variable input, program will stop after this has been inputed
		exit(1);
	}
	//Checks the number of row data points
	return data;
}

id populationtoDouble(iis x) {
	//converts the population data from string to a double
	id vector;
	double size_row = x.size();
	double size_col = x[0].size();
	double number;
	string numberstr;

	//loop to extract the population data and insert to a new vector
	for (size_t i = 0; i < size_row; i++) {
		numberstr = (x[i][2]);
		number = atof(numberstr.c_str());
		vector.push_back(number);
	}
	return vector;
}

id convertToDouble(iis x, int j) {
	//converts the latitude and longitdue data from degrees string to a double in radians
	id vector;
	double size_row = x.size();   //Checks the number of row data points
	double size_col = x[0].size(); //Checks the number of column data points
	double number;
	double numberd;
	string numberstr;

	//loop to extract the latitude and longitude data and insert to a new vector in radians
	for (size_t i = 0; i < size_row; i++) {
		numberstr = (x[i][j]);
		number = atof(numberstr.c_str());
		numberd = number * PI / 180;
		vector.push_back(numberd);
	}
	return vector;
}

double dCalculation(double lat2, double lat1, double lon2, double lon1) {
	// calculates the distance between two positions using the haversine formula.
	double b,c,d,dlat,dlon;
	dlat = lat2 - lat1; //difference in latitudes
	dlon = lon2 - lon1; //difference in longitudes
	b = (pow2(sin(dlat / 2)) + (cos(lat2))*(cos(lat1))*pow2(sin(dlon / 2)));
	c = 2 * atan2(sqrt(b), sqrt(1 - b));
	d = R * c;
	return d; //returns the distance in meters.
}

double randomCoords(int x) {
	int n = 999; // ticks
	n = rand() % n; // returns a random integer between 0 and (n – 1)
	int upper_limit;
	int lower_limit;
	if (x == 3) { //if x =3, we know to return a latitude
		upper_limit = 57;
		lower_limit = 50;
		//latitude
	}
	else if (x == 4) { //if x =4, we know to return a longitude
		upper_limit = 2;
		lower_limit = -4;
		//longitude
	}
	double r = lower_limit + (upper_limit - lower_limit)*(rand() % (n + 1) * (1. / n));
	return r;
}

int simulation1p1(iis x, id lat, id lon) {
	//First simulation, brute forced. Hub is at a city
	cout << "\n\n---Simulation 1.1---" << "\n\n";
	string place, place2, bestlat, bestlon;
	int a;
	int counter = 0;
	int counter2 = 0;
	double sumd, tlat, tlon;
	double sumbest = 10000000000000000000; //set as a large number
	// brute forces to find the city that has the lowest distance to the other cities
	for (size_t i = 0; i < 100; i++) {
		sumd = 0;
		for (size_t j = 0; j < 100; j++) {
			double distance = dCalculation(lat[j], lat[i], lon[j], lon[i]);
			sumd = sumd + 2 * fabs(distance);
			counter++;
		}
		if (sumbest > sumd) { //if currently calculated distance is lower than the shortest
			bestlat = x[i][3]; //saves best lat
			bestlon = x[i][4]; //saves best lon
			counter2++;
			sumbest = sumd;
			place = x[i][1];
			place2 = x[i][0];
		}
	}
	if (CUS == 1 || NOT == 1) { //if custom settings or notifications were turned on.
		GREEN_F;
		cout << "This simulation is brute forced, no notifications or custom changes can be made. \n";
		WHITE_F;
	}
	// displays final solution to the user.
	cout << counter << " calculations and " << counter2 - 1 << " improvements\n";
	cout << "The optimal route involves placing the hub in " << place << " " << place2 << " [" << bestlat << "," << bestlon << "] " << ". It would take the truck " << sumbest / 1000 << "km \n";
	return 0;
}
int simulation1p2(iis x, id lat, id lon) {
	cout << "\n\n---Simulation 1.2---" << "\n\n";
	string place, place2;
	int a;
	int counter = 0;
	int counter2 = 0;
	string bestlat, bestlon;
	double sumd, tlat, tlon;
	double sumbest = 10000000000000000000; //set as a large number
	for (size_t i = 0; i < 100; i++) {
		sumd = 0;
		for (size_t j = 0; j < 100; j++) {
			double distance = dCalculation(lat[j], lat[i], lon[j], lon[i]); //cal
			double totdistance = distance * POPULATION[j]; //weighted by population
			sumd = sumd + 2 * fabs(totdistance);
			counter++;
		}
		if (sumbest > sumd) { //if currently calculated distance is lower than the shortest
			bestlat = x[i][3]; //saves best lat
			bestlon = x[i][4]; //saves best lon
			counter2++;
			sumbest = sumd;
			place = x[i][1];
			place2 = x[i][0];
		}
	}
	if (CUS == 1 || NOT == 1) { //if custom settings or notifications were turned on.
		GREEN_F;
		cout << "This simulation is brute forced, no notifications or custom changes can be made. \n";
		WHITE_F;
	}
	// displays final solution to the user
	cout << counter << " calculations and " << counter2 - 1 << " improvements\n";
	cout << "The optimal route involves placing the hub in " << place << " " << place2 << " [" << bestlat << "," << bestlon << "]" << "\n";
	return 0;
}
int simulation2p1(iis x, id lat, id lon) {
	cout << "\n---Simulation 2.1---" << "\n\n";
	int a;
	int dx = 0;
	int dy = 0;
	int counter = 0;
	int counter2 = 0;
	int locations = 5000; // number of starting random points
	double sumd, bestlat, bestlon;
	double sumbest = 10000000000000000000;
	double step = 0.000001; //step size for hill climbing

	//If custom settings were enabled by the user
	if (CUS == 1) {
		do {
			cout << "Step (default =" << step << ") : ";
			cin >> step; //changes the step of the hill climb
		} while (checkforint() == 0);
		do {
			cout << "Initial random locations (default =" << locations << ") : ";
			cin >> locations; //changes the number of initial random locations
		} while (checkforint() == 0);
	}

	
	for (size_t i = 0; i < locations; i++) { // gets 'locations' random positions and picks the best one
		double hublat = randomCoords(3) * (PI / 180); //retrieves random lat coordinate
		double hublon = randomCoords(4) * (PI / 180); //retrieves random lon coordinate
		sumd = 0;
		// caluclates the distance to all the cities from the point
		for (size_t i = 0; i < 100; i++) {
			a = i;
			double distance = dCalculation(hublat, lat[a], hublon, lon[a]);
			sumd = sumd + 2 * fabs(distance);
			counter++;
		}
		// if the point is better than the best point, it sets it as the new best point
		if (sumbest > sumd) {
			counter2++;
			sumbest = sumd;
			bestlat = hublat;
			bestlon = hublon;
			if (NOT == 1) {
				//if notifications are turned on, it notifies the user a new optimal route was found
				GREEN_F;
				cout << "New optimal route found, place the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << ". It would take the truck " << sumbest / 1000 << "km" << endl;
				WHITE_F;
			}
		}

	}
	double oldbest = sumbest;
	double maxValue;
	// main loop to continually 'improve' the lat and lon
	do {

		// save the current value
		oldbest = sumbest;
		maxValue = oldbest; // set the maxValue for the local search to be the current value

		// now look around the current point to see if there's a better one nearby
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				sumd = 0;
				// this gives 9 points including the current point (when i=0, j=0)
				if (i == 0 && j == 0) {
				}
				else {
					for (size_t a = 0; a < 100; a++) {
						double distance = dCalculation(bestlat + step * i, lat[a], bestlon + step * j, lon[a]);
						sumd = sumd + 2 * fabs(distance);
						counter++;
					}
					if (sumd <= maxValue) { // is it smaller than maxValue?
						// yes so set maxValue and save point i,j values

						dx = i;
						dy = j;
						maxValue = sumd;
					}
				}
			}
		}

		// update bestlat and bestlon to new point with higher 'value'
		bestlat += step * dx;
		bestlon += step * dy;
		sumbest = maxValue;
		counter2++;
		if (NOT == 1) {
			GREEN_F;
			cout << "New optimal route found, place the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << ". It would take the truck " << sumbest / 1000 << "km" << endl;
			WHITE_F;
		}
	} while (sumbest < oldbest); // repeat all this while we can find a greater value than the previous one
	double bestdis = 10000000;
	string place;
	string place2;
	for (size_t a = 0; a < 100; a++) {
		double lowdis = dCalculation(bestlat, lat[a], bestlon, lon[a]);
		if (lowdis < bestdis) {
			bestdis = lowdis;
			place = x[a][1];
			place2 = x[a][0];
		}
	}

	//Shows the solution to the user
	cout << counter << " calculations and " << counter2 - 1 << " improvements\n";
	cout << "The optimal route involves placing the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << ". It would take the truck " << sumbest / 1000 << "km \n";
	cout << "The closest city/town is the " << place << " of " << place2 << ". It is " << bestdis / 1000 << "km away" << endl;

	return 0;
}
int simulation2p2(iis x, id lat, id lon) {
	cout << "\n---Simulation 2.2---" << "\n\n";
	int a;
	int dx = 0;
	int dy = 0;
	int counter = 0;
	int counter2 = 0;
	int locations = 5000; //locations for initial starting points
	double sumd, bestlat, bestlon;
	double sumbest = 10000000000000000000;
	double step = 0.000001; // step size for the hill climbing

	if (CUS == 1) { //same as earlier
		do {
			cout << "Step (default =" << step << ") : ";
			cin >> step;
		} while (checkforint() == 0);
		do {
			cout << "Initial random locations (default =" << locations << ") : ";
			cin >> locations;
		} while (checkforint() == 0);
	}

	for (size_t i = 0; i < locations; i++) {
		double hublat = randomCoords(3) * (PI / 180);
		double hublon = randomCoords(4) * (PI / 180);
		sumd = 0;
		for (size_t i = 0; i < 100; i++) {
			a = i;
			double distance = dCalculation(hublat, lat[a], hublon, lon[a]);
			double totdistance = distance * POPULATION[a]; //weighted by the population
			sumd = sumd + 2 * fabs(totdistance);
			counter++;
		}
		if (sumbest > sumd) {
			counter2++;
			sumbest = sumd;
			bestlat = hublat;
			bestlon = hublon;
			if (NOT == 1) {
				GREEN_F;
				cout << "New optimal route found, place the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << ". It would take the truck " << sumbest / 1000 << "km" << endl;
				WHITE_F;
			}
		}

	}
	double oldbest = sumbest;
	double maxValue;
	// main loop to continually 'improve' x, y
	do {

		// save the current value
		oldbest = sumbest;
		maxValue = oldbest; // set the maxValue for the local search to be the current value

		// now look around the current point to see if there's a better one nearby
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				sumd = 0;
				// this gives 9 points including the current point (when i=0, j=0)
				if (i == 0 && j == 0) {
				}
				else {
					for (size_t a = 0; a < 100; a++) {
						double distance = dCalculation(bestlat + step * i, lat[a], bestlon + step * j, lon[a]);
						double totdistance = distance * POPULATION[a];
						sumd = sumd + 2 * fabs(totdistance);
						counter++;
					}
					if (sumd <= maxValue) { // is it smaller than maxValue?
						// yes so set maxValue and save point i,j values

						dx = i;
						dy = j;
						maxValue = sumd;
					}
				}
			}
		}

		// update bestlat and bestlon to new point with higher 'value'
		bestlat += step * dx;
		bestlon += step * dy;
		sumbest = maxValue;
		counter2++;
		if (NOT == 1) {
			GREEN_F;
			cout << "New optimal route found, place the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << endl;
			WHITE_F;
		}
	} while (sumbest < oldbest); // repeat all this while we can find a lower value than the previous one
	double bestdis = 10000000;
	string place;
	string place2;
	//checks the total distance
	for (size_t a = 0; a < 100; a++) {
		double lowdis = dCalculation(bestlat, lat[a], bestlon, lon[a]);
		if (lowdis < bestdis) {
			bestdis = lowdis;
			place = x[a][1];
			place2 = x[a][0];
		}
	}
	// output to the user
	cout << counter << " calculations and " << counter2 - 1 << " improvements\n";
	cout << "The optimal route involves placing the hub at " << bestlat * 180 / PI << "," << bestlon * 180 / PI << "km \n";
	cout << "The closest city/town is the " << place << " of " << place2 << ". It is " << bestdis / 1000 << "km away" << endl;
	return 0;
}
int simulation3ps() {
	int maxima = 5;
	double gbest, hublat, hublon, maxValue, bestdis, maxValue2, bestdis2, bestlat, bestlon;
	double bestValue = 100000000000;
	double step = 0.0005;
	double locations = 25;
	string place, place2, bestplace, bestplace2;
	id sollon, sollat, bestlonv, bestlatv, bestlonr, bestlatr, bestlats, bestlons;
	cout << "\n---Simulation 3.1---" << "\n\n";
	//set custom settings
	if (CUS == 1) {
		do {
			cout << "Step (default =" << step << ") : ";
			cin >> step;
		} while (checkforint() == 0);
		do {
			cout << "Initial random locations (default =" << locations << ") : ";
			cin >> locations;
		} while (checkforint() == 0);
		do {
			cout << "Number of Local Maximas to search for (default =" << maxima << ") : ";
			cin >> maxima;
		} while (checkforint() == 0);
	}
	//calls simulation 3 part 1, then if it retrieves a lower value, it sets that solution parameters to current bestvalue
	for (size_t i = 0; i < maxima; i++) {
		tie(hublat, hublon, maxValue, bestdis, place, place2, sollat, sollon) = simulation3p1(DATA, LATITUDE, LONGITUDE, step, locations);
		if (maxValue < bestValue) {
			bestValue = maxValue;
			bestlat = hublat;
			bestlon = hublon;
			bestdis2 = bestdis;
			bestplace = place;
			bestplace2 = place2;
			bestlonv = sollon;
			bestlatv = sollat;
		}
	}
	//output to the user
	cout << "Global Maxima Found: " << bestlat * (180 / PI) << "," << bestlon * (180 / PI) << ". " << "It takes " << bestValue / 1000;
	cout << " km. First stop is " << bestplace << " " << bestplace2 << ". It is " << bestdis2 / 1000 << "km away." << endl;

	//saves the best route of Nearest Neighbour to a txt.
	ofstream outfile;
	outfile.open("NNBest.txt", ios::out); 	// output the data points to the file
	// Outputs the data to a file 
	for (size_t j = 0; j < bestlatv.size(); j++) {
		outfile << bestlatv[j] * (180 / PI) << "," << bestlonv[j] * (180 / PI) << endl;
	}
	//calls the simulation 3 part 2 function
	cout << "\n---Simulation 3.2---" << "\n\n";
	tie(gbest, bestlats, bestlons) = simulation3p2(DATA, bestlatv, bestlonv, bestValue);
	return 0;
}
tup simulation3p1(iis x, id lat, id lon, double step, int locations) {
	//simulation 3 part 1
	id sollat,sollon;
	int a;
	int remove = 100000000;
	int counter = 0;
	int counter2 = 0;
	string place,place2;
	double bestdis = 100000000;
	double dx = 0;
	double dy = 0;
	double best = 10000000000000000000;
	double sumbest = 10000000000000000000;
	double lowestdis = 10000000000000000000;
	double bestlat, newlon, newlat, bestlon, distance , hublatqm , hublonqm, sumd, maxValue;
	for (size_t i = 0; i < locations; i++) {
		iis x = DATA;
		id lat = LATITUDE;
		id lon = LONGITUDE;
		double hublat = randomCoords(3) * (PI / 180);
		double hublon = randomCoords(4) * (PI / 180);
		double firstlat = hublat;
		double firstlon = hublon;
		double sumd = 0;
		string firstplace1;
		string firstplace2;
		string lastplace1;
		string lastplace2;
		sollat.clear();
		sollon.clear();
		sollat.push_back(hublat);
		sollon.push_back(hublon);
		// performs the nearest neigbour algorithm for a random point
		for (int iteration = 0; iteration < 100; iteration++) {
			lowestdis = 10000000000;
			for (size_t j = 0; j < lat.size(); j++) {
				//looks at every city/town from current location and looks at which one is the closest
				distance = dCalculation(hublat, lat[j], hublon, lon[j]);
				counter++;
				if (lowestdis > distance) {
					//the closest one will end up being saved here
					lowestdis = distance;
					newlon = lon[j];
					remove = j;
					newlat = lat[j];
				}

			}
			//best location is pushed back to the solution to be later outputted in a file
			sollon.push_back(newlon);
			sollat.push_back(newlat);
			//added to total travel time
			sumd = sumd + lowestdis;

			
			hublat = newlat;
			hublon = newlon;
			id lat2;
			id lon2;
			//new vectors are created omitting the best location which will now be removed
			for (size_t i = 0; i < lat.size(); i++) {
				if (i != remove) {
					counter++;
					double templat = lat[i];
					double templon = lon[i];
					lat2.push_back(templat);
					lon2.push_back(templon);
				}
			}
			lat = lat2;
			lon = lon2;
			//now that it is removed the program will look for the next nearest city/town to that city/town
		}

		sollat.push_back(hublat);
		sollon.push_back(hublon);
		//pushes the hub location back to the solution
		double goback = dCalculation(hublat, firstlat, hublon, firstlon);
		sumd = sumd + goback;
		if (best > sumd) {
			counter2++;

			id lat = LATITUDE;
			id lon = LONGITUDE;
			bestdis = 100000000;
			for (size_t a = 0; a < 100; a++) {
				counter++;
				double lowdis = dCalculation(firstlat, lat[a], firstlon, lon[a]);
				if (lowdis < bestdis) {
					bestdis = lowdis;
					place = x[a][1];
					place2 = x[a][0];
					//checks which city/town is the closest to the final solution
				}
			}
			best = sumd;
			maxValue = sumd;
			hublatqm = firstlat;
			hublonqm = firstlon;
		}

	}
	double hublat = hublatqm;
	double hublon = hublonqm;
	best = 1000000000;
	//start of the hill climbing process for nearest neigbour
	//the process is the same. It slightly moves the point around the current best location
	//to find a better position for the hub
	do {
		double firstlat = hublat;
		double firstlon = hublon;
		best = maxValue;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				sumd = 0;
				if (i == 0 && j == 0) {
				}
				//checks every single combination.
				//and performs the same algorithm for nearest neighbour but with the adjusted starting location
				else {
					hublat = firstlat + (step * i);
					hublon = firstlon + (step * j);
					double changelat = hublat;
					double changelon = hublon;
					id lat = LATITUDE;
					id lon = LONGITUDE;
					sollat.clear();
					sollon.clear();
					sollat.push_back(hublat);
					sollon.push_back(hublon);
					for (int iteration = 0; iteration < 100; iteration++) {
						lowestdis = 10000000000;
						for (size_t f = 0; f < lat.size(); f++) {
							counter++;
							distance = dCalculation(changelat, lat[f], changelon, lon[f]);
							if (lowestdis > distance) {
								lowestdis = distance;
								newlon = lon[f];
								remove = f;
								newlat = lat[f];
							}
						}
						sollon.push_back(newlon);
						sollat.push_back(newlat);
						sumd = sumd + lowestdis;
						changelat = newlat;
						changelon = newlon;
						id lat2;
						id lon2;
						for (size_t g = 0; g < lat.size(); g++) {
							if (g != remove) {
								double templat = lat[g];
								double templon = lon[g];
								lat2.push_back(templat);
								lon2.push_back(templon);
							}
						}
						lat = lat2;
						lon = lon2;
					}
					sollat.push_back(hublat);
					sollon.push_back(hublon);



					double goback = dCalculation(changelat, firstlat + (step * i), changelon, firstlon + (step * j));
					sumd = sumd + goback;
					if (sumd <= maxValue) { // is it smaller than maxValue?
						// yes so set maxValue and save point i,j values
						// saves the increment
						counter2++;
						dx = i;
						dy = j;
						maxValue = sumd;
						counter++;
					}
				}

			}

		}
		hublat = firstlat + step * dx; //saves the new best starting latitude
		hublon = firstlon + step * dy; //saves the new best starting longitude
		if (best > maxValue) {
			id lat = LATITUDE;
			id lon = LONGITUDE;
			bestdis = 1000000000;
			for (size_t a = 0; a < 100; a++) {
				counter++;
				double lowdis = dCalculation(hublat, lat[a], hublon, lon[a]);
				//checks which city is closest to the hub and saves it
				if (lowdis < bestdis) {
					bestdis = lowdis;
					place = x[a][1];
					place2 = x[a][0];
				}
			}
		}
		firstlat = hublat;
		firstlon = hublon;
	} while (best > maxValue);
	if (NOT == 1) {
		GREEN_F;
		//output to the user the notifications
		cout << counter << " calculations and " << counter2 - 1 << " improvements\n";
		cout << "Local Maxima Found: " << hublat * (180 / PI) << "," << hublon * (180 / PI) << ". " << "It takes " << maxValue / 1000;
		cout << " km. First stop is " << place << " " << place2 << ". It is " << bestdis / 1000 << "km away." << endl;
		WHITE_F;
	}
	tup var;
	//required to return different types of variables
	var = make_tuple(hublat, hublon, maxValue, bestdis, place, place2, sollat, sollon);
	return var;
}

tuple <double, id, id> simulation3p2(iis x, id lat, id lon, double best) {
	int a = 0; //counter
	int trees = 100;
	id solutions;
	id lats;
	id lons;
	id bestlats;
	id bestlons;
	double bests;
	double gbest = 1000000000;
	int counter;
	if (CUS == 1) {
		do {
			cout << "Trees to search for (default =" << trees << ") : ";
			cin >> trees;
		} while (checkforint() == 0);
	}
	for (size_t count = 0; count < trees; count++) {
		a = 0;
		lats = lat;
		lons = lon;
		bests = best;
		counter = 0;
		do {
			int i = 98;
			int k = 20;
			double sum = 0;
			i = rand() % i; // returns a random integer between 0 and (n – 1)
			k = (rand() % k) + 1;
			if ((i + k) < 101) { //if i+k is smaller than 101, (total number of datapoints)
				iter_swap(lats.begin() + i + 1, lats.begin() + i + k + 1); //swap the two random points
				iter_swap(lons.begin() + i + 1, lons.begin() + i + k + 1);
				for (size_t j = 0; j < 101; j++) {
					double dist = dCalculation(lats[j + 1], lats[j], lons[j + 1], lons[j]);
					sum = sum + dist;
				} //calculate the new total distance
				if (sum < bests) {
					//if its better, keep the change
					bests = sum;
					counter++;
					a = 0;
				} 
				else {
					//if not return to previous state
					a++; //add one to the counter
					iter_swap(lats.begin() + i + 1, lats.begin() + i + k + 1);
					iter_swap(lons.begin() + i + 1, lons.begin() + i + k + 1);
				}
			}
		} while (a < 2000); //if counter reaches 2000/a.k.a no further solutions for 2000 iterations the while loop ends

		if (bests < gbest) { //if better solution, save the best solution
			gbest = bests;
			for (size_t i = 0; i < lats.size(); i++) {
				bestlats.push_back(lats[i]);
				bestlons.push_back(lons[i]);
			}
			if (NOT == 1) {
				GREEN_F;
				cout << count << ": Optimal route found, takes: " << bests / 1000 << "km. " << counter << " changes were made.\n";
				WHITE_F;
			}
		}
		//this looks for unique solution, if it finds the same one in the vector it will not pushback
		if (find(solutions.begin(), solutions.end(), bests) != solutions.end()) {
		}
		else {
			solutions.push_back(bests);
		}


	}
	//output to the user
	cout << "Going down " << trees << " trees, found " << solutions.size() << " unique solutions and resulted in finding an optimal route that takes " << gbest / 1000 << "km. ";
	tuple <double, id, id> var = make_tuple(gbest, bestlats, bestlons);
	return var;
}

int checkforint() {
	//Checks that the user entered a number
	if (cin.fail()) {
		cout << "An number must be entered\n";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return 0;
	}
	else {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		return 1;
	}

}

