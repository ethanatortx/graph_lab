#include <string>
#include "graph.h"

int main()
{
	graph<std::string> lol;
	lol.add_node("Ethan");
	lol.add_node("James");
	lol.add_node("Horace");

	lol.add_edge("Ethan", "James", 3.0);
	lol.add_edge("James", "Horace", 2.0);
	lol.add_edge("Ethan", "Horace", 6.0);

	dijkstra(lol, "Ethan", "Horace");
}