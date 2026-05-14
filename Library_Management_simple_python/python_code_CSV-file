import csv
import os
import random
from datetime import date, timedelta, datetime

file_name = "Check.csv"


# Create file if it does not exist
if not os.path.exists(file_name):

    with open(file_name, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
        writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])



genre = [ "Fiction", "Non-Fiction", "Science", "History", "Technology"]


def main_menu():
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
        Exit()
# ______________________________________________________________________
def Book_Added(x1,x2,genere,x4):
    #i =1
    i = 0
    with open(file_name,"r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "books":
                i+=1
    #x5 = random.randint(1, 100)
    x6 = f"BK00{i+1}"
    print("------------------------------------------")
    print("✓ Book added successfully!")
    print(f"Book ID: = {x6}")
    print("Title:", x1)
    print("Author:",x2)
    print("Genre:",genere)
    print("Quantity:",x4)

    with open(file_name, "a", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["books", x6, x1, x2, genere, x4])
    
    print("------------------------------------------")
    #print(books)
    print("------------------------------------------")

    return


def member_added(y1, y2, membership):
    i = 0
    with open(file_name,"r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "members":
                i+=1

    #y3 = random.randint(1, 100)
    y4 = f"MEM00{i+1}"
    print("------------------------------------------")
    print("✓ Member registered successfully!")
    print(f"Member ID: = {y4}")
    print("Name: ", y1)
    print("Phone: ", y2)
    print("Type: ", membership)
    print("Borrowed Books", [])
    print("Status: Active")  
    print("-----------------------------------------")

    with open(file_name, "a", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["members", y4, y1, y2, membership, ""])

    
    print("------------------------------------------")  

    return

def Book_Borrowed(z1,z2):
                        #z6 = datetime.datetime.now()

                        #z6 = []
                        today = date.today()
                        due_date = today + timedelta(days=7)

                        i = 0
                        with open(file_name,"r") as file:
                            reader = csv.reader(file)
                            for _ in range(4):
                                next(reader)
                            for row in reader:
                                if row[0] == "Transaction_records":
                                    i+=1

                        #z3 = random.randint(1, 100)
                        z4 = f" TXN00{i+1}"



                        count = 0
                        data = []     
                        with open(file_name,"r") as file:
                            reader = csv.reader(file)
                            for _ in range(4):
                                header = next(reader)  

                            for row in reader:
                                if row[0] == "members":
                                    if row[1] == z1:
                                        Name = row[2]
                                        Type = row[4]
                                        borrowed = row[5]
                                        if borrowed == "":
                                            count = 0
                                            row[5] = z2
        
                                        else:
                                            count = len(borrowed.split("|"))
                                            row[5] = borrowed + "|" + z2

                               
                                
                                if row[0] == "books":
                                    if row[1] == z2:
                                        x = int(row[5])
                                        Available_Quantity = x
                                        x -=1
                                        row[5] = x
                                        Title = row[2]

                                data.append(row)        
                                     

                        if Available_Quantity > 0:
                            if Type == "Standard":
                                if count <= 1:

                                    with open(file_name,"w",newline="") as file:
                                        writer = csv.writer(file)
                                        writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
                                        writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
                                        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
                                        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])

                                        writer.writerows(data)
                                        count+=1

                                    with open(file_name, "a", newline="") as file:
                                        writer = csv.writer(file)
                                        writer.writerow(["borrow_records",z4,z1,z2,today,due_date])
                                        writer.writerow(["Transaction_records",z4,z1,z2,today,due_date])                    

                                    print("✓ Book added successfully!")
                                    print("----------Borrowing Receipt-------------")
                                    print("Member Name: ",Name)
                                    print("Membership Type: ",Type)
                                    print(f"Transaction ID: = {z4}")

                                    print("Registered book is: ",Title)
                                    print("Borrow date: ", today)
                                    print("Due date: ", due_date)
                                    print(f"You have {count} books in your account now")
                                    print("-------------------------------------------------")
                            
                            

                                else:
                                    print("Your limit reached.No further book can be issued")    
                                    print(f"You already have {count} books in your account")
                                    print("-------------------------------------------------")

                            if Type == "Premium":
                                if count <= 3:

                                    with open(file_name,"w",newline="") as file:
                                        writer = csv.writer(file)
                                        writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
                                        writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
                                        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
                                        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])

                                        writer.writerows(data)
                                        count+=1

                                    with open(file_name, "a", newline="") as file:
                                        writer = csv.writer(file)
                                        writer.writerow(["borrow_records",z4,z1,z2,today,due_date])
                                        writer.writerow(["Transaction_records",z4,z1,z2,today,due_date])                    

                                    print("✓ Book added successfully!")
                                    print("----------Borrowing Receipt-------------")
                                    print("Member Name: ",Name)
                                    print("Membership Type: ",Type)
                                    print(f"Transaction ID: = {z4}")

                                    print("Registered book is: ",Title)
                                    print("Borrow date: ", today)
                                    print("Due date: ", due_date)
                                    print(f"You have {count} books in your account now")
                                    print("-------------------------------------------------")
                            
                            

                                else:
                                    print("Your limit reached.No further book can be issued")    
                                    print(f"You already have {count} books in your account")
                                    print("-------------------------------------------------")

                        else:
                            print("Your required book is out of stock")

                       
                        return


def Return_book(z6,z7):
    
    Fine_per_day = 10
    today1 = date.today()

    data = []
    data2 = []
    with open(file_name,"r") as file:
        reader = csv.reader(file)

        for _ in range(4):
            next(reader)
        
        for row in reader: 
            if row[0] == "members":
                if row[1] == z6:
                    books = row[5].split("|")
                    if z7 in books:
                        books.remove(z7)
                        row[5] = "|".join(books)   # Update row value
                    else:
                        print("You didn't borrowed this book")   

            
            if row[0] == "borrow_records":
                    if row[2] == z6:
                        if row[3] == z7:
                            continue        # Skip this row (delete it)

            if row[0] == "books":
                if row[1] == z7:
                    quant = int(row[5])
                    quant+=1
                    row[5] = quant
            
            data.append(row) 

           
            
    with open(file_name,"w",newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
        writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
        writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
        writer.writerows(data)                  

    
       
    with open(file_name,"r") as file:
        reader = csv.reader(file)

        for _ in range(4):
            next(reader)
        
        for row in reader:
            data2.append(row) 
            
        for row in data2:    
            if row[0] == "Transaction_records":
                if row[2] == z6:
                    if row[3] == z7:
                        print("✓ Book returned successfully!----------")
                        print("--------RETURN RECEIPT ---------")
                        Due_date = datetime.strptime(row[5], "%Y-%m-%d").date()
                        print("Yes code running")
                        DT = (today1 - Due_date).days
                        print(f"Transaction ID :{row[1]}")
                        print(f"Member        : {row[2]}")
                        print(f"Borrow Date   : {row[4]}")
                        print(f"Due Date      : {row[5]}")
                        print(f"Return Date   : {today1}")
                        print("Days Borrowed : ", 7 )
                        print("Days Taken : ", DT )

    

                        if DT <= 0:
                            print("Status        : Returned on time")
                            print("Fine Amount   : Rs. 0")
                        else:
                            print("Status: ", DT , "days Late returning") 
                            print("Fine: Rs",(DT)*Fine_per_day )   
                
               
                        break

                         
     
    return      


def Library_Summery():

    Total_books = []
    print("Books Summery is given below:")
    num = 0
    number = 0
    mem = 0
    prem = 0
    stan = 0
    TR = 0
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "Transaction_records":
                TR+=1
            elif row[0] == "members":
                mem+=1
                if row[4] == "Premium":
                    prem+=1
                if row[4] == "Standard":
                    stan+=1    
            elif row[0] == "books":
                num+=1
                book = row[1]
                quant = int(row[5])
                print(f"{book} has {quant} copies")
                Total_books.append(quant)
            elif row[0] == "borrow_records":
                number+=1

        total = sum(Total_books)         
        print(f"Total Book-Titles in Library are {num} ")
        print(f"Currently Books Borrowed are {number} ") 
        print(f"Available copies are {total - number}") 
        print(f"Total enrolled members in library are{mem} of which {prem} are primary members and {stan} are standard members")        
        print(f"Total transaction made till now are {TR}")
        print(f"Total returnings are {TR - number}")
    

    return


def Popular_Books():
    
    record = []
    count = {}
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "Transaction_records":
                ID = row[3]
                id = int(ID[3:])
                record.append(id)

        for n in record:
            if n in count:
                count[n] += 1
            else:
                count[n] = 1

# Print frequency of each number
    for num, freq in count.items():
        print(f"BK{num} borrowed {freq} times")
        #print(num, "appears", freq, "times")

# Find most repeated number
    most = max(count, key=count.get)
    print(f"Most repeated book is BK{most}")
   


    return


def Active_Members():
    record = []
    count = {}
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "Transaction_records":
                ID = row[2]
                id = int(ID[3:])
                record.append(id)

        for n in record:
            if n in count:
                count[n] += 1
            else:
                count[n] = 1

# Print frequency of each number
    for num, freq in count.items():
        print(f"MEM{num} borrowed {freq} times")
       

# Find most repeated number
    most = max(count, key=count.get)
    print(f"Most repeated book is MEM{most}")
   


    




    #for mem in members:
        #m1 = 0
        #for record in Transaction_records:
            #if record["Member ID"] == mem["Member ID"]:
                #m1 = m1+1
        #dict[mem["Name"]] = m1      # Appending pair in dictionary  
        #print(f"{mem["Name"]} has issued {m1} books")   
    #print( f"{max(dict, key=dict.get)} is most active member") 
    #print(dict)      
    #print("Top Members are: ")
    #print("Name:         Books issued")
    #Sorted_Keys = sorted(dict, key=dict.get, reverse=True)   # keys sorted by their corresponding values in descending order
    #for key in Sorted_Keys:
        #print(f"{key}:      {dict[key]}")       # Print the keys and their values


    return


def Genre():
    dict = {}
    print("Genre:           Count       Percentage")
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        data = list(reader)    
        for g in genre: 
            m1 = 0   
            b1 = 0
            for row in data:
                if row[0] == "books":
                    b1=b1+1
                    if row[4] == g:
                        m1=m1+1
            print(f"{g}         {m1}            {(m1/b1)*100} ")
            #dict[g] = m1            # Appending pair in dictionary  
            #print(f"{g}         {m1}            {int(m1/len(books))*100} ")      
    

    return


def S_T():
    TitleFound = False
    b1 = input("Search Books by Title")
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "books":
                if b1.lower()==row[2].lower():
                    TitleFound = True
                    print(row)

        if TitleFound == False:        
            print("You have entered wrong Title")        


    return

def S_A():
    AuthorFound = False
    b2 = input("Search Books by Author")
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "books":
                if b2.lower()==row[3].lower():
                    AuthorFound = True
                    print(row)

        if AuthorFound == False:    
            print("You have entered wrong Author ")    


    return


def S_G():
    GenreFound = False
    b3 = input("Search Books by Genre")
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "books":
                if b3.lower()==row[4].lower():
                    GenreFound = True
                    print(row)
        if GenreFound == False:
            print("Your entered genre doesn't exist")    
   

    return


def S_N():
    NameFound = False
    b4 = input("Search Member by Name")
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "members":
                if b4.lower() == row[2].lower():
                    NameFound = True
                    print(row)
        if NameFound == False:
            print("Such a Member doesn't exist")    



    return


def Monthly_Report():
    
    a1 = int(input("Enter Year whose transaction record you need"))
    a2 = int(input("Enter Month whose transaction record you need"))
    with open(file_name, "r") as file:
        reader = csv.reader(file)
        for _ in range(4):
            next(reader)
        for row in reader:
            if row[0] == "Transaction_records":
                borrow_date = datetime.strptime(row[4], "%Y-%m-%d").date()
                if borrow_date.year == a1 and borrow_date.month == a2:
                    print(row)


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
        book_found = False
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "books":
                    book_found = True
                    print(row)

            if book_found == False:
                print("Library is Empty")  

        return              

        
    if x==3:
        book_found = False
        x7 = input("Enter the Book ID")
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "books":
                    if row[1] == x7:
                        book_found = True
                        print("Number of ",x7,"books are",row[5])
                        if int(row[5]) <=5:
                            print("Order for more copies") 
                        else:
                            print("Books are enough") 
            if book_found == False:
                print("You have entered wrong ID")                      
                
        return        


    if x == 4:
        data = []
        new_data = []
        book_Found = False
        x8 = input("Enter the book id to be removed from Library")
        with open(file_name,"r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                data.append(row)
                if row[0] == "books":
                    if row[1] == x8:
                        book_Found = True

            for row in data:
                if row[0] != "books":
                    new_data.append(row)            
                if row[0] == "books":
                    if row[1] != x8:
                        new_data.append(row)

        with open(file_name, "w", newline="") as file:
            writer = csv.writer(file)
            writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
            writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
            writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
            writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
            writer.writerows(new_data)

        if book_Found == True:
            print(f"{x8} has been removed from Library") 
        else:
            print("You have enetered wrong Book ID")       

        return    

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

        member_found = False
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "members":
                    member_found = True
                    print(row)

            if member_found == False:
                print("No member is registered yet")

        return

        
    if y==3:
        
        y4 = input("Enter the Member ID")
        print("---------------------------------------------------")

        with open(file_name, "r") as file:
            mem_found = False
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "members":
                    if row[1] == y4:
                        mem_found = True
                        print(row)
                        break
            if mem_found == False:
                print("You have entered wrong ID")         

        return    


    if y == 4:
        data = []
        new_data = []
        mem_found = False
        y5 = input("Enter the member id to be removed from Library")
        with open(file_name,"r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                data.append(row)
                if row[0] == "members":
                    if row[1] == y5:
                        mem_found = True

            for row in data:
                if row[0] != "members":
                    new_data.append(row)    
                if row[0] == "members":
                    if row[1] != y5:
                        new_data.append(row)


        with open(file_name, "w", newline="") as file:
            writer = csv.writer(file)
            writer.writerow(["Category","Book ID", "Title", "Author", "Genre", "Quantity"])
            writer.writerow(["Category","Member ID", "Name", "Phone", "Type", "Borrowed Books"])
            writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
            writer.writerow(["Category","Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date"])
            writer.writerows(new_data)
        if mem_found == True:
            print(f"{y5} has been removed from Library")  
        else:
            print("You have entered wrong Member ID")      

        return    


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
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0]=="members":
                    if row[1]==z1:
                        m1 = True
                        break

            if m1==True:
                z2 = input("Enter Book ID you require")
                with open(file_name, "r") as file:
                    reader = csv.reader(file)
                    for _ in range(4):
                        next(reader)
                    for row in reader:
                        if row[0]=="books":
                            if row[1]==z2:
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
        z8 = input("Enter your Member ID to see your Borrowed Record") 
        mem_found = False
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "borrow_records":
                    if row[2] == z8:
                        mem_found = True
                        print(row)  
            if mem_found == False:
                print("You don't have any book in your Borrow Record")              


        #for i in range(len(borrow_records)):
            #if z8 == borrow_records[i]["Member ID"]:
                #print("Your Borrow Record is given below")
                #print(borrow_records[i]) 
            #else:
                #print("You don't have any book in your Borrow Record")  
        return


    if z==4:
        today2 = date.today()
        z9 = input("Enter you Member ID to check Your OVERDUE Record ")
        with open(file_name, "r") as file:
            reader = csv.reader(file)
            for _ in range(4):
                next(reader)
            for row in reader:
                if row[0] == "borrow_records":
                    if row[2] == z9:
                        print(row)
                        borrow_date = datetime.strptime(row[4], "%Y-%m-%d").date()
                        due_date = datetime.strptime(row[5], "%Y-%m-%d").date()
                       
                        total_days = (today2 - borrow_date).days
                        Extra_days = (today2 - due_date).days
                        days_left =  (due_date - today2).days
                        
                        print(f"You have taken {total_days} days till now")
                        if due_date >= today2:
                            print(f"you have {days_left} days left now ")
                        else:
                            print(f"You have taken {Extra_days} extra days till now")
                            print(f"Fine till today: {(today2 - due_date).days*10}")


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

while True:
    main_menu()
    print("____________________________________________________________")
    x = input(("Press 1 if you want to exit and Press 2 to go back to main menu ???"))
    
    if x == "1":
        break




