#include "LinkedListDefinitions.hpp"
#include <gtest/gtest.h>

#include <LinkedList.hpp>

inline int CharComparator (char *firstValue, char *secondValue) {
    return *firstValue - *secondValue;
}

TEST (InitTest, ListTests) {
    LinkedList::List <char, CharComparator> list = {};

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
    LinkedList::List <char, CharComparator> list = {};
    const size_t listSize = 10;

    EXPECT_EQ (LinkedList::InitList (&list, listSize), LinkedList::NO_LIST_ERRORS);

    ssize_t insertIndex = 0;

    for (char insertedData = 'a'; insertedData < 'a' + (char) listSize; insertedData++) {
        EXPECT_EQ (InsertAfter (&list, insertIndex, &insertIndex, &insertedData), LinkedList::NO_LIST_ERRORS);
        EXPECT_EQ (list.data [insertIndex], insertedData);
    }

    EXPECT_EQ (LinkedList::DestroyList (&list), LinkedList::NO_LIST_ERRORS);
}

TEST (ReallocTest, ListTests) {
    LinkedList::List <char, CharComparator> list = {};
    const size_t listSize = 5;

    EXPECT_EQ (LinkedList::InitList (&list, listSize), LinkedList::NO_LIST_ERRORS);

    ssize_t insertIndex = 0;

    for (char insertedData = 'a'; insertedData < 'a' + (char) listSize * 2; insertedData++) {
        EXPECT_EQ (InsertAfter (&list, insertIndex, &insertIndex, &insertedData), LinkedList::NO_LIST_ERRORS);
        EXPECT_EQ (list.data [insertIndex], insertedData);
    }

    EXPECT_EQ (LinkedList::DestroyList (&list), LinkedList::NO_LIST_ERRORS);
}

//TODO unit tests for search and deletion

