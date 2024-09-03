/**
 ****************************************************************************************
 *
 * @file uwifi_wlan_list.h
 *
 * @brief api of list operation
 *
 * Copyright (C) ASR
 *
 ****************************************************************************************
 */

#ifndef _UWIFI_WLAN_LIST_H_
#define _UWIFI_WLAN_LIST_H_

//#include "asr_rtos.h"

#if !defined(ALIOS_SUPPORT) && !defined (JL_SDK) && !defined (JXC_SDK)
#include "stddef.h"
#include <stdio.h>
#include <stdbool.h>
#include "asr_types.h"


#define CONFIG_ILASRL_POINTER_VALUE 0xdead0000
#define POISON_POINTER_DELTA CONFIG_ILASRL_POINTER_VALUE

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define LIST_POISON1  ((void *) 0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *) 0x00200200 + POISON_POINTER_DELTA)

//offsetof has been defined in <stddef.h>
//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({            \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


struct list_head {
    struct list_head *next, *prev;
};


/*
 * list_empty - tests whether a list is empty
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 * This is only for internal list manipulation where we know the prev/next entries already!
 */
static inline void __list_add(struct list_head * new, struct list_head * prev, struct list_head * next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/*
 * list_add - add a new entry
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/*
 * list_add_tail - add a new entry
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries point32 to each other.
 * This is only for internal list manipulation where we know the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
    if (list_empty(entry)) {
        return;
    }

    __list_del(entry->prev, entry->next);
    entry->next = (struct list_head *)LIST_POISON1;
    entry->prev = (struct list_head *)LIST_POISON2;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
                  struct list_head *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}

/**
 * list_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void list_rotate_left(struct list_head *head)
{
    struct list_head *first;

    if (!list_empty(head)) {
        first = head->next;
        list_move_tail(first, head);
    }
}


/**
 * list_entry - get the struct for this entry
 * @ptr: the &struct list_head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

/**
 * list_for_each - iterate over a list
 * @pos: the &struct list_head to use as a loop counter.
 * @head: the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

/**
 * list_next_entry - get the next element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
        list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_for_each_entry    -    iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:    the head for your list.
 * @member:    the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)                \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against
removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)            \
        for (pos = list_first_entry(head, typeof(*pos), member),    \
            n = list_next_entry(pos, member);           \
             &pos->member != (head);                    \
             pos = n, n = list_next_entry(n, member))

#endif

#endif //_UWIFI_WLAN_LIST_H_
