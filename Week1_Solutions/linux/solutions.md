# BootCamp Week 1

## Linux Introduction

### Exercises

1. Find the variant of the <code>ls</code> command (i.e. argument(s) to be used) to list contents of a folder including its hidden files, the variant of <code>cp</code> to copy folder and the command to rename a file.

Ans. <code>ls -a</code> is the command to list contents of the folder including its hidden files. Variant of <code>cp</code> to copy folder is <code>cp -R DistinationFolderAdd SourceFolderAdd</code> and the command to rename a file is <code>mv file1 file2</code>

2. In this question, we will understand the hardware configuration of your working machine using the <code>/proc</code> filesystem.

    - Run command <code>more /proc/cpuinfo</code> and explain the following terms: <code>processor</code> and <code>cores</code>. Use the command <code>lscpu</code> to verify your definitions.
        - The term <code>processor</code> represents the ID of the processor whose discription is being followed and <code>cores</code> is the count of CPU cores the processor has.
    - How many CPU sockets, cores, and processors does your machine have?
        - My machine has 8 processors labelled (0-7), 8 CPU Cores, 4 cores per socket and single socket
    - What is the frequency of each processor?
        - Frequency of each processor is 2400.000 MHz
    - How much memory does your machine have?
        - Total Memory is 7947212 kB
    - How much of it is free and available? What is the difference between them?
        - Free Memory is 703424 kB and Available Memory is 4817908 kB. Difference between them is Free memory is idle memory not in use and Available memory is the one in use which includes but not limited to caches and buffers, that can be freed without the performance penality of using swap space.
    - What is the total number of user-level processes in the system?
        - 296
    - What is the total number of number of forks since the boot in the system ?
        - 18419
    - How many context switches has the system performed since bootup?
        - 39618267


3. In this question, we will understand how to monitor the status of a running process using the <code>top</code> command. Compile the program [cpu.c](cpu.c) given to you and execute it in the bash or any other shell of your choice as follows.
    ```console
    gcc --version
    # If gcc is not installed then run :- sudo apt-get install gcc
    gcc cpu.c -o cpu
    ./cpu
    ```
    This program runs in an infinite loop without terminating. Now open another terminal, run the <code>top</code> command and answer the following questions about the cpu process.

    - What is the PID of the process running the cpu command?
        - PID of the process is 16809 
    - How much CPU and memory does this process consume?
        - Process consumes 100% CPU and 0% Memory
    - What is the current state of the process? For example, is it running or in a blocked state or a zombie state?
        - Current status of the system is its running shown by code "R"
