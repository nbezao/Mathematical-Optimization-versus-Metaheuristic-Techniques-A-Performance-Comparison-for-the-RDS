# Mathematical-Optimization-versus-Metaheuristic-Techniques-A-Performance-Comparison-for-the-RDS
The iterative backward/forward power flow algorithm is implemented in the C++ programming language,
whose execution can be characterized and divided by the following steps:

* Read network data and initial conditions such as voltage at the root bus (substation bus);
* Sort the buses into layers according to their connectivity distance to the substation;
* The Backward power sweep process, where the strategy traverses the system through the branches of the
direction of the final nodes located in the last layers until the first layer arriving at the root node,
also calculates the losses.
* The Forward scanning process, describes the process going through the system from the root node in the 
first layer to the last node in the last layer, calculating the voltages of the network nodes.
* The evaluation of the algorithm's stop criterion is determinate by the programmers, which can be satisfied
when it reaches convergence criterion or when a limit number of iterations the pre-established is reached.
The description of the sweep flow steps can be done in detail in "flow-PAPER.cpp, flow_definition-PAPER.cpp,
  flow_definition-PAPER.h, flow_memory-PAPER.cpp, and flow_memory-PAPER.h".

########################################################################################

