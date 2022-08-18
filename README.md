
## Database Management
This project consists of three programs. The main purpose of the program is to learn how to communicate with the processes that we run from different terminals with Named pipe, and to communicate with the child process with fork, pipe and exec, and how the programs work in a synchronized way.

When Program.c runs, it waits for the user to enter a query. It sends the incoming query to the Database.c program with Named Pipe. It reads the information from the Database.c program, and if the incoming information is not NULL, it asks the user "Save the query result? y/n” he asks. If the user says yes, it sends this information from the child process with pipe to the Save.c process with exec by forking.

The Save.c process writes the data from Program.c, which is the parent process, to the result.txt file. It sends a message to the parent process that the operation was successful.

The database.c program parses the query from the program.c process and searches for the desired data in the data1.txt or data2.txt file. Returns the query result to the program.c file.

Query examples:
select * from veri1.txt where ad=fatih
select number from veri2.txt where ad=selin
select ad from veri2.txt where number=2

![SS4](https://raw.githubusercontent.com/omerkucuker/DatabaseYonetim/main/ss4.PNG)

![SS1](https://raw.githubusercontent.com/omerkucuker/DatabaseYonetim/main/ss1.PNG)
![SS2](https://raw.githubusercontent.com/omerkucuker/DatabaseYonetim/main/ss2.PNG)
![SS3](https://raw.githubusercontent.com/omerkucuker/DatabaseYonetim/main/ss3.PNG)



