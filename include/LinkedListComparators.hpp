#ifndef LINKED_LIST_COMPARATORS_HPP_
#define LINKED_LIST_COMPARATORS_HPP_

template <typename elem_t>
inline int ListElementComparator (elem_t *firstElement, elem_t *secondElement) {
    return firstElement - secondElement;
}

#endif
