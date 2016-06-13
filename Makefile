topoDP: main.o dirFile.o SequenceNode.o StickNode.o Geometry.o Residue.o Atom.o Point.o Ultility.o GraphNode.o Set.o Path.o Vectors.o MRC.o Record.o Graph.o
	g++ -O3 -o topoDP GraphUpdate4/dirFile.o GraphUpdate4/SequenceNode.o GraphUpdate4/StickNode.o GraphUpdate4/Geometry.o GraphUpdate4/Residue.o GraphUpdate4/Atom.o GraphUpdate4/Point.o GraphUpdate4/Ultility.o GraphUpdate4/GraphNode.o GraphUpdate4/Set.o GraphUpdate4/Path.o GraphUpdate4/Vectors.o GraphUpdate4/MRC.o GraphUpdate4/Record.o GraphUpdate4/Graph.o GraphUpdate4/main.o 
Geometry.o: GraphUpdate4/Geometry.cpp
	g++ -c -o GraphUpdate4/Geometry.o GraphUpdate4/Geometry.cpp
Vectors.o: GraphUpdate4/Vectors.cpp
	g++ -c -o GraphUpdate4/Vectors.o GraphUpdate4/Vectors.cpp
MRC.o: GraphUpdate4/MRC.cpp
	g++ -c -o GraphUpdate4/MRC.o GraphUpdate4/MRC.cpp
Graph.o: GraphUpdate4/Graph.cpp
	g++ -c -o GraphUpdate4/Graph.o GraphUpdate4/Graph.cpp
Record.o: GraphUpdate4/Record.cpp
	g++ -c -o GraphUpdate4/Record.o GraphUpdate4/Record.cpp
Set.o: GraphUpdate4/Set.cpp
	g++ -c -o GraphUpdate4/Set.o GraphUpdate4/Set.cpp
Path.o: GraphUpdate4/Path.cpp
	g++ -c -o GraphUpdate4/Path.o GraphUpdate4/Path.cpp
GraphNode.o: GraphUpdate4/GraphNode.cpp
	g++ -c -o GraphUpdate4/GraphNode.o GraphUpdate4/GraphNode.cpp
Ultility.o: GraphUpdate4/Ultility.cpp
	g++ -c -o GraphUpdate4/Ultility.o GraphUpdate4/Ultility.cpp
Residue.o: GraphUpdate4/Residue.cpp
	g++ -c -o GraphUpdate4/Residue.o GraphUpdate4/Residue.cpp
Atom.o: GraphUpdate4/Atom.cpp
	g++ -c -o GraphUpdate4/Atom.o GraphUpdate4/Atom.cpp
Point.o: GraphUpdate4/Point.cpp
	g++ -c -o GraphUpdate4/Point.o GraphUpdate4/Point.cpp
StickNode.o: GraphUpdate4/StickNode.cpp
	g++ -c -o GraphUpdate4/StickNode.o GraphUpdate4/StickNode.cpp
SequenceNode.o: GraphUpdate4/SequenceNode.cpp
	g++ -c -o GraphUpdate4/SequenceNode.o GraphUpdate4/SequenceNode.cpp
dirFile.o: GraphUpdate4/dirFile.cpp
	g++ -c -o GraphUpdate4/dirFile.o GraphUpdate4/dirFile.cpp
main.o: GraphUpdate4/main.cpp
	g++ -c -o GraphUpdate4/main.o GraphUpdate4/main.cpp
clean:
	rm -rf *~ ./other GraphUpdate4/*.o topoDP
