# Project 1: Shell
## COP4610: Operating Systems

### Due: Monday, September 29th, 2025, at 11:59pm

---

## Project Objectives

1. Design and develop a shell interface that supports input/output redirection, piping, background processing, and a set of built-in functions.
2. Gain in-depth knowledge of process control mechanics, including parent-child relationships, process creation, and user-input parsing and verification.
3. Enhance understanding of command-line parsing techniques and strategies.
4. Design and implement error handling mechanisms to gracefully handle various types of errors that may occur during shell execution.

## Project Description

The purpose of this project is to design and develop a comprehensive shell interface that enhances process control, user interaction, and error handling mechanisms. By constructing this shell, you will gain valuable insights into the intricacies of operating system processes, command-line parsing, and the robustness required for error-free execution.

The project encompasses several key objectives. Firstly, you will delve into process control mechanics, acquiring in-depth knowledge of parent-child relationships, process creation, and the crucial aspects of user-input parsing and verification. Understanding these mechanics is essential for building an effective shell that can manage and execute user commands.

Next, you will delve into the execution of external commands by implementing path search and expanding tildes and environment variables. Path search involves searching for the executable file of a command by scanning through directories specified in the `$PATH` environment variable. This functionality ensures that the shell can locate and execute commands regardless of the current working directory. Additionally, the expansion of tildes and environment variables enables the shell to interpret and replace tilde (`~`) symbols and environment variable references (such as `$HOME`) with their corresponding absolute paths or values.

Lastly, the project focuses on the design and implementation of the shell interface itself. The shell should support essential features such as input/output redirection, allowing users to redirect standard input and output streams to files. Piping functionality should also be included, enabling the seamless flow of data between processes. Additionally, background processing support will allow users to execute commands concurrently.

By successfully completing this project, you will not only gain practical experience in process control, command-line parsing, and error handling, but also have the opportunity to build a robust and user-friendly shell interface that empowers users to efficiently interact with the operating system.

## Division of Labor

Submit your division of labor document on Canvas by 9/10/25.

## Part 0: Tokenization

You do not have to worry about implementing a lexer to parse inputs into tokens. You can use the files provided on Canvas as a starting point.

The zip is called shell.zip and is under the Project 1 Modules

Note: you do not have to use the provided files. You can implement your own lexer if you'd like.

If you are programming in Rust, you will have to start from scratch.

## Part 1: Prompt

The user will be greeted with a prompt that should indicate the absolute working directory, the user name, and the machine name. You can do this by expanding the $USER, $MACHINE, $PWD environment variables. The user will type on the same line as the prompt.

    USER@MACHINE:PWD>

Here is an example:

    mnguyen@linprog2.cs.fsu.edu:/home/grads/mnguyen>

## Part 2: Environment Variables

In the context of program execution within its own environment, you will undertake the task of replacing tokens prefixed with the dollar sign character ($) with their corresponding values. Within the bash shell, you can utilize the 'env' command to display a comprehensive list of your environmental variables. Your objective is to implement a mechanism that automatically expands tokens starting with the dollar sign into their respective values.

For instance, given the command ["echo", "$USER"], the token "$USER" should be expanded to its corresponding value, resulting in the output ["echo", "mnguyen"]. This expansion applies universally to any command, ensuring consistent token replacement throughout the shell. To accomplish this, you can utilize the getenv() function, the usage details of which can be found in its man page.

It's important to note that this token expansion occurs regardless of the command being executed, providing a seamless integration of environment variable values within commands and generating the expected output.
## Part 3: Tilde Expansion

In Bash, tilde (\~) may appear at the beginning of a path, and it expands to the environment variable $HOME. For example, tokens [“ls”, “\~/dir1”] should expand to [“ls”, “/home/grads/mnguyen/dir1”]. An output example of tilde expansion is:

    ~ → /home/grads/mnguyen

You will only have to handle the tilde expansion of a token if "\~" a standalone or if it begins with "\~/".
## Part 4: $PATH Search

In Bash, when a command like "ls" is entered, the shell is able to execute the corresponding program/executable located at "/usr/bin/ls" through a process called path search. This path search is not magical, but rather a straightforward search conducted within a predefined list of directories. The list of directories is specified in the environment variable $PATH.

For commands that do not include a slash (/) and are not built-in functions (covered in part 9), it becomes necessary to search each directory specified in $PATH. It's important to note that $PATH is a string containing multiple directories delimited by a colon. To perform the search, you will need to employ string operations to extract and examine each directory in the $PATH variable.

If you type in the command echo $PATH in your terminal you should get something like this:

    /home/grads/mnguyen/.bin:/home/grads/mnguyen/.scripts:/usr/local/bin:/opt/sfw/bin:/usr/sfw/bin:/bin:/usr/bin:/usr/ccs/bin:/usr/ucb:.

In the event that the command is not found in any of the directories listed in $PATH, an error message should be displayed. In Bash, this typically results in the familiar "command not found" error message. Handling this scenario involves proper error detection and reporting, ensuring a clear indication when a command is not available within the directories specified in $PATH.

## Part 5: External Command Execution

Once you have obtained the path to the program you intend to execute, either because the command included a slash or through the $PATH search, the next step is to execute the external command. However, executing an external command requires more than just a single line of code using the execv() function.

To accomplish this, a two-step process is involved. First, you need to fork() to create a child process. The child process will be responsible for executing the desired command using the execv() function. This separation between the parent and child processes ensures that the execution of the command does not interfere with the operation of the shell itself.

It's important to note that you must handle commands with arguments correctly. This means that commands such as "ls -al" with multiple arguments should be properly processed and executed in the child process.

By following this approach of forking and executing the command within the child process, you can ensure the proper execution of external commands, including those with arguments, within your shell. This separation of processes allows for efficient and accurate command execution while maintaining the stability and functionality of the shell as a whole.
## Part 6: I/O Redirection

In this section, we will focus on implementing input/output (I/O) redirection from/to a file. By default, the shell receives input from the keyboard and outputs to the screen. However, with I/O redirection, we can replace the keyboard with input from a specified file and redirect output to a designated file.

The behavior of I/O redirection should adhere to the following guidelines:

- `cmd > file_out`
  - cmd writes its standard output to the file_out.
  - If file does not exist, it will be created.
  - If file already exists, it will be overwritten.
- `cmd < file_in`
  - cmd receives its standard input from file_in.
  - An error will be signaled if file does not exist or is not a regular file.
- `cmd < file_in > file_out`
  - cmd receives standard input from file_in
  - cmd writes its standard output to file_out
- `cmd > file_out < file_in`
  - Same as above.

These combinations follow the aforementioned rules for input and output redirection. By implementing I/O redirection, we empower the shell to efficiently manage input and output streams, allowing users to redirect command output to files and read command input from files. This functionality greatly enhances the versatility and flexibility of the shell when interacting with external commands.

In the process of implementing I/O redirection, you will need to check/assign appropriate file permissions to the files you create/read when redirecting inputs and outputs. You can consult UNIX file permissions and open file permissions here: Permission Bits and Open Flags

Likewise, you should follow these standard guidelines when implementing I/O redirection.

- Your processes should not modify the contents of the input file whatsoever (assign the correct permissions when reading in).
- Output redirection should create a new file with the following file permissions: -rw-------.
- Output redirection should overwrite (not append) files with the same name and with the same permissions listed above.

_Note: Careful consideration should be given to the parsing of tokens, particularly in scenarios like CMD > FILE_OUT < FILE_IN. In this case, it is important to prioritize the order of operations. The input file (FILE_IN) should be read first and then passed as input to the command (CMD), which will execute using that input. The resulting output will be redirected to the output file (FILE_OUT). Paying attention to the correct sequencing and handling of tokens ensures that the desired input and output redirection behavior is achieved in complex scenarios like this._

## Part 7: Piping

Beyond simple I/O redirection, we will explore the concept of piping, a more sophisticated form of I/O manipulation. Contrary to regular I/O redirection, piping involves the simultaneous execution of multiple commands, with the input and output of these commands interconnected. This setup allows the output generated by the initial command to be seamlessly passed as input to the subsequent command.

In this project, we will handle a maximum of two pipes in any given command. Piping behavior should follow the following guidelines:

- `cmd1 | cmd2`
  - `cmd1` redirects its standard output to the standard input of `cmd2`.
- `cmd1 | cmd2 | cmd3`
  - `cmd1` redirects its standard output to the standard input of `cmd2`.
  - `cmd2` redirects its standard output to the standard input of `cmd3`.

By implementing piping functionality, we enable the seamless flow of data between commands, enhancing the flexibility and power of the shell. Piping allows for the creation of command pipelines, where the output of one command becomes the input for the next. This feature promotes the construction of complex and efficient command sequences, enabling sophisticated data processing and manipulation within the shell environment.

Remember that each command is an independent process that runs concurrently, so you must fork a new process for each command with the correct redirections. You can assume that each of the piped commands in the test cases will be logical (each command will take an input and provide an output).

## Part 8: Background Processing

The final functionality we will incorporate is background processing. Thus far, the shell has been waiting to prompt for additional user input whenever there were external commands running. Background processing allows the shell to execute external commands without waiting for their completion. However, it is still essential for the shell to keep track of their completion status periodically.

It's worth noting that background processing should seamlessly integrate with I/O redirection and piping functionalities. This means that background processing can be used in conjunction with I/O redirection or within command pipelines.

Background processing behavior should adhere to the following guidelines:

- `cmd &`
  - Execute cmd in the background.
  - Upon execution start, print `[Job number] [cmd's PID]`.
  - Upon completion, print `[Job number] + done [cmd's command line]`.
- `cmd1 | cmd2 &`
  - Execute `cmd1 | cmd2` in the background.
  - Upon execution start, print `[Job number] [cmd2's PID]`.
  - Upon completion, print `[Job number] + done [cmd1 | cmd2's command line]`.

Background processing also supports redirection functionalities:

- `cmd > file &`
  - cmd writes its standard output to file in the background.
- `cmd < file &`
  - cmd receives its standard input from file in the background.
- `cmd < file_in > file_out &`
  - cmd receives its standard input from file_in and writes its standard output to file_out in the background.

Additionally, all background processes executed by the shell must be kept track of with a relative job number starting from 1 and incrementing so forth. Job numbers will not be reused. You can also assume that there will not be more than 10 background processes running concurrently.

By implementing background processing, the shell gains the capability to execute commands concurrently, improving overall efficiency. Background processing, in combination with I/O redirection and piping, enables the execution of complex command sequences while providing informative feedback about job status to the user.

The standard way to check for finished processes would be to use signals. In this instance, you do not to need to implement process checking via signals. You can just check the list of finished processes in the main loop after execution.

## Part 9: Internal Command Execution

Having completed external command execution, the next aspect to address is the implementation of internal commands, often referred to as built-in functions. These functions are natively supported by the shell and will be integrated into your implementation.

- `exit`
  - If any background processes are still running, you must wait for them to finish.
  - You can assume that each command is less than 200 characters long.
  - Display the last three valid commands.
  - If there were less than three valid commands, print the last valid one.
  - If there are no valid commands, say so.
- `cd PATH`
  - Changes the current working directory.
  - If no arguments are supplied, change the current working directory to $HOME.
  - Signal an error if more than one argument is present.
  - Signal an error if the target is not a directory.
  - Signal an error if the target does not exist.
- `jobs`
  - Outputs a list of active background processes.
  - If there are no active background processes, say so.
  - Format: `[Job number]+ [CMD's PID] [CMD's command line]`

## Extra Credit

For those seeking additional challenges and opportunities to earn extra credit, the following options are available:

- **Support unlimited number of pipes [2]**
  - Expand the functionality of your shell to support an unlimited number of pipes in a single command. This allows for more complex command pipelines and advanced data processing.
- **Support piping and I/O redirection in a single command [2]**
  - Enhance your shell to handle both piping and I/O redirection simultaneously within a single command. This provides increased flexibility and allows for more advanced command compositions.
- **Shell-ception: Execute your shell from within a running shell process repeatedly [1]**
  - Implement the ability to execute your shell from within an already running shell process. This recursive execution allows for nested instances of your shell and demonstrates advanced control flow within the shell environment.

*Extra credit activities must be documented in the README to receive credit*

## Assumptions

Error messages do not need to match the exact wording of Bash, but should indicate the general cause of the error

No more than two pipes (|) will appear in a single command

You do not need to handle globs, regular expressions, special characters (other than the ones specified), quotes, escaped characters, etc.

There will be no more than 10 background processes at the same time

Piping and I/O redirection will not occur together in a single command

Multiple redirections of the same type will not appear in a single command

You do not need to implement auto-complete

You only need to expand environment variables given as whole arguments

The above decomposition of the project tasks is only a suggestion, you can implement the requirements in any order

## Restrictions

You are only allowed to use `C` or `Rust` (**no C++**).

You can only use `fork()` and `execv()` to spawn new processes.

You can not use `system()`, or any other `exec` family of system calls (execl, execlp, execle, execvp, execvpe, etc).

If you decide to use Rust, you can access the system calls, `fork()` and `execv()`, by using the “nix” or “libc” crate. _Note: you should not use std::process_. (you must be able to demonstrate that they provide access to these system calls in a functionally equivalent way; no extra features that make executing commands any easier).

You are not allowed to use any external libraries, except for Rust, where you can use `nix`, `libc`, `signal_hook`, and `regex`.

You may not use `execv()` in any of the built-ins (must be implemented from scratch)

## Rubric

A program with compilation errors will get 0 points (we must be able to compile and run your program on **linprog**)!

Please refer to the standard project guidelines: Project Syllabus

Total points: 105

- Implementation (`70 + 5`)
- Documentation (`30`)

### Implementation

    prompt (2)
    environment variables (3)
    tilde expansion (5)
    $PATH search (5)
    external command execution (15)
    I/O redirection (10)
    piping (10)
    background processing (10)
    internal command execution (10)
        exit [2]
        cd [5]
        jobs [3]
    extra credit (5)
        unlimited piping [2]
        piping & i/o redirection [2]
        shell-ception [1]

### Documentation

    division of labor (before) (5)
    readme.md (15)
        division of labor (after) [5]
        file listing [3]
        description of how to compile and run program [2]
        development log for each member [3]
        description of group meetings [2]
    project structure (5)
        source files are properly split into modular components according to functionality [2]
        files are appropriately placed in the correct directory following guidelines [2]
        executables produced in bin/ directory [1]
    format & organization (5)
        include all relevant files [1]
        doesn't include binaries or executables [1]
        readability (indentation, white space, max column-width < 100 chars) [1]
        submission does not contain unnecessary print statements [1]
        variable/functions names indicate purpose with appropriate comments [1]

## Deliverables

You should have the following files/directories in your Git repository:

    src
    include
    Makefile
    README.md

If you are using Rust, the standard Rust project structure (in rubric) will suffice (you do not need to create a makefile; the cargo build system will suffice).

Do not include your final executable(s) in your repository, doing so will result in a deduction!

Your makefile produice one executable, named shell. This executable should be placed in the bin/ directory.

## Submissions

Program submissions will be done through GitHub Classroom. Ensure that you have the deliverables in your Git repository by/after the due date. The latest commit will be used, so please account for late deductions (you can rollback to earlier work if you do not want to take any late deductions).

Additionally, make sure your project will compile in the linprog environment.
