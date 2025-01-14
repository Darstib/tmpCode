# Assignment 007 STL allocator + memory pool

## Requirements

- Implement your own memory allocator for STL vector.
- The allocator should optimize the memory allocation speed using memory pool.
- The allocator should support arbitrary memory size allocation request.

## File structure

```plaintext
as7
├── Makefile                <= make file
├── README.md               <= this file
├── include                 <= include files
│   ├── Allocator.hpp       <= my Allocator
│   ├── MemoryPool.hpp      <= using memory pool to speed up
│   └── Test.hpp            <= some function for test
└── src                     <= source code for test
    ├── containerTest.cpp   <= test Alloctor for different container
    ├── dataTypeTest.cpp    <= test Alloctor for different data type
    └── vectorTest.cpp      <= Namly the test on the PTA
```

As you can see, I implemented the Allocator using MemoryPool.

In **MemoryPool**: I use buffer_size as the min unit so it can only alloc memory size that is times of buffer_size.

In **vectorTest.cpp**: I believe that you can't know it more, so I just test it without doing any change.

Besides the test on the PTA, I test my Alloctor on two more tests, comparing with STL allocator.

In **\*Test.cpp**: I generate random number and use it to choose which operation to test, details are in *.cpp flles respectly. Generally, it presents as:

```cpp
int op = rng()%(number_of_operator);
switch (op) {
    case 0: {
        if (rng()%16 == 0) {
            // clear
            break;
        }
        // else, do the next one.
    }
    case 1: {
        if (!it.empty()) {
            // delete
        }
        // then, do the next one.
    }
    case 2: {
        // insert random number
        break;
    }
    ... // other operations
}
```

I wish clear less (or there won't be more than number_of_operator in the container in expection) and insert more to make sure there are always many memory allocated.

In **Test.hpp**: you can change the `const int OPERATIONS = 100000;` as you like.

## Test

For your convenience, you can just use CLI in linux and type:

```shell
# make sure you are at the path of the whole files, a.k. `as7`
$ make vector # or `make container` or `make datatype`
g++ -std=c++17 -I./include src/vectorTest.cpp -o bin/vectorTest && ./bin/vectorTest 2>/dev/null
correct assignment in vecints: 8015
correct assignment in vecpts: 8868

$ make # to test all of three
g++ -std=c++17 -I./include src/containerTest.cpp -o bin/containerTest && ./bin/containerTest 2>/dev/null
Running container tests...
Running vector test of int
Vector test passed.
Running set test of int
Set test passed.
Running map test of map<const int, int>
Map test passed.
All container tests passed.

g++ -std=c++17 -I./include src/dataTypeTest.cpp -o bin/dataTypeTest && ./bin/dataTypeTest 2>/dev/null
Running dataType tests...
Running test of short int
Passed.
Running test of int
Passed.
Running test of long long
Passed.
Running test of pair<int, long long>
Passed.
Running test of tuple<bool, char, int, double>
Passed.
All dataType tests passed.

g++ -std=c++17 -I./include src/vectorTest.cpp -o bin/vectorTest && ./bin/vectorTest 2>/dev/null
correct assignment in vecints: 4215
correct assignment in vecpts: 3396
```

If you want to know more details, you can see `Makefile`.

**Other info**:
```shell
$ g++ --version
g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```