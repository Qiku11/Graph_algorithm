//a1807323 Shuhao Duan ADSA assignment 4
//2022/06/02

#include <string>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct road{
  int start;
  int end;
  int cost;
};

int main (){

  //input parsing block

  int totalcost = 0;
  string input = "";
  getline(cin, input);

  //get city count first
  int counter = 0;
  int citycount = 1;
  for (unsigned int i = 0; i < input.length(); i ++){
    if (input[i] == ','){
      citycount++;
    }
    if (input[i] == ' '){
      break;
    }
  }

  //now we have citycount ready, we can create string arrays to store input data now
  string buffer;

  string connection[citycount];
  string buildcost[citycount];
  string destroycost[citycount];

  counter = 0;
  int whicharray = 0;
  for (unsigned int i = 0; i < input.length(); i ++){
    if (input[i] == ','){
      if (whicharray == 0){
        connection[counter] = buffer;
      } else if (whicharray == 1){
        buildcost[counter] = buffer;
      } else {
        destroycost[counter] = buffer;
      }
      buffer = "";
      counter++;
    } else if (input[i] == ' '){
      if (whicharray == 0){
        connection[counter] = buffer;
      } else if (whicharray == 1){
        buildcost[counter] = buffer;
      } else {
        destroycost[counter] = buffer;
      }
      buffer = "";
      counter = 0;
      whicharray++;
    } else {
      buffer = buffer + input[i];
    }
  }
  destroycost[counter] = buffer;
  buffer = "";

  //by now we should have the three data arrays ready for use

  //test code to see if they contain the correct data:
  /*
  for (int i = 0 ; i < citycount; i ++){
  cout<<connection[i]<<' ';
}
cout<<endl;

for (int i = 0 ; i < citycount; i ++){
cout<<buildcost[i]<<' ';
}
cout<<endl;

for (int i = 0 ; i < citycount; i ++){
cout<<destroycost[i]<<' ';
}
cout<<endl;
*/




//subtree identification block



//the next step is to use a graph navigation method to find out how many
//disjoint parts
//I decide to save the found inter-connected groups of cities into int arrays

//since the way data has been given, BFS is the most suitable way to construct
//the trees
counter = 0;
int visited[citycount];//record visited cities
for (int i = 0; i < citycount; i++){
  visited[i] = 0;
}
int newlyvisited[citycount];//works as temporary buffer for cities visited in current bfs round
for (int i = 0; i < citycount; i++){
  newlyvisited[i] = 0;
}
string parts[citycount]; //the parts (interconnected sub-graphs that are not connected to each other)
vector<road> allroads[citycount];
int partscount = 0; //the number of parts found
int needdestroy[citycount];//corresponds to parts index to mark parts that have cycles
for (int i = 0; i < citycount; i++){
  needdestroy[i] = 0;
}
queue<int> bfsqueue;//the working queue of bfs
int currencity = 0;//variable to hold current city in bfs algorithm
int lastvisited = 0;
vector<road> roads;
road currenroad;
int unique = 0;

//go through all cities
for (int i = 0; i < citycount; i++){
  //as preparation, we first set all values in newlyvisited to 0
  for (int j = 0; j < citycount; j ++){
    newlyvisited[j] = 0;
  }

  //if current city has not been reached by previous bfs trees
  //start bfs on it and save it as a new tree
  if (visited[i] == 0){
    //here begins the BFS code segement

    //initialization before real bfs takes place:

    //first gather all edges directly connected to it
    visited[i] = 1;
    newlyvisited[i] = 1;
    for (int j = 0; j < citycount; j++){
      //if it has a path to city j, check if j is already visited, if not
      //add j to our queue
      if (connection[i][j] == '1'){
        //for every path we visit, record the path into our roads
        if (i>j){
          currenroad.end = i;
          currenroad.start = j;
        } else {
          currenroad.end = j;
          currenroad.start = i;
        }
        //we also need to find its destruction cost
        if (isupper(destroycost[i][j])){
          currenroad.cost = destroycost[i][j] - 65;
        } else {
          currenroad.cost = destroycost[i][j] - 71;
        }
        //now see if this road has already been added to our collection of roads
        unique = 1;
        for (unsigned int k = 0; k < roads.size(); k++){
          if (roads[k].start==currenroad.start && roads[k].end==currenroad.end){
            unique = 0;
            break;
          }
        }
        //if road is unique, add it to our collection
        if (unique==1){
          roads.push_back(currenroad);
        }

        //if the target city has not been visited, enqueue it
        if (visited[j]!=1){
          bfsqueue.push(j);
          visited[j] = 1;
          newlyvisited[j] = 1;
        }
      }
    }
    lastvisited = i;
    //now all paths connected to the starting node has been enqueued
    //we can start the real bfs "prosedue"

    while (bfsqueue.size()!=0){
      //take out a city from our queue
      currencity = bfsqueue.front();
      bfsqueue.pop();
      //loop through all roads spots
      for (int j = 0; j < citycount; j++){
        //if a road exists
        if (connection[currencity][j] == '1'){
          //try to record this road
          //for every path we visit, record the path into our roads
          if (currencity>j){
            currenroad.end = currencity;
            currenroad.start = j;
          } else {
            currenroad.end = j;
            currenroad.start = currencity;
          }
          //we also need to find its destruction cost
          if (isupper(destroycost[currencity][j])){
            currenroad.cost = destroycost[currencity][j] - 65;
          } else {
            currenroad.cost = destroycost[currencity][j] - 71;
          }
          //now see if this road has already been added to our collection of roads
          unique = 1;
          for (unsigned int k = 0; k < roads.size(); k++){
            if (roads[k].start==currenroad.start && roads[k].end==currenroad.end){
              unique = 0;
              break;
            }
          }
          //if road is unique, add it to our collection
          if (unique==1){
            roads.push_back(currenroad);
          }


          //see if the city has already been visited
          if (visited[j]!=1){
            //if not visited, add it to queue to visit next,
            //and mark it as visited
            bfsqueue.push(j);
            visited[j] = 1;
            newlyvisited[j] = 1;
          } else {
            //in this case, we are trying to go to back to an already-visited
            //city if it is not where we came from just now,
            //then we have discovered a cycle.
            if (j!=lastvisited){
              needdestroy[partscount] = 1;
            }
          }
        }//if no path from current city to city j, do nothing
      }
      lastvisited = currencity;
    }
    //now we are done with the bfs search starting from this city
    //we should have all the cities in this subtree saved in visited
    //translate visited to a string containing numbers representing cities
    //and save it to parts
    for (int k = 0; k < citycount; k++){
      if (newlyvisited[k]==1){
        buffer = buffer + to_string(k);
      }
    }
    //now we should have the cities in this graph saved to buffer
    parts[partscount] = buffer;//save all the cities in this interconnected subpart to the array
    allroads[partscount] = roads;//save all roads between these cities to allroads array
    buffer = "";//empty the cities buffer;
    roads.clear();//empty the road buffer
    partscount++;
  }
}
//testing code to see if the bfs subtree finder block works properly
/*
for (int i = 0; i < partscount; i++){
cout<<parts[i]<<' ';
}
cout<<endl;
for (int i = 0; i < partscount; i++){
cout<<needdestroy[i]<<' ';
}
cout<<endl;
*/
/*
for (int i = 0; i < partscount; i++){
for (int j = 0; j < allroads[i].size(); j++){
cout<<allroads[i][j].start<<','<<allroads[i][j].end<<'_'<<allroads[i][j].cost<<' ';
}
cout<<endl;
}
*/





//destroy redundant roads block





//now we have saved all sub-graphs into their respective places.
//the next step is to destroy redundant paths
//i have convinently left markers for cycles in the bfs process for destroy use
int maxcost;
int maxcostposition;
int donothing = 0;
//loop through all the subtrees

for (int i = 0; i < partscount; i++){
  //if one subtree has been marked to have one or more cycles
  //jarnik-prim algorithm in action!
  if (needdestroy[i]==1){
    //we perform the re-structure here(actually just identify which roads should
    //we destroy)
    //we need to gather all the edges in current subtree
    //to prevent duplicate, i define all edges to be represented as
    //smaller city to larger city

    //we re-use the newlyvisited array to record cities that have been connected
    for (int j = 0; j < citycount; j ++){
      newlyvisited[j] = 0;
    }//set all cities to be 0 so we can begin

    //to initialize, we add the first, most-expensive path to begin
    maxcost = allroads[i][0].cost;
    maxcostposition = 0;
    for (unsigned int j = 0; j < allroads[i].size(); j++){
      if (allroads[i][j].cost>maxcost){
        maxcost = allroads[i][j].cost;
        maxcostposition = j;
      }
    }

    //mark both ends of our starter path as visited
    newlyvisited[allroads[i][maxcostposition].start] = 1;
    newlyvisited[allroads[i][maxcostposition].end] = 1;

    //also remove this path from the pool of all avaliable paths pending
    allroads[i].erase(allroads[i].begin()+maxcostposition);

    //now we begin adding the paths
    while (allroads[i].size()!=0){//before we empty out the entire vector
      //cout<<allroads[i].size()<<endl;
      //each time we find 1 candidate to remove from vector
      //initialize our candidate holder to "empty"
      maxcost = -1;
      maxcostposition = -1;
      donothing = 0;
      //loop through all remaining roads in the vector
      for (unsigned int j = 0; j < allroads[i].size(); j++){
        //if an edge is connected to our existing tree
        if (newlyvisited[allroads[i][j].start]==1||newlyvisited[allroads[i][j].end]==1){
          //if both its ends are connected to our existing tree, this is a road which we destroy
          if (newlyvisited[allroads[i][j].start]==1&&newlyvisited[allroads[i][j].end]==1){
            //record our destruction cost, and consider this path done with
            totalcost = totalcost + allroads[i][j].cost;
            allroads[i].erase(allroads[i].begin()+j);
            //re-initialize and start over because we just screwed up indexing
            donothing = 1;
            break;
          }

          //see if a previous competitor exists, if not, this first path
          //becomes the candidate
          if (maxcost==-1){
            maxcost = allroads[i][j].cost;
            maxcostposition = j;
          } else if (allroads[i][j].cost>maxcost){
            //if a previous candidate exists already, see if this road has
            //a higher cost than previous candidate
            //if so, this new road becomes the candidate
            maxcost = allroads[i][j].cost;
            maxcostposition = j;
          }
        }
      }//after this, we should have our candidate ready for removal

      //it might be the case that we did not find a candidate but a cycle
      //instead, in this case the cycle-forming element has already been
      //removed, we should just skip the rest of the code and enter the next
      //iteration
      if (donothing == 1) continue;

      //mark the new city connected to our tree by this new path
      newlyvisited[allroads[i][maxcostposition].start] = 1;
      newlyvisited[allroads[i][maxcostposition].end] = 1;

      //remove the path from pool, consier it already added to our mst
      allroads[i].erase(allroads[i].begin()+maxcostposition);
    }
  }
}
//after this, all destruction should be done





//construct road between disjuct parts block

//here we begin our construction work

//this is just a double loop find-minimum problem
//ezpz
int currenmincost = -1;
//cout<<totalcost<<endl;
for (int i = 0; i < partscount; i ++){
  //i choose to skip the first round and work with [i] and [i-1]
  if (i == 0) continue;
//char to int -> char-48
  for (unsigned int j = 0; j < parts[i-1].length(); j++){
    for (unsigned int k = 0; k < parts[i].length(); k++){
      if (currenmincost == -1){
        if (isupper(buildcost[parts[i-1][j]-48][parts[i][k]-48])){
          currenmincost = buildcost[parts[i-1][j]-48][parts[i][k]-48] - 65;
        } else {
          currenmincost = buildcost[parts[i-1][j]-48][parts[i][k]-48] - 71;
        }
      }
      if (currenmincost > buildcost[parts[i-1][j]-48][parts[i][k]-48]){
        if (isupper(buildcost[parts[i-1][j]-48][parts[i][k]-48])){
          currenmincost = buildcost[parts[i-1][j]-48][parts[i][k]-48] - 65;
        } else {
          currenmincost = buildcost[parts[i-1][j]-48][parts[i][k]-48] - 71;
        }
      }
    }
  }
  //cout<<totalcost<<" + "<<currenmincost<<endl;
  totalcost = totalcost + currenmincost;
  if (totalcost > 121){
    if (totalcost < 126){
      totalcost = totalcost - 60;
    }
  }
  currenmincost = -1;
  parts[i] = parts[i] + parts[i-1];
}

cout<<totalcost<<endl;
return 0;
}
