# About
Metaheuristic implementation for the travelling salesman problem

# Compiling
run `make`

# Running
Use `./main.out ./instances/instance-name.tsp`

# Test Results

The table below shows the average cost and time (in seconds) of each instance
after running this code 10 times in an i7-7700HQ

The best known solutions of each instance area available at [https://elib.zib.de/pub/mp-testdata/tsp/tsplib/stsp-sol.html](https://elib.zib.de/pub/mp-testdata/tsp/tsplib/stsp-sol.html)

| Instance | Avg. Time | Avg. Cost | Instance | Avg. Time | Avg. Cost |
--- | --- | ---| --- | --- | ---|
a280 | 88.184 | 2579 | kroA150 | 8.87437 | 26524 |
ali535 | 1642.12 | 202426 | kroA200 | 35.6782 | 29368 |
att48 | 0.212311 | 106280 | kroB100 | 3.1413 | 22141 |
att532 | 1578.74 | 27733.3 | kroB150 | 8.47341 | 26130 |
bayg29 | 0.0342724 | 1610 | kroB200 | 39.685 | 29437.1 |
bays29 | 0.0408951 | 2020 | kroC100 | 2.76246 | 20749 |
berlin52 | 0.271315 | 7542 | kroD100 | 3.1444 | 21294 |
bier127 | 8.89953 | 118282 | kroE100 | 3.01474 | 22068 |
brazil58 | 0.354373 | 25395 | lin105 | 3.50649 | 14379 |
brg180 | 10.0927 | 1950 | lin318 | 165.208 | 42053.9 |
burma14 | 0.0037 | 3323 | linhp318 | 163.733 | 42063.5 |
ch130 | 9.16257 | 6110 | pcb442 | 610.018 | 50893.6 |
ch150 | 7.81715 | 6528 | pr107 | 3.82949 | 44303 |
d198 | 34.2086 | 15780 | pr124 | 5.07624 | 59030 |
d493 | 1234.73 | 35037.5 | pr136 | 11.1224 | 96772 |
dantzig42 | 0.120156 | 699 | pr144 | 8.74354 | 58537 |
eil101 | 3.76311 | 629 | pr152 | 6.41037 | 73682 |
eil51 | 0.301137 | 426 | pr226 | 46.6397 | 80369 |
eil76 |  1.2416 | 538 | pr264 | 54.6146 | 49135 |
fl417 | 349.768 | 11861 | pr299 | 112.288 | 48191.6 |
fri26 | 0.0241379 | 937 | pr76 | 1.01781 | 108159 |
gil262 | 90.9447 | 2378.7 | rat195 | 32.8753 | 2323.8 |
gr120 | 7.27192 | 6942 | rat99 | 3.34251 | 1211 |
gr137 | 9.94779 | 69853 | rd100 | 3.3137 | 7910 |
gr17 | 0.0067802 | 2085 | rd400 | 538.509 | 15306.4 |
gr202 | 41.2612 | 40160.3 | si175 | 13.405 | 21407 |
gr21 | 0.0111119 | 2707 | si535 | 875.541 | 48465.1 |
gr229 | 66.6425 | 134606 | st70 | 0.875265 | 675 |
gr24 | 0.019501 | 1272 | swiss42 | 0.11726 | 1273 |
gr431 | 763.712 | 171615 | ts225 | 28.2625 | 126643 |
gr48 | 0.235172 | 5046 | tsp225 | 52.717 | 3916.3 |
gr96 | 2.68829 | 55209 | u159 | 8.676 | 42080 |
hk48 | 0.242406 | 11461 | ulysses16 | 0.0060382 | 6859 |
kroA100 | 2.84878 | 21282 | ulysses22 | 0.013806 | 7013 |

# Used Code
`readData.cpp` and `readData.hpp` taken from: [https://github.com/cvneves/kit-opt](https://github.com/cvneves/kit-opt)

# References
- Marcos Melo Silva, Anand Subramanian, Thibaut Vidal, Luiz Satoru Ochi.
A simple and effective metaheuristic for the Minimum Latency Problem,
European Journal of Operational Research,
2012,
ISSN 0377-2217,
[https://doi.org/10.1016/j.ejor.2012.03.044](https://doi.org/10.1016/j.ejor.2012.03.044).
