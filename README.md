# C progarm simulation of Covid19 like epidemic 

From a population of maximum 10000 people with atmost 3000 connections (implemented using directed graph), randomly selected people are first infected with the disease.\ 
Then using probability we compute the number of days after which its neighbours will get infected (provided they are susceptible to disease) and number of days after which infected people will recover.\
Once recovered, a person is not susceptible anymore.

Separate linked lists for suspected, infected and recovered people are maintained.\
Events of infection and recovery with time are stored in a priority queue.\
This is performed for a maximum of 300 days. 

Using matplotlib library in Python, infection curves are obtained.
