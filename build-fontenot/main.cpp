#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>

#include "DSString.h"
#include "Tweet.h"

void configData(map<DSString,int>&, map<DSString,int>&,DSString);
void testData(vector<Tweet>&, vector<Tweet>&, map<DSString,int>&, map<DSString,int>&,DSString);
double checkData(vector<Tweet>&, vector<Tweet>&,DSString, vector<int>&);

int testFunc(){
    return Catch::Session().run();
}

int main(int argc, char *argv[]) {
    if(argc == 0){
        testFunc();
        return 0;
    }
    /*
    DSString file1 = argv[0];
    DSString file2 = argv[1];
    DSString file3 = argv[2];
    DSString outputFile;
    map<DSString,int> goodWords;//list of words from definite positive tweets
    map<DSString,int> badWords;//list of words from definite negative tweets
    vector<Tweet> positiveTweets;//computer guess positive tweets
    vector<Tweet> negativeTweets;//computer guess negative tweets
    vector<int> wrongGuesses;//Tweet ids of tweets wrongfully classified by algorithm
    double algAccuracy = 0.0;//percentage of correct classifications done by algorithm

    configData(goodWords, badWords, file1);
    testData(positiveTweets, negativeTweets, goodWords, badWords, file2);
    algAccuracy = checkData( positiveTweets, negativeTweets, file3, wrongGuesses);

    cout << "The accuracy was: " <<algAccuracy;
    //TODO output to output file
    */
    return 0;
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
    char* temp;
    int sentiment;
    int holdInt;
    inFS.open(file.c_str());
    if(!inFS.is_open()){
        cout << "configData: Failed to open: " << file << endl;
    }//failed to open config file
    inFS.getline(temp,500);//reads column headers
    while(!inFS.eof()){//read in data until end of file
        inFS >> sentiment;//read sentiment
        inFS.getline(temp, 280, ',');//remove comma
        inFS >> holdInt;//read id
        inFS.getline(temp, 280, ',');//remove comma
        inFS.getline(temp, 280, ',');//read date
        inFS.getline(temp, 280, ',');//read query
        inFS.getline(temp, 280, ',');//read user
        inFS >> holdString;//read tweet//TODO skip characters you can't read
        if(sentiment == 0) {//if negative sentiment, add words to bad map
            DSString word("");
            for(int i = 0; i < holdString.getLength(); i++){
                if(holdString.substring(i,1) != " ") {
                    word += holdString.substring(i, 1);
                }//if not end of word
                else{//else if end of word, add word to bad word list
                    if(bad.count(word)==0)// if word not in list yet, add to list
                        bad.emplace(word,1);
                    else {//increse count of word
                        int count = bad.at(word)+1;
                        bad.erase(word);
                        bad.emplace(word,count);
                    }//end increase word count
                }//end if end of word
            }
        }//if(sentiment == 0
        else{//if positive sentiment
            DSString word("");
            for(int i = 0; i < holdString.getLength(); i++){
                if(holdString.substring(i,1) != " ") {
                    word += holdString.substring(i, 1);
                }//if not end of word
                else{//else if end of word, add word to good word list
                    if(good.count(word)==0)// if word not in list yet, add to list
                        good.emplace(word,1);
                    else {//increse count of word
                        int count = bad.at(word)+1;
                        good.erase(word);
                        good.emplace(word,count);
                    }//end increase word count
                }//end if end of word
            }
        }
    }//read until end of file
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
    ifstream inFS;
    int holdInt;
    DSString holdString;
    char* temp;
    Tweet test;
    inFS.open(file.c_str());
    if(!inFS.is_open()){
        cout << "testData: Failed to open: " << file << endl;
    }//failed to open config file
    inFS.getline(temp, 280);//reads column headers
    while(!inFS.eof()){//read in data until end of file

        inFS >> holdInt;//read id
        test.setId(holdInt);
        inFS.getline(temp, 280, ',');//remove comma
        inFS.getline(temp, 280, ',');//read date
        holdString = temp;
        test.setDate(holdString);
        inFS.getline(temp, 280, ',');//read query
        inFS.getline(temp, 280, ',');//read user
        holdString = temp;
        test.setUsername(holdString);
        inFS >> holdString;//read tweet
        test.setMessage(holdString);

        int negativeScore = 0;
        int positiveScore = 0;
        for(int i = 0; i < holdString.getLength(); i++) {
            DSString word("");

            if (holdString.substring(i, 1) != " ") {
                word += holdString.substring(i, 1);
            }//if not end of word
            else {//else if end of word, add word to bad word list
                negativeScore += bad.at(word);
                positiveScore += good.at(word);

            }//end if end of word
        }//read until end of message for each tweet
        if(positiveScore > negativeScore)
            gTweet.push_back(test);
        else if(negativeScore > positiveScore)
            bTweet.push_back(test);
        else {//if good score and bad score are equal, flip a coin
            int guess = rand() % 2;
            if (guess == 0)
                gTweet.push_back(test);
            else
                bTweet.push_back(test);
        }
    }//read until end of file
}//end test Data
/**
 *
 * @param gTweet vector of tweets that the machine deemed "good" (positive)
 * @param bTweet vector of tweets that the machine deemed "bad" (negative)
 * @param file takes a csv of tweet sentiments and id data
 * @return percent correct as a decimal; 1 is all correct, 0 is none correct
 * Reads in data file of correct sentiments for the tweets that were previously tested
 */
double checkData(vector<Tweet>& gTweet, vector<Tweet>& bTweet,DSString file, vector<int>& wrongClassifications){
    double accuracy = 0.0;
    int sentiment;
    int id;
    char* temp;
    int correctCount = 0;
    int totalCount = 0;
    DSString holdString;
    ifstream inFS;
    map<int,int> correctData;
//read in correct sentiments and ids into a map
    inFS.open(file.c_str());
    if(!inFS.is_open()){
        cout << "checkData: Failed to open: " << file << endl;
    }//failed to open file
    while(!inFS.eof()){
        inFS >> sentiment;
        inFS.getline(temp, 280, ',');//get rid of comma
        inFS >> id;
        correctData.emplace(id,sentiment);
    }//read until end of file
//check if each tweet in good tweets is correct
    for(Tweet item : gTweet) {
        id = item.getId();
        sentiment = item.getSentiment();
        if (correctData.count(id) > 0) {//makes sure map has that id so it doesn't throw out of bounds exception
            if (correctData.at(id) == sentiment) //if the tweet has the correct sentiment increase count of correct tweets
                correctCount++;
            else
                wrongClassifications.push_back(id);
        }
        totalCount++;
    }//for each tweet object in gTweet vector
//check if each tweet in bad tweets is correct
    for(Tweet item : bTweet){
        id = item.getId();
        sentiment = item.getSentiment();
        if(correctData.count(id)>0) {//makes sure map has that id so it doesn't throw out of bounds exception
            if (correctData.at(id) ==
                sentiment)//if the tweet has the correct sentiment increase count of correct tweets
                correctCount++;
            else
                wrongClassifications.push_back(id);
        }
        totalCount++;
    }//for each tweet object in gTweet vector

    accuracy = (correctCount*1.0)/(totalCount*1.0);
    return accuracy;
}//end checkData