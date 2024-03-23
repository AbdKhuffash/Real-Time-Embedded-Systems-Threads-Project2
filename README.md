This project aims to create a simulation of a supermarket environment where multiple processes and threads interact to manage product shelving and customer interactions. The simulation incorporates multiprocessing and multithreading techniques to emulate realistic behavior within the supermarket.

Overview
In this simulation:

Products: The supermarket sells a user-defined number of products. Each product has a defined initial amount placed on the shelves, with the remaining amount stored in a storage area.

Shelving Teams: User-defined numbers of shelving teams operate within the supermarket. Each team consists of a team manager and a group of employees. Shelving teams are responsible for replenishing products on the shelves.

Customer Interactions: Customers arrive randomly at the supermarket and choose random items and quantities from a list of available products. If an item is out of stock, it cannot be selected by customers.

Simulation Termination: The simulation ends under two conditions: when the storage area runs out of stock or when the simulation duration exceeds a user-defined limit.

Implementation
To implement this simulation:

Utilize a combined approach of multiprocessing and multithreading. Customers are represented as processes, while shelving teams are also processes. Within shelving teams, managers and employees are represented as threads.

Compile and thoroughly test the program, ensuring it is bug-free. Utilize debugging tools like gdb if issues arise during development.

Avoid hard-coding values by storing user-defined parameters in a text file. This enhances code flexibility and eliminates the need for frequent code modifications and recompilation.

Use simple and elegant graphics elements from the OpenGL library to visually represent the simulation. Graphics should be clear and intuitive, aiding in understanding the simulation's behavior.

Instructions
To run the simulation:

Clone this repository to your Linux machine.
Compile the program using the provided Makefile.
Run the executable with appropriate command-line arguments, including the text file containing user-defined parameters.
Monitor the simulation's progress and behavior.
If encountering issues, utilize gdb for debugging and ensure all aspects of the simulation function as intended.
Zip the folder containing your source code and executable before the deadline.
Submit the zipped folder containing the code and executable.
Conclusion
This supermarket simulation project offers a comprehensive exercise in combining multiprocessing and multithreading techniques to model real-world scenarios. By adhering to realistic choices and utilizing appropriate tools and techniques, the simulation aims to provide an insightful exploration of supermarket operations.
