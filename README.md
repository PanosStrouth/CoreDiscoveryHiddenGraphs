In this repository you can find 2 C++ implementations for discovering TOP-K degreeS and K-Cores in a Hidden Network.
Apart from the source code there are also the prebuilt binaries so that you can directly use/test the algorithms.
You can follow the examples shown below:


## GSOE  
GSOE is an algorithm that, given an undirected hidden graph and an integer K , discovers the top-K degree nodes without performing all probings among graph nodes.
For using our GSOE implementation you have to navigate to the CoreDiscoveryHiddenGraphs/GSOE folder and use the GSOE executable file. The GSOE executable can be used as follows:  
```bash
GSOE undirectedGraphFile K
```  
There's already a small undirected graph in the GSOE directory , called "testgraph.txt". So for example, in order to find the node with the highest degree in this graph (K=1) you should type in your terminal:  
```bash
./GSOE testgraph.txt 1
```  
You will see the following:  
```bash
==========Results==========
Here's the top-1 list:
2
Top-1 nodes found after 10 probings
Maximum number of probings: 15
```  
This means that the algorithm needs 10 probings. Since the testgraph has 6 nodes the maximum number of probings is  
(6 * 5) / 2 = 15 . As a result the algorithm has a 5 probings gain!  


## HiddenCore  
HiddenCore is an algorithm, that given an undirected hidden graph and an integer K, returns true of false in case there is or there is not a K-Core in the hidden network respectively.
For using our HiddenCore implementation you have to navigate to the CoreDiscoveryHiddenGraphs/HiddenCore folder and use the HiddenCore executable file. The HiddenCore executable can be used as follows:  
```bash
HiddenCore undirectedGraphFile K
```  
There's already a small undirected graph in the HiddenCore directory , called "testgraph.txt". So for example, in order to check if there's a 3-Core (K==3) in the graph file open a terminal and type:  
```bash
./HiddenCore testgraph.txt 3
```  
You will see the following:  
```bash
Core 3 not found after 12 probings!
``` 
This means that the algorithm needs 12 probings to identify that there's no 3-Core in the graph. Since the testgraph has 6 nodes the maximum number of probings is (6 * 5) / 2 = 15 . As a result the algorithm has a 3 probings gain!  

## Comments:


## Important Note:
