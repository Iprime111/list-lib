#include <gtest/gtest.h>

#include "LinkedList.hpp"

TEST (InitTest, ListTests) {
    LinkedList::List <char> list = {};

    EXPECT_EQ (LinkedList::InitList (&list, 10), LinkedList::NO_LIST_ERRORS);

    EXPECT_EQ (list.prev [0], 0);
    EXPECT_EQ (list.next [0], 0);
    EXPECT_GT (list.freeElem, 0);


    for (ssize_t listIndex = list.freeElem; listIndex < list.capacity; listIndex++) {
        EXPECT_EQ (list.prev [listIndex], -1);
    }

    EXPECT_EQ (LinkedList::DestroyList (&list), LinkedList::NO_LIST_ERRORS);
}

TEST (InsertTest, ListTests) {
    LinkedList::List <char> list = {};

    EXPECT_EQ (LinkedList::InitList (&list, 10), LinkedList::NO_LIST_ERRORS);

    ssize_t insertIndex  = 0;
    char    insertedData = 'a';

    while (list.freeElem != 0) {
        EXPECT_EQ (InsertAfter (&list, insertIndex, &insertIndex, insertedData), LinkedList::NO_LIST_ERRORS);
        EXPECT_EQ (list.data [insertIndex], insertedData);

        insertedData++;
    }

    EXPECT_NE (InsertAfter (&list, insertIndex, &insertIndex, insertedData), LinkedList::NO_LIST_ERRORS);

    EXPECT_EQ (LinkedList::DestroyList (&list), LinkedList::NO_LIST_ERRORS);
}

//TODO unit tests for search and deletion

