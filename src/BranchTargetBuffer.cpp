/*
* The cache model we use which utilises a doubly linked list to keep track of LRU / MRU with Head / Tail
* We use an array to keep track of data in cache
*
*
*/
#include "../include/BranchTargetBuffer.h"

BTBEntry::BTBEntry(int src, int tgt) :
    sourceAddr_tag(src), targetAddr_data(tgt), prev(nullptr), next(nullptr) {}


BranchTargetBuffer::BranchTargetBuffer(int cap) : capacity(cap) {
    // Setting head and tail values
    head = new BTBEntry(-1, -1);
    tail = new BTBEntry(-1, -1);
    head -> next = tail; // We use a wrap-around structure to move to the head
    tail -> prev = head;

    // Creating cache array
    entries = new BTBEntry*[capacity];
    for (int i = 0; i < capacity; i++){
        entries[i] = new BTBEntry(-1, -1);
        addToFront(entries[i]);
    }
}

BranchTargetBuffer::~BranchTargetBuffer() {
    for (int i = 0; i < capacity; i++) {
        delete entries[i];
    }
    delete[] entries;
    delete head;
    delete tail;
};



void BranchTargetBuffer::addToFront(BTBEntry *entry) const {
    entry -> next = head -> next;
    entry -> prev = head;
    head  -> next -> prev = entry;
    head -> next = entry;
}

void BranchTargetBuffer::removeNode(const BTBEntry *entry) {
   entry -> prev -> next = entry -> next;
   entry -> next -> prev = entry -> prev;
}

void BranchTargetBuffer::moveToFront(BTBEntry *entry) const {
    removeNode(entry);
    addToFront(entry);
}

int BranchTargetBuffer::getTargetAddress(int sourceAddr) const {
    for (int i = 0; i < capacity; i++){
        if (entries[i] -> sourceAddr_tag == sourceAddr) {
            moveToFront(entries[i]); // Make it MRU
            return entries[i] -> targetAddr_data;
        }
    }
    return -1; // Cache miss
}

void BranchTargetBuffer::insert(int sourceAddr, int targetAddr) const {
    // Check if already in cache before inserting
    for (int i = 0; i < capacity; i++){
        if (entries[i] -> sourceAddr_tag == sourceAddr){
            moveToFront(entries[i]);
            return;
        }
    }

    // evict tail -> prev and move new node to beginning of list
    BTBEntry* lru_node = tail -> prev;
    lru_node -> sourceAddr_tag = sourceAddr;
    lru_node -> targetAddr_data = targetAddr;
    moveToFront(lru_node);

}






