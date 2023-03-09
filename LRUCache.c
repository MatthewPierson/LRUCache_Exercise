/// @author Matthew Pierson
/// @date 09/03/2023
/// Compiliation: gcc LRUCache.c -o LRUCache

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    // Store a pointer to the next and previous nodes, making this a doubly linked list 
    struct Node *next, *prev;
    // Store the unique key and its value
    int key, value;
} Node;

typedef struct LRUCache {
    // Store a pointer to the start of the LRUCache and to the end
    struct Node *start, *end;
    // Store the capacity of the LRUCache and the current number of total nodes in the LRUCache
    int capacity, nodeTotal;
} LRUCache;

/// @brief Set the capacity of a given LRUCache
/// @param q pointer of LRUCache to set the capacity of
/// @param capacity value to set the capacity of the LRUCache to
/// @return 0 if the capacity of the LRUCache was updated, -1 if the value given is under 1 or over 100000, otherwise -2
int setCapacity(LRUCache* q, int capacity) {
    if (!q) {
        return -2;
    }
    // Check if given capacity is smaller or larger than outlined in the constraints
    if (capacity < 1 || capacity > 100000) {
        return -1;
    }
    // Set the new capacity for the cache
    q->capacity = capacity;
    return 0;
}

/// @brief Set the total number of Nodes in the LRUCache
/// @param q pointer of LRUCache to update the total number of Nodes
/// @param nodeTotal number of total nodes to set
/// @return 0 if the total number of Nodes in the LRUCache were updated, -1 if the value given is under 0 or over the capacity of the LRUCache, otherwise -2
int setNodeTotal(LRUCache* q, int nodeTotal) {
    if (!q) {
        return -2;
    }
    // Check if the given number of total Nodes is smaller than 0 or larger than the caches set capacity
    if (nodeTotal < 0 || nodeTotal > q->capacity) {
        return -1;
    }
    // Set the new total number of Nodes for the cache
    q->nodeTotal = nodeTotal;
    return 0;
}

/// @brief Get the value of a given key in a given LRUCache if present in LRUCache and moves the found Node to the front of the LRUCache
/// @param q pointer of LRUCache to be searched
/// @param key value of key to be searched for
/// @return value if found, -1 if not
int get(LRUCache* q, int key) {

    // Check if LRUCache is valid and contains at least 1 node
    if (!q || q->nodeTotal == 0) {
        return -1;
    }

    // Begin iterating over all the nodes in the LRUCache
    Node* currentNode = q->start;
    // Loop breaks when the current node is the end of the LRUCache
    while (currentNode != q->end) {
        // Compare the current nodes key with the given key
        if (currentNode->key == key) {
            // If the keys match, return its value and move the Node to the front of the LRUCache
            if (currentNode != q->start) {
                // Current node is not the start of the LRUCache
                Node* tempPrevNode = currentNode->prev;
                Node* tempNextNode = currentNode->next;
                // Remove the current node by setting the next nodes previous node to the current nodes previous node
                tempPrevNode->next = tempNextNode;
                // Remove the current node by setting the previous nodes next node to the current nodes next node
                tempNextNode->prev = tempPrevNode;
                // Move the current node to the start of the LRUCache
                currentNode->next = q->start;
                q->start = currentNode;
            }
            printf("%s: Returning value: %d for key: %d\n\n", __func__, currentNode->value, key);
            return currentNode->value;
        }
        // If the keys do not match, set the current node to the next in the LRUCache
        currentNode = currentNode->next;
    }
    // Catch the final node after breaking out of the loop
    // Compare the current nodes key with the given key
    if (currentNode->key == key) {
        // If the keys match, return its value
        // Get a pointer to the second to last node in the LRUCache and set its next node to NULL
        Node* tempNode = currentNode->prev;
        int wasStart = 0;
        if (tempNode = q->start) {
            wasStart = 1;
        }
        tempNode->next = NULL;
        // Set the pointer to the end of the LRUCache to the current second to last node
        q->end = tempNode;
        // Move the current node to the start of the LRUCache
        currentNode->next = q->start;
        q->start = currentNode;
        // If the LRUCache only has two Nodes, the new end will not have a pointer to a previous Node so we set it here
        if (wasStart) {
            q->end->prev = currentNode;
        }
        printf("%s: Returning value: %d for key: %d\n\n", __func__, currentNode->value, key);
        return currentNode->value;
    }
    // No matching key was found, return -1
    printf("%s: Could not find Node for key %d, returning -1\n\n", __func__, key);
    return -1;
}

/// @brief Put a new node in the LRUCache at the start of the LRUCache with a given key and value, if the key doesn't already exist in the LRUCache
/// @param q pointer of LRUCache to add the new node to
/// @param key key value of the node to be created
/// @param value value of the node to be created
void put(LRUCache* q, int key, int value) {

    // Check if LRUCache is valid
    if (!q) {
        return;
    }
    // If the LRUCache has no nodes then we handle that special case here
    if (q->nodeTotal == 0) {
        // Allocate memory for a new node
        Node* node = (Node*)malloc(sizeof(Node));
        // Set the values of the node to the given values
        node->key = key;
        node->value = value;
        node->next = NULL;
        // Set the start and end of the LRUCache to the newly created node
        q->start = node;
        q->end = node;
        printf("%s: Creating new start Node (Key: %d, Value: %d)\n\n", __func__, key, value);
        // Update the the total number of nodes in the LRUCache
        setNodeTotal(q, 1);

        return;
    }

    // Begin iterating over all the nodes in the LRUCache to check if node with given key already exists
    Node* currentNode = q->start;
    // Loop breaks when the current node is the end of the LRUCache
    while (currentNode != q->end) {
        // Check if current node matches the given key
        if (currentNode->key == key) {
            printf("%s: Node with key already exists, not creating new Node\n\n", __func__, key, value);
            // The key already exists in the LRUCache, return and do not add a new node
            return;
        }
        // If the keys do not match, set the current node to the next in the LRUCache
        currentNode = currentNode->next;
    }

    // Catch the final node after breaking out of the loop
    // Compare the current nodes key with the given key
    if (currentNode->key == key) {
        printf("%s: Node with key already exists, not creating new Node\n\n", __func__, key, value);
        // If the keys match, return and do not add a new node
        return;
    }

    // Check if adding a new node would push the LRUCache over its capacity
    if (q->nodeTotal + 1 > q->capacity) {
        // Get a pointer to the second to last node in the LRUCache and set its next node to NULL
        printf("%s: Max capacity reached, removing Least Recently Used Node (Key: %d, Value: %d)\n", __func__, q->end->key, q->end->value);
        Node* tempNode = q->end->prev;
        tempNode->next = NULL;
        // Set the pointer to the end of the LRUCache to the current second to last node
        q->end = tempNode;
    }
    // Allocate memory for a new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    // Set the values of the node to the given values
    newNode->key = key;
    newNode->value = value;
    // Get the current start of the LRUCache
    Node* tempNode = q->start;
    // Set the current start of the LRUCaches previous node to be the newly created node
    tempNode->prev = newNode;
    // Set the newly created nodes next node to be the current start node
    newNode->next = tempNode;
    // Set the start node of the LRUCache to be the newly created node
    q->start = newNode;
    printf("%s: Added new Node to start of cache (Key: %d, Value: %d)\n\n", __func__, key, value);
    // Add 1 to the total number of nodes
    setNodeTotal(q, q->nodeTotal + 1);

    return;

}

/// @brief Delete a node from the LRUCache by a given key, if that key is found in the LRUCache
/// @param q pointer of LRUCache to delete a node from
/// @param key key value of the node to be deleted
/// @return value of the deleted node if found, otherwise -1
int delete(LRUCache* q, int key) {
    // Check if LRUCache is valid and if it contains a start node
    if (!q || !q->start) {
        return -1;
    }
    // Begin iterating over all the nodes in the LRUCache to check if node with given key exists
    Node* currentNode = q->start;
    // Loop breaks when the current node is the end of the LRUCache
    while (currentNode->next) {
        // Check if the current nodes key matches the given key
        if (currentNode->key == key) {
            // Check if the found node is the start of the LRUCache
            if (currentNode != q->start) {
                // Current node is not the start of the LRUCache
                Node* tempPrevNode = currentNode->prev;
                Node* tempNextNode = currentNode->next;
                // Remove the current node by setting the next nodes previous node to the current nodes previous node
                tempPrevNode->next = tempNextNode;
                // Remove the current node by setting the previous nodes next node to the current nodes next node
                tempNextNode->prev = tempPrevNode;
            } else {
                // Current node is the start of the LRUCache
                Node* tempNextNode = currentNode->next;
                // Remove the current node by setting the LRUCaches starting node to the current nodes next node
                q->start = tempNextNode;
                // Remove the current node by setting the new starting nodes next node to NULL
                q->start->prev = NULL;
            }
            // Minus 1 from the total number of nodes
            setNodeTotal(q, q->nodeTotal - 1);
            // Return the deleted nodes value
            printf("%s: Deleted Node for key %d, returning value: %d\n\n", __func__, currentNode->value, key);
            return currentNode->value;
        }
        // Set the current node to its next node
        currentNode = currentNode->next;
    }
    // Catch the final node
    // Compare the final nodes key to the given key
    if (currentNode->key == key) {
            Node* tempPrevNode = currentNode->prev;
            // Set the second to last nodes next node to NULL
            tempPrevNode->next = NULL;
            // Set the LRUCaches end node to the currnet second to last node
            q->end = tempPrevNode;
            // Minus 1 from the total number of nodes
            setNodeTotal(q, q->nodeTotal - 1);
            // Return the deleted nodes value
            printf("%s: Deleted Node for key %d, returning value: %d\n\n", __func__, currentNode->value, key);
            return currentNode->value;
        }
    // No matching key was found, return -1
    printf("%s: Could not find Node for key %d, returning -1\n\n", __func__, key);
    return -1;

}

/// @brief Main function for the LRUCache example. Creates a 
int main(void) {

    // Define the capacity for the LRUCache
    int capacity = 2;

    // Allocate memory for the LRUCache
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    // Check memory was allocated
    if (!cache) {
        return -1;
    }

    // Set the capacity of the LRUCache
    setCapacity(cache, capacity);
    // Set the total number of Nodes to 0 for the LRUCache
    setNodeTotal(cache, 0);

    // Add Node with Key: 1, Value: 1
    put(cache, 1, 1);
    // Add Node with Key: 2, Value: 2
    put(cache, 2, 2);

    // Get value of Node with key = 1
    // Should return/print 1
    get(cache, 1);

    // Add Node with Key: 3, Value: 3
    // Will remove node with key 2
    put(cache, 3, 3);

    // Get value of Node with key = 2
    // Should return/print -1
    get(cache, 2);

    // Add Node with Key: 4, Value: 4
    // Will remove node with key 1
    put(cache, 4, 4);

    // Get value of Node with key = 1
    // Should return/print -1
    get(cache, 1);

    // Get value of Node with key = 3
    // Should return/print 3
    get(cache, 3);

    // Get value of Node with key = 4
    // Should return/print 4
    get(cache, 4);

    // Delete Node with key = 3
    // Should return/print 3
    delete(cache, 3);
    
    // Get value of Node with key = 3
    // Should return/print -1
    get(cache, 3);

    return 0;
}