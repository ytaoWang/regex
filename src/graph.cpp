#include <functional>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <string>
#include <list>
#include <map>

#include "graph.h"


int Vertex::count = 0;

Vertex::Vertex(const string &name_,STATUS status_):name(name_),status(status_)
{
	state = count++;
}

Vertex::Vertex(const Vertex &lhs)
{
	name = lhs.name;
	status = lhs.status;
	state = lhs.state;
}

Vertex& Vertex::operator=(const Vertex& other)
{
	if(this != &other)  {
		
		name = other.name;
		status = other.status;
		state = other.state;
	}

	return *this;
}

bool Vertex::isState(const STATUS &s) const
{
	return status == s;
}

bool Vertex::isStart() const
{
	return isState(S_START);
}

bool Vertex::isFinish() const
{
	return isState(S_FINISH);
}

bool Vertex::isContinue() const
{
	return isState(S_CONTINUE);
}

bool Vertex::operator==(const Vertex&other) const
{
	if(name == other.name && status == other.status && state == other.state)
		return true;
	return false;
}

bool Vertex::operator!=(const Vertex&other) const
{
	return !(*this == other);
}

Vertex::operator int() const
{
	return this->state;
}

std::ostream &operator<<(std::ostream &out,const Vertex& vertex)
{
	out << "Vertex{name=" << vertex.name;
	switch(vertex.status)
		{
		case S_START:
			out << ",status = start";
			break;
		case S_CONTINUE:
			out << ",status = continue";
			break;
		case S_FINISH:
			out << ",status = finish";
				break;
		default:
			break;
		}
	out << ", state=" << vertex.state;
	
	out << "}";

	return out;
}

Edge::Edge(Vertex *start_,Vertex *end_,const string& input_,double cost_):cost(cost_),input(input_)
{
	start = start_;
	end = end_;
}

std::ostream &operator<<(std::ostream &out,const Edge& e)
{
  /**
	if(&e == nullptr) {
		out << "Edge is null.";
		return out;
	}
  **/

	out << "Edge{input=" << e.input << endl;
	if(e.cost != 0.0) {
		out << ", cost=" << e.cost;
	}
	
	out << *(e.start);
	out << *(e.end);
	out << endl;
	out << "}";

	return out;
}

Edge::Edge(const Edge &other)
{
	this->start = other.start;
	this->end = other.end;
	cost = other.cost;
	input = other.input;
}

Edge& Edge::operator=(const Edge &other)
{
	if(this != &other) {
		this->start = other.start;
		this->end = other.end;
		cost = other.cost;
		input = other.input;
	}

	return *this;
}

bool Edge::operator==(const Edge &other) const
{
	return start == other.start && end == other.end && cost == other.cost && input == other.input;
}

bool Edge::operator!=(const Edge &other) const
{
	return !(*this == other);
}

Graph::Graph(const string &n):name(n)
{
	start = nullptr;
}

void Graph::addVertex(Vertex *vertex)
{
	if(existVertex(vertex)) return;

	if(vertex->isStart()) {
		assert(!start);
		start = vertex;
	}

	if(vertex->isFinish() && !isFinish(vertex)) {
		finish.push_back(vertex);
	}

	vertices.push_back(vertex);
}

template <typename T,typename F>
void mapFn(T& t,F fn)
{
	for(typename T::iterator it = t.begin(); it != t.end();++it)
	{
		fn(*it);
	}
}

template <typename T,typename F>
T filterFn(const T& t,F fn)
{
	T ret;

	for(typename T::const_iterator it = t.begin(); it != t.end();++it)
	{
		if(fn(*it)) {
			ret.push_back(*it);
		}
			
	}

	return ret;
}

void Graph::removeVertex(Vertex *vertex)
{	
	std::list<Edge*> es;
	auto fn = [&](Edge *it) {
		return it->start == vertex || it->end == vertex;
	};

	get(edges,es,fn);
	for(edge_iterator it = es.begin();it != es.end();++it)
		removeEdge(*it);

	vertices.remove(vertex);
	if(start == vertex) start = nullptr;
	if(isFinish(vertex)) finish.remove(vertex);
}

template <typename T,typename F>
void Graph::get(const std::list<T> &l,std::list<T> &r,F fn)
{
	r = filterFn(l,fn);
}

template <typename F>
void Graph::getEdges(std::list<Edge*> &e,F fn)
{
	get(edges,e,fn);
}
	
template <typename F>
void Graph::getVertices(std::list<Vertex*> &e,F fn)
{
	get(vertices,e,fn);
}

void Graph::addEdge(Edge *edge)
{
	if(!existVertex(edge->start) || !existVertex(edge->end))
		throw std::logic_error("vertex isn't in graph");
	if(!existEdge(edge))
		edges.push_back(edge);

	if(!isAcceptSym(edge->input))
		symbols.push_back(edge->input);
}

void Graph::removeEdge(Edge *edge)
{
	edges.remove(edge);
}

bool Graph::existVertex(Vertex *v)
{
	bool ret = false;
	auto fn = [&](Vertex *it)
		{
			if(it == v) ret = true;
		};

	mapFn(vertices,fn);
	return ret;
}

bool Graph::existEdge(Edge *e)
{
	bool ret = false;
	auto fn = [&](Edge *it)
		{
			if(it == e) ret = true;
		};

	mapFn(edges,fn);
	return ret;
}

bool Graph::isFinish(Vertex *v)
{
	bool ret = false;
		auto fn = [&](Vertex *it)
		{
			if(it == v) ret = true;
		};

	mapFn(finish,fn);
	return ret;
}

bool Graph::isAcceptSym(const std::string &s)
{
	bool ret = false;
	auto fn = [&](const std::string& it)
		{
			if(it == s) ret = true;
		};

	mapFn(symbols,fn);
	return ret;
}


Graph::Graph(const Graph& g)
{
	name = g.name;
	vertices = g.vertices;
	edges = g.edges;
	start = g.start;
	finish = g.finish;
}

Graph& Graph::operator=(const Graph& other)
{
	if(this != &other) {
		name = other.name;
		vertices = other.vertices;
		edges = other.edges;
	}

	return *this;
}

bool Graph::operator==(const Graph& other) const
{
	return name == other.name && edges == other.edges && vertices == other.vertices;
}

bool Graph::operator!=(const Graph& other) const
{
	return !(*this == other);
}



std::ostream &operator<<(std::ostream &os,const Graph& r)
{
	Graph::edge_type es;
	Graph& g = const_cast<Graph&>(r);
	os << "Graph:{" << endl << "name:" << g.name << endl;
	os << "start:" << endl;
	if(g.start)
		os << *(g.start) << endl;
	os << "finish:" << endl;
	for(Graph::vertex_iterator it = g.finish.begin();it != g.finish.end();++it)
		os << **it;
	os << endl;
	os << "alphabet:" << endl;
	for(std::string it:g.symbols)
		os << it << '\t';
	os << endl;
	for(Graph::vertex_iterator it = g.vertex_begin();it != g.vertex_end();++it)
		{
			os << **it << endl;;
			auto fn = [&](const Edge *e){
				return e->start == *it;
			};

			g.getEdges(es,fn);
			for(Graph::edge_iterator i = es.begin(); i != es.end();++i)
				{
					os << **i << endl;
				}
		}
	os << "}" << endl;

	return os;
}

bool Graph::beginWith(const std::string &dst, const string &src) const
{
	if(dst.size() < src.size()) return false;

	size_t i = 0;
	for(i = 0;i < dst.size() && dst[i] == src[i];i++)
		;

	return i == src.size();
}

// check vertices has finish state
bool Graph::containFinish(const vertex_type& vertices) const
{
	if(vertices.size() == 0) return false;
	for(vertex_type::const_iterator it = vertices.begin(); it != vertices.end(); ++it) {
		if(std::find(finish.begin(),finish.end(),*it) != finish.end()) {
			return true;
		}
	}

	return false;
}

bool Graph::DFAMatch(const std::string &str)
{
	Vertex *it = start;
	size_t i = 0;
	auto fn = [&](const Edge *e) 
	{
		return e->start == it && beginWith(str.substr(i),e->input);
	};

	edge_type es;
	
	while(i < str.size())
		{
			getEdges(es,fn);
			//cout << "match:" << (es.front()) ->input << endl;
			if(es.size() == 0) break;
			assert(es.size() ==1);
			it = (es.front())->end;
			cout << *(es.front()) << endl;
			i = i + (es.front())->input.size();
		}

	return it->isFinish() && i == str.size() && isFinish(it);
}


bool Graph::isNFA()
{
	string empty;
	
	if(isAcceptSym(empty)) return true;

	for(edge_iterator fit = edge_begin();fit != edge_end();++fit)
		for(edge_iterator sit = std::next(fit); sit != fit;++sit)
		{
			if(**fit == **sit) return true;
		}

	
	return false;
}

// calculate start symbol closure
Graph::vertex_type Graph::closure(const std::string &str)
{
	vertex_type svt,dvt;
	
	if(!isAcceptSym(str)) 
		throw std::logic_error(str + " is not in accept symbols.");
	
	svt.push_back(start);
	
	closure(str,svt,dvt);
	
	return dvt;
}

// find all vertex where start is in set svt and input is str with empty str
void Graph::closure(const std::string &str,const vertex_type &svt,vertex_type &dvt)
{
	vertex_type vt(svt);
	// first calculate empty state
	once_closure(std::string(),vt,dvt);
	if(str.empty()) return;
	// second calculate str state 
	vt = dvt;
	once_closure(str,vt,dvt);
	// third calculate empty state
	vt = dvt;
	once_closure(std::string(),vt,dvt);
}

// find all vertex where start is in set svt and input is str without empty str
void Graph::once_closure(const std::string &str,const vertex_type &svt,vertex_type &dvt)
{
	if(!isAcceptSym(str)) 
		throw std::logic_error(str + " is not in accept symbols.");
	
	edge_type es;
	
	for(vertex_type::const_iterator sit = svt.begin(); sit != svt.end(); ++sit) {

		auto fn = [&](const Edge *e) {
			return e->start == *sit && (e->input == str);
		};
		
		// find all edges
		getEdges(es,fn);
		
		for(edge_type::const_iterator eit = es.begin(); eit != es.end(); ++eit) {
		
			if(std::find(dvt.begin(),dvt.end(),(*eit)->end) == dvt.end()) {
				dvt.push_back((*eit)->end);
			}
		}
	}
}

/**
 *
 * make a NFA to DFA using 
 **/
bool Graph::toDFA(Graph &g)
{
	if(!isNFA()) {
		g = *this;
		return true;
	}

	vertex_type *dvt = new vertex_type();
	vertex_type tstart;
	//calculate start symbol closure
	*dvt = closure(std::string());
	tstart = *dvt;

	//new vertices set
	std::list<vertex_type *> vlist;
	// new edges set
	std::list<TEdge> tedges;
	// new finish set
	std::list<vertex_type*> tfinish;
 
	vlist.push_back(dvt);

	if(containFinish(tstart))
		tfinish.push_back(&tstart);
	
	
	vertex_type svt = *dvt;

	for(std::list<vertex_type *>::iterator it = vlist.begin();it != vlist.end();++it) {

		for(std::string s : symbols) {
			dvt = new vertex_type();
			//calculate *it 's input symbol
			closure(s,**it,*dvt);

			// new edge
			TEdge te;
			te.start = **it;
			te.end = *dvt;
			te.input = s;
			tedges.push_back(te);
			
			// make sure dvt contain finish state
			if(containFinish(*dvt))
				tfinish.push_back(dvt);
			

			// new state
			if(std::find(vlist.begin(),vlist.end(),dvt) == vlist.end()) {
				vlist.push_back(dvt);
			}
		}
	}
	
	// make a new graph
	newGraph(tstart,vlist,tedges,tfinish,g);

	return false;
}

void newGraph(const Graph::vertex_type& nstart,const std::list<Graph::vertex_type *> &nlist,
			  const std::list<Graph::TEdge> &nedges,
			  const std::list<Graph::vertex_type *> &nfinish,
			  Graph &ng)
{
	std::map<const Graph::vertex_type*,Vertex*> vtovt;

	// add start vertex
	Vertex *v1,*v2;
	Edge *e;

	Vertex *v = new Vertex("start vertex",S_START);
	vtovt.insert(std::pair<const Graph::vertex_type*,Vertex*>(&nstart,v));
	ng.addVertex(v);

	// add finish vertex
	for(std::list<Graph::vertex_type *>::const_iterator it = nfinish.begin();
		it != nfinish.end(); ++it) {
		if(vtovt.find(*it) != vtovt.end()) {
			v = new Vertex("finish vertex",S_FINISH);
			vtovt.insert(std::pair<const Graph::vertex_type*,Vertex*>(*it,v));
			ng.addVertex(v);
		}
	}

	for(std::list<Graph::TEdge>::const_iterator it = nedges.begin();
		it != nedges.end();++it)
		{
			if(vtovt.find(&(it->start)) != vtovt.end()) {
				v1 = new Vertex("continue vertex",S_CONTINUE);
				vtovt.insert(std::pair<const Graph::vertex_type*,Vertex*>(&(it->start),v1));
				ng.addVertex(v1);
			}

			if(vtovt.find(&(it->end)) != vtovt.end()) {
				v2 = new Vertex("continue vertex",S_CONTINUE);
				vtovt.insert(std::pair<const Graph::vertex_type*,Vertex*>(&(it->end),v2));
				ng.addVertex(v2);
			}
			//the edge is new,so it's also new
			e = new Edge(v1,v2,(*it).input);
			ng.addEdge(e);

		}
	
	assert(nlist.size() == vtovt.size());
}
