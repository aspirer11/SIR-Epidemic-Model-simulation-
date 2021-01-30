import matplotlib.pyplot as plt
file1 = open("output.txt","r")

d,S,I,R = [],[],[],[]

for line in file1:
	values = [float(s) for s in line.split()]
 	d.append(values[0])
 	S.append(values[1])
 	I.append(values[2])
 	R.append(values[3])
    
plt.plot(d, S, label='Suspected', color='blue')
plt.plot(d, I, label='Infected', color='red')
plt.plot(d, R, label='Recovered', color='green')

plt.xlabel('Day')
plt.ylabel('Number of people')
plt.title('Covid-19 Infection Curve')
plt.legend()

plt.show()
