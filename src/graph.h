#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <list>
#include <string>
#include <iterator>
#include <iostream>
#include <utility>
#include <set>
using namespace std;


enum STATUS {
	S_START,
	S_CONTINUE,
	S_FINISH
};

class Vertex {
public:
	Vertex(const string &name_,STATUS status_ = S_CONTINUE);
	friend std::ostream &operator<<(std::ostream &os,const Vertex& vertex);
private:
	string name;
	STATUS status;
	int state;
	static int count;
	bool isState(const STATUS& s) const;
public:
	Vertex(const Vertex&);
	Vertex& operator=(const Vertex& other);
	bool operator==(const Vertex&other) const;
	bool operator==(int value) const;
	bool operator!=(const Vertex&other) const;
	operator int() const;
	bool isFinish() const;
	bool isContinue() const;
	bool isStart() const;
};


struct Edge {
	Vertex *start;
	Vertex *end;
	double cost;
	string input;
	
	Edge(Vertex *start_,Vertex *end_,const string& input_,double cost_=0.0);
	friend std::ostream &operator<<(std::ostream &os,const Edge& edge);

	Edge(const Edge&);
	Edge& operator=(const Edge&other);
	bool operator==(const Edge &other) const;
        bool operator==(const pair<pair<int,int>,string> &other) const;
	bool operator!=(const Edge &other) const;
	bool isSelf() const;
};


class Graph {
	typedef std::set<Edge *> edge_type;
	typedef std::set<Vertex *> vertex_type;
	typedef std::list<std::string> symbol_type;
	edge_type edges;
	vertex_type vertices;
	string name;
	Vertex *start;
	vertex_type finish;
	symbol_type symbols;
	struct TEdge {
		vertex_type start;
		vertex_type end;
		string input;
	};
public:
	Graph(const string &n);
	void addVertex(Vertex *vertex);
	void removeVertex(Vertex *vertex);
	void addEdge(Edge *edge);
	void removeEdge(Edge *edge);

	template <typename F>
	void getEdges(edge_type &e,F fn);
	
	template <typename F>
	void getVertices(vertex_type &e,F fn);

	typedef std::set<Edge*>::iterator edge_iterator;
	typedef std::set<Vertex*>::iterator vertex_iterator;
	
	edge_iterator edge_begin()  {return edges.begin();}
	edge_iterator edge_end()  {return edges.end();}
	vertex_iterator vertex_begin()  {return vertices.begin();}
	vertex_iterator vertex_end()  {return vertices.end();}

	size_t edge_size() const {return edges.size();}
	size_t vertex_size() const {return vertices.size();}

	Graph(const Graph &);
	Graph& operator=(const Graph& other);
	bool operator==(const Graph &other) const;
	bool operator!=(const Graph &other) const;

	bool existVertex(Vertex *v);
	bool existEdge(Edge *e);
	bool isFinish(Vertex *v);
	bool isAcceptSym(const std::string &s);
	bool isNFA();
	bool DFAMatch(const std::string &str);
	vertex_type closure(const vertex_type &);
	void closure(const std::string &str,const vertex_type &svt,vertex_type &dvt);
	void once_closure(const std::string &str,const vertex_type &svt,vertex_type &dvt);
	bool toDFA(Graph &g);

	friend std::ostream &operator<<(std::ostream &os,const Graph& g);
	friend void newGraph(const vertex_type& nstart,const std::list<vertex_type *> &nlist,
						 const std::list<TEdge> &nedges,const std::list<vertex_type *> &nfinish,
						 Graph &ng);

private:
	template <typename T,typename F>
	void get(const std::set<T> &l,std::set<T> &r,F fn);

	bool beginWith(const std::string& dst,const string &src) const;
	bool containFinish(const vertex_type& vertices) const;

	
};

#endif
