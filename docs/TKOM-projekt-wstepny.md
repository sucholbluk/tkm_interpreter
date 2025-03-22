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
##### Podstawowe
- **całkowite**:
	- **int**: 
		- `i16`: od `−32_768`, do `32_767`
		- `i32`: od `-2_147_483_648`, do `2_147_483_647` 
		- `i64`: 
	- **unsigned**:
		- `u16`: od `0`, do `65_535`
		- `u32`: od `0`, do `4_294_967_295`
		- `u64`: od `0`, do `18_446_744_073_709_551_615`
- **zmiennoprzecinkowe:**
	- `f32`: pojedynczej precyzji
	- `f64`: podwójnej precyzji
- **logiczny:** `bool`
	- przyjmujący wartości `true`, lub `false`
- **typ znakowy:** `string`
- **reprezentacja niczego**: `none`
##### Złożone
- **wektor**: `vector<typ_elementów>`
- **mapa**: `map<typ_klucza, typ_wartości>`
- **funkcja**: `function<typy_argumenów_po_przecinkach:typ_zwracacny>``
ewentualnie:
- **opcji**: `option<typ_możliwej_zawartości>` - option będzie dostępne tylko dla typów wbudowanych

// może do ucięcia i zastąpienia przez: - PAMIĘTAĆ ŻEBY ZMIENIĆ W PRZYKŁADACH
- **całkowity:** `i64`
	- wartości: od `9_223_372_036_854_775_808`, do `9_223_372_036_854_775_807`
- **zmiennoprzecinkowy:** `f64`
- **logiczny:** `bool`
	- przyjmuje wartość `true` lub `false`
- **reprezentacja niczego:** `none`
- **typ znakowy:** `string`
- **funkcja**: `function<typy_argumenów_po_przecinkach:typ_zwracacny>``

- ewentualne rozwinięcie o typ **opcji**
	- `option<typ_możliwej_zawartości>` - będzie dostępne tylko dla typów wbudowanych

### Wbudowane operatory

### Inicjowanie zmiennych
- Zmienna tworzone poprzez `let {nazwa_zmiennej}: {typ_zmiennej} = wartość`
- Domyślnie tworzone zmienne są niemutowalne, próba przypisania wartości do zmiennej spowoduje błąd 
Przykłady:
```
let x: i32 = 12;
x = 4; # BŁĄD
```
*zmienna typu i32*

```
let hello_world: string = "Hello World";
hello_world = "foo"; # BŁĄD
```
*zmienna typu string*

##### Tworzenie zmiennej mutowalnej
- Tworzenie zmiennej, której zawartość może być modyfikowana używane jest przez dodanie `mut` przed nazwą zmiennej podczas inicjalizacji
Przykład:
```
let mut x: i32 = 12;
x = 3; # OK
```
### Scope
- `{}` określa zakres widoczności
- Zmienne zainicjowane w daneym zakresie są widoczne:
	- na poziomie tego zakresu
	- na poziomie zakresów potomnych (przestrzeni wewnątrz tej przestrzeni)
- Kiedy kończy się przestrzeń zmienne zainicjowane w niej są niszczone
Przykład:
```
def main() -> none {
	let a: i32 = 3;
	{
		let mut b: i32 = a + 4; # OK, a jest widoczne
	}
	b + 1; # BŁĄD b już nie isnieje
}
```
### Przykrywanie
- przykrywanie jest przydatne gdy chcemy użyć tej samej nazwy
- przykrywanie jest możliwe tylko w innym scopie i jest zreazizowane poprzez użycie składni takiej samej jak przy inicjowaniu zmiennej (żeby nie pomylić go z przypisaniem nowej wartości)
przykład
```
let a: i32 = 5;
{
	let mut a: i32 = 7; # OK, poprawnie przykryta
	a += 1;
}# przy opuszczeniu scopu przykrywające a zostaje zniszczone

a + 3; # 8 - w tym scopie widoczne a = 5

let mut a: i32 = 10; # BŁĄD - próba przykrycia w tym samym scopie
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
def sum_two(a: i32, b: i32) -> i32 {
	return a + b;
}

let a: i32 = 9;
let b: i32 = sum_two(a, 9); # OK
sum_two(a, b); # OK
sum_two(3, 4); # OK
```
##### Mutowalne argumenty
- do argumentów zadeklarowanych w powyższy sposób nie można stosować operatora przypisanie `=` -> nie można modyfikować ich wartości
```
def increment(a: i32) -> none {
	a += 1; # BŁĄD
}
```
- Aby to było możliwe mutowalność musi zostać wskazana poprzez dodanie `mut` przed nazwę argumentu
	- jednak ogranicza to elementy, które mogą zostać podane jako argument tej funkcji do zmiennych mutowalnych
```
def increment(mut a: i32) -> none {
	a += 1; # OK - a jest oznaczone jako mutowalne
}
...
let mut x: i32 = 4;
let y: i32 = 5;
increment(x); # OK, ale
increment(y); # BŁĄD - x nie jest mutowalne
increment(5); # BŁĄD - literał nie może wejść jako argument mut

```
#### Schemat przyjmowania jako argumenty funkcji

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
#### Mechanizm funkcji wyższego rzędu
- język zezwala na przekazywanie funkcji do funkcji
- musi być zdefiniowany typ funkcji wejściowej: `function<{typy_zmiennych_po_przecinkach}:{typ_zwracanej_wartości}>`
	- np. `function<u32, u32, bool:u32>`
#### Operator dekoracji
- powoduje, że jako pierwszy argument do funkcji przekazywana jest dekorowana funkcja
- typ dekorowanej funkcji musi być zgodny z typem określonym w dekoratorze
- schemat: `@{nazwa_dekoratora}({ewentualne podane argumenty}){definicja funkcji dekorowanej}`
Przykłady:
- dekorator nie przyjmujący żadnych dodatkowych argumentów:
```
def plus_one(f: function<i32:i32>) -> function<i32:i32> {
	def wrapper(b: i32) -> i32{
		return f(b) + 1;
	}
	return wrapper;
}

@plus_one()
def add_5(a: i32) -> i32 {
	return a + 5;
}
```
- dekorator przyjmujący dodatkowy argument:
```
def call_n_times(f: function<none:none>, n: u32) -> function<none:none> {
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
def sum_two(a: i32, b: i32) -> i32 {
	return a + b;
}

def add_5(a: i32) -> i32 &sum_two(5)


add_5(4); # daje 9
```
2. 
```
def add_three(a: u32, b: u32, c: u32) -> u32 {
	return a + b + c;
}

def add_12(a: u32) -> &add_three(5, 7)

add_12(5); # daje 17

```

## EBNF
#### Języka
#### Strumieni I/O ????
#### Danych konfiguracyjnych???

## Sposób uruchomienia
## Wymagania funkcjonalne i niefunkcjonalne

## Opis sposobu realizacji
## Opis sposobu testowania
