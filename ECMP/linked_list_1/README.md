## Linked List v1

## Contents

 * [Overview](#overview)
 * [References](#references)
 * [Basic Principals](#basic-principals)
 * [Detailed Workings](#detailed-workings)
 * [Example Output](#example-output)
 * [Linked List Code](#linked-list-code)


### Overview

In this example two linked lists are used to create a consistent hash ring  
(a.k.a consistent circular hash) to implement a "consistent ECMP method".  
This serves as a high level educational example of a traffic distribution [1]  
method that could be used inside a network device to achieve the following  
aims;  

 * To evenly spread a perfect distribution of IMIX traffic ingressing the device over all available egress indexes.
 * To consistently forward ingress transit traffic to the same egress destination even when additional egress indexes are added or removed.

In the context of this example egress indexes could be next-hop indexes in a  
routing table (e.g. a RIB or FIB), egress links in a LAG bundle, or egress  
adjacency entries out of a shared interface.  


### References

* https://en.wikipedia.org/wiki/Doubly_linked_list
* https://www.codeproject.com/articles/56138/consistent-hashing
* https://github.com/chrismoos/hash-ring


### Basic Principals

When the program starts a linked list of arbitrary length is created (1000  
entries by default). This first list is a list of "virtual nodes" (vnodes).  
A second linked list is created of zero length. To be specific a   
doubly-linked lists are used as opposed to a singly-linked lists. The second  
list is for "physical nodes" (pnodes) [2]. When a pnode is added to the  
pnodes linked list a portion of the vnodes are updated to point to the new  
pnode. An equal number of vnodes point to each pnode at any given time. For  
example, when the first pnode is added all 1000 vnodes are updated to point  
to pnode1. When a second pnode is added 500 vnodes will be updated to point  
to pnode2, the remaining 500 already point to pnode1. If 10 pnodes have been  
created then 100 vnodes point to each pnode, and so on.  

The reverse process occurs when a pnode is removed; if the 10th pnode were  
deleted (leaving 9 pnodes) the vnodes would be updated such that 1000/9  
vnodes point to each remaining pnode. This is just an example so the  
distribution isn't perfect.  

In this example one can think of pnodes as entires in a RIB or members of a  
LAG bundle, which are essentially pointers to a next-hop or egress interface.  
The vnodes are a list of pointers to the pnodes which provides a layer of  
abstraction and are essentially arbitrary. Traffic that ingresses a network  
device could be used to populate the keys in a hash function which could be  
used to produce an index into the vnodes list. The vnode entry in turn points  
to one of the pnode entries which one can think of as the next-hop for  
the transit traffic.

Entries could be added to the pnodes list as new links are added to a LAG or  
new equal cost routes are added to a routing table. Entries would thus be  
removed from the pnodes list if an egress interface in a LAG were to go down  
for example. The vnodes list however, is never changing in size. This means  
that a hash function can be used to hash incoming data packets into the index  
of the vnodes list and the outcome is consistent irrelevant of the number of  
pnodes or when they are added or removed.  


### Detailed Workings

In this example a simple modulus hash is performed on a 5 tuple (destination  
IP, source IP, IP protocol number, destination port, source port) which  
provides an index into the virtual nodes linked list. The pnode pointed to by  
that vnode is returned by the hash function. The hash function itself is very  
basic because "good" hashing is a separate and complex exercise out of the  
scope of this example. In this example a basic modulus hash is used just to  
return an index into the vnode list and in turn a pnode entry.  

In this example, the simple modulus method evenly distributes all possible 5  
tuple values over all available vnodes (and thus pnodes). This provides a  
perfect distribution of traffic over all egress pnode entries assuming that  
the ingressing traffic was equally sourced from all possible values within  
the 5 tuple range (i.e. traffic is from/to all possible IP addresses and port  
numbers). In a production network the hash function would need to be more  
complex to weight its result towards a statistical normal (if it is known in  
advance what cone of possible IP addresses and port numbers will enter the  
hash function) or use some sort of feedback mechanism, to improve the  
evenness of traffic distribution over the pnodes.  

There will always be collisions in the hash function, especially when using a  
basic `modulus()` approach. Any ECMP implementation should aim to reduce  
collisions because, collisions result in unequal distribution of traffic  
unless the collisions are known to be equal somehow.  

The table below compares the Big Theta stats for doubly-linked lists and hash  
tables. Hash tables will be examined in more details in a future example.  
Below it can be seen that upper bound search and access times of the  
doubly-linked list are slower however, they provide a simple method for  
implementing consistent load balancing:

| Data Structure     | Access | Search | Insertion | Deletion |
|--------------------|--------|--------|-----------|----------|
| Doubly-Linked List | Θ(n)   | Θ(n)   | Θ(1)      | Θ(1)     |
| Hash Table         | N/A    | Θ(1)   | Θ(1)      | Θ(1)     |


This example allows for a poor man’s consistent hash ring implementation.  
When a new pnode is added only the required vnodes are updated meaning that  
the re-hashing impact of existing traffic is as low as it can be, without  
adjusting the hashing function and extending the vnode list.  

Equally when removing a pnode list entry any traffic that was being  
distributed to an existing pnode shall continue to do so uninterrupted, the  
traffic that was being distributed to the pnode which was removed shall be  
evenly distributed over the remaining pnodes.  

This example creates an initial vnode list with 1000 entries linked together.  
This is an arbitrary number that is significantly higher than the expected  
number of pnodes. In production networks devices typically limit of the number  
of ECMP routes or LAG members that can be used: 32, 64, 128 and 256 and  
typical maximal values device vendors support. This example doesn't allow the  
user to create more pnodes than vnode entries because, pnodes would be unused  
if a vnode didn't point to them. The ratio of vnodes that point to any one  
pnode is always the same as any other pnode meaning that in the event that  
the number of vnodes and pnodes became equal (if in this example 1000 pnodes  
were added) the traffic distribution should be even for any number of pnodes  
from 1 to 1000.  


[1] This is an example of consistent traffic distribution meaning the same  
set of traffic `s1` is consistently forwarding to destination `d1` independent  
of the traffic volume or load.  

[2] The terms vnode(s) and pnode(s) are arbitrary. In this example they can  
be thought of as "virtual nodes" which could be a next-hop entry in a RIB or  
FIB and "physical nodes" which could be the resolved egress interface index  
or adjacency index to reach a next-hop.  


### Example Output

The following example shows that after adding three pnodes the vnode point to  
a roughly equal share of pnodes (in this case 1000 pnodes can't be evenly  
divided by 3).  

![Adding pnodes](https://null.53bits.co.uk/uploads/programming/github/ip-hashing/ecmp/linked_list_1/ll1_example_1.svg "Adding pnodes")   

In the below example 3 pnodes are added and their memory addresses listed.  
Next a random 5 tuple is generated and hashed to produce an index into the  
vnode list and ultimately points at a pnode (the `val` field in the printed  
vnode). The 5 tuple hash key produces an index into the vnode linked list at  
address 0x55b7345f43a0, its `val` points to pnode 0 at address  
0x55b7345f0260. The most recently added pnode (pnode 2) is deleted and the  
same 5 tuple is then entered manually. It will hash to the same vnode which  
in turn points to the same pnode. This shows that despite removing a pnode,  
i.e. a next-hop entry, the existing traffic balance would not have been  
affected; traffic destined to one of the two remaining next-hops continues to  
flow to the same next-hop.  

![Hashing](https://null.53bits.co.uk/uploads/programming/github/ip-hashing/ecmp/linked_list_1/ll1_example_2.svg "Hashing")  

If the recording is too fast the CLI output is here: [ll1_example_2.txt](ll1_example_2.txt)  


### Linked List Code

The code isn't the greatest but, it is simple and thus easy to understand, I  
hope. A simple struct is the basis for all vnode and pnode linked list items:  

```c
struct node {
    void*  val;
    struct node* next;
    struct node* prev;
};
```

`next` and `prev` point to the next and previous items in the linked list,  
respectively. For an item in the vnodes linked list, `val` will point a pnode  
item in the pnodes linked list. In this example code, for pnode items `val`  
is left as `NULL` but, it could serve as the pointer to an ifIndex or  
adjacency index etc.  

Using linked lists means that one can write functions like `node_add()` in  
[node.c](node.c) which will simply `calloc()` the memory required for an  
additional `struct node` and updates the `next` value in the last list item  
to point to this new node, and the `prev` value in the first item of the  
linked list to point to this new item. The list can be inserted into at any  
point. `root_pnode` and `root_vnode` are the same `node` struct objects and  
point to the first item in the linked list. This is why linked list insertion  
time is always Θ(1), the entire list doesn't need to be walked to find the  
end.  

In this implementation the deletion time isn't Θ(1) though. One method for  
implementing a delete function that isn't affected by list length is to  
allocate the memory for the entire list when the program starts. For example,  
we could `calloc()`/`malloc()` the space for 128 pnodes (assuming we know  
what our platform upper bound is). The first pnode entry would simply have  
it's `prev` and `next` values point to it's self. The second pnode, when  
required, would simply be located at the `malloc()` address +  
`sizeof(struct node)`, `prev` and `next` would both point back to the first  
pnode and the first pnode's `prev` and `next` values would both point to the  
second pnode address. The third pnode would be stored at the `malloc()`  
address + 2x`sizeof(struct node)`, and so on.  

When the n'th pnode is to be deleted the program can simply  
`free(root_pnode + (n-1)*sizeof(struct node));`. The problem with this  
approach is if there is no limit to the number of pnodes or pnodes aren't all  
allocated together, then discontigous memory space may be used, which is the  
case for this example. In this example the user adds pnodes manually, which  
calls `calloc()` each time. Although it is very likely, it is not guaranteed  
that the pnodes will be stored in continuous memory space. For this reason,  
this example has to walk the vnode or pnode linked lists to delete the n'th  
item (assuming n is neither the first or last item in the list).  
