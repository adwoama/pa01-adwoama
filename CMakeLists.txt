cmake_minimum_required(VERSION 3.14)
project(21S-PA01)
set (EXE_NAME SentimentAnalyzer)

##########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "train_data.csv")
#   set (input02 "train_target.csv")
# etc....

set (input01 "train_dataset_20k.csv")
set (input02 "test_dataset_10k.csv")
set (input03 "test_dataset_sentiment_10k.csv")
set (input04 "")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03 input04)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj")
#add_executable(${EXE_NAME}  main.cpp DSString.h Tweet.cpp Tweet.h )
#add_executable(${EXE_NAME} main.cpp DSString.h test.cpp catch.hpp Tweet.cpp Tweet.h )
add_executable(${EXE_NAME} tests-main.cpp catch.hpp main.cpp DSString.h test.cpp Tweet.cpp Tweet.h )
