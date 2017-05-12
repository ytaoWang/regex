#include <functional>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <string>
#include <list>
#include <map>
#include <set>
#include <stack>

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
  return (name == other.name && status == other.status && state == other.state);
}

bool Vertex::operator!=(const Vertex&other) const
{
	return !(*this == other);
}

bool Vertex::operator==(int value) const 
{
  return state == value;
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
	
	out << "start:" << *(e.start);
	out << "end:" << *(e.end);
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

bool Edge::operator==(const pair<pair<int,int>,string> &other) const
{
  return *start == std::get<0>(std::get<0>(other)) && *end == std::get<1>(std::get<0>(other)) && input == std::get<1>(other);
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
		finish.insert(vertex);
	}

	vertices.insert(vertex);
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
			ret.insert(*it);
		}
			
	}

	return ret;
}

void Graph::removeVertex(Vertex *vertex)
{	
	std::set<Edge*> es;
	auto fn = [&](Edge *it) {
		return it->start == vertex || it->end == vertex;
	};

	get(edges,es,fn);
	for(edge_iterator it = es.begin();it != es.end();++it)
		removeEdge(*it);

	vertices.erase(vertex);
	if(start == vertex) start = nullptr;
	if(isFinish(vertex)) finish.erase(vertex);
}

template <typename T,typename F>
void Graph::get(const std::set<T> &l,std::set<T> &r,F fn)
{
	r = filterFn(l,fn);
}

template <typename F>
void Graph::getEdges(edge_type &e,F fn)
{
	get(edges,e,fn);
}
	
template <typename F>
void Graph::getVertices(vertex_type &e,F fn)
{
	get(vertices,e,fn);
}

void Graph::addEdge(Edge *edge)
{
	if(!existVertex(edge->start) || !existVertex(edge->end))
		throw std::logic_error("vertex isn't in graph");
	if(!existEdge(edge))
		edges.insert(edge);

	if(!isAcceptSym(edge->input))
		symbols.push_back(edge->input);
}

void Graph::removeEdge(Edge *edge)
{
	edges.erase(edge);
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
			it = (*(es.begin()))->end;
			cout << *(es.begin()) << endl;
			i = i + (*(es.begin()))->input.size();
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

// calculate empty symbol closure
void Graph::closure(const vertex_type &svt,vertex_type &dvt)
{
  stack<Vertex *> sk;
  
  for_each(svt.begin(),svt.end(),
	   [&](Vertex *v){
	     dvt.insert(v);
	     sk.push(v);
	   });


  Vertex *item;
  edge_type es;
  while(!sk.empty()) {
    item = sk.top();
    sk.pop();
    //get edge which vertex start is item and input is empty
    es.clear();
    auto fn = [&](const Edge *e){
      return e->start == item && e->input.empty();
    };

    getEdges(es,fn);
    for_each(es.begin(),es.end(),[&](Edge *e){
	if(dvt.find(e->end) != dvt.end()) 
	  dvt.insert(e->end);
      });
  }
}

// find all vertex where start is in set svt and input is str without empty str
void Graph::closure(const std::string &str,const vertex_type &svt,vertex_type &dvt)
{

  dvt.clear();

  auto fn = [&](Vertex *v) {
    
    edge_type es;

    auto fn1 = [&](const Edge *e) {
      return e->start == v && e->input == str;
    };

    getEdges(es,fn1);
    for_each(es.begin(),es.end(),[&](Edge *e){
	if(dvt.find(e->end) != dvt.end()) 
	  dvt.insert(e->end);
      });
  };

  for_each(svt.begin(),svt.end(),fn);

}

/**
 *
 * make a NFA to DFA using subset construction
 **/
bool Graph::toDFA(Graph &g)
{
	if(!isNFA()) {
		g = *this;
		return true;
	}

	vertex_type nstart,tmp;
	std::list<vertex_type> nlist;
	std::list<vertex_visit_type*> nvt;
	std::list<Graph::TEdge> nedges;
	std::list<vertex_type> nfinish;

	tmp.insert(start);
	closure(tmp,nstart);

	
	vertex_visit_type *it =  new vertex_visit_type;
	it->vt = nstart;
	it->visited = false;
	nvt.push_back(it);
	nlist.push_back(nstart);
	
	for(std::list<vertex_visit_type *>::iterator item = nvt.begin();
	    item != nvt.end(); ++item)  {
	  if((*item)->visited) continue;
	(*item)->visited = true;
	for_each(symbols.begin(),symbols.end(),[&](string str)
		 {
		   vertex_type dvt;
		   tmp.clear();
		   closure(str,(*item)->vt,tmp);
		   closure(tmp,dvt);
		   if(std::find_if(nvt.begin(),nvt.end(),
				   [&dvt](const vertex_visit_type *vvt){
				     return vvt->vt == dvt && vvt->visited == false;
				   }) 
		      != nvt.end()) {
		     it =  new vertex_visit_type;
		     it->vt = dvt;
		     it->visited = false;
		     nvt.push_back(it);
		     nlist.push_back(dvt);
		     // insert finish status
		     if(containFinish(dvt))
		       nfinish.push_back(dvt);
		     
		   }
			      
		   // add edge into TEdge
		   Graph::TEdge tedge;
		   tedge.start = (*item)->vt;
		   tedge.end = dvt;
		   tedge.input = str;
		   nedges.push_back(tedge);
		 });
	}


	newGraph(nstart,nlist,nedges,nfinish,symbols,g);
	//delete nvt;
	while(!nvt.empty()){
	  it = nvt.back();
	  nvt.pop_back();
	  delete it;
	}

	return true;
}

void newGraph(const Graph::vertex_type& nstart,
	      const std::list<Graph::vertex_type> &nlist,
	      const std::list<Graph::TEdge> &nedges,
	      const std::list<Graph::vertex_type> &nfinish,
	      const Graph::symbol_type &symbols,
	      Graph &ng)
{
  int count = 0;
  std::map<Graph::vertex_type,Vertex*> mv;

  // add start state to ng
  Vertex *v = new Vertex("v" + std::to_string(count++),S_START);
  mv.insert(make_pair(nstart,v));

  
  ng.addVertex(v);

  //transform vertex
  auto fn = [&](const Graph::vertex_type &vt) {
    if(mv.find(vt) != mv.end()) {
      
      if(std::find(nfinish.begin(),nfinish.end(),vt) != nfinish.end())
	v = new Vertex("v" + std::to_string(count++),S_CONTINUE);
      else
	v = new Vertex("v" + std::to_string(count++),S_FINISH);
      
      mv.insert(make_pair(vt,v));
      ng.addVertex(v);      
    }
  };
  
  for_each(nlist.begin(),nlist.end(),fn);

  //transform edge
  auto fn1 = [&](const Graph::TEdge &e) {

    assert(mv.find(e.start) != mv.end());
    assert(mv.find(e.end) != mv.end());
    
    Vertex *start = mv.find(e.start)->second;
    Vertex *end = mv.find(e.end)->second;

    Edge *edge = new Edge(start,end,e.input);
    ng.addEdge(edge);
  };

  for_each(nedges.begin(),nedges.end(),fn1);
}
