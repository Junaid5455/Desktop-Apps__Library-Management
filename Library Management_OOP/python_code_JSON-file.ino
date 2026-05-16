import json
import os
from datetime import date, timedelta, datetime


# ─────────────────────────── FILE PATHS ────────────────────────────
BOOKS_FILE        = "books.json"
MEMBERS_FILE      = "members.json"
TRANSACTIONS_FILE = "transactions.json"
BORROW_RECORDS_FILE = "borrow_records.json"


# ──────────────────────── JSON HELPERS ─────────────────────────────

def load_json(filepath):
    """Load data from a JSON file. Returns empty list if file doesn't exist."""
    if os.path.exists(filepath):
        with open(filepath, "r") as f:
            return json.load(f)
    return []


def save_json(filepath, data):
    """Save data to a JSON file."""
    with open(filepath, "w") as f:
        json.dump(data, f, indent=4, default=str)


# ──────────────────────── DATA MODELS ──────────────────────────────
# Instead of class instances, every record is a plain Python dict
# that maps directly to a JSON object.
#
# Book dict structure:
#   { "book_id", "title", "author", "genre",
#     "quantity", "available_quantity", "borrow_quantity" }
#
# Member dict structure:
#   { "member_id", "name", "phone", "type", "borrowed_books": [] }
#
# Transaction / Borrow_Record dict structure:
#   { "Transaction_ID", "member_id", "book_id",
#     "Borrow_Date", "Due_Date" }


def book_str(b):
    return (f"{b['book_id']} | {b['title']} | {b['author']} | "
            f"{b['genre']} | {b['quantity']} | "
            f"{b['available_quantity']} | {b['borrow_quantity']}")


def member_str(m):
    return (f"{m['member_id']} | {m['name']} | {m['phone']} | "
            f"{m['type']} | Borrowed: {len(m['borrowed_books'])}")


def transaction_str(t):
    return (f"{t['Transaction_ID']} | {t['member_id']} | "
            f"{t['book_id']} | {t['Borrow_Date']} | {t['Due_Date']}")


# ──────────────────────── ACTIONS CLASS ────────────────────────────

class Actions:

    # ── internal helpers ──────────────────────────────────────────

    def _books(self):
        return load_json(BOOKS_FILE)

    def _members(self):
        return load_json(MEMBERS_FILE)

    def _transactions(self):
        return load_json(TRANSACTIONS_FILE)

    def _borrow_records(self):
        return load_json(BORROW_RECORDS_FILE)

    # ── book management ───────────────────────────────────────────

    def add_book(self):
        books = self._books()

        title          = input("Enter Title: ")
        author         = input("Enter Author: ")
        total_quantity = int(input("Enter Quantity: "))
        available_quantity = total_quantity
        borrow_quantity    = total_quantity - available_quantity

        book_id = f"BK00{len(books) + 1}"

        print("Now select Genre")
        print("1. Fiction")
        print("2. Non-Fiction")
        print("3. Science")
        print("4. History")
        print("5. Technology")
        choice = input("Choose the Genre type: ")
        genre_map = {"1": "Fiction", "2": "Non-Fiction",
                     "3": "Science", "4": "History", "5": "Technology"}
        genre = genre_map.get(choice, "Unknown")

        book = {
            "book_id"           : book_id,
            "title"             : title,
            "author"            : author,
            "genre"             : genre,
            "quantity"          : total_quantity,
            "available_quantity": available_quantity,
            "borrow_quantity"   : borrow_quantity
        }
        books.append(book)
        save_json(BOOKS_FILE, books)
        print("✅ Book added successfully!")

    def view_book(self):
        books = self._books()
        if not books:
            print("No books available.")
            return
        for b in books:
            print(book_str(b))

    def update_quantity(self):
        books = self._books()
        ID    = input("Enter the Book ID to know its quantity: ")
        found = False
        for b in books:
            if ID in b["book_id"]:
                found = True
                print(b["quantity"])
        if not found:
            print("😠 You have entered wrong ID")

    def remove_book(self):
        books = self._books()
        ID    = input("Enter the Book ID to remove: ")
        save  = None
        for b in books:
            if ID in b["book_id"]:
                save = b
        if save:
            books.remove(save)
            save_json(BOOKS_FILE, books)
            print("Book removed from Library")
        else:
            print("Book ID not found.")

    # ── member management ─────────────────────────────────────────

    def add_member(self):
        members = self._members()

        y1 = input("Enter Member's name: ")
        y2 = input("Enter his mobile number: ")
        print("Select membership type")
        print("1.  Standard (Max 2 books, 14 days)")
        print("2.  Premium  (Max 5 books, 30 days)")
        y3 = int(input("Enter choice (1-2): "))
        membership = "Standard" if y3 == 1 else "Premium"

        member_num = len(members) + 1
        member_id  = f"MEM00{member_num}"

        member = {
            "member_id"    : member_id,
            "name"         : y1,
            "phone"        : y2,
            "type"         : membership,
            "borrowed_books": []
        }
        members.append(member)
        save_json(MEMBERS_FILE, members)

        print("------------------------------------------")
        print("✓ Member registered successfully!")
        print(f"Member ID: = {member_id}")
        print("Name: ",  y1)
        print("Phone: ", y2)
        print("Type: ",  membership)
        print("Borrowed Books", [])
        print("Status: Active")
        print("-----------------------------------------")

    def view_member(self):
        members = self._members()
        if not members:
            print("No members available.")
            return
        for m in members:
            print(member_str(m))

    def update_member(self):
        members = self._members()
        ID = input("Enter Member ID to be updated: ")
        for m in members:
            if ID in m["member_id"]:
                print(member_str(m))

    def remove_member(self):
        members = self._members()
        ID   = input("Enter Member ID to be removed: ")
        save = None
        for m in members:
            if ID in m["member_id"]:
                save = m
        if save:
            members.remove(save)
            save_json(MEMBERS_FILE, members)
            print("Member has been removed")
        else:
            print("Member ID not found.")

    # ── borrow / return ───────────────────────────────────────────

    def borrow_book(self):
        members        = self._members()
        books          = self._books()
        transactions   = self._transactions()
        borrow_records = self._borrow_records()

        today    = date.today()
        due_date = today + timedelta(days=7)
        txn_num  = len(transactions) + 1
        txn_id   = f" TXN00{txn_num}"

        m1 = False
        b1 = False
        z1 = input("Enter your Member ID: ")

        member_obj = None
        for m in members:
            if z1 == m["member_id"]:
                m1 = True
                member_obj = m
                break

        if not m1:
            print("Member not registered")
            return

        z2 = input("Enter Book ID you require: ")
        book_obj = None
        for b in books:
            if z2 == b["book_id"]:
                b1 = True
                book_obj = b
                break

        if not b1:
            print("This book doesn't exist")
            return

        Name = member_obj["name"]
        Type = member_obj["type"]
        z5   = member_obj["borrowed_books"]

        limit = 2 if Type == "Standard" else 4

        if int(book_obj["available_quantity"]) > 0:
            if len(z5) < limit:
                book_obj["available_quantity"] = int(book_obj["available_quantity"]) - 1
                book_obj["borrow_quantity"]    = int(book_obj["borrow_quantity"])    + 1
                z5.append(z2)

                print("✓ Book added successfully!")
                print("----------Borrowing Receipt-------------")
                print("Member Name: ",      Name)
                print("Membership Type: ",  Type)
                print(f"Transaction ID: = {txn_id}")
                print("Registered book is: ", book_obj["title"])
                print("Borrow date: ", today)
                print("Due date: ",    due_date)
                print(f"You have {len(z5)} books in your account now")
                print("-------------------------------------------------")

                transaction = {
                    "Transaction_ID": txn_id,
                    "member_id"     : z1,
                    "book_id"       : z2,
                    "Borrow_Date"   : str(today),
                    "Due_Date"      : str(due_date)
                }
                transactions.append(transaction)
                borrow_records.append(transaction)

                # persist all changes
                save_json(BOOKS_FILE,          books)
                save_json(MEMBERS_FILE,        members)
                save_json(TRANSACTIONS_FILE,   transactions)
                save_json(BORROW_RECORDS_FILE, borrow_records)

            else:
                print("Your limit reached. No further book can be issued")
                print(f"You already have {len(z5)} books in your account")
                print("-------------------------------------------------")
        else:
            print("Sorry, no available copies of this book.")

    def return_book(self):
        members        = self._members()
        books          = self._books()
        borrow_records = self._borrow_records()

        z6          = input("Enter Member ID: ")
        z7          = input("Enter Book ID: ")
        Fine_per_day = 10
        today1      = date.today()

        print("✓ Book returned successfully!----------")
        print("--------RETURN RECEIPT ---------")

        save = None
        for BR in borrow_records:
            if BR["member_id"] == z6:
                duedate  = datetime.strptime(BR["Due_Date"],    "%Y-%m-%d").date()
                borrowdate = datetime.strptime(BR["Borrow_Date"], "%Y-%m-%d").date()
                DT = (today1 - duedate).days

                print(f"Transaction ID : {BR['Transaction_ID']}")
                print(f"Member         : {BR['member_id']}")
                print(f"Book           : {BR['book_id']}")
                print(f"Borrow Date    : {BR['Borrow_Date']}")
                print(f"Due Date       : {BR['Due_Date']}")
                print(f"Return Date    : {today1}")
                print("Days Borrowed  : ", 7)
                print("Days Taken     : ", DT)
                save = BR

                if DT <= 0:
                    print("Status         : Returned on time")
                    print("Fine Amount    : Rs. 0")
                else:
                    print("Status: ", DT, "days Late returning")
                    print("Fine: Rs", DT * Fine_per_day)

        if save:
            borrow_records.remove(save)
            save_json(BORROW_RECORDS_FILE, borrow_records)

        for m in members:
            if z6 == m["member_id"]:
                if z7 in m["borrowed_books"]:
                    m["borrowed_books"].remove(z7)
                print(f"{z7} has been removed from record of {z6}")
                print(m["borrowed_books"])
                print(f"Now {z6} has {len(m['borrowed_books'])} books")
                print("------------------------------------------------")
                print(f"You still have {m['borrowed_books']} in your account")
        save_json(MEMBERS_FILE, members)

        for b in books:
            if b["book_id"] == z7:
                b["available_quantity"] = int(b["available_quantity"]) + 1
                b["borrow_quantity"]    = int(b["borrow_quantity"])    - 1
        save_json(BOOKS_FILE, books)

    def active_borrowings(self):
        borrow_records = self._borrow_records()
        z8 = input("Enter your Member ID to see your Borrowed Record: ")
        print("Your Borrow Record is given below")
        record_found = False
        for BR in borrow_records:
            if z8 == BR["member_id"]:
                record_found = True
                print(transaction_str(BR))
        if not record_found:
            print("You don't have any book in your Borrow Record")

    def check_overdue(self):
        borrow_records = self._borrow_records()
        today2 = date.today()
        z9 = input("Enter your Member ID to check Your OVERDUE Record: ")
        for BR in borrow_records:
            if z9 == BR["member_id"]:
                print(f"{BR['book_id']} is found in your borrow record")
                borrowdate = datetime.strptime(BR["Borrow_Date"], "%Y-%m-%d").date()
                duedate    = datetime.strptime(BR["Due_Date"],    "%Y-%m-%d").date()

                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left  = (duedate - today2).days

                print(f"You have taken {total_days} days till now")
                if duedate >= today2:
                    print(f"You have {days_left} days left now")
                else:
                    print(f"You have taken {Extra_days} extra days till now")
                    print(f"Fine till today: {(today2 - duedate).days * 10}")

    # ── reports & statistics ──────────────────────────────────────

    def library_summery(self):
        books          = self._books()
        members        = self._members()
        transactions   = self._transactions()
        borrow_records = self._borrow_records()

        Total_Copies = []
        Standard     = []
        Premium      = []

        print("Books Summary is given below:")
        print(f"Total Book-Titles in Library are {len(books)}")

        for b in books:
            print(f"{b['title']} has {b['quantity']} copies registered in library")
            bk = sum(1 for BR in borrow_records if BR["book_id"] == b["book_id"])
            print(f"Available copies of {b['title']} are {b['quantity'] - bk}")
            Total_Copies.append(b["quantity"])

        print(f"Total Copies in library are {sum(Total_Copies)}")
        print(f"Currently Borrowed Copies are: {len(borrow_records)}")
        print("---------------------------------------------")
        print(f"Total Registered Members are: {len(members)}")

        for m in members:
            if m["type"] == "Standard":
                Standard.append(m["name"])
            else:
                Premium.append(m["name"])

        print(f"Total Standard Members are: {len(Standard)}")
        print(f"Total Premium Members are: {len(Premium)}")
        print("----------------------------------------------------------")
        print(f"Total Transactions made in whole history are: {len(transactions)}")
        print(f"Active Borrowings are: {len(borrow_records)}")
        print(f"Completed returns are: {len(transactions) - len(borrow_records)}")

    def popular_books(self):
        transactions = self._transactions()
        IDs = [record["book_id"] for record in transactions]
        if not IDs:
            print("No Book still borrowed in library")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            print(f"Most popular book is {most_repeated}")

    def popular_member(self):
        transactions = self._transactions()
        IDs = [record["member_id"] for record in transactions]
        if not IDs:
            print("No Member still borrowed any book in library")
        else:
            most_repeated = max(set(IDs), key=IDs.count)
            print(f"Most popular Member is {most_repeated}")

    def genre_wise(self):
        books = self._books()
        genres = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        genre_dict = {}
        print("Genre:           Count       Percentage")
        for g in genres:
            count = sum(1 for b in books if b["genre"] == g)
            genre_dict[g] = count
            pct = int(count / len(books) * 100) if books else 0
            print(f"{g}         {count}            {pct}")

    def monthly_borrow_report(self):
        transactions = self._transactions()
        a1 = int(input("Enter Year whose transaction record you need: "))
        a2 = int(input("Enter Month whose transaction record you need: "))
        for record in transactions:
            borrowdate = datetime.strptime(record["Borrow_Date"], "%Y-%m-%d").date()
            if borrowdate.year == a1 and borrowdate.month == a2:
                print(transaction_str(record))

    # ── search ────────────────────────────────────────────────────

    def search_by_title(self):
        books = self._books()
        b1 = input("Search Books by Title: ").lower()
        found = False
        for book in books:
            if b1 in book["title"].lower():
                print(book_str(book))
                found = True
        if not found:
            print("No matching books found")

    def search_by_author(self):
        books = self._books()
        b2 = input("Search Books by Author: ").lower()
        found = False
        for book in books:
            if b2 in book["author"].lower():
                print(book_str(book))
                found = True
        if not found:
            print("No matching books found")

    def search_by_genre(self):
        books = self._books()
        b3 = input("Search Books by Genre: ").lower()
        found = False
        for book in books:
            if b3 in book["genre"].lower():
                print(book_str(book))
                found = True
        if not found:
            print("No matching books found")

    def search_by_name(self):
        members = self._members()
        m1 = input("Search Members by Name: ").lower()
        found = False
        for mem in members:
            if m1 in mem["name"].lower():
                print(member_str(mem))
                found = True
        if not found:
            print("No matching Member found")


# ──────────────────────── LIBRARY CLASS ────────────────────────────

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


# ─────────────────────── LIBRARY SYSTEM ────────────────────────────

class LibrarySystem:
    def __init__(self):
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


# ─────────────────────────── ENTRY POINT ───────────────────────────
system = LibrarySystem()
system.mainmenu()
