//
// Created by robot on 2/8/2021.
//
#include "catch.hpp"
#include "DSString.h"

TEST_CASE("DSStrings can be compared", "[DSString]")
{
    DSString string1("testString");
    //SECTION 1
    SECTION("Check if string1 == string2" ) {
        DSString string2("testString");
        REQUIRE((string1 == string2));
        REQUIRE((string1 == "testString"));
    }//end section 1
        //SECTION 2
    SECTION("Check if string1 != string2" ) {
        DSString string2("notTestString");
        REQUIRE((string1 != string2));
        REQUIRE((string1 != "notTestString"));
    }//end section 2
        //SECTION 3
    SECTION("Check if string1 > string2"){
        DSString string2("a");
        CHECK((string1>string2));
        CHECK((string1>"a"));
    }//end of section3
        //SECTION 4
    SECTION("Check if string2 < string1"){
        DSString string2("a");
        CHECK((string2<string1));
        CHECK((string2<"testString"));
    }//end of section 4
        //SECTION 5
    SECTION("Check [] operator's ability to access"){
        CHECK((string1[0]=='t'));
    }//end of section 5

}//TEST_CASE 3


TEST_CASE("DSStrings can be set equal or copied", "[DSString]")
{
    DSString string1 = "";
    //SECTION 1
    SECTION("DSStrings can = a string literal or another DSString" ) {
        DSString string2;
        string2 = " equal a string literal";
        CHECK((string2== " equal a string literal"));
        string1 = string2;
        CHECK((string1== " equal a string literal"));
    }//end section1
        //SECTION 2
    SECTION("DSStrings can be copied via copy constructor" ) {
        string1 = "copy to string 2";
        DSString string2(string1);
        CHECK((string2 == "copy to string 2"));
    }// end section 2
}//TEST_CASE 2

TEST_CASE("DSStrings can be operated on", "[DSString]")
{
    DSString string1("string1");
    DSString string2("string2");
    SECTION("DSStrings can be concatenated w/ +" ) {
        DSString string3 = (string1+string2);
        CHECK((string3 == "string1string2" ));
        string3 = (string1+"string2");
        CHECK((string3 == "string1string2" ));
    }
    SECTION("DSStrings can be concatenated w/ +="){
        string1 += string2;
        CHECK((string1 == "string1string2"));
        string1 = "string1";
        string1 += "string2";
        CHECK((string1 == "string1string2"));
    }


}//TEST_CASE 3

TEST_CASE("DSStrings can be searched, accessed, etc...") {
    SECTION("DSStrings can have another string found") {
        DSString string1("find cat at a mat");
        CHECK((string1.find("at") == 6));
        DSString string2("at");
        CHECK((string1.find(string2) == 6));
    }
    SECTION("DSStrings can have a substring extracted"){
        DSString string1 ("mystring");
        CHECK((string1.substring(0,2) == "my"));
    }

}//TEST_CASE 4