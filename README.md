# BMP Converter
## video demo: <https://youtu.be/K1fJAobsUiU>

## description:
### what leads up to this project:

Hello !
i've had a few ideas about what type of projects i wanted:

Ideas in C:
   1. video to ascii frames
      - This was my main idea. i've though about it on how i could come up with something like. that. the main drawback would that i wouldn't be the one writing the code for the actual converter since i would use FFMPEG.
   2. sha generator
      - this was my second idea until i've figured out that i probably wasn't good enough for now since i didn't had any previous experience with this type of mathematics. it would have been a great opportunity to learn new things tho
   3. txt file to image
      - at first i wanted a medium sized project and some fun time. and since we saw a bit of  Memory management, and i already knew a bit how it worked since i did a lot of C in the past i decided to go with this one.


### the choice:

i decided to go with the third project since i didn't wanted to use an external frame to video converter and writing my own would take way too much time and way too much knowledge (which would have been interesting. and i planned to write my own already)


### the start

my main idea about what i'd have to do to write the program was as follow:
   1. memory management
   2. Understanding how image are made
   3. understanding a specific file format
   4. todo app

First of all i looked at "JPG" and "PNG" but there were compressed images and for some reason i didn't really wanted to compress an image. especially if i have to de-compress it after.

Then i remembered the BMP file format that we used for the week 4 (or 5): Memory management

i just remembered mid writing this README.md that i also though about a todo app in c but i didn't even noted it because of how easy it would have been in my opinion

and i remembered also looking into it that it was just storing raw pixel data and then displaying it to the screen

it was perfect

### Thinking process

the tought process for encoding was something alongs the lines of:
   1. read inputFile
   2. parse into list of R G B
   3. write the outputFile

and for the decoding:
   1. read the inputFile
   2. parse and convert the RGB to a file again
   3. write to outputFile



### actual challenges
   1. i'll talk about it later but bitshift
   2. understanding how to read and write bytes
   3. finding ways to works with bytes instead of '\0'
   4. understanding the format BMP
   5. writing the exact same file
      - for this one when i switched from only str to bytes it was a bit tricky. i had to store the information of the size of the real file somewhere. so i used the "UNUSED/RESERVED" bytes from 0x06 to 0x09
   6. and of course putting all of that together into this beautiful code

#### main.c

In srcs/main.c, the Main file
there's multiple things you can find:
   1. How to use the executable
   2. command line argument parser
      - Help
         - This will print the usage on how to use the executable with the command line arguments
      - Encoding
         - this will encode any input file and output a BMP file containing the data of the input file
      - Decoding
         - this will decode a BMP file and output it as a any file (txt, obj, exe)


#### main.h
not much to say about it to be honest

contains all the #include required and the linked_list structure and used to store all the linked list functions, encode and decode, parse and the utils


#### linked_list_utils.c

i've writted this file to create a sort of utility function for everything that will contains the linked list. Remove a node, add a node, create a node, create and add a node etc etc

in it there's multiple function that i make use of:
   1. `create_node`
      - as simple as the name suggest: malloc and return an empty node
   2. `create_node_from_buffer`
      - this use the function `create_node_from_values` and set the value of a new malloced node and return it
   3. `create_node_from_buffer_and_insert`
      - this one use the function `create_node_from_buffer` to create the node and insert it a the start of the linked list. making it the new head
   4. `free_linked_list`
      - free the whole linked list

#### bmp.h

before writing the encode.c i had to write the BMP utils and understand how it works under the hood
its actually there i was the majority of the time
i had to figure out a lots of things to be able to wrap my head around the fact that it wasn't just a simple "write-all-the-colors-and-thats-it"
things like padding, bit shifting, binary writing and reading. anyway here's the little utils for the WRITE to encode a file
   1. the big comment at the top that sum up everything about the header of a BMP file
   2. multiple define to check the CONSTANTS of a BMP file
      - BM for the first to bytes
      - 54 for complete size the header
      - 40 for the size of the information header
      - 1 for the color plane (always 1)
      - 24 for the BitPerPixel
   3. structures to help me "structurate" how i read the file
      - the first structure is 14 bytes and it contains:
         - header - fileSize - UNUSED - pointer
      - the next 40 bytes goes in the seconds structure and it contains:
         - fixed informations header size - width and height
         - color plane - number Bit per pixel - compression (if any)
         - imageSize (fileSize - 54) - vertical and horizontal resolution
         - number color used - important colors
   4. the structure that will hold the 2 structures together with the file size, width and height as info


   ps: 17.01.2024: i changed the unused space to write the exact file size information at bmp_write. so i can write the exact same at decode. it was a little challenge beacuse for some reason it worked perfectly on everything except on the t8.shakespeare.txt where it wrote a NULL byte at the end for seemingly no reasons
#### bmp_write.c

multiple important functions:
   1. `__write_init_header`
      - that will call the 2 other write function
      - it also return a malloc of t_header
      - `__write_initBMPFileHeader`
         - that will set the header for the first 14 bytes
      - `__write_initBMPInfoHeader`
         - that will set the last 40 bytes
      - These 2 functions were tricky because i did not knew how to and why bitshift i had to learn a lot about it there and i was glad i choosed that because of it
   2. `writeBMPHeader`
      - this one will be useful to write the first 54 bytes of the 2 structures (that will be grouped in 1 by a `my_memcpy` for simplicity)
   3. `write_all_with_linked_list_buffer`
      - this function use the linked list and iterate through it to get the BGR values (BGR in bmp is how it goes for some reason. thanks microsoft)
      - I had a function that iterated through the linked list and wrote as it goes but it was kinda slow and i decided to complicated myself for some time saved even tho it consume more memory
      - this function will create a buffer with all the informations required about the RGB + padding and write it in one go on the file

   the goal of this file is to get the utils for the big write


#### parse_data.c

this file contains the WRITE and READ parsing
only 2 functions are in there

   1. `parse`
      - This function was a bit technical to make to be fair and i had to tweak it **A LOT**
      - it create the linked list and get the values from the string read previously
      - for more colors (and since we are working with unsigned char) i decided to go with the ascii number * 2
      - it create the list every 3 char and return the size created
   2. `read_parse_bytes`
      - this one was the trickiest i'd say (wihtout accounting for the bitshift) because i had a lot of idea yet some of there were instantly wipped by my mind
         - i though about just skipping 0x00 but then what if (for some reason) there's a NULL byte in a text ? I couldn't go with this option so ->
         - I had to calculate the padding again then skip X and re read again and caring about the 2 last bytes that were automatically added for END OF FILE

#### encode.c

this file will contains everything about encoding
from the first node to freeing all the created nodes by the data parsing

it contains multiple important functions:
   1. `encode`
      - this function create an empty node, read the file with `readfile_bychar` (below) and then use `parse` to create the whole linked list
      - it then free's everything
   2. `handle_array_divisor`
      - This was not the hardest function to code but the one that had me thinking the most
      - this function return a list of divisors for a number and assign it to the width and height
      - i originally planned to just to a square and then fill the blank pixels with 0x00 but when i told my dad about it he got petty with a snarky smile saying that he would find a way to do it differently with a shape of a rectangle adapting to the size of the file and just told me to remember what i did in class something like 10 years ago (smallest multiple in commun or biggest divisors in commun)
      - so i got to work and found that i could "brutforce" the numbers and add them to get a number of divisors
      - if i find no divisors i take the square value and try to brutforce it with `brut_force_numbers`
         - exemple if there's no commun divisors:
         ```
         original_size = 27
         fastSqrt(original_size) = 6 (5.1 rounded up)
         brut_force_numbers(6, 6, original_size)
            6 * 6 = 36
            6 * 5 = 30
            7 * 4 = 28
           +1 *-1 until it find the good one that fits the most
         ```
      - and for the exemple of the array it output for 12:
      `[0, 2, 3, 4, 6]` -> i then take the 2 middle number (3, 4) so its the one closest to a square and having the most "normal" resolution
   3. `writeAll`
      - this function is used to write all the content using the functions from *`bmp_write.c`*
   4. `readfile_bychar`
      - this function will read all the content of the file and store it into a big `char *`


That's it for the encoding part

#### utils.c

i don't know if i should give a detailed exemple of all the function in there because most of them are from standard library that im used to write so its pretty easy so i just tweak them a little bit if needed

   1. `fastSqrt`
      - fast square root i did some time ago for leetcode
   1. `my_memset`
      - memset
   1. `my_memcpy`
      - memcpy
   1. `get_divisors`
      - function that i wrote to check all the divisors of a number and store them into an int array
   1. `brut_force_numbers`
      - function that i described a bit up with the exemple
   1. `my_strcmp`
      - strcmp
   1. `my_strchr`
      - strchr
   1. `my_strstr`
      - strstr a bit tweaked to give me 0 if found and 1 if not found instead of the address of the char
   1. `get_size`
      - function to get the size of a file using a file descriptor and lseek
   1. `rev_strldup`
      - strdup tweaked
         - reverse the string
         - operate with a size instead of stopping at a '\0'

#### bmp_read.c

multiple important functions:
the bmp_read.c is a bit different from the bmp_write.c
the key difference is the on the read function it will get values from the file and check if values are correctly setup
   1. `__attribute_size_header`
      - this function does the bitshifting to get the full size, width and height to put it into the main header structure
   2. `__read_init_header`
      -  that will call the 2 other read function and check multiples potential errors around the formatting of the bmp file that the program have to read
      - it also return a malloc of t_header
      - `__read_initBMPFileHeader`
         - read the first 14 bytes and memcpy them into the structure
      - `__read_initBMPInfoHeader`
         - read the next 40 bytes and memcpy them into the structure
   3. `checks_BMPFileCorrect`
      - used to check if the first 14 bytes are correctly formatted
   4. `checks_BMPInfoCorrect`
      - used to check if the next 40 bytes are correctly formatted

#### decode.c

this file also has some functions (surpisingly !)

   1. `read_hex`
      - this function will read the bytes contained in the BMP file starting at 0x36
      - it will then add a 0 at the end of the newly created string
      - and it will also modify the string with a / 2 (since we did * 2 on the bmp_write)
   1. `write_text`
      - it will take the string and write it into the outputFile also deleting all the NULL char at the end
      - i had to delete all the null char at the end because of how i make the RGB in the linked list creation
      - since i fill with 0 if i don't have any more char to take
   1. `decode`
      - the decode function handle the "decoding" by calling the other 2 functions and making use of them
      - it also call the function `read_parse_bytes`


### MAKEFILE,  VALGRIND AND DOCKERFILE


#### VALGRIND
i spent a lot of time making sure i don't get any memory leaks / unconditional jump / others by using valgrind in an ARCH docker

i used this specific command:
```
valgrind \
   --leak-check=full \
   --show-leak-kinds=all \
   --track-origins=yes \
   --verbose \
   --log-file=valgrind-out.txt \
   ./executable \[options\]
```

and i choose arch linux because of how easy is it to setup. also the -fsanitize=address -g show me the leaks if somehow valgrind doesn't detect them and it allowed me to make a lot of useful debugging with the gdb command

#### MAKEFILE
my makefile is pretty straight forward with multiple rules

all: $(target)
   call the target rule

$(target): $(objs)
   generate the -o from $(objs) and compile them together to create the executable

%.o: %.c
   take .c file from the variable containing all files and create an object (.o) file for each of them
   its supposed to not recompile every time but for some reason it recompile everything in the cs50 codespace

build:
   create the docker and run it

prune:
   destroy the docker

clean:
   remove all objects (.o) files

fclean: clean
   call the clean rule and delete the $(target) executable


re: fclean all
   destroy all and rebuild


.PHONY: all build prune fclean clean re
   making sure that it wont mess up everything and actually call the rules instead of doing undefinied behaviors with folders

#### DOCKERFILE

pretty straightforward also

FROM archlinux
   - import archlinux

RUN pacman -Sy --noconfirm vim make gcc neofetch valgrind glibc strace lldb gdb
   - download a bunch of useful command that i often use (sadly i couldn't use man so i always googled man write or man fwrite etc)

ENV DEBUGINFOD_URLS="https://debuginfod.archlinux.org"
   - the next rule is tricky since i spent almost an hour on that. for some reasy valgrind on the archlinux couldn't find the strlen command ??? so i had to add an environement variable (never happened to me before)

COPY . /checking_files
   - copy the current directory file to /checking_files

WORKDIR /checking_files
   - basically CD in /checking_files
   - it does a bit more than that but thats how i mainly used it



Well thats it thanks a lot for reading all of that !

i had a lot of fun doing this project and i probably forgot to tell a lot of things about the issues i had

i'd say i did it in around 7-10 hours of coding and 10-15 hours of reading things and debugging
