## Red-black tree

### Overview

A red-black tree is a self-balancing binary search tree and serves as a
backing data structure for implementing sets and maps.

### Design

The red-black tree is implemented as an intrusive data structure. This
design allows the red-black tree to be combined with other intrusive data
structures for the creation of higher level containers such as bidirectional
maps, linked maps and linked sets.

### Use

To use the red-black tree you need to add ``struct
rock_red_black_tree_node`` members to your structures.

```c
struct student {
    const char* name;
    size_t name_length;
    struct rock_red_black_tree_node by_name;
};
```

Each ``struct rock_red_black_tree_node`` member must be initialized with ``bool
rock_red_black_tree_node_init(struct rock_red_black_tree_node *node)`` before
being used.

```c
struct student bob = {};
seagrass_required_true(!rock_red_black_tree_node_init(&bob.by_name));
```

When we initialize a red-black tree instance, we need to provide a compare
function for the red-black tree to order the nodes accordingly.

```c
struct rock_red_black_tree student_by_name;

seagrass_required_true(!rock_red_black_tree_init(
        &student_by_name,
        student_name_compare));
```

As an example, the compare function provided below is for the ordering on
the name of a student.

```c
static int student_name_compare(
        const struct rock_red_black_tree_node *const a,
        const struct rock_red_black_tree_node *const b) {
    /* retrieve the `struct student` from the given nodes */
    const struct student *A = rock_container_of(a, struct student, by_name);
    const struct student *B = rock_container_of(b, struct student, by_name);
    /* first compare by name length ... */
    const int result = seagrass_uintmax_t_compare(
            A->name_length, B->name_length));
    if (result) {
        return result;
    }
    /* ... and then by name. */
    return strncmp(A->name, B->name, A->name_length);
}
```

Please note the use of ``rock_container_of`` to obtain the containing structure
of ``struct student`` from the member ``by_name``'s pointer.

### Memory

As a consequence of being an intrusive container, it is the user's
responsibility to manage the memory of the structures with ``struct
rock_red_black_tree_node`` members. You should only reclaim the memory of
the structure once you have removed the ``struct rock_reed_black_tree_node``  
from the associated ``struct rock_red_black_tree`` instance.

```c
seagrass_required_true(!rock_red_black_tree_remove(
        &student_by_name, &bob.by_name));
```
