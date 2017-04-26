#include <list.h>

node insert_right(node list, char* data){
	node new_node = (node) malloc(sizeof(struct list_node));
	new_node->data = data;
	new_node->next = list->next;
	list->next     = new_node;
	return new_node;
}

node delete_right(node list){
	node tmp   = list->next;
	list->next = list->next->next;
	free(tmp);
	return list;
}

node search_for(node list, char* data) {
    while (list != NULL) {
        if (list->data == data)
            return list;
        list = list->next;
    }
    return NULL;
}