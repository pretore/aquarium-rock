## Linked list

### Overview
Linked lists are used when order is important and you need fast insertion 
and or fast deletion anywhere within the list.

### Design
The flavour we have implemented is an intrusive circular doubly linked list.
* Doubly - _successor and predecessor nodes are tracked._
* Circular - _traversing in a single direction will eventually get you to the
  node you started with._
* Intrusive - _to use the linked list you need to change your structures by 
  embedding linked list nodes._

### Use

To use the linked list you need to embed ``struct rock_linked_list_node`` 
members to your structures.

```c
struct student {
    const char* name;
    size_t name_length;
    struct rock_linked_list_node by_insertion_order;
};
```

Each ``struct rock_linked_list_node`` member must be initialized with ``bool
rock_linked_list_node_init(struct rock_linked_list_node *node)`` before 
being used.

```c
struct student bob = {};
seagrass_required_true(!rock_linked_list_node_init(&bob.by_insertion_order));
```

### Memory

As a consequence of being an intrusive container, it is the user's
responsibility to manage the memory of the structures with ``struct
rock_linked_list_node`` members. You should only reclaim the memory of
the structure once you have removed the ``struct rock_linked_list_node`` 
from the linked list instance.

```c
seagrass_required_true(!rock_linked_list_remove(&bob.by_insertion_order));
```
