#include <bits/stdc++.h>
#include <vector>
#include <iostream>

using namespace std;

typedef struct {
  int destination;
  int distance;
} edge_t;

class graph {
private:
  vector<vector <edge_t>> edge;
  
public:
  void show(void);
  void add_vertex(int x);
  void add_edge(int x, int y, int distance);
  void remove_vertex(int x);
  void remove_edge(int x, int y);

  int  size(void);
  int  graphsize(void);
  int  edgesize(int idx);
  int  destination(int idx, int idy);
  int  distance(int idx, int idy);
};

//graph size
int graph::size(void) {
  return edge.size();
}

int graph::graphsize(void) {
  return edge.size();
}

int graph::edgesize(int idx) {
  return edge[idx].size();
}

//place in graph
int graph::destination(int idx, int idy) {
  return edge[idx][idy].destination;
}

//distance between the vertex
int graph::distance(int idx, int idy) {
  return edge[idx][idy].distance;
}

//adding edge
void graph::add_edge(int x, int y, int distance) {
  /* if size isn't enough, expand it */
  if (edge.size() < x + 1) edge.resize(x + 1);
  if (edge.size() < y + 1) edge.resize(y + 1);
  /* add data */
  edge[x].push_back({y, distance});
}

//adding vertex
void graph::add_vertex(int x) {
  add_edge(x, x, 0);
}

//removing edge
void graph::remove_edge(int x, int y) {
  int idx, idy;

  /* check if value is out of range */
  if (x >= edge.size()) return;

  /* idx is equal to x */
  idx = x;

  /* find idy */
  for (idy = 0; idy < edge[idx].size();) {
    if (edge[idx][idy].destination == y) {
      edge[idx].erase(edge[idx].begin() + idy);
    } else {
      idy ++;
    }
  }
}

//remove vertex
void graph::remove_vertex(int x) {
  int idx, idy;
  
  /* check if value is out of range */
  if (x >= edge.size()) return;
  
  for (idx = 0; idx < edge.size(); idx ++) {
    for (idy = 0; idy < edge[idx].size();) {
      if ((idx == x) || (edge[idx][idy].destination == x)) {
	remove_edge(idx, edge[idx][idy].destination);
      } else {
	idy ++;
      }
    }
  }
}

//print
void graph::show(void) {
  int idx, idy, x, y;
  int dist[size()][size()];
  
  /* Initialize Matrix */
  for (x = 0; x < size(); x ++) 
    for (y = 0; y < size(); y ++)
      dist[x][y] = INT_MAX;
  
  /* Filling Table */
  for (idx = 0; idx < graphsize(); idx ++) {
    for (idy = 0; idy < edgesize(idx); idy ++) {
      dist[idx][destination(idx, idy)] = distance(idx, idy);
    }
  }
  
  /* Printing Table */
  cout << "     ";
  for (idx = 0; idx < size(); idx ++) {
    cout << " " << setw(3) << idx;
  }
  cout << endl << "     ";

  for (idx = 0; idx < size(); idx ++) cout << "----";
  cout << endl;
  for (idx = 0; idx < size(); idx ++) {
    cout << setw(3) << idx << " :";
    for (idy = 0; idy < size(); idy ++) {
      if (dist[idx][idy] == INT_MAX) {
	cout << "    ";
      } else {
	cout << " " << setw(3) << dist[idx][idy];
      }
    }
    cout << endl;
  }
  cout << endl;
}

typedef struct queue_s {
  int destination;
  int distance;
  struct queue_s *next;
} queue_t;

class myqueue {
private:
  queue_t *root;

public:
  myqueue(void);
  int check(void);  
  void push(int destination, int distance);
  void pop(int &destination, int &distance);
};

myqueue::myqueue(void) {
  root = NULL;
}

void myqueue::push(int destination, int distance) {
  queue_t *newqueue, *t;
  
  newqueue = new queue_t;
  newqueue -> destination = destination;
  newqueue -> distance    = distance;
  newqueue -> next        = NULL;

  /* adding queue, but check the distance */
  if (root == NULL) {
    root = newqueue;
  } else if (root -> distance >= distance) {
    /* need insert to root */
    newqueue -> next = root;
    root = newqueue;
  } else {    
    t = root;
    while ((t -> next != NULL) && (t -> next -> distance < distance)) t = t -> next;
    newqueue -> next = t -> next;
    t -> next = newqueue;
  }
}

/* check if queue is empty */
int myqueue::check(void) { return (root != NULL); }

/* retrieve first queue */
void myqueue::pop(int &destination, int &distance) {
  queue_t *newqueue;
  
  newqueue = root;
  root  = root -> next;
  destination = newqueue -> destination;
  distance    = newqueue -> distance;
  delete newqueue;
}

static void print_route(int graphsize, int start, int goal, int *dist, int *prev) {
  int i, path[graphsize], pathsize;

  /* input data is reverse order */
  pathsize = 0;
  i = goal;
  while ((dist[i] > 0) && (dist[i] != INT_MAX)) {
    path[pathsize] = i;
    pathsize ++;
    i = prev[i];
  } 
  path[pathsize] = i;
  pathsize ++;
  
  cout << "Route from " << start << " to " << goal << " :";
    
  if (dist[i] == INT_MAX) {
    cout << " No path exists." << endl;
  } else {
    cout << endl;
    for (i = pathsize - 1; i > 0; i --) {
      cout << setw(3) << path[i] << "  --->  " << setw(3) << path[i - 1];
      cout << ",  distance: " << setw(3) << dist[path[i - 1]] - dist[path[i]];
      cout << ",  Total distance : " << setw(3) << dist[path[i - 1]] << endl;
    }
  }
  cout << endl;
}

static void dijksra(graph graph, int start, int goal) {
  int dist[graph.size()];
  int prev[graph.size()];
  myqueue myqueue;
  
  fill(dist, dist + graph.size(), INT_MAX);
  dist[start] = 0;
  
  myqueue.push(start, 0);
  while(myqueue.check()) {
    int idx, d;
    
    myqueue.pop(idx, d);
    
    if (d > dist[idx]) continue;
    for (int i = 0; i < graph.edgesize(idx); i++) {
      int dest = graph.destination(idx, i);
      int distance = graph.distance(idx, i);
      if (d + distance < dist[dest]) {
	dist[dest] = d + distance;
	prev[dest] = idx;
	myqueue.push(dest, dist[dest]);
      }
    }
  }
  
  print_route(graph.size(), start, goal, dist, prev);
}

int main(void) {
  graph graph;

  #if 0
  graph.add_edge(0, 1,  4);
  graph.add_edge(0, 6,  7);
  graph.add_edge(1, 0,  4);
  graph.add_edge(1, 2,  9);
  graph.add_edge(1, 6, 11);
  graph.add_edge(1, 7, 20);
  graph.add_edge(2, 1,  9);
  graph.add_edge(2, 3,  6);
  graph.add_edge(2, 4,  2);
  graph.add_edge(3, 2,  6);
  graph.add_edge(3, 4, 10);
  graph.add_edge(3, 5,  3);
  graph.add_edge(4, 2,  2);
  graph.add_edge(4, 3, 10);
  graph.add_edge(4, 5, 15);
  graph.add_edge(4, 7,  1);
  graph.add_edge(4, 8,  5);
  graph.add_edge(5, 3,  5);
  graph.add_edge(5, 4, 15);
  graph.add_edge(5, 8, 12);
  graph.add_edge(6, 0,  7);
  graph.add_edge(6, 1, 11);
  graph.add_edge(6, 7,  1);
  graph.add_edge(7, 6,  1);
  graph.add_edge(7, 1, 20);
  graph.add_edge(7, 4,  1);
  graph.add_edge(7, 8,  3);
  graph.add_edge(8, 4,  5);
  graph.add_edge(8, 5, 12);
  graph.add_edge(8, 7,  3);
  graph.add_edge(9, 9,  0);
  
  graph.show();

  //  graph.remove_edge(4, 3);
    graph.remove_vertex(7);

  graph.show();

  dijksra(graph, 0, 2);

  dijksra(graph, 5, 1);

  dijksra(graph, 9, 3);
#endif

  int x = 0;
  
  while (x == 0){
    cout << "Enter number:" << endl;
    cout << "   1. add vertex" << endl;
    cout << "   2. add edge" << endl;
    cout << "   3. remove vertex" << endl;
    cout << "   4. remove edge" << endl;
    cout << "   5. find shortest path" << endl;
    
    int input = 0;
    cin >> input;
    cin.get();
    
    //adding vertex
    if (input == 1){
      
      int numver = 0;
      cout << "input vertex number: ";
      cin >> numver;
      cin.get();
      
      graph.add_vertex(numver);

      graph.show();
    }
	
    //adding edge
    if (input == 2){

      int startver = 0;
      cout << "input starting vertex: ";
      cin >> startver;
      cin.get();
      
      int endver = 0;
      cout << "input ending vertex: ";
      cin >> endver;
      cin.get();
      
      int numedge = 0;
      cout << "input edge length: ";
      cin >> numedge;
      cin.get();
      
      graph.add_edge(startver, endver, numedge);

      graph.show();
    }
    
    //removing vertex
    if (input == 3){

      int numver = 0;
      cout << "input vertex number: ";
      cin >> numver;
      cin.get();
      
      graph.remove_vertex(numver);

      graph.show();
    }

    
    //removing edge
    if (input == 4){

      int startver = 0;
      cout << "input starting vertex of the edge: ";
      cin >> startver;
      cin.get();
      
      int endver = 0;
      cout << "input ending vertex of the edge: ";
      cin >> endver;
      cin.get();

      graph.remove_edge(startver, endver);

      graph.show();
    }

    
    //finding shortest path
    if (input ==5){

      int startver = 0;
      cout << "input starting vertex: ";
      cin >> startver;
      cin.get();

      int endver = 0;
      cout << "input ending vertex: ";
      cin >> endver;
      cin.get();

      
      dijksra(graph, startver, endver);
    }

    
  }
  
  
  return 0;
}
