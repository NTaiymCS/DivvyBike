// Bike stations in the DIVVY system and riders that have used a DIVVY bike. 
// Inputs data into two dynamically-allocated arrays by inputting the name of the stations file,
// and prompting and inputting the name of the bike trips file from the keyboard.
// 
#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <fstream>


using namespace std;

/* All functions must be less than 100 lines */

// BikeStation
// Defines each variable: Station ID, Capacity, Latitude, Longitude, and Name
struct BikeStation
{
   string  StationID;
   string  Name;
   int     Capacity;
   double  Latitude;
   double  Longitude;
};

// BikeTrips
// Defines each variable: Trip ID, Bike ID, Start station ID, End station ID, Trip Duration, Start Time
struct BikeTrip
{
   string  TripID;
   string  BikeID;
   string  StartTime;
   string  StartStID;
   string  EndStID;
   int     TripLength;
};
//InputBikeStData
//Inputs the bike station data into a dynamically allocated array of struct BikeStation
BikeStation* inputBikeStData(string stationFile, int& N)
{
    //tries to open the bike station file
    ifstream infile;
    infile.open(stationFile);
    
    //if the file failed to open
    if(!infile.good())
    {
        cout << "Error: Unable to open station file " << stationFile << endl;
        N = 0;
        return nullptr;
    }
    infile >> N;
    //allocated array of struct
    BikeStation* stationData = new BikeStation[N];
    
    int i;
    //inputting the station data data into the array
    for(i = 0; i < N; i++)
    {
        infile >> stationData[i].StationID;
        infile >> stationData[i].Capacity;
        infile >> stationData[i].Latitude;
        infile >> stationData[i].Longitude;
        getline(infile, stationData[i].Name);
    }
    //closes the infile
    infile.close();
    return stationData;
}

//InputBikeTripData
//Inputs the bike trip data into a dynamically allocated array of struct BikeTrip
BikeTrip* inputBikeTripData(string bikeTripFile, int& Q)
{
    
    ifstream infile;
    infile.open(bikeTripFile);
    
    //if the file failed to open
    if(!infile.good())
    {
        cout << "Error: Unable to open station file " << bikeTripFile << endl;
        Q = 0;
        return nullptr;
    }
    
    //file is open, inputs N as the number of total station data
    infile >> Q;
    
    
    //allocated array of struct
    BikeTrip* bikeTrip = new BikeTrip[Q];
    int i;
    //inputting the bike trip data data into the array
    for(i = 0; i < Q; i++)
    {
        infile >> bikeTrip[i].TripID;
        infile >> bikeTrip[i].BikeID;
        infile >> bikeTrip[i].StartStID;
        infile >> bikeTrip[i].EndStID;
        infile >> bikeTrip[i].TripLength;
        infile >> bikeTrip[i].StartTime;
    }
    
    infile.close();
    return bikeTrip;
}

//search function for station data
int searchStation(BikeStation* bikeStationData, int N, string& ID)
{   
    int i;
    for(i = 0; i < N; i++)
    {
        if(bikeStationData[i].StationID == ID) //compares the position of name to null position
        {
            return i; //returns index
        }
    }
    return -1; //if not equivalent return negative 1
}

// sorts data alphabetically by name
void sortAlphabetically(BikeStation* bikeStationData, int& N)
{
    for (int i = 0; i < N-1; i++)
    {
      int smallestName = i; // smallest index is i
      
      for (int j = i+1; j < N; j++)
      {
         if (bikeStationData[j].Name < bikeStationData[smallestName].Name) 
         {
            smallestName = j;
         }
      }
        //swaps
      BikeStation temp = bikeStationData[i];
      bikeStationData[i] = bikeStationData[smallestName];
      bikeStationData[smallestName] = temp;
   }
}
// counts and returns the total number of bike trips
int totalTrips(BikeTrip* bikeTripData, int& Q, string& StationID)
{
    int tripTotal = 0;
    int i;
    for (i = 0; i < Q; i++)
    {
        if(bikeTripData[i].StartStID == StationID || StationID == bikeTripData[i].EndStID)
        {
          tripTotal++;
        }

    }
    return tripTotal;
}

// distBetween2Points
//
// Originally written by Prof Hummel
// 
// Returns the distance in miles between 2 points (lat1, long1) and 
// (lat2, long2).  Latitudes are positive above the equator and 
// negative below; longitudes are positive heading east of Greenwich 
// and negative heading west.  Example: Chicago is (41.88, -87.63).
//
// NOTE: you may get slightly different results depending on which 
// (lat, long) pair is passed as the first parameter.
double distBetween2Points(double& lat1, double& long1, double& lat2, double& long2)
{
  
  double PI = 3.14159265;
  double earth_rad = 3963.1;  // statue miles:

  double lat1_rad = lat1 * PI / 180.0;
  double long1_rad = long1 * PI / 180.0;
  double lat2_rad = lat2 * PI / 180.0;
  double long2_rad = long2 * PI / 180.0;

  double dist = earth_rad * acos(
    (cos(lat1_rad) * cos(long1_rad) * cos(lat2_rad) * cos(long2_rad))
    +
    (cos(lat1_rad) * sin(long1_rad) * cos(lat2_rad) * sin(long2_rad))
    +
    (sin(lat1_rad) * sin(lat2_rad))
  );
  
  return dist;
}
// Quick Stats function
// Reads the total number of bike stations, the total number of trips, 
// and the total capacity between all the stations
void quickStats(BikeStation* bikeStationData, BikeTrip* bikeTripData, int& N, int& Q)
{
    cout << "stations: " << N << endl
         << "trips: " << Q << endl;
    
    int i;

    int totalCapacity = 0;

        for(i = 0; i < N; i++)
        {
            totalCapacity += bikeStationData[i].Capacity; //prints adds all capacities from bike station 
        }

    cout << "total bike capacity: " << totalCapacity << endl;
}

//Summary of bike durations function
//Analyzes the bike trips and places the trip duration into categories from 
//<=30 minutes, 30-60 minutes, 1-2 hours, 2-5 hours, and trips 5+ hours
void sumBikeDurations(BikeTrip* bikeTripData, int& Q)
{
    int i;
    int lessThan30min = 0; //variables for each of the times
    int between30and60min = 0;
    int between1to2hrs = 0;
    int between2to5hrs = 0;
    int over5hrs = 0;
        // compares time length in seconds
        for(i = 0; i < Q; i++)
        {
            if(bikeTripData[i].TripLength <= 1800) 
            {
                lessThan30min += 1;
            }
            else if(bikeTripData[i].TripLength <= 3600)
            {
                between30and60min += 1;
            }
            else if(bikeTripData[i].TripLength <= 7200)
            {
                between1to2hrs += 1;
            }
            else if(bikeTripData[i].TripLength <= 18000)
            {
                between2to5hrs += 1;
            }
            else
            {
                over5hrs+=1;
            }
        }
    //prints out data collected for the time frames
    cout << " trips <= 30 mins: " << lessThan30min << endl
         << " trips 30..60mins: " << between30and60min << endl
         << " trips 1-2 hrs: " << between1to2hrs << endl
         << " trips 2-5 hrs: " << between2to5hrs << endl
         << " trips > 5hrs: " << over5hrs << endl;
}

// Histogram of starting times function
// Reads the bike trips and places them into categories in a 24-hour
// format based on stating hour
void histogramStarting(BikeTrip* bikeTripData, int& Q)
{
    int timeSpan[24];
    int i;
    //initializes all values in the array from 0-23 to 0
    for(i = 0; i < 24; i++)
    {
        timeSpan[i] = 0;
    }
    
    // stores all data time into the array and increments element
    for(i = 0; i < Q; i++)
    {
        int tripStartTime = stoi(bikeTripData[i].StartTime); //converts the string time to integer
        timeSpan[tripStartTime]++;
    }
    // prints out the array and element ex 1: 3
    for(i = 0; i < 24; i++)
    {
        cout << i << ": " <<timeSpan[i] <<endl;
    }
 
}

// Stations near me function
// finds all near stations by the latitude and longitude coordinates and sorts them from closest to furthest.
// calls function distance between two points to compute
void stationsNearMe(BikeStation* bikeStationData, int& N, double& lat1, double& long1, double& distancegiven)
{
    cout << "The following stations are within " << distancegiven << " miles of (" << lat1 << ", " << long1 << "):" << endl;
    bool found = false;
    BikeStation temp;
    int smallestIndex = 0;
    int count = 0;
    int i, k, n;
    double shortestDistance = 0;
    double distance = 0;
    
    for(i = 0; i < N; i++)
    {
        distance = distBetween2Points(lat1, long1, bikeStationData[i].Latitude, bikeStationData[i].Longitude);
        if(distance <= distancegiven)
        {
            found = true;
        }
    }
    if(!found)
    {
        cout << "none found" << endl;
    }
        else{
        for(n = 0; n < N; n++)
        {
            smallestIndex = count; //assume smallest index is count before incremented
            for(k = count + 1; k < N; k++)
            {
                distance = distBetween2Points(lat1, long1, bikeStationData[k].Latitude, bikeStationData[k].Longitude); //stores into distance
                shortestDistance = distBetween2Points(lat1, long1, bikeStationData[smallestIndex].Latitude, bikeStationData[smallestIndex].Longitude);
                if(distance < shortestDistance)
                {
                    smallestIndex = k; //k is the smallest index stored
                }
            }
            count++; //increments
            temp = bikeStationData[n]; //swaps data
            bikeStationData[n] = bikeStationData[smallestIndex];
            bikeStationData[smallestIndex] = temp;
        }
        for(n = 0; n < N; n++)
        {
            distance = distBetween2Points(lat1, long1, bikeStationData[n].Latitude, bikeStationData[n].Longitude); //stores distance function to distance
            if(distance <= distancegiven)
            {
                bikeStationData[n].Name.erase(0,1); //erases space
                cout << "station " << bikeStationData[n].StationID << " (" << bikeStationData[n].Name << "): " << distance << " miles" << endl; //outputs
            }
        }
    }
}

// list all stations function
// Lists all the stations in alphabetical order by name, includes the name station ID,
// position in (latitude, longitude) and total number of trips
//
void listAllStations(BikeStation* bikeStationData, BikeTrip* bikeTripData, int& N, int& Q)
{
    int i;
    sortAlphabetically(bikeStationData, N); // sorts alphabetically
    for(i = 0; i < N; i++) //prints out data alphabetically
    {    
        cout << bikeStationData[i].Name << " (" << bikeStationData[i].StationID << ") @ ("
             << bikeStationData[i].Latitude << "," << bikeStationData[i].Longitude << "), " << bikeStationData[i].Capacity
             << " capacity, " << totalTrips(bikeTripData, Q, bikeStationData[i].StationID) << " trips" << endl; 
     }
}

// find stations function
// a user can search for stations by typing in a case-sensitive (one) word input
// output is in alphabetical order by name, includes station ID, position, capacity, and total number of bike trips
void findStations(BikeStation* bikeStationData, BikeTrip* bikeTripData, string& station, int& N, int& Q)
{ 
    sortAlphabetically(bikeStationData, N); //sorts data in alphabetical order by name
    int i;
    bool found = false;
    for(i = 0; i < N; i++)
    {
        if(bikeStationData[i].Name.find(station) != string::npos) //compares to null string position
        {
            found = true;
            cout << bikeStationData[i].Name << " (" << bikeStationData[i].StationID << ") @ ("
             << bikeStationData[i].Latitude << "," << bikeStationData[i].Longitude << "), " << bikeStationData[i].Capacity
             << " capacity, " << totalTrips(bikeTripData, Q, bikeStationData[i].StationID) << " trips" << endl; 
        }
    }   
    if(!found) //if there is no stations found
    {
        cout << "none found" << endl;
    }
}
//counts the number of trips there are within a given time range. for case: start time > stop time, 
//stop time > start time, and start time = stop time = trip time. returns count
int countTimeSpan(BikeStation* bikeStationData, BikeTrip* bikeTripData, string& starttime, string& stoptime, int& N, int& Q, int& count, int& counter, int& durationTrips, BikeStation*& trips, int& avgTrip)
{       
    int startHours = stoi(starttime.substr(0,starttime.find(':'))); // hours is the string up to the colon
    int startMin = stoi(starttime.substr(starttime.find(':')+1));  // minutes are colon to the end of string
    int endHours = stoi(stoptime.substr(0,stoptime.find(':')));  
    int endMin = stoi(stoptime.substr(stoptime.find(':')+1)); 
    string stringOfNames;
    int tripHr = 0;
    int tripMin = 0;
    for(int i = 0; i < Q; i++)
    {   
        tripHr = stoi(bikeTripData[i].StartTime.substr(0,bikeTripData[i].StartTime.find(':')));  //stores startTime into the tripHr
        tripMin = stoi(bikeTripData[i].StartTime.substr(bikeTripData[i].StartTime.find(':')+1));
        counter = count; //previous count = new count
        
        if(startHours > endHours) // if starting time is greater than/ equal to the stopping time  (wrap around case)
        {
            if(0 <= tripHr && tripHr <= endHours)
            {
                count++;
            }
        }
        else if(startHours < endHours) // else if the starting is less than/ equal to the stopping time (between)
        {
            if(tripHr >= startHours && tripHr <= endHours) //compares the trip hours to start hours and end hours
            {
                count++;
            }
        }
        else if(startHours == endHours && startHours == tripHr)
        { 
            if(startMin <= tripMin && tripMin <= endMin) //compares the minutes to the start and end minutes
            {
                 count++;
            }
        }  
        if(count > counter) //if count increments then 
        {
            // adding the duration here
            durationTrips += bikeTripData[i].TripLength;
                                  
            int index = searchStation(bikeStationData, N, bikeTripData[i].StartStID); //find station based on ID;
            //adding index into the new array
            if(index != -1)
            {                   
                trips[counter] = bikeStationData[index]; //array to store index found in the search function
            }
        }
    }
    avgTrip = (durationTrips/60) / count; // converts seconds to minutes then divides by count
    if(count == 0) //not found none found
    {
        cout << "none found" << endl;
    }
    return count;
}
// find trips within a give timespan
// searches trips by the timespan given from user input, if start time falls within the range then a number of trips is outputted along with average
// duration of the trip in minutes. Outputs the names of stations where each trip had started, no repitions. 
// 
void findTimeSpan(BikeStation* bikeStationData, BikeTrip* bikeTripData, string& starttime, string& stoptime, int& N, int& Q)
{   
   
    int avgTrip = 0; //initializes to 0
    int durationTrips = 0;
    
    BikeStation* trips = new BikeStation[N]; // new array
    BikeStation* stringOfNames = new BikeStation[N];
    int count = 0; //new count
    int counter = 0; // previous count
    
    //calling the time span function for the given timespan
    countTimeSpan(bikeStationData, bikeTripData, starttime, stoptime, N, Q, count, counter, durationTrips, trips, avgTrip); 
        if(count > 0)
        { //if found 

             sortAlphabetically(trips, count);   
             //output data
             cout << count << " trips found" << endl 
             << "avg duration: " << avgTrip << " minutes" << endl
             << "stations where trip started: ";
            
            for(int i = 0; i < count; i++) //for loop to store data into new array
            { 
                if(stringOfNames[i].Name == trips[i].Name)
                {
                    continue;
                }
                else{
                    stringOfNames[i] = trips[i];
                }
            }
            for(int i = 0; i < count; i++)
            {
                if(i < count-1) //checks to make sure index isnt last element
                {
                  if(i > 0 && stringOfNames[i].Name == stringOfNames[i-1].Name)
                  {
                      continue;
                  }
                  else{
                       cout << stringOfNames[i].Name << ","; //adds a comma in between spaces
                   }
                }
                else{
                    cout << stringOfNames[i].Name << endl; //prints names out
                }
            }
            
        }
    delete[] stringOfNames; //deletes array
    delete[] trips; //deletes  array
}

int main()
{
    //name of station data file
    string stationFile;
    
    //name of bike trip data file
    string bikeTripFile;
    
    cout << "** Divvy Bike Data Analysis **" << endl;
    cout << "Please enter name of stations file> " << endl;
    cin >> stationFile; //inputs station data file

    cout << "Please enter name of bike trips file> " << endl;
    cin >> bikeTripFile; //inputs station data file
    
    int N = 0; //total number in bike stations array
    int Q = 0; //total number in bike trips array
    
    //calls the bike station data function and inputs data 
    BikeStation* bikeStationData = nullptr;
    bikeStationData = inputBikeStData(stationFile, N);

    //calls bike trip data function and inputs data
    BikeTrip* bikeTripData = nullptr;
    bikeTripData = inputBikeTripData(bikeTripFile, Q);
    //user inputs 
    string command;
    string station, starttime, stoptime;
    double lat1, long1, distancegiven;
    
    cout << "Enter command (# to stop)> ";
    cin >> command;
    while (command != "#")
    {
        if(command == "stats")
        {
            quickStats(bikeStationData, bikeTripData, N, Q);
        }
        else if(command == "durations")
        {
            sumBikeDurations(bikeTripData, Q);
        }
        else if(command == "starting")
        {
            histogramStarting(bikeTripData, Q);
        }
        else if(command == "nearme")
        {
            cin >> lat1;
            cin >> long1;
            cin >> distancegiven;
            stationsNearMe(bikeStationData, N, lat1, long1, distancegiven);
        }
        else if(command == "stations")
        {
            listAllStations(bikeStationData, bikeTripData, N, Q);
        }
        else if(command == "find")
        {
            cin >> station;
            findStations(bikeStationData,bikeTripData, station, N, Q);
        }
        else if(command == "trips")
        {
            cin >> starttime;
            cin >> stoptime;
            findTimeSpan(bikeStationData, bikeTripData, starttime, stoptime, N, Q);
        }
        else
        {
            cout << "** Invalid command, try again..." << endl;
        }
        
        cout << "Enter command (# to stop)> ";
        cin >> command;
    }
    cout << "** Done **";
    //deletes the allocated arrays
    delete[] bikeStationData;
    delete[] bikeTripData;
    
    return 0;
}
