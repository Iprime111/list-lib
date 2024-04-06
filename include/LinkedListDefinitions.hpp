#ifndef LINKED_LIST_DEFINITIONS_HPP_
#define LINKED_LIST_DEFINITIONS_HPP_

#include <stddef.h>
#include <sys/types.h>

#define ListTemplate template <typename Element, Comparator <Element *> ElementComparator>

namespace LinkedList {
    const size_t REALLOC_SCALE = 2;

    enum ListErrorCode {
        NO_LIST_ERRORS        = 0,
        LIST_NULL_POINTER     = 1 << 0,
        PREV_NULL_POINTER     = 1 << 1,
        NEXT_NULL_POINTER     = 1 << 2,
        DATA_NULL_POINTER     = 1 << 3,
        FREE_LIST_ERROR       = 1 << 4,
        WRONG_INDEX           = 1 << 5,
        GRAPHVIZ_BUFFER_ERROR = 1 << 6,
        LOG_FILE_ERROR        = 1 << 7,
        INVALID_CAPACITY      = 1 << 8,
        INVALID_HEAD          = 1 << 9,
        INVALID_TAIL          = 1 << 10,
    };

    struct CallingFileData {
        int line             = -1;
        const char *file     = nullptr;
        const char *function = nullptr;
    };

    template <typename ElementPointer>
    using Comparator = int (*) (ElementPointer firstElement, ElementPointer secondElement);

    ListTemplate
    struct List {
        Element  *data = nullptr;
        ssize_t *next = nullptr;
        ssize_t *prev = nullptr;

        ssize_t size     = -1;
        ssize_t capacity = -1;
        ssize_t freeElem = -1;

        ListErrorCode errors;
        CallingFileData creationData;
    };

    ListTemplate
    ListErrorCode InitList_    (List <Element, ElementComparator> *list, size_t capacity, CallingFileData creationData);
    ListTemplate
    ListErrorCode DestroyList_ (List <Element, ElementComparator> *list);
    ListTemplate
    ListErrorCode InsertAfter_ (List <Element, ElementComparator> *list, ssize_t insertIndex, 
                                ssize_t *newIndex, Element *element, CallingFileData callData);
    ListTemplate
    ListErrorCode DeleteValue_ (List <Element, ElementComparator> *list, ssize_t deleteIndex, CallingFileData callData);
    ListTemplate
    ListErrorCode VerifyList_  (List <Element, ElementComparator> *list);
    ListTemplate
    ListErrorCode DumpList_    (List <Element, ElementComparator> *list, char *logFolder, CallingFileData callData);
    ListTemplate
    ListErrorCode FindValue_   (List <Element, ElementComparator> *list, Element *value, ssize_t *index, CallingFileData callData);
    ListTemplate
    ListErrorCode ReallocUp_   (List <Element, ElementComparator> *list, CallingFileData callData);

    ListErrorCode ClearHtmlFile ();

    #define CreateCallingFileData {__LINE__, __FILE__, __PRETTY_FUNCTION__}

    #define InitList(list, capacity)                          InitList_    (list, capacity, CreateCallingFileData)
    #define InsertAfter(list, insertIndex, newIndex, element) InsertAfter_ (list, insertIndex, newIndex, element, CreateCallingFileData)
    #define DeleteValue(list, deleteIndex)                    DeleteValue_ (list, deleteIndex, CreateCallingFileData)
    #define DumpList(list, logFolder)                         DumpList_    (list, logFolder, CreateCallingFileData)
    #define DestroyList(list)                                 DestroyList_ (list)
    #define VerifyList(list)                                  VerifyList_  (list)

    #define FindValue(list, value, index) FindValue_ (list, value, index, CreateCallingFileData);

}

#undef ListTemplate

#endif
