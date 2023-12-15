//
// Created by robot on 2/7/2021.
//

#ifndef INC_21S_PA01_TWEET_H
#define INC_21S_PA01_TWEET_H
#include "DSString.h"
class Tweet{
private:
    int sentiment;
    int id;
    DSString date;
    DSString username;
    DSString message;
public:
    //Constructors
    Tweet();
    //set methods
    void setSentiment(int);
    void setId(int);
    void setDate(DSString);
    void setUsername(DSString);
    void setMessage(DSString);
    //get methods
    int getSentiment();
    int getId();
    DSString getDate();
    DSString getUsername();
    DSString getMessage();
};
#endif //INC_21S_PA01_TWEET_H
