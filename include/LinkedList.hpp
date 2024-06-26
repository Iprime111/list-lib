#ifndef LINKED_LIST_HPP_
#define LINKED_LIST_HPP_

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

#ifdef TRACY_ENABLE
    #include <Tracy.hpp>
#endif

#include "LinkedListDefinitions.hpp"

#define ListTemplate template <typename Element, Comparator <Element *> ElementComparator>

#ifndef TRACY_ENABLE
    #define ZoneScoped
#endif

#ifndef NDEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

//TODO dump
#define Verification(list, callData)                        \
    ON_DEBUG (                                              \
        do {                                                \
            ListErrorCode errorCode_ = VerifyList_ (list);  \
            if (errorCode_ != NO_LIST_ERRORS) {             \
                /*DumpList_ (list, ".", callData);*/        \
                return errorCode_;                          \
            }                                               \
        } while (0)                                         \
    )

#define WriteErrors(list, errorCodes)  (list)->errors = (ListErrorCode) ((list)->errors | (errorCodes))
#define ReturnErrors(list, errorCodes) WriteErrors (list, errorCodes); return (list)->errors
#define ErrorCheck(condition, errorCodes)   \
    do {                                    \
        if (!(condition)) {                 \
            ReturnErrors (list, errorCodes);\
        }                                   \
    } while (0)

namespace LinkedList {
    ListTemplate
    ListErrorCode InitList_ (List <Element, ElementComparator> *list, size_t capacity, CallingFileData creationData) {
        ZoneScoped;

        if (!list)
            return LIST_NULL_POINTER;

        list->size     = 1;
        list->capacity = (ssize_t) capacity + 1;

        #define AllocateArray(name, type) list->name = (type *) calloc ((size_t) list->capacity, sizeof (type))

        AllocateArray (next, ssize_t);
        AllocateArray (prev, ssize_t);
        AllocateArray (data, Element);

        #undef AllocateArray

        #define CheckForNull(expression, error) if (!(expression)) {return error;}

        CheckForNull (list->prev, PREV_NULL_POINTER);
        CheckForNull (list->next, NEXT_NULL_POINTER);
        CheckForNull (list->data, DATA_NULL_POINTER);

        #undef CheckForNull

        list->prev [0] = 0;
        list->next [0] = 0;

        list->freeElem = 1;

        for (ssize_t freeIndex = list->freeElem; freeIndex < list->capacity; freeIndex++) {
            list->next [freeIndex] = (freeIndex + 1) % list->capacity;
            list->prev [freeIndex] = -1;
        }

        list->creationData = creationData;

        Verification (list, creationData);

        return NO_LIST_ERRORS;
    }
    
    ListTemplate
    ListErrorCode DestroyList_ (List <Element, ElementComparator> *list) {
        ZoneScoped;

        if (!list)
            return LIST_NULL_POINTER;

        list->capacity = list->size = -1;

        free (list->data);
        free (list->prev);
        free (list->next);

        return NO_LIST_ERRORS;
    }

    ListTemplate
    ListErrorCode InsertAfter_ (List <Element, ElementComparator> *list, ssize_t insertIndex, 
                                ssize_t *newIndex, Element *element, CallingFileData callData) {
        ZoneScoped;

        assert (newIndex);
        assert (element);

        Verification (list, callData);

        if (insertIndex < 0 || insertIndex >= list->capacity)
            return WRONG_INDEX;

        if (list->prev [insertIndex] == -1)
            return WRONG_INDEX;

        if (list->freeElem == 0 && ReallocUp_ (list, callData) != NO_LIST_ERRORS)
                return INVALID_CAPACITY;

        *newIndex = list->freeElem;
        list->freeElem = list->next [list->freeElem];

        list->prev [list->next [insertIndex]] = *newIndex;

        list->next [*newIndex]   = list->next [insertIndex];
        list->next [insertIndex] = *newIndex;
        list->prev [*newIndex]   = insertIndex;
        list->data [*newIndex]   = *element;

        list->size++;

        return NO_LIST_ERRORS;
    }

    ListTemplate
    ListErrorCode DeleteValue_ (List <Element, ElementComparator> *list, ssize_t deleteIndex, CallingFileData callData) {
        ZoneScoped;
        Verification (list, callData);

        if (deleteIndex <= 0)
            return WRONG_INDEX;

        if (list->prev [deleteIndex] == -1)
            return WRONG_INDEX;

        list->prev [list->next [deleteIndex]] = list->prev [deleteIndex];
        list->next [list->prev [deleteIndex]] = list->next [deleteIndex];

        list->next [deleteIndex] = list->freeElem;
        list->prev [deleteIndex] = -1;
        list->freeElem           = deleteIndex;

        list->size--;

        return NO_LIST_ERRORS;
    }

    ListTemplate
    ListErrorCode ReallocUp_ (List <Element, ElementComparator> *list, CallingFileData callData) {
        ZoneScoped;
        Verification (list, callData);

        ssize_t prevCapacity = list->capacity;

        list->capacity *= (ssize_t) REALLOC_SCALE;

        #define ReallocateArray(name, type, error)                                                          \
            do {                                                                                            \
                list->name = (type *) reallocarray (list->name, (size_t) list->capacity, sizeof (type));    \
                if (!list->name) {                                                                          \
                    ReturnErrors (list, error);                                                             \
                }                                                                                           \
            } while (0)

        ReallocateArray (next, ssize_t, NEXT_NULL_POINTER);
        ReallocateArray (prev, ssize_t, PREV_NULL_POINTER);
        ReallocateArray (data, Element,  DATA_NULL_POINTER);

        #undef ReallocateArray

        for (ssize_t freeIndex = prevCapacity; freeIndex < list->capacity; freeIndex++) {
            list->next [freeIndex] = (freeIndex + 1) % list->capacity;
            list->prev [freeIndex] = -1;
        }

        list->next [list->capacity - 1] = list->freeElem;
        list->freeElem = prevCapacity;

        Verification (list, callData);

        return NO_LIST_ERRORS;
    }

    ListTemplate
    ListErrorCode VerifyList_ (List <Element, ElementComparator> *list) {
        ZoneScoped;
        
        if (!list)
            return LIST_NULL_POINTER;

        ErrorCheck (list->data,                                             DATA_NULL_POINTER);
        ErrorCheck (list->prev,                                             PREV_NULL_POINTER);
        ErrorCheck (list->next,                                             NEXT_NULL_POINTER);
        ErrorCheck (list->capacity >= 0,                                    INVALID_CAPACITY);
        ErrorCheck (list->next [0] >= 0 && list->next [0] < list->capacity, INVALID_HEAD);
        ErrorCheck (list->prev [0] >= 0 && list->prev [0] < list->capacity, INVALID_TAIL);
        ErrorCheck (list->freeElem >= 0 && list->freeElem < list->capacity, FREE_LIST_ERROR);

        ssize_t freeIndex = list->freeElem;

        while (freeIndex > 0) {
            ErrorCheck (list->prev [freeIndex] <= 0, FREE_LIST_ERROR);

            freeIndex = list->next [freeIndex];
        }

        return list->errors;
    }

    ListTemplate
    ListErrorCode FindValue_ (List <Element, ElementComparator> *list, Element *value, ssize_t *index, CallingFileData callData) {
        ZoneScoped;
        Verification (list, callData);

        for (ssize_t elementIndex = list->next [0]; elementIndex != 0; elementIndex = list->next [elementIndex]) {
            if (!ElementComparator (&list->data [elementIndex], value)) {
                *index = elementIndex;
                return NO_LIST_ERRORS;
            }
        }

        *index = -1;
        return NO_LIST_ERRORS;
    }
}

#ifndef TRACY_ENABLE
    #undef ZoneScoped
#endif

#undef ListTemplate
#undef ON_DEBUG
#undef Verification
#undef WriteErrors
#undef ReturnErrors
#undef ErrorCheck

#endif
