# syxc — the syx compiler

## Status

- Experimental
- No parser or codegen modules yet
- Unstable
- File structure **may change frequently**

## Build
bash: 
```bash
git clone https://github.com/code-trueink/syxc.git
cd syxc
./build.sh
```
the executable will be 'syxc' in the same dir
## Requirments
- Linux
- C++20 or later
- nasm
- ld

## how to run 
Running the binary without arguments will display usage instructions.

## Contribution
Contribution are welcome.

*Branches
- feature brach -> small change
- dev brach -> big change

when a feature or dev are stable, it shall get merge into main

*Code Guide
- Avoid using inheritance
- Indentation: 2 spaces
**Prefix Rules:
- core of lexer: lc_*
- core of parser: pc_*
- core of codegen: cc_*
- module of lexer: lm_* 
- module of parser: pm_* 
- module of codegen: cm_*

EVERY single one of the header file must be put inside 'hpp_file' 


