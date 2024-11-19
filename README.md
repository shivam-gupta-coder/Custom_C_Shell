#  ASSUMPTIONS 
 
# Major assumptions took for the project

# Part-A
1. Only proclore command also working. Printing data of most recent process.
2. Seek specification also working with just -e flag.
3. Colour given for reveal but not for displaying shell prompt and error messages.
4. On typing wrong user command execvp error will be displayed (for non-custom commands).
5. Upon typing enter the space will be directed to comms.txt file and nothing will happen upon calling log execute for such an index.


# Part-B

1. Multiple word aliases not allowed. Alias also working for custom commands and when in pipe or separated by semi-colons.
2. Functions (bonus specification) not implemented.
3. Pipes: Allowing custom commands in pipes. However seek is not allowed. For running pipes in background, it is expected that user won't type & within the pipes i.e allowing & after the whole pipe command is typed.
4. Pipe commands are being implemented from left to right sequentially. 
5. If a pipe is run in background, the whole pipe command before & will be considered.
6. Activities will print only status of those  processes which last for some time i.e which are not executed very early.
