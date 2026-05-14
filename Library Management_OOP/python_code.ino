from datetime import date, timedelta, datetime

class Book:
    def __init__(self, book_id, title, author, genre ,total_quantity, available_quantity, borrow_quantity):
        self.book_id = book_id
        self.title = title
        self.author = author
        self.genre = genre
        self.quantity = total_quantity
        self.available_quantity = available_quantity
        self.borrow_quantity = borrow_quantity


    def __str__(self):
        return f"{self.book_id} | {self.title} | {self.author} | {self.genre} | {self.quantity} | {self.available_quantity} | {self.borrow_quantity}"


class Member:
    def __init__(self, member_id, name, phone, type):
        self.member_id = member_id
        self.name = name
        self.phone = phone
        self.type = type
        self.borrowed_books = []

    def __str__(self):
        return f"{self.member_id} | {self.name} | {self.phone} | {self.type} | Borrowed: {len(self.borrowed_books)}"


class Transaction:
    def __init__(self,Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id = member_id
        self.book_id = book_id
        self.Borrow_Date = Borrow_Date
        self.Due_Date = Due_Date

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | { self.Due_Date}"


class Borrow_Records:
    def __init__(self,Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id = member_id
        self.book_id = book_id
        self.Borrow_Date = Borrow_Date
        self.Due_Date = Due_Date

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | { self.Due_Date}"      



class Actions:
    def __init__(self):
        self.books = []
        self.members = []
        self.transactions = []
        self.borrow_records = []

    def add_book(self):

        borrowed_books = []
        title = input("Enter Title: ")
        author = input("Enter Author: ")
        total_quantity = int(input("Enter Quantity: "))
        available_quantity = total_quantity
        borrow_quantity = total_quantity - available_quantity

        total_books = len(self.books) 
        book_id = f"BK00{total_books + 1}"

        print("Now select Genre")
        print("1. Fiction")
        print("2. Non-Fiction")
        print("3. Science")
        print("4. History")
        print("5. Technology")
        type = input("choose the Genre type")
        if type == "1":
            genre = "Fiction"
        if type == "2":
            genre = "Non-Fiction"
        if type == "3":
            genre = "Science"
        if type == "4":
            genre = "History"    
        if type == "5":
            genre = "Technology"

        self.books.append(Book(book_id, title, author, genre ,total_quantity, available_quantity, borrow_quantity))    
        print("✅ Book added successfully!")


    def view_book(self):
        if not self.books:
            print("No books available.")
            return
        for book in self.books:
            print(book)


    def update_quantity(self):
        Found = False
        ID = input("Enter the Book ID to know its quantity")
        for book in self.books:
            if ID in book.book_id:
                Found = True
                print(book.quantity)

        if Found == False:
            print("😠You have entered wrond ID")


    def remove_book(self):
        ID = input("Enter the Book ID to remove")
        for book in self.books:
            if ID in book.book_id:
                save = book
        self.books.remove(save)
        print("Book removed from Library")


    def add_member(self):
        y1 = input("Enter Member's name")
        y2 = input("Enter his mobile number")
        print("Select membership type")
        print("1.  Standard (Max 2 books, 14 days")
        print("2.  Premium (Max 5 books, 30 days")
        y3 = int(input("Enter choice (1-2)"))
        if y3==1:
            membership = "Standard"
        if y3 ==2:    
            membership = "Premium"

        y3 = len(self.members) + 1
        y4 = f"MEM00{y3}"
        print("------------------------------------------")
        print("✓ Member registered successfully!")
        print(f"Member ID: = {y4}")
        print("Name: ", y1)
        print("Phone: ", y2)
        print("Type: ", membership)
        print("Borrowed Books", [])
        print("Status: Active")  
        print("-----------------------------------------")
        self.members.append(Member(y4, y1, y2, membership))


    def view_member(self):
        if not self.members:
            print("No members available.")
            return
        for member in self.members:
            print(member)


    def update_member(self):
        ID = input("Enter Member ID to be updated")
        for mem in self.members:
            if ID in mem.member_id:
                print(mem)


    def remove_member(self):
        ID = input("Enter Member ID to be removed")
        for mem in self.members:
            if ID in mem.member_id:
                save = mem

        self.members.remove(save)
        print("Member has been removed")


    def borrow_book(self):
        today = date.today()
        due_date = today + timedelta(days=7)
        z3 = len(self.transactions) + 1
        z4 = f" TXN00{z3}"

        m1 = False
        b1 = False
        z1 = input("Enter your Member ID")
        for m in self.members:
            if z1 == m.member_id:
                m1 = True 
                break

        if m1 == True:    
            z2 = input("Enter Book ID you require")

            for b in self.books:
                if z2==b.book_id:
                    b1 = True 
                    break
        else:
            print("Member not registered")
            return

        if b1 == False:    
            print("This book doesn't exist")
            return
        else:
            for m in self.members:
                if z1 == m.member_id:
                    Name = m.name
                    Type = m.type
                                
                    z5 = m.borrowed_books 
                    break

            for b in self.books:
                if z2 == b.book_id:
                    if int(b.available_quantity) > 0:
                        if Type == "Standard":
                            if len(z5) < 2: 
                                b.available_quantity = int(b.available_quantity) - 1
                                b.borrow_quantity = int(b.borrow_quantity) + 1
                                print("✓ Book added successfully!")
                                print("----------Borrowing Receipt-------------")
                                print("Member Name: ",Name)
                                print("Membership Type: ",Type)
                                print(f"Transaction ID: = {z4}")

                                print("Registered book is: ",b.title)
                                z5.append(z2)
                                print("Borrow date: ", today)
                                print("Due date: ", due_date)
                                print(f"You have {len(z5)} books in your account now")
                                print("-------------------------------------------------")
                                self.transactions.append(Transaction(z4, z1, z2,today,due_date ))
                                self.borrow_records.append(Transaction(z4, z1, z2,today,due_date ))
                            else:
                                print("Your limit reached.No further book can be issued")    
                                print(f"You already have {len(z5)} books in your account")
                                print("-------------------------------------------------")
        
                        if Type == "Premium":
                            if len(z5) < 4: 
                                b.available_quantity = int(b.available_quantity)- 1
                                b.borrow_quantity = int(b.borrow_quantity) + 1
                                print("✓ Book added successfully!")
                                print("----------Borrowing Receipt-------------")
                                print("Member Name: ",Name)
                                print("Membership Type: ",Type)
                                print(f"Transaction ID: = {z4}")

                                print("Registered book is: ",b.title)
                                z5.append(z2)
                                print("Borrow date: ", today)
                                print("Due date: ", due_date)
                                print(f"You have {len(z5)} books in your account now")
                                print("-------------------------------------------------")
                                self.transactions.append(Transaction(z4, z1, z2,today,due_date ))
                                self.borrow_records.append(Transaction(z4, z1, z2,today,due_date ))
                            else:
                                print("Your limit reached.No further book can be issued")    
                                print(f"You already have {len(z5)} books in your account")
                                print("-------------------------------------------------")

    def return_book(self):
        z6 = input("Enter Member ID")
        z7 = input("Enter Book ID")
        Fine_per_day = 10
        today1 = date.today()
        print("✓ Book returned successfully!----------")
        print("--------RETURN RECEIPT ---------")
        for BR in self.borrow_records:
            if BR.member_id == z6:
                #duedate = datetime.strptime(BR.Due_Date, "%Y-%m-%d").date()
                duedate = BR.Due_Date
                DT = (today1 - duedate).days
                print(f"Transaction ID :{BR.Transaction_ID}")
                print(f"Member        : {BR.member_id}")
                print(f"Book          : {BR.book_id}")
                print(f"Borrow Date   : {BR.Borrow_Date}")
                print(f"Due Date      : {BR.Due_Date}")
                print(f"Return Date   : {today1}")
                print("Days Borrowed : ", 7 )
                print("Days Taken : ", DT )
                save = BR

                if DT <= 0:
                    print("Status        : Returned on time")
                    print("Fine Amount   : Rs. 0")
                else:
                    print("Status: ", DT , "days Late returning") 
                    print("Fine: Rs",(DT)*Fine_per_day )

        self.borrow_records.remove(save)

        for m in self.members:
            if z6 == m.member_id:
                m.borrowed_books.remove(z7)
                print(f"{z7} has been removed from record of {z6}")
                print(m.borrowed_books)
                print(f"Now {z6} has {len(m.borrowed_books)} books")
                print("------------------------------------------------")
                print(f"You still have{m.borrowed_books} in your account")  

        for b in self.books:
            if b.book_id == z7:
                b.available_quantity = int(b.available_quantity) + 1 
                b.borrow_quantity = int(b.borrow_quantity) - 1


    def active_borrowings(self):
        record_found = False
        z8 = input("Enter your Member ID to see your Borrowed Record")
        print("Your Borrow Record is given below")  
        for BR in self.borrow_records:
            if z8 == BR.member_id: 
                record_found = True     
                print(BR) 

        if record_found == False:
            print("You don't have any book in your Borrow Record") 


    def check_overdue(self):
        today2 = date.today()
        z9 = input("Enter you Member ID to check Your OVERDUE Record ")              
        for BR in self.borrow_records:
            if z9 == BR.member_id:
                print(f"{BR.book_id} is found in your borrow record")
                borrowdate = BR.Borrow_Date
                duedate = BR.Due_Date
                
                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left =  (duedate - today2).days
                
                print(f"You have taken {total_days} days till now")

                if duedate >= today2:
                    print(f"you have {days_left} days left now ")
                else:
                    print(f"You have taken {Extra_days} extra days till now")
                    print(f"Fine till today: {(today2 - duedate).days*10}")


    def library_summery(self):
        Total_Copies = []
        Standard = []
        Premium = []
        print("Books Summery is given below:")
        print(f"Total Book-Titles in Library are {len(self.books)} ")
        for b in self.books:
            print(f"{b.title} has {b.quantity} copies registered in library")
            bk = 0
            for BR in self.borrow_records:
                if BR.book_id == b.book_id:
                    bk = bk + 1
                    #a1 = books[i]["Quantity"] - 1        
            print(f"Available copies of {b.title} are {b.quantity - bk}")
            Total_Copies.append(b.quantity) 
        print(f"Total Copies in library are {sum(Total_Copies)}")  
        print(f"Currently Borrowed Copies are: {len(self.borrow_records)}") 

        print("---------------------------------------------")
        print(f"Total Registered Members are: {len(self.members)}")
        for m in self.members:
            if m.type == "Standard":
                Standard.append(m.name)
            else:
                Premium.append(m.name)

        print(f"Total Standard Members are: {len(Standard)}")
        print(f"Total Premium Members are: {len(Premium)}")
        print("----------------------------------------------------------")
        print(f"Total Transactions made in whole history are:{len(self.transactions)}")
        print(f"Active Borrowing are: {len(self.borrow_records)}")
        print(f"Completed returns are: {len(self.transactions) - len(self.borrow_records)}")



    def popular_books(self):
        
        IDs = []
        for record in self.transactions:
            IDs.append(record.book_id)

        if not IDs:
            print("No Book still borrowed in library")
        else:
            most_repeated = max(set(IDs), key = IDs.count)
            print(f"Most popular book is {most_repeated}")


    def popular_member(self):

        IDs = []
        for record in self.transactions:
            IDs.append(record.member_id)

        if not IDs:
            print("No Member still borrowed any book in library")
        else:
            most_repeated = max(set(IDs), key = IDs.count)
            print(f"Most popular Member is {most_repeated}")


    def genre_wise(self):

        genre = [ "Fiction", "Non-Fiction", "Science", "History", "Technology"]
        dict = {}
        print("Genre:           Count       Percentage")
        for g in genre:
            m1 = 0
            for b in self.books:
                if b.genre == g:
                    m1 = m1+1
            dict[g] = m1      # Appending pair in dictionary  
            print(f"{g}         {m1}            {int(m1/len(self.books))*100} ")   
    
    def monthly_borrow_report(self):
        a1 = int(input("Enter Year whose transaction record you need"))
        a2 = int(input("Enter Month whose transaction record you need"))
    
        for record in self.transactions:
            borrowdate = record.Borrow_Date
            if borrowdate.year ==a1 and borrowdate.month ==a2 :
                print(record)  
  

    def search_by_title(self):
        b1 = input("Search Books by Title: ").lower()

        found = False
        for book in self.books:
            if b1 in book.title.lower():
                print(book)
                found = True

        if not found:
            print("No matching books found")


    def search_by_author(self):
        b2 = input("Search Books by Author: ").lower()

        found = False
        for book in self.books:
            if b2 in book.author.lower():
                print(book)
                found = True

        if not found:
            print("No matching books found")



    def search_by_genre(self):
        b3 = input("Search Books by Genre: ").lower()

        found = False
        for book in self.books:
            if b3 in book.genre.lower():
                print(book)
                found = True

        if not found:
            print("No matching books found")  



    def search_by_name(self):
        m1 = input("Search Members by Name: ").lower()

        found = False
        for mem in self.members:
            if m1 in mem.name.lower():
                print(mem)
                found = True

        if not found:
            print("No matching Member found") 


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
        choice = input("Enter your choice:")

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
        print("5.  Back to Main Menu")
        print("-------------------------------------")
        choice = input("Enter your choice:")

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
        choice = input("Enter your choice:")

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
        choice = input("Enter your choice:")

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
        choice = input("Enter your choice:")

        if choice == "1":
            self.action.search_by_title()

        if choice == "2":
            self.action.search_by_author()

        if choice == "3":
            self.action.search_by_genre()

        if choice == "4":
            self.action.search_by_name()




    



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


system = LibrarySystem()
system.mainmenu()   
        
