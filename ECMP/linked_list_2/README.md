## Linked List v2


### Overview

In this example two linked lists are used to create a consistent hash ring  
(a.k.a consistent circular hash) to implement a consistent ECMP method. The   
aim of the ECMP method is to consistently hash ingress transit traffic to the  
same egress destination even when additional next-hops or egress links are  
added or removed from the destination "pool".  


### References

* https://www.codeproject.com/articles/56138/consistent-hashing
* https://github.com/chrismoos/hash-ring


### Details

A linked list of arbitrary length is created (1000 by default). This first  
list is a list of "virtual nodes" (vnodes). A second linked list is created  
of zero length. The second list is for "physical nodes" (pnodes) [1]. As each  
pnode is added to the pnodes linked list a portion of the vnodes are updated  
to point to the new pnode. An equal number of vnodes point to each pnode at  
any given time. For example, when the first pnode is added 1000 vnodes point  
to pnode1. When a seconds pnode is add 500 vnodes will point to each pnode,  
and so on. The reverse process occurs when a pnode is removed; if the 5th  
pnode was deleted the vnodes would be updated such that 250 vnodes point to  
each remaining pnode.  

This dual linked list implementation provides a level of indirection. The  
pnodes list could represent physical egress links or perhaps next-hop indices  
in a lookup table. Entries could be added to the pnodes list as new links are  
added to a LAG or new equal costs routes are added to a routing table.  
Entries are thus removed from the pnodes list if an egress interface in a  
LAG were do go down for example. The vnodes list is never changing in size  
though. This means that a hash function can be used to hash incoming IP  
packets into the index of the vnodes list and the outcome is consistent.  

A simple modulus hash is performed on a 5 tuple (destination IP, source IP,  
IP protocol number, destination port, source port) which provides an index  
into the virtual nodes linked list. The pnode pointed to by that vnode is  
returned by the hash function. The hash function itself is very basic  
because "good" hashing is a separate and complex exercise out of the scope of  
this example. In this example a basic modulus hash is used just to return an  
index into the vnode list and thus a pnode entry. In a production network the  
hash function would need to be more complex or use some sort of feedback  
mechanism to improve the evenness of traffic distribution over the pnodes.  
This would allow for a poor mans consistent hash ring implementation. When a  
new pnode is added to the 2nd linked list only the required vnodes are  
updated meaning the re-hashing impact of existing traffic is as low as it can  
be without adjusting the hashing function and extending the vnode list.  
Equally when removing a pnode list entry any traffic that was hashing to an  
existing pnode shall continue to do so uninterrupted.  

[1] The terms vnode(s) and pnode(s) are arbitrary. In this example they can  
be thought of as "virtual nodes" which could be a next-hop entry in a RIB or  
FIB and "physical nodes" which could be the resolved egress interface index  
or adjacency index to reach that next-hop.  

The hash function determines may use fields within traffic headers as keys that cause traffic in the same IP flow to be consistently grouped.