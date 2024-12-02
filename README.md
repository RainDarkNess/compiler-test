# Compiller test
A simple object file compiler for x8086 windws in C, written in a single file (historically, this is how it happened). The input is the source modal language, the output is a compiled object file that can be compiled using gcc


## Usage
Compile using ide or gcc main.c, then run it as follows

```cmd
.\example.exe .\\path_to_modal_language 1 1 1
```

The first parameter is the debug mode
The second parameter is the output of all variables written in human-readable form, the third is the output of all delimiters and system words.

Modal language example:
```js
# comment #
program var

    # comment #
fl:float;
test:int;
test_lb:bool;
test_nl:bool;
hex_t:int;

# comment #
test1,test2,test3:int;

begin

test1 assign 11;
test2 assign 1020;
test3 assign 123E1;
hex_t assign 1AABh;

displ hex_t;

fl assign 1.1;

test assign 200;

test_lb assign false;

if test_lb EQV true then
displ fl;
else
displ 123123;
end

if test_lb GRE 1 then
displ test_lb;
else
enter test;
end

displ fl;

displ test3;

while test do
    test assign test - 10;
displ test;
next

while test GRE true do
    test assign test - 10;
displ test;
next

for i assign fl val 10 do
    displ i;
next

# comment
  , _ ,
 ( o o )
/'` ' `'\
+'''''''+
+\\'''//+
   """

comment #

end..
```

## Compile
```cmd
gcc .\main.c.obj -o file  
```


### Requirements
win 64, ~500kb ram.

## To do
- [ ] code refactoring

## project team
im
- [rain (git)](https://github.com/RainDarkNess/compiler-test) — dev
