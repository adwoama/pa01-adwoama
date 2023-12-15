# Spring 2021 PA01 - Sentiment Analysis

## Important Information:

Project handout:  [Click Here](https://docs.google.com/document/d/1JohQZiMtnEBWZe_EEkmPz6tTSQG0EPM8xO45ThvgRRk/edit?usp=sharing)

## Input and Output Files

As you begin using input files and output files with this project, 
you'll need to modify the `CMakeLists.txt` file and the `.github/workflows/build.yaml`
files. 

Place any input files in the folder that contains your source code.  When your project is built, it will be copied into the build directory by CMake. 

- `CMakeLists.txt`
    - Open it and look for the commented section (with `#` signs).  Edit the `set(input01 "")` etc.
    based upon how many files you need.
    - there should be one `set(...)` statement per input file
    - you DO NOT need to list output files in `CMakeLists.txt`.
    
- `.github/workflows/build.yaml`
    - Open this file and look for the `INPUT_FILES:` command near the top.
    - List the input file names in the order they should be sent to argv of main. 
    - OUTPUT file need to be listed here as well in the `OUTPUT_FILES:` section. 
    - For example:
        ```yaml
        INPUT_FILES: train_data.csv test_data.csv test_sentiment.csv
        OUTPUT_FILES: output01.txt
        ```
      would be equivalent to the following execution command:
      ```yaml
      ./executableName train_data.csv test_data.csv test_sentiment.csv output01.txt
      ```   
    
