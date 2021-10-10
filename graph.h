#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct RelatedNode {
    string data;
    int weight;
    struct RelatedNode *next = nullptr;
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

// Add console

class Graph {
    private:
        int count;
        bool WeightFlag, OrientFlag; 

        map<string, RelatedNode*> AdjacencyList;
        map<pair<string, string>, int> EdgeList;

        void AdjacencyListToEdgeList() {
            for (auto node : AdjacencyList) {
                struct RelatedNode *end = node.second;

                while (end) {
                    EdgeList.insert(make_pair(make_pair(node.first, end->data), end->weight));
                    end = end->next;
                }
            }
        }

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
                    cout << str_node << endl;
                }

                cout << endl;

                // set AdjacencyList
                RelatedNode *RelatedList, *pos;

                if (nodes.size() - 1 > 0) {
                    RelatedList = new RelatedNode();
                    pos = RelatedList;
                }
                else {
                    AddNode(make_pair(nodes[0], nullptr));
                    continue;
                }

                for (int id = 1; id < nodes.size(); ++id) {
                    istringstream ss(nodes[id]);

                    cout << "str_node:" << nodes[id] << endl;

                    if (!WeightFlag)
                        pos->data = str_node;

                    while (getline(ss, str_node, '(')) {
                        if (str_node[str_node.size() - 1] != ')') {
                            pos->data = str_node;

                            if (!OrientFlag) {
                                if (!AdjacencyList.count(nodes[0])) {
                                    RelatedNode *lst = GetRelatedList(str_node);
                                    while (lst->next)
                                        lst = lst->next;
                                    lst->next = new RelatedNode();
                                    lst->data = pos->data;
                                }
                            }
                        }
                        else {
                            pos->weight = stoi(str_node.substr(0, str_node.size() - 1));

                            if (!OrientFlag) {
                                RelatedNode *lst = GetRelatedList(str_node);
                                while (lst->next)
                                        lst = lst->next;
                                lst->next = new RelatedNode();
                                lst->weight = pos->weight;
                            }
                        }
                    }

                    if (id != nodes.size() - 1) {
                        pos->next = new RelatedNode();
                        pos = pos->next;
                    }
                }

                if (nodes.size())
                    AddNode(make_pair(nodes[0], RelatedList));

            }

            fin.close();
        }

        Graph(int c, bool Weight, bool Oriented, map<string, RelatedNode*> AdjList, map<pair<string, string>, int> EList) {
            count = c;
            WeightFlag = Weight;
            OrientFlag = Oriented;
            AdjacencyList = AdjList;
            EdgeList = EList;
        }

        int GetCount() {
            return count;
        }

        map<string, RelatedNode*> GetAdjacencyList() {
            return AdjacencyList;
        }

        map<pair<string, string>, int> GetEdgeList() {
            return EdgeList;
        }

        void AddNode(pair<string, RelatedNode*> node) {
            AdjacencyList.insert(node);
        }

        void AddEdge(string begin, string end, int weight) {
            EdgeList.insert(make_pair(make_pair(begin, end), weight));
        }

        // Del in RelatedList
        void DelNode(string data) {
            AdjacencyList.erase(data);
        }

        void DelEdge(string begin, string end) {
            EdgeList.erase(make_pair(begin, end));
        }

        RelatedNode* GetRelatedList(string key) {
            return AdjacencyList[key];
        }

        void WriteInFile(string path) {
            ofstream fout(path);

            fout << count << endl;
            for (auto node : AdjacencyList) {
                fout << node.first << " ";
                struct RelatedNode *end = node.second;

                while (end) {
                    fout << end->data;
                    if (WeightFlag)
                        fout << "(" << end->weight << ")" << " ";
                    end = end->next;
                }

                fout << endl;
            }

            fout.close();
        }

        Graph operator=(Graph g) {
            return Graph(g.count, g.WeightFlag, g.OrientFlag, g.GetAdjacencyList(), g.GetEdgeList());
        }
};
#endif
