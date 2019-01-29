//
//  main.cpp
//  3500_Assignment2
//
//  Created by Irwan Winarto on 2/4/18.
//  Copyright © 2018 Irwan Winarto. All rights reserved.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include "pthread.h"

using namespace std;

void* countForMe(void* fileNum);

int main(int argc, const char * argv[])
{
    int numFiles = 20;
    string *filename = new string[numFiles];
    pthread_t pid[numFiles];
    int createCheck = 0;
    int joinCheck = 0;
    int* thread_return;
    int* get_returns = new int[5];
    
    // Initialize arrays to 0
    for(int i = 0; i < 5; i++)
    {
        get_returns[i] = 0;
    }
    
    // Write filenames
    for (int i = 0; i < numFiles; i++)
    {
        filename[i] = "file" + to_string(i) + ".txt";
    }

    // Create threads
    for (int i = 0; i < numFiles; i++)
    {
        createCheck = pthread_create(&pid[i], NULL, countForMe, (void**) &filename[i]);
        if (createCheck != 0)
        {
            cout << "Error creating thread!\n";
            return 0;
        }
    }
    
    // Join threads
    for (int i = 0; i < numFiles; i++)
    {
        joinCheck = pthread_join(pid[i], (void**)&thread_return);
        if (joinCheck != 0)
        {
            cout << "Error joining thread!\n";
            return 0;
        }
        for (int j = 0; j < 5; j++)
            get_returns[j] = get_returns[j] + thread_return[j];
    }
    

        cout << "As: " << get_returns[0] << endl;
        cout << "Es: " << get_returns[1] << endl;
        cout << "Is: " << get_returns[2] << endl;
        cout << "Os: " << get_returns[3] << endl;
        cout << "Us: " << get_returns[4] << endl;

    delete [] filename;
    delete [] get_returns;
    return 0;
}

void* countForMe(void* fileName)
{
    string filename = *(string*)fileName;
    char c;
    int *vowelCount = new int[5];
    
    // Initialize array ptr to 0
    for (int i =0; i < 5; i++)
        vowelCount[i] = 0;
    
    ifstream inFile;
    inFile.open("//Users/TonyWijaya/Desktop/Xcode/3500_Assignment2/3500_Assignment2/"+filename);
    // /Users/TonyWijaya/Desktop/Xcode/3500_Assignment2/3500_Assignment2/file0.txt
    // /home/fac/elarson/cpsc3500/pa2a
    if(!inFile)
    {
        cerr << "Unable to open file.\n";
        pthread_exit(NULL);
    }
    else
    {
        while(!inFile.eof())
        {
            inFile.get(c);
            c = tolower(c);
            if(inFile.eof())
                break;
            else if ( c == 'a')
                vowelCount[0]++;
            else if ( c == 'e')
                vowelCount[1]++;
            else if ( c == 'i')
                vowelCount[2]++;
            else if ( c == 'o')
                vowelCount[3]++;
            else if ( c == 'u')
                vowelCount[4]++;
        }
    }
    
    inFile.close();
    pthread_exit( (void*)vowelCount );
}