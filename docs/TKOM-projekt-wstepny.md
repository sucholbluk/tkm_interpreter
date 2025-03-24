**Autor:** Łukasz Suchołbiak 325236
## Temat projektu
Tematem projektu jest stworzenie języka, oraz :
- Wymagane cechy języka:
	- typowanie:
		- statycze
		- silne
	- zmienne:
		- domyślnie niemutowalne
		- przekazywane do funkcji przez referencję
	- posiada wsparcia dla mechanizmu funkcji wyższego rzędu
	- posiada conajmniej 2 wbudowane operatory dla mechanizmu funkcji wyższego rzędu 

## Założenia języka
#### Typy danych
- **całkowity:** `int`
	- wartości: od `9_223_372_036_854_775_808`, do `9_223_372_036_854_775_807`
- **zmiennoprzecinkowy:** `float`
- **logiczny:** `bool`
	- przyjmuje wartość `true` lub `false`
- **reprezentacja niczego:** `none`
	- język nie zezwala na tworzenie zmiennych typu `none`
- **typ znakowy:** `string`
- **funkcja**: `function<{typy_argumenów_po_przecinkach}:{typ_zwracacny}>``
	- np `function<int, int: int>`

### Znaki specjalne
### Operatory

##### Arytmetyczne
- `*`  - mnożenie
- `+`  - dodawanie
- `-`  - odejmowanie
- `/`  - dzielenie
##### Logiczne
- `not` - negacja
- `and` - logiczny and
- `or` - logiczne or
- `==` - równość
- `!=` - nierówność
- `>` - większość
- `<` - mniejszość
- `<=` - mniejsze lub równe
- `>=` większe lub równe
****
#### Tabela Typów Wspieranych Przez Operatory
Każdy z operatorów ma wybrane typy, które wspiera. Poniższa tabela przedstawia wsparcie operatorów dla typu

| operator \\ typ | integer | float            | boolean               | string |
| --------------- | ------- | ---------------- | --------------------- | ------ |
| `*`             | TAK     | TAK              | NIE                   | NIE    |
| `+`             | TAK     | TAK              | NIE                   | TAK    |
| `-`             | TAK     | TAK              | NIE                   | NIE    |
| `/`             | TAK     | TAK              | NIE                   | NIE    |
| `==`            | TAK     | TAK(? powiedzmy) | TAK (ale redundancja) | TAK    |
| `!=`            | TAK     | TAK              | TAK (ale redundancja) | TAK    |
| `>`             | TAK     | TAK              | NIE                   | TAK    |
| `<`             | TAK     | TAK              | NIE                   | TAK    |
| `<=`            | TAK     | TAK              | NIE                   | TAK    |
| `>=`            | TAK     | TAK              | NIE                   | TAK    |
| `not`           | NIE     | NIE              | TAK                   | NIE    |
| `and`           | NIE     | NIE              | TAK                   | NIE    |
| `or`            | NIE     | NIE              | TAK                   | NIE    |

Operatory działające na 2 argumentach **wymagają**, aby argumenty te były **tego samego typu**.
#### Konwersja typów
Wykorzystywana gdy zrzutować pewną wartość na inny typ. Aby nastąpiła konwersja typu, język wymaga aby programista jawnie ją zadeklarował.
##### Operator konwersji typu - `as`
- Szablon wykorzystania: `{wartość} as {typ}`
Przykłady:
- `4 as float`
- `"40000" as int`
- `70 as string` - rezultat: `"70"`
##### Tabela Konwersji Typów
| typ źródłowy\\typ docelowy | int | float | string | boolean | Możliwy błąd |
| -------------------------- | --- | ----- | ------ | ------- | ------------ |
| **int**                    | -   | TAK   | TAK    | NIE     | NIE          |
| **float**                  | TAK | -     | TAK    | NIE     | TAK          |
| **string**                 | TAK | TAK   | -      | TAK     | TAK          |
| **boolean**                | NIE | NIE   | TAK    | -       | NIE          |

**Realizacjia konwersji**
- int
	- -> float: prosta konwersja
	- -> string: zapisanie liczby w postaci ciągu znaków (np `123 as string == "123"`)
- float
	- -> int: obcięcie wartości po przecinku (floor)
		- możliwy błąd przekroczenia zakresu typu int
	- -> string: zapis jeszcze do ustalenia
- string
	- -> int: zeskanowanie liczby w formacie dziesiętnym
		- błąd gdy ciąg znaków nie jest poprawną liczbą całkowitą 
	- -> float: zeskanowanie floata
		- błąd gdy ciąg znaków nie jest poprawną liczbą zmiennoprzecinkową
	- -> bool:
		- błąd gdy ciąg znaków jest inny niż "true" lub "false"
##### Operatory funkcji wyższego rzędu
- `@` - operator dekoracji
- `$` - bind front
omówione dokładniej w [sekcji o funkcjach](#funkcje)
####  String
- `string`
- zapisywany między `""`
	- np. `"Hello World"t`
- Do zapisu niektórych znaków wykorzystywany jest escaping
- Dostępne sekwencje escape:
	- `\n` - nowa linia
	- `\t` - tabulacja
	- `\"` - cudzysłów
	- `\\` - backslash
### Komentarze
- komentaż zaczyna się od wartości `#`, od tego miejsca trwa do końca linii
Przykład:
```
4 + 8;# ta część należy już do komentarza
```
*komentarz*

W dalszej części komentarze będą używane do opisu konktretnych sytuacji w kodzie lub oznaczenia
### Zmienne
#### Nazywanie zmiennych
- składają się ze znaków ze zbioru - `[a-zA-Z0-9_]`
	- jendak pierwszy znak nie może być cyfrą
Przykłady
- poprawne nazwy: `_private`, `_`, `_1`, `numerek1`, `snake_case`, `camelCase`, `PascalCase`, `AAAAAAAA`, `_007`
- niepoprawne nazwy: `0`, `0klient`, `9_8_7`, `8mila`, `with.dot`, `ca$h`, etc.
#### Inicjowanie zmiennych
- Zmienna tworzone poprzez `let {nazwa_zmiennej}: {typ_zmiennej} = wartość`
- Domyślnie tworzone zmienne są niemutowalne, próba przypisania wartości do zmiennej spowoduje błąd 
Przykłady:
```
let x: int = 12;
x = 4; # BŁĄD
```
*inicjacja zmiennej typu i32*

```
let hello_world: string = "Hello World";
hello_world = "foo"; # BŁĄD
```
*inicjacja zmiennej typu string*

##### Tworzenie zmiennej mutowalnej
- Tworzenie zmiennej, której zawartość może być modyfikowana używane jest przez dodanie `mut` przed nazwą zmiennej podczas inicjalizacji
Przykład:
```
let mut x: int = 12;
x = 3; # OK
```
*inicjacja zmiennej mutowalnej*
### Scope
- `{}` określa zakres widoczności/scope
- Zmienne zainicjowane w daneym zakresie są widoczne:
	- na poziomie tego zakresu
	- na poziomie zakresów potomnych (przestrzeni wewnątrz tej przestrzeni)
- Kiedy kończy się zakres zmienne zainicjowane w niej są niszczone
Przykład:
```
def main() -> none {
	let a: int = 3;
	{
		let mut b: int = a + 4; # OK, a jest widoczne
	}
	b + 1; # BŁĄD b już nie isnieje
}
```
### Przykrywanie
- przykrywanie jest przydatne gdy chcemy użyć tej samej nazwy
- przykrywanie jest możliwe tylko w innym scopie i jest zreazizowane poprzez użycie składni takiej samej jak przy inicjowaniu zmiennej (żeby nie pomylić go z przypisaniem nowej wartości)
przykład
```
let a: int = 5;
{
	let mut a: int = 7; # OK, poprawnie przykryta
	a += 1;
}# przy opuszczeniu scopu przykrywające a zostaje zniszczone

a + 3; # 8 - w tym scopie widoczne a = 5

let mut a: int = 10; # BŁĄD - próba przykrycia w tym samym scopie
```
### Instrukcje warunkowe
- Zrealizowane standardowo przy użyciu `if ({warunek}) {scope i kod wewnątrz} else {scope i kod wewnątrz}`
- możliwe pominięcie zapewnienia `else`
- jako warunek może być podany tylko statement zwijający się do wartości logicznej - `bool`
- 
Przykłady:
```
let mut a: int = 12;

if (a < 13) {
	a += 13;
}
```
*pojedynczy if*

```
let animal: string = "cat";

if (animal is "cat"){
	print("meaow");
} else {
	print("bark");
}
```
*if else*

#### Łączenie instrukcji warunkowych
- zrealizowane prosto przez dodanie `if ({warunek})` po else
	- if nie musi być wewnątz `{}`
Przykład:
```
let animal: string = "cat";

if (animal is "cat"){
	print("meaow");
} else if (animal is "dog"){
	print("bark");
} else if (animal is "cow"){
	print("muuu");
} else {
	print("*silence*");
}

```
*połączone instrukcje warunkowe*
### Pętle
- standardowe for ({zmienna}; {warunek}; {operacja na zmiennej})
- jako uproszczenie składni zmienną określa się tylko przez nazwę, typ i wartość początkową
	- pomijane słówka let i mut ponieważ wiadomo, że  inicjujemy zmienną, oraz, że musi być mutowalna
Przykład:
```
for (i: int = 0; i < 7; i = i + 1) {
	print(i);
}
```
- Jednak nie można modyfikować wartości zmiennej na której operuje pętla wewnątrz jej ciała
	- pomimo, że iterujemy po wartości, więc będzie modyfikowana, to wewnątrz ciała pętli traktowana jest jako zmienna niemutowalna
Przykłady:
```
for (i: int = 0; i < 7; i = i + 1) {
	i += 1; # BŁĄD
}
```
### Funkcje
#### Definiowanie funkcji
- w definicji funkcji określana jest ilość, oraz typy przyjmowanych argumentów, oraz wartość zwracana przez tę funkcję
- schemat definicji: 
	1. `def {nazwa_funkcji}({argumenty_funkcji}) -> {zwracany typ}`
		- schemat standardowego argumentu to `{nazwa_argumentu}: {typ_argumentu}`
	2. a następnie scope - `{}`, wewnątrz którego znajduje się kod funkcji
		- wewnątrz tego scopu można odwoływać się do argumentów funkcji
przykład:
```
def sum_two(a: int, b: int) -> int {
	return a + b;
}

let a: int = 9;
let b: int = sum_two(a, 9); # OK
sum_two(a, b); # OK
sum_two(3, 4); # OK
```
##### Mutowalne argumenty
- do argumentów zadeklarowanych w powyższy sposób nie można stosować operatora przypisanie `=` -> nie można modyfikować ich wartości
```
def increment(a: int) -> none {
	a += 1; # BŁĄD
}
```
- Aby to było możliwe mutowalność musi zostać wskazana poprzez dodanie `mut` przed nazwę argumentu
	- jednak ogranicza to elementy, które mogą zostać podane jako argument tej funkcji do zmiennych mutowalnych
```
def increment(mut a: int) -> none {
	a += 1; # OK - a jest oznaczone jako mutowalne
}
...
let mut x: int = 4;
let y: int = 5;
increment(x); # OK, ale
increment(y); # BŁĄD - x nie jest mutowalne
increment(5); # BŁĄD - literał nie może wejść jako argument mut

```
##### Schemat przyjmowania jako argumenty funkcji

| **rodzaj deklracji argumentu**\\ **podane przy wywołaniu** | Zmienna (domyślna) | **Zmienna mutowalna** | **Literał**   |
| ---------------------------------------------------------- | ------------------ | --------------------- | ------------- |
| **Domyślna**                                               | przyjmuje          | przyjmuje             | przyjmuje     |
| **Jako mutowalny**                                         | nie przyjmuje      | przyjmuje             | nie przyjmuje |
##### Redefinicja funkcji
Język **nie zezwala** na redefinicję funkcji. Funkcja w programie może być zdefiniowana tylko raz. Wymusza to również selektywne importowanie funkcji.
```
def foo(word: string) -> string {
	return "Foo::" + word;
} 

def foo(word: string) -> string { # BŁĄD - redefinicja
	return word + "::ooF"; 
}
```
##### Przeciążanie funkcji
Język **nie zezwala na przeciążanie funkcji**, próba przeciążenia zostanie potraktowana jako redefinicja funkcji i będzie skutkowała zgłoszeniem błędu o redefinicji funkcji
Przykład:
```
def make_greetings(name: string) -> string {
	return "Hello " + name + "!";
} 

def make_greetings(name1: string, name2: string) -> string { # BŁĄD - redefinicja
	return "Hello " + name1 + " and " + name2"; 
}
```
#### Rekursywne wołanie funkcji
- możliwe wykonywanie przez podanie własnej nazwy wewnątrz funkcji z argumentem
Przykład:
```
def factorial(num: int) -> int {
	if (num is 2) {
		return 2;
	}

	return factorial(num - 1) * n;
}
```
*rekursywne wołanie funkcji*

#### Mechanizm funkcji wyższego rzędu
- język zezwala na przekazywanie funkcji do funkcji
- musi być zdefiniowany typ funkcji wejściowej: `function<{typy_zmiennych_po_przecinkach}:{typ_zwracanej_wartości}>`
	- np. `function<int, int, bool:int>`
#### Operator dekoracji
- powoduje, że jako pierwszy argument do funkcji przekazywana jest dekorowana funkcja
- typ dekorowanej funkcji musi być zgodny z typem określonym w dekoratorze
- schemat: `@{nazwa_dekoratora}({ewentualne podane argumenty}){definicja funkcji dekorowanej}`
Przykłady:
- dekorator nie przyjmujący żadnych dodatkowych argumentów:
```
def plus_one(f: function<int:int>) -> function<int:int> {
	def wrapper(b: i32) -> i32{
		return f(b) + 1;
	}
	return wrapper;
}

@plus_one()
def add_5(a: int) -> int {
	return a + 5;
}
```
- dekorator przyjmujący dodatkowy argument:
```
def call_n_times(f: function<none:none>, n: int) -> function<none:none> {
	def wrapper() -> none {
		for n in range(0, n) {
			f();
		}
	}
	return wrapper;
}

@call_n_times(5)
def hello_world() -> none {
	print("hello world");
}
```
#### Operator bind front
- powoduje stworzenie funkcji z ustalonymi pierwszymi argumentami wejściowymi
- relizowany za pomocą `&{nazwa_funkcji_do_ktorej_przywiązuje_argumenty}`
Przykłady:
1. 
```
def sum_two(a: int, b: int) -> int {
	return a + b;
}

def add_5(a: int) -> int &sum_two(5)


add_5(4); # daje 9
```
2. 
```
def add_three(a: int, b: int, c: int) -> int {
	return a + b + c;
}

def add_12(a: int) -> int &add_three(5, 7)

add_12(5); # daje 17

```


### Biblioteka standardowa języka - funkcje wbudowane
#### Obsługa strumieni I/O
- Relizowana przez funkcje **print()** oraz **input()**
##### print
- Typ: `function<string:none>`
- Działanie: Wypisuje zawartość podanego stringa na wyjście standardowe
##### input
- Typ: `function<none:string>`
- Działanie: Przyjmuje wartość podaną przez użytkownika z wejścia standardowego, czyta do wystąpienia pierwszego newline'a
#### Obsługa stringów
##### is_int
- Typ: `function<string:bool>`
- Działanie: Sprawdza czy podany string jest liczbą typu całkowitego, obsługuje również czytanie liczby ujemnej.
##### is_float
- Typ: `function<string:bool>`
- Działanie: Sprawcza czy podany string jest liczbą typu zmiennoprzecinkowego
- **czy ma obsługiwać notację matematyczną????** - chyba tak
##### is_bool
- Typ: `function<string:bool>`
- Działanie: Sprawdza czy podany string jest wartością logiczną

##### lower
- Typ: `function<string:string>`
- Działanie: Zwraca stringa wejściowego, ze wszystkimi literami zmienionymi na małe
##### upper
- Typ: `function<string:string>`
- Działanie: Zwraca stringa wejściowego, ze wszystkimi literami zmienionymi na duże
##### capitalize
- Typ: `function<string:string>`
- Działanie: Zmienia pierwszą literę stringa na dużą, a resztę na małe


## EBNF
```BNF
program = { statement };

statement = function_definition
          | code_block
          | variable_declaration
          | ( expression, ";" )
          | assignment
          | if_statement
          | for-loop
          | decoration
          ;


function_definition = function_signature, code_block | bind_front;
function_signature  = def, identifier, "(", parameter_list, ")", "->", type;

bind_front = bindf, function_call;

variable_declaration = let, [ mut ], typed_identifier, assign ";";

parameter_list   = {typed_identifier};
typed_identifier = identifier, ":", type_not_none;

assignment = identifier, assign ";";
assign = asgn, expression;

if_statement = if, condition, code_block [else, code_block | if_statement];
condition    = "(", expression, ")";

for-loop        = for, "(", loop-var-decl, ";", expression, ";", assignment, ")", code_block;
loop-var-decl   = typed_identifier, assign;

decoration = dec, function_call;


code_block = "{", { statement } "}";

expression              = logical_or;
logical_or_expression   = logical_and_expression { or, logical_and_expression };
logical_and_expression  = equality_expression { and, equality_expression };
equality_expression     = comparison_expression { equality_operator, comparison_expression };
comparison_expression   = term { comparison_operator, term };
term                    = factor { additive_operator, factor };
factor                  = cast { multiplicative_operator, cast };
cast                    = unary [ as, type_not_none ];
unary                   = primary | ( unary_operator, primary );
primary                 = identifier
                        | literal
                        | "(", expression ")"
                        | function_call;

function_call = identifier, "(", [ function_call_arguments ], ")";
function_call_arguments = function_call_argument, {",", function_call_argument};
function_call_argument  = expression; 

literal = literal_integer
        | literal_float
        | literal_bool
        | literal_string;

literal_integer = "0" | ( non_zero_digit, { digit } );
literal_float   = { digit }, ".", { digit };
literal_bool    = "true" | "false";
literal_string  = '"', { escape_sequence | string_char}, '"';

escape_sequence = "\", escape_chars;
escape_chars    = '"', "n", "t", "\";
string_char     = letter | digit | special_chars;


identifier = "_" | letter, { "_" | letter | digit};

type = none 
     | type_not_none;

type_not_none = int
              | float
              | bool
              | string;

function_type = function, "<", argument_types, ":", return_type, ">";
argument_types = [type_not_none, {",", type_not_none}];
return_type = type;

equality_operator       = eq 
                        | neq;

unary_operator          = minus 
                        | not;

comparison_operator     = eq
                        | neq
                        | lt
                        | gt
                        | leq
                        | geq;

multiplicative_operator = mul
                        | div;

additive_operator       = plus
                        | minus;


letter  = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" 
        | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
        | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" 
        | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z";

digit   = "0" | non_zero_digit;
non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";

special_chars   = "!" | "#" | "$" | "%" | "&" | "'" | "(" | ")" | "*" | "+" | "," | "-"
                | "." | "/" | ":" | ";" | "<" | "=" | ">" | "?" | "@" | "[" | "]" | "^"
                | "_" | "`" | "{" | "|" | "}" | "~";

not     = "not";
or      = "or";
and     = "and";

minus   = "-";
plus    = "+";
mul     = "*";
div     = "/";
asgn    = "=";

eq      = "==";
neq     = "!=";
lt      = "<";
gt      = ">";
leq     = "<=";
geq     = ">=";

def      = "def";
let      = "let";
mut      = "mut";
as       = "as";
if       = "if";
else     = "else";
for      = "for";

int      = "int";
float    = "float";
bool     = "bool";
string   = "string";
none     = "none";
function = "function";

bindf    = "&";
dec      = "@";
```
## Obsługa - korzystanie z kompilatora
```
```
#### Dane konfiguracyjne
#### Sposób uruchomienia
## Wymagania funkcjonalne i niefunkcjonalne

## Sposób realizacji
### Koncepcja realizacji
### Testowanie
