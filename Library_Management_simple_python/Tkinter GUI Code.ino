import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
from datetime import date, timedelta

# ─────────────────────────────────────────────
#  DATA
# ─────────────────────────────────────────────
books = [
    {"Book ID": "BK001", "Title": "The Python Handbook",  "Author": "John Smith",
     "Genre": "Technology", "Quantity": 7, "Available": 6, "Borrowed": 1},
    {"Book ID": "BK002", "Title": "The Java Handbook",    "Author": "John Smith",
     "Genre": "Technology", "Quantity": 5, "Available": 5, "Borrowed": 0},
]

genre_list = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]

members = [
    {"Member ID": "MEM001", "Name": "Ahmad Ali",      "Phone": "03001234567",
     "Type": "Standard", "Borrowed Books": []},
    {"Member ID": "MEM002", "Name": "Junaid Shabeer", "Phone": "03234578123",
     "Type": "Premium",  "Borrowed Books": ["BK001"]},
]

borrow_records = [{
    "Transaction ID": "TXN005", "Member ID": "MEM002", "Book ID": "BK001",
    "Borrow Date": date(2026, 3, 1), "Due Date": date(2026, 3, 6)
}]

Transaction_records = [{
    "Transaction ID": "TXN005", "Member ID": "MEM002", "Book ID": "BK001",
    "Borrow Date": date(2026, 3, 1), "Due Date": date(2026, 3, 6)
}]

# ─────────────────────────────────────────────
#  COLOURS / THEME
# ─────────────────────────────────────────────
BG         = "#0f172a"   # dark navy
SIDEBAR_BG = "#1e293b"
CARD_BG    = "#1e293b"
ACCENT     = "#6366f1"   # indigo
ACCENT2    = "#818cf8"
SUCCESS    = "#22c55e"
WARNING    = "#f59e0b"
DANGER     = "#ef4444"
TEXT       = "#f1f5f9"
MUTED      = "#94a3b8"
ENTRY_BG   = "#0f172a"
BORDER     = "#334155"
WHITE      = "#ffffff"

FONT_TITLE  = ("Segoe UI", 22, "bold")
FONT_HEAD   = ("Segoe UI", 14, "bold")
FONT_SUB    = ("Segoe UI", 11, "bold")
FONT_BODY   = ("Segoe UI", 10)
FONT_SMALL  = ("Segoe UI",  9)
FONT_BTN    = ("Segoe UI", 10, "bold")
FONT_MONO   = ("Courier New", 10)


# ─────────────────────────────────────────────
#  HELPERS
# ─────────────────────────────────────────────
def make_entry(parent, placeholder="", width=28, show=None):
    var = tk.StringVar()
    e = tk.Entry(parent, textvariable=var, bg=ENTRY_BG, fg=TEXT,
                 insertbackground=TEXT, relief="flat", font=FONT_BODY,
                 width=width, show=show or "")
    e.config(highlightthickness=1, highlightcolor=ACCENT,
             highlightbackground=BORDER)
    if placeholder:
        e.insert(0, placeholder)
        e.config(fg=MUTED)
        def on_focus_in(event):
            if e.get() == placeholder:
                e.delete(0, tk.END)
                e.config(fg=TEXT)
        def on_focus_out(event):
            if e.get() == "":
                e.insert(0, placeholder)
                e.config(fg=MUTED)
        e.bind("<FocusIn>",  on_focus_in)
        e.bind("<FocusOut>", on_focus_out)
    return e, var


def styled_button(parent, text, command, color=ACCENT, fg=WHITE,
                  width=18, font=FONT_BTN, pady=8):
    btn = tk.Button(parent, text=text, command=command,
                    bg=color, fg=fg, activebackground=ACCENT2,
                    activeforeground=WHITE, relief="flat",
                    font=font, width=width, cursor="hand2",
                    pady=pady, bd=0)
    btn.bind("<Enter>", lambda e: btn.config(bg=ACCENT2))
    btn.bind("<Leave>", lambda e: btn.config(bg=color))
    return btn


def label(parent, text, font=FONT_BODY, fg=TEXT, bg=None, **kw):
    return tk.Label(parent, text=text, font=font, fg=fg,
                    bg=bg or CARD_BG, **kw)


def section_title(parent, text):
    tk.Label(parent, text=text, font=FONT_TITLE, fg=ACCENT,
             bg=BG).pack(anchor="w", padx=30, pady=(25, 5))
    tk.Frame(parent, bg=ACCENT, height=2).pack(fill="x", padx=30, pady=(0, 18))


def card(parent, **kw):
    f = tk.Frame(parent, bg=CARD_BG, bd=0,
                 highlightthickness=1, highlightbackground=BORDER, **kw)
    return f


def result_box(parent):
    frame = tk.Frame(parent, bg=CARD_BG)
    sb = tk.Scrollbar(frame)
    sb.pack(side="right", fill="y")
    txt = tk.Text(frame, bg="#0d1117", fg=SUCCESS, font=FONT_MONO,
                  insertbackground=TEXT, relief="flat",
                  yscrollcommand=sb.set, state="disabled",
                  wrap="word", pady=8, padx=8)
    txt.pack(side="left", fill="both", expand=True)
    sb.config(command=txt.yview)
    frame.txt = txt
    return frame


def write_result(txt_widget, text, clear=True):
    txt_widget.config(state="normal")
    if clear:
        txt_widget.delete("1.0", tk.END)
    txt_widget.insert(tk.END, text + "\n")
    txt_widget.config(state="disabled")
    txt_widget.see(tk.END)


def get_val(entry, placeholder=""):
    v = entry.get().strip()
    return "" if v == placeholder else v


# ─────────────────────────────────────────────
#  BACKEND LOGIC  (your original logic, intact)
# ─────────────────────────────────────────────
def Book_Added(x1, x2, genere, x4):
    x5 = len(books) + 1
    x6 = f"BK{str(x5).zfill(3)}"
    books.append({"Book ID": x6, "Title": x1, "Author": x2,
                  "Genre": genere, "Quantity": x4,
                  "Available": x4, "Borrowed": 0})
    return x6


def member_added(y1, y2, membership):
    y3 = len(members) + 1
    y4 = f"MEM{str(y3).zfill(3)}"
    members.append({"Member ID": y4, "Name": y1, "Phone": y2,
                    "Type": membership, "Borrowed Books": []})
    return y4


def Book_Borrowed(z1, z2):
    today    = date.today()
    due_date = today + timedelta(days=7)
    z3 = len(borrow_records) + 1
    z4 = f"TXN{str(z3).zfill(3)}"

    Name = Type = None
    z5   = None
    for mem in members:
        if z1 == mem["Member ID"]:
            Name = mem["Name"]
            Type = mem["Type"]
            z5   = mem["Borrowed Books"]
            break

    if Name is None:
        return False, "Member not found."

    # sync z5 from active borrow_records
    for rec in borrow_records:
        if rec["Member ID"] == z1:
            if rec["Book ID"] not in z5:
                z5.append(rec["Book ID"])

    limit = 2 if Type == "Standard" else 4

    for book in books:
        if z2 == book["Book ID"]:
            if int(book["Available"]) > 0:
                if len(z5) < limit:
                    book["Available"] -= 1
                    book["Borrowed"]  += 1
                    z5.append(z2)
                    rec = {"Transaction ID": z4, "Member ID": z1,
                           "Book ID": z2, "Borrow Date": today,
                           "Due Date": due_date}
                    borrow_records.append(rec)
                    Transaction_records.append(rec)
                    receipt = (
                        f"✓ Book Borrowed Successfully!\n"
                        f"{'─'*38}\n"
                        f"  Transaction ID : {z4}\n"
                        f"  Member Name    : {Name}\n"
                        f"  Membership     : {Type}\n"
                        f"  Book           : {book['Title']}\n"
                        f"  Borrow Date    : {today}\n"
                        f"  Due Date       : {due_date}\n"
                        f"  Books in acct  : {len(z5)}\n"
                        f"{'─'*38}"
                    )
                    return True, receipt
                else:
                    return False, (f"Limit reached! You already have {len(z5)} books.\n"
                                   f"({Type} limit = {limit})")
            else:
                return False, "This book is out of stock."
    return False, "Book not found."


def Return_book(z6, z7):
    Fine_per_day = 10
    today1 = date.today()
    output = []
    found_rec = False

    for i in range(len(borrow_records)):
        if borrow_records[i]["Member ID"] == z6 and borrow_records[i]["Book ID"] == z7:
            DT = (today1 - borrow_records[i]["Due Date"]).days
            r  = borrow_records[i]
            output.append(f"✓ Book Returned Successfully!")
            output.append(f"{'─'*38}")
            output.append(f"  Transaction ID : {r['Transaction ID']}")
            output.append(f"  Member         : {r['Member ID']}")
            output.append(f"  Book           : {r['Book ID']}")
            output.append(f"  Borrow Date    : {r['Borrow Date']}")
            output.append(f"  Due Date       : {r['Due Date']}")
            output.append(f"  Return Date    : {today1}")
            output.append(f"  Days Borrowed  : 7")
            output.append(f"  Days Taken     : {DT}")
            if DT <= 0:
                output.append(f"  Status         : Returned on time ✓")
                output.append(f"  Fine           : Rs. 0")
            else:
                output.append(f"  Status         : {DT} day(s) LATE ⚠")
                output.append(f"  Fine           : Rs. {DT * Fine_per_day}")
            output.append(f"{'─'*38}")
            borrow_records.pop(i)
            found_rec = True
            break

    if not found_rec:
        return False, "No active borrow record found for this Member + Book."

    for mem in members:
        if z6 == mem["Member ID"]:
            if z7 in mem["Borrowed Books"]:
                mem["Borrowed Books"].remove(z7)
            output.append(f"  {z7} removed from {z6}'s account.")
            output.append(f"  Books remaining: {mem['Borrowed Books']}")

    for book in books:
        if book["Book ID"] == z7:
            book["Available"] += 1
            book["Borrowed"]  -= 1

    return True, "\n".join(output)


def Library_Summary():
    out = []
    Total_Copies = []
    Standard, Premium = [], []
    out.append("📚  LIBRARY SUMMARY")
    out.append("═" * 40)
    out.append(f"Total Book Titles : {len(books)}")
    for b in books:
        cnt = sum(1 for r in borrow_records if r["Book ID"] == b["Book ID"])
        avail = b["Quantity"] - cnt
        Total_Copies.append(b["Quantity"])
        out.append(f"  • {b['Title']}")
        out.append(f"    Quantity: {b['Quantity']}  |  Available: {avail}  |  Borrowed: {cnt}")
    out.append(f"\nTotal Copies       : {sum(Total_Copies)}")
    out.append(f"Currently Borrowed : {len(borrow_records)}")
    out.append("─" * 40)
    for mem in members:
        (Standard if mem["Type"] == "Standard" else Premium).append(mem["Name"])
    out.append(f"Total Members    : {len(members)}")
    out.append(f"  Standard       : {len(Standard)}")
    out.append(f"  Premium        : {len(Premium)}")
    out.append("─" * 40)
    out.append(f"Total Transactions : {len(Transaction_records)}")
    out.append(f"Active Borrowings  : {len(borrow_records)}")
    out.append(f"Completed Returns  : {len(Transaction_records) - len(borrow_records)}")
    return "\n".join(out)


def Popular_Books():
    IDs = [r["Book ID"] for r in Transaction_records]
    if not IDs:
        return "No books borrowed yet."
    most = max(set(IDs), key=IDs.count)
    title = next((b["Title"] for b in books if b["Book ID"] == most), most)
    out = ["🏆  MOST POPULAR BOOKS", "═" * 40]
    counts = {}
    for bid in IDs:
        counts[bid] = counts.get(bid, 0) + 1
    sorted_books = sorted(counts.items(), key=lambda x: x[1], reverse=True)
    for bid, cnt in sorted_books:
        t = next((b["Title"] for b in books if b["Book ID"] == bid), bid)
        out.append(f"  {bid}  |  {t:<30}  |  Borrowed: {cnt}x")
    out.append(f"\n⭐ Most Popular: {title}")
    return "\n".join(out)


def Active_Members():
    dct = {}
    for mem in members:
        m1 = sum(1 for r in Transaction_records if r["Member ID"] == mem["Member ID"])
        dct[mem["Name"]] = m1
    out = ["👥  ACTIVE MEMBERS REPORT", "═" * 40,
           f"{'Name':<25} {'Books Issued':>12}"]
    out.append("─" * 40)
    for name in sorted(dct, key=dct.get, reverse=True):
        out.append(f"  {name:<23} {dct[name]:>12}")
    best = max(dct, key=dct.get)
    out.append(f"\n⭐ Most Active: {best}")
    return "\n".join(out)


def Genre_report():
    dct = {}
    out = ["📂  GENRE-WISE DISTRIBUTION", "═" * 40,
           f"{'Genre':<18} {'Books':>6}  {'Percentage':>10}"]
    out.append("─" * 40)
    for g in genre_list:
        cnt = sum(1 for b in books if b.get("Genre") == g)
        dct[g] = cnt
        pct = f"{(cnt/len(books)*100):.1f}%" if books else "0%"
        out.append(f"  {g:<16} {cnt:>6}  {pct:>10}")
    return "\n".join(out)


def Monthly_Report(year, month):
    out = [f"📅  MONTHLY REPORT — {year}/{month:02d}", "═" * 40]
    found = False
    for r in Transaction_records:
        if r["Borrow Date"].year == year and r["Borrow Date"].month == month:
            out.append(f"  TxnID: {r['Transaction ID']}  |  "
                       f"Member: {r['Member ID']}  |  "
                       f"Book: {r['Book ID']}  |  "
                       f"Date: {r['Borrow Date']}")
            found = True
    if not found:
        out.append("No transactions found for this month.")
    return "\n".join(out)


# ─────────────────────────────────────────────
#  MAIN APPLICATION
# ─────────────────────────────────────────────
class LibraryApp(tk.Tk):

    def __init__(self):
        super().__init__()
        self.title("City Library Management System")
        self.geometry("1280x780")
        self.minsize(1100, 680)
        self.configure(bg=BG)
        self._build_ui()
        self.show_section("dashboard")

    # ── root layout ──────────────────────────
    def _build_ui(self):
        # sidebar
        self.sidebar = tk.Frame(self, bg=SIDEBAR_BG, width=210)
        self.sidebar.pack(side="left", fill="y")
        self.sidebar.pack_propagate(False)

        # content area
        self.content = tk.Frame(self, bg=BG)
        self.content.pack(side="left", fill="both", expand=True)

        self._build_sidebar()

        # pages dict
        self.pages = {}
        for name, builder in [
            ("dashboard",  self._page_dashboard),
            ("books",      self._page_books),
            ("members",    self._page_members),
            ("borrow",     self._page_borrow),
            ("reports",    self._page_reports),
            ("search",     self._page_search),
        ]:
            frame = tk.Frame(self.content, bg=BG)
            frame.place(relx=0, rely=0, relwidth=1, relheight=1)
            builder(frame)
            self.pages[name] = frame

    def _build_sidebar(self):
        # logo
        tk.Label(self.sidebar, text="📚", font=("Segoe UI", 36),
                 bg=SIDEBAR_BG, fg=ACCENT).pack(pady=(28, 4))
        tk.Label(self.sidebar, text="City Library", font=("Segoe UI", 13, "bold"),
                 bg=SIDEBAR_BG, fg=TEXT).pack()
        tk.Label(self.sidebar, text="Management System", font=("Segoe UI", 9),
                 bg=SIDEBAR_BG, fg=MUTED).pack(pady=(0, 20))
        tk.Frame(self.sidebar, bg=BORDER, height=1).pack(fill="x", padx=16)

        nav_items = [
            ("🏠  Dashboard",       "dashboard"),
            ("📖  Book Management", "books"),
            ("👤  Members",         "members"),
            ("↕️   Borrow & Return", "borrow"),
            ("📊  Reports",         "reports"),
            ("🔍  Search",          "search"),
        ]
        self.nav_buttons = {}
        for label_text, key in nav_items:
            btn = tk.Button(
                self.sidebar, text=label_text,
                font=("Segoe UI", 10), anchor="w", padx=20,
                bg=SIDEBAR_BG, fg=TEXT, activebackground=ACCENT,
                activeforeground=WHITE, relief="flat", bd=0,
                cursor="hand2", pady=11,
                command=lambda k=key: self.show_section(k)
            )
            btn.pack(fill="x", pady=1)
            self.nav_buttons[key] = btn

        # footer
        tk.Frame(self.sidebar, bg=BORDER, height=1).pack(fill="x", padx=16, side="bottom", pady=10)
        tk.Label(self.sidebar, text=f"Today: {date.today()}",
                 font=FONT_SMALL, bg=SIDEBAR_BG, fg=MUTED).pack(side="bottom", pady=4)

    def show_section(self, key):
        for k, btn in self.nav_buttons.items():
            btn.config(bg=ACCENT if k == key else SIDEBAR_BG,
                       fg=WHITE  if k == key else TEXT)
        self.pages[key].lift()

    # ── DASHBOARD ────────────────────────────
    def _page_dashboard(self, parent):
        tk.Label(parent, text="Welcome to City Library", font=("Segoe UI", 26, "bold"),
                 fg=TEXT, bg=BG).pack(anchor="w", padx=32, pady=(30, 4))
        tk.Label(parent, text="Your complete library management solution",
                 font=("Segoe UI", 11), fg=MUTED, bg=BG).pack(anchor="w", padx=32)
        tk.Frame(parent, bg=ACCENT, height=2).pack(fill="x", padx=32, pady=(10, 20))

        stat_frame = tk.Frame(parent, bg=BG)
        stat_frame.pack(fill="x", padx=32)

        def stat_card(f, icon, label, val, color):
            c = tk.Frame(f, bg=CARD_BG, padx=18, pady=18,
                         highlightthickness=1, highlightbackground=color)
            c.pack(side="left", expand=True, fill="both", padx=8)
            tk.Label(c, text=icon,  font=("Segoe UI", 28), bg=CARD_BG, fg=color).pack()
            tk.Label(c, text=str(val), font=("Segoe UI", 28, "bold"),
                     bg=CARD_BG, fg=color).pack()
            tk.Label(c, text=label, font=FONT_SMALL, bg=CARD_BG, fg=MUTED).pack()

        stat_card(stat_frame, "📚", "Total Books",   len(books),          ACCENT)
        stat_card(stat_frame, "👤", "Members",        len(members),        SUCCESS)
        stat_card(stat_frame, "↗️", "Active Borrows", len(borrow_records), WARNING)
        stat_card(stat_frame, "🔄", "Transactions",  len(Transaction_records), "#38bdf8")

        # Recent Transactions
        tk.Label(parent, text="Recent Transactions", font=FONT_HEAD,
                 fg=TEXT, bg=BG).pack(anchor="w", padx=32, pady=(30, 8))

        cols = ("TxnID", "Member", "Book", "Borrow Date", "Due Date")
        tv_frame = tk.Frame(parent, bg=BG)
        tv_frame.pack(fill="both", expand=True, padx=32, pady=(0, 20))

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("Treeview",
                        background=CARD_BG, foreground=TEXT,
                        rowheight=26, fieldbackground=CARD_BG,
                        font=FONT_BODY, borderwidth=0)
        style.configure("Treeview.Heading",
                        background=ACCENT, foreground=WHITE,
                        font=FONT_SUB, relief="flat")
        style.map("Treeview", background=[("selected", ACCENT)])

        tv = ttk.Treeview(tv_frame, columns=cols, show="headings", height=8)
        for c in cols:
            tv.heading(c, text=c)
            tv.column(c, width=180, anchor="center")
        sb = ttk.Scrollbar(tv_frame, orient="vertical", command=tv.yview)
        tv.configure(yscrollcommand=sb.set)
        tv.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")

        for r in Transaction_records[-10:][::-1]:
            tv.insert("", "end",
                      values=(r["Transaction ID"], r["Member ID"],
                              r["Book ID"], r["Borrow Date"], r["Due Date"]))
        self._dash_tv = tv

    def _refresh_dashboard(self):
        tv = self._dash_tv
        for row in tv.get_children():
            tv.delete(row)
        for r in Transaction_records[-10:][::-1]:
            tv.insert("", "end",
                      values=(r["Transaction ID"], r["Member ID"],
                              r["Book ID"], r["Borrow Date"], r["Due Date"]))

    # ── BOOK MANAGEMENT ──────────────────────
    def _page_books(self, parent):
        section_title(parent, "📖  Book Management")
        pane = tk.Frame(parent, bg=BG)
        pane.pack(fill="both", expand=True, padx=30)

        # LEFT: form
        left = card(pane)
        left.pack(side="left", fill="y", padx=(0, 12), ipadx=14, ipady=14)

        label(left, "Add New Book", font=FONT_HEAD, fg=ACCENT).pack(anchor="w", padx=16, pady=(12, 10))

        form_fields = {}
        for lbl, ph in [("Title",  "e.g. The Great Gatsby"),
                        ("Author", "e.g. F. Scott Fitzgerald")]:
            label(left, lbl).pack(anchor="w", padx=16)
            e, v = make_entry(left, ph)
            e.pack(padx=16, pady=(2, 10), fill="x")
            form_fields[lbl] = (e, v, ph)

        label(left, "Genre").pack(anchor="w", padx=16)
        genre_var = tk.StringVar(value=genre_list[0])
        genre_cb  = ttk.Combobox(left, textvariable=genre_var,
                                 values=genre_list, state="readonly",
                                 font=FONT_BODY, width=26)
        genre_cb.pack(padx=16, pady=(2, 10), fill="x")

        label(left, "Quantity").pack(anchor="w", padx=16)
        qty_e, qty_v = make_entry(left, "e.g. 5")
        qty_e.pack(padx=16, pady=(2, 14), fill="x")

        result_lbl = tk.Label(left, text="", font=FONT_BODY,
                              fg=SUCCESS, bg=CARD_BG, wraplength=220, justify="left")
        result_lbl.pack(padx=16)

        def do_add_book():
            t  = get_val(form_fields["Title"][0],  "e.g. The Great Gatsby")
            a  = get_val(form_fields["Author"][0], "e.g. F. Scott Fitzgerald")
            g  = genre_var.get()
            q  = get_val(qty_e, "e.g. 5")
            if not t or not a or not q:
                messagebox.showwarning("Missing Fields", "Please fill in all fields.")
                return
            try:
                q = int(q)
            except ValueError:
                messagebox.showerror("Invalid Quantity", "Quantity must be a number.")
                return
            bid = Book_Added(t, a, g, q)
            result_lbl.config(text=f"✓ Book added!\nID: {bid}")
            self._refresh_books_table()
            for (e, v, ph) in form_fields.values():
                e.delete(0, tk.END); e.insert(0, ph); e.config(fg=MUTED)
            qty_e.delete(0, tk.END); qty_e.insert(0, "e.g. 5"); qty_e.config(fg=MUTED)

        styled_button(left, "➕  Add Book", do_add_book, width=24).pack(padx=16, pady=10)

        # Remove book
        tk.Frame(left, bg=BORDER, height=1).pack(fill="x", padx=16, pady=8)
        label(left, "Remove Book", font=FONT_HEAD, fg=DANGER).pack(anchor="w", padx=16)
        rm_e, rm_v = make_entry(left, "Book ID  e.g. BK001")
        rm_e.pack(padx=16, pady=(4, 8), fill="x")

        def do_remove_book():
            bid = get_val(rm_e, "Book ID  e.g. BK001").upper()
            if not bid:
                messagebox.showwarning("Missing", "Enter a Book ID."); return
            target = next((b for b in books if b["Book ID"] == bid), None)
            if target is None:
                messagebox.showerror("Not Found", f"Book {bid} not found."); return
            if messagebox.askyesno("Confirm", f"Remove '{target['Title']}'?"):
                books.remove(target)
                self._refresh_books_table()
                messagebox.showinfo("Removed", f"'{target['Title']}' removed.")

        styled_button(left, "🗑  Remove Book", do_remove_book,
                      color=DANGER, width=24).pack(padx=16, pady=(0, 14))

        # Update Quantity info
        tk.Frame(left, bg=BORDER, height=1).pack(fill="x", padx=16, pady=8)
        label(left, "Check / Update Quantity", font=FONT_HEAD, fg=WARNING).pack(anchor="w", padx=16)
        chk_e, chk_v = make_entry(left, "Book ID  e.g. BK001")
        chk_e.pack(padx=16, pady=(4, 4), fill="x")
        chk_lbl = tk.Label(left, text="", font=FONT_BODY, fg=WARNING,
                           bg=CARD_BG, wraplength=220, justify="left")
        chk_lbl.pack(padx=16)

        def do_check_qty():
            bid = get_val(chk_e, "Book ID  e.g. BK001").upper()
            if not bid:
                chk_lbl.config(text="Enter a Book ID."); return
            b = next((x for x in books if x["Book ID"] == bid), None)
            if b is None:
                chk_lbl.config(text="Book not found."); return
            msg = f"{bid}: {b['Quantity']} copies.\n"
            msg += "⚠ Order more copies!" if b["Quantity"] <= 5 else "✓ Stock sufficient."
            chk_lbl.config(text=msg)

        styled_button(left, "🔎  Check Quantity", do_check_qty,
                      color=WARNING, fg="#0f172a", width=24).pack(padx=16, pady=(4, 14))

        # RIGHT: table
        right = tk.Frame(pane, bg=BG)
        right.pack(side="left", fill="both", expand=True)

        label(right, "All Books", font=FONT_HEAD, fg=TEXT, bg=BG).pack(
            anchor="w", pady=(0, 6))

        cols = ("Book ID", "Title", "Author", "Genre", "Quantity", "Available", "Borrowed")
        tv_f = tk.Frame(right, bg=BG)
        tv_f.pack(fill="both", expand=True)
        tv = ttk.Treeview(tv_f, columns=cols, show="headings")
        for c in cols:
            tv.heading(c, text=c)
            tv.column(c, width=120 if c not in ("Title","Author") else 190, anchor="center")
        sb2 = ttk.Scrollbar(tv_f, orient="vertical", command=tv.yview)
        tv.configure(yscrollcommand=sb2.set)
        tv.pack(side="left", fill="both", expand=True)
        sb2.pack(side="right", fill="y")
        self._books_tv = tv

        styled_button(right, "🔄  Refresh", self._refresh_books_table,
                      color=ACCENT, width=16).pack(anchor="e", pady=6)
        self._refresh_books_table()

    def _refresh_books_table(self):
        tv = self._books_tv
        for row in tv.get_children():
            tv.delete(row)
        for b in books:
            tv.insert("", "end",
                      values=(b["Book ID"], b["Title"], b["Author"],
                               b.get("Genre",""), b["Quantity"],
                               b["Available"], b["Borrowed"]))

    # ── MEMBER MANAGEMENT ────────────────────
    def _page_members(self, parent):
        section_title(parent, "👤  Member Management")
        pane = tk.Frame(parent, bg=BG)
        pane.pack(fill="both", expand=True, padx=30)

        # LEFT: form
        left = card(pane)
        left.pack(side="left", fill="y", padx=(0, 12), ipadx=14, ipady=14)

        label(left, "Register New Member", font=FONT_HEAD, fg=ACCENT).pack(anchor="w", padx=16, pady=(12, 10))

        form_fields = {}
        for lbl, ph in [("Name",  "e.g. Ali Hassan"),
                        ("Phone", "e.g. 03001234567")]:
            label(left, lbl).pack(anchor="w", padx=16)
            e, v = make_entry(left, ph)
            e.pack(padx=16, pady=(2, 10), fill="x")
            form_fields[lbl] = (e, v, ph)

        label(left, "Membership Type").pack(anchor="w", padx=16)
        mtype_var = tk.StringVar(value="Standard")
        for t in ["Standard", "Premium"]:
            tk.Radiobutton(left, text=t, variable=mtype_var, value=t,
                           bg=CARD_BG, fg=TEXT, selectcolor=ACCENT,
                           activebackground=CARD_BG, font=FONT_BODY).pack(anchor="w", padx=24)

        reg_lbl = tk.Label(left, text="", font=FONT_BODY,
                           fg=SUCCESS, bg=CARD_BG, wraplength=220)
        reg_lbl.pack(padx=16, pady=6)

        def do_register():
            n  = get_val(form_fields["Name"][0],  "e.g. Ali Hassan")
            ph = get_val(form_fields["Phone"][0], "e.g. 03001234567")
            if not n or not ph:
                messagebox.showwarning("Missing Fields", "Fill in all fields."); return
            mid = member_added(n, ph, mtype_var.get())
            reg_lbl.config(text=f"✓ Registered!\nID: {mid}")
            self._refresh_members_table()

        styled_button(left, "➕  Register", do_register, width=24).pack(padx=16, pady=8)

        # Deactivate
        tk.Frame(left, bg=BORDER, height=1).pack(fill="x", padx=16, pady=8)
        label(left, "Deactivate Member", font=FONT_HEAD, fg=DANGER).pack(anchor="w", padx=16)
        da_e, _ = make_entry(left, "Member ID  e.g. MEM001")
        da_e.pack(padx=16, pady=(4, 8), fill="x")

        def do_deactivate():
            mid = get_val(da_e, "Member ID  e.g. MEM001").upper()
            if not mid:
                messagebox.showwarning("Missing", "Enter Member ID."); return
            target = next((m for m in members if m["Member ID"] == mid), None)
            if target is None:
                messagebox.showerror("Not Found", f"Member {mid} not found."); return
            if messagebox.askyesno("Confirm", f"Remove '{target['Name']}'?"):
                members.remove(target)
                self._refresh_members_table()
                messagebox.showinfo("Done", f"'{target['Name']}' deactivated.")

        styled_button(left, "🗑  Deactivate", do_deactivate,
                      color=DANGER, width=24).pack(padx=16, pady=(0, 14))

        # View details
        tk.Frame(left, bg=BORDER, height=1).pack(fill="x", padx=16, pady=8)
        label(left, "View Member Details", font=FONT_HEAD, fg=WARNING).pack(anchor="w", padx=16)
        vd_e, _ = make_entry(left, "Member ID  e.g. MEM001")
        vd_e.pack(padx=16, pady=(4, 4), fill="x")
        vd_lbl = tk.Label(left, text="", font=FONT_BODY, fg=WARNING,
                          bg=CARD_BG, justify="left", wraplength=220)
        vd_lbl.pack(padx=16)

        def do_view_details():
            mid = get_val(vd_e, "Member ID  e.g. MEM001").upper()
            m = next((x for x in members if x["Member ID"] == mid), None)
            if m is None:
                vd_lbl.config(text="Member not found."); return
            vd_lbl.config(
                text=f"ID   : {m['Member ID']}\n"
                     f"Name : {m['Name']}\n"
                     f"Phone: {m['Phone']}\n"
                     f"Type : {m['Type']}\n"
                     f"Books: {m['Borrowed Books']}")

        styled_button(left, "🔎  View Details", do_view_details,
                      color=WARNING, fg="#0f172a", width=24).pack(padx=16, pady=(4, 14))

        # RIGHT: table
        right = tk.Frame(pane, bg=BG)
        right.pack(side="left", fill="both", expand=True)

        label(right, "All Members", font=FONT_HEAD, fg=TEXT, bg=BG).pack(
            anchor="w", pady=(0, 6))

        cols = ("Member ID", "Name", "Phone", "Type", "Borrowed Books")
        tv_f = tk.Frame(right, bg=BG)
        tv_f.pack(fill="both", expand=True)
        tv = ttk.Treeview(tv_f, columns=cols, show="headings")
        for c in cols:
            tv.heading(c, text=c)
            tv.column(c, width=160 if c != "Borrowed Books" else 200, anchor="center")
        sb2 = ttk.Scrollbar(tv_f, orient="vertical", command=tv.yview)
        tv.configure(yscrollcommand=sb2.set)
        tv.pack(side="left", fill="both", expand=True)
        sb2.pack(side="right", fill="y")
        self._members_tv = tv

        styled_button(right, "🔄  Refresh", self._refresh_members_table,
                      color=ACCENT, width=16).pack(anchor="e", pady=6)
        self._refresh_members_table()

    def _refresh_members_table(self):
        tv = self._members_tv
        for row in tv.get_children():
            tv.delete(row)
        for m in members:
            tv.insert("", "end",
                      values=(m["Member ID"], m["Name"], m["Phone"],
                               m["Type"], str(m["Borrowed Books"])))

    # ── BORROW & RETURN ──────────────────────
    def _page_borrow(self, parent):
        section_title(parent, "↕️   Borrow & Return")
        pane = tk.Frame(parent, bg=BG)
        pane.pack(fill="both", expand=True, padx=30, pady=(0, 20))

        # ── top row: borrow + return side by side ──
        top = tk.Frame(pane, bg=BG)
        top.pack(fill="x")

        # BORROW card
        bc = card(top)
        bc.pack(side="left", fill="y", padx=(0, 10), ipadx=14, ipady=14)
        label(bc, "Borrow a Book", font=FONT_HEAD, fg=SUCCESS).pack(anchor="w", padx=16, pady=(10, 8))

        bm_e, _ = make_entry(bc, "Member ID  e.g. MEM001")
        bm_e.pack(padx=16, pady=(2, 6), fill="x")
        bb_e, _ = make_entry(bc, "Book ID  e.g. BK001")
        bb_e.pack(padx=16, pady=(2, 8), fill="x")

        borrow_out = result_box(bc)
        borrow_out.pack(padx=16, pady=(0, 8), fill="x")
        borrow_out.config(height=100)

        def do_borrow():
            mid = get_val(bm_e, "Member ID  e.g. MEM001").upper()
            bid = get_val(bb_e, "Book ID  e.g. BK001").upper()
            if not mid or not bid:
                write_result(borrow_out.txt, "⚠ Enter both Member ID and Book ID."); return
            ok, msg = Book_Borrowed(mid, bid)
            write_result(borrow_out.txt, msg)
            if ok:
                self._refresh_active_table()
                self._refresh_members_table()
                self._refresh_books_table()
                self._refresh_dashboard()

        styled_button(bc, "📥  Borrow", do_borrow, color=SUCCESS, width=22).pack(padx=16, pady=(0, 14))

        # RETURN card
        rc = card(top)
        rc.pack(side="left", fill="y", padx=(0, 0), ipadx=14, ipady=14)
        label(rc, "Return a Book", font=FONT_HEAD, fg=WARNING).pack(anchor="w", padx=16, pady=(10, 8))

        rm_e2, _ = make_entry(rc, "Member ID  e.g. MEM001")
        rm_e2.pack(padx=16, pady=(2, 6), fill="x")
        rb_e, _  = make_entry(rc, "Book ID  e.g. BK001")
        rb_e.pack(padx=16, pady=(2, 8), fill="x")

        return_out = result_box(rc)
        return_out.pack(padx=16, pady=(0, 8), fill="x")
        return_out.config(height=100)

        def do_return():
            mid = get_val(rm_e2, "Member ID  e.g. MEM001").upper()
            bid = get_val(rb_e,  "Book ID  e.g. BK001").upper()
            if not mid or not bid:
                write_result(return_out.txt, "⚠ Enter both Member ID and Book ID."); return
            ok, msg = Return_book(mid, bid)
            write_result(return_out.txt, msg)
            if ok:
                self._refresh_active_table()
                self._refresh_members_table()
                self._refresh_books_table()
                self._refresh_dashboard()

        styled_button(rc, "📤  Return", do_return, color=WARNING, fg="#0f172a", width=22).pack(padx=16, pady=(0, 14))

        # OVERDUE card
        oc = card(top)
        oc.pack(side="left", fill="y", padx=(10, 0), ipadx=14, ipady=14)
        label(oc, "Check Overdue", font=FONT_HEAD, fg=DANGER).pack(anchor="w", padx=16, pady=(10, 8))
        od_e, _ = make_entry(oc, "Member ID  e.g. MEM001")
        od_e.pack(padx=16, pady=(2, 8), fill="x")
        overdue_out = result_box(oc)
        overdue_out.pack(padx=16, pady=(0, 8), fill="x")
        overdue_out.config(height=100)

        def do_overdue():
            z9 = get_val(od_e, "Member ID  e.g. MEM001").upper()
            if not z9:
                write_result(overdue_out.txt, "Enter a Member ID."); return
            today2 = date.today()
            out = [f"Overdue check for {z9}"]
            out.append("─" * 38)
            found = False
            for BR in borrow_records:
                if z9 == BR["Member ID"]:
                    found = True
                    total_days  = (today2 - BR["Borrow Date"]).days
                    Extra_days  = (today2 - BR["Due Date"]).days
                    days_left   = (BR["Due Date"] - today2).days
                    out.append(f"Book     : {BR['Book ID']}")
                    out.append(f"Borrowed : {total_days} days ago")
                    if BR["Due Date"] >= today2:
                        out.append(f"Status   : ✓ On time — {days_left} day(s) left")
                        out.append(f"Fine     : Rs. 0")
                    else:
                        out.append(f"Status   : ⚠ {Extra_days} day(s) OVERDUE")
                        out.append(f"Fine     : Rs. {Extra_days * 10}")
                    out.append("")
            if not found:
                out.append("No active borrow records.")
            write_result(overdue_out.txt, "\n".join(out))

        styled_button(oc, "⚠  Check Overdue", do_overdue,
                      color=DANGER, width=22).pack(padx=16, pady=(0, 14))

        # Active borrowings table
        tk.Label(pane, text="Active Borrowings", font=FONT_HEAD,
                 fg=TEXT, bg=BG).pack(anchor="w", pady=(18, 6))

        cols = ("Transaction ID", "Member ID", "Book ID", "Borrow Date", "Due Date", "Status")
        tv_f = tk.Frame(pane, bg=BG)
        tv_f.pack(fill="both", expand=True)
        tv = ttk.Treeview(tv_f, columns=cols, show="headings", height=7)
        for c in cols:
            tv.heading(c, text=c)
            tv.column(c, width=170, anchor="center")
        sb2 = ttk.Scrollbar(tv_f, orient="vertical", command=tv.yview)
        tv.configure(yscrollcommand=sb2.set)
        tv.pack(side="left", fill="both", expand=True)
        sb2.pack(side="right", fill="y")
        self._active_tv = tv
        self._refresh_active_table()

    def _refresh_active_table(self):
        tv = self._active_tv
        for row in tv.get_children():
            tv.delete(row)
        today = date.today()
        for r in borrow_records:
            status = "On time ✓" if r["Due Date"] >= today else f"OVERDUE ⚠"
            tv.insert("", "end",
                      values=(r["Transaction ID"], r["Member ID"],
                               r["Book ID"], r["Borrow Date"],
                               r["Due Date"], status))

    # ── REPORTS ──────────────────────────────
    def _page_reports(self, parent):
        section_title(parent, "📊  Reports & Statistics")
        pane = tk.Frame(parent, bg=BG)
        pane.pack(fill="both", expand=True, padx=30, pady=(0, 20))

        btn_frame = tk.Frame(pane, bg=BG)
        btn_frame.pack(anchor="w", pady=(0, 16))

        out_frame = result_box(pane)
        out_frame.pack(fill="both", expand=True)

        def show(fn):
            write_result(out_frame.txt, fn())

        buttons = [
            ("📚  Library Summary",      lambda: Library_Summary()),
            ("🏆  Popular Books",         lambda: Popular_Books()),
            ("👥  Active Members",        lambda: Active_Members()),
            ("📂  Genre Distribution",    lambda: Genre_report()),
        ]
        for txt, fn in buttons:
            styled_button(btn_frame, txt, lambda f=fn: show(f),
                          width=20, pady=9).pack(side="left", padx=6)

        # Monthly Report
        mo_frame = card(pane)
        mo_frame.pack(fill="x", pady=(10, 0), ipadx=10, ipady=8)
        label(mo_frame, "Monthly Borrowing Report", font=FONT_HEAD, fg=ACCENT).pack(
            anchor="w", padx=14, pady=(8, 6))
        row = tk.Frame(mo_frame, bg=CARD_BG)
        row.pack(anchor="w", padx=14)
        label(row, "Year:").pack(side="left")
        yr_e, _ = make_entry(row, str(date.today().year), width=8)
        yr_e.pack(side="left", padx=6)
        label(row, "Month (1-12):").pack(side="left", padx=(10, 0))
        mn_e, _ = make_entry(row, str(date.today().month), width=5)
        mn_e.pack(side="left", padx=6)

        def do_monthly():
            y = get_val(yr_e, str(date.today().year))
            m = get_val(mn_e, str(date.today().month))
            try:
                y, m = int(y), int(m)
            except ValueError:
                write_result(out_frame.txt, "Enter valid Year and Month."); return
            write_result(out_frame.txt, Monthly_Report(y, m))

        styled_button(row, "📅  Generate", do_monthly, width=16, pady=6).pack(side="left", padx=8)

    # ── SEARCH ───────────────────────────────
    def _page_search(self, parent):
        section_title(parent, "🔍  Search")
        pane = tk.Frame(parent, bg=BG)
        pane.pack(fill="both", expand=True, padx=30)

        # Book searches
        bc = card(pane)
        bc.pack(fill="x", pady=(0, 14), ipadx=12, ipady=12)
        label(bc, "Search Books", font=FONT_HEAD, fg=ACCENT).pack(anchor="w", padx=14, pady=(8, 6))

        row = tk.Frame(bc, bg=CARD_BG)
        row.pack(anchor="w", padx=14, fill="x")

        t_e, _ = make_entry(row, "Title keyword...", width=22)
        t_e.pack(side="left", padx=(0, 8))
        a_e, _ = make_entry(row, "Author keyword...", width=22)
        a_e.pack(side="left", padx=(0, 8))
        g_e, _ = make_entry(row, "Genre keyword...", width=22)
        g_e.pack(side="left", padx=(0, 8))

        bk_out = result_box(bc)
        bk_out.pack(padx=14, pady=(8, 4), fill="x")
        bk_out.config(height=130)

        def search_books_title():
            kw = get_val(t_e, "Title keyword...").lower()
            if not kw: write_result(bk_out.txt, "Enter a keyword."); return
            res = [b for b in books if kw in b["Title"].lower()]
            if not res: write_result(bk_out.txt, "No matching books found."); return
            out = [f"Results for Title: '{kw}'", "─"*40]
            for b in res:
                out.append(f"  {b['Book ID']} | {b['Title']} | {b['Author']} | {b.get('Genre','')} | Avail: {b['Available']}/{b['Quantity']}")
            write_result(bk_out.txt, "\n".join(out))

        def search_books_author():
            kw = get_val(a_e, "Author keyword...").lower()
            if not kw: write_result(bk_out.txt, "Enter a keyword."); return
            res = [b for b in books if kw in b["Author"].lower()]
            if not res: write_result(bk_out.txt, "No matching books found."); return
            out = [f"Results for Author: '{kw}'", "─"*40]
            for b in res:
                out.append(f"  {b['Book ID']} | {b['Title']} | {b['Author']} | {b.get('Genre','')} | Avail: {b['Available']}/{b['Quantity']}")
            write_result(bk_out.txt, "\n".join(out))

        def search_books_genre():
            kw = get_val(g_e, "Genre keyword...").lower()
            if not kw: write_result(bk_out.txt, "Enter a keyword."); return
            res = [b for b in books if kw in b.get("Genre","").lower()]
            if not res: write_result(bk_out.txt, "No matching books found."); return
            out = [f"Results for Genre: '{kw}'", "─"*40]
            for b in res:
                out.append(f"  {b['Book ID']} | {b['Title']} | {b['Author']} | {b.get('Genre','')} | Avail: {b['Available']}/{b['Quantity']}")
            write_result(bk_out.txt, "\n".join(out))

        btns = tk.Frame(bc, bg=CARD_BG)
        btns.pack(anchor="w", padx=14, pady=(4, 10))
        styled_button(btns, "🔍  By Title",  search_books_title,  width=18, pady=7).pack(side="left", padx=(0, 8))
        styled_button(btns, "🔍  By Author", search_books_author, width=18, pady=7).pack(side="left", padx=(0, 8))
        styled_button(btns, "🔍  By Genre",  search_books_genre,  width=18, pady=7).pack(side="left")

        # Member search
        mc = card(pane)
        mc.pack(fill="x", pady=(0, 14), ipadx=12, ipady=12)
        label(mc, "Search Members", font=FONT_HEAD, fg=ACCENT).pack(anchor="w", padx=14, pady=(8, 6))

        mrow = tk.Frame(mc, bg=CARD_BG)
        mrow.pack(anchor="w", padx=14, fill="x")
        mn_e2, _ = make_entry(mrow, "Member name keyword...", width=32)
        mn_e2.pack(side="left", padx=(0, 8))

        mem_out = result_box(mc)
        mem_out.pack(padx=14, pady=(8, 4), fill="x")
        mem_out.config(height=120)

        def search_member_name():
            kw = get_val(mn_e2, "Member name keyword...").lower()
            if not kw: write_result(mem_out.txt, "Enter a keyword."); return
            res = [m for m in members if kw in m["Name"].lower()]
            if not res: write_result(mem_out.txt, "No matching members found."); return
            out = [f"Results for Name: '{kw}'", "─"*40]
            for m in res:
                out.append(f"  {m['Member ID']} | {m['Name']} | {m['Phone']} | {m['Type']} | Books: {m['Borrowed Books']}")
            write_result(mem_out.txt, "\n".join(out))

        mbtn = tk.Frame(mc, bg=CARD_BG)
        mbtn.pack(anchor="w", padx=14, pady=(4, 10))
        styled_button(mbtn, "🔍  By Name", search_member_name, width=18, pady=7).pack(side="left")


# ─────────────────────────────────────────────
#  ENTRY POINT
# ─────────────────────────────────────────────
if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
