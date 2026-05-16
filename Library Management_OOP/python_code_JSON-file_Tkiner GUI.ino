import json
import os
import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
from datetime import date, timedelta, datetime

# ─────────────────────────────────────────────────────────────
#  JSON FILE PATHS
# ─────────────────────────────────────────────────────────────
BOOKS_FILE          = "books.json"
MEMBERS_FILE        = "members.json"
TRANSACTIONS_FILE   = "transactions.json"
BORROW_RECORDS_FILE = "borrow_records.json"

# ─────────────────────────────────────────────────────────────
#  THEME & COLORS
# ─────────────────────────────────────────────────────────────
THEME = {
    "bg_dark":      "#0F1117",
    "bg_panel":     "#1A1D27",
    "bg_card":      "#22263A",
    "bg_input":     "#2A2E42",
    "accent":       "#6C63FF",
    "accent_hover": "#8B84FF",
    "accent2":      "#00D4AA",
    "accent3":      "#FF6B6B",
    "accent4":      "#FFB347",
    "text_primary": "#F0F0F8",
    "text_secondary":"#9098C0",
    "text_muted":   "#5A6080",
    "border":       "#2E3250",
    "success":      "#00D4AA",
    "warning":      "#FFB347",
    "danger":       "#FF6B6B",
    "sidebar_w":    220,
}

FONTS = {
    "title":    ("Helvetica", 22, "bold"),
    "heading":  ("Helvetica", 14, "bold"),
    "subhead":  ("Helvetica", 11, "bold"),
    "body":     ("Helvetica", 10),
    "small":    ("Helvetica", 9),
    "mono":     ("Courier", 10),
    "sidebar":  ("Helvetica", 11, "bold"),
    "stat":     ("Helvetica", 28, "bold"),
}

# ─────────────────────────────────────────────────────────────
#  JSON HELPERS
# ─────────────────────────────────────────────────────────────
def load_json(filepath):
    if not os.path.exists(filepath):
        return []
    try:
        with open(filepath, "r") as f:
            data = json.load(f)
        return data if isinstance(data, list) else []
    except:
        return []

def save_json(filepath, data):
    with open(filepath, "w") as f:
        json.dump(data, f, indent=4)

# ─────────────────────────────────────────────────────────────
#  DATA STORE
# ─────────────────────────────────────────────────────────────
class DataStore:
    def __init__(self):
        self.reload()

    def reload(self):
        self.books          = load_json(BOOKS_FILE)
        self.members        = load_json(MEMBERS_FILE)
        self.transactions   = load_json(TRANSACTIONS_FILE)
        self.borrow_records = load_json(BORROW_RECORDS_FILE)

    def save_books(self):        save_json(BOOKS_FILE,          self.books)
    def save_members(self):      save_json(MEMBERS_FILE,        self.members)
    def save_transactions(self): save_json(TRANSACTIONS_FILE,   self.transactions)
    def save_borrow_records(self):save_json(BORROW_RECORDS_FILE,self.borrow_records)

    def next_book_id(self):   return f"BK{len(self.books)+1:04d}"
    def next_member_id(self): return f"MEM{len(self.members)+1:04d}"
    def next_txn_id(self):    return f"TXN{len(self.transactions)+1:04d}"

DB = DataStore()

# ─────────────────────────────────────────────────────────────
#  STYLED WIDGETS
# ─────────────────────────────────────────────────────────────
class StyledButton(tk.Frame):
    def __init__(self, parent, text, command=None, color=None, width=120, height=36, icon="", **kwargs):
        bg = color or THEME["accent"]
        super().__init__(parent, bg=parent["bg"] if hasattr(parent,"cget") else THEME["bg_panel"], **kwargs)
        self.btn = tk.Label(
            self, text=f"{icon}  {text}" if icon else text,
            bg=bg, fg=THEME["text_primary"],
            font=FONTS["subhead"], cursor="hand2",
            width=width//8, height=1,
            relief="flat", padx=12, pady=6
        )
        self.btn.pack(fill="both", expand=True)
        self._color = bg
        self._hover = self._lighten(bg)
        self._cmd   = command
        self.btn.bind("<Button-1>", lambda e: command() if command else None)
        self.btn.bind("<Enter>", lambda e: self.btn.config(bg=self._hover))
        self.btn.bind("<Leave>", lambda e: self.btn.config(bg=self._color))

    def _lighten(self, hex_color):
        r,g,b = int(hex_color[1:3],16), int(hex_color[3:5],16), int(hex_color[5:7],16)
        r,g,b = min(255,r+30), min(255,g+30), min(255,b+30)
        return f"#{r:02x}{g:02x}{b:02x}"


class StyledEntry(tk.Frame):
    def __init__(self, parent, placeholder="", **kwargs):
        super().__init__(parent, bg=parent["bg"] if hasattr(parent,"cget") else THEME["bg_panel"],
                         highlightthickness=1, highlightbackground=THEME["border"],
                         highlightcolor=THEME["accent"])
        self.entry = tk.Entry(
            self, bg=THEME["bg_input"], fg=THEME["text_primary"],
            insertbackground=THEME["accent"], font=FONTS["body"],
            relief="flat", bd=0,
            **{k:v for k,v in kwargs.items() if k in ("show","width")}
        )
        self.entry.pack(fill="both", expand=True, padx=8, pady=6)
        if placeholder:
            self.entry.insert(0, placeholder)
            self.entry.config(fg=THEME["text_muted"])
            self.entry.bind("<FocusIn>",  lambda e: self._on_focus_in(placeholder))
            self.entry.bind("<FocusOut>", lambda e: self._on_focus_out(placeholder))
        self._ph = placeholder

    def _on_focus_in(self, ph):
        if self.entry.get() == ph:
            self.entry.delete(0, "end")
            self.entry.config(fg=THEME["text_primary"])

    def _on_focus_out(self, ph):
        if not self.entry.get():
            self.entry.insert(0, ph)
            self.entry.config(fg=THEME["text_muted"])

    def get(self):
        v = self.entry.get()
        return "" if v == self._ph else v

    def set(self, val):
        self.entry.delete(0,"end")
        self.entry.insert(0, val)
        self.entry.config(fg=THEME["text_primary"])

    def clear(self):
        self.entry.delete(0,"end")
        if self._ph:
            self.entry.insert(0, self._ph)
            self.entry.config(fg=THEME["text_muted"])


class StyledCombo(ttk.Combobox):
    def __init__(self, parent, values, **kwargs):
        style = ttk.Style()
        style.configure("Dark.TCombobox",
            fieldbackground=THEME["bg_input"],
            background=THEME["bg_input"],
            foreground=THEME["text_primary"],
            arrowcolor=THEME["accent"],
            bordercolor=THEME["border"],
            lightcolor=THEME["border"],
            darkcolor=THEME["border"],
        )
        super().__init__(parent, values=values, style="Dark.TCombobox",
                         font=FONTS["body"], state="readonly", **kwargs)


class StyledTable(tk.Frame):
    def __init__(self, parent, columns, **kwargs):
        super().__init__(parent, bg=THEME["bg_dark"], **kwargs)
        style = ttk.Style()
        style.configure("Dark.Treeview",
            background=THEME["bg_card"],
            foreground=THEME["text_primary"],
            fieldbackground=THEME["bg_card"],
            rowheight=34,
            font=FONTS["body"],
            borderwidth=0,
        )
        style.configure("Dark.Treeview.Heading",
            background=THEME["bg_input"],
            foreground=THEME["accent"],
            font=FONTS["subhead"],
            relief="flat",
            borderwidth=0,
        )
        style.map("Dark.Treeview",
            background=[("selected", THEME["accent"])],
            foreground=[("selected", "white")],
        )
        style.layout("Dark.Treeview", [('Dark.Treeview.treearea', {'sticky': 'nswe'})])

        self.tree = ttk.Treeview(self, columns=columns, show="headings",
                                  style="Dark.Treeview", selectmode="browse")
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, anchor="center", width=max(80, len(col)*10))

        sb = ttk.Scrollbar(self, orient="vertical", command=self.tree.yview)
        self.tree.configure(yscrollcommand=sb.set)
        self.tree.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")

    def clear(self):
        for item in self.tree.get_children():
            self.tree.delete(item)

    def insert(self, values, tag=""):
        self.tree.insert("", "end", values=values, tags=(tag,))
        self.tree.tag_configure("alt", background=THEME["bg_panel"])

    def selection(self):
        sel = self.tree.selection()
        return self.tree.item(sel[0], "values") if sel else None


# ─────────────────────────────────────────────────────────────
#  STAT CARD
# ─────────────────────────────────────────────────────────────
def stat_card(parent, title, value, color, icon=""):
    card = tk.Frame(parent, bg=THEME["bg_card"],
                    highlightthickness=1, highlightbackground=color)
    card.pack(side="left", fill="both", expand=True, padx=6, pady=4)

    top = tk.Frame(card, bg=THEME["bg_card"])
    top.pack(fill="x", padx=16, pady=(14,4))

    tk.Label(top, text=icon, bg=THEME["bg_card"], fg=color,
             font=("Helvetica", 20)).pack(side="left")
    tk.Label(top, text=title, bg=THEME["bg_card"],
             fg=THEME["text_secondary"], font=FONTS["small"]).pack(side="left", padx=8)

    tk.Label(card, text=str(value), bg=THEME["bg_card"],
             fg=color, font=FONTS["stat"]).pack(padx=16, pady=(0,14), anchor="w")
    return card


# ─────────────────────────────────────────────────────────────
#  SECTION LABEL
# ─────────────────────────────────────────────────────────────
def section_header(parent, text, bg=None):
    bg = bg or THEME["bg_panel"]
    f = tk.Frame(parent, bg=bg)
    f.pack(fill="x", pady=(16,8))
    tk.Label(f, text=text, bg=bg, fg=THEME["text_primary"],
             font=FONTS["heading"]).pack(side="left")
    tk.Frame(f, bg=THEME["border"], height=1).pack(side="left", fill="x",
                                                    expand=True, padx=(12,0), pady=6)
    return f


# ─────────────────────────────────────────────────────────────
#  NOTIFICATION TOAST
# ─────────────────────────────────────────────────────────────
class Toast:
    def __init__(self, root):
        self.root = root
        self._win = None

    def show(self, msg, kind="success"):
        if self._win:
            try: self._win.destroy()
            except: pass
        color = THEME["success"] if kind=="success" else \
                THEME["warning"] if kind=="warning" else THEME["danger"]
        icon  = "✓" if kind=="success" else "⚠" if kind=="warning" else "✗"

        self._win = tk.Toplevel(self.root)
        self._win.overrideredirect(True)
        self._win.attributes("-topmost", True)
        self._win.configure(bg=color)

        tk.Label(self._win, text=f"  {icon}  {msg}  ",
                 bg=color, fg="white", font=FONTS["subhead"],
                 pady=10, padx=8).pack()

        # Position bottom-right
        self.root.update_idletasks()
        w, h = 340, 44
        rx = self.root.winfo_x() + self.root.winfo_width()  - w - 20
        ry = self.root.winfo_y() + self.root.winfo_height() - h - 20
        self._win.geometry(f"{w}x{h}+{rx}+{ry}")
        self.root.after(3000, self._close)

    def _close(self):
        try:
            if self._win: self._win.destroy()
        except: pass


# ═════════════════════════════════════════════════════════════
#  PAGES
# ═════════════════════════════════════════════════════════════

# ─── DASHBOARD ───────────────────────────────────────────────
class DashboardPage(tk.Frame):
    def __init__(self, parent):
        super().__init__(parent, bg=THEME["bg_panel"])
        self._build()

    def refresh(self):
        for w in self.winfo_children(): w.destroy()
        self._build()

    def _build(self):
        DB.reload()

        # Header
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,8))
        tk.Label(hdr, text="📚  Library Dashboard", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")
        tk.Label(hdr, text=date.today().strftime("%A, %d %B %Y"),
                 bg=THEME["bg_panel"], fg=THEME["text_muted"],
                 font=FONTS["body"]).pack(side="right", pady=4)

        # Stat cards row
        row = tk.Frame(self, bg=THEME["bg_panel"])
        row.pack(fill="x", padx=22, pady=8)
        stat_card(row, "Total Books",    len(DB.books),          THEME["accent"],  "📖")
        stat_card(row, "Members",        len(DB.members),        THEME["accent2"], "👥")
        stat_card(row, "Active Borrows", len(DB.borrow_records), THEME["accent4"], "🔄")
        stat_card(row, "Transactions",   len(DB.transactions),   THEME["accent3"], "📋")

        # Two-column lower section
        cols = tk.Frame(self, bg=THEME["bg_panel"])
        cols.pack(fill="both", expand=True, padx=22, pady=8)

        left  = tk.Frame(cols, bg=THEME["bg_panel"])
        right = tk.Frame(cols, bg=THEME["bg_panel"])
        left.pack(side="left", fill="both", expand=True, padx=(0,8))
        right.pack(side="right", fill="both", expand=True)

        # Recent transactions
        section_header(left, "Recent Transactions", THEME["bg_panel"])
        tbl = StyledTable(left, ("Txn ID","Member","Book","Date","Due"))
        tbl.pack(fill="both", expand=True)
        for t in reversed(DB.transactions[-10:]):
            tbl.insert((t["Transaction_ID"], t["member_id"],
                        t["book_id"], t["Borrow_Date"], t["Due_Date"]))

        # Overdue
        section_header(right, "Overdue Books", THEME["bg_panel"])
        ovr = StyledTable(right, ("Member","Book","Due Date","Days Late"))
        ovr.pack(fill="both", expand=True)
        today = date.today()
        for br in DB.borrow_records:
            due = datetime.strptime(br["Due_Date"], "%Y-%m-%d").date()
            if due < today:
                late = (today - due).days
                ovr.insert((br["member_id"], br["book_id"],
                             br["Due_Date"], f"+{late} days"), tag="alt")

        # Genre distribution
        section_header(self, "Genre Distribution", THEME["bg_panel"])
        grow = tk.Frame(self, bg=THEME["bg_panel"])
        grow.pack(fill="x", padx=22, pady=(0,16))
        genres = ["Fiction","Non-Fiction","Science","History","Technology"]
        total  = max(len(DB.books), 1)
        for g in genres:
            cnt = sum(1 for b in DB.books if b.get("genre") == g)
            pct = cnt / total
            gf = tk.Frame(grow, bg=THEME["bg_panel"])
            gf.pack(fill="x", pady=3)
            tk.Label(gf, text=g, bg=THEME["bg_panel"],
                     fg=THEME["text_secondary"], font=FONTS["small"],
                     width=12, anchor="w").pack(side="left")
            bar_bg = tk.Frame(gf, bg=THEME["border"], height=14, width=300)
            bar_bg.pack(side="left", padx=8)
            bar_bg.pack_propagate(False)
            bar_fill = tk.Frame(bar_bg, bg=THEME["accent"],
                                width=int(300*pct), height=14)
            bar_fill.place(x=0, y=0, relheight=1)
            tk.Label(gf, text=f"{cnt} ({pct*100:.0f}%)",
                     bg=THEME["bg_panel"], fg=THEME["text_muted"],
                     font=FONTS["small"]).pack(side="left")


# ─── BOOKS PAGE ──────────────────────────────────────────────
class BooksPage(tk.Frame):
    def __init__(self, parent, toast):
        super().__init__(parent, bg=THEME["bg_panel"])
        self.toast = toast
        self._build()

    def refresh(self):
        self._load_table()

    def _build(self):
        # Header
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,0))
        tk.Label(hdr, text="📖  Book Management", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")

        # Search bar
        sf = tk.Frame(self, bg=THEME["bg_panel"])
        sf.pack(fill="x", padx=28, pady=12)
        tk.Label(sf, text="🔍", bg=THEME["bg_panel"],
                 fg=THEME["text_muted"], font=FONTS["heading"]).pack(side="left")
        self.search_var = tk.StringVar()
        self.search_var.trace("w", lambda *a: self._load_table())
        se = tk.Entry(sf, textvariable=self.search_var,
                      bg=THEME["bg_input"], fg=THEME["text_primary"],
                      insertbackground=THEME["accent"], font=FONTS["body"],
                      relief="flat", bd=0, width=36)
        se.pack(side="left", ipady=6, padx=8)

        btns = tk.Frame(sf, bg=THEME["bg_panel"])
        btns.pack(side="right")
        StyledButton(btns, "Add Book",    self._show_add_form, icon="＋").pack(side="left", padx=4)
        StyledButton(btns, "Edit",        self._edit_book,     color=THEME["accent2"], icon="✏").pack(side="left", padx=4)
        StyledButton(btns, "Remove",      self._remove_book,   color=THEME["danger"],  icon="🗑").pack(side="left", padx=4)
        StyledButton(btns, "Refresh",     self._load_table,    color=THEME["bg_card"], icon="↺").pack(side="left", padx=4)

        # Table
        main = tk.Frame(self, bg=THEME["bg_panel"])
        main.pack(fill="both", expand=True, padx=28, pady=(0,8))
        self.table = StyledTable(main, ("ID","Title","Author","Genre","Total","Available","Borrowed"))
        self.table.pack(fill="both", expand=True)

        # Form panel (hidden by default)
        self.form_frame = tk.Frame(self, bg=THEME["bg_card"],
                                   highlightthickness=1,
                                   highlightbackground=THEME["accent"])
        self._build_form()
        self._load_table()

    def _build_form(self):
        f = self.form_frame
        tk.Label(f, text="Book Details", bg=THEME["bg_card"],
                 fg=THEME["accent"], font=FONTS["heading"]).pack(pady=(16,8))

        fields = [("Title","title"),("Author","author"),("Quantity","qty")]
        self.fvars = {}
        for lbl, key in fields:
            row = tk.Frame(f, bg=THEME["bg_card"])
            row.pack(fill="x", padx=20, pady=4)
            tk.Label(row, text=lbl, bg=THEME["bg_card"],
                     fg=THEME["text_secondary"], font=FONTS["small"],
                     width=9, anchor="w").pack(side="left")
            e = StyledEntry(row)
            e.pack(side="left", fill="x", expand=True)
            self.fvars[key] = e

        row = tk.Frame(f, bg=THEME["bg_card"])
        row.pack(fill="x", padx=20, pady=4)
        tk.Label(row, text="Genre", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=9, anchor="w").pack(side="left")
        self.genre_var = tk.StringVar(value="Fiction")
        self.genre_cb = StyledCombo(row,
            values=["Fiction","Non-Fiction","Science","History","Technology"])
        self.genre_cb.set("Fiction")
        self.genre_cb.pack(side="left", fill="x", expand=True, ipady=4)

        btns = tk.Frame(f, bg=THEME["bg_card"])
        btns.pack(pady=14)
        StyledButton(btns, "Save",   self._save_book,   icon="💾").pack(side="left", padx=6)
        StyledButton(btns, "Cancel", self._hide_form,   color=THEME["bg_input"]).pack(side="left", padx=6)

        self._edit_id = None

    def _show_add_form(self):
        for k in self.fvars: self.fvars[k].clear()
        self.genre_cb.set("Fiction")
        self._edit_id = None
        self.form_frame.pack(fill="x", padx=28, pady=8)

    def _hide_form(self):
        self.form_frame.pack_forget()

    def _load_table(self, *_):
        DB.reload()
        self.table.clear()
        q = self.search_var.get().lower()
        for b in DB.books:
            if q and q not in b.get("title","").lower() and \
               q not in b.get("author","").lower() and \
               q not in b.get("genre","").lower():
                continue
            self.table.insert((
                b["book_id"], b["title"], b["author"], b["genre"],
                b["quantity"], b["available_quantity"], b["borrow_quantity"]
            ))

    def _save_book(self):
        title  = self.fvars["title"].get().strip()
        author = self.fvars["author"].get().strip()
        genre  = self.genre_cb.get()
        qty    = self.fvars["qty"].get().strip()

        if not all([title, author, qty]):
            self.toast.show("All fields are required", "error")
            return
        try: qty = int(qty)
        except:
            self.toast.show("Quantity must be a number", "error")
            return

        if self._edit_id:
            for b in DB.books:
                if b["book_id"] == self._edit_id:
                    diff = qty - b["quantity"]
                    b["title"]  = title
                    b["author"] = author
                    b["genre"]  = genre
                    b["quantity"] = qty
                    b["available_quantity"] = max(0, b["available_quantity"] + diff)
                    break
            self.toast.show("Book updated successfully")
        else:
            DB.books.append({
                "book_id": DB.next_book_id(),
                "title": title, "author": author, "genre": genre,
                "quantity": qty, "available_quantity": qty, "borrow_quantity": 0
            })
            self.toast.show("Book added successfully")

        DB.save_books()
        self._load_table()
        self._hide_form()

    def _edit_book(self):
        sel = self.table.selection()
        if not sel:
            self.toast.show("Select a book to edit", "warning")
            return
        bid = sel[0]
        for b in DB.books:
            if b["book_id"] == bid:
                self._edit_id = bid
                self.fvars["title"].set(b["title"])
                self.fvars["author"].set(b["author"])
                self.fvars["qty"].set(str(b["quantity"]))
                self.genre_cb.set(b.get("genre","Fiction"))
                self.form_frame.pack(fill="x", padx=28, pady=8)
                return

    def _remove_book(self):
        sel = self.table.selection()
        if not sel:
            self.toast.show("Select a book to remove", "warning")
            return
        bid = sel[0]
        if messagebox.askyesno("Confirm", f"Remove book {bid}?"):
            DB.books = [b for b in DB.books if b["book_id"] != bid]
            DB.save_books()
            self._load_table()
            self.toast.show("Book removed")


# ─── MEMBERS PAGE ────────────────────────────────────────────
class MembersPage(tk.Frame):
    def __init__(self, parent, toast):
        super().__init__(parent, bg=THEME["bg_panel"])
        self.toast = toast
        self._build()

    def refresh(self): self._load_table()

    def _build(self):
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,0))
        tk.Label(hdr, text="👥  Member Management", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")

        sf = tk.Frame(self, bg=THEME["bg_panel"])
        sf.pack(fill="x", padx=28, pady=12)
        tk.Label(sf, text="🔍", bg=THEME["bg_panel"],
                 fg=THEME["text_muted"], font=FONTS["heading"]).pack(side="left")
        self.search_var = tk.StringVar()
        self.search_var.trace("w", lambda *a: self._load_table())
        tk.Entry(sf, textvariable=self.search_var,
                 bg=THEME["bg_input"], fg=THEME["text_primary"],
                 insertbackground=THEME["accent"], font=FONTS["body"],
                 relief="flat", bd=0, width=36).pack(side="left", ipady=6, padx=8)

        btns = tk.Frame(sf, bg=THEME["bg_panel"])
        btns.pack(side="right")
        StyledButton(btns, "Add Member", self._show_form,   icon="＋").pack(side="left", padx=4)
        StyledButton(btns, "Remove",     self._remove_mem,  color=THEME["danger"], icon="🗑").pack(side="left", padx=4)
        StyledButton(btns, "Refresh",    self._load_table,  color=THEME["bg_card"], icon="↺").pack(side="left", padx=4)

        main = tk.Frame(self, bg=THEME["bg_panel"])
        main.pack(fill="both", expand=True, padx=28, pady=(0,8))
        self.table = StyledTable(main, ("ID","Name","Phone","Type","Books Borrowed"))
        self.table.pack(fill="both", expand=True)

        self.form_frame = tk.Frame(self, bg=THEME["bg_card"],
                                    highlightthickness=1,
                                    highlightbackground=THEME["accent"])
        self._build_form()
        self._load_table()

    def _build_form(self):
        f = self.form_frame
        tk.Label(f, text="Register Member", bg=THEME["bg_card"],
                 fg=THEME["accent"], font=FONTS["heading"]).pack(pady=(16,8))

        self.fname  = self._field(f, "Name")
        self.fphone = self._field(f, "Phone")

        row = tk.Frame(f, bg=THEME["bg_card"])
        row.pack(fill="x", padx=20, pady=4)
        tk.Label(row, text="Type", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=9, anchor="w").pack(side="left")
        self.type_cb = StyledCombo(row, values=["Standard","Premium"])
        self.type_cb.set("Standard")
        self.type_cb.pack(side="left", fill="x", expand=True, ipady=4)

        btns = tk.Frame(f, bg=THEME["bg_card"])
        btns.pack(pady=14)
        StyledButton(btns, "Register", self._save_member, icon="💾").pack(side="left", padx=6)
        StyledButton(btns, "Cancel",   self._hide_form,   color=THEME["bg_input"]).pack(side="left", padx=6)

    def _field(self, parent, label):
        row = tk.Frame(parent, bg=THEME["bg_card"])
        row.pack(fill="x", padx=20, pady=4)
        tk.Label(row, text=label, bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=9, anchor="w").pack(side="left")
        e = StyledEntry(row)
        e.pack(side="left", fill="x", expand=True)
        return e

    def _show_form(self):
        self.fname.clear(); self.fphone.clear()
        self.type_cb.set("Standard")
        self.form_frame.pack(fill="x", padx=28, pady=8)

    def _hide_form(self):
        self.form_frame.pack_forget()

    def _load_table(self, *_):
        DB.reload()
        self.table.clear()
        q = self.search_var.get().lower()
        for m in DB.members:
            if q and q not in m.get("name","").lower() and q not in m.get("member_id","").lower():
                continue
            self.table.insert((
                m["member_id"], m["name"], m["phone"],
                m["type"], len(m.get("borrowed_books",[]))
            ))

    def _save_member(self):
        name  = self.fname.get().strip()
        phone = self.fphone.get().strip()
        mtype = self.type_cb.get()
        if not name:
            self.toast.show("Name is required", "error"); return
        DB.members.append({
            "member_id": DB.next_member_id(),
            "name": name, "phone": phone,
            "type": mtype, "borrowed_books": []
        })
        DB.save_members()
        self._load_table()
        self._hide_form()
        self.toast.show(f"Member registered as {DB.members[-1]['member_id']}")

    def _remove_mem(self):
        sel = self.table.selection()
        if not sel:
            self.toast.show("Select a member", "warning"); return
        mid = sel[0]
        if messagebox.askyesno("Confirm", f"Remove member {mid}?"):
            DB.members = [m for m in DB.members if m["member_id"] != mid]
            DB.save_members()
            self._load_table()
            self.toast.show("Member removed")


# ─── BORROW / RETURN PAGE ────────────────────────────────────
class BorrowPage(tk.Frame):
    def __init__(self, parent, toast):
        super().__init__(parent, bg=THEME["bg_panel"])
        self.toast = toast
        self._build()

    def refresh(self): self._load_table()

    def _build(self):
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,0))
        tk.Label(hdr, text="🔄  Borrow & Return", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")

        # Borrow form
        bc = tk.Frame(self, bg=THEME["bg_card"],
                      highlightthickness=1, highlightbackground=THEME["border"])
        bc.pack(fill="x", padx=28, pady=16)

        tk.Label(bc, text="Issue Book", bg=THEME["bg_card"],
                 fg=THEME["accent"], font=FONTS["heading"]).pack(anchor="w", padx=18, pady=(14,4))

        rf = tk.Frame(bc, bg=THEME["bg_card"])
        rf.pack(fill="x", padx=18, pady=6)

        tk.Label(rf, text="Member ID", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=12, anchor="w").pack(side="left")
        self.borrow_mid = StyledEntry(rf, placeholder="MEM0001")
        self.borrow_mid.pack(side="left", padx=8)

        tk.Label(rf, text="Book ID", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=8, anchor="w").pack(side="left", padx=(16,0))
        self.borrow_bid = StyledEntry(rf, placeholder="BK0001")
        self.borrow_bid.pack(side="left", padx=8)

        StyledButton(rf, "Issue Book", self._borrow, icon="📤").pack(side="left", padx=16)

        # Return form
        rc = tk.Frame(self, bg=THEME["bg_card"],
                      highlightthickness=1, highlightbackground=THEME["border"])
        rc.pack(fill="x", padx=28, pady=(0,16))
        tk.Label(rc, text="Return Book", bg=THEME["bg_card"],
                 fg=THEME["accent2"], font=FONTS["heading"]).pack(anchor="w", padx=18, pady=(14,4))

        rrf = tk.Frame(rc, bg=THEME["bg_card"])
        rrf.pack(fill="x", padx=18, pady=6)

        tk.Label(rrf, text="Member ID", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=12, anchor="w").pack(side="left")
        self.ret_mid = StyledEntry(rrf, placeholder="MEM0001")
        self.ret_mid.pack(side="left", padx=8)

        tk.Label(rrf, text="Book ID", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"],
                 width=8, anchor="w").pack(side="left", padx=(16,0))
        self.ret_bid = StyledEntry(rrf, placeholder="BK0001")
        self.ret_bid.pack(side="left", padx=8)

        StyledButton(rrf, "Return Book", self._return, color=THEME["accent2"], icon="📥").pack(side="left", padx=16)

        # Active borrowings table
        section_header(self, "Active Borrowings", THEME["bg_panel"])
        main = tk.Frame(self, bg=THEME["bg_panel"])
        main.pack(fill="both", expand=True, padx=28, pady=(0,16))
        self.table = StyledTable(main, ("Txn ID","Member","Book","Borrow Date","Due Date","Status"))
        self.table.pack(fill="both", expand=True)
        self._load_table()

    def _load_table(self):
        DB.reload()
        self.table.clear()
        today = date.today()
        for br in DB.borrow_records:
            due   = datetime.strptime(br["Due_Date"], "%Y-%m-%d").date()
            late  = (today - due).days
            status = f"⚠ {late}d late" if late > 0 else f"✓ {abs(late)}d left"
            self.table.insert((
                br["Transaction_ID"], br["member_id"], br["book_id"],
                br["Borrow_Date"], br["Due_Date"], status
            ))

    def _borrow(self):
        DB.reload()
        mid = self.borrow_mid.get().strip()
        bid = self.borrow_bid.get().strip()

        member = next((m for m in DB.members if m["member_id"] == mid), None)
        book   = next((b for b in DB.books   if b["book_id"]   == bid), None)

        if not member:
            self.toast.show("Member not found", "error"); return
        if not book:
            self.toast.show("Book not found", "error"); return
        if int(book["available_quantity"]) <= 0:
            self.toast.show("No copies available", "warning"); return

        limit = 2 if member["type"] == "Standard" else 5
        if len(member["borrowed_books"]) >= limit:
            self.toast.show(f"Borrow limit ({limit}) reached", "warning"); return

        today    = date.today()
        due_date = today + timedelta(days=7)
        txn_id   = DB.next_txn_id()

        book["available_quantity"] = int(book["available_quantity"]) - 1
        book["borrow_quantity"]    = int(book["borrow_quantity"]) + 1
        member["borrowed_books"].append(bid)

        record = {"Transaction_ID": txn_id, "member_id": mid,
                  "book_id": bid, "Borrow_Date": str(today), "Due_Date": str(due_date)}
        DB.transactions.append(record)
        DB.borrow_records.append(record)

        DB.save_books(); DB.save_members()
        DB.save_transactions(); DB.save_borrow_records()

        self._load_table()
        self.borrow_mid.clear(); self.borrow_bid.clear()
        self.toast.show(f"Issued {bid} to {mid} — due {due_date}")

    def _return(self):
        DB.reload()
        mid = self.ret_mid.get().strip()
        bid = self.ret_bid.get().strip()
        today = date.today()

        rec = next((r for r in DB.borrow_records
                    if r["member_id"] == mid and r["book_id"] == bid), None)
        if not rec:
            self.toast.show("No active borrow record found", "error"); return

        due  = datetime.strptime(rec["Due_Date"], "%Y-%m-%d").date()
        late = (today - due).days
        fine = max(0, late * 10)

        DB.borrow_records = [r for r in DB.borrow_records
                             if not (r["member_id"]==mid and r["book_id"]==bid)]

        for m in DB.members:
            if m["member_id"] == mid and bid in m["borrowed_books"]:
                m["borrowed_books"].remove(bid)

        for b in DB.books:
            if b["book_id"] == bid:
                b["available_quantity"] = int(b["available_quantity"]) + 1
                b["borrow_quantity"]    = max(0, int(b["borrow_quantity"]) - 1)

        DB.save_books(); DB.save_members(); DB.save_borrow_records()
        self._load_table()
        self.ret_mid.clear(); self.ret_bid.clear()

        msg = f"Book returned" + (f" — Fine: Rs.{fine}" if fine > 0 else " — on time!")
        kind = "warning" if fine > 0 else "success"
        self.toast.show(msg, kind)


# ─── REPORTS PAGE ────────────────────────────────────────────
class ReportsPage(tk.Frame):
    def __init__(self, parent, toast):
        super().__init__(parent, bg=THEME["bg_panel"])
        self.toast = toast
        self._build()

    def refresh(self): pass

    def _build(self):
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,16))
        tk.Label(hdr, text="📊  Reports & Statistics", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")

        btns = tk.Frame(self, bg=THEME["bg_panel"])
        btns.pack(fill="x", padx=28, pady=8)
        StyledButton(btns, "Library Summary",     self._summary,       icon="🏛", width=160).pack(side="left", padx=4)
        StyledButton(btns, "Popular Books",       self._popular_books, color=THEME["accent2"], icon="🏆", width=160).pack(side="left", padx=4)
        StyledButton(btns, "Active Members",      self._pop_member,    color=THEME["accent4"], icon="⭐", width=160).pack(side="left", padx=4)
        StyledButton(btns, "Genre Distribution",  self._genre,         color="#9B59B6", icon="📚", width=160).pack(side="left", padx=4)

        # Month filter
        mf = tk.Frame(self, bg=THEME["bg_panel"])
        mf.pack(fill="x", padx=28, pady=4)
        tk.Label(mf, text="Monthly Report →  Year:", bg=THEME["bg_panel"],
                 fg=THEME["text_secondary"], font=FONTS["small"]).pack(side="left")
        self.year_e = StyledEntry(mf)
        self.year_e.set(str(date.today().year))
        self.year_e.pack(side="left", padx=6)
        tk.Label(mf, text="Month:", bg=THEME["bg_panel"],
                 fg=THEME["text_secondary"], font=FONTS["small"]).pack(side="left", padx=(12,0))
        self.month_e = StyledEntry(mf)
        self.month_e.set(str(date.today().month))
        self.month_e.pack(side="left", padx=6)
        StyledButton(mf, "Run Report", self._monthly, color=THEME["accent3"], icon="📅").pack(side="left", padx=10)

        # Output area
        section_header(self, "Report Output", THEME["bg_panel"])
        out_frame = tk.Frame(self, bg=THEME["bg_card"],
                             highlightthickness=1, highlightbackground=THEME["border"])
        out_frame.pack(fill="both", expand=True, padx=28, pady=(0,16))

        self.output = tk.Text(out_frame, bg=THEME["bg_card"], fg=THEME["text_primary"],
                              font=FONTS["mono"], relief="flat", bd=0,
                              insertbackground=THEME["accent"],
                              selectbackground=THEME["accent"],
                              state="disabled", wrap="none")
        sb = ttk.Scrollbar(out_frame, command=self.output.yview)
        self.output.configure(yscrollcommand=sb.set)
        self.output.pack(side="left", fill="both", expand=True, padx=12, pady=12)
        sb.pack(side="right", fill="y")

    def _write(self, text):
        self.output.configure(state="normal")
        self.output.delete("1.0","end")
        self.output.insert("end", text)
        self.output.configure(state="disabled")

    def _summary(self):
        DB.reload()
        lines = []
        lines.append("═"*50)
        lines.append("  LIBRARY SUMMARY REPORT")
        lines.append(f"  Generated: {datetime.now().strftime('%Y-%m-%d %H:%M')}")
        lines.append("═"*50)
        lines.append(f"\n  Total Book Titles    : {len(DB.books)}")
        total_copies = sum(b["quantity"] for b in DB.books)
        lines.append(f"  Total Copies         : {total_copies}")
        lines.append(f"  Currently Borrowed   : {len(DB.borrow_records)}")
        lines.append(f"  Available Copies     : {total_copies - len(DB.borrow_records)}")
        lines.append(f"\n  Total Members        : {len(DB.members)}")
        std = sum(1 for m in DB.members if m["type"]=="Standard")
        pre = sum(1 for m in DB.members if m["type"]=="Premium")
        lines.append(f"  Standard Members     : {std}")
        lines.append(f"  Premium Members      : {pre}")
        lines.append(f"\n  Total Transactions   : {len(DB.transactions)}")
        lines.append(f"  Active Borrowings    : {len(DB.borrow_records)}")
        lines.append(f"  Completed Returns    : {len(DB.transactions)-len(DB.borrow_records)}")
        lines.append("\n" + "─"*50)
        lines.append("  BOOKS DETAIL")
        lines.append("─"*50)
        for b in DB.books:
            lines.append(f"  [{b['book_id']}] {b['title']} by {b['author']}")
            lines.append(f"       Genre:{b['genre']}  Total:{b['quantity']}  Available:{b['available_quantity']}  Borrowed:{b['borrow_quantity']}")
        self._write("\n".join(lines))

    def _popular_books(self):
        DB.reload()
        if not DB.transactions:
            self._write("No transactions found."); return
        from collections import Counter
        counts = Counter(t["book_id"] for t in DB.transactions)
        lines = ["═"*50, "  MOST POPULAR BOOKS", "═"*50]
        for bid, cnt in counts.most_common(10):
            title = next((b["title"] for b in DB.books if b["book_id"]==bid), bid)
            lines.append(f"  [{bid}] {title:<30} — borrowed {cnt}x")
        self._write("\n".join(lines))

    def _pop_member(self):
        DB.reload()
        if not DB.transactions:
            self._write("No transactions found."); return
        from collections import Counter
        counts = Counter(t["member_id"] for t in DB.transactions)
        lines = ["═"*50, "  MOST ACTIVE MEMBERS", "═"*50]
        for mid, cnt in counts.most_common(10):
            name = next((m["name"] for m in DB.members if m["member_id"]==mid), mid)
            lines.append(f"  [{mid}] {name:<30} — {cnt} borrow(s)")
        self._write("\n".join(lines))

    def _genre(self):
        DB.reload()
        genres = ["Fiction","Non-Fiction","Science","History","Technology"]
        total  = max(len(DB.books), 1)
        lines  = ["═"*50, "  GENRE DISTRIBUTION", "═"*50, ""]
        for g in genres:
            cnt = sum(1 for b in DB.books if b.get("genre")==g)
            pct = cnt / total * 100
            bar = "█" * int(pct / 5)
            lines.append(f"  {g:<15}  {bar:<20}  {cnt} books  ({pct:.1f}%)")
        self._write("\n".join(lines))

    def _monthly(self):
        DB.reload()
        try:
            yr = int(self.year_e.get())
            mo = int(self.month_e.get())
        except:
            self.toast.show("Enter valid year and month", "error"); return
        recs = [t for t in DB.transactions
                if datetime.strptime(t["Borrow_Date"],"%Y-%m-%d").year==yr
                and datetime.strptime(t["Borrow_Date"],"%Y-%m-%d").month==mo]
        lines = ["═"*60, f"  MONTHLY REPORT: {yr}-{mo:02d}", f"  Total Transactions: {len(recs)}", "═"*60]
        for r in recs:
            lines.append(f"  {r['Transaction_ID']}  |  {r['member_id']}  |  {r['book_id']}  |  {r['Borrow_Date']}  →  {r['Due_Date']}")
        if not recs: lines.append("  No transactions for this period.")
        self._write("\n".join(lines))


# ─── SEARCH PAGE ─────────────────────────────────────────────
class SearchPage(tk.Frame):
    def __init__(self, parent, toast):
        super().__init__(parent, bg=THEME["bg_panel"])
        self.toast = toast
        self._build()

    def refresh(self): pass

    def _build(self):
        hdr = tk.Frame(self, bg=THEME["bg_panel"])
        hdr.pack(fill="x", padx=28, pady=(28,0))
        tk.Label(hdr, text="🔍  Search", bg=THEME["bg_panel"],
                 fg=THEME["text_primary"], font=FONTS["title"]).pack(side="left")

        sf = tk.Frame(self, bg=THEME["bg_card"],
                      highlightthickness=1, highlightbackground=THEME["border"])
        sf.pack(fill="x", padx=28, pady=18)

        inner = tk.Frame(sf, bg=THEME["bg_card"])
        inner.pack(fill="x", padx=18, pady=14)

        tk.Label(inner, text="Query:", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"]).pack(side="left")
        self.query = StyledEntry(inner, placeholder="Type to search...")
        self.query.pack(side="left", fill="x", expand=True, padx=10)

        tk.Label(inner, text="Search in:", bg=THEME["bg_card"],
                 fg=THEME["text_secondary"], font=FONTS["small"]).pack(side="left")
        self.scope = StyledCombo(inner,
            values=["Book Title","Book Author","Book Genre","Member Name","Member ID"])
        self.scope.set("Book Title")
        self.scope.pack(side="left", padx=8, ipady=4)

        StyledButton(inner, "Search", self._do_search, icon="🔍").pack(side="left", padx=10)

        section_header(self, "Results", THEME["bg_panel"])
        main = tk.Frame(self, bg=THEME["bg_panel"])
        main.pack(fill="both", expand=True, padx=28, pady=(0,16))
        self.table = StyledTable(main, ("ID","Field 1","Field 2","Field 3","Field 4"))
        self.table.pack(fill="both", expand=True)

        self.result_lbl = tk.Label(self, text="", bg=THEME["bg_panel"],
                                    fg=THEME["text_muted"], font=FONTS["small"])
        self.result_lbl.pack()

    def _do_search(self):
        DB.reload()
        q     = self.query.get().strip().lower()
        scope = self.scope.get()
        self.table.clear()
        results = []

        if scope == "Book Title":
            results = [(b["book_id"],b["title"],b["author"],b["genre"],str(b["available_quantity"]))
                       for b in DB.books if q in b.get("title","").lower()]
        elif scope == "Book Author":
            results = [(b["book_id"],b["title"],b["author"],b["genre"],str(b["available_quantity"]))
                       for b in DB.books if q in b.get("author","").lower()]
        elif scope == "Book Genre":
            results = [(b["book_id"],b["title"],b["author"],b["genre"],str(b["available_quantity"]))
                       for b in DB.books if q in b.get("genre","").lower()]
        elif scope == "Member Name":
            results = [(m["member_id"],m["name"],m["phone"],m["type"],str(len(m.get("borrowed_books",[]))))
                       for m in DB.members if q in m.get("name","").lower()]
        elif scope == "Member ID":
            results = [(m["member_id"],m["name"],m["phone"],m["type"],str(len(m.get("borrowed_books",[]))))
                       for m in DB.members if q in m.get("member_id","").lower()]

        for r in results:
            self.table.insert(r)
        self.result_lbl.config(text=f"{len(results)} result(s) found")
        if not results:
            self.toast.show("No results found", "warning")


# ═════════════════════════════════════════════════════════════
#  MAIN APPLICATION
# ═════════════════════════════════════════════════════════════
class LibraryApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Library Management System")
        self.geometry("1200x750")
        self.minsize(900, 600)
        self.configure(bg=THEME["bg_dark"])

        # DPI scaling hint
        try: self.tk.call("tk", "scaling", 1.3)
        except: pass

        self.toast    = Toast(self)
        self._pages   = {}
        self._cur_nav = None

        self._build_layout()
        self._show("dashboard")

    def _build_layout(self):
        # Sidebar
        self.sidebar = tk.Frame(self, bg=THEME["bg_panel"],
                                width=THEME["sidebar_w"])
        self.sidebar.pack(side="left", fill="y")
        self.sidebar.pack_propagate(False)

        # Logo area
        logo = tk.Frame(self.sidebar, bg=THEME["bg_panel"], height=70)
        logo.pack(fill="x")
        logo.pack_propagate(False)
        tk.Label(logo, text="📚 LibraOS", bg=THEME["bg_panel"],
                 fg=THEME["accent"], font=("Helvetica", 15, "bold")).pack(pady=20)

        # Divider
        tk.Frame(self.sidebar, bg=THEME["border"], height=1).pack(fill="x", padx=16)

        # Nav items
        nav_items = [
            ("dashboard", "⬛ Dashboard",   "dashboard"),
            ("books",     "📖 Books",        "books"),
            ("members",   "👥 Members",      "members"),
            ("borrow",    "🔄 Borrow/Return","borrow"),
            ("reports",   "📊 Reports",      "reports"),
            ("search",    "🔍 Search",       "search"),
        ]
        self._nav_btns = {}
        for key, label, _ in nav_items:
            btn = tk.Label(self.sidebar, text=label,
                           bg=THEME["bg_panel"], fg=THEME["text_secondary"],
                           font=FONTS["sidebar"], anchor="w",
                           padx=22, pady=12, cursor="hand2")
            btn.pack(fill="x")
            btn.bind("<Button-1>", lambda e, k=key: self._show(k))
            btn.bind("<Enter>",    lambda e, b=btn: b.config(bg=THEME["bg_card"]) if b != self._cur_nav else None)
            btn.bind("<Leave>",    lambda e, b=btn, k=key: b.config(bg=THEME["accent"] if k==self._active_key else THEME["bg_panel"]))
            self._nav_btns[key] = btn

        # Bottom: date/time
        tk.Frame(self.sidebar, bg=THEME["border"], height=1).pack(fill="x", padx=16, side="bottom", pady=4)
        self.clock_lbl = tk.Label(self.sidebar, text="", bg=THEME["bg_panel"],
                                   fg=THEME["text_muted"], font=FONTS["small"])
        self.clock_lbl.pack(side="bottom", pady=8)
        self._tick()

        # Content area
        self.content = tk.Frame(self, bg=THEME["bg_panel"])
        self.content.pack(side="right", fill="both", expand=True)

        # Instantiate pages
        self._pages["dashboard"] = DashboardPage(self.content)
        self._pages["books"]     = BooksPage(self.content, self.toast)
        self._pages["members"]   = MembersPage(self.content, self.toast)
        self._pages["borrow"]    = BorrowPage(self.content, self.toast)
        self._pages["reports"]   = ReportsPage(self.content, self.toast)
        self._pages["search"]    = SearchPage(self.content, self.toast)

        self._active_key = None

    def _show(self, key):
        for k, pg in self._pages.items():
            pg.pack_forget()

        self._pages[key].pack(fill="both", expand=True)
        self._pages[key].refresh()

        for k, btn in self._nav_btns.items():
            if k == key:
                btn.config(bg=THEME["accent"], fg="white")
            else:
                btn.config(bg=THEME["bg_panel"], fg=THEME["text_secondary"])

        self._active_key = key

    def _tick(self):
        now = datetime.now().strftime("%H:%M:%S\n%d %b %Y")
        self.clock_lbl.config(text=now)
        self.after(1000, self._tick)


# ─────────────────────────────────────────────────────────────
#  ENTRY POINT
# ─────────────────────────────────────────────────────────────
if __name__ == "__main__":
    app = LibraryApp()
    app.mainloop()
