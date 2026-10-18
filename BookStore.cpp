/**
 * Name: Lorenzo Garcia-Hernandez
 * CWID: 878877109
 * Email: lagarcia85@csu.fullerton.edu
 */

#include "BookStore.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;
using std::stringstream;

namespace CPSC131::BookStore
{

    // Default constructor
    BookStore::BookStore()
    {
    }

    // Copy constructor
    BookStore::BookStore(const BookStore& other)
        : book_list_(other.book_list_),
          account_balance_(other.account_balance_)
    {
    }

    // Adjust account balance
    void BookStore::adjustAccountBalance(int adjustment)
    {
        account_balance_ += adjustment;
    }

    // Return account balance
    int BookStore::getAccountBalance() const
    {
        return account_balance_;
    }

    // Find a book by ISBN
    DoublyLinkedList::DoublyLinkedList<Book>::Iterator
    BookStore::findBook(std::string isbn) const
    {
        auto it = book_list_.begin();

        while (it != book_list_.end())
        {
            if ((*it).getIsbn() == isbn)
            {
                return it;
            }

            ++it;
        }

        return book_list_.end();
    }

    // Check if a book exists
    bool BookStore::bookExists(std::string isbn) const
    {
        return findBook(isbn) != book_list_.end();
    }

    // Get available stock
    size_t BookStore::getBookStockAvailable(std::string isbn) const
    {
        auto it = findBook(isbn);

        if (it == book_list_.end())
        {
            return 0;
        }

        return (*it).getStockAvailable();
    }

    // Get a book reference
    Book& BookStore::getBook(std::string isbn) const
    {
        auto it = findBook(isbn);

        if (it == book_list_.end())
        {
            throw std::out_of_range("Book not found");
        }

        return *it;
    }

    // Purchase inventory using a Book object
    void BookStore::purchaseInventory(const Book& book)
    {
        size_t quantity = book.getStockAvailable();
        size_t price = book.getPriceCents();

        // Buying books decreases account balance
        adjustAccountBalance(
            -static_cast<int>(price * quantity)
        );

        auto it = findBook(book.getIsbn());

        if (it != book_list_.end())
        {
            // Existing ISBN: update stock only
            (*it).adjustStockAvailable(
                static_cast<int>(quantity)
            );
        }
        else
        {
            // New ISBN: add book to inventory
            book_list_.push_back(book);
        }
    }

    // Purchase inventory using book details
    void BookStore::purchaseInventory(
        std::string title,
        std::string author,
        std::string isbn,
        size_t price_cents,
        size_t unit_count
    )
    {
        Book book(
            title,
            author,
            isbn,
            price_cents,
            unit_count
        );

        purchaseInventory(book);
    }

    // Print store inventory
    void BookStore::printInventory() const
    {
        cout << "*** Book Store Inventory ***" << endl;

        auto it = book_list_.begin();

        while (it != book_list_.end())
        {
            Book& book = *it;

            cout << "\""
                 << book.getTitle()
                 << "\", by "
                 << book.getAuthor()
                 << " ["
                 << book.getIsbn()
                 << "] ("
                 << book.getStockAvailable()
                 << " in stock)"
                 << endl;

            ++it;
        }
    }

    // Sell a book using ISBN
    void BookStore::sellToCustomer(
        std::string isbn,
        size_t price_cents,
        size_t quantity
    )
    {
        auto it = findBook(isbn);

        if (it == book_list_.end())
        {
            throw InsufficientInventory(
                "Book not found in inventory"
            );
        }

        sellToCustomer(*it, price_cents, quantity);
    }

    // Sell a book using a Book reference
    void BookStore::sellToCustomer(
        Book& book,
        size_t price_cents,
        size_t quantity
    )
    {
        // Check available inventory
        if (quantity > book.getStockAvailable())
        {
            throw InsufficientInventory(
                "Insufficient inventory to complete sale"
            );
        }

        // Reduce stock
        book.adjustStockAvailable(
            -static_cast<int>(quantity)
        );

        // Increase account balance
        adjustAccountBalance(
            static_cast<int>(price_cents * quantity)
        );
    }

} // namespace CPSC131::BookStore
