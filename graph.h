#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

struct RelatedNode {
    auto data;
    int weight;
    struct RelatedNode *next = nullptr;
};

// AdjacencyList
// 5
// 1 2(34) 3(25) 5(14)
// 2 4(12) 5(14)
// 3 5(27)
// 4 1(25)
// 5

// EdgeList
// 5
// 1 2 34
// 1 3 25
// 1 5 14
// 2 4 12
// 2 5 14
// 3 5 27
// 4 1 25
// 5

// Учесть ориентированность и вес

class Graph {
    private:
        int count;

        map<auto, RelatedNode*> AdjacencyList;
        map<pair<auto, auto>, int> EdgeList;

        void AdjacencyListToEdgeList() {
            for (auto node : AdjacencyList) {
                struct RelatedNode *end = node.second;

                while (end) {
                    EdgeList.insert({node.first, end->data}, end->weight);
                    end = end->next;
                }
            }
        }

    public:
        Graph() {}
        
        Graph(string path) {
            ifstream fin(path);

            fin >> count;

            for (int index = 0; index < count; ++index) {
                string str;

                // split string
                vector<string> nodes;

                while (getline(fin, str, ' '))
                    nodes.push_back(str);

                // set AdjacencyList
                RelatedNode *RelatedList, *pos;

                if (nodes.size() - 1 > 0)
                    RelatedList = new RelatedNode();
                    pos = RelatedList;
                else {
                    AdjacencyList.AddNode(nodes[0], nullptr);
                    continue;
                }

                for (int id = 1; id < nodes.size(); ++id) {
                    stringstream ss(nodes[id]);

                    while (getline(ss, str, '(')) {
                        if (str[str.size() - 1] != ')')
                            pos->data = str;
                        else
                            pos->weight = stoi(str.substr(0, str.size() - 1));
                    }

                    pos = pos->next;
                    pos = new RelatedNode();
                }

                AdjacencyList.AddNode({nodes[0], RelatedList});

            }

            fin.close();
        }

        int GetCount() {
            return count;
        }

        map<auto, RelatedNode*> GetAdjacencyList() {
            return AdjacencyList;
        }

        map<pair<auto, auto>, int> GetEdgeList() {
            return EdgeList;
        }

        void AddNode(pair<auto, RelatedNode*> node) {
            AdjacencyList.insert(node);
        }

        void AddEdge(auto begin, auto end, int weight) {

        }

        void DelNode(auto data) {
            AdjacencyList.erase(data);
        }

        void DelEdge(auto begin, auto end) {}

        void WriteInFile(string path) {
            ofstream fout(path);

            fout << count << endl;
            for (int index = 0; index < count; ++index) {
                fout << AdjacencyList[index] << " ";
                struct RelatedNode *end = AdjacencyList[ListIndex]->RelatedList;

                while (end) {
                    fout << end->data << "(" << end->weight << ")" << " ";
                    end = end->next;
                }

                fout << endl;
            }

            fout.close();
        }

        =operator(Graph g) {
            if (g.GetCount() != count) {
                // throw exception
            }

            AdjacencyList = g.GetAdjacencyList();
            EdgeList = g.GetEdgeList();
        }
}
#endif