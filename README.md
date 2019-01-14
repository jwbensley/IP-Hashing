# Overview

This repo contains small programs and code snippets. They are intended as  
educational examples of how basic programing techniques such as memory  
structures and hashing work. These are then used explore and understand how  
to implement basic network functions that rely on these programming  
techniques, such as forwarding lookups and ECMP.  


## Equal Cost Multipath Routing

### [Link List v1](ECMP/linked_list_1)

This is an example in C of how to implement a doubly-linked list. Two  
doubly-linked lists are used to implement a "consisten ECMP method" using a  
poor mans version of a consistant hash ring a.k.a consistent circular hash.  
