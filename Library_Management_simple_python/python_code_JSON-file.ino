import json
import os
import random
from datetime import date, timedelta, datetime


BOOK_FILE = "books.json"
MEMBER_FILE = "members.json"
BORROW_RECORD_FILE = "borrow_records.json"
TRANSACTION_RECORD_FILE = "Transaction_records.json"

if not os.path.exists(BOOK_FILE):
    with open(BOOK_FILE, "w") as file:
        json.dump([ {"Book ID": "BK001",
                     "Title": "The Python Handbook",
                     "Author": "John Smith",
                     "Genre": "Technology",
                     "Quantity": 7 },
                     {"Book ID": "BK002",
                     "Title": "The Java Handbook",
                     "Author": "John Smith",
                     "Genre": "Technology",
                     "Quantity": 5 }
                     ], file, indent=4)

if not os.path.exists(MEMBER_FILE):
    with open(MEMBER_FILE, "w") as file:
        json.dump([
                 {
                  "Member ID": "MEM001",
                  "Name": "Ahmad Ali",
                  "Phone": "03001234567",
                  "Type": "Standard",
                  "Borrowed Books": []
                },
                {
                  "Member ID": "MEM002",
                  "Name": "Junaid Shabeer",
                  "Phone": "03234578123",
                  "Type": "Premium",
                  "Borrowed Books": []
                 }
                  ], file, indent=4)
        

if not os.path.exists(BORROW_RECORD_FILE):
    with open(BORROW_RECORD_FILE, "w") as file:
        json.dump([ {
                    "Transaction ID": "TXN005",
                    "Member ID": "MEM002",
                    "Book ID": "BK001",
                    "Borrow Date":str(date(2026, 3, 1)),   # json doesn't support date or datetime directly
                    "Due Date": str(date(2026, 3, 6)),
                    }
                     ], file, indent=4)        


if not os.path.exists(TRANSACTION_RECORD_FILE):
    with open(TRANSACTION_RECORD_FILE, "w") as file:
        json.dump([ {
                    "Transaction ID": "TXN005",
                    "Member ID": "MEM002",
                    "Book ID": "BK001",
                    "Borrow Date":str(date(2026, 3, 1)),
                    "Due Date": str(date(2026, 3, 6)),
                    }
                     ], file, indent=4)  


genre = [ "Fiction", "Non-Fiction", "Science", "History", "Technology"]


def load_books():
    with open(BOOK_FILE, "r") as file:
        return json.load(file)

def save_books(data):
    with open(BOOK_FILE, "w") as file:
        json.dump(data, file, indent=4)



def load_members():
    with open(MEMBER_FILE, "r") as file:
        return json.load(file)

def save_members(data):
    with open(MEMBER_FILE, "w") as file:
        json.dump(data, file, indent=4)  



def load_borrow():
    with open(BORROW_RECORD_FILE, "r") as file:
        return json.load(file)

def save_borrow(data):
    with open(BORROW_RECORD_FILE, "w") as file:
        json.dump(data, file, indent=4)



def load_transaction():
    with open(TRANSACTION_RECORD_FILE, "r") as file:
        return json.load(file)

def save_transaction(data):
    with open(TRANSACTION_RECORD_FILE, "w") as file:
        json.dump(data, file, indent=4)





# ______________________________________________________________________
def Book_Added(x1,x2,genere,x4):
    book_detail = load_books()
    i = 1
    #x5 = random.randint(1, 100)
    x5 = len(book_detail) + 1
    x6 = f"BK00{x5}"
    print("------------------------------------------")
    print("✓ Book added successfully!")
    print(f"Book ID: = {x6}")
    print("Title:", x1)
    print("Author:",x2)
    print("Genre:",genere)
    print("Quantity:",x4)
    book_detail.append({"Book ID": x6, "Title": x1, "Author": x2, "Genre": genere, "Quantity": x4 })
    print(book_detail)
    save_books(book_detail)
    print("------------------------------------------")
    new_book_detail = load_books()
    print(new_book_detail)
    print("------------------------------------------")


def member_added(y1, y2, membership):
    mem_detail = load_members()
    #y3 = random.randint(1, 100)
    y3 = len(mem_detail) + 1
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
    mem_detail.append({"Member ID": y4,"Name": y1, "Phone": y2, "Type": membership, "Borrowed Books": [] })
    save_members(mem_detail)
    print(mem_detail)
    #new_mem_detail = load_members()
    #print(new_mem_detail)
    #print("------------------------------------------")  

   

def Book_Borrowed(z1,z2):
                        #z6 = datetime.datetime.now()

                        #z6 = []
                        mem = load_members()
                        books = load_books()
                        BR = load_borrow()
                        TR = load_transaction()

                        today = date.today()
                        due_date = today + timedelta(days=7)

                        #z3 = random.randint(1, 100)
                        z3 = len(TR) + 1
                        z4 = f" TXN00{z3}"

                        
 
                        for m in mem:
                            if z1 == m["Member ID"]:
                                Name = m["Name"]
                                Type = m["Type"]
                                
                                z5 = m["Borrowed Books"] 
                                break

                                          
                        for b in books:
                            if z2 == b["Book ID"]:
                                if int(b["Quantity"]) > 0:
                                    if Type == "Standard":
                                        if len(z5) < 2: 
                                            b["Quantity"] = int(b["Quantity"]) - 1
                                            print("✓ Book added successfully!")
                                            print("----------Borrowing Receipt-------------")
                                            print("Member Name: ",Name)
                                            print("Membership Type: ",Type)
                                            print(f"Transaction ID: = {z4}")

                                            print("Registered book is: ",b["Title"])
                                            z5.append(z2)
                                            print("Borrow date: ", today)
                                            print("Due date: ", due_date)
                                            print(f"You have {len(z5)} books in your account now")
                                            print("-------------------------------------------------")
                                            save_members(mem)

                                            BR.append({
                                                        "Transaction ID": z4,
                                                        "Member ID": z1,
                                                        "Book ID": z2,
                                                        "Borrow Date": str(today),
                                                        "Due Date": str(due_date)
                                                        }) 
                                            save_borrow(BR)


                                            TR.append({
                                                        "Transaction ID": z4,
                                                        "Member ID": z1,
                                                        "Book ID": z2,
                                                        "Borrow Date": str(today),
                                                        "Due Date": str(due_date)
                                                        }) 
                                            save_transaction(TR)                                  

                                        else:
                                            print("Your limit reached.No further book can be issued")    
                                            print(f"You already have {len(z5)} books in your account")
                                            print("-------------------------------------------------")
                        

                                    if Type == "Premium":
                                        if len(z5) < 4:
                                            b["Quantity"] = int(b["Quantity"]) - 1
                                            print("✓ Book added successfully!")
                                            print("----------Borrowing Receipt-------------")
                                            print("Member Name: ",Name)
                                            print("Membership Type: ",Type)
                                            print(f"Transaction ID: = {z4}")

                                            print("Registered book is: ",b["Title"])
                                            z5.append(z2)
                                            print("Borrow date: ", today)
                                            print("Due date: ", due_date)
                                            print(f"You have {len(z5)} books in your account now")
                                            print("-------------------------------------------------")
                                            save_members(mem)

                                            BR.append({
                                                        "Transaction ID": z4,
                                                        "Member ID": z1,
                                                        "Book ID": z2,
                                                        "Borrow Date": str(today),
                                                        "Due Date": str(due_date)
                                                        }) 
                                            save_borrow(BR)


                                            TR.append({
                                                        "Transaction ID": z4,
                                                        "Member ID": z1,
                                                        "Book ID": z2,
                                                        "Borrow Date": str(today),
                                                        "Due Date": str(due_date)
                                                        }) 
                                            save_transaction(TR)                                  

                                        else:
                                            print("Your limit reached.No further book can be issued")    
                                            print(f"You already have {len(z5)} books in your account")
                                            print("-------------------------------------------------")
                        
                                else:
                                    print("Your required book is out of stock")


                        save_books(books)

                        
                        
                                


def Return_book(z6,z7):

    BR = load_borrow()
    mem = load_members()
    books = load_books()
    
    Fine_per_day = 10
    today1 = date.today()
    print("✓ Book returned successfully!----------")
    print("--------RETURN RECEIPT ---------")
    for i in range(len(BR)):
            if BR[i]["Member ID"] == z6:
                duedate = datetime.strptime(BR[i]["Due Date"], "%Y-%m-%d").date()
                DT = (today1 - duedate).days
                print(f"Transaction ID :{BR[i]["Transaction ID"]}")
                print(f"Member        : {BR[i]["Member ID"]}")
                print(f"Book          : {BR[i]["Book ID"]}")
                print(f"Borrow Date   : {BR[i]["Borrow Date"]}")
                print(f"Due Date      : {BR[i]["Due Date"]}")
                print(f"Return Date   : {today1}")
                print("Days Borrowed : ", 7 )
                print("Days Taken : ", DT )
                BR.pop(i)

                if DT <= 0:
                    print("Status        : Returned on time")
                    print("Fine Amount   : Rs. 0")
                else:
                    print("Status: ", DT , "days Late returning") 
                    print("Fine: Rs",(DT)*Fine_per_day )   

                break

    save_borrow(BR)        
                    

    for m in mem:
            if z6 == m["Member ID"]:
                m["Borrowed Books"].remove(z7)
                print(f"{z7} has been removed from record of {z6}")
                print(m["Borrowed Books"])
                print(f"Now {z6} has {len(m["Borrowed Books"])} books")
                print("------------------------------------------------")
                print(f"You still have{m["Borrowed Books"]} in your account")   

                break    

    save_members(mem)  


    for b in books:
        if b["Book ID"] == z7:
            b["Quantity"] = int(b["Quantity"]) + 1

    save_books(books)               
     
          


def Library_Summery():
    
    books = load_books()
    BR = load_borrow()
    mem = load_members()
    TR = load_transaction()

    Total_Copies = []
    Standard = []
    Premium = []
    print("Books Summery is given below:")
    print(f"Total Book-Titles in Library are {len(books)} ")
    for i in range(len(books)):
        print(f"{books[i]["Title"]} has {books[i]["Quantity"]} copies registered in library")
        b = 0
        for j in range(len(BR)):
            if BR[j]["Book ID"] == books[i]["Book ID"]:
                b = b+1
                #a1 = books[i]["Quantity"] - 1        
        print(f"Available copies of {books[i]["Title"]} are {books[i]["Quantity"] - b}")
        Total_Copies.append(books[i]["Quantity"]) 
    print(f"Total Copies in library are {sum(Total_Copies)}")  
    print(f"Currently Borrowed Copies are: {len(BR)}") 

    print("---------------------------------------------")
    print(f"Total Registered Members are: {len(mem)}")
    for m in mem:
        if m["Type"] == "Standard":
            Standard.append(m["Name"])
        else:
            Premium.append(m["Name"])

    print(f"Total Standard Members are: {len(Standard)}")
    print(f"Total Premium Members are: {len(Premium)}")
    print("----------------------------------------------------------")
    print(f"Total Transactions made in whole history are:{len(TR)}")
    print(f"Active Borrowing are: {len(BR)}")
    print(f"Completed returns are: {len(TR) - len(BR)}")



def Popular_Books():
    TR = load_transaction()
    b1 = 0
    b2 = 0
    for record in TR:
        if record["Book ID"] == "BK001":
            b1 = b1+1   
        if record["Book ID"] == "BK002":
            b2 = b2+1

    print(f"Python Hand Book has been borrowed {b1} times")
    print(f"Java Hand Book has been borrowed {b2} times")    



def Active_Members():

    members = load_members()
    TR = load_transaction()

    dict = {}
    for mem in members:
        m1 = 0
        for record in TR:
            if record["Member ID"] == mem["Member ID"]:
                m1 = m1+1
        dict[mem["Name"]] = m1      # Appending pair in dictionary  
        print(f"{mem["Name"]} has issued {m1} books")   
    print( f"{max(dict, key=dict.get)} is most active member") 
    print(dict)      
    print("Top Members are: ")
    print("Name:         Books issued")
    Sorted_Keys = sorted(dict, key=dict.get, reverse=True)   # keys sorted by their corresponding values in descending order
    for key in Sorted_Keys:
        print(f"{key}:      {dict[key]}")       # Print the keys and their values




def Genre():

    books = load_books()

    dict = {}
    print("Genre:           Count       Percentage")
    for g in genre:
        m1 = 0
        for book in books:
            if book["Genre"] == g:
                m1 = m1+1
        dict[g] = m1      # Appending pair in dictionary  
        print(f"{g}         {m1}            {int(m1/len(books))*100} ")   
    


def S_T():

    books = load_books()

    b1 = input("Search Books by Title: ").lower()

    found = False
    for book in books:
        if b1 in book["Title"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")

def S_A():

    books = load_books()

    b2 = input("Search Books by Author: ").lower()

    found = False
    for book in books:
        if b2 in book["Author"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")



def S_G():

    books = load_books()

    b3 = input("Search Books by Genre: ").lower()

    found = False
    for book in books:
        if b3 in book["Genre"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")  



def S_N():

    members = load_members()

    m1 = input("Search Members by Name: ").lower()

    found = False
    for mem in members:
        if m1 in mem["Name"].lower():
            print(mem)
            found = True

    if not found:
        print("No matching Member found")  
                      



def Monthly_Report():

    TR = load_transaction()
    
    a1 = int(input("Enter Year whose transaction record you need"))
    a2 = int(input("Enter Month whose transaction record you need"))
    
    for record in TR:
        borrowdate = datetime.strptime(record["Borrow Date"], "%Y-%m-%d").date()
        if borrowdate.year ==a1 and borrowdate.month ==a2 :
            print(record)                 

# ____________________________________________________________________


def Book_Management():
    print("---------- BOOK MANAGEMENT ----------")
    print("1. Add New Book")
    print("2. View All Books")
    print("3. Update Book Quantity")
    print("4. Remove Book")
    print("5. Back to Main Menu")
    print("-------------------------------------")
    print("")
    x = int(input(" Enter your choice"))
    if x==1:
        x1 = input(" Enter the book title")
        x2 = input(" Enter the Author name")
        print("Select genere")
        print("1. Fiction")
        print("2. Non-Fiction")
        print("3. Science")
        print("4. History")
        print("5. Technology")
        x3 = int(input("Enter genre choice (1-5)"))
        if x3==1:
            genere = "Fiction"

        if x3==2:
            genere = "Non-Fiction"

        if x3==3:
            genere = "Science"


        if x3==4:
            genere = "History"


        if x3==5:      
            genere = "Technology"

        x4 = int(input(" Enter quantity"))
              
        Book_Added(x1,x2,genere,x4)

    if x==2:
        books = load_books()
        for book in books:
            print(book)


    if x==3:
        BOOK_FOUND = False
        x7 = input("Enter the Book ID")
        books = load_books()
        for i in range(len(books)):
            if x7 == books[i]["Book ID"]:
                BOOK_FOUND = True
                print("Number of ",x7,"books are ",books[i]["Quantity"])
                if books[i]["Quantity"] <= 5:
                    print("Order for more copies")
                else:
                    print(" These books are enough")
                break

        if BOOK_FOUND == False:
            print("You have entered wrong ID")        
      

    if x == 4:
        books = load_books()
        new_books = []
        x8 = input("Enter the Book ID to be removed from Library")
        for b in books:
            if b["Book ID"] != x8:
                new_books.append(b)

        save_books(new_books)        
 

    if x==5:
        return     
             


    
def Member_Management():
    print("1. Register New Member")
    print("2. View All Members")
    print("3. View Member Details")
    print("4. Deactivate Member")
    print("5. Back to Main Menu")
    print("------------------------------------")
    y = int(input("Enter your choice"))

    if y==1:
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

        member_added(y1, y2, membership)

    elif y==2:
        mem = load_members()
        print(mem)
        if not mem:
            print("No member registered yet")  

    elif y==3:
        mem = load_members()
        y4 = input("Enter the Member ID")
        print("---------------------------------------------------")
        for i in range(len(mem)):
            if y4 == mem[i]["Member ID"]:
                print(mem[i]["Member ID"])  
                print(mem[i]["Name"])
                print(mem[i]["Phone"])
                print(mem[i]["Type"]) 
                break
        
    elif y == 4:
        mem = load_members()
        new_mem = []
        y5 = input("Enter the Member ID to be removed from Library")
        for m in mem:
            if m["Member ID"] != y5:
                new_mem.append(m)

        save_members(new_mem)
        

    elif y==5:
        return    
                  


def Borrowing_Returns():
    print("1. Borrow Book")
    print("2. Return Book")
    print("3. View All Active Borrowings")
    print("4. Check Overdue Books")
    print("5. Back to Main Menu")
    print("----------------------------------------")
    z=int(input("Enter your choice"))

    if z==1:
        m1 = False
        b1 = False
        z1 = input("Enter your Member ID")
        mem = load_members()
        books = load_books()

        for m in mem:
            if z1 == m["Member ID"]:
                m1 = True 
                break

        if m1 == True:    
            z2 = input("Enter Book ID you require")

            for b in books:
                if z2==b["Book ID"]:
                    b1 = True 
                    break
        else:
            print("Member not registered")
            return

        if b1 == True:    
            Book_Borrowed(z1,z2)
        else:
            print("This book doesn't exist")
            return
            

    if z==2:
        z6 = input("Enter Member ID")
        z7 = input("Enter Book ID")
        
        
        Return_book(z6, z7)

    if z==3:
        BR = load_borrow()
        record_found = False
        z8 = input("Enter your Member ID to see your Borrowed Record")
        print("Your Borrow Record is given below")  
        for i in range(len(BR)):
            if z8 == BR[i]["Member ID"]: 
                record_found = True     
                print(BR[i]) 

        if record_found == False:
            print("You don't have any book in your Borrow Record")  
        


    if z==4:
        BR = load_borrow()
        today2 = date.today()
        z9 = input("Enter you Member ID to check Your OVERDUE Record ")              
        for i in range(len(BR)):
            if z9 == BR[i]["Member ID"]:
                print(f"{BR[i]["Book ID"]} is found in your borrow record")
                borrowdate = datetime.strptime(BR[i]["Borrow Date"], "%Y-%m-%d").date()
                duedate = datetime.strptime(BR[i]["Due Date"], "%Y-%m-%d").date()
                
                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left =  (duedate - today2).days
                
                print(f"You have taken {total_days} days till now")

                if duedate >= today2:
                    print(f"you have {days_left} days left now ")
                else:
                    print(f"You have taken {Extra_days} extra days till now")
                    print(f"Fine till today: {(today2 - duedate).days*10}")
                #if duedate < today2:
                    #days = (today2 - duedate).days
                    #print(BR[i])
                    #print(f"You have taken {days} Extra days till now")
                    #print(f"Fine till today: {days*10}")
                #else:
                    #print("You have still some days")    

               


    if z==5:
        return        
                            

def Reports_Statistics():
    print("1. Library Summary")
    print("2. Most Popular Books")
    print("3. Most Active Members")
    print("4. Genre-wise Book Distribution")
    print("5. Monthly Borrowing Report")
    print("6. Back to Main Menu")
    print("-----------------------------------------")
    a = int(input("Enter your choice")) 
    if a == 1:
        Library_Summery() 

    if a ==2:
        Popular_Books()     

    if a == 3:
        Active_Members()    

    if a == 4:
        Genre() 

    if a == 5:
        Monthly_Report()    

    if a == 6:
        return    

def Search():
    print("1. Search Books by Title")
    print("2. Search Books by Author")
    print("3. Search Books by Genre")
    print("4. Search Members by Name")
    print("5. Back to Main Menu")
    print("---------------------------")
    b = int(input("Enter your choice"))

    if b==1:
        S_T()

    if b==2:
        S_A()

    if b==3:
        S_G()

    if b==4:
        S_N()

    if b==5:
        return                    

def Exit():
    return

while True:
    print("WELCOME TO CITY LIBRARY MANAGEMENT SYSTEM ")
    print("==================== MAIN MENU ==================== ")
    print("1. Book Management")
    print("2. Member Management")
    print("3. Borrowing & Returns")
    print("4. Reports & Statistics")
    print("5. Search")
    print("6. Exit")
    x = int(input("Enter your choice (1-6):"))
    if x == 1:
        Book_Management()
    elif x == 2:
        Member_Management()
    elif x == 3:
        Borrowing_Returns()
    elif x == 4:
        Reports_Statistics()
    elif x == 5:
     Search()
    elif x == 6:
        break
    else:
        print("Invalid choice")



