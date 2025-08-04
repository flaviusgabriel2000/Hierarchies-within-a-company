// Hierarchies within a company

	The general idea of my code to complete the implementation was, of course,
using the data structure provided and the operations on it, such as traversing the
tree in preorder, obtaining a specific node, freeing memory, etc.

			IMPLEMENTATION

HIRE - Allocate memory for the top member if the hierarchy is empty.
       Otherwise, the employee will be taken over by the manager given
       as a parameter. Finally, the employees are sorted alphabetically
       with the qsort function.

FIRE - Mainly pointer operations. The algorithm distinguished several cases:
       1) the node to be deleted has only one employee, 2) the node to be
       deleted has no employees, 3) the node to be deleted has 2 or more employees.
       In each case, at the end, the memory allocated for the respective node will
       be freed. For each case, I added additional comments in the code that explain
       what the following block of instructions will do.

PROMOTE, MOVE_EMPLOYEE, MOVE_TEAM, FIRE_TEAM - The algorithm is similar to the one
from the FIRE task, each task having small modifications corresponding to its
requirement. Also,  additional comments in the source explaining the pieces of code
that seem more difficult to interpret.

GET_EMPLOYEES_BY_MANAGER - I created an additional recursive function that adds the
names of the employees of "employee_name" to a vector.

GET_EMPLOYEES_BY_LEVEL - I created an additional recursive function that gets a node
from the given level as a parameter. Then, I can go back to the manager of the
obtained node's manager (node->manager->manager) to display all employees from that level.

GET_BEST_MANAGER - I created an additional recursive function that inserts all members
of the hierarchy into a vector. I then traversed the vector looking for the node(s) with
the maximum number of employees. Finally, I extracted these nodes, sorted them alphabetically
with qsort, and then wrote them to the file.