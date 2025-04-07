#include<iostream>
#include<stdlib.h>
#include<queue>
#include<omp.h>

using namespace std;

class node {
public:
    node *left, *right;
    int data;

    node(int val) {
        data = val;
        left = right = NULL;
    }
};

class Breadthfs {
public:
    node* insert(node* root, int data);
    void bfs(node* head);
};

node* Breadthfs::insert(node* root, int data) {
    if (!root) {
        return new node(data);
    }

    queue<node*> q;
    q.push(root);

    while (!q.empty()) {
        node* temp = q.front();
        q.pop();

        if (temp->left == NULL) {
            temp->left = new node(data);
            return root;
        } else {
            q.push(temp->left);
        }

        if (temp->right == NULL) {
            temp->right = new node(data);
            return root;
        } else {
            q.push(temp->right);
        }
    }

    return root;
}

void Breadthfs::bfs(node* head) {
    if (!head) return;

    queue<node*> q;
    q.push(head);

    while (!q.empty()) {
        int qSize = q.size();
        vector<node*> levelNodes(qSize);

        // Extract nodes from the queue
        for (int i = 0; i < qSize; ++i) {
            levelNodes[i] = q.front();
            q.pop();
        }

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            node* currNode = levelNodes[i];

            #pragma omp critical
            {
                cout << "\t" << currNode->data;
            }

            #pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);
                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
}

int main() {
    node* root = NULL;
    int data;
    char ans;
    Breadthfs tree;

    do {
        cout << "\nEnter data => ";
        cin >> data;

        root = tree.insert(root, data);

        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    cout << "\nBreadth First Traversal (Parallelized):\n";
    tree.bfs(root);
    cout<<"\n";

    return 0;
}

