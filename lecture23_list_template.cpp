#include <iostream>

template <typename T>
class List
{
public:
    struct Cell
    {
        T info;
        Cell *next;
    };

    typedef Cell *Pcell;

    Pcell head;
    T dummy;

    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(Cell *p)
        {
            this->ptr = p;
        }

        iterator &operator++()
        { // prefix
            //++i
            this->ptr = this->ptr->next;
            return *this;
        }

        iterator operator++(int)
        { // postfix
            // i++
            iterator old = *this;
            ++this;
            // this->ptr = this->ptr->next;
            return old;
        }

        reference operator*()
        {
            return this->ptr->info;
        }

        pointer operator->()
        {
            return &(this->ptr);
        }

        bool operator==(const iterator &rhs) const
        {
            return this->ptr == rhs->ptr;
        }

        bool operator!=(const iterator &rhs) const
        {
            return this->ptr != rhs.ptr;
        }

        Cell *ptr;
    };

    iterator begin()
    {
        return iterator{this->head};
    }

    iterator end()
    {
        return iterator{nullptr};
    }

    List()
    {
        head = nullptr;
    }

    ~List()
    {
        destroy(head);
    }

    void destroy(Pcell pc)
    {

        if (pc != nullptr)
        {
            destroy(pc->next);
            delete pc;
        }
    }

    void read()
    {
        destroy(head);
        head = nullptr;
        T elem;
        std::cin >> elem;

        for (T i = 0; i < elem; i++)
        {
            T e;
            std::cin >> e;
            append(e);
        }
    }

    void append(T el)
    {
        if (head == nullptr)
            prepend(el);
        else
        {
            Pcell pc = head;
            while (pc->next != nullptr)
                pc = pc->next;
            pc->next = new Cell;
            pc->next->info = el;
            pc->next->next = nullptr;
        }
    }

    void prepend(T el)
    {
        Pcell newone = new Cell;
        newone->info = el;
        newone->next = head;
        head = newone;
    }

    void print() const
    {
        Pcell pc = head;
        while (pc)
        {
            std::cout << pc->info << std::endl;
            pc = pc->next;
        }
    }

    List(const List &s)
    {
        head = copy(s.head);
    }

    Pcell copy(Cell *source) const
    {
        if (source == nullptr)
            return nullptr;
        else
        {
            Pcell dest = new Cell;
            dest->info = source->info;
            dest->next = copy(source->next);
            return dest;
        }
    }

    List operator+(const List &x) const
    {
        List supp(*this);
        Pcell pc = supp.head;
        if (pc == nullptr)
        {
            supp.head = copy(x.head);
        }
        else
        {
            while (pc->next != nullptr)
                pc = pc->next;
            pc->next = copy(x.head);
        }

        return supp;
    }

    T &at(unsigned int pos)
    {
        Pcell pc = head;
        while (pc != nullptr && pos > 0)
        {
            pc = pc->next;
            pos--;
        }

        if (pc)
            return pc->info;
        else
            return dummy;
    }

    const T &at(unsigned int pos) const
    {
        Pcell pc = head;
        while (pc != nullptr && pos > 0)
        {
            pc = pc->next;
            pos--;
        }

        if (pc)
            return pc->info;
        else
            return dummy;
    }

    void merge(const List &l1, const List &l2)
    {
        destroy(head);
        head = merge_rec(l1.head, l2.head);
    }

    Pcell merge_rec(Pcell h1, Pcell h2)
    {
        if (h1 == nullptr && h2 == nullptr)
            return nullptr;
        else
        {
            if (h1 == nullptr)
            { // h1 vuota, h2 non vuota
                Pcell newone = new Cell;
                newone->info = h2->info;
                newone->next = merge_rec(h1, h2->next);
                return newone;
            }
            else
            {
                if (h2 == nullptr)
                { // h1 non vuota, h2 vuota
                    Pcell newone = new Cell;
                    newone->info = h1->info;
                    newone->next = merge_rec(h1->next, h2);
                    return newone;
                }
                else
                { // nessuna lista vuota
                    Pcell newone = new Cell;
                    if (h1->info < h2->info)
                    {
                        newone->info = h1->info;
                        newone->next = merge_rec(h1->next, h2);
                    }
                    else
                    {
                        newone->info = h2->info;
                        newone->next = merge_rec(h1, h2->next);
                    }
                    return newone;
                }
            }
        }
    }

    void merge_and_destroy(List &l1, List &l2)
    {

        if (this != &l1 && this != &l2)
        {
            if (&l1 != &l2)
            { // tre oggetti distinti
                destroy(head);

                head = merge_dest_rec(l1.head, l2.head);

                l1.head = nullptr;
                l2.head = nullptr;
            }
            else
            { // chiamata a = merge_and_destroy(b,b);
                List supp(l2);
                merge_and_destroy(l1, supp);
            }
        }
        else
        {
            if (this == &l1 && this != &l2)
            {                  // chiamata a = merge_and_destroy(a,b)
                List supp(l1); // duplico l1
                merge_and_destroy(supp, l2);
            }
            else if (this != &l1 && this == &l2)
            {                  // chiamata a = merge_and_destroy(b,a)
                List supp(l2); // duplico l2
                merge_and_destroy(l1, supp);
            }
            else
            {                   // chiamata a = merge_and_destroy(a,a)
                List supp1(l1); // duplico entrambi
                List supp2(l2);
                merge_and_destroy(supp1, supp2);
            }
        }
    }

    Pcell merge_dest_rec(Pcell l1, Pcell l2)
    {
        if (l1 == nullptr && l2 == nullptr)
            return nullptr;
        else
        {
            if (l1 == nullptr)
                return l2;
            else if (l2 == nullptr)
                return l1;
            else if (l1->info < l2->info)
            {
                l1->next = merge_dest_rec(l1->next, l2);
                return l1;
            }
            else
            {
                l2->next = merge_dest_rec(l1, l2->next);
                return l2;
            }
        }
    }

    void pop()
    {
        if (!this->empty())
        {
            Cell *temp = this->head;
            this->head = this->head->next;
            delete temp;
        }
    }

    bool empty()
    {
        return this->head == nullptr;
    }

    List<T> &operator=(List<T> &right)
    {
        if (this != &right)
        {
            while (!this->empty())
                this->pop();
            for (T el : right)
                this->append(el);
        }
        return *this;
    }

    List<T> &operator=(List<T> &&right)
    {
        if (this != &right)
        {
            while (!this->empty())
                this->pop();
            this->head = right.head;
            right.head = nullptr;
        }
        return *this;
    }

    List(List<T> &&s) : head(s.head)
    {
        // this->head = s.head;
        s.head = nullptr;
    }
};

List<std::string> foo()
{
    List<std::string> l1;
    std::string s1 = "pippo", s2 = "pluto";
    l1.append(s1);
    l1.append(s2);
    return l1;
}

template <typename T>
std::ostream &operator<<(std::ostream &lhs, List<T> &rhs)
{
    for (T el : rhs)
        lhs << el << ",";
    return lhs;
}

int main()
{
    List<std::string> l1 = foo();
    List<std::string> l2;
    std::string s3 = "paperino", s4 = "qui  quo qua";
    l2.append(s3);
    l2.append(s4);
    l1 = l2;
    std::cout << l1 << std::endl;
    return 0;
}
