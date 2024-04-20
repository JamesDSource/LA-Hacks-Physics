#ifndef LIST_H_
#define LIST_H_

#include<stdio.h>

typedef enum {
	LIST_RESULT_SUCCESS = 0,
	LIST_RESULT_OUT_OF_MEMORY,
	LIST_RESULT_OUT_OF_BOUNDS,
} List_Result;

#define LIST(T, L) List_Result L ## Init(size_t cap, L **list) { \
			*list = (L*)(malloc(sizeof(L) + (cap - 1)*sizeof(T))); \
			if (*list == NULL) { \
				return LIST_RESULT_OUT_OF_MEMORY; \
			} \
			return LIST_RESULT_SUCCESS; \
		} \
	void L ## Free(L *list) { \
			free(list); \
		} \
	List_Result L ## Append(L **list, T item) { \
			size_t len = (*list)->len; \
			if(len + 1 >= (*list)->cap) { \
				(*list)->cap *= 2; \
				*list = (L*)(realloc(*list, sizeof(L) + sizeof(T)*((*list)->cap - 1))); \
				if (*list == NULL) { \
					return LIST_RESULT_OUT_OF_MEMORY; \
				} \
			} \
			T *items = &(*list)->items; \
			items[len] = item; \
			(*list)->len += 1; \
			return LIST_RESULT_SUCCESS; \
		} \
	List_Result L ## UnorderedRemove(L *list, size_t idx) {\
			if (idx >= list->len) {\
				return LIST_RESULT_OUT_OF_BOUNDS; \
			} \
			T* items = &list->items; \
			items[idx] = items[list->len - 1]; \
			list->len -= 1; \
			return LIST_RESULT_SUCCESS; \
		}

#define LIST_H(T, L) typedef struct L { size_t len; size_t cap; T items; } L; \
	List_Result L ## Init(size_t cap, L **list); \
	void L ## Free(L *list); \
	List_Result L ## Append(L **list, T item); \
	List_Result L ## UnorderedRemove(L *list, size_t idx); \

#endif
