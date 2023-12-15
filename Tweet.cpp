#include "Tweet.h"
#include "DSString.h"
//Constructors
Tweet::Tweet(){
    sentiment = 0;
    id = 0;
    date = "no date";
    username = "no user";
    message = "no message";
}//Tweet()
//set methods
void Tweet::setSentiment(int newSent){
    sentiment = newSent;
}//setSentiment
void Tweet::setId(long int newId){
    id = newId;
}//setId
void Tweet::setDate(DSString newDate){
    date = newDate;
}//setDate
void Tweet::setUsername(DSString newUser){
    username = newUser;
}//setUsername
void Tweet::setMessage(DSString newMess){
    message = newMess;
}//setMessage
//get methods
int Tweet::getSentiment(){
    return sentiment;
}//getSentiment
long int Tweet::getId(){
    return id;
}//getId
DSString Tweet::getDate(){
    return date;
}//getDate
DSString Tweet::getUsername(){
    return username;
}//getUsername
DSString Tweet::getMessage(){
    return message;
}//getMessage