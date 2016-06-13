# MultiTopoDP

## Installation and Execution 

The project is available on GitHub at https://github.com/abiswas-odu/MultiTopoDP

### Prerequisites 

You will need the GNU C++ compiler installed on any Linuc distribution.
 
### Steps

Clone the project from the git repo and run make using the command below:
	git pull https://github.com/abiswas-odu/MultiTopoDP.git
	make
A executable with the name "topoDP" will be created.
Test installation 
	./topoDP TestInput/1ICX/parameter TestInput/1ICX/trueTopology
A directory named other will be created will the output files.

### Execution

Prepare a parameter file and a true topology file. See the example in the TestInput directory.  
Change only the first 4 parameters in the parameter file. 

Run the command:
	<path>/topoDP parameter trueTopology
