<div id="top"></div>

# Assembly Emulator

## About The Project
This is an Assembly Emulator written in C++ language. It supports an easier version of RISC (Reduced Instruction Set Computer) instruction set, so that it's more readable for user, and also easy to understand. Therefore, it does not support some fancier instructions or addressing modes featured by
older Complex Instruction Set (CISC) designs. 

_For more information about the language used, please refer to the [Documentation](https://github.com/Gluncho/Assembly-Emulator/blob/main/DOCUMENTATION.md)_

### Built With
* [C++](https://www.cplusplus.com/)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

If you want to set up this project locally, you should read this section

### Prerequisites
You must have g++ installed in order to run this program.
  #### Linux
  ```sh
  $ sudo apt install build-essential
  ```
  #### Windows
  Check out how to install g++ on Windows [here](https://www3.cs.stonybrook.edu/~alee/g++/g++.html)

### Installation

Clone the repo
   ```sh
   git clone https://github.com/Gluncho/Assembly-Emulator.git
   ```

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

### Compile and run
In order to compile and run the project, use the following commands:

#### Linux
 ```sh
   g++ *.cpp && ./a.out
   ```
#### Windows
  ```sh
   g++ *.cpp
   ./a.exe
   ```
### Execute an assembly file
After running the project, you'll need to test it by a file in which assembly instructions are written. I've provided some [tests](https://github.com/Gluncho/Assembly-Emulator/tree/main/tests) which you can also use. If you want to know what particular test file really does, you must know that almost every test file has its corresponding C file, so if you're not familiar with those instructions, you can see what C code translates into this assembly file. For example, [tests/recursion/gcd.c](https://github.com/Gluncho/Assembly-Emulator/tree/main/tests/recursion/gcd.c) would translate into [tests/recursion/gcd](https://github.com/Gluncho/Assembly-Emulator/tree/main/tests/recursion/gcd)

To test, you can just enter the filename in the command or type ```-help``` for more information about how to interact with command prompt.

Here are two examples of how would you test [gcd](https://github.com/Gluncho/Assembly-Emulator/tree/main/tests/recursion/gcd) on Linux:
1. ``` sh
    g++ *.cpp && ./a.out
    tests/recursion/gcd
    ```
2. (If you want to compile with debugging mode) 
    ``` sh
    g++ *.cpp && ./a.out
    -dbg tests/recursion/gcd
    ```

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>
