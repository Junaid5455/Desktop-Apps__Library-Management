"""
City Library Management System — Modern Tkinter GUI
JSON-backed data layer (books.json, members.json,
borrow_records.json, Transaction_records.json)
Run: python library_gui.py
Requires: Python 3.10+  (stdlib only — tkinter, json, datetime)
"""

import json, os, tkinter as tk
from tkinter import ttk, messagebox
from datetime import date, timedelta, datetime
from collections import Counter

# ═══════════════════════════════════════════════════════════
#  FILE PATHS
# ═══════════════════════════════════════════════════════════
BOOK_FILE        = "books.json"
MEMBER_FILE      = "members.json"
BORROW_FILE      = "borrow_records.json"
TRANSACTION_FILE = "Transaction_records.json"

GENRE_LIST = ["Fiction", "Non-Fiction", "Science", "History", "Technology"]

def _seed(path, data):
    if not os.path.exists(path):
        with open(path, "w") as f:
            json.dump(data, f, indent=4)

_seed(BOOK_FILE, [
    {"Book ID":"BK001","Title":"The Python Handbook","Author":"John Smith","Genre":"Technology","Quantity":7},
    {"Book ID":"BK002","Title":"The Java Handbook",  "Author":"John Smith","Genre":"Technology","Quantity":5},
])
_seed(MEMBER_FILE, [
    {"Member ID":"MEM001","Name":"Ahmad Ali",     "Phone":"03001234567","Type":"Standard","Borrowed Books":[]},
    {"Member ID":"MEM002","Name":"Junaid Shabeer","Phone":"03234578123","Type":"Premium", "Borrowed Books":[]},
])
_seed(BORROW_FILE, [
    {"Transaction ID":"TXN001","Member ID":"MEM002","Book ID":"BK001",
     "Borrow Date":str(date(2026,3,1)),"Due Date":str(date(2026,3,8))},
])
_seed(TRANSACTION_FILE, [
    {"Transaction ID":"TXN001","Member ID":"MEM002","Book ID":"BK001",
     "Borrow Date":str(date(2026,3,1)),"Due Date":str(date(2026,3,8))},
])

# ─── CRUD helpers ───────────────────────────────────────────
def _load(path):
    with open(path) as f: return json.load(f)
def _save(path, data):
    with open(path,"w") as f: json.dump(data, f, indent=4)

def load_books():        return _load(BOOK_FILE)
def save_books(d):       _save(BOOK_FILE, d)
def load_members():      return _load(MEMBER_FILE)
def save_members(d):     _save(MEMBER_FILE, d)
def load_borrow():       return _load(BORROW_FILE)
def save_borrow(d):      _save(BORROW_FILE, d)
def load_transaction():  return _load(TRANSACTION_FILE)
def save_transaction(d): _save(TRANSACTION_FILE, d)

def next_book_id():
    bks = load_books()
    return f"BK{str(len(bks)+1).zfill(3)}"

def next_member_id():
    mem = load_members()
    return f"MEM{str(len(mem)+1).zfill(3)}"

def next_txn_id():
    tr = load_transaction()
    return f"TXN{str(len(tr)+1).zfill(3)}"

def api_add_book(title, author, genre, qty):
    bid = next_book_id()
    bks = load_books()
    bks.append({"Book ID":bid,"Title":title,"Author":author,"Genre":genre,"Quantity":qty})
    save_books(bks)
    return bid

def api_add_member(name, phone, mtype):
    mid = next_member_id()
    mem = load_members()
    mem.append({"Member ID":mid,"Name":name,"Phone":phone,"Type":mtype,"Borrowed Books":[]})
    save_members(mem)
    return mid

def api_remove_book(bid):
    bks = [b for b in load_books() if b["Book ID"] != bid]
    save_books(bks)

def api_remove_member(mid):
    mem = [m for m in load_members() if m["Member ID"] != mid]
    save_members(mem)

def api_borrow(mid, bid):
    mem   = load_members()
    books = load_books()
    BR    = load_borrow()
    TR    = load_transaction()
    today    = date.today()
    due_date = today + timedelta(days=7)
    member = next((m for m in mem   if m["Member ID"]==mid), None)
    book   = next((b for b in books if b["Book ID"]==bid),   None)
    if not member: return False, "Member not found."
    if not book:   return False, "Book not found."
    if book["Quantity"] < 1: return False, "Book out of stock."
    limit = 2 if member["Type"]=="Standard" else 4
    if len(member["Borrowed Books"]) >= limit:
        return False, f"Borrow limit ({limit}) reached for {member['Type']} members."
    tid = next_txn_id()
    book["Quantity"] -= 1
    member["Borrowed Books"].append(bid)
    rec = {"Transaction ID":tid,"Member ID":mid,"Book ID":bid,
           "Borrow Date":str(today),"Due Date":str(due_date)}
    BR.append(rec); TR.append(rec)
    save_books(books); save_members(mem); save_borrow(BR); save_transaction(TR)
    return True, {"tid":tid,"name":member["Name"],"mtype":member["Type"],
                  "title":book["Title"],"borrow":today,"due":due_date}

def api_return(mid, bid):
    BR    = load_borrow()
    mem   = load_members()
    books = load_books()
    today = date.today()
    fine_rate = 10
    rec = next((r for r in BR if r["Member ID"]==mid and r["Book ID"]==bid), None)
    if not rec: return False, "No active borrow record found."
    due  = datetime.strptime(rec["Due Date"],"%Y-%m-%d").date()
    late = max(0,(today-due).days)
    fine = late * fine_rate
    BR   = [r for r in BR if not (r["Member ID"]==mid and r["Book ID"]==bid)]
    for m in mem:
        if m["Member ID"]==mid and bid in m["Borrowed Books"]:
            m["Borrowed Books"].remove(bid)
    for b in books:
        if b["Book ID"]==bid:
            b["Quantity"] += 1
    save_borrow(BR); save_members(mem); save_books(books)
    return True, {"tid":rec["Transaction ID"],"borrow":rec["Borrow Date"],
                  "due":rec["Due Date"],"return":today,"fine":fine,"late":late}

# ═══════════════════════════════════════════════════════════
#  DESIGN TOKENS  — warm slate + amber gold theme
# ═══════════════════════════════════════════════════════════
C = {
    "bg":          "#13151C",
    "sidebar":     "#1A1D28",
    "nav_hover":   "#22263A",
    "nav_active":  "#1E2540",
    "card":        "#1E2236",
    "card_alt":    "#232840",
    "border":      "#2C3050",
    "input_bg":    "#111420",
    "text":        "#DDE4F5",
    "muted":       "#6B77A4",
    "accent":      "#F5A623",          # amber-gold
    "accent_dim":  "#B87A18",
    "blue":        "#4A8CF7",
    "green":       "#2DD4A0",
    "red":         "#E85454",
    "purple":      "#9B72F5",
    "row_odd":     "#1B1F33",
    "row_even":    "#1E2236",
    "sel":         "#2A3260",
}
F = {
    "h1":   ("Georgia",      20, "bold"),
    "h2":   ("Georgia",      13, "bold"),
    "nav":  ("Segoe UI",     10, "bold"),
    "body": ("Segoe UI",     10),
    "sm":   ("Segoe UI",      9),
    "tag":  ("Courier New",   9, "bold"),
    "stat": ("Georgia",      28, "bold"),
}
NAV = [
    ("⌂", "Dashboard",      "dash"),
    ("⊞", "Books",          "books"),
    ("⊙", "Members",        "members"),
    ("⇄", "Borrow & Return","borrow"),
    ("▤", "Reports",        "reports"),
    ("◎", "Search",         "search"),
]

# ═══════════════════════════════════════════════════════════
#  REUSABLE WIDGET HELPERS
# ═══════════════════════════════════════════════════════════
def scrollable(parent):
    outer = tk.Frame(parent, bg=C["bg"])
    outer.pack(fill="both", expand=True)
    canvas = tk.Canvas(outer, bg=C["bg"], highlightthickness=0)
    sb = ttk.Scrollbar(outer, orient="vertical", command=canvas.yview)
    frame = tk.Frame(canvas, bg=C["bg"])
    frame.bind("<Configure>", lambda e: canvas.config(scrollregion=canvas.bbox("all")))
    canvas.create_window((0,0), window=frame, anchor="nw")
    canvas.config(yscrollcommand=sb.set)
    canvas.pack(side="left", fill="both", expand=True)
    sb.pack(side="right", fill="y")
    def _scroll(e): canvas.yview_scroll(int(-1*(e.delta/120)), "units")
    canvas.bind_all("<MouseWheel>", _scroll)
    return frame

def entry(parent, w=30):
    e = tk.Entry(parent, width=w, bg=C["input_bg"], fg=C["text"],
                 insertbackground=C["text"], relief="flat",
                 font=F["body"], highlightthickness=1,
                 highlightbackground=C["border"], highlightcolor=C["accent"])
    return e

def combo(parent, values, w=28):
    s = ttk.Style(); s.configure("G.TCombobox",
        fieldbackground=C["input_bg"], background=C["input_bg"],
        foreground=C["text"], selectbackground=C["accent"])
    c = ttk.Combobox(parent, values=values, width=w,
                     font=F["body"], state="readonly", style="G.TCombobox")
    return c

def btn(parent, text, cmd, color=None, w=None):
    bg = color or C["accent"]
    kw = dict(text=text, command=cmd, bg=bg, fg="#111" if bg==C["accent"] else "#FFF",
              font=F["nav"], relief="flat", cursor="hand2", padx=14, pady=7, bd=0,
              activebackground=C["accent_dim"] if bg==C["accent"] else "#333",
              activeforeground="#111")
    if w: kw["width"] = w
    b = tk.Button(parent, **kw)
    dim = "#B87A18" if bg==C["accent"] else _darken(bg)
    b.bind("<Enter>", lambda e: b.config(bg=dim))
    b.bind("<Leave>", lambda e: b.config(bg=bg))
    return b

def _darken(hex_col):
    r,g,b2 = int(hex_col[1:3],16), int(hex_col[3:5],16), int(hex_col[5:7],16)
    return f"#{max(0,r-30):02x}{max(0,g-30):02x}{max(0,b2-30):02x}"

def divider(parent, padx=16):
    tk.Frame(parent, bg=C["border"], height=1).pack(fill="x", padx=padx)

def card_header(parent, text):
    tk.Label(parent, text=text, bg=C["card"], fg=C["text"],
             font=F["h2"]).pack(anchor="w", padx=18, pady=(14,6))
    divider(parent)

def lbl_field(parent, label, widget, row, col=0):
    tk.Label(parent, text=label, bg=C["card"], fg=C["muted"],
             font=F["sm"]).grid(row=row, column=col, sticky="w", padx=(0,0), pady=(0,3))
    widget.grid(row=row+1, column=col, sticky="ew", pady=(0,12))

def stat_card(parent, label, value, color, row, col):
    f = tk.Frame(parent, bg=C["card"])
    f.grid(row=row, column=col, sticky="nsew", padx=7, pady=7)
    tk.Label(f, text=label, bg=C["card"], fg=C["muted"], font=F["sm"]).pack(
        anchor="w", padx=16, pady=(14,2))
    tk.Label(f, text=str(value), bg=C["card"], fg=color, font=F["stat"]).pack(
        anchor="w", padx=16, pady=(0,14))

def table(parent, cols, rows, widths=None):
    style = ttk.Style()
    style.theme_use("default")
    style.configure("Lib.Treeview",
        background=C["card"], foreground=C["text"], rowheight=32,
        fieldbackground=C["card"], borderwidth=0, font=F["body"])
    style.configure("Lib.Treeview.Heading",
        background=C["card_alt"], foreground=C["muted"],
        borderwidth=0, font=F["sm"], relief="flat")
    style.map("Lib.Treeview",
        background=[("selected", C["sel"])],
        foreground=[("selected", C["accent"])])
    wrap = tk.Frame(parent, bg=C["card"])
    wrap.pack(fill="both", expand=True, padx=16, pady=10)
    tv = ttk.Treeview(wrap, columns=cols, show="headings",
                      style="Lib.Treeview", selectmode="browse")
    sb = ttk.Scrollbar(wrap, orient="vertical", command=tv.yview)
    tv.config(yscrollcommand=sb.set)
    for i,c in enumerate(cols):
        w = widths[i] if widths else 120
        tv.heading(c, text=c); tv.column(c, width=w, anchor="w", minwidth=40)
    for idx,row in enumerate(rows):
        tag = "odd" if idx%2 else "even"
        tv.insert("","end", values=row, tags=(tag,))
    tv.tag_configure("odd",  background=C["row_odd"])
    tv.tag_configure("even", background=C["row_even"])
    tv.pack(side="left", fill="both", expand=True)
    sb.pack(side="right", fill="y")
    return tv

def dialog(title, w=440, h=420):
    d = tk.Toplevel(); d.title(title)
    d.geometry(f"{w}x{h}"); d.configure(bg=C["card"])
    d.resizable(False,False); d.grab_set()
    tk.Label(d, text=title, bg=C["card"], fg=C["text"], font=F["h1"]).pack(
        anchor="w", padx=22, pady=(20,6))
    divider(d, 22)
    return d

# ═══════════════════════════════════════════════════════════
#  MAIN APPLICATION
# ═══════════════════════════════════════════════════════════
class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("City Library Management System")
        self.geometry("1300x780"); self.minsize(1100,680)
        self.config(bg=C["bg"])
        self._nav_refs: dict[str, tuple] = {}
        self._cur = ""
        self._build_shell()
        self._goto("dash")

    # ── Shell ──────────────────────────────────────────────
    def _build_shell(self):
        # Sidebar
        self.sb = tk.Frame(self, bg=C["sidebar"], width=228)
        self.sb.pack(side="left", fill="y"); self.sb.pack_propagate(False)

        logo = tk.Frame(self.sb, bg=C["sidebar"])
        logo.pack(fill="x")
        tk.Label(logo, text="📖  City Library", bg=C["sidebar"], fg=C["text"],
                 font=F["h1"]).pack(pady=(24,2), padx=18, anchor="w")
        tk.Label(logo, text="Management System", bg=C["sidebar"], fg=C["muted"],
                 font=F["sm"]).pack(padx=18, anchor="w")
        tk.Frame(self.sb, bg=C["border"], height=1).pack(fill="x", padx=14, pady=14)

        nav_wrap = tk.Frame(self.sb, bg=C["sidebar"])
        nav_wrap.pack(fill="x")
        for icon, label, key in NAV:
            self._make_nav(nav_wrap, icon, label, key)

        tk.Frame(self.sb, bg=C["border"], height=1).pack(
            fill="x", padx=14, side="bottom", pady=10)
        tk.Label(self.sb, text=f"© {date.today().year}  City Library",
                 bg=C["sidebar"], fg=C["muted"], font=F["sm"]).pack(
            side="bottom", pady=(0,12))

        # Right side
        right = tk.Frame(self, bg=C["bg"])
        right.pack(side="left", fill="both", expand=True)

        # Topbar
        top = tk.Frame(right, bg=C["sidebar"], height=54)
        top.pack(fill="x"); top.pack_propagate(False)
        self._page_lbl = tk.Label(top, text="", bg=C["sidebar"], fg=C["text"],
                                   font=F["h2"])
        self._page_lbl.pack(side="left", padx=22, pady=14)
        tk.Label(top, text=f"📅  {date.today().strftime('%d %b %Y')}",
                 bg=C["sidebar"], fg=C["muted"], font=F["body"]).pack(
            side="right", padx=22)
        tk.Frame(right, bg=C["border"], height=1).pack(fill="x")

        # Page area
        self.area = tk.Frame(right, bg=C["bg"])
        self.area.pack(fill="both", expand=True)

    def _make_nav(self, parent, icon, label, key):
        row = tk.Frame(parent, bg=C["sidebar"], cursor="hand2")
        row.pack(fill="x", padx=10, pady=2)
        bar = tk.Frame(row, bg=C["sidebar"], width=4)
        bar.pack(side="left", fill="y")
        lbl = tk.Label(row, text=f" {icon}  {label}", bg=C["sidebar"],
                       fg=C["muted"], font=F["nav"], anchor="w",
                       padx=10, pady=10, cursor="hand2")
        lbl.pack(fill="x", side="left", expand=True)
        self._nav_refs[key] = (row, lbl, bar)
        for w in (row, lbl):
            w.bind("<Button-1>", lambda e,k=key: self._goto(k))
            w.bind("<Enter>",  lambda e,r=row,l=lbl: (
                r.config(bg=C["nav_hover"]), l.config(bg=C["nav_hover"])))
            w.bind("<Leave>",  lambda e,r=row,l=lbl,k=key: (
                r.config(bg=C["nav_active"] if self._cur==k else C["sidebar"]),
                l.config(bg=C["nav_active"] if self._cur==k else C["sidebar"],
                         fg=C["accent"] if self._cur==k else C["muted"])))

    def _goto(self, key):
        self._cur = key
        names = {n[2]:n[1] for n in NAV}
        self._page_lbl.config(text=names.get(key,""))
        for k,(row,lbl,bar) in self._nav_refs.items():
            active = k==key
            row.config(bg=C["nav_active"] if active else C["sidebar"])
            lbl.config(bg=C["nav_active"] if active else C["sidebar"],
                       fg=C["accent"] if active else C["muted"])
            bar.config(bg=C["accent"] if active else C["sidebar"])
        for w in self.area.winfo_children(): w.destroy()
        {
            "dash":    self._pg_dash,
            "books":   self._pg_books,
            "members": self._pg_members,
            "borrow":  self._pg_borrow,
            "reports": self._pg_reports,
            "search":  self._pg_search,
        }[key]()

    # ══════════════════════════════════════════════════════
    #  DASHBOARD
    # ══════════════════════════════════════════════════════
    def _pg_dash(self):
        wrap = scrollable(self.area)
        for c in range(4): wrap.columnconfigure(c, weight=1)

        books = load_books(); mem  = load_members()
        BR    = load_borrow(); TR  = load_transaction()

        total_titles  = len(books)
        total_copies  = sum(b["Quantity"] for b in books)
        active_borrows= len(BR)
        total_mem     = len(mem)
        overdue_cnt   = sum(1 for r in BR
                            if datetime.strptime(r["Due Date"],"%Y-%m-%d").date()<date.today())

        stat_card(wrap,"Total Titles",     total_titles,   C["blue"],   0,0)
        stat_card(wrap,"Copies in Library",total_copies,   C["green"],  0,1)
        stat_card(wrap,"Active Borrows",   active_borrows, C["accent"], 0,2)
        stat_card(wrap,"Registered Members",total_mem,     C["purple"], 0,3)

        # Alert strip
        strip = tk.Frame(wrap, bg=C["card_alt"])
        strip.grid(row=1, column=0, columnspan=4, sticky="ew", padx=7, pady=4)
        premium = sum(1 for m in mem if m["Type"]=="Premium")
        pills = [
            (f"⚠  Overdue: {overdue_cnt}", C["red"]    if overdue_cnt else C["muted"]),
            (f"★  Premium: {premium}",      C["purple"]),
            (f"◈  Standard: {total_mem-premium}", C["blue"]),
            (f"✓  Returns: {len(TR)-active_borrows}", C["green"]),
        ]
        for txt,col in pills:
            tk.Label(strip, text=txt, bg=C["card_alt"], fg=col,
                     font=F["nav"], padx=18, pady=8).pack(side="left")

        # Recent txns
        c1 = tk.Frame(wrap, bg=C["card"])
        c1.grid(row=2, column=0, columnspan=4, sticky="nsew", padx=7, pady=7)
        card_header(c1,"📋  Recent Transactions")
        rcols = ("TXN ID","Member","Book","Borrow Date","Due Date")
        rdata = [(r["Transaction ID"],r["Member ID"],r["Book ID"],
                  r["Borrow Date"],r["Due Date"]) for r in TR[-10:][::-1]]
        table(c1, rcols, rdata, [100,100,100,120,120])

        # Genre + Low stock
        c2 = tk.Frame(wrap, bg=C["card"])
        c2.grid(row=3, column=0, columnspan=2, sticky="nsew", padx=7, pady=7)
        card_header(c2,"📚  Genre Distribution")
        total_b = len(books) or 1
        gdata = [(g, sum(1 for b in books if b["Genre"]==g),
                  f"{sum(1 for b in books if b['Genre']==g)/total_b*100:.0f}%")
                 for g in GENRE_LIST]
        table(c2,("Genre","Books","% Share"), gdata, [130,70,80])

        c3 = tk.Frame(wrap, bg=C["card"])
        c3.grid(row=3, column=2, columnspan=2, sticky="nsew", padx=7, pady=7)
        card_header(c3,"⚠  Low Stock  (≤ 2 copies)")
        low = [(b["Book ID"],b["Title"],b["Quantity"])
               for b in books if b["Quantity"]<=2]
        table(c3,("ID","Title","Copies"), low, [80,250,70])

    # ══════════════════════════════════════════════════════
    #  BOOKS
    # ══════════════════════════════════════════════════════
    def _pg_books(self):
        tools = tk.Frame(self.area, bg=C["bg"])
        tools.pack(fill="x", padx=16, pady=12)
        btn(tools,"＋  Add Book",    self._dlg_add_book).pack(side="left")
        btn(tools,"✕  Remove",       self._dlg_del_book, C["red"]).pack(side="left",padx=8)
        btn(tools,"⟳  Refresh", lambda: self._goto("books"), C["card_alt"]).pack(side="left")

        host = tk.Frame(self.area, bg=C["card"])
        host.pack(fill="both", expand=True, padx=16, pady=(0,16))
        card_header(host,"All Books")
        books = load_books()
        data  = [(b["Book ID"],b["Title"],b["Author"],b["Genre"],b["Quantity"])
                 for b in books]
        self._books_tv = table(host,
            ("Book ID","Title","Author","Genre","Qty"),
            data, [80,260,170,110,50])

    def _dlg_add_book(self):
        d = dialog("Add New Book", 440, 460)
        frm = tk.Frame(d, bg=C["card"]); frm.pack(padx=22, pady=16, fill="x")
        frm.columnconfigure(0, weight=1)
        e_t = entry(frm); e_a = entry(frm)
        e_g = combo(frm, GENRE_LIST); e_q = entry(frm, 10)
        for i,(lbl,w) in enumerate([("Title",e_t),("Author",e_a),
                                     ("Genre",e_g),("Quantity",e_q)]):
            lbl_field(frm, lbl, w, i*2)
        def go():
            t,a,g,q = e_t.get().strip(),e_a.get().strip(),e_g.get(),e_q.get().strip()
            if not all([t,a,g,q]): messagebox.showerror("Error","All fields required.",parent=d); return
            try: qty=int(q); assert qty>0
            except: messagebox.showerror("Error","Quantity must be a positive integer.",parent=d); return
            bid = api_add_book(t,a,g,qty)
            messagebox.showinfo("Added",f"Book added!\nID: {bid}",parent=d)
            d.destroy(); self._goto("books")
        btn(frm,"Add Book",go).grid(row=8,column=0,sticky="ew",pady=(6,0))

    def _dlg_del_book(self):
        sel = self._books_tv.selection()
        if not sel: messagebox.showwarning("Select","Click a book row first."); return
        v = self._books_tv.item(sel[0])["values"]
        if messagebox.askyesno("Confirm",f"Remove {v[0]} — {v[1]}?"):
            api_remove_book(v[0]); self._goto("books")

    # ══════════════════════════════════════════════════════
    #  MEMBERS
    # ══════════════════════════════════════════════════════
    def _pg_members(self):
        tools = tk.Frame(self.area, bg=C["bg"])
        tools.pack(fill="x", padx=16, pady=12)
        btn(tools,"＋  Register Member",self._dlg_add_member).pack(side="left")
        btn(tools,"✕  Remove",          self._dlg_del_member, C["red"]).pack(side="left",padx=8)
        btn(tools,"⟳  Refresh", lambda: self._goto("members"), C["card_alt"]).pack(side="left")

        host = tk.Frame(self.area, bg=C["card"])
        host.pack(fill="both", expand=True, padx=16, pady=(0,16))
        card_header(host,"All Members")
        mem  = load_members()
        data = [(m["Member ID"],m["Name"],m["Phone"],m["Type"],
                 ", ".join(m["Borrowed Books"]) or "—") for m in mem]
        self._mem_tv = table(host,
            ("Member ID","Name","Phone","Type","Borrowed Books"),
            data, [90,160,120,90,260])

    def _dlg_add_member(self):
        d = dialog("Register Member", 440, 380)
        frm = tk.Frame(d, bg=C["card"]); frm.pack(padx=22, pady=16, fill="x")
        frm.columnconfigure(0, weight=1)
        e_n = entry(frm); e_p = entry(frm)
        e_t = combo(frm, ["Standard","Premium"])
        for i,(lbl,w) in enumerate([("Full Name",e_n),("Phone",e_p),("Membership",e_t)]):
            lbl_field(frm, lbl, w, i*2)
        def go():
            n,p,t = e_n.get().strip(),e_p.get().strip(),e_t.get()
            if not all([n,p,t]): messagebox.showerror("Error","All fields required.",parent=d); return
            mid = api_add_member(n,p,t)
            messagebox.showinfo("Registered",f"Member added!\nID: {mid}",parent=d)
            d.destroy(); self._goto("members")
        btn(frm,"Register",go).grid(row=6,column=0,sticky="ew",pady=(6,0))

    def _dlg_del_member(self):
        sel = self._mem_tv.selection()
        if not sel: messagebox.showwarning("Select","Click a member row first."); return
        v = self._mem_tv.item(sel[0])["values"]
        if messagebox.askyesno("Confirm",f"Remove {v[0]} — {v[1]}?"):
            api_remove_member(v[0]); self._goto("members")

    # ══════════════════════════════════════════════════════
    #  BORROW & RETURN
    # ══════════════════════════════════════════════════════
    def _pg_borrow(self):
        top = tk.Frame(self.area, bg=C["bg"])
        top.pack(fill="x", padx=16, pady=12)
        top.columnconfigure((0,1), weight=1)

        # ── Borrow card ──────────────────────────
        bc = tk.Frame(top, bg=C["card"])
        bc.grid(row=0, column=0, sticky="nsew", padx=(0,8))
        card_header(bc,"📤  Borrow a Book")
        bf = tk.Frame(bc, bg=C["card"]); bf.pack(padx=18, pady=14, fill="x")
        bf.columnconfigure(0, weight=1)
        e_bm = entry(bf); e_bb = entry(bf)
        lbl_field(bf,"Member ID", e_bm, 0)
        lbl_field(bf,"Book ID",   e_bb, 2)
        bres = tk.Label(bc, text="", bg=C["card"], fg=C["green"],
                        font=F["sm"], wraplength=340, justify="left")
        bres.pack(padx=18, pady=(0,8))
        def do_borrow():
            ok,info = api_borrow(e_bm.get().strip(), e_bb.get().strip())
            if ok:
                bres.config(fg=C["green"],
                    text=(f"✓ Borrowed!\n"
                          f"TXN: {info['tid']}  |  {info['name']}  ({info['mtype']})\n"
                          f"Book: {info['title']}\n"
                          f"Borrowed: {info['borrow']}   Due: {info['due']}"))
                e_bm.delete(0,"end"); e_bb.delete(0,"end")
                self._refresh_active()
            else:
                bres.config(fg=C["red"], text=f"✗  {info}")
        btn(bf,"Borrow Book",do_borrow).grid(row=4,column=0,sticky="ew",pady=(4,0))

        # ── Return card ──────────────────────────
        rc = tk.Frame(top, bg=C["card"])
        rc.grid(row=0, column=1, sticky="nsew", padx=(8,0))
        card_header(rc,"📥  Return a Book")
        rf = tk.Frame(rc, bg=C["card"]); rf.pack(padx=18, pady=14, fill="x")
        rf.columnconfigure(0, weight=1)
        e_rm = entry(rf); e_rb = entry(rf)
        lbl_field(rf,"Member ID", e_rm, 0)
        lbl_field(rf,"Book ID",   e_rb, 2)
        rres = tk.Label(rc, text="", bg=C["card"], fg=C["green"],
                        font=F["sm"], wraplength=340, justify="left")
        rres.pack(padx=18, pady=(0,8))
        def do_return():
            ok,info = api_return(e_rm.get().strip(), e_rb.get().strip())
            if ok:
                fine_txt = (f"Rs. {info['fine']}  ({info['late']}d late)"
                            if info["fine"] else "Rs. 0  — on time ✓")
                rres.config(fg=C["green"] if not info["fine"] else C["accent"],
                    text=(f"✓ Returned!\n"
                          f"TXN: {info['tid']}\n"
                          f"Borrowed: {info['borrow']}   Due: {info['due']}\n"
                          f"Returned: {info['return']}   Fine: {fine_txt}"))
                e_rm.delete(0,"end"); e_rb.delete(0,"end")
                self._refresh_active()
            else:
                rres.config(fg=C["red"], text=f"✗  {info}")
        btn(rf,"Return Book",do_return, C["green"]).grid(row=4,column=0,sticky="ew",pady=(4,0))

        # ── Active borrows table ─────────────────
        self._borrow_host = tk.Frame(self.area, bg=C["card"])
        self._borrow_host.pack(fill="both", expand=True, padx=16, pady=(4,16))
        card_header(self._borrow_host,"📋  Active Borrows")
        self._active_table_frame = tk.Frame(self._borrow_host, bg=C["card"])
        self._active_table_frame.pack(fill="both", expand=True)
        self._refresh_active()

    def _refresh_active(self):
        for w in self._active_table_frame.winfo_children(): w.destroy()
        BR    = load_borrow()
        today = date.today()
        cols  = ("TXN","Member","Book","Borrow Date","Due Date","Status")
        data  = []
        for r in BR:
            due  = datetime.strptime(r["Due Date"],"%Y-%m-%d").date()
            late = (today-due).days
            st   = f"⚠ {late}d late" if late>0 else f"✓ {(due-today).days}d left"
            data.append((r["Transaction ID"],r["Member ID"],r["Book ID"],
                         r["Borrow Date"],r["Due Date"],st))
        table(self._active_table_frame, cols, data, [90,90,90,110,110,110])

    # ══════════════════════════════════════════════════════
    #  REPORTS
    # ══════════════════════════════════════════════════════
    def _pg_reports(self):
        wrap = scrollable(self.area)
        wrap.columnconfigure((0,1), weight=1)

        books = load_books(); mem = load_members()
        BR    = load_borrow(); TR = load_transaction()
        today = date.today()
        total_copies   = sum(b["Quantity"] for b in books)
        active_borrows = len(BR)
        premium        = sum(1 for m in mem if m["Type"]=="Premium")

        # Summary
        c0 = tk.Frame(wrap, bg=C["card"])
        c0.grid(row=0, column=0, columnspan=2, sticky="nsew", padx=7, pady=7)
        card_header(c0,"📊  Library Summary")
        sdata = [
            ("Total Book Titles",           len(books)),
            ("Total Copies",                total_copies),
            ("Currently Borrowed",          active_borrows),
            ("Available Copies",            total_copies - active_borrows),
            ("Total Members",               len(mem)),
            ("Premium Members",             premium),
            ("Standard Members",            len(mem)-premium),
            ("Total Transactions (all time)",len(TR)),
            ("Completed Returns",           len(TR)-active_borrows),
        ]
        table(c0,("Metric","Value"), sdata, [280,120])

        # Most borrowed
        c1 = tk.Frame(wrap, bg=C["card"])
        c1.grid(row=1, column=0, sticky="nsew", padx=7, pady=7)
        card_header(c1,"🏆  Most Borrowed Books")
        bk_cnt  = Counter(r["Book ID"] for r in TR)
        bk_map  = {b["Book ID"]:b["Title"] for b in books}
        bk_data = [(bid, bk_map.get(bid,"?"), cnt)
                   for bid,cnt in bk_cnt.most_common(10)]
        table(c1,("Book ID","Title","Borrows"), bk_data, [80,220,70])

        # Most active members
        c2 = tk.Frame(wrap, bg=C["card"])
        c2.grid(row=1, column=1, sticky="nsew", padx=7, pady=7)
        card_header(c2,"👤  Most Active Members")
        mm_cnt  = Counter(r["Member ID"] for r in TR)
        mm_map  = {m["Member ID"]:m["Name"] for m in mem}
        mm_data = [(mid, mm_map.get(mid,"?"), cnt)
                   for mid,cnt in mm_cnt.most_common(10)]
        table(c2,("Member ID","Name","Borrows"), mm_data, [90,160,70])

        # Genre
        c3 = tk.Frame(wrap, bg=C["card"])
        c3.grid(row=2, column=0, sticky="nsew", padx=7, pady=7)
        card_header(c3,"📚  Genre Distribution")
        total_b = len(books) or 1
        gdata = [(g, sum(1 for b in books if b["Genre"]==g),
                  f"{sum(1 for b in books if b['Genre']==g)/total_b*100:.1f}%")
                 for g in GENRE_LIST]
        table(c3,("Genre","Count","% Share"), gdata, [130,80,100])

        # Overdue
        c4 = tk.Frame(wrap, bg=C["card"])
        c4.grid(row=2, column=1, sticky="nsew", padx=7, pady=7)
        card_header(c4,"⚠  Overdue Books")
        od = [(r["Transaction ID"],r["Member ID"],r["Book ID"],r["Due Date"],
               f"{(today-datetime.strptime(r['Due Date'],'%Y-%m-%d').date()).days}d")
              for r in BR
              if datetime.strptime(r["Due Date"],"%Y-%m-%d").date()<today]
        table(c4,("TXN","Member","Book","Due","Days Late"), od, [90,90,90,110,80])

    # ══════════════════════════════════════════════════════
    #  SEARCH
    # ══════════════════════════════════════════════════════
    def _pg_search(self):
        bar = tk.Frame(self.area, bg=C["card"])
        bar.pack(fill="x", padx=16, pady=14)
        bar.columnconfigure(1, weight=1)

        tk.Label(bar, text="Search:", bg=C["card"], fg=C["muted"],
                 font=F["body"]).grid(row=0, column=0, padx=(16,8), pady=12)
        self._sq = tk.StringVar()
        se = tk.Entry(bar, textvariable=self._sq, bg=C["input_bg"], fg=C["text"],
                      insertbackground=C["text"], relief="flat", font=F["body"],
                      highlightthickness=1, highlightbackground=C["border"],
                      highlightcolor=C["accent"])
        se.grid(row=0, column=1, sticky="ew", ipady=6, pady=12, padx=(0,8))
        self._stype = combo(bar,["Books — Title","Books — Author",
                                 "Books — Genre","Members — Name"], 22)
        self._stype.current(0)
        self._stype.grid(row=0, column=2, padx=(0,8), pady=12)
        btn(bar,"Search",self._do_search).grid(row=0, column=3, padx=(0,16), pady=12)

        self._shost = tk.Frame(self.area, bg=C["card"])
        self._shost.pack(fill="both", expand=True, padx=16, pady=(0,16))
        card_header(self._shost,"Results")
        tk.Label(self._shost, text="Enter a query and press Search.",
                 bg=C["card"], fg=C["muted"], font=F["body"]).pack(pady=30)
        se.bind("<Return>", lambda e: self._do_search())

    def _do_search(self):
        q  = self._sq.get().strip().lower()
        st = self._stype.get()
        for w in self._shost.winfo_children(): w.destroy()
        card_header(self._shost, f'Results for "{q}"')
        if "Book" in st:
            fi = {"Books — Title":1,"Books — Author":2,"Books — Genre":3}.get(st,1)
            keys=["Book ID","Title","Author","Genre","Quantity"]
            rows = [(b["Book ID"],b["Title"],b["Author"],b["Genre"],b["Quantity"])
                    for b in load_books() if q in list(b.values())[fi-1+1].lower()]
            # fi maps: Title->Title, Author->Author, Genre->Genre (values()[1..3])
            books = load_books()
            rows = []
            for b in books:
                field_val = [b["Title"],b["Author"],b["Genre"]][
                    ["Books — Title","Books — Author","Books — Genre"].index(st)
                    if st in ["Books — Title","Books — Author","Books — Genre"] else 0]
                if q in field_val.lower():
                    rows.append((b["Book ID"],b["Title"],b["Author"],b["Genre"],b["Quantity"]))
            table(self._shost,("Book ID","Title","Author","Genre","Qty"),
                  rows,[80,240,160,110,50])
        else:
            mem = load_members()
            rows = [(m["Member ID"],m["Name"],m["Phone"],m["Type"],
                     ", ".join(m["Borrowed Books"]) or "—")
                    for m in mem if q in m["Name"].lower()]
            table(self._shost,("Member ID","Name","Phone","Type","Borrowed"),
                  rows,[90,160,120,90,200])
        if not rows:
            tk.Label(self._shost, text="No results found.",
                     bg=C["card"], fg=C["muted"], font=F["body"]).pack(pady=20)

# ═══════════════════════════════════════════════════════════
if __name__ == "__main__":
    App().mainloop()
