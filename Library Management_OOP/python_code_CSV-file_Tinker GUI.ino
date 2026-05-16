import csv
import os
import tkinter as tk
from tkinter import ttk, messagebox, font
from datetime import date, timedelta, datetime

# ─────────────────────────────────────────────
#  CSV FILE PATHS
# ─────────────────────────────────────────────
BOOKS_FILE          = "books.csv"
MEMBERS_FILE        = "members.csv"
TRANSACTIONS_FILE   = "transactions.csv"
BORROW_RECORDS_FILE = "borrow_records.csv"

BOOKS_HEADERS          = ["book_id", "title", "author", "genre", "quantity", "available_quantity", "borrow_quantity"]
MEMBERS_HEADERS        = ["member_id", "name", "phone", "type", "borrowed_books"]
TRANSACTIONS_HEADERS   = ["Transaction_ID", "member_id", "book_id", "Borrow_Date", "Due_Date"]
BORROW_RECORDS_HEADERS = ["Transaction_ID", "member_id", "book_id", "Borrow_Date", "Due_Date"]

# ─────────────────────────────────────────────
#  THEME / PALETTE
# ─────────────────────────────────────────────
C = {
    "bg":        "#0F1117",
    "sidebar":   "#161B27",
    "card":      "#1C2333",
    "card2":     "#212940",
    "border":    "#2A3352",
    "accent":    "#4F8EF7",
    "accent2":   "#6C63FF",
    "green":     "#2DD4BF",
    "red":       "#F87171",
    "yellow":    "#FBBF24",
    "text":      "#E8EDF5",
    "text2":     "#8B9BBE",
    "text3":     "#4A5578",
    "white":     "#FFFFFF",
    "hover":     "#253047",
    "selected":  "#1A2D55",
    "input_bg":  "#141925",
    "input_bdr": "#2E3D5C",
}

FONT_FAMILY = "Segoe UI"

# ─────────────────────────────────────────────
#  CSV HELPERS
# ─────────────────────────────────────────────
def init_csv(filepath, headers):
    if not os.path.exists(filepath):
        with open(filepath, "w", newline="") as f:
            csv.DictWriter(f, fieldnames=headers).writeheader()

def read_csv(filepath, headers):
    init_csv(filepath, headers)
    with open(filepath, "r", newline="") as f:
        return list(csv.DictReader(f))

def write_csv(filepath, headers, rows):
    with open(filepath, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=headers)
        w.writeheader(); w.writerows(rows)

def append_csv(filepath, headers, row_dict):
    init_csv(filepath, headers)
    with open(filepath, "a", newline="") as f:
        csv.DictWriter(f, fieldnames=headers).writerow(row_dict)

# ─────────────────────────────────────────────
#  DATA MODELS
# ─────────────────────────────────────────────
class Book:
    def __init__(self, book_id, title, author, genre, quantity, available_quantity, borrow_quantity):
        self.book_id = book_id; self.title = title; self.author = author
        self.genre = genre; self.quantity = int(quantity)
        self.available_quantity = int(available_quantity); self.borrow_quantity = int(borrow_quantity)
    def to_dict(self):
        return {"book_id": self.book_id, "title": self.title, "author": self.author,
                "genre": self.genre, "quantity": self.quantity,
                "available_quantity": self.available_quantity, "borrow_quantity": self.borrow_quantity}

class Member:
    def __init__(self, member_id, name, phone, type_, borrowed_books=None):
        self.member_id = member_id; self.name = name; self.phone = phone; self.type = type_
        if borrowed_books is None: self.borrowed_books = []
        elif isinstance(borrowed_books, list): self.borrowed_books = borrowed_books
        else: self.borrowed_books = [b for b in borrowed_books.split("|") if b]
    def to_dict(self):
        return {"member_id": self.member_id, "name": self.name, "phone": self.phone,
                "type": self.type, "borrowed_books": "|".join(self.borrowed_books)}

class Transaction:
    def __init__(self, tid, member_id, book_id, borrow_date, due_date):
        self.Transaction_ID = tid; self.member_id = member_id; self.book_id = book_id
        self.Borrow_Date = borrow_date; self.Due_Date = due_date
    def to_dict(self):
        return {"Transaction_ID": self.Transaction_ID, "member_id": self.member_id,
                "book_id": self.book_id, "Borrow_Date": str(self.Borrow_Date), "Due_Date": str(self.Due_Date)}

class BorrowRecord:
    def __init__(self, tid, member_id, book_id, borrow_date, due_date):
        self.Transaction_ID = tid; self.member_id = member_id; self.book_id = book_id
        self.Borrow_Date = borrow_date; self.Due_Date = due_date
    def to_dict(self):
        return {"Transaction_ID": self.Transaction_ID, "member_id": self.member_id,
                "book_id": self.book_id, "Borrow_Date": str(self.Borrow_Date), "Due_Date": str(self.Due_Date)}

# ─────────────────────────────────────────────
#  LOAD / SAVE
# ─────────────────────────────────────────────
def load_books():
    return [Book(r["book_id"], r["title"], r["author"], r["genre"],
                 r["quantity"], r["available_quantity"], r["borrow_quantity"])
            for r in read_csv(BOOKS_FILE, BOOKS_HEADERS)]

def save_books(books):
    write_csv(BOOKS_FILE, BOOKS_HEADERS, [b.to_dict() for b in books])

def load_members():
    return [Member(r["member_id"], r["name"], r["phone"], r["type"], r["borrowed_books"])
            for r in read_csv(MEMBERS_FILE, MEMBERS_HEADERS)]

def save_members(members):
    write_csv(MEMBERS_FILE, MEMBERS_HEADERS, [m.to_dict() for m in members])

def load_transactions():
    out = []
    for r in read_csv(TRANSACTIONS_FILE, TRANSACTIONS_HEADERS):
        out.append(Transaction(r["Transaction_ID"], r["member_id"], r["book_id"],
                               datetime.strptime(r["Borrow_Date"], "%Y-%m-%d").date(),
                               datetime.strptime(r["Due_Date"],    "%Y-%m-%d").date()))
    return out

def save_transactions(transactions):
    write_csv(TRANSACTIONS_FILE, TRANSACTIONS_HEADERS, [t.to_dict() for t in transactions])

def load_borrow_records():
    out = []
    for r in read_csv(BORROW_RECORDS_FILE, BORROW_RECORDS_HEADERS):
        out.append(BorrowRecord(r["Transaction_ID"], r["member_id"], r["book_id"],
                                datetime.strptime(r["Borrow_Date"], "%Y-%m-%d").date(),
                                datetime.strptime(r["Due_Date"],    "%Y-%m-%d").date()))
    return out

def save_borrow_records(brs):
    write_csv(BORROW_RECORDS_FILE, BORROW_RECORDS_HEADERS, [b.to_dict() for b in brs])

# ─────────────────────────────────────────────
#  REUSABLE WIDGETS
# ─────────────────────────────────────────────
def styled_label(parent, text, size=11, color=None, bold=False, **kw):
    weight = "bold" if bold else "normal"
    lbl = tk.Label(parent, text=text, bg=parent["bg"] if "bg" not in kw else kw.pop("bg"),
                   fg=color or C["text"], font=(FONT_FAMILY, size, weight), **kw)
    return lbl

def styled_entry(parent, textvariable=None, width=28):
    e = tk.Entry(parent, textvariable=textvariable, width=width,
                 bg=C["input_bg"], fg=C["text"], insertbackground=C["accent"],
                 relief="flat", font=(FONT_FAMILY, 11),
                 highlightthickness=1, highlightcolor=C["accent"],
                 highlightbackground=C["input_bdr"])
    return e

def styled_combo(parent, values, textvariable=None, width=26):
    style = ttk.Style()
    style.theme_use("clam")
    style.configure("Custom.TCombobox",
                    fieldbackground=C["input_bg"], background=C["card2"],
                    foreground=C["text"], selectbackground=C["accent"],
                    bordercolor=C["input_bdr"], arrowcolor=C["accent"],
                    lightcolor=C["input_bdr"], darkcolor=C["input_bdr"])
    cb = ttk.Combobox(parent, values=values, textvariable=textvariable,
                      width=width, style="Custom.TCombobox", font=(FONT_FAMILY, 11), state="readonly")
    return cb

def styled_btn(parent, text, command, color=None, text_color=None, width=None, pady=8):
    bg = color or C["accent"]
    fg = text_color or C["white"]
    kw = {}
    if width: kw["width"] = width
    btn = tk.Button(parent, text=text, command=command, bg=bg, fg=fg,
                    relief="flat", font=(FONT_FAMILY, 11, "bold"),
                    activebackground=C["accent2"], activeforeground=C["white"],
                    cursor="hand2", pady=pady, **kw)
    btn.bind("<Enter>", lambda e: btn.config(bg=C["accent2"] if bg == C["accent"] else bg))
    btn.bind("<Leave>", lambda e: btn.config(bg=bg))
    return btn

def make_table(parent, columns, col_widths=None):
    style = ttk.Style()
    style.theme_use("clam")
    style.configure("Dark.Treeview",
                    background=C["card"], foreground=C["text"],
                    fieldbackground=C["card"], rowheight=36,
                    borderwidth=0, font=(FONT_FAMILY, 10))
    style.configure("Dark.Treeview.Heading",
                    background=C["card2"], foreground=C["accent"],
                    font=(FONT_FAMILY, 10, "bold"), relief="flat", borderwidth=0)
    style.map("Dark.Treeview",
              background=[("selected", C["selected"])],
              foreground=[("selected", C["white"])])
    style.map("Dark.Treeview.Heading", background=[("active", C["hover"])])

    frame = tk.Frame(parent, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
    vsb = tk.Scrollbar(frame, orient="vertical", bg=C["card2"], troughcolor=C["card"])
    hsb = tk.Scrollbar(frame, orient="horizontal", bg=C["card2"], troughcolor=C["card"])
    tree = ttk.Treeview(frame, columns=columns, show="headings",
                        style="Dark.Treeview", yscrollcommand=vsb.set, xscrollcommand=hsb.set)
    vsb.config(command=tree.yview); hsb.config(command=tree.xview)
    vsb.pack(side="right", fill="y"); hsb.pack(side="bottom", fill="x")
    tree.pack(fill="both", expand=True)

    for i, col in enumerate(columns):
        tree.heading(col, text=col, anchor="w")
        w = col_widths[i] if col_widths else 120
        tree.column(col, width=w, minwidth=60, anchor="w")

    # Alternating row colors
    tree.tag_configure("odd",  background=C["card"])
    tree.tag_configure("even", background=C["card2"])

    return frame, tree

def stat_card(parent, title, value, color, icon=""):
    card = tk.Frame(parent, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
    card.pack_propagate(False)
    card.config(width=160, height=110)

    accent_bar = tk.Frame(card, bg=color, width=4)
    accent_bar.pack(side="left", fill="y")

    inner = tk.Frame(card, bg=C["card"])
    inner.pack(side="left", fill="both", expand=True, padx=14, pady=14)

    tk.Label(inner, text=icon + " " + title if icon else title,
             bg=C["card"], fg=C["text2"], font=(FONT_FAMILY, 9)).pack(anchor="w")
    tk.Label(inner, text=str(value), bg=C["card"], fg=color,
             font=(FONT_FAMILY, 26, "bold")).pack(anchor="w", pady=(4, 0))
    return card

# ─────────────────────────────────────────────
#  MAIN APPLICATION
# ─────────────────────────────────────────────
class LibraryApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("📚  Library Management System")
        self.geometry("1280x760")
        self.minsize(1100, 680)
        self.configure(bg=C["bg"])
        self.resizable(True, True)

        for f in [BOOKS_FILE, MEMBERS_FILE, TRANSACTIONS_FILE, BORROW_RECORDS_FILE]:
            init_csv(f, globals()[f.replace(".csv","").upper()+"_HEADERS"])

        self._build_layout()
        self._show_section("dashboard")

    # ── Layout ──────────────────────────────
    def _build_layout(self):
        # Sidebar
        self.sidebar = tk.Frame(self, bg=C["sidebar"], width=220)
        self.sidebar.pack(side="left", fill="y")
        self.sidebar.pack_propagate(False)

        # Logo area
        logo_frame = tk.Frame(self.sidebar, bg=C["sidebar"], pady=24)
        logo_frame.pack(fill="x")
        tk.Label(logo_frame, text="📚", bg=C["sidebar"], font=("Segoe UI Emoji", 28)).pack()
        tk.Label(logo_frame, text="LibraryOS", bg=C["sidebar"], fg=C["white"],
                 font=(FONT_FAMILY, 15, "bold")).pack()
        tk.Label(logo_frame, text="Management System", bg=C["sidebar"], fg=C["text3"],
                 font=(FONT_FAMILY, 8)).pack()

        # Separator
        tk.Frame(self.sidebar, bg=C["border"], height=1).pack(fill="x", padx=16, pady=4)

        # Nav items
        self._nav_btns = {}
        nav_items = [
            ("dashboard",   "⊞",  "Dashboard"),
            ("books",       "▣",  "Book Management"),
            ("members",     "◉",  "Member Management"),
            ("borrow",      "↗",  "Borrow & Return"),
            ("reports",     "◈",  "Reports & Stats"),
            ("search",      "⊕",  "Search"),
        ]
        for key, icon, label in nav_items:
            btn = self._nav_button(key, icon, label)
            self._nav_btns[key] = btn

        # Bottom info
        tk.Frame(self.sidebar, bg=C["border"], height=1).pack(fill="x", padx=16, pady=8, side="bottom")
        tk.Label(self.sidebar, text=f"© {date.today().year}  LibraryOS",
                 bg=C["sidebar"], fg=C["text3"], font=(FONT_FAMILY, 8)).pack(side="bottom", pady=8)

        # Main content
        self.main = tk.Frame(self, bg=C["bg"])
        self.main.pack(side="left", fill="both", expand=True)

        # Top bar
        topbar = tk.Frame(self.main, bg=C["sidebar"], height=52)
        topbar.pack(fill="x"); topbar.pack_propagate(False)
        self._page_title = tk.Label(topbar, text="Dashboard", bg=C["sidebar"], fg=C["white"],
                                    font=(FONT_FAMILY, 14, "bold"))
        self._page_title.pack(side="left", padx=24, pady=14)
        tk.Label(topbar, text=f"📅  {date.today().strftime('%B %d, %Y')}",
                 bg=C["sidebar"], fg=C["text2"], font=(FONT_FAMILY, 10)).pack(side="right", padx=24)

        # Content area (scrollable)
        self.canvas = tk.Canvas(self.main, bg=C["bg"], highlightthickness=0)
        self.scrollbar = tk.Scrollbar(self.main, orient="vertical", command=self.canvas.yview)
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.scrollbar.pack(side="right", fill="y")
        self.canvas.pack(fill="both", expand=True)
        self.content = tk.Frame(self.canvas, bg=C["bg"])
        self._canvas_window = self.canvas.create_window((0, 0), window=self.content, anchor="nw")
        self.content.bind("<Configure>", self._on_frame_configure)
        self.canvas.bind("<Configure>", self._on_canvas_configure)
        self.canvas.bind_all("<MouseWheel>", self._on_mousewheel)

    def _on_frame_configure(self, e):
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def _on_canvas_configure(self, e):
        self.canvas.itemconfig(self._canvas_window, width=e.width)

    def _on_mousewheel(self, e):
        self.canvas.yview_scroll(int(-1 * (e.delta / 120)), "units")

    def _nav_button(self, key, icon, label):
        f = tk.Frame(self.sidebar, bg=C["sidebar"], cursor="hand2")
        f.pack(fill="x", pady=1)
        inner = tk.Frame(f, bg=C["sidebar"])
        inner.pack(fill="x", padx=10, pady=2)
        icon_lbl = tk.Label(inner, text=icon, bg=C["sidebar"], fg=C["text2"],
                            font=(FONT_FAMILY, 13), width=2)
        icon_lbl.pack(side="left", padx=(8, 6), pady=8)
        txt_lbl = tk.Label(inner, text=label, bg=C["sidebar"], fg=C["text2"],
                           font=(FONT_FAMILY, 10), anchor="w")
        txt_lbl.pack(side="left", fill="x", expand=True)
        ind = tk.Frame(f, bg=C["sidebar"], width=3)
        ind.pack(side="right", fill="y")

        def on_click(k=key, fi=f, ii=inner, il=icon_lbl, tl=txt_lbl, ind=ind):
            self._show_section(k)

        for w in [f, inner, icon_lbl, txt_lbl]:
            w.bind("<Button-1>", lambda e, k=key: self._show_section(k))
            w.bind("<Enter>", lambda e, fi=inner: fi.config(bg=C["hover"]) or
                   [c.config(bg=C["hover"]) for c in fi.winfo_children()])
            w.bind("<Leave>", lambda e, fi=inner, k=key: None)

        return {"frame": f, "inner": inner, "icon": icon_lbl, "text": txt_lbl, "ind": ind}

    def _set_nav_active(self, key):
        for k, widgets in self._nav_btns.items():
            if k == key:
                widgets["inner"].config(bg=C["selected"])
                for c in widgets["inner"].winfo_children():
                    c.config(bg=C["selected"], fg=C["accent"])
                widgets["ind"].config(bg=C["accent"])
            else:
                widgets["inner"].config(bg=C["sidebar"])
                for c in widgets["inner"].winfo_children():
                    c.config(bg=C["sidebar"], fg=C["text2"])
                widgets["ind"].config(bg=C["sidebar"])

    def _clear_content(self):
        for w in self.content.winfo_children():
            w.destroy()
        self.canvas.yview_moveto(0)

    def _show_section(self, key):
        self._clear_content()
        self._set_nav_active(key)
        titles = {"dashboard": "Dashboard", "books": "Book Management",
                  "members": "Member Management", "borrow": "Borrow & Return",
                  "reports": "Reports & Statistics", "search": "Search"}
        self._page_title.config(text=titles.get(key, key.title()))
        getattr(self, f"_page_{key}")()

    # ─────────────────────────────────────────
    #  DASHBOARD
    # ─────────────────────────────────────────
    def _page_dashboard(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        books   = load_books()
        members = load_members()
        txns    = load_transactions()
        brs     = load_borrow_records()

        total_books    = sum(b.quantity for b in books)
        avail_books    = sum(b.available_quantity for b in books)
        active_borrow  = len(brs)
        overdue_count  = sum(1 for br in brs if br.Due_Date < date.today())

        # Stat cards row
        tk.Label(pad, text="Overview", bg=C["bg"], fg=C["text2"],
                 font=(FONT_FAMILY, 10, "bold")).pack(anchor="w", pady=(0, 10))
        cards_row = tk.Frame(pad, bg=C["bg"])
        cards_row.pack(fill="x")

        stats = [
            ("Book Titles",    len(books),     C["accent"],  "◉"),
            ("Total Copies",   total_books,    C["accent2"], "▣"),
            ("Available",      avail_books,    C["green"],   "✓"),
            ("Active Loans",   active_borrow,  C["yellow"],  "↗"),
            ("Overdue",        overdue_count,  C["red"],     "!"),
            ("Members",        len(members),   C["accent"],  "◈"),
        ]
        for title, val, color, icon in stats:
            c = stat_card(cards_row, title, val, color, icon)
            c.pack(side="left", padx=(0, 12), pady=4)

        # Recent Activity + Genre Split row
        tk.Frame(pad, bg=C["border"], height=1).pack(fill="x", pady=20)
        bottom_row = tk.Frame(pad, bg=C["bg"])
        bottom_row.pack(fill="both", expand=True)

        # Recent transactions table
        left = tk.Frame(bottom_row, bg=C["bg"])
        left.pack(side="left", fill="both", expand=True, padx=(0, 12))
        tk.Label(left, text="Recent Transactions", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        cols = ["TxnID", "Member", "Book", "Borrow Date", "Due Date"]
        widths = [90, 90, 90, 110, 110]
        tframe, tree = make_table(left, cols, widths)
        tframe.pack(fill="both", expand=True)
        recent = sorted(txns, key=lambda t: str(t.Borrow_Date), reverse=True)[:10]
        for i, t in enumerate(recent):
            tag = "odd" if i % 2 == 0 else "even"
            tree.insert("", "end", values=(t.Transaction_ID, t.member_id, t.book_id,
                                           str(t.Borrow_Date), str(t.Due_Date)), tags=(tag,))

        # Genre distribution
        right = tk.Frame(bottom_row, bg=C["bg"], width=260)
        right.pack(side="left", fill="y")
        right.pack_propagate(False)
        tk.Label(right, text="Genre Distribution", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        genres = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        colors = [C["accent"], C["accent2"], C["green"], C["yellow"], C["red"]]
        total_titles = len(books) or 1
        for g, col in zip(genres, colors):
            cnt = sum(1 for b in books if b.genre == g)
            pct = int(cnt / total_titles * 100)
            row = tk.Frame(right, bg=C["bg"])
            row.pack(fill="x", pady=4)
            tk.Label(row, text=g, bg=C["bg"], fg=C["text2"],
                     font=(FONT_FAMILY, 9), width=12, anchor="w").pack(side="left")
            bar_bg = tk.Frame(row, bg=C["card"], height=12, relief="flat")
            bar_bg.pack(side="left", fill="x", expand=True, padx=6)
            bar_bg.update_idletasks()
            bar_fill = tk.Frame(bar_bg, bg=col, height=12)
            bar_fill.place(x=0, y=0, relwidth=max(0.02, pct/100), relheight=1)
            tk.Label(row, text=f"{cnt}", bg=C["bg"], fg=col,
                     font=(FONT_FAMILY, 9, "bold"), width=3).pack(side="left")

        # Overdue table
        if overdue_count:
            tk.Frame(pad, bg=C["border"], height=1).pack(fill="x", pady=20)
            tk.Label(pad, text=f"⚠  Overdue Books  ({overdue_count})", bg=C["bg"], fg=C["red"],
                     font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
            ocols = ["TxnID", "Member", "Book", "Due Date", "Days Overdue", "Fine (Rs)"]
            owidths = [90, 90, 90, 110, 120, 100]
            of, otree = make_table(pad, ocols, owidths)
            of.pack(fill="x")
            for i, br in enumerate([b for b in brs if b.Due_Date < date.today()]):
                od = (date.today() - br.Due_Date).days
                tag = "odd" if i % 2 == 0 else "even"
                otree.insert("", "end",
                             values=(br.Transaction_ID, br.member_id, br.book_id,
                                     str(br.Due_Date), od, od * 10), tags=(tag,))

    # ─────────────────────────────────────────
    #  BOOKS
    # ─────────────────────────────────────────
    def _page_books(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        # ── Add book form ──
        form_card = tk.Frame(pad, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
        form_card.pack(fill="x", pady=(0, 20))
        header = tk.Frame(form_card, bg=C["card2"])
        header.pack(fill="x")
        tk.Label(header, text=" ▣  Add New Book", bg=C["card2"], fg=C["accent"],
                 font=(FONT_FAMILY, 11, "bold")).pack(side="left", padx=16, pady=10)

        fields_frame = tk.Frame(form_card, bg=C["card"], pady=16)
        fields_frame.pack(fill="x", padx=16)

        v_title  = tk.StringVar()
        v_author = tk.StringVar()
        v_qty    = tk.StringVar()
        v_genre  = tk.StringVar()

        row1 = tk.Frame(fields_frame, bg=C["card"])
        row1.pack(fill="x", pady=4)
        for lbl, var, col_num in [("Title", v_title, 0), ("Author", v_author, 2)]:
            tk.Label(row1, text=lbl, bg=C["card"], fg=C["text2"],
                     font=(FONT_FAMILY, 9)).grid(row=0, column=col_num, sticky="w", padx=(0,6))
            styled_entry(row1, textvariable=var, width=30).grid(row=0, column=col_num+1, padx=(0,20))

        row2 = tk.Frame(fields_frame, bg=C["card"])
        row2.pack(fill="x", pady=4)
        tk.Label(row2, text="Quantity", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).grid(row=0, column=0, sticky="w", padx=(0,6))
        styled_entry(row2, textvariable=v_qty, width=10).grid(row=0, column=1, padx=(0,20))
        tk.Label(row2, text="Genre", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).grid(row=0, column=2, sticky="w", padx=(0,6))
        styled_combo(row2, ["Fiction","Non-Fiction","Science","History","Technology"],
                     textvariable=v_genre, width=20).grid(row=0, column=3, padx=(0,20))

        def add_book():
            title  = v_title.get().strip()
            author = v_author.get().strip()
            genre  = v_genre.get().strip()
            try: qty = int(v_qty.get())
            except: return messagebox.showerror("Error", "Quantity must be a number.")
            if not all([title, author, genre, qty]):
                return messagebox.showerror("Error", "All fields are required.")
            books = load_books()
            book_id = f"BK{len(books)+1:03d}"
            books.append(Book(book_id, title, author, genre, qty, qty, 0))
            save_books(books)
            for v in [v_title, v_author, v_qty, v_genre]: v.set("")
            refresh_table()
            messagebox.showinfo("Success", f"Book '{title}' added! ID: {book_id}")

        btn_row = tk.Frame(form_card, bg=C["card"], pady=12)
        btn_row.pack(fill="x", padx=16)
        styled_btn(btn_row, "  + Add Book  ", add_book).pack(side="left")

        # ── Books table ──
        tk.Label(pad, text="All Books", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))

        # Filter bar
        filt_row = tk.Frame(pad, bg=C["bg"])
        filt_row.pack(fill="x", pady=(0, 8))
        v_search = tk.StringVar()
        styled_entry(filt_row, textvariable=v_search, width=30).pack(side="left")
        tk.Label(filt_row, text="  Search by title/author/genre…",
                 bg=C["bg"], fg=C["text3"], font=(FONT_FAMILY, 9)).pack(side="left")

        cols = ["ID", "Title", "Author", "Genre", "Total", "Available", "Borrowed"]
        widths = [80, 200, 160, 110, 70, 90, 90]
        tframe, tree = make_table(pad, cols, widths)
        tframe.pack(fill="both", expand=True)

        def refresh_table(query=""):
            for row in tree.get_children(): tree.delete(row)
            q = query.lower()
            for i, b in enumerate(load_books()):
                if q and not any(q in x.lower() for x in [b.title, b.author, b.genre]): continue
                tag = "odd" if i % 2 == 0 else "even"
                avail_color = "green" if b.available_quantity > 0 else "red"
                tree.insert("", "end", values=(b.book_id, b.title, b.author, b.genre,
                                               b.quantity, b.available_quantity, b.borrow_quantity),
                            tags=(tag,))

        v_search.trace("w", lambda *a: refresh_table(v_search.get()))
        refresh_table()

        # Remove book
        def remove_selected():
            sel = tree.selection()
            if not sel: return messagebox.showwarning("Select", "Select a book to remove.")
            book_id = tree.item(sel[0])["values"][0]
            if messagebox.askyesno("Confirm", f"Remove book {book_id}?"):
                books = [b for b in load_books() if b.book_id != book_id]
                save_books(books); refresh_table()
                messagebox.showinfo("Removed", f"Book {book_id} removed.")

        btn_row2 = tk.Frame(pad, bg=C["bg"])
        btn_row2.pack(fill="x", pady=8)
        styled_btn(btn_row2, "  🗑 Remove Selected  ", remove_selected,
                   color=C["red"]).pack(side="left")
        styled_btn(btn_row2, "  ↺ Refresh  ", lambda: refresh_table(v_search.get()),
                   color=C["card2"]).pack(side="left", padx=8)

    # ─────────────────────────────────────────
    #  MEMBERS
    # ─────────────────────────────────────────
    def _page_members(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        form_card = tk.Frame(pad, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
        form_card.pack(fill="x", pady=(0, 20))
        tk.Frame(form_card, bg=C["card2"]).pack(fill="x")
        header = tk.Frame(form_card, bg=C["card2"])
        header.pack(fill="x")
        tk.Label(header, text=" ◉  Register New Member", bg=C["card2"], fg=C["accent"],
                 font=(FONT_FAMILY, 11, "bold")).pack(side="left", padx=16, pady=10)

        fields = tk.Frame(form_card, bg=C["card"], pady=16)
        fields.pack(fill="x", padx=16)
        v_name  = tk.StringVar()
        v_phone = tk.StringVar()
        v_type  = tk.StringVar(value="Standard")

        row = tk.Frame(fields, bg=C["card"])
        row.pack(fill="x")
        tk.Label(row, text="Name", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).grid(row=0, column=0, sticky="w", padx=(0,6))
        styled_entry(row, textvariable=v_name, width=28).grid(row=0, column=1, padx=(0,20))
        tk.Label(row, text="Phone", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).grid(row=0, column=2, sticky="w", padx=(0,6))
        styled_entry(row, textvariable=v_phone, width=20).grid(row=0, column=3, padx=(0,20))
        tk.Label(row, text="Type", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).grid(row=0, column=4, sticky="w", padx=(0,6))
        styled_combo(row, ["Standard","Premium"], textvariable=v_type, width=14).grid(row=0, column=5)

        def add_member():
            name  = v_name.get().strip()
            phone = v_phone.get().strip()
            mtype = v_type.get().strip()
            if not all([name, phone, mtype]):
                return messagebox.showerror("Error", "All fields are required.")
            members  = load_members()
            mem_id   = f"MEM{len(members)+1:03d}"
            members.append(Member(mem_id, name, phone, mtype, []))
            save_members(members)
            v_name.set(""); v_phone.set("")
            refresh_table()
            messagebox.showinfo("Success", f"Member '{name}' registered! ID: {mem_id}")

        tk.Frame(form_card, bg=C["card"], pady=4).pack()
        btn_row = tk.Frame(form_card, bg=C["card"])
        btn_row.pack(fill="x", padx=16, pady=(0, 14))
        styled_btn(btn_row, "  + Register  ", add_member).pack(side="left")

        # Table
        tk.Label(pad, text="All Members", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        v_search = tk.StringVar()
        filt = tk.Frame(pad, bg=C["bg"])
        filt.pack(fill="x", pady=(0, 8))
        styled_entry(filt, textvariable=v_search, width=30).pack(side="left")
        tk.Label(filt, text="  Search by name…", bg=C["bg"], fg=C["text3"],
                 font=(FONT_FAMILY, 9)).pack(side="left")

        cols = ["ID", "Name", "Phone", "Type", "Books Borrowed"]
        widths = [90, 180, 130, 100, 130]
        tf, tree = make_table(pad, cols, widths)
        tf.pack(fill="both", expand=True)

        def refresh_table(query=""):
            for row in tree.get_children(): tree.delete(row)
            q = query.lower()
            for i, m in enumerate(load_members()):
                if q and q not in m.name.lower(): continue
                tag = "odd" if i % 2 == 0 else "even"
                tree.insert("", "end",
                            values=(m.member_id, m.name, m.phone, m.type, len(m.borrowed_books)),
                            tags=(tag,))

        v_search.trace("w", lambda *a: refresh_table(v_search.get()))
        refresh_table()

        def remove_selected():
            sel = tree.selection()
            if not sel: return messagebox.showwarning("Select", "Select a member to remove.")
            mid = tree.item(sel[0])["values"][0]
            if messagebox.askyesno("Confirm", f"Remove member {mid}?"):
                members = [m for m in load_members() if m.member_id != mid]
                save_members(members); refresh_table()

        btn_row2 = tk.Frame(pad, bg=C["bg"])
        btn_row2.pack(fill="x", pady=8)
        styled_btn(btn_row2, "  🗑 Remove Selected  ", remove_selected, color=C["red"]).pack(side="left")
        styled_btn(btn_row2, "  ↺ Refresh  ", lambda: refresh_table(v_search.get()),
                   color=C["card2"]).pack(side="left", padx=8)

    # ─────────────────────────────────────────
    #  BORROW & RETURN
    # ─────────────────────────────────────────
    def _page_borrow(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        # Two side-by-side cards
        top_row = tk.Frame(pad, bg=C["bg"])
        top_row.pack(fill="x", pady=(0, 20))

        # ── Borrow card ──
        bc = tk.Frame(top_row, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
        bc.pack(side="left", fill="both", expand=True, padx=(0, 12))
        tk.Frame(bc, bg=C["card2"]).pack(fill="x")
        hdr = tk.Frame(bc, bg=C["card2"])
        hdr.pack(fill="x")
        tk.Label(hdr, text=" ↗  Borrow a Book", bg=C["card2"], fg=C["green"],
                 font=(FONT_FAMILY, 11, "bold")).pack(side="left", padx=16, pady=10)

        bf = tk.Frame(bc, bg=C["card"], padx=16, pady=16)
        bf.pack(fill="both")
        v_bmem = tk.StringVar(); v_bbook = tk.StringVar()

        # Member dropdown
        def get_member_opts():
            return [f"{m.member_id} — {m.name}" for m in load_members()]
        def get_book_opts():
            return [f"{b.book_id} — {b.title} (avail: {b.available_quantity})" for b in load_books()]

        tk.Label(bf, text="Member", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(anchor="w")
        mem_cb = styled_combo(bf, get_member_opts(), textvariable=v_bmem, width=38)
        mem_cb.pack(pady=(2, 10), anchor="w")
        tk.Label(bf, text="Book", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(anchor="w")
        book_cb = styled_combo(bf, get_book_opts(), textvariable=v_bbook, width=38)
        book_cb.pack(pady=(2, 14), anchor="w")

        borrow_status = tk.Label(bf, text="", bg=C["card"], fg=C["green"],
                                 font=(FONT_FAMILY, 10), wraplength=360)
        borrow_status.pack(anchor="w")

        def do_borrow():
            mem_str  = v_bmem.get()
            book_str = v_bbook.get()
            if not mem_str or not book_str:
                return messagebox.showerror("Error", "Select both member and book.")
            member_id = mem_str.split(" — ")[0]
            book_id   = book_str.split(" — ")[0]
            today     = date.today()
            due_date  = today + timedelta(days=7)

            members  = load_members()
            books    = load_books()
            m_obj    = next((m for m in members if m.member_id == member_id), None)
            b_obj    = next((b for b in books   if b.book_id   == book_id),   None)
            if not m_obj: return messagebox.showerror("Error", "Member not found.")
            if not b_obj: return messagebox.showerror("Error", "Book not found.")
            limit = 2 if m_obj.type == "Standard" else 5
            if len(m_obj.borrowed_books) >= limit:
                return messagebox.showwarning("Limit Reached",
                    f"Member already has {len(m_obj.borrowed_books)} books (limit: {limit}).")
            if b_obj.available_quantity <= 0:
                return messagebox.showwarning("Unavailable", "No available copies of this book.")

            txns = load_transactions()
            tid  = f"TXN{len(txns)+1:03d}"
            b_obj.available_quantity -= 1
            b_obj.borrow_quantity    += 1
            m_obj.borrowed_books.append(book_id)
            save_books(books); save_members(members)
            txns.append(Transaction(tid, member_id, book_id, today, due_date))
            save_transactions(txns)
            brs = load_borrow_records()
            brs.append(BorrowRecord(tid, member_id, book_id, today, due_date))
            save_borrow_records(brs)

            borrow_status.config(
                text=f"✓ Borrowed!  TxnID: {tid}  |  Due: {due_date}", fg=C["green"])
            mem_cb["values"] = get_member_opts()
            book_cb["values"] = get_book_opts()
            v_bmem.set(""); v_bbook.set("")
            refresh_active()

        styled_btn(bf, "  ↗ Confirm Borrow  ", do_borrow, color=C["green"]).pack(anchor="w", pady=8)

        # ── Return card ──
        rc = tk.Frame(top_row, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
        rc.pack(side="left", fill="both", expand=True)
        hdr2 = tk.Frame(rc, bg=C["card2"])
        hdr2.pack(fill="x")
        tk.Label(hdr2, text=" ↙  Return a Book", bg=C["card2"], fg=C["yellow"],
                 font=(FONT_FAMILY, 11, "bold")).pack(side="left", padx=16, pady=10)

        rf = tk.Frame(rc, bg=C["card"], padx=16, pady=16)
        rf.pack(fill="both")
        v_rmem = tk.StringVar(); v_rbook = tk.StringVar()

        def get_active_member_opts():
            brs = load_borrow_records()
            ids = list({br.member_id for br in brs})
            members = load_members()
            return [f"{m.member_id} — {m.name}" for m in members if m.member_id in ids]

        def get_borrowed_book_opts(mid=None):
            brs = load_borrow_records()
            books = load_books()
            if mid:
                ids = {br.book_id for br in brs if br.member_id == mid}
            else:
                ids = {br.book_id for br in brs}
            return [f"{b.book_id} — {b.title}" for b in books if b.book_id in ids]

        tk.Label(rf, text="Member", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(anchor="w")
        rmem_cb = styled_combo(rf, get_active_member_opts(), textvariable=v_rmem, width=38)
        rmem_cb.pack(pady=(2, 10), anchor="w")

        tk.Label(rf, text="Book", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(anchor="w")
        rbook_cb = styled_combo(rf, [], textvariable=v_rbook, width=38)
        rbook_cb.pack(pady=(2, 14), anchor="w")

        def on_member_select(e=None):
            mid = v_rmem.get().split(" — ")[0] if " — " in v_rmem.get() else None
            rbook_cb["values"] = get_borrowed_book_opts(mid)
            v_rbook.set("")

        rmem_cb.bind("<<ComboboxSelected>>", on_member_select)

        return_info = tk.Label(rf, text="", bg=C["card"], fg=C["yellow"],
                               font=(FONT_FAMILY, 10), wraplength=360, justify="left")
        return_info.pack(anchor="w")

        def do_return():
            mem_str  = v_rmem.get()
            book_str = v_rbook.get()
            if not mem_str or not book_str:
                return messagebox.showerror("Error", "Select both member and book.")
            member_id = mem_str.split(" — ")[0]
            book_id   = book_str.split(" — ")[0]
            today     = date.today()
            fine_per_day = 10

            brs  = load_borrow_records()
            br   = next((b for b in brs if b.member_id == member_id and b.book_id == book_id), None)
            if not br: return messagebox.showerror("Error", "No active borrow record found.")

            od   = (today - br.Due_Date).days
            fine = max(0, od * fine_per_day)
            info = (f"TxnID: {br.Transaction_ID}  |  Borrowed: {br.Borrow_Date}  |  Due: {br.Due_Date}\n"
                    f"Return Date: {today}  |  ")
            info += f"Fine: Rs. {fine}  ({od} days overdue)" if od > 0 else "On Time  ✓  Fine: Rs. 0"
            return_info.config(text=info, fg=C["red"] if od > 0 else C["green"])

            new_brs = [b for b in brs if not (b.member_id == member_id and b.book_id == book_id)]
            save_borrow_records(new_brs)
            members = load_members()
            for m in members:
                if m.member_id == member_id and book_id in m.borrowed_books:
                    m.borrowed_books.remove(book_id)
            save_members(members)
            books = load_books()
            for b in books:
                if b.book_id == book_id:
                    b.available_quantity += 1; b.borrow_quantity -= 1
            save_books(books)

            rmem_cb["values"] = get_active_member_opts()
            v_rmem.set(""); v_rbook.set("")
            rbook_cb["values"] = []
            refresh_active()

        styled_btn(rf, "  ↙ Confirm Return  ", do_return, color=C["yellow"],
                   text_color=C["bg"]).pack(anchor="w", pady=8)

        # ── Active borrowings table ──
        tk.Label(pad, text="Active Borrowings", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        cols = ["TxnID", "Member", "Book", "Borrow Date", "Due Date", "Status"]
        widths = [90, 110, 110, 110, 110, 110]
        af, atree = make_table(pad, cols, widths)
        af.pack(fill="both", expand=True)

        def refresh_active():
            for row in atree.get_children(): atree.delete(row)
            for i, br in enumerate(load_borrow_records()):
                od     = (date.today() - br.Due_Date).days
                status = f"Overdue {od}d" if od > 0 else f"{-od}d left"
                tag    = "odd" if i % 2 == 0 else "even"
                atree.insert("", "end",
                             values=(br.Transaction_ID, br.member_id, br.book_id,
                                     str(br.Borrow_Date), str(br.Due_Date), status),
                             tags=(tag,))

        refresh_active()

    # ─────────────────────────────────────────
    #  REPORTS
    # ─────────────────────────────────────────
    def _page_reports(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        books   = load_books()
        members = load_members()
        txns    = load_transactions()
        brs     = load_borrow_records()

        # Summary section
        tk.Label(pad, text="Library Summary", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 13, "bold")).pack(anchor="w", pady=(0, 12))
        stats_row = tk.Frame(pad, bg=C["bg"])
        stats_row.pack(fill="x", pady=(0, 20))
        for title, val, color in [
            ("Total Titles",        len(books),                    C["accent"]),
            ("Total Copies",        sum(b.quantity for b in books), C["accent2"]),
            ("Active Loans",        len(brs),                      C["yellow"]),
            ("Completed Returns",   len(txns) - len(brs),          C["green"]),
            ("Standard Members",    sum(1 for m in members if m.type=="Standard"), C["text2"]),
            ("Premium Members",     sum(1 for m in members if m.type=="Premium"),  C["accent"]),
        ]:
            c = stat_card(stats_row, title, val, color)
            c.pack(side="left", padx=(0, 12))

        tk.Frame(pad, bg=C["border"], height=1).pack(fill="x", pady=8)

        # Most popular
        row2 = tk.Frame(pad, bg=C["bg"])
        row2.pack(fill="x", pady=(0, 20))

        for title, items, key in [
            ("Most Borrowed Books",   txns, lambda t: t.book_id),
            ("Most Active Members",   txns, lambda t: t.member_id),
        ]:
            card = tk.Frame(row2, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
            card.pack(side="left", fill="both", expand=True, padx=(0, 12))
            tk.Frame(card, bg=C["card2"]).pack(fill="x")
            hdr = tk.Frame(card, bg=C["card2"])
            hdr.pack(fill="x")
            tk.Label(hdr, text=f"  {title}", bg=C["card2"], fg=C["accent"],
                     font=(FONT_FAMILY, 10, "bold")).pack(side="left", padx=12, pady=8)
            body = tk.Frame(card, bg=C["card"], padx=12, pady=12)
            body.pack(fill="both")
            counts = {}
            for t in items:
                k = key(t); counts[k] = counts.get(k, 0) + 1
            top5 = sorted(counts.items(), key=lambda x: -x[1])[:5]
            for rank, (k, cnt) in enumerate(top5, 1):
                row = tk.Frame(body, bg=C["card"])
                row.pack(fill="x", pady=2)
                color = [C["yellow"], C["text"], C["text2"]][min(rank-1, 2)]
                tk.Label(row, text=f"#{rank}", bg=C["card"], fg=color,
                         font=(FONT_FAMILY, 9, "bold"), width=3).pack(side="left")
                tk.Label(row, text=k, bg=C["card"], fg=C["text"],
                         font=(FONT_FAMILY, 10)).pack(side="left", padx=6)
                tk.Label(row, text=f"{cnt} txns", bg=C["card"], fg=C["text2"],
                         font=(FONT_FAMILY, 9)).pack(side="right")

        # Genre distribution
        tk.Label(pad, text="Genre Distribution", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        genres = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]
        colors = [C["accent"], C["accent2"], C["green"], C["yellow"], C["red"]]
        total = len(books) or 1
        for g, col in zip(genres, colors):
            cnt = sum(1 for b in books if b.genre == g)
            pct = cnt / total * 100
            row = tk.Frame(pad, bg=C["bg"])
            row.pack(fill="x", pady=3)
            tk.Label(row, text=g, bg=C["bg"], fg=C["text2"],
                     font=(FONT_FAMILY, 10), width=14, anchor="w").pack(side="left")
            bar_bg = tk.Frame(row, bg=C["card"], height=16)
            bar_bg.pack(side="left", fill="x", expand=True, padx=8)
            bar_bg.update_idletasks()
            tk.Frame(bar_bg, bg=col, height=16).place(x=0, y=0, relwidth=max(0.01, pct/100), relheight=1)
            tk.Label(row, text=f"{cnt} ({pct:.0f}%)", bg=C["bg"], fg=col,
                     font=(FONT_FAMILY, 9, "bold"), width=10).pack(side="left")

        # Monthly report
        tk.Frame(pad, bg=C["border"], height=1).pack(fill="x", pady=16)
        tk.Label(pad, text="Monthly Borrowing Report", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 11, "bold")).pack(anchor="w", pady=(0, 8))
        filt = tk.Frame(pad, bg=C["bg"])
        filt.pack(fill="x", pady=(0, 8))
        v_year  = tk.StringVar(value=str(date.today().year))
        v_month = tk.StringVar(value=str(date.today().month))
        tk.Label(filt, text="Year:", bg=C["bg"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(side="left")
        styled_entry(filt, textvariable=v_year, width=6).pack(side="left", padx=(4, 12))
        tk.Label(filt, text="Month:", bg=C["bg"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(side="left")
        styled_combo(filt, [str(i) for i in range(1, 13)],
                     textvariable=v_month, width=6).pack(side="left", padx=(4, 12))

        mcols = ["TxnID", "Member", "Book", "Borrow Date", "Due Date"]
        mwidths = [90, 110, 110, 130, 130]
        mf, mtree = make_table(pad, mcols, mwidths)
        mf.pack(fill="both", expand=True)

        def load_monthly():
            for row in mtree.get_children(): mtree.delete(row)
            try: yr = int(v_year.get()); mo = int(v_month.get())
            except: return
            for i, t in enumerate(txns):
                if t.Borrow_Date.year == yr and t.Borrow_Date.month == mo:
                    tag = "odd" if i % 2 == 0 else "even"
                    mtree.insert("", "end",
                                 values=(t.Transaction_ID, t.member_id, t.book_id,
                                         str(t.Borrow_Date), str(t.Due_Date)), tags=(tag,))

        styled_btn(filt, "  ▶ Load  ", load_monthly).pack(side="left")
        load_monthly()

    # ─────────────────────────────────────────
    #  SEARCH
    # ─────────────────────────────────────────
    def _page_search(self):
        p = self.content
        pad = tk.Frame(p, bg=C["bg"]); pad.pack(fill="both", expand=True, padx=28, pady=20)

        tk.Label(pad, text="Search", bg=C["bg"], fg=C["text"],
                 font=(FONT_FAMILY, 13, "bold")).pack(anchor="w", pady=(0, 16))

        search_card = tk.Frame(pad, bg=C["card"], highlightthickness=1, highlightbackground=C["border"])
        search_card.pack(fill="x", pady=(0, 20))
        sf = tk.Frame(search_card, bg=C["card"], padx=16, pady=16)
        sf.pack(fill="x")

        v_query = tk.StringVar()
        v_type  = tk.StringVar(value="Book Title")

        row = tk.Frame(sf, bg=C["card"])
        row.pack(fill="x")
        tk.Label(row, text="Search By:", bg=C["card"], fg=C["text2"],
                 font=(FONT_FAMILY, 9)).pack(side="left", padx=(0, 8))
        styled_combo(row, ["Book Title", "Book Author", "Book Genre", "Member Name"],
                     textvariable=v_type, width=18).pack(side="left", padx=(0, 12))
        styled_entry(row, textvariable=v_query, width=36).pack(side="left", padx=(0, 12))

        # Results
        cols  = ["Type", "ID", "Name/Title", "Detail 1", "Detail 2", "Detail 3"]
        widths= [80, 90, 200, 160, 120, 120]
        rf, rtree = make_table(pad, cols, widths)
        rf.pack(fill="both", expand=True)

        status = tk.Label(pad, text="", bg=C["bg"], fg=C["text2"], font=(FONT_FAMILY, 9))
        status.pack(anchor="w", pady=4)

        def do_search(*args):
            for row in rtree.get_children(): rtree.delete(row)
            q   = v_query.get().strip().lower()
            typ = v_type.get()
            if not q: return

            results = []
            if typ == "Book Title":
                results = [(b, "Book") for b in load_books() if q in b.title.lower()]
            elif typ == "Book Author":
                results = [(b, "Book") for b in load_books() if q in b.author.lower()]
            elif typ == "Book Genre":
                results = [(b, "Book") for b in load_books() if q in b.genre.lower()]
            elif typ == "Member Name":
                results = [(m, "Member") for m in load_members() if q in m.name.lower()]

            for i, (obj, kind) in enumerate(results):
                tag = "odd" if i % 2 == 0 else "even"
                if kind == "Book":
                    rtree.insert("", "end",
                                 values=(kind, obj.book_id, obj.title, obj.author, obj.genre,
                                         f"Avail: {obj.available_quantity}/{obj.quantity}"),
                                 tags=(tag,))
                else:
                    rtree.insert("", "end",
                                 values=(kind, obj.member_id, obj.name, obj.phone, obj.type,
                                         f"Borrowed: {len(obj.borrowed_books)}"),
                                 tags=(tag,))
            status.config(text=f"{len(results)} result(s) found.")

        v_query.trace("w", do_search)
        styled_btn(row, "  ⊕ Search  ", do_search).pack(side="left")


# ─────────────────────────────────────────────
#  ENTRY POINT
# ─────────────────────────────────────────────
if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
