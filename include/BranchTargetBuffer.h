//
// Created by root on 3/21/25.
//

#ifndef BRANCHTARGETBUFFER_H
#define BRANCHTARGETBUFFER_H

// Node for the doubly linked list
struct BTBEntry {
    int sourceAddr_tag; // Tag (source address of the branch)
    int targetAddr_data; // Data (target address of the branch)
    BTBEntry* prev; // Pointer to the previous node
    BTBEntry* next; // Pointer to the next node

    BTBEntry(int src, int tgt);
};

class BranchTargetBuffer {
private:
    // Variables goes here
    int capacity; // Maximum number of entries in the cache
    BTBEntry* head; // Dummy head (most recently used)
    BTBEntry* tail; // Dummy tail (least recently used)
    BTBEntry** entries; // Fixed-size array to store cache entries

    // Helper functions for managing the doubly linked list
    void addToFront(BTBEntry* entry) const;

    static void removeNode(const BTBEntry* entry);

    void moveToFront(BTBEntry* entry) const;

public:

    BranchTargetBuffer(int cap);

    ~BranchTargetBuffer();

    int getTargetAddress(int sourceAddr) const;

    void insert(int sourceAddr, int targetAddr) const;

};



#endif //BRANCHTARGETBUFFER_H
