#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include "shortestPath.h"
#include "list.h"

template<class T>
class graph_node
{
public:
	struct edge {
		edge() { dest = nullptr; cost = 0.0; }
		edge(graph_node<T>* _end, double _cost) { dest = _end; cost = _cost; }
		
		graph_node<T>* dest;
		double cost;
	};
	
	// default
	graph_node();
	// construct from name
	graph_node(const T& nm);
	// check if edge exists
	bool is_edge(graph_node<T>* end);
	bool is_edge(graph_node<T>* end, double cost);
	// add edge
	bool add_edge(const edge& e);
	bool add_edge(graph_node<T>* end, double cost);
	// remove edge
	bool erase_edge(graph_node<T>* end);
	bool erase_edge(graph_node<T>* end, double cost);

	// clear edges
	void clear();

	// return and assign name
	T& name() const;
	void name(const T& nm);
	// return adjacent nodes
	list<edge>& adjacent() const;
	// get count of adjacent nodes
	unsigned adj_count() const;

private:
	T _name;
	list<edge> _edges;
};

// default constructer
template<class T>
graph_node<T>::graph_node()
{}

/* construct from name/data */
template<class T>
graph_node<T>::graph_node(const T& nm)
{ _name = nm; }

/*
	is_edge(graph_node<T>*)
		check if edge with destination already exists
		return true if it is adjacent
*/
template<class T>
bool graph_node<T>::is_edge(graph_node<T>* end)
{
	for(typename list<edge>::const_iterator i = _edges.cbegin(); i != _edges.cend(); ++i)
		if(i->dest == end) return 1;
	return 0;
}

/*
	is_edge(graph_node<T>*, double)
		check if edge with destination and cost already exists
		return true if it is adjacent
*/
template<class T>
bool graph_node<T>::is_edge(graph_node<T>* end, double cost)
{
	for(typename list<edge>::const_iterator i = _edges.cbegin(); i != _edges.cend(); ++i)
		if((i->dest == end) && (i->cost == cost)) return 1;
	return 0;
}

/*
	add_edge(const edge&)
		add edge from already existing edge
		return true if add succeeds
*/
template<class T>
inline bool graph_node<T>::add_edge(const edge& e)
{ return add_edge(e.dest, e.cost); }

/*
	add_edge(graph_node<T>*, double)
		add edge leading to node pointer with cost
		return true if add succeeds
*/
template<class T>
inline bool graph_node<T>::add_edge(graph_node<T>* end, double cost)
{
	if(!is_edge(end, cost))
	{
		_edges.push_back(edge(end, cost));
	}
}

/*
	erase_edge(graph_node<T>*)
		remove edge leading to provided node pointer
		return true if removal succeeds
*/
template<class T>
bool graph_node<T>::erase_edge(graph_node<T>* end)
{
	typename list<edge>::iterator i;
	for(i; i != _edges.end(); ++i)
	{
		if(i->dest == end)
		{
			_edges.erase(i);
			return 1;
		}
	}
	return 0;
}

/*
	erase_edge(graph_node<T>*, double)
		remove edge leading to provided node pointer with cost
		return true if removal succeeds
*/
template<class T>
bool graph_node<T>::erase_edge(graph_node<T>* end, double cost)
{
	typename list<edge>::iterator i;
	for(i; i != _edges.end(); ++i)
	{
		if((i->dest == end) && (i->cost == cost))
		{
			_edges.erase(i);
			return 1;
		}
	}
	return 0;
}

/*
	clear()
		clear adjacent nodes (erase edges)
*/
template<class T>
void graph_node<T>::clear()
{ _edges.clear(); }

/*
	name()
		return name/data of node
*/
template<class T>
T& graph_node<T>::name() const
{ return _name; }

/*
	name(const T&)
		assign name/data of node
*/
template<class T>
void graph_node<T>::name(const T& nm)
{ _name = nm; }

/*
	adjacent()
		return edges list
*/
template<class T>
list<typename graph_node<T>::edge>& graph_node<T>::adjacent() const
{ return _edges; }
/*
	adj_count()
		return unsigned int representing adjacent nodes count
*/
template<class T>
unsigned graph_node<T>::adj_count() const
{ return _edges.size(); }

/*
	graph class
*/
template<class T>
class graph
{
public:
	// default constructor
	graph();

	// add edge between two nodes
	bool add_edge(const T&, const T&, double);
	// remove edge
	void erase_edge(const T&, const T&);
	void erase_edge(const T&, const T&, double);
	// check if node with name exists
	bool is_node(const T&);
	// add node with name/data
	bool add_node(const T&);
	// erase node with name/data
	void erase_node(const T&);

	// clear graph
	void clear();
	// check if empty
	bool empty() const;
	// return size of graph
	unsigned size() const;

	template<class typeG>
	friend void dijkstra(const graph<typeG>&, const typeG&, const typeG&);
private:
	std::map<T, graph_node<T>*> nodeRef;
};

/*
	default constructor
*/
template<class T>
graph<T>::graph()
{}

/*
	add_edge(const T& orig, const T& dest, double c)
		add edge from orig to dest with cost c
		return true if edge addition is succesful
*/
template<class T>
bool graph<T>::add_edge(const T& orig, const T& dest, double cost)
{
	graph_node<T>* tRef;
	if(nodeRef.find(orig) == nodeRef.end())
	{
		graph_node<T>* n = new graph_node<T>(orig);
		nodeRef[orig] = n;
	}
	if(nodeRef.find(dest) == nodeRef.end())
	{
		tRef = new graph_node<T>(dest);
		nodeRef[dest] = tRef;
	}
	else
	{
		tRef = nodeRef[dest];
	}

	return (nodeRef[orig]->add_edge(tRef, cost));
}

/*
	erase_edge(const T& orig, const T& dest)
		erase edge from node orig to node dest
*/
template<class T>
void graph<T>::erase_edge(const T& orig, const T& dest)
{
	if((nodeRef.find(orig) == nodeRef.end()) || (nodeRef.find(dest) == nodeRef.end()))
	{ return; }
	else
	{
		graph_node<T>* eRef = nodeRef[dest];
		for(typename list<graph_node<T>*>::iterator i = nodeRef[orig]->adjacent().begin(); i != nodeRef[orig]->adjacent().end(); ++i)
		{
			if(i->dest == eRef)
			{
				nodeRef[orig]->adjacent().erase(i);
				return;
			}
		}
	}
}

/*
	erase_edge(const T& orig, const T& dest, double c)
		erase edge from node orig to node dest with cost c
*/
template<class T>
void graph<T>::erase_edge(const T& orig, const T& dest, double cost)
{
	if((nodeRef.find(orig) == nodeRef.end()) || (nodeRef.find(dest) == nodeRef.end()))
	{ return; }
	else
	{
		graph_node<T>* eRef = nodeRef[dest];
		for(typename list<graph_node<T>*>::iterator i = nodeRef[orig]->adjacent().begin(); i != nodeRef[orig]->adjacent().end(); ++i)
		{
			if((i->dest == eRef) && (i->cost == cost))
			{
				nodeRef[orig]->adjacent().erase(i);
				return;
			}
		}
	}
}

/*
	is_node(const T& nm)
		return if node exists
*/
template<class T>
bool graph<T>::is_node(const T& nm)
{
	return (nodeRef.find(nm) != nodeRef.end());
}
/*
	add_node(const T& nm)
		add node with name/data of nm
		return true if insertion is succesful
		insertion fails if node with that name already exists
*/
template<class T>
bool graph<T>::add_node(const T& nm)
{
	if(nodeRef.find(nm) == nodeRef.end())
	{
		graph_node<T>* n = new graph_node<T>(nm);
		nodeRef[nm] = n;
		return 1;
	}
	else
		return 0;
}

/*
	erase_node(const T& nm)
		remove node with name/data
		return true if removal succeeds
*/
template<class T>
void graph<T>::erase_node(const T& nm)
{ nodeRef.erase(nm); }


/*
	clear()
		clear all nodes from graph
*/
template<class T>
void graph<T>::clear()
{
	graph_node<T>* n;
	for(typename std::map<T, graph_node<T>*>::iterator i = nodeRef.begin(); i != nodeRef.end(); ++i)
	{
		n = i->second;
		i->second = nullptr;
		delete n;
	}
	nodeRef.clear();
}

/*
	size()
		returns count of nodes in graph
*/
template<class T>
unsigned graph<T>::size() const
{ return nodeRef.size(); }

/*
	empty()
		returns if graph contains any nodes
*/
template<class T>
bool graph<T>::empty() const
{ return nodeRef.empty(); }

#endif