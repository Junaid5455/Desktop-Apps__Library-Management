import random
from datetime import date, timedelta

books = [ {"Book ID": "BK001",
"Title": "The Python Handbook",
"Author": "John Smith",
"Genre": "Technology",
"Quantity": 7,
"Available": 6,
"Borrowed" : 1 },
{"Book ID": "BK002",
"Title": "The Java Handbook",
"Author": "John Smith",
"Genre": "Technology",
"Quantity": 5,
"Available": 5,
"Borrowed" : 0 }
]

genre = [ "Fiction", "Non-Fiction", "Science", "History", "Technology"]

members = [
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
"Borrowed Books": ['BK001']
    }
]

borrow_records = [{
"Transaction ID": "TXN005",
"Member ID": "MEM002",
"Book ID": "BK001",
"Borrow Date":date(2026, 3, 1),
"Due Date": date(2026, 3, 6)
}]

Transaction_records = [{
"Transaction ID": "TXN005",
"Member ID": "MEM002",
"Book ID": "BK001",
"Borrow Date":date(2026, 3, 1),
"Due Date": date(2026, 3, 6)
}]
 
# ______________________________________________________________________
def Book_Added(x1,x2,genere,x4):
    i =1
    x5 = len(books) + 1
    x6 = f"BK00{x5}"
    print("------------------------------------------")
    print("✓ Book added successfully!")
    print(f"Book ID: = {x6}")
    print("Title:", x1)
    print("Author:",x2)
    print("Genre:",genere)
    print("Quantity:",x4)
    books.append({"Book ID": x6, "Title": x1, "Author": x2, "Genere": genere, "Quantity": x4, "Available": x4, "Borrowed":0 })
    print("------------------------------------------")
    print(books)
    print("------------------------------------------")


def member_added(y1, y2, membership):
    y3 = len(members) + 1
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
    members.append({"Member ID": y4, "Name": y1, "Phone": y2, "Type": membership, "Borrowed Books": [] })
    print(members)
    print("------------------------------------------")  

    return

def Book_Borrowed(z1,z2):
                        #z6 = datetime.datetime.now()

                        #z6 = []
                        today = date.today()
                        due_date = today + timedelta(days=7)

                        z3 = len(borrow_records)
                        z4 = f" TXN00{z3}"

                        
 
                        for mem in members:
                            if z1 == mem["Member ID"]:
                                Name = mem["Name"]
                                Type = mem["Type"]
                                
                                z5 = mem["Borrowed Books"] 
                                break

                        for i in range(len(borrow_records)):
                            if z1 == borrow_records[i]["Member ID"]:
                                if borrow_records[i]["Book ID"] not in z5:
                                    z5.append(borrow_records[i]["Book ID"])

                                                                      
                        for book in books:
                            if z2 == book["Book ID"]:
                                if int(book["Available"]) > 0:
                                    if Type == "Standard":
                                        if len(z5) < 2: 
                                            print("✓ Book added successfully!")
                                            print("----------Borrowing Receipt-------------")
                                            print("Member Name: ",Name)
                                            print("Membership Type: ",Type)
                                            print(f"Transaction ID: = {z4}")

                                            print("Registered book is: ",book["Title"])
                                            z5.append(z2)
                                            print("Borrow date: ", today)
                                            print("Due date: ", due_date)
                                            print(f"You have {len(z5)} books in your account now")
                                            print("-------------------------------------------------")
                                            book["Available"] -=1
                                            book["Borrowed"] +=1

                                            borrow_records.append({
                                                "Transaction ID": z4,
                                                "Member ID": z1,
                                                "Book ID": z2,
                                                "Borrow Date": today,
                                                "Due Date": due_date
                                                }) 

                                            Transaction_records.append({
                                                "Transaction ID": z4,
                                                "Member ID": z1,
                                                "Book ID": z2,
                                                "Borrow Date": today,
                                                "Due Date": due_date
                                                })                                   

                                        else:
                                            print("Your limit reached.No further book can be issued")    
                                            print(f"You already have {len(z5)} books in your account")
                                            print("-------------------------------------------------")
                        
                        
                                    else:
                                        if len(z5) < 4: 
                                            print("✓ Book added successfully!")
                                            print("----------Borrowing Receipt-------------")
                                            print("Member Name: ",Name)
                                            print("Membership Type: ",Type)
                                            print(f"Transaction ID: = {z4}")

                                            print("Registered book is: ",book["Title"])
                                            z5.append(z2)
                                            print("Borrow date: ", today)
                                            print("Due date: ", due_date)
                                            print(f"You have {len(z5)} books in your account now")
                                            print("-------------------------------------------------")
                                            book["Available"] -=1
                                            book["Borrowed"] +=1

                                            borrow_records.append({
                                                "Transaction ID": z4,
                                                "Member ID": z1,
                                                "Book ID": z2,
                                                "Borrow Date": today,
                                                "Due Date": due_date
                                                }) 

                                            Transaction_records.append({
                                                "Transaction ID": z4,
                                                "Member ID": z1,
                                                "Book ID": z2,
                                                "Borrow Date": today,
                                                "Due Date": due_date
                                                })                                   

                                        else:
                                            print("Your limit reached.No further book can be issued")    
                                            print(f"You already have {len(z5)} books in your account")
                                            print("-------------------------------------------------")
                        
                                else:
                                    print("Your required book is out of stock")


                                                          
                    
                        
                                return


def Return_book(z6,z7):
    
    Fine_per_day = 10
    today1 = date.today()
    print("✓ Book returned successfully!----------")
    print("--------RETURN RECEIPT ---------")
    for i in range(len(borrow_records)):
            if borrow_records[i]["Member ID"] == z6:
                DT = (today1 - borrow_records[i]["Due Date"]).days
                print(f"Transaction ID :{borrow_records[i]["Transaction ID"]}")
                print(f"Member        : {borrow_records[i]["Member ID"]}")
                print(f"Book          : {borrow_records[i]["Book ID"]}")
                print(f"Borrow Date   : {borrow_records[i]["Borrow Date"]}")
                print(f"Due Date      : {borrow_records[i]["Due Date"]}")
                print(f"Return Date   : {today1}")
                print("Days Borrowed : ", 7 )
                print("Days Taken : ", DT )
                borrow_records.pop(i)

                if DT <= 0:
                    print("Status        : Returned on time")
                    print("Fine Amount   : Rs. 0")
                else:
                    print("Status: ", DT , "days Late returning") 
                    print("Fine: Rs",(DT)*Fine_per_day )   
                
         
                break
                    

    for mem in members:
        if z6 == mem["Member ID"]:
            mem["Borrowed Books"].remove(z7)
            print(f"{z7} has been removed from record of {z6}")
            print(mem["Borrowed Books"])
            print(f"Now {z6} has {len(mem["Borrowed Books"])} books")
            print("------------------------------------------------")
            print(f"You still have{mem["Borrowed Books"]} in your aaccount")

    for book in books:
        if book["Book ID"] ==   z7:
            book["Available"] += 1
            book["Borrowed"] -= 1      
     
    return      


def Library_Summery():
    Total_Copies = []
    Standard = []
    Premium = []
    print("Books Summery is given below:")
    print(f"Total Book-Titles in Library are {len(books)} ")
    for i in range(len(books)):
        print(f"{books[i]["Title"]} has {books[i]["Quantity"]} copies")
        b = 0
        for j in range(len(borrow_records)):
            if borrow_records[j]["Book ID"] == books[i]["Book ID"]:
                b = b+1
                
        print(b)        
        print(f"Available copies of {books[i]["Title"]} are {books[i]["Quantity"] - b}")
        Total_Copies.append(books[i]["Quantity"]) 
    print(f"Total Copies in library are {sum(Total_Copies)}")  
    print(f"Currently Borrowed Copies are: {len(borrow_records)}") 

    print("---------------------------------------------")
    print(f"Total Registered Members are: {len(members)}")
    for mem in members:
        if mem["Type"] == "Standard":
            Standard.append(mem["Name"])
        else:
            Premium.append(mem["Name"])

    print(f"Total Standard Members are: {len(Standard)}")
    print(f"Total Premium Members are: {len(Premium)}")
    print("----------------------------------------------------------")
    print(f"Total Transactions made in whole history are:{len(Transaction_records)}")
    print(f"Active Borrowing are: {len(borrow_records)}")
    print(f"Completed returns are: {len(Transaction_records) - len(borrow_records)}")


    return


def Popular_Books():
    IDs = []
    for record in Transaction_records:
        IDs.append(record["Book ID"])

    if not IDs:
        print("No Book still borrowed in library")
    else:
        most_repeated = max(set(IDs), key = IDs.count)
        print(f"Most popular book is {most_repeated}")   


    return


def Active_Members():
    dict = {}
    for mem in members:
        m1 = 0
        for record in Transaction_records:
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


    return


def Genre():
    dict = {}
    print("Genre:           Count       Percentage")
    for g in genre:
        m1 = 0
        for book in books:
            if book["Genre"] == g:
                m1 = m1+1
        dict[g] = m1      # Appending pair in dictionary  
        print(f"{g}         {m1}            {int(m1/len(books))*100} ")   
    

    return


def S_T():
    b1 = input("Search Books by Title: ").lower()

    found = False
    for book in books:
        if b1 in book["Title"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")

    return

def S_A():
    b2 = input("Search Books by Author: ").lower()

    found = False
    for book in books:
        if b2 in book["Author"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")
    
    return


def S_G():
    b3 = input("Search Books by Genre: ").lower()

    found = False
    for book in books:
        if b3 in book["Genre"].lower():
            print(book)
            found = True

    if not found:
        print("No matching books found")  

    return


def S_N():
    m1 = input("Search Members by Name: ").lower()

    found = False
    for mem in members:
        if m1 in mem["Name"].lower():
            print(mem)
            found = True

    if not found:
        print("No matching Member found") 

    return


def Monthly_Report():

    Found = False
    a1 = int(input("Enter Year whose transaction record you need"))
    a2 = int(input("Enter Month whose transaction record you need"))
    
    for record in Transaction_records:
        if record["Borrow Date"].year ==a1 and record["Borrow Date"].month ==a2 :
            Found = True
            print(record)

    if Found == False:
        print("You have entered wrong Year or Month")
    return        
         

       
           
           

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
        print(books)
        if not books:
            print("Library is empty")

    if x==3:
        Found = False
        x7 = input("Enter the Book ID")
        for i in range(len(books)):
            if x7 == books[i]["Book ID"]:
                Found = True
                print("Number of ",x7,"books are ",books[i]["Quantity"])
                if books[i]["Quantity"] <= 5:
                    print("Order for more copies")
                else:
                    print(" These books are enough")
                break
        if Found == False:
            print("You have entered wrong ID")        
        
        
    if x==4:
        new_books = []
        x8 = input("Enter the Book ID to be removed from Library")
        for b in books:
            if b["Book ID"] == x8:
                save = b

        books.remove(save)
        print(f"{save} has been removed from Library")
  

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

    if y==2:
        print(members)
        if not members:
            print("No member registered yet")  

    if y==3:
        
        y4 = input("Enter the Member ID")
        print("---------------------------------------------------")
        for i in range(len(members)):
            if y4 == members[i]["Member ID"]:
                print(members[i]["Member ID"])  
                print(members[i]["Name"])
                print(members[i]["Phone"])
                print(members[i]["Type"]) 
        

    if y==4:
        y5 = input("Enter the Member ID to be removed from Library")
        for m in members:
            if m["Member ID"] == y5:
                save = m

        members.remove(save)
        print(f"{save} has been removed from Library")

    if y==5:
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
        for m in members:
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
        for i in range(len(borrow_records)):
            if borrow_records[i]["Member ID"] == z6:
                print(borrow_records[i])
        
        Return_book(z6, z7)

    if z==3:
        z8 = input("Enter your Member ID to see your Borrowed Record")  
        for i in range(len(borrow_records)):
            if z8 == borrow_records[i]["Member ID"]:
                print("Your Borrow Record is given below")
                print(borrow_records[i]) 
            else:
                print("You don't have any book in your Borrow Record")  
        return


    if z==4:
        today2 = date.today()
        z9 = input("Enter you Member ID to check Your OVERDUE Record ")              
        for BR in borrow_records:
            if z9 == BR["Member ID"]:
                print(f"{BR["Book ID"]} is found in your borrow record")
                borrowdate = BR["Borrow Date"]
                duedate = BR["Due Date"]
                
                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left =  (duedate - today2).days
                
                print(f"You have taken {total_days} days till now")

                if duedate >= today2:
                    print(f"you have {days_left} days left now ")
                else:
                    print(f"You have taken {Extra_days} extra days till now")
                    print(f"Fine till today: {(today2 - duedate).days*10}")

        return        


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

#return
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
    if x == 2:
        Member_Management()
    if x == 3:
        Borrowing_Returns()
    if x == 4:
        Reports_Statistics()
    if x == 5:
     Search()
    if x == 6:
        break
        
