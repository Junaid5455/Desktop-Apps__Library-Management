import csv
import os
from datetime import date, timedelta, datetime

# ─────────────────────────────────────────────
#  CSV FILE PATHS
# ─────────────────────────────────────────────
BOOKS_FILE        = "books.csv"
MEMBERS_FILE      = "members.csv"
TRANSACTIONS_FILE = "transactions.csv"
BORROW_RECORDS_FILE = "borrow_records.csv"

BOOKS_HEADERS         = ["book_id", "title", "author", "genre", "quantity", "available_quantity", "borrow_quantity"]
MEMBERS_HEADERS       = ["member_id", "name", "phone", "type", "borrowed_books"]
TRANSACTIONS_HEADERS  = ["Transaction_ID", "member_id", "book_id", "Borrow_Date", "Due_Date"]
BORROW_RECORDS_HEADERS = ["Transaction_ID", "member_id", "book_id", "Borrow_Date", "Due_Date"]


# ─────────────────────────────────────────────
#  CSV HELPER FUNCTIONS
# ─────────────────────────────────────────────

def init_csv(filepath, headers):
    """Create CSV file with headers if it does not exist."""
    if not os.path.exists(filepath):
        with open(filepath, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=headers)
            writer.writeheader()


def read_csv(filepath, headers):
    """Read all rows from a CSV file and return list of dicts."""
    init_csv(filepath, headers)
    with open(filepath, "r", newline="") as f:
        reader = csv.DictReader(f)
        return list(reader)


def write_csv(filepath, headers, rows):
    """Overwrite CSV file with given list of dicts."""
    with open(filepath, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=headers)
        writer.writeheader()
        writer.writerows(rows)


def append_csv(filepath, headers, row_dict):
    """Append a single dict row to a CSV file."""
    init_csv(filepath, headers)
    with open(filepath, "a", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=headers)
        writer.writerow(row_dict)


# ─────────────────────────────────────────────
#  DATA CLASSES  (kept for __str__ display)
# ─────────────────────────────────────────────

class Book:
    def __init__(self, book_id, title, author, genre, total_quantity, available_quantity, borrow_quantity):
        self.book_id            = book_id
        self.title              = title
        self.author             = author
        self.genre              = genre
        self.quantity           = total_quantity
        self.available_quantity = available_quantity
        self.borrow_quantity    = borrow_quantity

    def __str__(self):
        return (f"{self.book_id} | {self.title} | {self.author} | {self.genre} | "
                f"{self.quantity} | {self.available_quantity} | {self.borrow_quantity}")

    def to_dict(self):
        return {
            "book_id": self.book_id,
            "title": self.title,
            "author": self.author,
            "genre": self.genre,
            "quantity": self.quantity,
            "available_quantity": self.available_quantity,
            "borrow_quantity": self.borrow_quantity,
        }


class Member:
    def __init__(self, member_id, name, phone, type, borrowed_books=None):
        self.member_id     = member_id
        self.name          = name
        self.phone         = phone
        self.type          = type
        # borrowed_books stored as pipe-separated string in CSV
        if borrowed_books is None:
            self.borrowed_books = []
        elif isinstance(borrowed_books, list):
            self.borrowed_books = borrowed_books
        else:
            # Parse from CSV string e.g. "BK001|BK002"
            self.borrowed_books = [b for b in borrowed_books.split("|") if b]

    def __str__(self):
        return (f"{self.member_id} | {self.name} | {self.phone} | "
                f"{self.type} | Borrowed: {len(self.borrowed_books)}")

    def to_dict(self):
        return {
            "member_id": self.member_id,
            "name": self.name,
            "phone": self.phone,
            "type": self.type,
            "borrowed_books": "|".join(self.borrowed_books),
        }


class Transaction:
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id      = member_id
        self.book_id        = book_id
        self.Borrow_Date    = Borrow_Date   # date object or string
        self.Due_Date       = Due_Date      # date object or string

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | {self.Due_Date}"

    def to_dict(self):
        return {
            "Transaction_ID": self.Transaction_ID,
            "member_id": self.member_id,
            "book_id": self.book_id,
            "Borrow_Date": str(self.Borrow_Date),
            "Due_Date": str(self.Due_Date),
        }


class Borrow_Records:
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id      = member_id
        self.book_id        = book_id
        self.Borrow_Date    = Borrow_Date
        self.Due_Date       = Due_Date

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | {self.Due_Date}"

    def to_dict(self):
        return {
            "Transaction_ID": self.Transaction_ID,
            "member_id": self.member_id,
            "book_id": self.book_id,
            "Borrow_Date": str(self.Borrow_Date),
            "Due_Date": str(self.Due_Date),
        }


# ─────────────────────────────────────────────
#  CSV LOAD HELPERS  (CSV rows → objects)
# ─────────────────────────────────────────────

def load_books():
    rows = read_csv(BOOKS_FILE, BOOKS_HEADERS)
    books = []
    for r in rows:
        books.append(Book(
            r["book_id"], r["title"], r["author"], r["genre"],
            int(r["quantity"]), int(r["available_quantity"]), int(r["borrow_quantity"])
        ))
    return books


def save_books(books):
    write_csv(BOOKS_FILE, BOOKS_HEADERS, [b.to_dict() for b in books])


def load_members():
    rows = read_csv(MEMBERS_FILE, MEMBERS_HEADERS)
    members = []
    for r in rows:
        members.append(Member(
            r["member_id"], r["name"], r["phone"], r["type"], r["borrowed_books"]
        ))
    return members


def save_members(members):
    write_csv(MEMBERS_FILE, MEMBERS_HEADERS, [m.to_dict() for m in members])


def load_transactions():
    rows = read_csv(TRANSACTIONS_FILE, TRANSACTIONS_HEADERS)
    txns = []
    for r in rows:
        txns.append(Transaction(
            r["Transaction_ID"], r["member_id"], r["book_id"],
            datetime.strptime(r["Borrow_Date"], "%Y-%m-%d").date(),
            datetime.strptime(r["Due_Date"],    "%Y-%m-%d").date(),
        ))
    return txns


def save_transactions(transactions):
    write_csv(TRANSACTIONS_FILE, TRANSACTIONS_HEADERS, [t.to_dict() for t in transactions])


def load_borrow_records():
    rows = read_csv(BORROW_RECORDS_FILE, BORROW_RECORDS_HEADERS)
    records = []
    for r in rows:
        records.append(Borrow_Records(
            r["Transaction_ID"], r["member_id"], r["book_id"],
            datetime.strptime(r["Borrow_Date"], "%Y-%m-%d").date(),
            datetime.strptime(r["Due_Date"],    "%Y-%m-%d").date(),
        ))
    return records


def save_borrow_records(borrow_records):
    write_csv(BORROW_RECORDS_FILE, BORROW_RECORDS_HEADERS, [br.to_dict() for br in borrow_records])


# ─────────────────────────────────────────────
#  ACTIONS CLASS  (all operations use CSV)
# ─────────────────────────────────────────────

class Actions:

    # ── Book Management ──────────────────────

    def add_book(self):
        title          = input("Enter Title: ")
        author         = input("Enter Author: ")
        total_quantity = int(input("Enter Quantity: "))
        available_quantity = total_quantity
        borrow_quantity    = total_quantity - available_quantity

        books   = load_books()
        book_id = f"BK00{len(books) + 1}"

        print("Now select Genre")
        print("1. Fiction")
        print("2. Non-Fiction")
        print("3. Science")
        print("4. History")
        print("5. Technology")
        type_choice = input("choose the Genre type: ")
        genre_map = {"1": "Fiction", "2": "Non-Fiction", "3": "Science",
                     "4": "History", "5": "Technology"}
        genre = genre_map.get(type_choice, "Unknown")

        new_book = Book(book_id, title, author, genre,
                        total_quantity, available_quantity, borrow_quantity)
        books.append(new_book)
        save_books(books)
        print("✅ Book added successfully!")

    def view_book(self):
        books = load_books()
        if not books:
            print("No books available.")
            return
        for book in books:
            print(book)

    def update_quantity(self):
        ID    = input("Enter the Book ID to know its quantity: ")
        books = load_books()
        found = False
        for book in books:
            if ID in book.book_id:
                print(book.quantity)
                found = True
        if not found:
            print("😠 You have entered wrong ID")

    def remove_book(self):
        ID    = input("Enter the Book ID to remove: ")
        books = load_books()
        new_books = [b for b in books if ID not in b.book_id]
        if len(new_books) == len(books):
            print("Book ID not found.")
        else:
            save_books(new_books)
            print("Book removed from Library")

    # ── Member Management ────────────────────

    def add_member(self):
        y1 = input("Enter Member's name: ")
        y2 = input("Enter his mobile number: ")
        print("Select membership type")
        print("1.  Standard (Max 2 books, 14 days)")
        print("2.  Premium  (Max 5 books, 30 days)")
        y3 = int(input("Enter choice (1-2): "))
        membership = "Standard" if y3 == 1 else "Premium"

        members   = load_members()
        member_id = f"MEM00{len(members) + 1}"

        print("------------------------------------------")
        print("✓ Member registered successfully!")
        print(f"Member ID: = {member_id}")
        print("Name: ", y1)
        print("Phone: ", y2)
        print("Type: ", membership)
        print("Borrowed Books", [])
        print("Status: Active")
        print("-----------------------------------------")

        new_member = Member(member_id, y1, y2, membership, [])
        members.append(new_member)
        save_members(members)

    def view_member(self):
        members = load_members()
        if not members:
            print("No members available.")
            return
        for member in members:
            print(member)

    def update_member(self):
        ID      = input("Enter Member ID to be updated: ")
        members = load_members()
        for mem in members:
            if ID in mem.member_id:
                print(mem)

    def remove_member(self):
        ID      = input("Enter Member ID to be removed: ")
        members = load_members()
        new_members = [m for m in members if ID not in m.member_id]
        if len(new_members) == len(members):
            print("Member ID not found.")
        else:
            save_members(new_members)
            print("Member has been removed")

    # ── Borrow & Return ──────────────────────

    def borrow_book(self):
        today    = date.today()
        due_date = today + timedelta(days=7)

        transactions = load_transactions()
        z4 = f"TXN00{len(transactions) + 1}"

        z1 = input("Enter your Member ID: ")
        members = load_members()
        m1, member_obj = False, None
        for m in members:
            if z1 == m.member_id:
                m1, member_obj = True, m
                break

        if not m1:
            print("Member not registered")
            return

        z2 = input("Enter Book ID you require: ")
        books = load_books()
        b1, book_obj = False, None
        for b in books:
            if z2 == b.book_id:
                b1, book_obj = True, b
                break

        if not b1:
            print("This book doesn't exist")
            return

        Name  = member_obj.name
        Type  = member_obj.type
        z5    = member_obj.borrowed_books
        limit = 2 if Type == "Standard" else 4

        if int(book_obj.available_quantity) > 0:
            if len(z5) < limit:
                book_obj.available_quantity = int(book_obj.available_quantity) - 1
                book_obj.borrow_quantity    = int(book_obj.borrow_quantity) + 1
                print("✓ Book added successfully!")
                print("----------Borrowing Receipt-------------")
                print("Member Name: ", Name)
                print("Membership Type: ", Type)
                print(f"Transaction ID: = {z4}")
                print("Registered book is: ", book_obj.title)
                z5.append(z2)
                print("Borrow date: ", today)
                print("Due date: ", due_date)
                print(f"You have {len(z5)} books in your account now")
                print("-------------------------------------------------")

                # Save updated books
                save_books(books)

                # Save updated member (borrowed_books changed)
                save_members(members)

                # Append new transaction
                new_txn = Transaction(z4, z1, z2, today, due_date)
                transactions.append(new_txn)
                save_transactions(transactions)

                # Append new borrow record
                borrow_records = load_borrow_records()
                new_br = Borrow_Records(z4, z1, z2, today, due_date)
                borrow_records.append(new_br)
                save_borrow_records(borrow_records)
            else:
                print("Your limit reached. No further book can be issued")
                print(f"You already have {len(z5)} books in your account")
                print("-------------------------------------------------")
        else:
            print("This book is not available right now.")

    def return_book(self):
        z6          = input("Enter Member ID: ")
        z7          = input("Enter Book ID: ")
        Fine_per_day = 10
        today1      = date.today()

        borrow_records = load_borrow_records()
        save_record    = None

        print("✓ Book returned successfully!----------")
        print("--------RETURN RECEIPT ---------")

        for BR in borrow_records:
            if BR.member_id == z6 and BR.book_id == z7:
                duedate = BR.Due_Date
                DT      = (today1 - duedate).days
                print(f"Transaction ID : {BR.Transaction_ID}")
                print(f"Member         : {BR.member_id}")
                print(f"Book           : {BR.book_id}")
                print(f"Borrow Date    : {BR.Borrow_Date}")
                print(f"Due Date       : {BR.Due_Date}")
                print(f"Return Date    : {today1}")
                print("Days Borrowed  : ", 7)
                print("Days Taken     : ", DT)
                save_record = BR
                if DT <= 0:
                    print("Status         : Returned on time")
                    print("Fine Amount    : Rs. 0")
                else:
                    print("Status: ", DT, "days Late returning")
                    print("Fine: Rs", DT * Fine_per_day)

        if save_record:
            # Remove from borrow_records CSV
            new_brs = [br for br in borrow_records
                       if not (br.member_id == z6 and br.book_id == z7)]
            save_borrow_records(new_brs)

        # Update member's borrowed_books list
        members = load_members()
        for m in members:
            if z6 == m.member_id:
                if z7 in m.borrowed_books:
                    m.borrowed_books.remove(z7)
                print(f"{z7} has been removed from record of {z6}")
                print(m.borrowed_books)
                print(f"Now {z6} has {len(m.borrowed_books)} books")
                print("------------------------------------------------")
                print(f"You still have {m.borrowed_books} in your account")
                break
        save_members(members)

        # Update book's available/borrow quantities
        books = load_books()
        for b in books:
            if b.book_id == z7:
                b.available_quantity = int(b.available_quantity) + 1
                b.borrow_quantity    = int(b.borrow_quantity) - 1
                break
        save_books(books)

    def active_borrowings(self):
        z8           = input("Enter your Member ID to see your Borrowed Record: ")
        borrow_records = load_borrow_records()
        record_found = False
        print("Your Borrow Record is given below")
        for BR in borrow_records:
            if z8 == BR.member_id:
                record_found = True
                print(BR)
        if not record_found:
            print("You don't have any book in your Borrow Record")

    def check_overdue(self):
        today2 = date.today()
        z9     = input("Enter your Member ID to check Your OVERDUE Record: ")
        borrow_records = load_borrow_records()
        for BR in borrow_records:
            if z9 == BR.member_id:
                print(f"{BR.book_id} is found in your borrow record")
                borrowdate = BR.Borrow_Date
                duedate    = BR.Due_Date
                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left  = (duedate - today2).days
                print(f"You have taken {total_days} days till now")
                if duedate >= today2:
                    print(f"You have {days_left} days left now")
                else:
                    print(f"You have taken {Extra_days} extra days till now")
                    print(f"Fine till today: {(today2 - duedate).days * 10}")

    # ── Reports & Statistics ─────────────────

    def library_summery(self):
        books          = load_books()
        members        = load_members()
        transactions   = load_transactions()
        borrow_records = load_borrow_records()

        Total_Copies = []
        Standard     = []
        Premium      = []

        print("Books Summary is given below:")
        print(f"Total Book-Titles in Library are {len(books)}")
        for b in books:
            print(f"{b.title} has {b.quantity} copies registered in library")
            bk = sum(1 for BR in borrow_records if BR.book_id == b.book_id)
            print(f"Available copies of {b.title} are {b.quantity - bk}")
            Total_Copies.append(b.quantity)
        print(f"Total Copies in library are {sum(Total_Copies)}")
        print(f"Currently Borrowed Copies are: {len(borrow_records)}")

        print("---------------------------------------------")
        print(f"Total Registered Members are: {len(members)}")
        for m in members:
            if m.type == "Standard":
                Standard.append(m.name)
            else:
                Premium.append(m.name)
        print(f"Total Standard Members are: {len(Standard)}")
        print(f"Total Premium Members are: {len(Premium)}")
        print("----------------------------------------------------------")
        print(f"Total Transactions made in whole history are: {len(transactions)}")
        print(f"Active Borrowings are: {len(borrow_records)}")
        print(f"Completed returns are: {len(transactions) - len(borrow_records)}")

    def popular_books(self):
        transactions = load_transactions()
        IDs = [record.book_id for record in transactions]
        if not IDs:
            print("No Book still borrowed in library")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            print(f"Most popular book is {most_repeated}")

    def popular_member(self):
        transactions = load_transactions()
        IDs = [record.member_id for record in transactions]
        if not IDs:
            print("No Member has borrowed any book in library")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            print(f"Most popular Member is {most_repeated}")

    def genre_wise(self):
        books = load_books()
        if not books:
            print("No books in library.")
            return
        genre_list = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        genre_dict = {}
        print("Genre:           Count       Percentage")
        for g in genre_list:
            m1 = sum(1 for b in books if b.genre == g)
            genre_dict[g] = m1
            print(f"{g}         {m1}            {int(m1 / len(books)) * 100}")

    def monthly_borrow_report(self):
        a1 = int(input("Enter Year whose transaction record you need: "))
        a2 = int(input("Enter Month whose transaction record you need: "))
        transactions = load_transactions()
        for record in transactions:
            borrowdate = record.Borrow_Date
            if borrowdate.year == a1 and borrowdate.month == a2:
                print(record)

    # ── Search ───────────────────────────────

    def search_by_title(self):
        b1    = input("Search Books by Title: ").lower()
        books = load_books()
        found = False
        for book in books:
            if b1 in book.title.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_author(self):
        b2    = input("Search Books by Author: ").lower()
        books = load_books()
        found = False
        for book in books:
            if b2 in book.author.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_genre(self):
        b3    = input("Search Books by Genre: ").lower()
        books = load_books()
        found = False
        for book in books:
            if b3 in book.genre.lower():
                print(book)
                found = True
        if not found:
            print("No matching books found")

    def search_by_name(self):
        m1      = input("Search Members by Name: ").lower()
        members = load_members()
        found   = False
        for mem in members:
            if m1 in mem.name.lower():
                print(mem)
                found = True
        if not found:
            print("No matching Member found")


# ─────────────────────────────────────────────
#  LIBRARY CLASS
# ─────────────────────────────────────────────

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
        if choice == "2":
            self.action.view_book()
        if choice == "3":
            self.action.update_quantity()
        if choice == "4":
            self.action.remove_book()
        if choice == "5":
            return

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
        if choice == "2":
            self.action.view_member()
        if choice == "3":
            self.action.update_member()
        if choice == "4":
            self.action.remove_member()
        if choice == "5":
            return

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
        if choice == "2":
            self.action.return_book()
        if choice == "3":
            self.action.active_borrowings()
        if choice == "4":
            self.action.check_overdue()
        if choice == "5":
            return

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
        if choice == "2":
            self.action.popular_books()
        if choice == "3":
            self.action.popular_member()
        if choice == "4":
            self.action.genre_wise()
        if choice == "5":
            self.action.monthly_borrow_report()
        if choice == "6":
            return

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
        if choice == "2":
            self.action.search_by_author()
        if choice == "3":
            self.action.search_by_genre()
        if choice == "4":
            self.action.search_by_name()


# ─────────────────────────────────────────────
#  LIBRARY SYSTEM  (entry point)
# ─────────────────────────────────────────────

class LibrarySystem:
    def __init__(self):
        # Ensure all CSV files exist before starting
        init_csv(BOOKS_FILE,         BOOKS_HEADERS)
        init_csv(MEMBERS_FILE,       MEMBERS_HEADERS)
        init_csv(TRANSACTIONS_FILE,  TRANSACTIONS_HEADERS)
        init_csv(BORROW_RECORDS_FILE, BORROW_RECORDS_HEADERS)
        self.library = Library()

    def mainmenu(self):
        while True:
            print("==================== MAIN MENU ====================")
            print("")
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
                print("👋 Exiting system...")
                break
            else:
                print("❌ Invalid choice!")


system = LibrarySystem()
system.mainmenu()
