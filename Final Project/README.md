# Smart Switch using Eye Tracking and IOT

### INTRODUCTION
This project implements a eyes and fist/palm recognition system using OpenCV on Python 3.5, which is further extended to a sensor interaction environment. The sensors takes input from the recognition engine and activate the actuators depending on neural network logic implemented as part of machine learning. The network aims in understanding and learning the behavior of user activity and use it to directly implement the actuation without any user intervention.

**OS:** Raspbian OS

**Platform:** Python 3.5

**Libraries:** OpenCV 2.4.8, Numpy

**Hardware Requirements:** Camera/Webcam, Sensors (touch, track, temperature, flash LED, RGB LED), Raspberry pi3

### USAGE
Run fistpalm.py to begin the program. This will start the recognition system for detecting eyes & fist/palm. When the same are detected, the program creates two separate output files viz "output_WC.txt" for eyes & "output_HT.txt" and populates with them with either "1" for success & "0" as failed. "Program.c" takes input from these output files and actautes the sensors through the sensor interaction program, implemented during individual project. Program.c also takes input from "output_ML.txt" which instruct it about which state it has to start the LED.

Along with this, we run "Neural.py" which takes input from "input_ML.txt" & from "current_status.txt", reads and writes output in "output_ML.txt" based on the neural network logic which facilitates supervised learning. 

input_ML - Contains training data for neural.py. First three columns - state of environment. Last column state of RGB(1- Red,2 - Green, 3- Blue). 
Program.C writes into file as the user changes preference while changing the LED state while running.
output_ML - Instructs Program.C as to which state to start. neural.py will write into this file based on the learning


### HOW TO RUN
Run all the following commands in parallel:

python fistpalm.py -haarcascade
python neural.py
gcc Program.c

Now, run the compiled file of "Program.c"

Provide gesture input to the webcam to trigger the sensor actuation & the learning algorithm.

**Full explanation with screenshots is provided in project report**
