#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
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
                    EdgeList.insert(make_pair(node.first, end->data), end->weight);
                    end = end->next;
                }
            }
        }

    public:
        Graph() {}
        
        Graph(string path) {
            ifstream fin(path);

            fin >> WeightFlag >> OrientFlag >> count;

            for (int index = 0; index < count; ++index) {
                string str;

                // split string
                vector<string> nodes;

                while (getline(fin, str, ' '))
                    nodes.push_back(str);

                // set AdjacencyList
                RelatedNode *RelatedList, *pos;

                if (nodes.size() - 1 > 0) {
                    RelatedList = new RelatedNode();
                    pos = RelatedList;
                }
                else {
                    AddNode(nodes[0], nullptr);
                    continue;
                }

                for (int id = 1; id < nodes.size(); ++id) {
                    stringstream ss(nodes[id]);

                    if (!WeightFlag)
                        pos->data = str;

                    while (getline(ss, str, '(')) {
                        if (str[str.size() - 1] != ')') {
                            pos->data = str;

                            if (!OrientFlag) {
                                if (!AdjacencyList.count(str)) {
                                    RelatedNode *lst = GetRelatedList(str);
                                    while (lst->next)
                                        lst = lst->next;
                                    lst->next = new RelatedNode();
                                    lst->data = pos->data;
                                }
                            }
                        }
                        else {
                            pos->weight = stoi(str.substr(0, str.size() - 1));

                            if (!OrientFlag) {
                                RelatedNode *lst = GetRelatedList(str);
                                while (lst->next)
                                        lst = lst->next;
                                lst->next = new RelatedNode();
                                lst->weight = pos->weight;
                            }
                        }
                    }

                    pos = pos->next;
                    pos = new RelatedNode();
                }

                AddNode(make_pair(nodes[0], RelatedList));

            }

            fin.close();
        }

        Graph(map<string, RelatedNode*> AdjList, map<pair<string, string>, int> EList) {
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
            EdgeList.insert(make_pair(begin, end), weight);
        }

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
            for (int index = 0; index < count; ++index) {
                fout << AdjacencyList[index] << " ";
                struct RelatedNode *end = AdjacencyList[index]->RelatedList;

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

        /**
        Graph Graph::operator=(Graph g) {
            if (g.GetCount() != count) {
                throw -1;
            }

            return Graph(g.GetAdjacencyList(), g.GetEdgeList());
        }
        **/
};
#endif
