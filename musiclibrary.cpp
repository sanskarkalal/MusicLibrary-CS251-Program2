
///**-------------------------------------------
//
//Project 2 : Music Library App
//
//Course: CS 251, Spring 2022, UIC
//
//Author: Sanskar Kalal(651632119)
//
//**--------------------------------------------
/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023



#include <iostream>
#include <string>
#include <set>
#include <map>
using namespace std;

#include "helpers.cpp"

//create a struct that contains the set of artists and songs
struct albumData{
    set<string>artist;
    set<string>songs;
};


// loads the data into the library
void loadData(const string& filename,map<string,albumData>&library){

    string line;
    string albumName;
    ifstream myfile (filename);
    if(!myfile.is_open()){
        if(!filename.empty())
            cout<<"Error: Could not open music library file - "<<filename<<endl;
    }
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            //first line will be albumName
            if (albumName.empty())
            {
                albumName = line;
                continue;
            }
            //check if albumName reappears
            if(line == albumName)
            {
                //reset albumName
                albumName = "";
            }
            else {
                //insert songs in the set of songs
                if (line[0] == '0'||line[0]=='1') {
                    library[albumName].songs.insert(line);
                } else{
                    //insert artists in the set of artists
                    library[albumName].artist.emplace(line);
                }
            }

        }
        //close the file
        myfile.close();

    }

}


// calculates the stats of the library and shows the contents if specified
void stats(const string& remains,map<string,albumData>&library){
    // counter for unique songs and artists
    set<string> songs;
    set<string> artists;
    for(auto i:library){
        for(auto j:i.second.songs){
            songs.emplace(j);
        }
        for(auto k:i.second.artist){
            artists.emplace(k);
        }
    }
    //Print the stats
    cout<<"Overall Music Library Stats"<<endl;
    cout<<"==========================="<<endl;
    cout<<"Total Records: "<<library.size()<<endl;
    cout<<"Total Unique Artists: "<<artists.size()<<endl;
    cout<<"Total Songs: "<<songs.size()<<endl;
    cout<<endl;

    //Print the details if specified
    if(remains == "-d"){
        cout<<"Your Current Music Library Includes"<<endl;
        cout<<"==================================="<<endl;

        for(auto i:library){
            cout<<i.first<<endl;
            for (auto j:library[i.first].artist) {
                cout<<" "<<j<<endl;
            }
            for (auto j:library[i.first].songs) {
                cout<<"   "<<j<<endl;
            }

        }
        cout<<endl;
    }

}


// exports the current library into the
void exportFile(map<string,albumData> &library,string filename){

    ofstream myfile;
    //default file would be musicdatabase.txt
    if(filename.empty()){
        filename = "musicdatabase.txt";
    }
    //open the file
    myfile.open(filename);

    //write to the file
    for(auto i:library){
        myfile<<i.first<<endl;
        for (auto j:library[i.first].artist) {
            myfile<<j<<endl;
        }
        for (auto j:library[i.first].songs) {
            myfile<<j<<endl;
        }
        myfile<<i.first<<endl;
    }
    //close the file
    myfile.close();
}


//Prints the search results to the screen
void printResults(set<string>&searchResults){
    // check if results are empty
    if(searchResults.empty()){
        cout<<"No results found."<<endl;
    }else{
        cout<<"Your search results exist in the following albums: "<<endl;
        for(auto i:searchResults){
            cout<<i<<endl;
        }
        cout<<endl;
    }
}


//searches songs artists and albums in the map gives the result albums
void search( string& remains,map<string,albumData>&library){
    // check if the search terms exist
    if(remains.empty()){
        cout<<"Error: Search terms cannot be empty.\n"
              "No results found.\n\n";
    }else{

        string type;
        string modifier;
        string searchTerms;
        // get the type of data to be searched and the search terms
        splitFirstWord(trimWhiteSpace(remains), type, searchTerms);
        //create a set to store the results
        set<string> searchResults;

        //loop for reading the all the search terms until there is no term left
        while (!(searchTerms.empty()) && searchTerms!=" ") {
            string word;

            //get the word to search
            splitFirstWord(trimWhiteSpace(searchTerms), word, searchTerms);
            tolower(word);
            //create a set to store the result of a single word search
            set<string> wordSearch;

            //lower for comparison
            tolower(type);

            // looping through the library
            for(const auto &i:library){
                //check if user is searching an album
                if(type == "album"){
                    string name = i.first;
                    tolower(name);
                    //add to album to wordSearch if word found in albumName
                    if (name.find(word) != string::npos) {
                        wordSearch.insert(i.first);
                    }

                    //check if user is searching a song
                }else if(type == "song"){
                    // loop through the set of songs
                    for (string j: i.second.songs) {
                        string songName = j;
                        tolower(songName);
                        //add album to wordSearch if word found in the song
                        if (songName.find(word) != string::npos) {
                            wordSearch.insert(i.first);
                        }
                    }
                    //check if user is searching an artist
                }else if(type=="artist"){
                    //loop through the set of artists
                    for (const string& j: i.second.artist) {
                        string artistName = j;
                        tolower(artistName);
                        //add the album to wordSearch if word found in artistName
                        if (artistName.find(word) != string::npos) {
                            wordSearch.insert(i.first);
                        }
                    }
                }
            }

            // check for modifier (first word would never have one)
            if (modifier.empty()) {
                //add the words to the final searchResults set
                for (const auto& i: wordSearch) {
                    searchResults.insert(i);
                }
            }
            //intersection modifier would find the common elements in the new wordSearch and the searchResults set
            if (modifier == "+") {
                set<string> intersection;
                for (const auto& i: searchResults) {
                    //if count is 1 add to the intersection set
                    if (wordSearch.count(i)) {
                        intersection.insert(i);
                    }
                }
                // clear the results
                searchResults.clear();
                //final result is an intersection
                searchResults = intersection;
                // reset the modifier for the next interarion
                modifier = "";
            }
            //difference will find the unique elements of the newSearch that are not in the searchResults
            if (modifier == "-") {
                set<string> difference;
                //loop through the searchResults
                for (const auto& i: searchResults) {
                    //if count is 0 then add the album to difference set
                    if (wordSearch.count(i) == 0) {
                        difference.insert(i);
                    }
                }
                //clear the searchResults
                searchResults.clear();
                //final result is the difference set
                searchResults = difference;
                //reset the modifier for next iteration
                modifier = "";

            }

            // check if additional search terms exist
            if (!searchTerms.empty()) {
                // get the modifier
                if ((searchTerms.at(0) == '+' || searchTerms.at(0) == '-')) {
                    modifier = searchTerms.at(0);
                    // remove the modifier
                    searchTerms = searchTerms.substr(1);
                }

            }
        }
        //print the final results
        printResults(searchResults);
    }
}


int main()
{
    // create a map that has the albumName as the key and struct albumData as the value
    map<string,albumData> library;
    string userEntry;
    string command, remains;


    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;

        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(trimWhiteSpace(userEntry), command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            //clear the map
            library.clear();

        }
        else if (command == "export")
        {
            //export the file into the given file
            exportFile(library,remains);
        }
        else if (command == "load")
        {
            //load the data into the library
            loadData(remains,library);
        }
        else if (command == "stats")
        {
            //prints the stats of the library
            stats(remains,library);
        }
        else if (command == "search")
        {
            //search for terms inside the library
            search(remains,library);
        }

    }while(command != "exit");

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
