#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <list>
#include <memory>

using namespace std;

struct RelatedNode {
    string data;
    int weight;

    bool operator== (RelatedNode r) {
        return (data == r.data) && (weight == r.weight);
    }
};

// AdjacencyList with weight
// 1 - weight flag(if not than without brackets)
// 1 - orient flag(if not than dup)
// 5 - count of nodes
// 1 2(34) 3(25) 5(14)
// 2 4(12) 5(14)
// 3 5(27)
// 4 1(25)
// 5

// EdgeList
// 1 - weight flag(if not than without brackets)
// 1 - orient flag(if not than dup)
// 5
// 1 2 34
// 1 3 25
// 1 5 14
// 2 4 12
// 2 5 14
// 3 5 27
// 4 1 25
// 5

class Graph {
    private:
        int count;
        bool WeightFlag, OrientFlag; 

        map<string, list<RelatedNode>> AdjacencyList;
        map<pair<string, string>, int> EdgeList;

    public:
        Graph() {}
        
        Graph(string path) {
            ifstream fin(path);

            fin >> WeightFlag >> OrientFlag >> count;

            string str_newline;
            while (getline(fin, str_newline, '\n')) {

                // split string
                vector<string> nodes;

                istringstream str_space(str_newline);
                string str_node;

                while (getline(str_space, str_node, ' ')) {
                    nodes.push_back(str_node);
                    // cout << str_node << endl;
                }

                // cout << endl;

                unique_ptr<RelatedNode> pos = make_unique<RelatedNode>();

                // for dup in not oriented graph
                unique_ptr<RelatedNode> Rnode1 = make_unique<RelatedNode>();
                unique_ptr<RelatedNode> Rnode2 = make_unique<RelatedNode>();

                if (nodes.size() == 1) {
                    AddNode(nodes[0]);
                    continue;
                }

                for (int id = 1; id < nodes.size(); ++id) {
                    istringstream ss(nodes[id]);

                    cout << "str_node:" << nodes[id] << endl;
                
                    if (!WeightFlag) {
                        pos->data = nodes[id];
                        AdjacencyList[nodes[0]].push_back(*pos);
                        continue;
                    }

                    string key;

                    while (getline(ss, str_node, '(')) {
                        // cout << "str_node:" << str_node << endl;

                        if (str_node[str_node.size() - 1] != ')') {
                            pos->data = str_node;

                            // dup if not oriented
                            if (!OrientFlag) {
                                Rnode1->data = pos->data;
                                key = pos->data;
                            }
                        }
                        else {
                            pos->weight = stoi(str_node.substr(0, str_node.size() - 1));

                            if (!OrientFlag) {
                                Rnode1->weight = pos->weight;
                                AdjacencyList[nodes[0]].push_back(*Rnode1);

                                Rnode2->data = nodes[0];
                                Rnode2->weight = pos->weight;
                                AdjacencyList[key].push_back(*Rnode2);
                            }
                            else
                                AdjacencyList[nodes[0]].push_back(*pos);
                        }
                    }
                }
            }

            fin.close();
        }

        Graph(int c, bool Weight, bool Oriented, map<string, list<RelatedNode>> AdjList, map<pair<string, string>, int> EList) {
            count = c;
            WeightFlag = Weight;
            OrientFlag = Oriented;
            AdjacencyList = AdjList;
            EdgeList = EList;
        }

        int GetCount() {
            return count;
        }

        map<string, list<RelatedNode>> GetAdjacencyList() {
            return AdjacencyList;
        }

        map<pair<string, string>, int> GetEdgeList() {
            return EdgeList;
        }

        void AddEdge(string begin, string end, int weight) {
            unique_ptr<RelatedNode> Rnode1 = make_unique<RelatedNode>();
            Rnode1->data = end;

            if (WeightFlag)
                Rnode1->weight = weight;

            AdjacencyList[begin].push_back(*Rnode1);

            if (!OrientFlag) {
                unique_ptr<RelatedNode> Rnode2 = make_unique<RelatedNode>();
                Rnode2->data = begin;

                if (WeightFlag)
                    Rnode2->weight = weight;

                AdjacencyList[end].push_back(*Rnode2);
            }
        }

        void AddNode(string str) {
            AdjacencyList[str];
        }

        void DelEdge(string begin, string end) {
            for (auto Rnode : AdjacencyList[begin])
                if (Rnode.data == end) {
                    AdjacencyList[begin].remove(Rnode);
                    break;
                }

            if (!OrientFlag) {
                for (auto Rnode : AdjacencyList[end])
                    if (Rnode.data == begin) {
                        AdjacencyList[end].remove(Rnode);
                        break;
                    }
            }
        }

        void DelNode(string data) {
            AdjacencyList.erase(data);

            for (auto &node : AdjacencyList) {
                for (auto &lst : node.second) {
                    cout << lst.data << endl;
                    if (lst.data == data) {
                        node.second.remove(lst);
                        break;
                    }
                }
            }
        }

        void WriteInFile(string path) {
            ofstream fout(path);

            fout << count << endl;
            for (auto node : AdjacencyList) {
                fout << node.first << " ";

                for (auto &lst : node.second) {
                    fout << lst.data;

                    if (WeightFlag)
                        fout << "(" << lst.weight << ")" << " ";
                    else
                        fout << " ";
                }

                fout << endl;
            }

            fout.close();
        }

        void DisplayAdjacencyList() {
            if (WeightFlag)
                cout << "Weight graph" << endl;
            else
                cout << "Not Weight graph" << endl;

            if (OrientFlag)
                cout << "Oriented graph" << endl;
            else
                cout << "Not Oriented graph" << endl;

            cout << "Count: " << count << endl;

            for (auto node : AdjacencyList) {
                cout << node.first << " ";

                for (auto &lst : node.second) {
                    cout << lst.data;
                    if (WeightFlag)
                        cout << "(" << lst.weight << ")" << " ";
                    else
                        cout << " ";
                }

                cout << endl;
            }
        }

        void AdjacencyListToEdgeList() {
            EdgeList.clear();

            for (auto node : AdjacencyList) {
                for (auto &lst : node.second) {
                    EdgeList.insert(make_pair(make_pair(node.first, lst.data), lst.weight));
                }
            }
        }

        void DisplayEdgeList() {
            AdjacencyListToEdgeList();

            if (WeightFlag)
                cout << "Weight graph" << endl;
            else 
                cout << "Not Weight graph" << endl;

            if (OrientFlag)
                cout << "Oriented graph" << endl;
            else
                cout << "Not Oriented graph" << endl;

            cout << "Count: " << count << endl;

            for (auto node : EdgeList) {
                if (WeightFlag)
                    cout << node.first.first << " " << node.first.second << " " << node.second << endl;
                else
                    cout << node.first.first << " " << node.first.second << endl;
            }
        }

        Graph operator=(Graph g) {
            return Graph(g.count, g.WeightFlag, g.OrientFlag, g.GetAdjacencyList(), g.GetEdgeList());
        }
};
#endif
