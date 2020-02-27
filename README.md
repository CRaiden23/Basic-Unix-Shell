Program Description:
    Group Project for CSCE 3600 to build a basic shell with several built in functions
    Basic Shell with built in functions, myhistory, cd, exit, and path.
    Shell also supports up to two pipes, redirects and any combination of the two.
    Includes bonus "chprompt" command! NOW IN COLOR!
        
    Steven Hurley | Piping and history
        Responsible for the creation and organization
            of the base shell as well as implementation of
            built in history function and pipelining
            
    Aaron Hurley | Signaling and cd
        Responsible for the implementation of signal handling
            and the cd command.
    
    Alex Luong   | Combination and exit
        Responsible for the implementation of combination of redirection
            AND pipe (that support up to 2 pipes) and exit command.
            ***NOTE!!!: Only run if the command is typed correctly*** 
                E.g. cmd < infile | cmd | cmd > outfile
                     cmd < infile | cmd       or cmd | cmd > outfile
                     cmd < infile | cmd | cmd or cmd | cmd | cmd > outfile
                ****Won't work for (e.g. cmd < file |) without any command given, will 
                    return segfault****
                     
    Oscar Garcia | Redirection and path
    Responsible for the implementation of redirection and creating a new path variable.
    Unfortunately I was unable to due so without causing errors that would make the shell unusable.
    I commented out the code to prevent this.
        
To Compile:
"make"

To Run:
"make run" -- Interactive Mode
		To improve readability, input is printed in yellow, CMD in green
"make run [filename]" -- Batch Mode

To Cleanup:
"make clean"
