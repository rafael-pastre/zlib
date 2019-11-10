# Random Data Generator
This program generates files filled with random data

## How to use:
* compilation command: make all
* execution command: make run

## Configurations macros:
Program configurations can be altered in random_data_generator.cfg file.

### READ_FILE_INPUT
* Syntax: "READ_FILE_INPUT \<bool\>".
* If this option is set to TRUE, the program will get it's inputs from a file. The path and file name must be inserted at execution time.
* default value = FALSE.

### SET_INPUT_FILE
* Syntax: "SET_INPUT_FILE \</path/file_name\>".
* If this macro is used, "READ_FILE_INPUT" will be considered false, even if it's declared in config file.
* this macro is interpreted as a command, therefore it doesn't have a default value.

### READ_FILE_OUTPUT
* Syntax: "READ_FILE_OUTPUT \<bool\>".
* If this option is set to TRUE, the program will send it's output from a specified file. The path and file name must be inserted at execution time.
* default value = FALSE.

### SET_OUTPUT_FILE
* Syntax: "SET_OUTPUT_FILE \</path/file_name\>".
* If this macro is used, "READ_FILE_OUTPUT" will be considered false, even if it's declared in config file.
* this macro is interpreted as a command, therefore it doesn't have a default value.

### READ_SEED
* Syntax: "READ_SEED \<bool\>".
* If this option is changed to TRUE, a new seed must be inserted at execution time. If this option is set to FALSE, the seed for random function will be "time(NULL)" or the value passed in "SET_SEED" macro. 
* default value = FALSE.

### SET_SEED
* Syntax: "SET_SEED \<integer_value\>".
* Sets the seed for random function with the value specificated.
* If this macro is used, "READ_SEED" will be considered false, even if it's declared in config file.
* this macro is interpreted as a command, therefore it doesn't have a default value.

### PRINT_DIM
* Syntax: "PRINT_DIM \<style\>"
* \<style\> is an integer value.
	* If \<style\> = 0, no dimension will be printed.
	* If \<style\> = 1, dimensions will be printed separeted by a space.
	* If \<style\> = 2, dimensions will be printed separeted by a new line.
* default value = 0.

### READ_BLOCKS_NUM
* Syntax: "READ_BLOCKS_NUM \<bool\>".
* default value = FALSE.
* default number of blocks = 1.

### SET_BLOCKS_NUM