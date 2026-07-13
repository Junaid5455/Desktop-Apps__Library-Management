import tkinter as tk
from tkinter import ttk, messagebox, font
import csv
import os
from datetime import date, timedelta, datetime

# ─────────────────────────── CONSTANTS ───────────────────────────
FILE_NAME = "LibraryData.csv"

COLORS = {
    "bg":           "#0F1117",
    "sidebar":      "#161B27",
    "sidebar_hover":"#1E2638",
    "card":         "#1A2035",
    "card2":        "#1E2740",
    "accent":       "#4F8EF7",
    "accent2":      "#6C63FF",
    "success":      "#2DD4BF",
    "warning":      "#F59E0B",
    "danger":       "#EF4444",
    "text":         "#E8EDF5",
    "text_muted":   "#7B8DB0",
    "border":       "#252D42",
    "input_bg":     "#111827",
    "row_alt":      "#1C2438",
    "header_bg":    "#0F1117",
    "active":       "#263354",
    "active_text":  "#4F8EF7",
    "divider":      "#1E2638",
    "tag_std":      "#1E3A5F",
    "tag_prem":     "#2D1B69",
}

FONTS = {
    "title":    ("Segoe UI", 22, "bold"),
    "heading":  ("Segoe UI", 14, "bold"),
    "subhead":  ("Segoe UI", 11, "bold"),
    "body":     ("Segoe UI", 10),
    "small":    ("Segoe UI", 9),
    "mono":     ("Courier New", 9),
    "nav":      ("Segoe UI", 10, "bold"),
    "stat":     ("Segoe UI", 26, "bold"),
}

NAV_ITEMS = [
    ("🏠", "Dashboard",       "dashboard"),
    ("📚", "Books",           "books"),
    ("👥", "Members",         "members"),
    ("🔄", "Borrow & Return", "borrow"),
    ("📊", "Reports",         "reports"),
    ("🔍", "Search",          "search"),
]

GENRE_LIST = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]

# ─────────────────────────── DATA LAYER ───────────────────────────
def init_file():
    if not os.path.exists(FILE_NAME):
        with open(FILE_NAME, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["Category","Col1","Col2","Col3","Col4","Col5"])

def read_rows():
    rows = []
    with open(FILE_NAME, "r") as f:
        reader = csv.reader(f)
        next(reader, None)
        for row in reader:
            rows.append(row)
    return rows

def write_all(rows):
    with open(FILE_NAME, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["Category","Col1","Col2","Col3","Col4","Col5"])
        w.writerows(rows)

def count_category(cat):
    return sum(1 for r in read_rows() if r and r[0] == cat)

def next_id(cat, prefix):
    n = count_category(cat)
    return f"{prefix}{str(n+1).zfill(3)}"

def get_books():
    return [r for r in read_rows() if r and r[0] == "books"]

def get_members():
    return [r for r in read_rows() if r and r[0] == "members"]

def get_borrows():
    return [r for r in read_rows() if r and r[0] == "borrow_records"]

def get_transactions():
    return [r for r in read_rows() if r and r[0] == "Transaction_records"]

def add_book(title, author, genre, qty):
    bid = next_id("books", "BK")
    with open(FILE_NAME, "a", newline="") as f:
        csv.writer(f).writerow(["books", bid, title, author, genre, qty])
    return bid

def add_member(name, phone, mtype):
    mid = next_id("members", "MEM")
    with open(FILE_NAME, "a", newline="") as f:
        csv.writer(f).writerow(["members", mid, name, phone, mtype, ""])
    return mid

def borrow_book(mid, bid):
    today = date.today()
    due = today + timedelta(days=7)
    rows = read_rows()
    member, book = None, None
    for r in rows:
        if r[0]=="members" and r[1]==mid: member = r
        if r[0]=="books"   and r[1]==bid: book   = r
    if not member: return False, "Member not found."
    if not book:   return False, "Book not found."
    qty = int(book[5])
    if qty <= 0:   return False, "Book is out of stock."
    borrowed = [b for b in member[5].split("|") if b]
    limit = 2 if member[4]=="Standard" else 4
    if len(borrowed) >= limit: return False, f"Borrow limit ({limit}) reached."
    tid = next_id("Transaction_records", "TXN")
    new_rows = []
    for r in rows:
        if r[0]=="members" and r[1]==mid:
            borrowed.append(bid)
            r = list(r); r[5] = "|".join(borrowed)
        if r[0]=="books" and r[1]==bid:
            r = list(r); r[5] = str(int(r[5])-1)
        new_rows.append(r)
    write_all(new_rows)
    with open(FILE_NAME, "a", newline="") as f:
        w = csv.writer(f)
        w.writerow(["borrow_records", tid, mid, bid, today, due])
        w.writerow(["Transaction_records", tid, mid, bid, today, due])
    return True, {"tid": tid, "name": member[2], "type": member[4],
                  "title": book[2], "borrow": today, "due": due}

def return_book(mid, bid):
    today = date.today()
    rows = read_rows()
    found_borrow = None
    for r in rows:
        if r[0]=="borrow_records" and r[2]==mid and r[3]==bid:
            found_borrow = r; break
    if not found_borrow: return False, "No active borrow record found."
    new_rows = []
    for r in rows:
        if r[0]=="borrow_records" and r[2]==mid and r[3]==bid: continue
        if r[0]=="members" and r[1]==mid:
            r = list(r)
            books = [b for b in r[5].split("|") if b and b!=bid]
            r[5] = "|".join(books)
        if r[0]=="books" and r[1]==bid:
            r = list(r); r[5] = str(int(r[5])+1)
        new_rows.append(r)
    write_all(new_rows)
    due = datetime.strptime(found_borrow[5], "%Y-%m-%d").date()
    fine = max(0, (today - due).days) * 10
    return True, {"tid": found_borrow[1], "borrow": found_borrow[4],
                  "due": found_borrow[5], "return": today,
                  "fine": fine, "late": max(0,(today-due).days)}

def remove_book(bid):
    rows = [r for r in read_rows() if not (r[0]=="books" and r[1]==bid)]
    write_all(rows)

def remove_member(mid):
    rows = [r for r in read_rows() if not (r[0]=="members" and r[1]==mid)]
    write_all(rows)

def update_book_qty(bid, delta):
    rows = read_rows()
    for r in rows:
        if r[0]=="books" and r[1]==bid:
            r[5] = str(max(0, int(r[5]) + delta))
    write_all(rows)

# ─────────────────────────── WIDGETS ───────────────────────────────
def make_scrollable(parent):
    canvas = tk.Canvas(parent, bg=COLORS["bg"], highlightthickness=0, bd=0)
    sb = ttk.Scrollbar(parent, orient="vertical", command=canvas.yview)
    frame = tk.Frame(canvas, bg=COLORS["bg"])
    frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
    canvas.create_window((0,0), window=frame, anchor="nw")
    canvas.configure(yscrollcommand=sb.set)
    canvas.pack(side="left", fill="both", expand=True)
    sb.pack(side="right", fill="y")
    canvas.bind_all("<MouseWheel>", lambda e: canvas.yview_scroll(-1*(e.delta//120), "units"))
    return frame

def styled_entry(parent, width=28, show=None):
    e = tk.Entry(parent, width=width, bg=COLORS["input_bg"], fg=COLORS["text"],
                 insertbackground=COLORS["text"], relief="flat",
                 font=FONTS["body"], bd=0, show=show or "")
    e.configure(highlightthickness=1, highlightbackground=COLORS["border"],
                highlightcolor=COLORS["accent"])
    return e

def styled_combo(parent, values, width=26):
    style = ttk.Style()
    style.configure("Modern.TCombobox",
                    fieldbackground=COLORS["input_bg"],
                    background=COLORS["input_bg"],
                    foreground=COLORS["text"],
                    selectbackground=COLORS["accent"],
                    borderwidth=1, relief="flat")
    c = ttk.Combobox(parent, values=values, width=width, style="Modern.TCombobox",
                     font=FONTS["body"], state="readonly")
    return c

def styled_button(parent, text, cmd, color=None, width=None):
    bg = color or COLORS["accent"]
    kwargs = dict(text=text, command=cmd, bg=bg, fg="#FFFFFF",
                  font=FONTS["subhead"], relief="flat", cursor="hand2",
                  activebackground=COLORS["accent2"], activeforeground="#FFF",
                  padx=16, pady=8, bd=0)
    if width: kwargs["width"] = width
    btn = tk.Button(parent, **kwargs)
    btn.bind("<Enter>", lambda e: btn.config(bg=COLORS["accent2"]))
    btn.bind("<Leave>", lambda e: btn.config(bg=bg))
    return btn

def label_input_row(parent, label_text, widget, row, padx=(0,0)):
    tk.Label(parent, text=label_text, bg=COLORS["card"],
             fg=COLORS["text_muted"], font=FONTS["small"]).grid(
        row=row, column=0, sticky="w", pady=(0,4), padx=padx)
    widget.grid(row=row+1, column=0, sticky="ew", pady=(0,12), padx=padx)

def section_card(parent, title, row=0, col=0, colspan=1, padx=12, pady=8):
    f = tk.Frame(parent, bg=COLORS["card"], bd=0)
    f.grid(row=row, column=col, columnspan=colspan,
           sticky="nsew", padx=padx, pady=pady)
    tk.Label(f, text=title, bg=COLORS["card"], fg=COLORS["text"],
             font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,6))
    tk.Frame(f, bg=COLORS["border"], height=1).pack(fill="x", padx=16)
    return f

def stat_card(parent, label, value, color, row, col):
    f = tk.Frame(parent, bg=COLORS["card"], bd=0)
    f.grid(row=row, column=col, sticky="nsew", padx=8, pady=8)
    tk.Label(f, text=label, bg=COLORS["card"], fg=COLORS["text_muted"],
             font=FONTS["small"]).pack(anchor="w", padx=16, pady=(14,2))
    tk.Label(f, text=value, bg=COLORS["card"], fg=color,
             font=FONTS["stat"]).pack(anchor="w", padx=16, pady=(0,14))
    return f

def build_table(parent, columns, data, col_widths=None):
    style = ttk.Style()
    style.theme_use("default")
    style.configure("Library.Treeview",
        background=COLORS["card"], foreground=COLORS["text"],
        rowheight=34, fieldbackground=COLORS["card"],
        borderwidth=0, font=FONTS["body"])
    style.configure("Library.Treeview.Heading",
        background=COLORS["card2"], foreground=COLORS["text_muted"],
        borderwidth=0, font=FONTS["small"], relief="flat")
    style.map("Library.Treeview",
        background=[("selected", COLORS["active"])],
        foreground=[("selected", COLORS["accent"])])

    frame = tk.Frame(parent, bg=COLORS["card"])
    frame.pack(fill="both", expand=True, padx=16, pady=10)

    tree = ttk.Treeview(frame, columns=columns, show="headings",
                        style="Library.Treeview", selectmode="browse")
    vsb = ttk.Scrollbar(frame, orient="vertical", command=tree.yview)
    tree.configure(yscrollcommand=vsb.set)

    for i, col in enumerate(columns):
        w = (col_widths[i] if col_widths else 120)
        tree.heading(col, text=col)
        tree.column(col, width=w, anchor="w", minwidth=50)

    for idx, row in enumerate(data):
        tag = "odd" if idx % 2 else "even"
        tree.insert("", "end", values=row, tags=(tag,))
    tree.tag_configure("odd",  background=COLORS["row_alt"])
    tree.tag_configure("even", background=COLORS["card"])

    tree.pack(side="left", fill="both", expand=True)
    vsb.pack(side="right", fill="y")
    return tree

# ─────────────────────────── MAIN APP ──────────────────────────────
class LibraryApp(tk.Tk):
    def __init__(self):
        super().__init__()
        init_file()
        self.title("City Library Management System")
        self.geometry("1280x760")
        self.minsize(1100, 680)
        self.configure(bg=COLORS["bg"])
        self.current_page = tk.StringVar(value="dashboard")
        self._build_ui()
        self._show_page("dashboard")

    def _build_ui(self):
        # Sidebar
        self.sidebar = tk.Frame(self, bg=COLORS["sidebar"], width=220)
        self.sidebar.pack(side="left", fill="y")
        self.sidebar.pack_propagate(False)

        # Logo area
        logo_frame = tk.Frame(self.sidebar, bg=COLORS["sidebar"])
        logo_frame.pack(fill="x", pady=(0,0))
        tk.Label(logo_frame, text="📖", bg=COLORS["sidebar"],
                 fg=COLORS["accent"], font=("Segoe UI", 26)).pack(pady=(22,2))
        tk.Label(logo_frame, text="City Library", bg=COLORS["sidebar"],
                 fg=COLORS["text"], font=("Segoe UI", 13, "bold")).pack()
        tk.Label(logo_frame, text="Management System", bg=COLORS["sidebar"],
                 fg=COLORS["text_muted"], font=("Segoe UI", 8)).pack(pady=(0,16))
        tk.Frame(self.sidebar, bg=COLORS["divider"], height=1).pack(fill="x", padx=14)

        # Nav buttons
        self.nav_btns = {}
        nav_wrap = tk.Frame(self.sidebar, bg=COLORS["sidebar"])
        nav_wrap.pack(fill="x", pady=10)
        for icon, label, key in NAV_ITEMS:
            self._make_nav_btn(nav_wrap, icon, label, key)

        # Bottom info
        tk.Frame(self.sidebar, bg=COLORS["divider"], height=1).pack(
            fill="x", padx=14, side="bottom", pady=(0,10))
        tk.Label(self.sidebar, text="v1.0  •  Powered by Python",
                 bg=COLORS["sidebar"], fg=COLORS["text_muted"],
                 font=FONTS["small"]).pack(side="bottom", pady=(0,14))

        # Main content
        self.content = tk.Frame(self, bg=COLORS["bg"])
        self.content.pack(side="left", fill="both", expand=True)

        # Top bar
        topbar = tk.Frame(self.content, bg=COLORS["header_bg"], height=56)
        topbar.pack(fill="x")
        topbar.pack_propagate(False)
        self.page_title = tk.Label(topbar, text="Dashboard",
                                   bg=COLORS["header_bg"], fg=COLORS["text"],
                                   font=FONTS["heading"])
        self.page_title.pack(side="left", padx=24, pady=14)
        tk.Label(topbar, text=f"📅  {date.today().strftime('%B %d, %Y')}",
                 bg=COLORS["header_bg"], fg=COLORS["text_muted"],
                 font=FONTS["body"]).pack(side="right", padx=24)
        tk.Frame(self.content, bg=COLORS["border"], height=1).pack(fill="x")

        # Page container
        self.page_frame = tk.Frame(self.content, bg=COLORS["bg"])
        self.page_frame.pack(fill="both", expand=True)

    def _make_nav_btn(self, parent, icon, label, key):
        f = tk.Frame(parent, bg=COLORS["sidebar"], cursor="hand2")
        f.pack(fill="x", padx=10, pady=2)
        lbl = tk.Label(f, text=f"  {icon}  {label}", bg=COLORS["sidebar"],
                       fg=COLORS["text_muted"], font=FONTS["nav"],
                       anchor="w", padx=8, pady=10, cursor="hand2")
        lbl.pack(fill="x")
        indicator = tk.Frame(f, bg=COLORS["sidebar"], width=4)
        indicator.place(relx=0, rely=0, relheight=1)

        def activate():
            self._show_page(key)

        for w in (f, lbl):
            w.bind("<Button-1>", lambda e, k=key: self._show_page(k))
            w.bind("<Enter>", lambda e, fr=f, l=lbl: (
                fr.config(bg=COLORS["sidebar_hover"]),
                l.config(bg=COLORS["sidebar_hover"])))
            w.bind("<Leave>", lambda e, fr=f, l=lbl, k=key: (
                fr.config(bg=COLORS["active"] if self.current_page.get()==k else COLORS["sidebar"]),
                l.config(bg=COLORS["active"] if self.current_page.get()==k else COLORS["sidebar"],
                         fg=COLORS["active_text"] if self.current_page.get()==k else COLORS["text_muted"])))

        self.nav_btns[key] = (f, lbl, indicator)

    def _show_page(self, key):
        # Reset all nav
        for k, (f, l, ind) in self.nav_btns.items():
            if k == key:
                f.config(bg=COLORS["active"])
                l.config(bg=COLORS["active"], fg=COLORS["active_text"])
                ind.config(bg=COLORS["accent"])
            else:
                f.config(bg=COLORS["sidebar"])
                l.config(bg=COLORS["sidebar"], fg=COLORS["text_muted"])
                ind.config(bg=COLORS["sidebar"])

        self.current_page.set(key)
        titles = {item[2]: item[1] for item in NAV_ITEMS}
        self.page_title.config(text=titles.get(key, key))

        for w in self.page_frame.winfo_children():
            w.destroy()

        pages = {
            "dashboard": self._page_dashboard,
            "books":     self._page_books,
            "members":   self._page_members,
            "borrow":    self._page_borrow,
            "reports":   self._page_reports,
            "search":    self._page_search,
        }
        pages.get(key, self._page_dashboard)()

    # ─────────────── DASHBOARD ───────────────
    def _page_dashboard(self):
        outer = make_scrollable(self.page_frame)
        outer.columnconfigure((0,1,2,3), weight=1)

        books = get_books()
        members = get_members()
        borrows = get_borrows()
        txns = get_transactions()

        total_titles = len(books)
        total_copies = sum(int(b[5]) for b in books)
        active_borrows = len(borrows)
        total_members = len(members)
        premium = sum(1 for m in members if m[4]=="Premium")
        overdue = sum(1 for b in borrows
                      if datetime.strptime(b[5], "%Y-%m-%d").date() < date.today())

        # Stat cards
        stat_card(outer, "Total Book Titles",  str(total_titles),  COLORS["accent"],  0, 0)
        stat_card(outer, "Total Copies",        str(total_copies),  COLORS["success"], 0, 1)
        stat_card(outer, "Active Borrows",      str(active_borrows),COLORS["warning"], 0, 2)
        stat_card(outer, "Total Members",       str(total_members), COLORS["accent2"], 0, 3)

        for c in range(4): outer.columnconfigure(c, weight=1)

        # Quick info row
        f2 = tk.Frame(outer, bg=COLORS["bg"])
        f2.grid(row=1, column=0, columnspan=4, sticky="ew", padx=8, pady=4)
        f2.columnconfigure((0,1,2), weight=1)

        def info_pill(parent, text, col, color):
            tk.Label(parent, text=text, bg=color, fg=COLORS["text"],
                     font=FONTS["body"], padx=14, pady=8).grid(
                row=0, column=col, sticky="ew", padx=6)
        info_pill(f2, f"📗 Premium Members: {premium}",     0, COLORS["tag_prem"])
        info_pill(f2, f"📙 Standard Members: {total_members-premium}", 1, COLORS["tag_std"])
        info_pill(f2, f"⚠️  Overdue Books: {overdue}",      2, COLORS["danger"] if overdue else COLORS["card"])

        # Recent transactions
        card = section_card(outer, "📋  Recent Transactions", row=2, col=0, colspan=4)
        cols = ("TXN ID", "Member ID", "Book ID", "Borrow Date", "Due Date")
        data = [(t[1],t[2],t[3],t[4],t[5]) for t in txns[-10:][::-1]]
        build_table(card, cols, data, [120,110,110,130,130])

        # Genre summary
        card2 = section_card(outer, "📚  Genre Summary", row=3, col=0, colspan=2)
        gcols = ("Genre","Count","% of Collection")
        total_b = len(books) or 1
        gdata = []
        for g in GENRE_LIST:
            n = sum(1 for b in books if b[4]==g)
            gdata.append((g, n, f"{n/total_b*100:.1f}%"))
        build_table(card2, gcols, gdata, [120,80,140])

        # Low stock alert
        card3 = section_card(outer, "⚠️  Low Stock Alert (≤2 copies)", row=3, col=2, colspan=2)
        low = [(b[1],b[2],b[5]) for b in books if int(b[5]) <= 2]
        build_table(card3, ("Book ID","Title","Copies"), low, [90,220,80])

    # ─────────────── BOOKS ───────────────
    def _page_books(self):
        outer = self.page_frame
        outer.columnconfigure(0, weight=1)

        # Toolbar
        toolbar = tk.Frame(outer, bg=COLORS["bg"])
        toolbar.pack(fill="x", padx=16, pady=12)
        styled_button(toolbar, "＋  Add New Book", self._dialog_add_book).pack(side="left")
        styled_button(toolbar, "🗑  Remove Book", self._dialog_remove_book,
                      color=COLORS["danger"]).pack(side="left", padx=8)
        styled_button(toolbar, "🔄  Refresh", self._show_page_books,
                      color=COLORS["card2"]).pack(side="left")

        # Table
        card = tk.Frame(outer, bg=COLORS["card"])
        card.pack(fill="both", expand=True, padx=16, pady=(0,16))
        tk.Label(card, text="All Books", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,4))
        tk.Frame(card, bg=COLORS["border"], height=1).pack(fill="x", padx=16)

        books = get_books()
        cols = ("Book ID","Title","Author","Genre","Copies")
        data = [(b[1],b[2],b[3],b[4],b[5]) for b in books]
        self._books_tree = build_table(card, cols, data, [90,260,180,120,80])

    def _show_page_books(self):
        self._show_page("books")

    def _dialog_add_book(self):
        d = tk.Toplevel(self); d.title("Add New Book")
        d.geometry("420x440"); d.configure(bg=COLORS["card"]); d.resizable(False,False)
        d.grab_set()
        tk.Label(d, text="Add New Book", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["heading"]).pack(pady=(20,4), padx=24, anchor="w")
        tk.Frame(d, bg=COLORS["border"], height=1).pack(fill="x", padx=24)

        frm = tk.Frame(d, bg=COLORS["card"]); frm.pack(fill="both", expand=True, padx=24, pady=16)
        frm.columnconfigure(0, weight=1)

        e_title  = styled_entry(frm)
        e_author = styled_entry(frm)
        e_genre  = styled_combo(frm, GENRE_LIST)
        e_qty    = styled_entry(frm, width=10)

        for i,(lbl,w) in enumerate([("Book Title",e_title),("Author",e_author),
                                     ("Genre",e_genre),("Quantity",e_qty)]):
            label_input_row(frm, lbl, w, i*2)

        def submit():
            t,a,g,q = e_title.get().strip(), e_author.get().strip(), e_genre.get(), e_qty.get().strip()
            if not all([t,a,g,q]):
                messagebox.showerror("Error","All fields required.", parent=d); return
            try: qty = int(q); assert qty > 0
            except: messagebox.showerror("Error","Quantity must be a positive integer.", parent=d); return
            bid = add_book(t,a,g,qty)
            messagebox.showinfo("Success", f"Book added!\nBook ID: {bid}", parent=d)
            d.destroy(); self._show_page("books")

        styled_button(frm, "Add Book", submit).grid(row=8, column=0, sticky="ew", pady=(8,0))

    def _dialog_remove_book(self):
        sel = self._books_tree.selection()
        if not sel:
            messagebox.showwarning("Select Book","Please select a book row first."); return
        row = self._books_tree.item(sel[0])["values"]
        bid = row[0]
        if messagebox.askyesno("Confirm", f"Remove book {bid} – {row[1]}?"):
            remove_book(bid)
            self._show_page("books")

    # ─────────────── MEMBERS ───────────────
    def _page_members(self):
        outer = self.page_frame
        toolbar = tk.Frame(outer, bg=COLORS["bg"])
        toolbar.pack(fill="x", padx=16, pady=12)
        styled_button(toolbar, "＋  Register Member", self._dialog_add_member).pack(side="left")
        styled_button(toolbar, "🗑  Remove Member", self._dialog_remove_member,
                      color=COLORS["danger"]).pack(side="left", padx=8)
        styled_button(toolbar, "🔄  Refresh", lambda: self._show_page("members"),
                      color=COLORS["card2"]).pack(side="left")

        card = tk.Frame(outer, bg=COLORS["card"])
        card.pack(fill="both", expand=True, padx=16, pady=(0,16))
        tk.Label(card, text="All Members", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,4))
        tk.Frame(card, bg=COLORS["border"], height=1).pack(fill="x", padx=16)

        members = get_members()
        cols = ("Member ID","Name","Phone","Type","Borrowed Books")
        data = [(m[1],m[2],m[3],m[4],m[5] or "—") for m in members]
        self._members_tree = build_table(card, cols, data, [100,180,130,100,250])

    def _dialog_add_member(self):
        d = tk.Toplevel(self); d.title("Register Member")
        d.geometry("420x380"); d.configure(bg=COLORS["card"]); d.resizable(False,False)
        d.grab_set()
        tk.Label(d, text="Register New Member", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["heading"]).pack(pady=(20,4), padx=24, anchor="w")
        tk.Frame(d, bg=COLORS["border"], height=1).pack(fill="x", padx=24)

        frm = tk.Frame(d, bg=COLORS["card"]); frm.pack(fill="both", expand=True, padx=24, pady=16)
        frm.columnconfigure(0, weight=1)

        e_name  = styled_entry(frm)
        e_phone = styled_entry(frm)
        e_type  = styled_combo(frm, ["Standard","Premium"])

        for i,(lbl,w) in enumerate([("Full Name",e_name),("Phone Number",e_phone),("Membership Type",e_type)]):
            label_input_row(frm, lbl, w, i*2)

        def submit():
            n,p,t = e_name.get().strip(), e_phone.get().strip(), e_type.get()
            if not all([n,p,t]):
                messagebox.showerror("Error","All fields required.", parent=d); return
            mid = add_member(n,p,t)
            messagebox.showinfo("Success", f"Member registered!\nMember ID: {mid}", parent=d)
            d.destroy(); self._show_page("members")

        styled_button(frm, "Register", submit).grid(row=6, column=0, sticky="ew", pady=(8,0))

    def _dialog_remove_member(self):
        sel = self._members_tree.selection()
        if not sel:
            messagebox.showwarning("Select","Please select a member row first."); return
        row = self._members_tree.item(sel[0])["values"]
        mid = row[0]
        if messagebox.askyesno("Confirm", f"Remove member {mid} – {row[1]}?"):
            remove_member(mid)
            self._show_page("members")

    # ─────────────── BORROW & RETURN ───────────────
    def _page_borrow(self):
        outer = self.page_frame
        outer.columnconfigure((0,1), weight=1)

        # Borrow panel
        bcard = tk.Frame(outer, bg=COLORS["card"])
        bcard.grid(row=0, column=0, sticky="nsew", padx=(16,8), pady=16)
        tk.Label(bcard, text="📤  Borrow a Book", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,4))
        tk.Frame(bcard, bg=COLORS["border"], height=1).pack(fill="x", padx=16)
        bf = tk.Frame(bcard, bg=COLORS["card"]); bf.pack(padx=16, pady=16, fill="x")
        bf.columnconfigure(0, weight=1)

        e_bmid = styled_entry(bf)
        e_bbid = styled_entry(bf)
        label_input_row(bf, "Member ID", e_bmid, 0)
        label_input_row(bf, "Book ID",   e_bbid, 2)

        self._borrow_result = tk.Label(bcard, text="", bg=COLORS["card"],
                                       fg=COLORS["success"], font=FONTS["small"],
                                       wraplength=340, justify="left")
        self._borrow_result.pack(padx=16, pady=(0,4))

        def do_borrow():
            ok, info = borrow_book(e_bmid.get().strip(), e_bbid.get().strip())
            if ok:
                self._borrow_result.config(
                    fg=COLORS["success"],
                    text=(f"✓ Borrowed Successfully!\n"
                          f"TXN: {info['tid']}  |  Member: {info['name']}\n"
                          f"Book: {info['title']}\n"
                          f"Borrow: {info['borrow']}  |  Due: {info['due']}"))
                e_bmid.delete(0,"end"); e_bbid.delete(0,"end")
                self._refresh_borrow_table()
            else:
                self._borrow_result.config(fg=COLORS["danger"], text=f"✗ {info}")

        styled_button(bf, "Borrow Book", do_borrow).grid(row=4, column=0, sticky="ew", pady=(4,0))

        # Return panel
        rcard = tk.Frame(outer, bg=COLORS["card"])
        rcard.grid(row=0, column=1, sticky="nsew", padx=(8,16), pady=16)
        tk.Label(rcard, text="📥  Return a Book", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,4))
        tk.Frame(rcard, bg=COLORS["border"], height=1).pack(fill="x", padx=16)
        rf = tk.Frame(rcard, bg=COLORS["card"]); rf.pack(padx=16, pady=16, fill="x")
        rf.columnconfigure(0, weight=1)

        e_rmid = styled_entry(rf)
        e_rbid = styled_entry(rf)
        label_input_row(rf, "Member ID", e_rmid, 0)
        label_input_row(rf, "Book ID",   e_rbid, 2)

        self._return_result = tk.Label(rcard, text="", bg=COLORS["card"],
                                       fg=COLORS["success"], font=FONTS["small"],
                                       wraplength=340, justify="left")
        self._return_result.pack(padx=16, pady=(0,4))

        def do_return():
            ok, info = return_book(e_rmid.get().strip(), e_rbid.get().strip())
            if ok:
                fine_str = f"Rs. {info['fine']} ({info['late']} days late)" if info['fine'] else "Rs. 0 (on time)"
                self._return_result.config(
                    fg=COLORS["success"] if not info["fine"] else COLORS["warning"],
                    text=(f"✓ Returned Successfully!\n"
                          f"TXN: {info['tid']}\n"
                          f"Borrow: {info['borrow']}  |  Due: {info['due']}\n"
                          f"Return: {info['return']}  |  Fine: {fine_str}"))
                e_rmid.delete(0,"end"); e_rbid.delete(0,"end")
                self._refresh_borrow_table()
            else:
                self._return_result.config(fg=COLORS["danger"], text=f"✗ {info}")

        styled_button(rf, "Return Book", do_return, color=COLORS["success"]).grid(
            row=4, column=0, sticky="ew", pady=(4,0))

        # Active borrows
        btable_card = tk.Frame(outer, bg=COLORS["card"])
        btable_card.grid(row=1, column=0, columnspan=2, sticky="nsew", padx=16, pady=(0,16))
        outer.rowconfigure(1, weight=1)
        tk.Label(btable_card, text="📋  Active Borrows", bg=COLORS["card"], fg=COLORS["text"],
                 font=FONTS["subhead"]).pack(anchor="w", padx=16, pady=(14,4))
        tk.Frame(btable_card, bg=COLORS["border"], height=1).pack(fill="x", padx=16)
        self._btable_host = btable_card
        self._refresh_borrow_table()

    def _refresh_borrow_table(self):
        for w in self._btable_host.winfo_children():
            if isinstance(w, tk.Frame) and hasattr(w,'_is_table'):
                w.destroy()
        borrows = get_borrows()
        today = date.today()
        cols = ("TXN ID","Member ID","Book ID","Borrow Date","Due Date","Status")
        data = []
        for b in borrows:
            due = datetime.strptime(b[5], "%Y-%m-%d").date()
            late = (today - due).days
            status = f"⚠️ {late}d late" if late > 0 else f"✓ {(due-today).days}d left"
            data.append((b[1],b[2],b[3],b[4],b[5],status))
        f = tk.Frame(self._btable_host, bg=COLORS["card"])
        f._is_table = True
        f.pack(fill="both", expand=True)
        build_table(f, cols, data, [120,110,110,120,120,120])

    # ─────────────── REPORTS ───────────────
    def _page_reports(self):
        outer = make_scrollable(self.page_frame)
        outer.columnconfigure((0,1), weight=1)

        books = get_books()
        members = get_members()
        txns = get_transactions()
        borrows = get_borrows()

        # Library summary
        card = section_card(outer,"📊  Library Summary",row=0,col=0,colspan=2)
        total_copies = sum(int(b[5]) for b in books)
        currently_out = len(borrows)
        premium = sum(1 for m in members if m[4]=="Premium")
        rows = [
            ("Total Book Titles",    len(books)),
            ("Total Copies",         total_copies),
            ("Currently Borrowed",   currently_out),
            ("Available Copies",     total_copies - currently_out),
            ("Total Members",        len(members)),
            ("Premium Members",      premium),
            ("Standard Members",     len(members)-premium),
            ("Total Transactions",   len(txns)),
            ("Total Returns",        len(txns) - currently_out),
        ]
        build_table(card, ("Metric","Value"), rows, [260,120])

        # Popular books
        card2 = section_card(outer,"🏆  Most Borrowed Books",row=1,col=0)
        from collections import Counter
        bk_count = Counter(t[3] for t in txns)
        bk_rows = [(bid, cnt) for bid,cnt in bk_count.most_common(10)]
        # enrich with title
        bk_map = {b[1]:b[2] for b in books}
        bk_rows_rich = [(bid, bk_map.get(bid,"—"), cnt) for bid,cnt in bk_rows]
        build_table(card2, ("Book ID","Title","Borrows"), bk_rows_rich, [90,220,80])

        # Active members
        card3 = section_card(outer,"👤  Most Active Members",row=1,col=1)
        mem_count = Counter(t[2] for t in txns)
        mem_map = {m[1]:m[2] for m in members}
        mem_rows = [(mid, mem_map.get(mid,"—"), cnt) for mid,cnt in mem_count.most_common(10)]
        build_table(card3, ("Member ID","Name","Borrows"), mem_rows, [100,180,80])

        # Genre distribution
        card4 = section_card(outer,"📚  Genre Distribution",row=2,col=0)
        total_b = len(books) or 1
        gdata = [(g, sum(1 for b in books if b[4]==g),
                  f"{sum(1 for b in books if b[4]==g)/total_b*100:.1f}%") for g in GENRE_LIST]
        build_table(card4, ("Genre","Books","% Share"), gdata, [120,80,100])

        # Overdue list
        card5 = section_card(outer,"⚠️  Overdue Books",row=2,col=1)
        today = date.today()
        overdue = [(b[1],b[2],b[3],b[5],f"{(today-datetime.strptime(b[5],'%Y-%m-%d').date()).days}d")
                   for b in borrows
                   if datetime.strptime(b[5], "%Y-%m-%d").date() < today]
        build_table(card5, ("TXN","Member","Book","Due","Days Late"), overdue, [100,90,90,110,80])

    # ─────────────── SEARCH ───────────────
    def _page_search(self):
        outer = self.page_frame
        outer.columnconfigure(0, weight=1)
        outer.rowconfigure(1, weight=1)

        # Search bar
        bar = tk.Frame(outer, bg=COLORS["card"])
        bar.pack(fill="x", padx=16, pady=16)
        bar.columnconfigure(1, weight=1)

        tk.Label(bar, text="Search:", bg=COLORS["card"], fg=COLORS["text_muted"],
                 font=FONTS["body"]).grid(row=0, column=0, padx=(16,8), pady=12)
        self._search_var = tk.StringVar()
        e = tk.Entry(bar, textvariable=self._search_var, bg=COLORS["input_bg"],
                     fg=COLORS["text"], insertbackground=COLORS["text"],
                     relief="flat", font=FONTS["body"], bd=0)
        e.configure(highlightthickness=1, highlightbackground=COLORS["border"],
                    highlightcolor=COLORS["accent"])
        e.grid(row=0, column=1, sticky="ew", pady=12, padx=(0,8), ipady=6)

        self._search_type = styled_combo(bar, ["Books – Title","Books – Author",
                                               "Books – Genre","Members – Name"], width=20)
        self._search_type.current(0)
        self._search_type.grid(row=0, column=2, padx=(0,8), pady=12)
        styled_button(bar, "Search", self._do_search).grid(row=0, column=3, padx=(0,16), pady=12)

        # Results
        self._search_card = tk.Frame(outer, bg=COLORS["card"])
        self._search_card.pack(fill="both", expand=True, padx=16, pady=(0,16))
        tk.Label(self._search_card, text="Results will appear here", bg=COLORS["card"],
                 fg=COLORS["text_muted"], font=FONTS["body"]).pack(pady=40)

        e.bind("<Return>", lambda e: self._do_search())

    def _do_search(self):
        q = self._search_var.get().strip().lower()
        stype = self._search_type.get()
        for w in self._search_card.winfo_children():
            w.destroy()
        tk.Label(self._search_card, text=f'Results for "{q}"',
                 bg=COLORS["card"], fg=COLORS["text_muted"],
                 font=FONTS["small"]).pack(anchor="w", padx=16, pady=(12,4))
        tk.Frame(self._search_card, bg=COLORS["border"], height=1).pack(fill="x", padx=16)

        if "Book" in stype:
            field_map = {"Books – Title":2, "Books – Author":3, "Books – Genre":4}
            fi = field_map.get(stype, 2)
            rows = [(b[1],b[2],b[3],b[4],b[5]) for b in get_books() if q in b[fi].lower()]
            build_table(self._search_card, ("Book ID","Title","Author","Genre","Copies"),
                        rows, [90,240,180,120,80])
        else:
            rows = [(m[1],m[2],m[3],m[4],m[5] or "—") for m in get_members() if q in m[2].lower()]
            build_table(self._search_card, ("Member ID","Name","Phone","Type","Borrowed"),
                        rows, [100,180,130,100,220])

        if not rows:
            tk.Label(self._search_card, text="No results found.",
                     bg=COLORS["card"], fg=COLORS["text_muted"],
                     font=FONTS["body"]).pack(pady=20)


# ─────────────────────────── RUN ───────────────────────────────────
if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
