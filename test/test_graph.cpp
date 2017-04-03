// test_graph.cpp --- 
// 
// Filename: test_graph.cpp
// Description: 
// Author: 
// Maintainer: 
// Created: 星期二 六月 28 20:22:48 2016 (+0800)
// Version: 
// Package-Requires: ()
// Last-Updated: 星期二 六月 28 20:23:41 2016 (+0800)
//           By: wyt
//     Update #: 5
// URL: 
// Doc URL: 
// Keywords: 
// Compatibility: 
// 
// 

// Commentary: 
// 
// 
// 
// 

// Change Log:
// 
// 
// 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// 

// Code:



// 
// test_graph.cpp ends here
#include <gtest/gtest.h>
#include <iostream>
#include <exception>

#include "graph.h"

using namespace std;


TEST (VertexTest,AllMethods) {
	Vertex v1("v1");
	Vertex v2("v2");
	EXPECT_FALSE(v1==v2);
	EXPECT_TRUE(v1!=v2);
	v1 = v2;
	EXPECT_TRUE(v1==v2);
	cout <<"v1:" << v1 << ",v2:" << v2 << endl;
}

TEST (EdgeTest,AllMethods) {
	Vertex v1("v1");
	Vertex v2("v2");

	Edge e0(&v1,&v1,"a");
	Edge e1(&v1,&v2,"b");
	Edge e2(&v2,&v2,"c");


	EXPECT_EQ(e0,e0);
	EXPECT_FALSE(e0 == e1);
	EXPECT_TRUE(e1 == e1);

	e0 = e1;
	EXPECT_TRUE(e0 == e1);

	cout << "e0:" << e0 << endl << "e1:" << e1 << endl << "e2:" << e2;

}

TEST (GraphTest,AllMethods) {
	Vertex v1("v1",S_START);
	Vertex v2("v2",S_FINISH);
	Vertex v3("v3",S_CONTINUE);

	Edge e0(&v1,&v1,"a");
	Edge e1(&v1,&v2,"b");
	Edge e2(&v2,&v2,"c");
	Edge e3(&v1,&v3,"");


	Graph g("g");
	g.addVertex(&v1);
	g.addVertex(&v1);
	g.addEdge(&e0);

	Graph g2(g);
	EXPECT_TRUE(g == g2);
	try {
		g.addEdge(&e1);
	}
	catch(std::exception &e)
		{
			cout << "catch exception:" << e.what() << endl;
		}
	EXPECT_EQ(g.vertex_size(),1);
	EXPECT_EQ(g.edge_size(),1);
	EXPECT_TRUE(g.existVertex(&v1));
	EXPECT_FALSE(g.existEdge(&e1));

	g.addVertex(&v2);
	g.addEdge(&e0);
	g.addEdge(&e1);
	g.addEdge(&e2);
	
	cout << g << endl;
	std::string str = "aabc";
	//cout << "match: " << str.c_str() << ",size:" << str.size() << "," << g.DFAMatch(str) << endl;
	EXPECT_EQ(g.DFAMatch(str),true);
	g.removeVertex(&v1);
	EXPECT_EQ(g.vertex_size(),1);
	EXPECT_EQ(g.edge_size(),1);

	g.removeEdge(&e0);

	EXPECT_FALSE(g == g2);

	g.addVertex(&v1);
	g.addVertex(&v3);
	
	g.addEdge(&e3);
	g.addEdge(&e1);
	EXPECT_TRUE(g.existEdge(&e1));
	EXPECT_TRUE(g.isNFA());
	EXPECT_EQ(g.vertex_size(),3);
	cout << g << endl;
	//cout  << "e1:" << e1 << endl;
	Graph g1("g1");
	g.toDFA(g1);
	cout << "g1" << endl << g1 << endl;
}

int main(int argc,char *argv[])
{
	::testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}
