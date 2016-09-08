/*
   The following codes are totally legal!
   Try it!
   $ ./nl code.nl
*/

// how to print variables to screen
println("Hello, NL!");
print("Hello,", "NL!");
println();

// how to concat strings
println("Hello"..",".." ".."NL".."!");

// how to define table, the magical data structure of NL!
x = {c = {d = "Hello, NL!"}};
x = {b = x};
a = x;
println(a.b.c.d);

// or we can define the same table in this way!
x.c.d = "Hello, NL!";
x.b = x;
a = x;
println(a.b.c.d);


// conditional structure
if (a == a){
    println("Hello, NL!");
}elseif(a == b){
	println("Hello, C++!");
}elseif(a == c){
	println("Hello, Python!");
}else{
	println("Hello, JS!");
}

// switch structure
language = "NL";
switch language {
// it's ok for this style:
// switch(language){
    "NL" {
        println("Hello, NL!");
        break;
        println("Hello, C++!");
    }
    "C++" {
        println("Hello, Python!");
    }
    default {
        println("Hello, Unknown!");
    }
}

// loop structure
str = "Hello, NL!";
counter = 0;
while (counter < 100){
    println(str);
    ++counter;
    if (counter == 1) break;
}
for (index = 0; index < 1; ++index){
    println(str);
    if (index == 0) break;
}
// how to define a class with 'setclass'!
// define a 'Cat' class


func Cat.init(name){
    _cat = {name=name, age=18};
    setclass(_cat, Cat);
    return _cat;
}
func Cat.show_name(self){
    println("Hello, " .. self.name .. "!");
}
cat_obj = Cat.init("NL");
cat_obj:show_name();

// or we can create the instance like this!


newCat = Cat.init;
cat_obj = newCat("NL");
// there's other way to define a class!
// define a 'Person' class!
func Person(name){
    person = {};
    person.name = name;
    // parameters of a member function should start with
    // a var which stands for an instance of the class.
    func person.show_name(self){
        println("Hello, " .. self.name .. "!");
    }
    func person.hello(){
        println("Hello, NL!");
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
println("Hello, " .. person.name .. "!");
