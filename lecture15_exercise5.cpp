#include <iostream>
#include <string>

class Number
{
public:
    Number();
    Number(unsigned int m);
    Number(const Number &l);
    Number(std::string s);
    ~Number();

    void print() const;

    Number operator+(const Number &x) const;
    Number operator*(const Number &x) const;
    bool operator==(const Number &x) const;
    bool operator!=(const Number &x) const;
    Number &operator=(const Number &x);

private:
    // Non modificare
    struct Cell
    {
        char info;
        Cell *next;
    };

    typedef Cell *List;

    List n;
    List sum(List l1, List l2, char carry) const;
    List convert(unsigned int m);
    void print_rec(List l) const;

    // Parte modificabile (ma attenzione: ora è progettata per guidarvi)

    void destroy(List h);
    List copy(const List pc);
    bool equal(List l1, List l2) const;
};

// Costruttore di default, 0 e' codificato con la lista vuota
Number::Number()
{
    n = nullptr;
}

// converte un unsigned int nella lista richiesta
Number::Number(unsigned int m)
{
    n = convert(m);
}

// studia questa funzione per comprendere completamente la rappresentazione nel numero
// e' un bell'esempio di ricorsione
Number::List Number::convert(unsigned int m)
{
    if (m == 0)
        return nullptr;
    else
    {
        List pc = new Cell;
        pc->info = m % 10;
        pc->next = convert(m / 10);
        return pc;
    }
}

// stampa la lista: siccome le cifre meno significative sono in testa, devo stampare al contrario
void Number::print() const
{
    if (n == nullptr)
        std::cout << "0";
    else
        print_rec(n);
}

// parte ricorsiva della stampa
void Number::print_rec(List l) const
{
    if (l)
    {
        print_rec(l->next);
        std::cout << static_cast<char>(l->info + '0'); // Ecco il type casting explicito in C++
    }
}

// distruttore
Number::~Number()
{
    destroy(n);
}

// funzione di disruzione ricorsiva
void Number::destroy(List h)
{
    if (h)
    {
        destroy(h->next);
        delete (h);
    }
}

// primo metodo da implementare: dato un numero codificato con una stringa
// scritto in modo che la cifra piu' significativa e' nella prima posizione
// generare un Number. Attenzione non fare la conversione string->int->list
// perche' potresti avere errori di overflow
Number::Number(std::string s)
{
    n = nullptr;
    if (s == "0")
        return;
    for (int i = 0; i < s.size(); i++)
    {
        char cifra = s[i] - '0';
        n = new Cell{cifra, n};
    }
}

// secondo metodo da implementare: copy constructor
// consigliato far uso della funzione privata ricorsiva copy
// potrebbe esserti utile anche per l'assegnamento!
Number::Number(const Number &x)
{
    n = copy(x.n);
}

Number::List Number::copy(const List pc)
{
    if (pc == nullptr)
        return nullptr;
    else
        return new Cell{pc->info, copy(pc->next)};
}

// l'operatore + non va modificato. Deve utilizzare il meotodo sum come indicato
Number Number::operator+(const Number &x) const
{
    Number res;
    res.n = sum(x.n, n, 0);
    return res;
}

// Questa funzione deve essere ricorsiva. Implementala con cura. Ricorda
// che l'ultimo carry potrebbe essere diverso da 0
Number::List Number::sum(List l1, List l2, char carry) const
{
    if (l1 == nullptr && l2 == nullptr && carry == 0)
        return nullptr;
    char added_value = carry;
    added_value = added_value +
                  (l1 == nullptr ? 0 : l1->info) +
                  (l2 == nullptr ? 0 : l2->info);
    List rest = sum(
        (l1 == nullptr ? nullptr : l1->next),
        (l2 == nullptr ? nullptr : l2->next),
        added_value / 10);
    if (added_value == 0 && rest == nullptr)
        return nullptr;
    else
        return new Cell{(char)(added_value % 10), rest};
}

// operatore di assegnamento. Segui la traccia
Number &Number::operator=(const Number &x)
{
    if (this != &x)
    {
        destroy(this->n);
        n = copy(x.n);
    }
    return *this;
}

// La seguente funzione puo' essere modificata, ma e'
// consigliato usare la equal sviluppata ricorsivamente
bool Number::operator==(const Number &x) const
{
    return equal(x.n, n);
}

// Da sviluppare
bool Number::equal(List l1, List l2) const
{
    if (l1 == nullptr && l2 == nullptr)
        return true;
    if (l1 == nullptr || l2 == nullptr)
        return false;
    return l1->info == l2->info && equal(l1->next, l2->next);
}

// Metodo velore per implementare il !=
bool Number::operator!=(const Number &x) const
{
    return !(*this == x);
}

// Fare il prodotto x*y significa sommare x+x+x+..+x (y volte...)
// non il modo piu' efficiente, ma il piu' facile
// Ricorda che tutte le operazioni vanno fatte coi numbers!
Number Number::operator*(const Number &x) const
{
    Number res(0);
    Number counter(0);
    Number one(1);
    for (Number counter(0); counter != *this; counter = counter + one)
        res = res + x;
    return res;
}

int main()
{

    std::string s1 = "1";
    std::string s2 = "14";
    Number n1 = Number(s1);
    Number n2 = Number(s2);
    Number result = n1 * n2;

    return 0;
}
