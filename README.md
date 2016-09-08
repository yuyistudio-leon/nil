![18258PICziJ.png](https://csdn-code.oss.aliyuncs.com/note-attachments/20150910-0010-3804-7156/18258PICziJ.png)
#Hello, **NL**!
**NL** is abbreviation for new language, and **yes, it's a new language**!
**NL** is inspired by **Lua**, a widely used scripting language for game programming!
We try to keep the implement of **NL** simple, and sometimes ugly. And we won't take efforts to optimize the performence as long as the implement is functioning properly on the line util we think **NL** itself, not the implement, is beautiful enough.

##Features
* scrpting language
* byte code support
* week type
* c style
* the only data structure called table

##Get started!
> 1. Press `Download` button above, and you will get `nl` in the zipped file.
2. Run `./nl main.nl` in the shell!

##Here's the magic!
```cpp

/*
Don't borther to define a structure for `person'.
According to the data structure in my mind, person.name.first and person.name.first may be a function or a string!
*/

person.name.first = "George";
func person.name.last(){
    return "Bush";
}

/*
How to print the first name and the last name when the type of them can be function or string?
The solution: the result to call a non-func value returns the value itself!
*/

print(person.name.first());
// > George

print(person.name.last());
// > Bush

/*
It's not sure whether `person' has a property called age or not.
Just assume that `person' has already got a property named `age' if we need this property!
*/

print(person.age);
// > <null>

/*
It's not sure whether `person' has a member function called update or not.
Just assume that `person' has already got a function named `update' if we need to call it!
As for c++, we must define a pointer to the function, and test whether it's null when call the function pointer.
*/

person.update();
// >

```


A demo: Quick Sort!
-------------------

```
/*
Quick Sort Demo
*/
func show(array){
    for(i = 0; i < #array; ++i)
        write(array[i], " ");
    print();
}
func swap(arr, i, j){
    arr[i], arr[j] = arr[j], arr[i];
}
func partition(array, l, r){
    pivot = array[r];
    curpos = l;
    for(j = l; j < r; ++j){
        if(array[j] < pivot){
            swap(array, j, curpos);
            ++curpos;
        }
    }
    swap(array, r, curpos);
    return curpos;
}
func quicksort(array, l, r){
    if(l < r){
        interval = partition(array,l,r);
        quicksort(array,l,interval-1);
        quicksort(array,interval+1,r);
    }
}
func main(argc, argv)
{
    array = [49, 38, 65, 97, 48, 13, 27, 11, 56, 45];
    maxlen = #array;
    quicksort(array, 0, #array-1);
    show(array);
}
main();

```


##Simple tutorial!
```cpp
/*
   The following codes are totally legal!
   Try it!
   $ ./nl code.nl
*/

a[b] = 120;

a = ["Hello,", "NL!"];
print(a[0], a[1]);


for (i = 0; i < 2; ++i){
    write(a[i] .. " ");
}
print();

// how to print variables to screen
print("Hello, NL!");
write("Hello, ", "NL!");
print();

// how to concat strings
print("Hello"..",".." ".."NL".."!");

// how to define table, the magical data structure of NL!
x = [c = [d = "Hello, NL!"]];
x = [b = x];
a = x;
print(a.b.c.d);

// or we can define the same table in this way!
x.c.d = "Hello, NL!";
x.b = x;
a = x;
print(a.b.c.d);


// conditional structure
if (a == a){
    print("Hello, NL!");
}elseif(a == b){
    print("Hello, C++!");
}elseif(a == c){
    print("Hello, Python!");
}else{
    print("Hello, JS!");
}
// switch structure
language = "NL";
switch language {
// it's ok for this style:
// switch(language){
    "NL" {
        print("Hello, NL!");
        break;
        print("Hello, C++!");
    }
    "C++" {
        print("Hello, Python!");
    }
    default {
        print("Hello, Unknown!");
    }
}
// loop structure
str = "Hello, NL!";
counter = 0;
while (++counter < 100){
    print(str);
    if (counter == 1) break;
}

for (index = 0; index < 1; ++index){
    print(str);
    if (index == 0) break;
}

// define a class with 'setclass'!
// define 'Cat':
func Cat.init(name){
    _cat = [name=name, age=18];
    setclass(_cat, Cat);
    return _cat;
}
func Cat.show_name(self){
    print("Hello, " .. self.name .. "!");
}
cat_obj = Cat.init("NL");
cat_obj:show_name();
// or we can create the instance in this way!
newCat = Cat.init;
cat_obj = newCat("NL");
// there's another way to define a class!
// define 'Person':
func Person(name){
    person = [];
    person.name = name;
    // parameters of a member function should start with
    // a var which stands for an instance of the class.
    func person.show_name(self){
        print("Hello, " .. self.name .. "!");
    }
    func person.hello(){
        print("Hello, NL!");
    }
    return person;
}
// how to create an instance of Person
person = Person("NL");
// how to call member function
person:show_name();
// how to call class function
person.hello();
// how to visit member var

print("Hello, " .. person.name .. "!");


```

##Complete grammer!

```
program:
	stats

stats:
    stats stat
    | stat

stat:
	stat_clear_stack
	| expr_if | expr_for | expr_while | BREAK ';'
	| '{' stats '}'
	| '{' '}'
	| stat_return
	| ';'
stat_clear_stack:
	expr_func
	| stat_assign
	| expr_switch // clear stack for 'case' expr
	| expr ';'

stat_return:
	RETURN expr_list ';'

stat_assign:
	expr_not_empty_list ASSIGN

expr:
	expr_binop %prec EXPR_BINOP_TO_EXPR
	| table
expr_list:
	expr_not_empty_list
	|
expr_not_empty_list:
	expr
	| expr_not_empty_list ',' expr
	;
expr_func:
	expr_func_delegate expr_func_name_delegate '(' expr_func_param_list_delegate ')' '{' expr_func_body '}'
expr_func_delegate:
	TK_FUNC
expr_func_name_delegate:
	expr_func_name
expr_func_name:
	expr_func_name '.'  ID
	| ID
expr_func_param_list_delegate:
	expr_func_param_list
expr_func_param_list:
	expr_func_param_list ',' expr_func_param
	| expr_func_param
	|
expr_func_body:
	stats
expr_func_param:
	ID

expr_switch:
	SWITCH expr_switch_cond '{' expr_switch_body '}'
expr_switch_cond:
	expr
expr_switch_body:
	expr_switch_body_without_default
	| expr_switch_body_without_default expr_switch_default_delegate '{' stats '}'
expr_switch_default_delegate:
	DEFAULT
expr_switch_body_without_default:
	expr_switch_branch expr_switch_body_without_default
	|
expr_switch_branch:
	expr_switch_branch_cond '{' stats '}'
expr_switch_branch_cond:
	expr

expr_for:
	FOR '(' expr_for_init expr_for_cond ';' expr_for_loop ')' stat
expr_for_init:
	stat
expr_for_cond:
	expr
	|
expr_for_loop:
	expr
	|

expr_if:
	IF expr_if_cond  expr_if_body  expr_elseif_list expr_else
expr_if_cond:
	expr
expr_elseif_list:
	expr_elseif_list expr_elseif
	|
expr_elseif:
	ELSEIF expr_if_cond expr_if_body
expr_else:
	ELSE stat
	|
expr_if_body:
	stat

expr_while:
	WHILE expr_while_cond stat
expr_while_cond:
	'(' expr ')'

table:
	'['table_body ']'
	| '[' ']'
table_body:
	table_body ',' table_expr
	| table_expr
table_expr:
	expr ASSIGN expr
	| ID ASSIGN expr
	| expr

expr_binop:
	expr_binop ADD expr_binop
	| expr_binop SUB expr_binop
	| expr_binop MUL expr_binop
	| expr_binop DIV expr_binop
	| expr_binop LT expr_binop
	| expr_binop GT expr_binop
	| expr_binop EQ expr_binop
	| expr_binop NE expr_binop
	| expr_binop GE expr_binop
	| expr_binop LE expr_binop
	| expr_binop CONCAT expr_binop
	| expr_binop AND expr_binop
	| expr_binop OR expr_binop
	| expr_prefix
expr_prefix:
	'!' expr_postfix
	| SIZEOF expr_postfix
	| INC expr_postfix
	| DEC expr_postfix
	| SUB expr_postfix
	| expr_postfix
expr_postfix:
	expr_postfix '.' ID
	| expr_postfix '[' expr ']'
	| expr_postfix '(' expr_list ')'
	| expr_postfix ':' ID '(' expr_list ')'
	| expr_primary
expr_primary:
	TK_NUM
	| ID
	| TK_STR
	| '(' expr ')'

```

##Contact me!
![efc269616d61676f6f64677579c41e.jpg](https://csdn-code.oss.aliyuncs.com/note-attachments/20150910-0011-11652-2270/efc269616d61676f6f64677579c41e.jpg)
  *181*****772 [AT] qq [DOT] com* :)

