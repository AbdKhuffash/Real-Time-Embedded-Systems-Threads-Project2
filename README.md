# Supermarket Simulation - IPCS & Threads
This project aims to create a simulation of a supermarket environment where multiple processes and threads interact to manage product shelving and customer interactions. The simulation incorporates multiprocessing and multithreading techniques to emulate realistic behavior within the supermarket.

## Overview
In this simulation:

Products: The supermarket sells a user-defined number of products. Each product has a defined initial amount placed on the shelves, with the remaining amount stored in a storage area.

Shelving Teams: User-defined numbers of shelving teams operate within the supermarket. Each team consists of a team manager and a group of employees. Shelving teams are responsible for replenishing products on the shelves.

Customer Interactions: Customers arrive randomly at the supermarket and choose random items and quantities from a list of available products. If an item is out of stock, it cannot be selected by customers.

Simulation Termination: The simulation ends under two conditions: when the storage area runs out of stock or when the simulation duration exceeds a user-defined limit.

## Conclusion

This supermarket simulation project offers a comprehensive exercise in combining multiprocessing and multithreading techniques to model real-world scenarios. By adhering to realistic choices and utilizing appropriate tools and techniques, the simulation aims to provide an insightful exploration of supermarket operations.
