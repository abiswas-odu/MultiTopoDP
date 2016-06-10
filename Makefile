topoDP: main.o dirFile.o SequenceNode.o StickNode.o Geometry.o Residue.o Atom.o Point.o Ultility.o GraphNode.o Set.o Path.o Vectors.o MRC.o Record.o Graph.o
	icpc -O -o topoDP GraphUpdate4/dirFile.o GraphUpdate4/SequenceNode.o GraphUpdate4/StickNode.o GraphUpdate4/Geometry.o GraphUpdate4/Residue.o GraphUpdate4/Atom.o GraphUpdate4/Point.o GraphUpdate4/Ultility.o GraphUpdate4/GraphNode.o GraphUpdate4/Set.o GraphUpdate4/Path.o GraphUpdate4/Vectors.o GraphUpdate4/MRC.o GraphUpdate4/Record.o GraphUpdate4/Graph.o GraphUpdate4/main.o 
Geometry.o: GraphUpdate4/Geometry.cpp
	icpc -c -o GraphUpdate4/Geometry.o GraphUpdate4/Geometry.cpp
Vectors.o: GraphUpdate4/Vectors.cpp
	icpc -c -o GraphUpdate4/Vectors.o GraphUpdate4/Vectors.cpp
MRC.o: GraphUpdate4/MRC.cpp
	icpc -c -o GraphUpdate4/MRC.o GraphUpdate4/MRC.cpp
Graph.o: GraphUpdate4/Graph.cpp
	icpc -c -o GraphUpdate4/Graph.o GraphUpdate4/Graph.cpp
Record.o: GraphUpdate4/Record.cpp
	icpc -c -o GraphUpdate4/Record.o GraphUpdate4/Record.cpp
Set.o: GraphUpdate4/Set.cpp
	icpc -c -o GraphUpdate4/Set.o GraphUpdate4/Set.cpp
Path.o: GraphUpdate4/Path.cpp
	icpc -c -o GraphUpdate4/Path.o GraphUpdate4/Path.cpp
GraphNode.o: GraphUpdate4/GraphNode.cpp
	icpc -c -o GraphUpdate4/GraphNode.o GraphUpdate4/GraphNode.cpp
Ultility.o: GraphUpdate4/Ultility.cpp
	icpc -c -o GraphUpdate4/Ultility.o GraphUpdate4/Ultility.cpp
Residue.o: GraphUpdate4/Residue.cpp
	icpc -c -o GraphUpdate4/Residue.o GraphUpdate4/Residue.cpp
Atom.o: GraphUpdate4/Atom.cpp
	icpc -c -o GraphUpdate4/Atom.o GraphUpdate4/Atom.cpp
Point.o: GraphUpdate4/Point.cpp
	icpc -c -o GraphUpdate4/Point.o GraphUpdate4/Point.cpp
StickNode.o: GraphUpdate4/StickNode.cpp
	icpc -c -o GraphUpdate4/StickNode.o GraphUpdate4/StickNode.cpp
SequenceNode.o: GraphUpdate4/SequenceNode.cpp
	icpc -c -o GraphUpdate4/SequenceNode.o GraphUpdate4/SequenceNode.cpp
dirFile.o: GraphUpdate4/dirFile.cpp
	icpc -c -o GraphUpdate4/dirFile.o GraphUpdate4/dirFile.cpp
main.o: GraphUpdate4/main.cpp
	icpc -c -o GraphUpdate4/main.o GraphUpdate4/main.cpp
clean:
	rm -rf *~ ./other GraphUpdate4/*.o topoDP
