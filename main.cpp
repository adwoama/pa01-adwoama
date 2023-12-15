#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

//train_dataset_20k.csv test_dataset_10k.csv test_dataset_sentiment_10k.csv
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iomanip>

#include "DSString.h"
#include "Tweet.h"

void configData(map<DSString,int>&, map<DSString,int>&,DSString);
void testData(vector<Tweet>&, vector<Tweet>&, map<DSString,int>&, map<DSString,int>&,DSString);
double checkData(vector<Tweet>&, vector<Tweet>&,DSString, DSString, vector<int>&);

int testFunc(){
    return Catch::Session().run();
}

int main(int argc, char *argv[]) {
    if(argc == 1){
        testFunc();
        return 0;
    }

    else {
        cout << "TEST" << endl;
        DSString file1 = argv[1];
        DSString file2 = argv[2];
        DSString file3 = argv[3];
        DSString outputFile = "output01.txt";

        //cout << file1 << endl;
        //cout << file2 << endl;
        //cout << file3 << endl;

        map<DSString, int> goodWords;//list of words from definite positive tweets
        map<DSString, int> badWords;//list of words from definite negative tweets
        vector<Tweet> positiveTweets;//computer guess positive tweets
        vector<Tweet> negativeTweets;//computer guess negative tweets
        vector<int> wrongGuesses;//Tweet ids of tweets wrongfully classified by algorithm
        double algAccuracy = 0.0;//percentage of correct classifications done by algorithm


        configData(goodWords, badWords, file1);
        cout << "finished configData method" << endl;

        testData(positiveTweets, negativeTweets, goodWords, badWords, file2);
        cout << "finished testData method" << endl;
        algAccuracy = checkData(positiveTweets, negativeTweets, file3, outputFile, wrongGuesses);
        cout << "finished check data" << endl;
        cout << fixed << setprecision(3) << "The accuracy was: " << algAccuracy;

        return 0;
    }
}
/**
 *
 * @param good map container of value: # of times word appears, key: positive word
 * @param bad map container of value: # of times word appears, key: negative word
 * @param file
 *
 * reads in tweet data from a file and depending on the Tweet's sentiment
 * add the word to the good tweet list or the bad tweet list and adds 1
 * to the counter for the number of times that word has appeared in
 * positive(good) or negative(bad) tweets (the key in the map object)
 */
void configData(map<DSString,int>& good, map<DSString,int>& bad,DSString file){
    ifstream inFS;
    DSString holdString;
    char* temp = new char [10000];
    int sentiment = 1;
    int holdInt = 1;
    char holdChar;
    int loop = 0;
    cout << " in cofigData" << endl;
    inFS.open(file.c_str());
    cout << "opening file" << endl;
    if(!inFS.is_open()){
        cout << "configData: Failed to open: " << file << endl;
    }//failed to open config file
    cout << "configData: File opened: " << file << endl;
    inFS >> temp;//reads in column headers

    while(!inFS.eof()){//read in data until end of file

        inFS.getline(temp, 1000,',');
        sentiment = atoi(temp);

        inFS.getline(temp, 1000,',');

        inFS.getline(temp, 1000,',');//read date

        inFS.getline(temp, 1000, ',');//read query

        inFS.getline(temp, 1000, ',');//read user

        inFS.getline(temp, 1000);//read message
        holdString = temp;


        if(holdString.find("д")>0||holdString.find("и")>0||holdString.find("л")>0){
            holdString = "";
        }

        if(sentiment == 0) {//if negative sentiment, add words to bad map

            DSString word("");
            int wordSize = 0;
            for(int i = 0; i < holdString.getLength(); i++){

                if (wordSize > 13){
                    word = "";
                }
                if( (holdString.find("http")>=0)){
                    int linkAt = holdString.find("http");
                    holdString = holdString.substring(linkAt+4,holdString.getLength()-3-linkAt);

                }
                if(holdString.substring(i,1) != " ") {
                    if(isalpha(holdString[i]))
                        holdString[i] = tolower(holdString[i]);
                    word = word + holdString.substring(i, 1);
                    wordSize++;

                }//if not end of word
                else{//else if end of word, add word to bad word list
                    if(bad.count(word)==0 && wordSize > 4) {// if word not in list yet, add to list
                        bad.emplace(word, 2);
                        word ="";
                        wordSize = 0;
                    }
                    else if((bad.count(word)!=0 && wordSize > 4)){//increase count of word

                        int count = bad.at(word)+1;
                        bad.erase(word);
                        bad.emplace(word,count);
                        word ="";
                        wordSize = 0;
                    }//end increase word count
                    else{
                        word ="";
                        wordSize = 0;
                    }
                }//end if end of word

            }

        }//if(sentiment == 0
        else{//if positive sentiment
            DSString word("");
            int wordSize = 0;
            for(int i = 0; i < holdString.getLength(); i++){
                if( (holdString.find("http")>=0)){
                    int linkAt = holdString.find("http");
                    holdString = holdString.substring(linkAt+4,holdString.getLength()-3-linkAt);
                }
                if (wordSize > 13){
                    word = "";
                }

                if(holdString.substring(i,1) != " ") {
                    if(isalpha(holdString[i]))
                        holdString[i] = tolower(holdString[i]);
                    word = word + holdString.substring(i, 1);
                    wordSize++;
                }//if not end of word
                else{//else if end of word, add word to good word list
                    if(good.count(word)==0 && wordSize>4) {// if word not in list yet, add to list
                        good.emplace(word, 1);
                        word ="";
                        wordSize = 0;
                    }
                    else if((good.count(word)!=0 && wordSize>4)){//increase count of word
                        int count = good.at(word)+1;
                        good.erase(word);
                        good.emplace(word,count);

                        word ="";
                        wordSize = 0;
                    }//end increase word count
                    else{
                        word ="";
                        wordSize = 0;
                    }
                }//end if end of word
            }
        }


    }//read until end of file
    inFS.close();
}//end configData
/**
 *
 * @param gTweet vector of good tweets post analyzing
 * @param bTweet vector of bad tweets post analyzing
 * @param good list of good words
 * @param bad list of bad words
 * @param file to read from
 *
 * Reads in unlabeled tweets from a file and puts them into tweet objects,
 * based on count of positive words and negative words and how positive or negative those words are,
 * adds tweet to a vector of positve or negative tweets
 */
void testData(vector<Tweet>& gTweet, vector<Tweet>& bTweet, map<DSString,int>& good, map<DSString,int>& bad,DSString file){
    //cout << "In test data method" << endl;
    ifstream inFS;
    int holdInt;
    long int holdId;
    DSString holdString;
    char* temp = new char [10000];
    Tweet test;
    inFS.open(file.c_str());
    if(!inFS.is_open()){
        //cout << "testData: Failed to open: " << file << endl;
    }//failed to open config file
    inFS >> temp;//reads column headers
    while(!inFS.eof()){//read in data until end of file

        inFS.getline(temp, 1000,',');
        holdId = atol(temp);

        test.setId(holdId);

        inFS.getline(temp, 1000,',');//read date

        test.setDate(temp);

        inFS.getline(temp, 1000, ',');//read query

        inFS.getline(temp, 1000, ',');//read user
        test.setUsername(temp);

        inFS.getline(temp, 1000);//read message
        holdString = temp;
        test.setMessage(holdString);

        test.setMessage(holdString);

        if(holdString.find("д")>0||holdString.find("и")>0||holdString.find("л")>0){//gets rid of foreign
            holdString = "";
        }

        int wordSize = 0;

        int negativeScore = 0;
        int positiveScore = 0;
        DSString word("");
        for(int i = 0; i < holdString.getLength(); i++) {

            if( (holdString.find("http")>=0)){//breaks up URLs
                int linkAt = holdString.find("http");

            }
            if (wordSize > 13){//breaks up big words that are usually emails or links
                word = "";
            }


            if (holdString.substring(i, 1) != " ") {
                if(isalpha(holdString[i]))
                    holdString[i] = tolower(holdString[i]);
                word + holdString.substring(i, 1);
                wordSize++;
            }//if not end of word
            else {//else if end of word, change score

                if((wordSize>4)&&(wordSize<20)) {
                    if(bad.count(word)>0)
                        negativeScore += bad.at(word);
                    if(good.count(word)>0)
                        positiveScore += (good.at(word));//accounts for the fact that more good words end up in negative tweets

                }//wordsize>3 add to score to avoid including trivial words
                word = "";
                wordSize = 0;
            }//end if end of word
        }//read until end of message for each tweet

        //cout<< "P Score: " << positiveScore << endl;
        //cout << "N Score: " << negativeScore << endl;
        srand(time(0));
        if(holdString.getLength()>2) {
            if (positiveScore > negativeScore) {
                test.setSentiment(4);
                gTweet.push_back(test);
                //cout << "pos" << endl;
            }
            else if (negativeScore > positiveScore*1.2) {
                test.setSentiment(0);
                bTweet.push_back(test);
                //cout << "neg" << endl;
            }
            else {//if good score and bad score are equal, flip a coin
                int guess = rand() % 2;
                //cout << "Guess? " << guess << endl;
                if (guess == 0) {
                    test.setSentiment(4);
                    gTweet.push_back(test);
                }
                else {
                    test.setSentiment(0);
                    bTweet.push_back(test);
                }
            }
        }
    }//read until end of file
    inFS.close();
}//end test Data
/**
 *
 * @param gTweet vector of tweets that the machine deemed "good" (positive)
 * @param bTweet vector of tweets that the machine deemed "bad" (negative)
 * @param file takes a csv of tweet sentiments and id data
 * @return percent correct as a decimal; 1 is all correct, 0 is none correct
 * Reads in data file of correct sentiments for the tweets that were previously tested
 */
double checkData(vector<Tweet>& gTweet, vector<Tweet>& bTweet,DSString file, DSString output, vector<int>& wrongClassifications){
    double accuracy = 0.0;
    int sentiment;
    long int id;
    char* temp = new char [10000];;
    int correctCount = 0;
    int totalCount = 0;
    DSString holdString;
    ifstream inFS;
    map<long int,int> correctData;
//read in correct sentiments and ids into a map
    inFS.open(file.c_str());
    if(!inFS.is_open()){
        cout << "checkData: Failed to open: " << file << endl;
    }//failed to open file
    cout << "checkData: Opened File: " << file << endl;
    inFS >> temp;
    while(!inFS.eof()){
        inFS.getline(temp, 10000,',');
        sentiment = atoi(temp);
        inFS.getline(temp, 10000);
        id = atol(temp);

        correctData.emplace(id,sentiment);
    }//read until end of file
//check if each tweet in good tweets is correct

    for(Tweet item : gTweet) {
        id = item.getId();
        //sentiment = item.getSentiment();
        sentiment = 4;// from good tweet list so we know the sentiment is 4

        if (correctData.count(id) > 0) {//makes sure map has that id so it doesn't throw out of bounds exception
            //cout << "Positive tweet id: " << id << ", correct sentiment: " << correctData.at(id) << endl;
            if (correctData.at(id) == sentiment) { //if the tweet has the correct sentiment increase count of correct tweets
                correctCount++;
            }
            else {
                wrongClassifications.push_back(id);

            }
            totalCount++;
        }

    }//for each tweet object in gTweet vector
//check if each tweet in bad tweets is correct

    for(Tweet item : bTweet){
        id = item.getId();
        //sentiment = item.getSentiment();
        sentiment = 0;//from bad tweet list so we know the sentiment is 0
        if(correctData.count(id)>0) {//makes sure map has that id so it doesn't throw out of bounds exception
            //cout << "Negative tweet id: " << id << ", correct sentiment: " << correctData.at(id) << endl;
            if (correctData.at(id) ==sentiment) {//if the tweet has the correct sentiment increase count of correct tweets
                correctCount++;
            }
            else {
                wrongClassifications.push_back(id);

            }
            totalCount++;
        }

    }//for each tweet object in gTweet vector
    inFS.close();
    accuracy = (correctCount*1.0)/(totalCount*1.0);
    ofstream outFS;
    outFS.open(output.c_str());
    cout << "opening output file: " << output;
    outFS << fixed << setprecision(3)<< accuracy << endl;
    for(int item: wrongClassifications){
        outFS << item << endl;
    }
    outFS.close();
    return accuracy;
}//end checkData