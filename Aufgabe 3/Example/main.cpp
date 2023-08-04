#include <iostream>
#include <new>

struct Node {
    char data[1024];  // Jeder Knoten belegt etwa 1KB
    Node* next;
};

int main() {
    Node* head = new Node;
    Node* current = head;

    // Versuchen, 1.000.000 Knoten zu erstellen, was etwa 1GB Speicher benötigt
    for (int i = 0; i < 1000000; ++i) {
        try {
            current->next = new Node;
            current = current->next;
            // Speicher tatsächlich verwenden
            for (int j = 0; j < 1024; ++j) {
                current->data[j] = j % 256;
            }
        } catch (std::bad_alloc& ba) {
            std::cerr << "bad_alloc caught: " << ba.what() << '\n';
            break;
        }
    }

    std::cout << "Finished allocating memory\n";

     //Gebrauchten Speicher freigeben
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }

    return 0;
}
