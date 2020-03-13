# Lambda Calculus

## Grammar

__Lexer__
```
DOT       -> .
LPAREN    -> (
RPAREN    -> )
LAMBDA    -> $
ID        -> [a-zA-Z0-9]+
SEMICOLON -> ;
EQUALS    -> =
AT        -> @
LBRACK    -> [
RBRACK    -> ]
LCURLY    -> {
RCURLY    -> }
```
__Parser__
```
input  -> lines
lines  -> line
lines  -> line lines
line   -> expr SEMICOLON
expr   -> name EQUALS term
expr   -> term
var    -> ID
name   -> AT ID
atom   -> var
atom   -> name
term   -> atom
term   -> LPAREN term RPAREN
term   -> LCURLY LAMBDA var DOT term RCURLY
term   -> LBRACK term term RBRACK
```

The brackets and curly braces are not required. These are added by the Preprocessor automatically

### perl one-liner

perl script to obtain grammar format for automatic parser  
Line 1: gets input and puts in STDIN  
Line 2: filter out non-rules  
Line 3: filter out commented rules  
Line 4: modify rules to have a `#` at the end  
```
cat [INPUT] | \
perl -ne "print if /\-\>/g" | \
perl -ne "print unless /^#/g" | \
perl -ne 'chomp;print;print(" #\n")' | \
cat - <(echo "##")
```

### FIRST Sets

```

```

### FOLLOW Sets

```

```

### Examples

- `$x. x x;`
- `$x. $x. x (x x);`
- `x (x ($x. $x. x (x x) x) x);`
- `$x. x; $y. y;`

Example program:
```
@tru = $t. $f. t;
@true = @tru;
@fls = $t. $f. f;
@and = $a. $b. a b @fls;
@tru x y;
@fls x y;
```

## Disambiguation rules

__Abstraction__ is the grouping of `lamba`, it extends as far right as possible without crossing parenthesis
- `$x. x x => ($x. x x)`
- `$x. x (x x) => ($x. x (x x))`
- `(x $x. x x) x => (x ($x. x x)) x`

__Application__ is the grouping of terms left to right
- `x x x => ((x x) x)`

__Order__ of disambiguation is __Abstraction__ then __Application__
- `x $x. x x => x ($x. x x) => (x ($x. x x))`

### Preprocessor

The preprocessor solves the disambiguation issues


__Step 1.__ Brace all Abstractions's  
Input: `term -> LAMBDA var DOT term`  
Output: `term -> { LAMBDA var DOT term }`  

__Step 2.__ Bracket all Application's  
Input: `term -> term term`  
Output: `term -> [ term term ]`  

#### Examples

Input: `x x x x`  
2: `[x x] x x`  
2: `[[x x] x] x`  
2: `[[[x x] x] x]`  

Input: `x (x x) x`  
2: `[x (x x)] x`  
2: `[[x (x x)] x]`  
2: `[[x ([x x])] x]`  

Input: `$x. x`  
1: `{$x. x}`  

Input: `($x. x x) x x`  
1: `({$x. x x}) x x`  
2: `[({$x. x x}) x] x`  
2: `[[({$x. x x}) x] x]`  

Input: `$x. x x`  
1: `{$x. x x}`  
2: `{$x. [x x]}`  

Input: `x $x. x x`  
1. `x {$x. x x}`  
2. `[x {$x. x x}]`  
2. `[x {$x. [x x]}]`  

Input: `($x. x) x (x $x. x x ($x. x x) x) x x`  
1. `({$x. x}) x (x $x. x x ($x. x x) x) x x`  
1. `({$x. x}) x (x {$x. x x ($x. x x) x}) x x`  
1. `({$x. x}) x (x {$x. x x ({$x. x x}) x}) x x`  
2. `[({$x. x}) x] (x {$x. x x ({$x. x x}) x}) x x`  
2. `[[({$x. x}) x] (x {$x. x x ({$x. x x}) x})] x x`  
2. `[[({$x. x}) x] ([x {$x. x x ({$x. x x}) x}])] x x`  
2. `[[({$x. x}) x] ([x {$x. [x x] ({$x. x x}) x}])] x x`  
2. `[[({$x. x}) x] ([x {$x. [[x x] ({$x. x x})] x}])] x x`  
2. `[[({$x. x}) x] ([x {$x. [[x x] ({$x. [x x]})] x}])] x x`  
2. `[[({$x. x}) x] ([x {$x. [[[x x] ({$x. [x x]})] x]}])] x x`  
2. `[[[({$x. x}) x] ([x {$x. [[[x x] ({$x. [x x]})] x]}])] x] x`  
2. `[[[[({$x. x}) x] ([x {$x. [[[x x] ({$x. [x x]})] x]}])] x] x]`  


## Binding

`x` is bound to `lambda x` if:  
`x` is in the body of the abstraction  
AND  
`lambda x` is the closest `lambda x` to the left of `x`  
`x` is free if not bound


### Examples:

- `x ($y. y x) y`
    - free: both `x`, second `y`
    - bound: first `y` bound to `$y`
- `x ($y. y z ($x. x y))`
    - free: first `x`, `z`
    - bound: both `y`'s bound to `$y`, second `x` bound to `$x`
