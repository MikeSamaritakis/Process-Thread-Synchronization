###################################################
#
# Makefile by Michail Samaritakis csd4529
# HY-345 Assignement 2
#
####################################################

# This is for last year's assignment (2022)
2022: 4529_2022.c
	gcc -Wall -pthread -o 4529_2022 4529_2022.c

# This is for this year's assignment (2023)   
all: 4529.c
	gcc -Wall -pthread -o 4529 4529.c
    
sia: assignment2-sia.c
	gcc -Wall -pthread -o sia assignment2-sia.c

clean:
	rm -rf 4529_2022 4529 


