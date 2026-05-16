import json
import os
from datetime import date, timedelta, datetime

# ─────────────────────────────────────────────────────────────
#  JSON FILE PATHS  (created automatically in same folder)
# ─────────────────────────────────────────────────────────────
BOOKS_FILE          = "books.json"
MEMBERS_FILE        = "members.json"
TRANSACTIONS_FILE   = "transactions.json"
BORROW_RECORDS_FILE = "borrow_records.json"


# ─────────────────────────────────────────────────────────────
#  JSON HELPER FUNCTIONS
# ─────────────────────────────────────────────────────────────
def load_json(filepath):
    """
    Load a JSON file and return its contents as a list.
    If the file does not exist, returns [].
    If the file is corrupted/has wrong format, deletes it and returns [].
    """
    if not os.path.exists(filepath):
        return []
    try:
        with open(filepath, "r") as f:
            data = json.load(f)
        if isinstance(data, list):
            return data
        # File exists but is not a list – reset it
        print(f"Warning: '{filepath}' had unexpected format and was reset.")
        os.remove(filepath)
        return []
    except (json.JSONDecodeError, ValueError):
        print(f"Warning: '{filepath}' was corrupted and has been reset.")
        os.remove(filepath)
        return []


def save_json(filepath, data):
    """Save a list of dicts to a JSON file."""
    with open(filepath, "w") as f:
        json.dump(data, f, indent=4)


# ─────────────────────────────────────────────────────────────
#  MODEL CLASSES
# ─────────────────────────────────────────────────────────────

class Book:
    def __init__(self, book_id, title, author, genre,
                 total_quantity, available_quantity, borrow_quantity):
        self.book_id            = book_id
        self.title              = title
        self.author             = author
        self.genre              = genre
        self.quantity           = total_quantity
        self.available_quantity = available_quantity
        self.borrow_quantity    = borrow_quantity

    def __str__(self):
        return (f"{self.book_id} | {self.title} | {self.author} | {self.genre} | "
                f"Total:{self.quantity} | Available:{self.available_quantity} | "
                f"Borrowed:{self.borrow_quantity}")

    # Convert object → dict for JSON saving
    def to_dict(self):
        return {
            "book_id":            self.book_id,
            "title":              self.title,
            "author":             self.author,
            "genre":              self.genre,
            "quantity":           self.quantity,
            "available_quantity": self.available_quantity,
            "borrow_quantity":    self.borrow_quantity,
        }

    # Rebuild object from dict loaded from JSON
    @staticmethod
    def from_dict(d):
        return Book(
            d.get("book_id",            "UNKNOWN"),
            d.get("title",              "Unknown Title"),
            d.get("author",             "Unknown Author"),
            d.get("genre",              "Unknown"),
            d.get("quantity",           0),
            d.get("available_quantity", 0),
            d.get("borrow_quantity",    0),
        )


class Member:
    def __init__(self, member_id, name, phone, member_type, borrowed_books=None):
        self.member_id      = member_id
        self.name           = name
        self.phone          = phone
        self.type           = member_type
        self.borrowed_books = borrowed_books if borrowed_books is not None else []

    def __str__(self):
        return (f"{self.member_id} | {self.name} | {self.phone} | "
                f"{self.type} | Borrowed: {len(self.borrowed_books)}")

    def to_dict(self):
        return {
            "member_id":      self.member_id,
            "name":           self.name,
            "phone":          self.phone,
            "type":           self.type,
            "borrowed_books": self.borrowed_books,
        }

    @staticmethod
    def from_dict(d):
        return Member(
            d.get("member_id",      "UNKNOWN"),
            d.get("name",           "Unknown"),
            d.get("phone",          ""),
            d.get("type",           "Standard"),
            d.get("borrowed_books", []),
        )


class Transaction:
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id      = member_id
        self.book_id        = book_id
        self.Borrow_Date    = Borrow_Date   # always stored as "YYYY-MM-DD" string
        self.Due_Date       = Due_Date      # always stored as "YYYY-MM-DD" string

    def __str__(self):
        return (f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | "
                f"{self.Borrow_Date} | {self.Due_Date}")

    def to_dict(self):
        return {
            "Transaction_ID": self.Transaction_ID,
            "member_id":      self.member_id,
            "book_id":        self.book_id,
            "Borrow_Date":    str(self.Borrow_Date),
            "Due_Date":       str(self.Due_Date),
        }

    @staticmethod
    def from_dict(d):
        return Transaction(
            d.get("Transaction_ID", "UNKNOWN"),
            d.get("member_id",      "UNKNOWN"),
            d.get("book_id",        "UNKNOWN"),
            d.get("Borrow_Date",    "1900-01-01"),
            d.get("Due_Date",       "1900-01-01"),
        )


class Borrow_Records:
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id      = member_id
        self.book_id        = book_id
        self.Borrow_Date    = Borrow_Date   # always stored as "YYYY-MM-DD" string
        self.Due_Date       = Due_Date      # always stored as "YYYY-MM-DD" string

    def __str__(self):
        return (f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | "
                f"{self.Borrow_Date} | {self.Due_Date}")

    def to_dict(self):
        return {
            "Transaction_ID": self.Transaction_ID,
            "member_id":      self.member_id,
            "book_id":        self.book_id,
            "Borrow_Date":    str(self.Borrow_Date),
            "Due_Date":       str(self.Due_Date),
        }

    @staticmethod
    def from_dict(d):
        return Borrow_Records(
            d.get("Transaction_ID", "UNKNOWN"),
            d.get("member_id",      "UNKNOWN"),
            d.get("book_id",        "UNKNOWN"),
            d.get("Borrow_Date",    "1900-01-01"),
            d.get("Due_Date",       "1900-01-01"),
        )


# ─────────────────────────────────────────────────────────────
#  ACTIONS CLASS  –  all business logic lives here
# ─────────────────────────────────────────────────────────────

class Actions:
    def __init__(self):
        # Load previously saved data from JSON files on every startup
        self.books          = [Book.from_dict(d)          for d in load_json(BOOKS_FILE)]
        self.members        = [Member.from_dict(d)        for d in load_json(MEMBERS_FILE)]
        self.transactions   = [Transaction.from_dict(d)   for d in load_json(TRANSACTIONS_FILE)]
        self.borrow_records = [Borrow_Records.from_dict(d)for d in load_json(BORROW_RECORDS_FILE)]

    # ── private save helpers ──────────────────────────────────
    def _save_books(self):
        save_json(BOOKS_FILE, [b.to_dict() for b in self.books])

    def _save_members(self):
        save_json(MEMBERS_FILE, [m.to_dict() for m in self.members])

    def _save_transactions(self):
        save_json(TRANSACTIONS_FILE, [t.to_dict() for t in self.transactions])

    def _save_borrow_records(self):
        save_json(BORROW_RECORDS_FILE, [br.to_dict() for br in self.borrow_records])

    # ══════════════════════════════════════════════════════════
    #  BOOK MANAGEMENT
    # ══════════════════════════════════════════════════════════

    def add_book(self):
        title          = input("Enter Title: ")
        author         = input("Enter Author: ")
        total_quantity = int(input("Enter Quantity: "))
        available_quantity = total_quantity
        borrow_quantity    = 0          # nothing borrowed yet

        total_books = len(self.books)
        book_id     = f"BK00{total_books + 1}"

        print("Now select Genre")
        print("1. Fiction")
        print("2. Non-Fiction")
        print("3. Science")
        print("4. History")
        print("5. Technology")
        genre_choice = input("Choose the Genre type: ")

        genre_map = {
            "1": "Fiction",
            "2": "Non-Fiction",
            "3": "Science",
            "4": "History",
            "5": "Technology",
        }
        genre = genre_map.get(genre_choice, "Unknown")

        self.books.append(Book(book_id, title, author, genre,
                               total_quantity, available_quantity, borrow_quantity))
        self._save_books()
        print("✅ Book added successfully!")

    def view_book(self):
        if not self.books:
            print("No books available.")
            return
        for book in self.books:
            print(book)

    def update_quantity(self):
        found = False
        ID = input("Enter the Book ID to know its quantity: ")
        for book in self.books:
            if ID in book.book_id:
                found = True
                print(f"Total: {book.quantity} | "
                      f"Available: {book.available_quantity} | "
                      f"Borrowed: {book.borrow_quantity}")
        if not found:
            print("😠 You have entered a wrong ID")

    def remove_book(self):
        ID   = input("Enter the Book ID to remove: ")
        save = None
        for book in self.books:
            if ID in book.book_id:
                save = book
                break
        if save:
            self.books.remove(save)
            self._save_books()
            print("Book removed from Library")
        else:
            print("❌ Book ID not found.")

    # ══════════════════════════════════════════════════════════
    #  MEMBER MANAGEMENT
    # ══════════════════════════════════════════════════════════

    def add_member(self):
        y1 = input("Enter Member's name: ")
        y2 = input("Enter mobile number: ")
        print("Select membership type")
        print("1.  Standard (Max 2 books, 7 days)")
        print("2.  Premium  (Max 5 books, 7 days)")
        y3 = input("Enter choice (1-2): ")

        if y3 == "1":
            membership = "Standard"
        elif y3 == "2":
            membership = "Premium"
        else:
            print("❌ Invalid choice. Defaulting to Standard.")
            membership = "Standard"

        member_number = len(self.members) + 1
        y4 = f"MEM00{member_number}"

        print("------------------------------------------")
        print("✓ Member registered successfully!")
        print(f"Member ID : {y4}")
        print(f"Name      : {y1}")
        print(f"Phone     : {y2}")
        print(f"Type      : {membership}")
        print(f"Borrowed  : []")
        print("Status    : Active")
        print("-----------------------------------------")

        self.members.append(Member(y4, y1, y2, membership))
        self._save_members()

    def view_member(self):
        if not self.members:
            print("No members available.")
            return
        for member in self.members:
            print(member)

    def update_member(self):
        ID    = input("Enter Member ID to view details: ")
        found = False
        for mem in self.members:
            if ID in mem.member_id:
                print(mem)
                found = True
        if not found:
            print("❌ Member ID not found.")

    def remove_member(self):
        ID   = input("Enter Member ID to be removed: ")
        save = None
        for mem in self.members:
            if ID in mem.member_id:
                save = mem
                break
        if save:
            self.members.remove(save)
            self._save_members()
            print("Member has been removed")
        else:
            print("❌ Member ID not found.")

    # ══════════════════════════════════════════════════════════
    #  BORROW & RETURN
    # ══════════════════════════════════════════════════════════

    def borrow_book(self):
        today    = date.today()
        due_date = today + timedelta(days=7)
        z3 = len(self.transactions) + 1
        z4 = f"TXN00{z3}"

        # Validate member
        z1 = input("Enter your Member ID: ")
        found_member = None
        for m in self.members:
            if z1 == m.member_id:
                found_member = m
                break

        if found_member is None:
            print("Member not registered")
            return

        # Validate book
        z2 = input("Enter Book ID you require: ")
        found_book = None
        for b in self.books:
            if z2 == b.book_id:
                found_book = b
                break

        if found_book is None:
            print("This book doesn't exist")
            return

        # Check availability
        if int(found_book.available_quantity) <= 0:
            print("❌ No available copies of this book right now.")
            return

        # Check member borrow limit
        Name  = found_member.name
        Type  = found_member.type
        z5    = found_member.borrowed_books
        limit = 2 if Type == "Standard" else 5

        if len(z5) >= limit:
            print("Your limit reached. No further book can be issued.")
            print(f"You already have {len(z5)} book(s) in your account.")
            print("-------------------------------------------------")
            return

        # All checks passed – process borrow
        found_book.available_quantity = int(found_book.available_quantity) - 1
        found_book.borrow_quantity    = int(found_book.borrow_quantity) + 1
        z5.append(z2)

        print("✓ Book borrowed successfully!")
        print("----------Borrowing Receipt-------------")
        print(f"Member Name      : {Name}")
        print(f"Membership Type  : {Type}")
        print(f"Transaction ID   : {z4}")
        print(f"Book Borrowed    : {found_book.title}")
        print(f"Borrow Date      : {today}")
        print(f"Due Date         : {due_date}")
        print(f"Books in account : {len(z5)}")
        print("-------------------------------------------------")

        # Store dates as strings so JSON can save them
        self.transactions.append(
            Transaction(z4, z1, z2, str(today), str(due_date)))
        self.borrow_records.append(
            Borrow_Records(z4, z1, z2, str(today), str(due_date)))

        self._save_books()
        self._save_members()
        self._save_transactions()
        self._save_borrow_records()

    def return_book(self):
        z6          = input("Enter Member ID: ")
        z7          = input("Enter Book ID: ")
        Fine_per_day = 10
        today1      = date.today()

        # Find matching borrow record (must match BOTH member and book)
        save = None
        for BR in self.borrow_records:
            if BR.member_id == z6 and BR.book_id == z7:
                save = BR
                break

        if save is None:
            print("❌ No active borrow record found for this Member ID + Book ID.")
            return

        # Parse date strings back to date objects for arithmetic
        duedate    = datetime.strptime(save.Due_Date,    "%Y-%m-%d").date()
        borrowdate = datetime.strptime(save.Borrow_Date, "%Y-%m-%d").date()
        DT         = (today1 - duedate).days
        days_taken = (today1 - borrowdate).days

        print("✓ Book returned successfully!")
        print("-------- RETURN RECEIPT ---------")
        print(f"Transaction ID : {save.Transaction_ID}")
        print(f"Member         : {save.member_id}")
        print(f"Book           : {save.book_id}")
        print(f"Borrow Date    : {save.Borrow_Date}")
        print(f"Due Date       : {save.Due_Date}")
        print(f"Return Date    : {today1}")
        print(f"Days Borrowed  : 7")
        print(f"Days Taken     : {days_taken}")

        if DT <= 0:
            print("Status         : Returned on time")
            print("Fine Amount    : Rs. 0")
        else:
            print(f"Status         : {DT} days late")
            print(f"Fine           : Rs. {DT * Fine_per_day}")

        # Remove from active borrow records
        self.borrow_records.remove(save)

        # Remove book from member's borrowed list
        for m in self.members:
            if z6 == m.member_id:
                if z7 in m.borrowed_books:
                    m.borrowed_books.remove(z7)
                print(f"{z7} removed from {z6}'s record")
                print(f"Remaining books: {m.borrowed_books}")
                print(f"Now {z6} has {len(m.borrowed_books)} book(s)")
                print("------------------------------------------------")
                break

        # Update book availability
        for b in self.books:
            if b.book_id == z7:
                b.available_quantity = int(b.available_quantity) + 1
                b.borrow_quantity    = int(b.borrow_quantity) - 1
                break

        self._save_books()
        self._save_members()
        self._save_borrow_records()

    def active_borrowings(self):
        record_found = False
        z8 = input("Enter your Member ID to see your Borrowed Record: ")
        print("Your Borrow Record is given below")
        for BR in self.borrow_records:
            if z8 == BR.member_id:
                record_found = True
                print(BR)
        if not record_found:
            print("You don't have any book in your Borrow Record")

    def check_overdue(self):
        today2 = date.today()
        z9 = input("Enter your Member ID to check your OVERDUE Record: ")
        found = False
        for BR in self.borrow_records:
            if z9 == BR.member_id:
                found = True
                print(f"{BR.book_id} is found in your borrow record")
                # Dates are strings in JSON – convert before arithmetic
                borrowdate = datetime.strptime(BR.Borrow_Date, "%Y-%m-%d").date()
                duedate    = datetime.strptime(BR.Due_Date,    "%Y-%m-%d").date()

                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left  = (duedate - today2).days

                print(f"You have taken {total_days} days till now")

                if duedate >= today2:
                    print(f"You have {days_left} day(s) left")
                else:
                    print(f"You have taken {Extra_days} extra days")
                    print(f"Fine till today: Rs. {Extra_days * 10}")
        if not found:
            print("No borrow record found for this Member ID.")

    # ══════════════════════════════════════════════════════════
    #  REPORTS & STATISTICS
    # ══════════════════════════════════════════════════════════

    def library_summery(self):
        Total_Copies = []
        Standard     = []
        Premium      = []
        print("Books Summary:")
        print(f"Total Book Titles in Library : {len(self.books)}")

        for b in self.books:
            print(f"  {b.title} — {b.quantity} copies registered")
            bk = sum(1 for BR in self.borrow_records if BR.book_id == b.book_id)
            print(f"    Available copies: {b.quantity - bk}")
            Total_Copies.append(b.quantity)

        print(f"Total Copies in Library    : {sum(Total_Copies)}")
        print(f"Currently Borrowed Copies  : {len(self.borrow_records)}")
        print("---------------------------------------------")
        print(f"Total Registered Members   : {len(self.members)}")

        for m in self.members:
            if m.type == "Standard":
                Standard.append(m.name)
            else:
                Premium.append(m.name)

        print(f"Standard Members           : {len(Standard)}")
        print(f"Premium Members            : {len(Premium)}")
        print("----------------------------------------------------------")
        print(f"Total Transactions (all time) : {len(self.transactions)}")
        print(f"Active Borrowings             : {len(self.borrow_records)}")
        print(f"Completed Returns             : {len(self.transactions) - len(self.borrow_records)}")

    def popular_books(self):
        IDs = [record.book_id for record in self.transactions]
        if not IDs:
            print("No book has been borrowed yet in this library")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            title = most_repeated
            for b in self.books:
                if b.book_id == most_repeated:
                    title = b.title
                    break
            print(f"Most popular book: {title} ({most_repeated})")

    def popular_member(self):
        IDs = [record.member_id for record in self.transactions]
        if not IDs:
            print("No member has borrowed any book yet")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            name = most_repeated
            for m in self.members:
                if m.member_id == most_repeated:
                    name = m.name
                    break
            print(f"Most active member: {name} ({most_repeated})")

    def genre_wise(self):
        genre_list = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        total      = len(self.books) if self.books else 1   # avoid division by zero

        print(f"{'Genre':<15} {'Count':<10} {'Percentage'}")
        print("-" * 35)
        for g in genre_list:
            m1         = sum(1 for b in self.books if b.genre == g)
            percentage = round((m1 / total) * 100, 1)
            print(f"{g:<15} {m1:<10} {percentage}%")

    def monthly_borrow_report(self):
        a1 = int(input("Enter Year (e.g. 2025): "))
        a2 = int(input("Enter Month (1-12): "))
        found = False
        for record in self.transactions:
            # Dates stored as strings – parse for comparison
            borrowdate = datetime.strptime(record.Borrow_Date, "%Y-%m-%d").date()
            if borrowdate.year == a1 and borrowdate.month == a2:
                print(record)
                found = True
        if not found:
            print("No transactions found for that month/year.")

    # ══════════════════════════════════════════════════════════
    #  SEARCH
    # ══════════════════════════════════════════════════════════

    def search_by_title(self):
        b1    = input("Search Books by Title: ").lower()
        found = False
        for book in self.books:
            if b1 in book.title.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_author(self):
        b2    = input("Search Books by Author: ").lower()
        found = False
        for book in self.books:
            if b2 in book.author.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_genre(self):
        b3    = input("Search Books by Genre: ").lower()
        found = False
        for book in self.books:
            if b3 in book.genre.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_name(self):
        m1    = input("Search Members by Name: ").lower()
        found = False
        for mem in self.members:
            if m1 in mem.name.lower():
                print(mem)
                found = True
        if not found:
            print("No matching Member found")


# ─────────────────────────────────────────────────────────────
#  LIBRARY CLASS  –  menus only, calls Actions methods
# ─────────────────────────────────────────────────────────────

class Library:
    def __init__(self):
        self.action = Actions()

    def book_management(self):
        print("1. Add New Book")
        print("2. View All Books")
        print("3. Update Book Quantity")
        print("4. Remove Book")
        print("5. Back to Main Menu")
        print("-------------------------------------")
        choice = input("Enter your choice: ")

        if choice == "1":
            self.action.add_book()
        elif choice == "2":
            self.action.view_book()
        elif choice == "3":
            self.action.update_quantity()
        elif choice == "4":
            self.action.remove_book()
        elif choice == "5":
            return
        else:
            print("❌ Invalid choice!")

    def member_management(self):
        print("1. Register New Member")
        print("2. View All Members")
        print("3. View Member Details")
        print("4. Deactivate Member")
        print("5. Back to Main Menu")
        print("-------------------------------------")
        choice = input("Enter your choice: ")

        if choice == "1":
            self.action.add_member()
        elif choice == "2":
            self.action.view_member()
        elif choice == "3":
            self.action.update_member()
        elif choice == "4":
            self.action.remove_member()
        elif choice == "5":
            return
        else:
            print("❌ Invalid choice!")

    def borrow_return(self):
        print("1. Borrow Book")
        print("2. Return Book")
        print("3. View All Active Borrowings")
        print("4. Check Overdue Books")
        print("5. Back to Main Menu")
        print("-----------------------------------------")
        choice = input("Enter your choice: ")

        if choice == "1":
            self.action.borrow_book()
        elif choice == "2":
            self.action.return_book()
        elif choice == "3":
            self.action.active_borrowings()
        elif choice == "4":
            self.action.check_overdue()
        elif choice == "5":
            return
        else:
            print("❌ Invalid choice!")

    def Reports_Statistics(self):
        print("1. Library Summary")
        print("2. Most Popular Books")
        print("3. Most Active Members")
        print("4. Genre-wise Book Distribution")
        print("5. Monthly Borrowing Report")
        print("6. Back to Main Menu")
        print("------------------------------------------")
        choice = input("Enter your choice: ")

        if choice == "1":
            self.action.library_summery()
        elif choice == "2":
            self.action.popular_books()
        elif choice == "3":
            self.action.popular_member()
        elif choice == "4":
            self.action.genre_wise()
        elif choice == "5":
            self.action.monthly_borrow_report()
        elif choice == "6":
            return
        else:
            print("❌ Invalid choice!")

    def search(self):
        print("---------- SEARCH ----------")
        print("1. Search Books by Title")
        print("2. Search Books by Author")
        print("3. Search Books by Genre")
        print("4. Search Members by Name")
        print("5. Back to Main Menu")
        print("----------------------------")
        choice = input("Enter your choice: ")

        if choice == "1":
            self.action.search_by_title()
        elif choice == "2":
            self.action.search_by_author()
        elif choice == "3":
            self.action.search_by_genre()
        elif choice == "4":
            self.action.search_by_name()
        elif choice == "5":
            return
        else:
            print("❌ Invalid choice!")


# ─────────────────────────────────────────────────────────────
#  MAIN SYSTEM
# ─────────────────────────────────────────────────────────────

class LibrarySystem:
    def __init__(self):
        self.library = Library()

    def mainmenu(self):
        while True:
            print("\n==================== MAIN MENU ====================")
            print("1. Book Management")
            print("2. Member Management")
            print("3. Borrowing & Returns")
            print("4. Reports & Statistics")
            print("5. Search")
            print("6. Exit")

            choice = input("Enter choice: ")

            if choice == "1":
                self.library.book_management()
            elif choice == "2":
                self.library.member_management()
            elif choice == "3":
                self.library.borrow_return()
            elif choice == "4":
                self.library.Reports_Statistics()
            elif choice == "5":
                self.library.search()
            elif choice == "6":
                print("👋 Exiting system. All data saved!")
                break
            else:
                print("❌ Invalid choice!")


# ─────────────────────────────────────────────────────────────
#  ENTRY POINT
# ─────────────────────────────────────────────────────────────
system = LibrarySystem()
system.mainmenu()
