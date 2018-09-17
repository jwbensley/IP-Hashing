In this example two linked lists are used to create a basic consistent hashing implementation (or circular hash).

https://www.codeproject.com/articles/56138/consistent-hashing
https://github.com/chrismoos/hash-ring


A linked list of virtual nodes (vnodes) is created which and the vnodes point to physical nodes (pnodes). A simple modulus hash is performed on a 5 tuple (destination IP, source IP, IP protocol number, destination port, source port) which provides a index into the virtual nodes linked list. The physical node pointed to by that virtual node is then returned.

When adding and removing a physical node to the list of pnodes, additional vnodes are added and deleted. When a pnode is added one or more vnodes are added, the number of vnodes added ensures that there is an equal number of vnodes pointing to each pnode. When a pnode is deleted all vnodes pointing to that pnode are deleted too

Number of Vnodes should be fixed?!