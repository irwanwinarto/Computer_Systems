// Name: Irwan Winarto
//

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void SJF(vector<vector<int> > &array, int row, int col);
void SRTF(vector<vector<int> > &array, int row, int col);
void NP(vector<vector<int> > &array, int row, int col);
void PP(vector<vector<int> > &array, int row, int col);

bool sortArrival(const vector<int>& v1, const vector<int>& v2)
{
    return v1[1] < v2[1];
}
bool sortBT(const vector<int>& v1, const vector<int>& v2)
{
    return v1[2] < v2[2];
}
bool sortRemBT(const vector<int>& v1, const vector<int>& v2)
{
    return v1[5] < v2[5];
}
bool sortPriority(const vector<int>& v1, const vector<int>& v2)
{
    return v1[3] < v2[3];
}

int main(int argc, const char * argv[])
{
    // First argument is input text file. Second argument is scheduling algorithm.
    //string filename = argv[1];
    string filename = "sample.txt";
    string scheduler = "PP";
    //string scheduler = argv[2];
    int option = 0;
    
    // for vectors
    int row = 0;
    int col = 4;
    int colSize = 4;
    // Create 2d vector
    vector<vector<int> > array;
    vector<int> rowVtr(colSize);
    
    // Sort the case
    if(scheduler == "SJF")
        option = 1;
    else if(scheduler == "SRTF")
        option = 2;
    else if (scheduler == "NP")
        option = 3;
    else if (scheduler == "PP")
        option = 4;
    else
        cout << "Invalid scheduler option.\n";

    // Declare input filestream
    ifstream inFile;
    inFile.open("sample.txt");
    if(!inFile)
    {
        cerr << "Unable to open file...gg";
        exit(1);
    }
    else
    {
        while(inFile)
        {
            array.push_back(rowVtr);   // add a new row
            for (col = 0; col < colSize; col++)
            {
                inFile >> array [row][col];

            }
            row++;
        }
    }
    for (int i = 0; i < array.size(); i++)
    {
        int num = 0;
        int num1=0;
        num = array[i][2];
        num1 = array[i][0];
        if(num == 0 && num1 ==0)
        {
            array.erase(array.begin()+i);
            row--;
        }
    }
    
    switch (option) {
        case 1:
            SJF(array, row, col);
            break;
        case 2:
            SRTF(array, row, col);
            break;
        case 3:
            NP(array, row, col);
            break;
        case 4:
            PP(array, row, col);
            break;
            
        default:
            cout << "Invalid input.\n";
          break;
    }
    
    return 0;
}

//*********************************************************************************************************************************************************
//*********************************************************************************************************************************************************
void SJF(vector<vector<int> > &array, int row, int col)
{
    vector<vector<int> > duplicate;
    for (int i = 0; i < row; i++)
    {
        duplicate.push_back(vector<int>());
        for (int j = 0; j < col+1; j++)
        {
            duplicate[i].push_back(array[i][j]);
            duplicate[i][4] = 0;
        }
    }
    // Sort by arrival time
    sort(duplicate.begin(), duplicate.end(), sortArrival);
    
    //create queue
    vector<vector<int> > readyV;
    vector<int> data(5);
    
    //create timer
    int timer = 0;
    int dead = 0;
    vector<vector<int> > outputVector;
    vector<int> out(3);
    bool outThis = true;
    vector<vector<float> > calculation;
    vector<float> otherData(6);
    float terminationTime=0;
    float waitingTime=0;
    float totalWaitingTime=0;
    float totalDelay=0;
    float lastTime=0;
    
    while (timer < 100)
    {
        if(duplicate.size() > 0)
        {
            dead = 0;
            for(int i = 0; i < duplicate.size(); i++)
            {
                if (duplicate[i][1] <= timer)
                {
                    readyV.push_back(duplicate[i]);
                    dead++;
                }
            }
            duplicate.erase( duplicate.begin(), (duplicate.begin()+dead));
            sort(readyV.begin(), readyV.end(), sortBT);
        }

        if (!readyV.empty())
        {
            outThis = true;
            cout << "Time " << timer << " Process " << readyV[0][0] << endl;;
            timer += readyV[0][2];

            // zees
            terminationTime = timer;
            waitingTime = terminationTime-readyV[0][1] - readyV[0][2];
            totalWaitingTime += waitingTime;
            
            readyV.erase(readyV.begin());
            lastTime = timer;
        }
        else
        {
            if(outThis)
            {
                float idleTime=0;
                idleTime=timer;
                cout << "Time " << idleTime << " idle\n";
                outThis = false;
                totalDelay += idleTime;
            }
            timer++;
        }
    }
    cout << "CPU: " << (lastTime-totalDelay)/lastTime*100.0 << "%" << endl;
    cout << "Average Waiting Time: " << totalWaitingTime/array.size() << endl;
    cout << endl;
}
//*********************************************************************************************************************************************************
//*********************************************************************************************************************************************************
void SRTF(vector<vector<int> > &array, int row, int col)
{
    // duplicate and create a column for "processed"
    vector<vector<int> > duplicate;
    vector<int> cols(6);
    for (int i = 0; i < row; i++)
    {
        duplicate.push_back(cols);
        for (int j = 0; j < col+2; j++)
        {
            duplicate[i][j] = array[i][j];
            duplicate[i][4] = 0;
            duplicate[i][5] = duplicate[i][2];
        }
    }
    
    // Sort by arrival time
    sort(duplicate.begin(), duplicate.end(), sortArrival);
    
    //create queue
    vector<vector<int> > readyV;
    vector<int> data(6);

    vector<vector<int> > outputVector;
    vector<int> out(3);
    bool firstOut = true;
    float terminationTime=0;
    float waitingTime=0;
    float totalWaitingTime=0;
    float idleTime=0;
    float lastTime=0;
    
    //create timer
    int timer = 0;
    int dead = 0;
    int tempCount =0;
    
    while (timer < 50)
    {
        // Add vectors to readyV. Go through duplicate and add to queue
        // erase begin to bwgin + number of pushed
        if(duplicate.size() > 0)
        {
            dead = 0;
            for(int i = 0; i < duplicate.size(); i++)
            {
                if (duplicate[i][1] <= timer)
                {
                    readyV.push_back(duplicate[i]);
                    dead++;//variable keeo track how many gets oushed
                }
            }
            duplicate.erase( duplicate.begin(), (duplicate.begin()+dead));
            sort(readyV.begin(), readyV.end(), sortBT);
        }
        
        // Determine if we need to swtich based on lower burst time
        if(!readyV.empty())
        {
            tempCount = timer;
            for (int i = 0; i < readyV.size(); i++)
            {
                if (readyV[i][5] == 0)
                {
                    // zees
                    terminationTime = timer;
                    waitingTime = terminationTime-readyV[0][1] - readyV[0][2];
                    totalWaitingTime += waitingTime;
                    
                    readyV.erase(readyV.begin());

                }
                else
                {
                    // decrement the current remBT, then sort by remBT
                    sort(readyV.begin(), readyV.end(), sortRemBT);
                }
                out[0] = readyV[0][0];
                out[1] = timer;
                out[2] = 0;
                if(readyV.size()==0)
                    out[2] =1;
                outputVector.push_back(out);
            }
        }
        readyV[0][5]--;
        timer++;
    }


    for (int i = 1; i < outputVector.size(); i++)
    {
        if(firstOut)
        {
            cout << "Time " << outputVector[i-1][1] << " Process " << outputVector[i-1][0] << endl;
            firstOut = false;
        }
        if(outputVector[i][0] != outputVector[i-1][0])
        {
            cout << "Time " << outputVector[i][1] << " Process " << outputVector[i][0] << endl;
            lastTime += outputVector[i][1];
        }
        else if (outputVector[i][2] ==1)
        {
            cout << "Time " << outputVector[i][1] << " Idle\n";
            idleTime +=outputVector[i][1];
        }
    }

    cout << "CPU: " << (lastTime-idleTime)/lastTime*100.0 << "%" << endl;
    cout << "Average Waiting Time: " << totalWaitingTime/array.size() << endl;
    cout << endl;
}

//*********************************************************************************************************************************************************
//*********************************************************************************************************************************************************
void NP(vector<vector<int> > &array, int row, int col)
{
    // duplicate and create a column for "processed"
    vector<vector<int> > duplicate;
    for (int i = 0; i < row; i++)
    {
        duplicate.push_back(vector<int>());
        for (int j = 0; j < col+1; j++)
        {
            duplicate[i].push_back(array[i][j]);
            duplicate[i][4] = 0;
        }
    }

    // Sort by arrival time
    sort(duplicate.begin(), duplicate.end(), sortArrival);
    
    //create queue
    vector<vector<int> > readyV;
    vector<int> data(5);
    
    //create timer
    int timer = 0;
    int dead = 0;
    vector<vector<int> > outputVector;
    vector<int> out(3);
    bool outThis = true;
    float terminationTime=0;
    float waitingTime=0;
    float totalWaitingTime=0;
    float idleTime=0;
    float lastTime=0;
    
    while (timer < 100)
    {
        // Add vectors to readyV
        //readyV.push_back(data);
        // Go through duplicate and add to queue
        if(duplicate.size() > 0)
        {
            dead = 0;
            for(int i = 0; i < duplicate.size(); i++)
            {
                if (duplicate[i][1] <= timer)
                {
                    readyV.push_back(duplicate[i]);
                    dead++;//variable keeo track how many gets oushed
                }
            }
            // erase begin to bwgin + number of pushed
            duplicate.erase( duplicate.begin(), (duplicate.begin()+dead));
            
            sort(readyV.begin(), readyV.end(), sortPriority);
        }
        // Check
        if (!readyV.empty())
        {
            outThis = true;
            cout << "Time " << timer << " Process " << readyV[0][0] << endl;;
            timer += readyV[0][2];
            
            // zees
            terminationTime = timer;
            waitingTime = terminationTime-readyV[0][1] - readyV[0][2];
            totalWaitingTime += waitingTime;
            readyV.erase(readyV.begin());
        }
        else
        {
            if(outThis)
            {
                cout << "Time " << timer << " idle\n";
                idleTime += timer;
                outThis = false;
            }
            timer++;
        }
    }
    

    cout << "CPU: " << (lastTime-idleTime)/lastTime*100.0 << "%" << endl;
    cout << "Average Waiting Time: " << totalWaitingTime/array.size() << endl;
    cout << endl;
}
//*********************************************************************************************************************************************************
//*********************************************************************************************************************************************************
void PP(vector<vector<int> > &array, int row, int col)
{
    // duplicate and create a column for "processed"
    vector<vector<int> > duplicate;
    vector<int> cols(6);
    for (int i = 0; i < row; i++)
    {
        duplicate.push_back(cols);
        for (int j = 0; j < col+2; j++)
        {
            duplicate[i][j] = array[i][j];
            duplicate[i][4] = 0;
            duplicate[i][5] = duplicate[i][2];
        }
    }
    
    // Sort by arrival time
    sort(duplicate.begin(), duplicate.end(), sortArrival);
    
    //create queue
    vector<vector<int> > readyV;
    vector<int> data(6);
    
    //create timer
    int timer = 0;
    int dead = 0;
    int tempCount =0;
    vector<vector<int> > outputVector;
    vector<int> out(3);
    bool firstOut = true;
    float terminationTime=0;
    float waitingTime=0;
    float totalWaitingTime=0;
    float idleTime=0;
    float lastTime=0;
    
    while (timer < 70)
    {
        // Add vectors to readyV. Go through duplicate and add to queue
        // erase begin to bwgin + number of pushed
        if(duplicate.size() > 0)
        {
            dead = 0;
            for(int i = 0; i < duplicate.size(); i++)
            {
                if (duplicate[i][1] <= timer)
                {
                    readyV.push_back(duplicate[i]);
                    dead++;//variable keeo track how many gets oushed
                }
            }
            duplicate.erase( duplicate.begin(), (duplicate.begin()+dead));
            sort(readyV.begin(), readyV.end(), sortPriority);
        }
        
        // Determine if we need to swtich based on lower burst time
        if(!readyV.empty())
        {
            tempCount = timer;
            for (int i = 0; i < readyV.size(); i++)
            {
                if (readyV[i][5] == 0)
                {
                    //delete if their remBT has ended
                    // zees
                    terminationTime = timer;
                    waitingTime = terminationTime-readyV[0][1] - readyV[0][2];
                    totalWaitingTime += waitingTime;
                    
                    readyV.erase(readyV.begin());
                }
                else
                {
                    // decrement the current remBT, then sort by remBT
                    sort(readyV.begin(), readyV.end(), sortPriority);
                }
                out[0] = readyV[0][0];
                out[1] = timer;
                out[2] = 0;
                if(readyV.size()==0)
                    out[2] =1;
                outputVector.push_back(out);
            }
        }
        readyV[0][5]--;
        timer++;
    }
    
    for (int i = 1; i < outputVector.size(); i++)
    {
        if(firstOut)
        {
            cout << "Time " << outputVector[i-1][1] << " Process " << outputVector[i-1][0] << endl;
            firstOut = false;
        }
        if(outputVector[i][0] != outputVector[i-1][0])
            cout << "Time " << outputVector[i][1] << " Process " << outputVector[i][0] << endl;
        else if (outputVector[i][2] ==1)
            cout << "Time " << outputVector[i][1] << " Idle\n";
    }
    cout << "CPU: " << (lastTime-idleTime)/lastTime*100.0 << "%" << endl;
    cout << "Average Waiting Time: " << totalWaitingTime/array.size() << endl;
    cout << endl;
}


