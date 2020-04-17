#!/bin/sh
#########################################
#This script is for CS444 GRID assignment
#
#Run tests for time vs thread number      
#Written by Wenjin HU @clarkson.edu
#on 2/11/09
#
#You are encourged to modify or improve it
#########################################

#The grid size is fixed at 10*10
gridSize=10

#The thread number accessed to the Grid size changes from 1 to 10
threadNr=1

###########################################################
#each test will repeat 5 times
#the result time will be calculated by means and variance
#means will show the average of time usage
#variance will show the validation of running results
###########################################################
repeatTimes=0

#For the test, it will be run at different granularity
#First at none level, then cell level, next row level, last is grid level
lockTypes="none cell row grid"

        ###############################################################
        ###############################################################
        #create and clean the file named raw.data
        #All the program display information will be stored in raw.data.thread
        #Each test's display inforamtion is temporarily stored in raw.data
        #If something wrong with the script, go to check raw.data.thread

        touch raw.data
        touch raw.data.thread


        ###############################################################
        ###############################################################



        ####################################
        #for loop for different granularity
        # none cell row grid
        ####################################

	#for loop for different granularity
	for granularity in $lockTypes
	do
               #inner loop for different thread numbers from 1 to 10
                echo "at $granularity granularity"
 		echo "------------------------------------------------"
		threadNr=1	

                #################################################
                #loop for trying different thread number accesses  
                #################################################
 		until [ $threadNr = 11 ]
  		do
                	echo "Grid: $gridSize Thread: $threadNr Test"


                        ###############################
                        #repeate 5 times for each times
                        ###############################			
			repeatTimes=0
			until [ $repeatTimes = 5 ]
                        do
                                ################################
                                #Run the program
                                #Display is saved in raw.data.thread
                                #################################
  				./gridapp $gridSize $threadNr -$granularity > raw.data
                cat raw.data >> raw.data.thread
				cat raw.data | grep DATA 
				cat raw.data | grep Secs
				repeatTimes=$((repeatTimes+1))
			done
			echo "***********************************************"
			threadNr=$((threadNr+1))
		done
 	        echo "***********************************************"
	done
