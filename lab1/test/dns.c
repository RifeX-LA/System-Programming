
/*************************************************************************
   LAB 1                                                                

    Edit this file ONLY!

*************************************************************************/



#include "dns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_CAPACITY 100000

typedef struct HashtableItem HashtableItem;
struct HashtableItem {
    char* key;
    IPADDRESS value;
};

typedef struct List List;
struct List {
    HashtableItem* item;
    List* next;
};


typedef struct hashtable hashtable;
struct hashtable {
    HashtableItem** items;
    List** buckets;
    int capacity;
    int size;
};

unsigned int getNumOfLines(FILE* fInput);

unsigned long hash(const char* str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++) {
        i += str[j];
    }
    return i % HASHTABLE_CAPACITY;
}

static List* createList() {
    List* list = (List*)malloc(sizeof(List));
    return list;
}

static List* listInsert(List* list, HashtableItem* item) {
    if (!list) {
        List* head = createList();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }
    else if (list->next == NULL) {
        List* node = createList();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    List* temp = list;
    while (temp->next!=NULL) {
        temp = temp->next;
    }

    List* node = createList();
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

static HashtableItem* listRemove(List* list) {
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list || !list->next) {
        return NULL;
    }

    List* node = list->next;
    List* temp = list;
    temp->next = NULL;
    list = node;
    HashtableItem* it = NULL;
    memcpy(temp->item, it, sizeof(HashtableItem));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);

    return it;
}

static void listClear(List* list) {
    while (list) {
        List* temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item);
        free(temp);
    }
}

static List** createBuckets(hashtable* table) {
    List** buckets = (List**)calloc(table->capacity, sizeof(List*));
    for (int i = 0; i < table->capacity; i++) {
        buckets[i] = NULL;
    }
    return buckets;
}

static void bucketsClear(hashtable* table) {
    List** buckets = table->buckets;
    for (int i = 0; i < table->capacity; i++) {
        listClear(buckets[i]);
    }
    free(buckets);
}


HashtableItem* createItem(char* key, IPADDRESS value) {
    HashtableItem* item = (HashtableItem*)malloc(sizeof(HashtableItem));
    item->key = (char*)malloc(strlen(key) + 1);
    item->value = value;
    strcpy(item->key, key);

    return item;
}

hashtable* createHashtable(int cap) {
    hashtable* table = (hashtable*)malloc(sizeof(hashtable));
    table->capacity = cap;
    table->size = 0;
    table->items = (HashtableItem**)calloc(table->capacity, sizeof(HashtableItem*));
    for (int i = 0; i < table->capacity; i++) {
        table->items[i] = NULL;
    }
    table->buckets = createBuckets(table);

    return table;
}

void itemClear(HashtableItem* item) {
    free(item->key);
    free(item); 
}

void hashtableClear(hashtable* table) {
    for (int i = 0; i < table->capacity; i++) {
        HashtableItem* item = table->items[i];
        if (item != NULL)
            itemClear(item);
    }

    bucketsClear(table);
    free(table->items);
    free(table);
}

void handleCollision(hashtable* table, unsigned long index, HashtableItem* item) {
    List* head = table->buckets[index];
    table->buckets[index] = listInsert(head, item);
}

void hashtableInsert(hashtable* table, char* key, IPADDRESS value) {
    HashtableItem* item = createItem(key, value);
    unsigned long index = hash(key);

    HashtableItem* current_item = table->items[index];
    if (current_item == NULL) {
        table->items[index] = item;
        table->size++;
    }
    else {
        if (strcmp(current_item->key, key) == 0) {
            table->items[index]->value = value;
        }
        else {
            handleCollision(table, index, item);
        }
    }
}

IPADDRESS hashtableFind(hashtable* table, char* key) {
    int index = hash(key);
    HashtableItem* item = table->items[index];
    List* head = table->buckets[index];

    while (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->value;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }

    return NULL;
}

hashtable** DNSTables[50];
DNSHandle DNSCounter = (DNSHandle)1;

DNSHandle InitDNS() {
    DNSTables[DNSCounter] = createHashtable(HASHTABLE_CAPACITY);
    DNSCounter += 1;
    return DNSCounter-1;
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath) {
    FILE* fInput = NULL;
    unsigned int dnCount = 0;
    unsigned int i = 0;

    fInput = fopen("../hosts", "r");
    if (NULL == fInput)
        return;

    dnCount = getNumOfLines(fInput);
    if (dnCount == 0) {
        fclose(fInput);
        return;
    }

    fseek(fInput, 0, SEEK_SET);
    for (i = 0; i < dnCount && !feof(fInput); i++) {
        char buffer[201] = {0};
        char* pStringWalker = &buffer[0];
        unsigned int uHostNameLength = 0;
        unsigned int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;

        fgets(buffer, 200, fInput);
        if (fscanf_s(fInput, "%d.%d.%d.%d %s", &ip1, &ip2, &ip3, &ip4, buffer, 200) != 5)
            continue;

        IPADDRESS ip = (ip1 & 0xFF) << 24 |
                       (ip2 & 0xFF) << 16 |
                       (ip3 & 0xFF) << 8 |
                       (ip4 & 0xFF);

        char* domainName;
        uHostNameLength = strlen(buffer);
        if (uHostNameLength) {
            domainName = (char*)malloc(uHostNameLength + 1);
            strcpy(domainName, pStringWalker);
            hashtableInsert(DNSTables[hDNS], domainName, ip);
        }
    }
    
    fclose(fInput);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName) {
    return hashtableFind(DNSTables[hDNS], hostName);
}

void ShutdownDNS(DNSHandle hDNS) {
    hashtableClear(DNSTables[hDNS]);
}
