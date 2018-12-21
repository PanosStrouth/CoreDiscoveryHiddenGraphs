In this repository you can find 2 C++ implementations for discovering TOP-K degreeS and K-Cores in a Hidden Network.
Apart from the source code there are also the prebuilt binaries so that you can directly use/test the algorithms.
You can follow the examples shown below:


GSOE
GSOE is an algorithm that, given an undirected hidden graph and an integer K , discovers the top-K degree nodes without performing all probings among graph nodes.
For using our GSOE implementation you have to navigate to the CoreDiscoveryHiddenGraphs/GSOE folder and use the GSOE executable file. The GSOE executable can be used as follows:
GSOE undirectedGraphFile K

There's already a small undirected graph in the GSOE directory , called "testgraph.txt". So for example, in order to find the node with the highest degree in this graph (K=1) you should type:
GSOE testgraph.txt 1
