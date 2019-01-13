## Linked List v1

## Contents

 * [Overview](Overview)
 * [References](References)
 * [Linked List Operations](Linked List Operations)
 * [Basic Principals](Basic Principals)
 * [Detailed Workings](Detailed Workings)
 * [Example Output](Example Output)


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


### Details

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

The table below compares the BigO stats for doubly-linked lists and hash  
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
set of traffic `s1` is consistently forwarding to destination `d1` independant  
of the traffic volume or load.  

[2] The terms vnode(s) and pnode(s) are arbitrary. In this example they can  
be thought of as "virtual nodes" which could be a next-hop entry in a RIB or  
FIB and "physical nodes" which could be the resolved egress interface index  
or adjacency index to reach a next-hop.  


### Example Output

The following example shows that after adding three pnodes the vnode point to a roughly equal share of pnodes (in this case 1000 pnodes can't be evenly devided by 3).  

<p align="center">
    <img src="https://github.com/jwbensley/jwbensley.github.io/blob/master/repos/IP-Hashing/ll1_example_1.svg">
</p>

In this example 

