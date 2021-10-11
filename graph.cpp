#include "graph.h"

using namespace std;

int main() {
    // Oriented-Weight
    // 1 - weight flag(if not than without brackets)
    // 1 - orient flag(if not than dup)
    // 5 - count of nodes
    // 1 2(34) 3(25) 5(14)
    // 2 4(12) 5(14)
    // 3 5(27)
    // 4 1(25)
    // 5 1(26)

    // Oriented-NotWeight
    // 0 - weight flag(if not than without brackets)
    // 1 - orient flag(if not than dup)
    // 5 - count of nodes
    // 1 2 3 5
    // 2 4 5
    // 3 5
    // 4 1
    // 5 1

    // NoOriented-Weight
    // 1 - weight flag(if not than without brackets)
    // 0 - orient flag(if not than dup)
    // 5 - count of nodes
    // 1 2(34) 3(25) 5(14)
    // 2 4(12) 5(14)
    // 3 5(27)
    // 4 1(25)
    // 5 1(26)

    // NoOriented-NotWeight
    // 0 - weight flag(if not than without brackets)
    // 0 - orient flag(if not than dup)
    // 5 - count of nodes
    // 1 2 3 5
    // 2 4 5
    // 3 5
    // 4 1
    // 5 1

    Graph g("input1.txt");
    
    // g.DelNode("3");
    
    /**
    RelatedNode *lst = new RelatedNode();
    lst->data = "1";
    lst->weight = 26;
    g.AddNode(make_pair("7", lst));
    **/
    
    g.WriteInFile("output.txt");
    cout << g.GetCount() << endl;
    g.DisplayAdjacencyList();

    /**
    cout << "GetCount:" << g.GetCount() << endl;

    Graph g2("input2.txt");
    g = g2;
    cout << "GetCount:" << g.GetCount() << endl;
    **/
}
