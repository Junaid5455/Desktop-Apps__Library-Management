import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
from datetime import date, timedelta
import re

# ─────────────────────────── DATA MODELS ────────────────────────────

class Book:
    def __init__(self, book_id, title, author, genre, total_quantity, available_quantity, borrow_quantity):
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
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id = member_id
        self.book_id = book_id
        self.Borrow_Date = Borrow_Date
        self.Due_Date = Due_Date

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | {self.Due_Date}"


class Borrow_Records:
    def __init__(self, Transaction_ID, member_id, book_id, Borrow_Date, Due_Date):
        self.Transaction_ID = Transaction_ID
        self.member_id = member_id
        self.book_id = book_id
        self.Borrow_Date = Borrow_Date
        self.Due_Date = Due_Date

    def __str__(self):
        return f"{self.Transaction_ID} | {self.member_id} | {self.book_id} | {self.Borrow_Date} | {self.Due_Date}"


# ─────────────────────────── ACTIONS (LOGIC) ────────────────────────────

class Actions:
    def __init__(self):
        self.books = []
        self.members = []
        self.transactions = []
        self.borrow_records = []

    def add_book(self, title, author, total_quantity, genre):
        total_quantity = int(total_quantity)
        available_quantity = total_quantity
        borrow_quantity = 0
        total_books = len(self.books)
        book_id = f"BK{str(total_books + 1).zfill(3)}"
        self.books.append(Book(book_id, title, author, genre, total_quantity, available_quantity, borrow_quantity))
        return f"✅ Book added! ID: {book_id}"

    def remove_book(self, book_id):
        for book in self.books:
            if book_id == book.book_id:
                self.books.remove(book)
                return f"✅ Book '{book_id}' removed."
        return "❌ Book ID not found."

    def update_quantity(self, book_id):
        for book in self.books:
            if book_id == book.book_id:
                return f"Total: {book.quantity} | Available: {book.available_quantity} | Borrowed: {book.borrow_quantity}"
        return "❌ Book ID not found."

    def add_member(self, name, phone, membership):
        idx = len(self.members) + 1
        member_id = f"MEM{str(idx).zfill(3)}"
        self.members.append(Member(member_id, name, phone, membership))
        return member_id

    def remove_member(self, member_id):
        for mem in self.members:
            if member_id == mem.member_id:
                self.members.remove(mem)
                return f"✅ Member '{member_id}' removed."
        return "❌ Member ID not found."

    def borrow_book(self, member_id, book_id):
        today = date.today()
        due_date = today + timedelta(days=7)
        z3 = len(self.transactions) + 1
        txn_id = f"TXN{str(z3).zfill(3)}"

        member = next((m for m in self.members if m.member_id == member_id), None)
        if not member:
            return False, "❌ Member not registered."

        book = next((b for b in self.books if b.book_id == book_id), None)
        if not book:
            return False, "❌ Book not found."

        if book.available_quantity <= 0:
            return False, "❌ No copies available."

        limit = 2 if member.type == "Standard" else 5
        if len(member.borrowed_books) >= limit:
            return False, f"❌ Limit reached. You already have {len(member.borrowed_books)} books."

        book.available_quantity -= 1
        book.borrow_quantity += 1
        member.borrowed_books.append(book_id)
        self.transactions.append(Transaction(txn_id, member_id, book_id, today, due_date))
        self.borrow_records.append(Borrow_Records(txn_id, member_id, book_id, today, due_date))

        receipt = (
            f"✅ Book Borrowed Successfully!\n"
            f"{'─'*40}\n"
            f"Transaction ID : {txn_id}\n"
            f"Member Name    : {member.name}\n"
            f"Membership     : {member.type}\n"
            f"Book Title     : {book.title}\n"
            f"Borrow Date    : {today}\n"
            f"Due Date       : {due_date}\n"
            f"Books in Hand  : {len(member.borrowed_books)}\n"
            f"{'─'*40}"
        )
        return True, receipt

    def return_book(self, member_id, book_id):
        Fine_per_day = 10
        today1 = date.today()

        record = next((br for br in self.borrow_records if br.member_id == member_id and br.book_id == book_id), None)
        if not record:
            return False, "❌ No active borrow record found for this Member+Book combination."

        duedate = record.Due_Date
        DT = (today1 - duedate).days

        if DT <= 0:
            status = "Returned on time"
            fine = 0
        else:
            status = f"{DT} days late"
            fine = DT * Fine_per_day

        self.borrow_records.remove(record)

        member = next((m for m in self.members if m.member_id == member_id), None)
        if member and book_id in member.borrowed_books:
            member.borrowed_books.remove(book_id)

        book = next((b for b in self.books if b.book_id == book_id), None)
        if book:
            book.available_quantity += 1
            book.borrow_quantity -= 1

        receipt = (
            f"✅ Book Returned Successfully!\n"
            f"{'─'*40}\n"
            f"Transaction ID : {record.Transaction_ID}\n"
            f"Member         : {member_id}\n"
            f"Book           : {book_id}\n"
            f"Borrow Date    : {record.Borrow_Date}\n"
            f"Due Date       : {record.Due_Date}\n"
            f"Return Date    : {today1}\n"
            f"Days Borrowed  : {(today1 - record.Borrow_Date).days}\n"
            f"Status         : {status}\n"
            f"Fine           : Rs. {fine}\n"
            f"{'─'*40}"
        )
        return True, receipt

    def active_borrowings(self, member_id):
        records = [br for br in self.borrow_records if br.member_id == member_id]
        if not records:
            return []
        return [(br.Transaction_ID, br.member_id, br.book_id, str(br.Borrow_Date), str(br.Due_Date)) for br in records]

    def check_overdue(self, member_id):
        today2 = date.today()
        results = []
        for br in self.borrow_records:
            if br.member_id == member_id:
                duedate = br.Due_Date
                borrowdate = br.Borrow_Date
                total_days = (today2 - borrowdate).days
                Extra_days = (today2 - duedate).days
                days_left = (duedate - today2).days
                if duedate >= today2:
                    status = f"{days_left} days left"
                    fine = 0
                else:
                    status = f"{Extra_days} days overdue"
                    fine = Extra_days * 10
                results.append((br.book_id, str(br.Borrow_Date), str(br.Due_Date), total_days, status, fine))
        return results

    def library_summary(self):
        total_copies = sum(b.quantity for b in self.books)
        standard = sum(1 for m in self.members if m.type == "Standard")
        premium = sum(1 for m in self.members if m.type == "Premium")
        completed = len(self.transactions) - len(self.borrow_records)
        return {
            "titles": len(self.books),
            "total_copies": total_copies,
            "total_members": len(self.members),
            "standard": standard,
            "premium": premium,
            "total_transactions": len(self.transactions),
            "active_borrowings": len(self.borrow_records),
            "completed_returns": completed
        }

    def popular_books(self):
        IDs = [r.book_id for r in self.transactions]
        if not IDs:
            return None, None
        most = max(set(IDs), key=IDs.count)
        count = IDs.count(most)
        book = next((b for b in self.books if b.book_id == most), None)
        title = book.title if book else most
        return most, title, count

    def popular_member(self):
        IDs = [r.member_id for r in self.transactions]
        if not IDs:
            return None, None
        most = max(set(IDs), key=IDs.count)
        count = IDs.count(most)
        mem = next((m for m in self.members if m.member_id == most), None)
        name = mem.name if mem else most
        return most, name, count

    def genre_wise(self):
        genres = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        total = len(self.books) if self.books else 1
        result = []
        for g in genres:
            count = sum(1 for b in self.books if b.genre == g)
            pct = round((count / total) * 100, 1)
            result.append((g, count, pct))
        return result

    def monthly_borrow_report(self, year, month):
        results = []
        for r in self.transactions:
            if r.Borrow_Date.year == year and r.Borrow_Date.month == month:
                results.append((r.Transaction_ID, r.member_id, r.book_id, str(r.Borrow_Date), str(r.Due_Date)))
        return results

    def search_by_title(self, query):
        return [b for b in self.books if query.lower() in b.title.lower()]

    def search_by_author(self, query):
        return [b for b in self.books if query.lower() in b.author.lower()]

    def search_by_genre(self, query):
        return [b for b in self.books if query.lower() in b.genre.lower()]

    def search_by_name(self, query):
        return [m for m in self.members if query.lower() in m.name.lower()]


# ─────────────────────────── GUI APPLICATION ────────────────────────────

COLORS = {
    "bg":        "#0f1117",
    "sidebar":   "#161b27",
    "card":      "#1e2433",
    "card2":     "#252d40",
    "accent":    "#4f8ef7",
    "accent2":   "#7c5cbf",
    "green":     "#3ecf8e",
    "red":       "#f76f6f",
    "yellow":    "#f7c948",
    "text":      "#e8eaf0",
    "subtext":   "#8892a4",
    "border":    "#2a3348",
    "hover":     "#2e3a52",
    "white":     "#ffffff",
}

FONTS = {
    "title":   ("Georgia", 22, "bold"),
    "heading": ("Georgia", 14, "bold"),
    "sub":     ("Courier New", 11, "bold"),
    "body":    ("Courier New", 10),
    "small":   ("Courier New", 9),
    "btn":     ("Courier New", 10, "bold"),
    "big":     ("Georgia", 32, "bold"),
    "medium":  ("Georgia", 18, "bold"),
}


def styled_button(parent, text, command, color=None, width=18):
    c = color or COLORS["accent"]
    btn = tk.Button(
        parent, text=text, command=command,
        bg=c, fg=COLORS["white"],
        font=FONTS["btn"],
        relief="flat", bd=0,
        padx=12, pady=8,
        width=width,
        activebackground=COLORS["hover"],
        activeforeground=COLORS["white"],
        cursor="hand2"
    )
    return btn


def make_entry(parent, label, row, default=""):
    tk.Label(parent, text=label, bg=COLORS["card"], fg=COLORS["subtext"], font=FONTS["small"]).grid(
        row=row, column=0, sticky="w", padx=10, pady=4)
    var = tk.StringVar(value=default)
    e = tk.Entry(parent, textvariable=var, bg=COLORS["card2"], fg=COLORS["text"],
                 font=FONTS["body"], relief="flat", insertbackground=COLORS["text"],
                 bd=1, highlightthickness=1, highlightcolor=COLORS["accent"],
                 highlightbackground=COLORS["border"])
    e.grid(row=row, column=1, sticky="ew", padx=10, pady=4, ipady=5)
    return var


def scrolled_text(parent, height=12):
    frame = tk.Frame(parent, bg=COLORS["card2"])
    sb = tk.Scrollbar(frame, bg=COLORS["border"])
    txt = tk.Text(frame, height=height, bg=COLORS["card2"], fg=COLORS["text"],
                  font=FONTS["body"], relief="flat", wrap="word",
                  yscrollcommand=sb.set, insertbackground=COLORS["text"],
                  selectbackground=COLORS["accent"])
    sb.config(command=txt.yview)
    sb.pack(side="right", fill="y")
    txt.pack(side="left", fill="both", expand=True)
    return frame, txt


def make_treeview(parent, columns, headings, height=10):
    style = ttk.Style()
    style.theme_use("clam")
    style.configure("Custom.Treeview",
                    background=COLORS["card2"],
                    fieldbackground=COLORS["card2"],
                    foreground=COLORS["text"],
                    rowheight=28,
                    font=FONTS["body"],
                    borderwidth=0)
    style.configure("Custom.Treeview.Heading",
                    background=COLORS["card"],
                    foreground=COLORS["accent"],
                    font=FONTS["sub"],
                    relief="flat")
    style.map("Custom.Treeview", background=[("selected", COLORS["accent"])])

    frame = tk.Frame(parent, bg=COLORS["bg"])
    sb = ttk.Scrollbar(frame, orient="vertical")
    tree = ttk.Treeview(frame, columns=columns, show="headings",
                        height=height, yscrollcommand=sb.set,
                        style="Custom.Treeview")
    sb.config(command=tree.yview)
    for col, head in zip(columns, headings):
        tree.heading(col, text=head)
        tree.column(col, anchor="center", width=120)
    sb.pack(side="right", fill="y")
    tree.pack(side="left", fill="both", expand=True)
    return frame, tree


# ══════════════════════════════════════════
#              MAIN APP WINDOW
# ══════════════════════════════════════════

class LibraryApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.actions = Actions()
        self.title("📚 Library Management System")
        self.geometry("1280x800")
        self.configure(bg=COLORS["bg"])
        self.resizable(True, True)
        self._build_ui()

    def _build_ui(self):
        # ── Sidebar ──
        self.sidebar = tk.Frame(self, bg=COLORS["sidebar"], width=230)
        self.sidebar.pack(side="left", fill="y")
        self.sidebar.pack_propagate(False)

        # Logo
        logo_frame = tk.Frame(self.sidebar, bg=COLORS["sidebar"], pady=24)
        logo_frame.pack(fill="x")
        tk.Label(logo_frame, text="📚", bg=COLORS["sidebar"], font=("Arial", 32)).pack()
        tk.Label(logo_frame, text="LibraDesk", bg=COLORS["sidebar"],
                 fg=COLORS["white"], font=FONTS["heading"]).pack()
        tk.Label(logo_frame, text="Management System", bg=COLORS["sidebar"],
                 fg=COLORS["subtext"], font=FONTS["small"]).pack()

        ttk.Separator(self.sidebar, orient="horizontal").pack(fill="x", padx=16, pady=8)

        # Nav buttons
        self.current_section = tk.StringVar(value="dashboard")
        nav_items = [
            ("🏠  Dashboard",      "dashboard"),
            ("📖  Books",          "books"),
            ("👥  Members",        "members"),
            ("🔄  Borrow & Return","borrow"),
            ("📊  Reports",        "reports"),
            ("🔍  Search",         "search"),
        ]
        self.nav_btns = {}
        for label, key in nav_items:
            btn = tk.Button(
                self.sidebar, text=label, anchor="w",
                bg=COLORS["sidebar"], fg=COLORS["text"],
                font=FONTS["btn"], relief="flat", bd=0,
                padx=20, pady=10,
                activebackground=COLORS["hover"],
                activeforeground=COLORS["white"],
                cursor="hand2",
                command=lambda k=key: self.show_section(k)
            )
            btn.pack(fill="x")
            self.nav_btns[key] = btn

        # ── Main Content Area ──
        self.content = tk.Frame(self, bg=COLORS["bg"])
        self.content.pack(side="right", fill="both", expand=True)

        self.frames = {}
        for key in ["dashboard", "books", "members", "borrow", "reports", "search"]:
            f = tk.Frame(self.content, bg=COLORS["bg"])
            f.place(relx=0, rely=0, relwidth=1, relheight=1)
            self.frames[key] = f

        self._build_dashboard(self.frames["dashboard"])
        self._build_books(self.frames["books"])
        self._build_members(self.frames["members"])
        self._build_borrow(self.frames["borrow"])
        self._build_reports(self.frames["reports"])
        self._build_search(self.frames["search"])

        self.show_section("dashboard")

    def show_section(self, key):
        for k, btn in self.nav_btns.items():
            btn.config(bg=COLORS["sidebar"], fg=COLORS["text"])
        self.nav_btns[key].config(bg=COLORS["accent"], fg=COLORS["white"])
        self.frames[key].lift()
        self.current_section.set(key)
        if key == "dashboard":
            self._refresh_dashboard()

    # ──────────────────────────────────────────
    #                DASHBOARD
    # ──────────────────────────────────────────
    def _build_dashboard(self, parent):
        # Header
        header = tk.Frame(parent, bg=COLORS["bg"], pady=20)
        header.pack(fill="x", padx=30)
        tk.Label(header, text="Dashboard Overview", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        # Stats row
        self.stat_frame = tk.Frame(parent, bg=COLORS["bg"])
        self.stat_frame.pack(fill="x", padx=30, pady=10)
        self.stat_cards = {}
        stats = [
            ("Total Books",        "titles",             COLORS["accent"]),
            ("Total Members",      "total_members",      COLORS["accent2"]),
            ("Active Borrowings",  "active_borrowings",  COLORS["yellow"]),
            ("Completed Returns",  "completed_returns",  COLORS["green"]),
        ]
        for i, (label, key, color) in enumerate(stats):
            card = tk.Frame(self.stat_frame, bg=COLORS["card"], padx=20, pady=16,
                            relief="flat", bd=0)
            card.grid(row=0, column=i, padx=10, sticky="nsew")
            self.stat_frame.columnconfigure(i, weight=1)
            num_lbl = tk.Label(card, text="0", bg=COLORS["card"],
                               fg=color, font=FONTS["big"])
            num_lbl.pack()
            tk.Label(card, text=label, bg=COLORS["card"],
                     fg=COLORS["subtext"], font=FONTS["small"]).pack()
            self.stat_cards[key] = num_lbl

        # Recent books table
        mid = tk.Frame(parent, bg=COLORS["bg"])
        mid.pack(fill="both", expand=True, padx=30, pady=10)
        mid.columnconfigure(0, weight=2)
        mid.columnconfigure(1, weight=1)

        # Books table
        left = tk.Frame(mid, bg=COLORS["card"], padx=10, pady=10)
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 10))
        tk.Label(left, text="All Books", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(anchor="w", pady=(0, 6))
        cols = ("ID", "Title", "Author", "Genre", "Total", "Available")
        tf, self.dash_book_tree = make_treeview(left, cols, cols, height=10)
        tf.pack(fill="both", expand=True)

        # Members table
        right = tk.Frame(mid, bg=COLORS["card"], padx=10, pady=10)
        right.grid(row=0, column=1, sticky="nsew")
        tk.Label(right, text="All Members", bg=COLORS["card"],
                 fg=COLORS["accent2"], font=FONTS["sub"]).pack(anchor="w", pady=(0, 6))
        mcols = ("ID", "Name", "Type", "Borrowed")
        mf, self.dash_mem_tree = make_treeview(right, mcols, mcols, height=10)
        mf.pack(fill="both", expand=True)

    def _refresh_dashboard(self):
        s = self.actions.library_summary()
        for key, lbl in self.stat_cards.items():
            lbl.config(text=str(s.get(key, 0)))

        for row in self.dash_book_tree.get_children():
            self.dash_book_tree.delete(row)
        for b in self.actions.books:
            self.dash_book_tree.insert("", "end", values=(
                b.book_id, b.title[:22], b.author[:18], b.genre, b.quantity, b.available_quantity))

        for row in self.dash_mem_tree.get_children():
            self.dash_mem_tree.delete(row)
        for m in self.actions.members:
            self.dash_mem_tree.insert("", "end", values=(
                m.member_id, m.name[:18], m.type, len(m.borrowed_books)))

    # ──────────────────────────────────────────
    #              BOOK MANAGEMENT
    # ──────────────────────────────────────────
    def _build_books(self, parent):
        # Header
        hdr = tk.Frame(parent, bg=COLORS["bg"], pady=16)
        hdr.pack(fill="x", padx=30)
        tk.Label(hdr, text="📖  Book Management", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        body = tk.Frame(parent, bg=COLORS["bg"])
        body.pack(fill="both", expand=True, padx=30, pady=4)
        body.columnconfigure(0, weight=1)
        body.columnconfigure(1, weight=2)
        body.rowconfigure(0, weight=1)

        # ── Left panel: form ──
        left = tk.Frame(body, bg=COLORS["card"], padx=18, pady=18)
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 12))
        left.columnconfigure(1, weight=1)

        tk.Label(left, text="Add New Book", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["heading"]).grid(
            row=0, columnspan=2, sticky="w", pady=(0, 10))

        self.b_title = make_entry(left, "Title", 1)
        self.b_author = make_entry(left, "Author", 2)
        self.b_qty = make_entry(left, "Quantity", 3, "1")

        tk.Label(left, text="Genre", bg=COLORS["card"],
                 fg=COLORS["subtext"], font=FONTS["small"]).grid(row=4, column=0, sticky="w", padx=10, pady=4)
        self.b_genre = tk.StringVar(value="Fiction")
        genre_box = ttk.Combobox(left, textvariable=self.b_genre,
                                 values=["Fiction", "Non-Fiction", "Science", "History", "Technology"],
                                 state="readonly", font=FONTS["body"])
        genre_box.grid(row=4, column=1, sticky="ew", padx=10, pady=4, ipady=4)
        style = ttk.Style()
        style.configure("TCombobox", fieldbackground=COLORS["card2"],
                        background=COLORS["card2"], foreground=COLORS["text"])

        btn_row = tk.Frame(left, bg=COLORS["card"])
        btn_row.grid(row=5, columnspan=2, pady=14)
        styled_button(btn_row, "➕  Add Book", self._add_book, COLORS["green"], 16).pack(side="left", padx=4)

        ttk.Separator(left, orient="horizontal").grid(row=6, columnspan=2, sticky="ew", pady=10)

        tk.Label(left, text="Manage Book", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["heading"]).grid(row=7, columnspan=2, sticky="w")

        self.b_manage_id = make_entry(left, "Book ID", 8)

        btn_row2 = tk.Frame(left, bg=COLORS["card"])
        btn_row2.grid(row=9, columnspan=2, pady=8)
        styled_button(btn_row2, "❌  Remove Book", self._remove_book, COLORS["red"], 16).pack(side="left", padx=4)
        styled_button(btn_row2, "📦  Check Qty", self._check_qty, COLORS["yellow"], 14).pack(side="left", padx=4)

        # ── Right panel: treeview ──
        right = tk.Frame(body, bg=COLORS["card"], padx=12, pady=12)
        right.grid(row=0, column=1, sticky="nsew")

        top = tk.Frame(right, bg=COLORS["card"])
        top.pack(fill="x", pady=(0, 8))
        tk.Label(top, text="All Books", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(side="left")
        styled_button(top, "🔄 Refresh", self._refresh_books, COLORS["accent"], 10).pack(side="right")

        cols = ("ID", "Title", "Author", "Genre", "Total", "Available", "Borrowed")
        tf, self.book_tree = make_treeview(right, cols, cols, height=16)
        tf.pack(fill="both", expand=True)

    def _add_book(self):
        t = self.b_title.get().strip()
        a = self.b_author.get().strip()
        q = self.b_qty.get().strip()
        g = self.b_genre.get()
        if not t or not a or not q:
            messagebox.showerror("Error", "Please fill all fields.")
            return
        try:
            int(q)
        except ValueError:
            messagebox.showerror("Error", "Quantity must be a number.")
            return
        msg = self.actions.add_book(t, a, q, g)
        messagebox.showinfo("Success", msg)
        self.b_title.set(""); self.b_author.set(""); self.b_qty.set("1")
        self._refresh_books()

    def _remove_book(self):
        bid = self.b_manage_id.get().strip()
        if not bid:
            messagebox.showerror("Error", "Enter a Book ID.")
            return
        if messagebox.askyesno("Confirm", f"Remove book '{bid}'?"):
            msg = self.actions.remove_book(bid)
            messagebox.showinfo("Result", msg)
            self._refresh_books()

    def _check_qty(self):
        bid = self.b_manage_id.get().strip()
        if not bid:
            messagebox.showerror("Error", "Enter a Book ID.")
            return
        msg = self.actions.update_quantity(bid)
        messagebox.showinfo("Book Quantity", msg)

    def _refresh_books(self):
        for row in self.book_tree.get_children():
            self.book_tree.delete(row)
        for b in self.actions.books:
            self.book_tree.insert("", "end", values=(
                b.book_id, b.title, b.author, b.genre,
                b.quantity, b.available_quantity, b.borrow_quantity))

    # ──────────────────────────────────────────
    #             MEMBER MANAGEMENT
    # ──────────────────────────────────────────
    def _build_members(self, parent):
        hdr = tk.Frame(parent, bg=COLORS["bg"], pady=16)
        hdr.pack(fill="x", padx=30)
        tk.Label(hdr, text="👥  Member Management", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        body = tk.Frame(parent, bg=COLORS["bg"])
        body.pack(fill="both", expand=True, padx=30, pady=4)
        body.columnconfigure(0, weight=1)
        body.columnconfigure(1, weight=2)
        body.rowconfigure(0, weight=1)

        # ── Left: Form ──
        left = tk.Frame(body, bg=COLORS["card"], padx=18, pady=18)
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 12))
        left.columnconfigure(1, weight=1)

        tk.Label(left, text="Register New Member", bg=COLORS["card"],
                 fg=COLORS["accent2"], font=FONTS["heading"]).grid(
            row=0, columnspan=2, sticky="w", pady=(0, 10))

        self.m_name = make_entry(left, "Full Name", 1)
        self.m_phone = make_entry(left, "Phone", 2)

        tk.Label(left, text="Membership", bg=COLORS["card"],
                 fg=COLORS["subtext"], font=FONTS["small"]).grid(row=3, column=0, sticky="w", padx=10, pady=4)
        self.m_type = tk.StringVar(value="Standard")
        mem_box = ttk.Combobox(left, textvariable=self.m_type,
                               values=["Standard", "Premium"],
                               state="readonly", font=FONTS["body"])
        mem_box.grid(row=3, column=1, sticky="ew", padx=10, pady=4, ipady=4)

        tk.Label(left, text="Standard: max 2 books, 7 days\nPremium: max 5 books, 7 days",
                 bg=COLORS["card"], fg=COLORS["subtext"], font=FONTS["small"],
                 justify="left").grid(row=4, columnspan=2, padx=10, sticky="w")

        styled_button(left, "➕  Register Member", self._add_member, COLORS["accent2"], 18).grid(
            row=5, columnspan=2, pady=14)

        ttk.Separator(left, orient="horizontal").grid(row=6, columnspan=2, sticky="ew", pady=10)

        tk.Label(left, text="Manage Member", bg=COLORS["card"],
                 fg=COLORS["accent2"], font=FONTS["heading"]).grid(row=7, columnspan=2, sticky="w")

        self.m_manage_id = make_entry(left, "Member ID", 8)

        btn_row = tk.Frame(left, bg=COLORS["card"])
        btn_row.grid(row=9, columnspan=2, pady=8)
        styled_button(btn_row, "❌  Remove Member", self._remove_member, COLORS["red"], 16).pack(side="left", padx=4)
        styled_button(btn_row, "🔍  View Details", self._view_member_details, COLORS["accent"], 14).pack(side="left", padx=4)

        # ── Right: Table ──
        right = tk.Frame(body, bg=COLORS["card"], padx=12, pady=12)
        right.grid(row=0, column=1, sticky="nsew")

        top = tk.Frame(right, bg=COLORS["card"])
        top.pack(fill="x", pady=(0, 8))
        tk.Label(top, text="All Members", bg=COLORS["card"],
                 fg=COLORS["accent2"], font=FONTS["sub"]).pack(side="left")
        styled_button(top, "🔄 Refresh", self._refresh_members, COLORS["accent2"], 10).pack(side="right")

        cols = ("ID", "Name", "Phone", "Type", "Borrowed Count")
        tf, self.mem_tree = make_treeview(right, cols, cols, height=16)
        tf.pack(fill="both", expand=True)

    def _add_member(self):
        n = self.m_name.get().strip()
        p = self.m_phone.get().strip()
        t = self.m_type.get()
        if not n or not p:
            messagebox.showerror("Error", "Please fill all fields.")
            return
        mid = self.actions.add_member(n, p, t)
        messagebox.showinfo("Success",
            f"✅ Member registered!\n\nMember ID: {mid}\nName: {n}\nPhone: {p}\nType: {t}")
        self.m_name.set(""); self.m_phone.set("")
        self._refresh_members()

    def _remove_member(self):
        mid = self.m_manage_id.get().strip()
        if not mid:
            messagebox.showerror("Error", "Enter a Member ID.")
            return
        if messagebox.askyesno("Confirm", f"Remove member '{mid}'?"):
            msg = self.actions.remove_member(mid)
            messagebox.showinfo("Result", msg)
            self._refresh_members()

    def _view_member_details(self):
        mid = self.m_manage_id.get().strip()
        if not mid:
            messagebox.showerror("Error", "Enter a Member ID.")
            return
        mem = next((m for m in self.actions.members if m.member_id == mid), None)
        if not mem:
            messagebox.showerror("Error", "Member not found.")
            return
        info = (f"Member ID   : {mem.member_id}\n"
                f"Name        : {mem.name}\n"
                f"Phone       : {mem.phone}\n"
                f"Type        : {mem.type}\n"
                f"Books Held  : {len(mem.borrowed_books)}\n"
                f"Book IDs    : {', '.join(mem.borrowed_books) if mem.borrowed_books else 'None'}")
        messagebox.showinfo("Member Details", info)

    def _refresh_members(self):
        for row in self.mem_tree.get_children():
            self.mem_tree.delete(row)
        for m in self.actions.members:
            self.mem_tree.insert("", "end", values=(
                m.member_id, m.name, m.phone, m.type, len(m.borrowed_books)))

    # ──────────────────────────────────────────
    #           BORROW & RETURN
    # ──────────────────────────────────────────
    def _build_borrow(self, parent):
        hdr = tk.Frame(parent, bg=COLORS["bg"], pady=16)
        hdr.pack(fill="x", padx=30)
        tk.Label(hdr, text="🔄  Borrow & Return", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        body = tk.Frame(parent, bg=COLORS["bg"])
        body.pack(fill="both", expand=True, padx=30, pady=4)
        body.columnconfigure(0, weight=1)
        body.columnconfigure(1, weight=1)
        body.rowconfigure(0, weight=1)

        # ── LEFT: Borrow + Return forms ──
        left = tk.Frame(body, bg=COLORS["bg"])
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 10))

        # Borrow Card
        bc = tk.Frame(left, bg=COLORS["card"], padx=18, pady=16)
        bc.pack(fill="x", pady=(0, 10))
        bc.columnconfigure(1, weight=1)

        tk.Label(bc, text="📗  Borrow a Book", bg=COLORS["card"],
                 fg=COLORS["green"], font=FONTS["heading"]).grid(row=0, columnspan=2, sticky="w", pady=(0, 8))
        self.br_mem_id = make_entry(bc, "Member ID", 1)
        self.br_book_id = make_entry(bc, "Book ID", 2)
        styled_button(bc, "📗  Borrow Book", self._borrow_book, COLORS["green"], 18).grid(
            row=3, columnspan=2, pady=10)

        # Return Card
        rc = tk.Frame(left, bg=COLORS["card"], padx=18, pady=16)
        rc.pack(fill="x", pady=(0, 10))
        rc.columnconfigure(1, weight=1)

        tk.Label(rc, text="📕  Return a Book", bg=COLORS["card"],
                 fg=COLORS["red"], font=FONTS["heading"]).grid(row=0, columnspan=2, sticky="w", pady=(0, 8))
        self.ret_mem_id = make_entry(rc, "Member ID", 1)
        self.ret_book_id = make_entry(rc, "Book ID", 2)
        styled_button(rc, "📕  Return Book", self._return_book, COLORS["red"], 18).grid(
            row=3, columnspan=2, pady=10)

        # Active Borrowings / Overdue
        qc = tk.Frame(left, bg=COLORS["card"], padx=18, pady=16)
        qc.pack(fill="x")
        qc.columnconfigure(1, weight=1)
        tk.Label(qc, text="🔎  Quick Check", bg=COLORS["card"],
                 fg=COLORS["yellow"], font=FONTS["heading"]).grid(row=0, columnspan=2, sticky="w", pady=(0, 8))
        self.chk_mem_id = make_entry(qc, "Member ID", 1)
        btn_row = tk.Frame(qc, bg=COLORS["card"])
        btn_row.grid(row=2, columnspan=2, pady=6)
        styled_button(btn_row, "📋  Active Borrows", self._active_borrowings, COLORS["yellow"], 16).pack(side="left", padx=4)
        styled_button(btn_row, "⚠️  Overdue Check", self._check_overdue, COLORS["accent2"], 16).pack(side="left", padx=4)

        # ── RIGHT: Results ──
        right = tk.Frame(body, bg=COLORS["card"], padx=12, pady=12)
        right.grid(row=0, column=1, sticky="nsew")

        tk.Label(right, text="Results", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(anchor="w", pady=(0, 6))

        tf, self.borrow_tree = make_treeview(right,
            ("TxnID", "MemberID", "BookID", "BorrowDate", "DueDate"),
            ("Txn ID", "Member ID", "Book ID", "Borrow Date", "Due Date"),
            height=8)
        tf.pack(fill="x")

        ttk.Separator(right, orient="horizontal").pack(fill="x", pady=8)

        tk.Label(right, text="Receipt / Info", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(anchor="w")
        rf, self.borrow_result_txt = scrolled_text(right, height=12)
        rf.pack(fill="both", expand=True, pady=6)
        self.borrow_result_txt.config(state="disabled")

    def _borrow_book(self):
        mid = self.br_mem_id.get().strip()
        bid = self.br_book_id.get().strip()
        if not mid or not bid:
            messagebox.showerror("Error", "Please enter both Member ID and Book ID.")
            return
        ok, msg = self.actions.borrow_book(mid, bid)
        self._set_result_text(msg)
        if ok:
            self.br_mem_id.set(""); self.br_book_id.set("")
            self._refresh_dashboard()

    def _return_book(self):
        mid = self.ret_mem_id.get().strip()
        bid = self.ret_book_id.get().strip()
        if not mid or not bid:
            messagebox.showerror("Error", "Please enter both Member ID and Book ID.")
            return
        ok, msg = self.actions.return_book(mid, bid)
        self._set_result_text(msg)
        if ok:
            self.ret_mem_id.set(""); self.ret_book_id.set("")
            self._refresh_dashboard()

    def _active_borrowings(self):
        mid = self.chk_mem_id.get().strip()
        if not mid:
            messagebox.showerror("Error", "Enter a Member ID.")
            return
        records = self.actions.active_borrowings(mid)
        for row in self.borrow_tree.get_children():
            self.borrow_tree.delete(row)
        if not records:
            self._set_result_text(f"No active borrowings for member: {mid}")
        else:
            for r in records:
                self.borrow_tree.insert("", "end", values=r)
            self._set_result_text(f"Found {len(records)} active borrowing(s) for {mid}.")

    def _check_overdue(self):
        mid = self.chk_mem_id.get().strip()
        if not mid:
            messagebox.showerror("Error", "Enter a Member ID.")
            return
        results = self.actions.check_overdue(mid)
        if not results:
            self._set_result_text(f"No overdue records for member: {mid}")
            return
        lines = [f"Overdue Report for {mid}\n{'─'*40}"]
        for (book_id, borrow_date, due_date, total_days, status, fine) in results:
            lines.append(
                f"Book       : {book_id}\n"
                f"Borrow Date: {borrow_date}\n"
                f"Due Date   : {due_date}\n"
                f"Days Taken : {total_days}\n"
                f"Status     : {status}\n"
                f"Fine       : Rs. {fine}\n{'─'*40}"
            )
        self._set_result_text("\n".join(lines))

    def _set_result_text(self, msg):
        self.borrow_result_txt.config(state="normal")
        self.borrow_result_txt.delete("1.0", "end")
        self.borrow_result_txt.insert("end", msg)
        self.borrow_result_txt.config(state="disabled")

    # ──────────────────────────────────────────
    #           REPORTS & STATISTICS
    # ──────────────────────────────────────────
    def _build_reports(self, parent):
        hdr = tk.Frame(parent, bg=COLORS["bg"], pady=16)
        hdr.pack(fill="x", padx=30)
        tk.Label(hdr, text="📊  Reports & Statistics", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        body = tk.Frame(parent, bg=COLORS["bg"])
        body.pack(fill="both", expand=True, padx=30, pady=4)
        body.columnconfigure(0, weight=1)
        body.columnconfigure(1, weight=2)
        body.rowconfigure(0, weight=1)

        # ── Left: Buttons ──
        left = tk.Frame(body, bg=COLORS["card"], padx=18, pady=18)
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 12))

        tk.Label(left, text="Generate Report", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["heading"]).pack(anchor="w", pady=(0, 14))

        btns = [
            ("📋  Library Summary",        self._rpt_summary,    COLORS["accent"]),
            ("⭐  Most Popular Book",       self._rpt_popular_book, COLORS["yellow"]),
            ("🏆  Most Active Member",      self._rpt_popular_mem, COLORS["accent2"]),
            ("📚  Genre Distribution",      self._rpt_genre,      COLORS["green"]),
        ]
        for txt, cmd, col in btns:
            styled_button(left, txt, cmd, col, 22).pack(pady=5, fill="x")

        ttk.Separator(left, orient="horizontal").pack(fill="x", pady=12)

        tk.Label(left, text="Monthly Borrow Report", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["heading"]).pack(anchor="w")

        f = tk.Frame(left, bg=COLORS["card"])
        f.pack(fill="x", pady=6)
        f.columnconfigure(1, weight=1)
        tk.Label(f, text="Year", bg=COLORS["card"], fg=COLORS["subtext"], font=FONTS["small"]).grid(row=0, column=0, sticky="w", padx=6)
        self.rpt_year = tk.StringVar(value=str(date.today().year))
        tk.Entry(f, textvariable=self.rpt_year, bg=COLORS["card2"], fg=COLORS["text"],
                 font=FONTS["body"], relief="flat", width=8, insertbackground=COLORS["text"]).grid(
            row=0, column=1, sticky="ew", padx=6, ipady=4)

        tk.Label(f, text="Month (1-12)", bg=COLORS["card"], fg=COLORS["subtext"], font=FONTS["small"]).grid(row=1, column=0, sticky="w", padx=6, pady=4)
        self.rpt_month = tk.StringVar(value=str(date.today().month))
        tk.Entry(f, textvariable=self.rpt_month, bg=COLORS["card2"], fg=COLORS["text"],
                 font=FONTS["body"], relief="flat", width=8, insertbackground=COLORS["text"]).grid(
            row=1, column=1, sticky="ew", padx=6, ipady=4)

        styled_button(left, "📅  Generate Monthly", self._rpt_monthly, COLORS["accent"], 22).pack(pady=8, fill="x")

        # ── Right: Output ──
        right = tk.Frame(body, bg=COLORS["card"], padx=14, pady=14)
        right.grid(row=0, column=1, sticky="nsew")

        tk.Label(right, text="Report Output", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(anchor="w", pady=(0, 8))
        rf, self.rpt_text = scrolled_text(right, height=20)
        rf.pack(fill="both", expand=True)
        self.rpt_text.config(state="disabled")

    def _rpt_set(self, text):
        self.rpt_text.config(state="normal")
        self.rpt_text.delete("1.0", "end")
        self.rpt_text.insert("end", text)
        self.rpt_text.config(state="disabled")

    def _rpt_summary(self):
        s = self.actions.library_summary()
        genres = self.actions.genre_wise()
        lines = [
            "╔══════════════════════════════════════╗",
            "║         LIBRARY SUMMARY REPORT       ║",
            "╚══════════════════════════════════════╝",
            "",
            f"  📚  Total Book Titles     : {s['titles']}",
            f"  📦  Total Copies          : {s['total_copies']}",
            f"  👥  Total Members         : {s['total_members']}",
            f"      ↳ Standard            : {s['standard']}",
            f"      ↳ Premium             : {s['premium']}",
            "",
            f"  🔄  Total Transactions    : {s['total_transactions']}",
            f"  📋  Active Borrowings     : {s['active_borrowings']}",
            f"  ✅  Completed Returns     : {s['completed_returns']}",
            "",
            "─"*42,
            "  GENRE BREAKDOWN",
            "─"*42,
            f"  {'Genre':<16} {'Count':>6}   {'%':>6}",
            "─"*42,
        ]
        for g, count, pct in genres:
            bar = "█" * int(pct / 5)
            lines.append(f"  {g:<16} {count:>6}   {pct:>5.1f}%  {bar}")
        self._rpt_set("\n".join(lines))

    def _rpt_popular_book(self):
        result = self.actions.popular_books()
        if result[0] is None:
            self._rpt_set("No transactions found.")
            return
        bid, title, count = result
        self._rpt_set(
            f"⭐  Most Popular Book\n{'─'*40}\n"
            f"Book ID  : {bid}\n"
            f"Title    : {title}\n"
            f"Borrowed : {count} time(s)"
        )

    def _rpt_popular_mem(self):
        result = self.actions.popular_member()
        if result[0] is None:
            self._rpt_set("No transactions found.")
            return
        mid, name, count = result
        self._rpt_set(
            f"🏆  Most Active Member\n{'─'*40}\n"
            f"Member ID : {mid}\n"
            f"Name      : {name}\n"
            f"Borrows   : {count} time(s)"
        )

    def _rpt_genre(self):
        genres = self.actions.genre_wise()
        lines = [
            "📚  Genre-wise Book Distribution",
            "─"*44,
            f"  {'Genre':<16} {'Books':>6}   {'%':>6}   Bar",
            "─"*44,
        ]
        for g, count, pct in genres:
            bar = "█" * int(pct / 5) if pct else ""
            lines.append(f"  {g:<16} {count:>6}   {pct:>5.1f}%  {bar}")
        self._rpt_set("\n".join(lines))

    def _rpt_monthly(self):
        try:
            y = int(self.rpt_year.get())
            m = int(self.rpt_month.get())
        except ValueError:
            messagebox.showerror("Error", "Enter valid year and month numbers.")
            return
        records = self.actions.monthly_borrow_report(y, m)
        if not records:
            self._rpt_set(f"No transactions found for {y}/{m:02d}.")
            return
        lines = [
            f"📅  Monthly Report — {y}/{m:02d}",
            f"    Total Transactions: {len(records)}",
            "─"*66,
            f"  {'TxnID':<10} {'MemberID':<12} {'BookID':<10} {'BorrowDate':<14} {'DueDate'}",
            "─"*66,
        ]
        for r in records:
            lines.append(f"  {r[0]:<10} {r[1]:<12} {r[2]:<10} {r[3]:<14} {r[4]}")
        self._rpt_set("\n".join(lines))

    # ──────────────────────────────────────────
    #                  SEARCH
    # ──────────────────────────────────────────
    def _build_search(self, parent):
        hdr = tk.Frame(parent, bg=COLORS["bg"], pady=16)
        hdr.pack(fill="x", padx=30)
        tk.Label(hdr, text="🔍  Search", bg=COLORS["bg"],
                 fg=COLORS["white"], font=FONTS["title"]).pack(side="left")

        body = tk.Frame(parent, bg=COLORS["bg"])
        body.pack(fill="both", expand=True, padx=30, pady=4)
        body.columnconfigure(0, weight=1)
        body.columnconfigure(1, weight=2)
        body.rowconfigure(0, weight=1)

        # ── Left: search form ──
        left = tk.Frame(body, bg=COLORS["card"], padx=18, pady=18)
        left.grid(row=0, column=0, sticky="nsew", padx=(0, 12))
        left.columnconfigure(1, weight=1)

        tk.Label(left, text="Search Books", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["heading"]).grid(row=0, columnspan=2, sticky="w", pady=(0, 8))
        self.s_query = make_entry(left, "Search Query", 1)

        btn_row = tk.Frame(left, bg=COLORS["card"])
        btn_row.grid(row=2, columnspan=2, pady=8)
        styled_button(btn_row, "🔤  By Title", lambda: self._search("title"), COLORS["accent"], 13).pack(side="left", padx=3)
        styled_button(btn_row, "✍️  By Author", lambda: self._search("author"), COLORS["accent"], 13).pack(side="left", padx=3)
        styled_button(btn_row, "📌  By Genre", lambda: self._search("genre"), COLORS["green"], 13).pack(side="left", padx=3)

        ttk.Separator(left, orient="horizontal").grid(row=3, columnspan=2, sticky="ew", pady=12)

        tk.Label(left, text="Search Members", bg=COLORS["card"],
                 fg=COLORS["accent2"], font=FONTS["heading"]).grid(row=4, columnspan=2, sticky="w", pady=(0, 8))
        self.s_mem_query = make_entry(left, "Member Name", 5)
        styled_button(left, "👤  Search Member", lambda: self._search("member"), COLORS["accent2"], 20).grid(
            row=6, columnspan=2, pady=8)

        # ── Right: Results ──
        right = tk.Frame(body, bg=COLORS["card"], padx=12, pady=12)
        right.grid(row=0, column=1, sticky="nsew")

        tk.Label(right, text="Search Results", bg=COLORS["card"],
                 fg=COLORS["accent"], font=FONTS["sub"]).pack(anchor="w", pady=(0, 6))

        # Book results
        self.s_book_label = tk.Label(right, text="Books", bg=COLORS["card"],
                                     fg=COLORS["subtext"], font=FONTS["small"])
        self.s_book_label.pack(anchor="w")
        cols = ("ID", "Title", "Author", "Genre", "Available")
        tf, self.s_book_tree = make_treeview(right, cols, cols, height=7)
        tf.pack(fill="x", pady=4)

        ttk.Separator(right, orient="horizontal").pack(fill="x", pady=6)

        # Member results
        self.s_mem_label = tk.Label(right, text="Members", bg=COLORS["card"],
                                    fg=COLORS["subtext"], font=FONTS["small"])
        self.s_mem_label.pack(anchor="w")
        mcols = ("ID", "Name", "Phone", "Type", "Borrowed")
        mf, self.s_mem_tree = make_treeview(right, mcols, mcols, height=6)
        mf.pack(fill="x", pady=4)

    def _search(self, mode):
        for row in self.s_book_tree.get_children():
            self.s_book_tree.delete(row)
        for row in self.s_mem_tree.get_children():
            self.s_mem_tree.delete(row)

        if mode == "title":
            results = self.actions.search_by_title(self.s_query.get())
            self.s_book_label.config(text=f"Books matching title — {len(results)} found")
            for b in results:
                self.s_book_tree.insert("", "end", values=(b.book_id, b.title, b.author, b.genre, b.available_quantity))

        elif mode == "author":
            results = self.actions.search_by_author(self.s_query.get())
            self.s_book_label.config(text=f"Books matching author — {len(results)} found")
            for b in results:
                self.s_book_tree.insert("", "end", values=(b.book_id, b.title, b.author, b.genre, b.available_quantity))

        elif mode == "genre":
            results = self.actions.search_by_genre(self.s_query.get())
            self.s_book_label.config(text=f"Books matching genre — {len(results)} found")
            for b in results:
                self.s_book_tree.insert("", "end", values=(b.book_id, b.title, b.author, b.genre, b.available_quantity))

        elif mode == "member":
            results = self.actions.search_by_name(self.s_mem_query.get())
            self.s_mem_label.config(text=f"Members matching name — {len(results)} found")
            for m in results:
                self.s_mem_tree.insert("", "end", values=(m.member_id, m.name, m.phone, m.type, len(m.borrowed_books)))


# ─────────────────────────── LAUNCH ────────────────────────────

if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
