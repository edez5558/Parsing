# Parsing programming language

## Overview

This project is a parser and lexer for a programming language implemented in C.

The parser is designed to handle an LL(1) Grammar. You can check the grammar used in bin/compiler.grammar or modify it according to your needs.

## Usage

### Get the code 

```
git clone https://github.com/edez5558/Parsing.git
cd Parsing
```

### Build

First, create a folder for precompiled headers called "precompiler": 

```
mkdir precompiler
make
```

Or use a custom folder changing the variable "PRE_FOLDER":

```
make PRE_FOLDER=CUSTOM_PATH/
```

The binary file will be in the "bin" folder. You can also change it with the variable "BIN_FOLDER". Make sure that you are inside the "bin" folder to use it:

```
cd bin
```


### Run

You can run it without a file to read simple code using:

```
./compiler
```

Alternatively, you can run it with a file by providing the file path. For example, in the "bin" folder, there exists the file "main.kok":

```
//Comment

/* 
Another comment
*/

func int : main(int: argc, string: []argv){
  int: var;
  int: var2;

  var2 = 5;
  var1 = var2 + (var2 * 4);

  while(var == (4 * var2)){
    var -= 1;
  }

  if(true)
    call print("hello world");
  else
    call print("bye world");
  endif

  return 0;
}
```

To check if the file is valid, run:

```
./compiler main.kok
```

## Contributing

Contributions are welcome! If you find any issues or have improvements to suggest, please open an issue or create a pull request.



