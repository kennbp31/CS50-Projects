import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    #TODO Query portfolio for the user
    rows = db.execute("SELECT * FROM portfolio WHERE uid = :uid", uid=session["user_id"])

    #TODO Create a list of current stock price
    prices = []
    total_value = 0

    for row in rows:
       clookup = lookup(row["symbol"])
       price = clookup["price"]
       prices.append(price)
       total_value = total_value + (price * row["shares"])

    #TODO Calculate stock values

    # Get users cash available
    cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])
    # Load table with company info
    return render_template("index.html", rows=rows, cash=round(cash[0]["cash"],3), prices=prices, total=total_value)




@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        # Ensure symbol typed in
        if not request.form.get("symbol"):
                return apology("must provide stock symbol", 403)

        # Ensure # of shares enterd
        if not request.form.get("shares") or int(request.form.get("shares")) <= 0 or not float(request.form.get("shares")).is_integer() :
            return apology("Shares must be enterd as whole non-negative numbers", 403)

        #load local variable with stock information
        clookup = lookup(request.form.get("symbol"))

        # Ensure symbol found
        if not clookup:
            return apology("Stock Symbol Not Found", 403)

        # Multiply number of shares by cost per share
        total_price = float(clookup["price"]) * int(request.form.get("shares"))

        # If shares * cost <= users cash continue
        users_cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])
        if total_price > users_cash[0]["cash"]:
            return apology("Not enough cash!", 403)
        new_cash = users_cash[0]["cash"] - total_price

        # Create History and Portfolio tables if they do not exist already
        db.execute("CREATE TABLE IF NOT EXISTS portfolio (pid INTEGER PRIMARY KEY, uid INTEGER NOT NULL, symbol TEXT, stock_name TEXT, shares INTEGER NOT NULL, date_time TEXT, FOREIGN KEY (uid) REFERENCES users (id) )")
        db.execute("CREATE TABLE IF NOT EXISTS history (hid INTEGER PRIMARY KEY, uid INTEGER NOT NULL, symbol TEXT, stock_name TEXT, shares INTEGER NOT NULL, price_per_share REAL NOT NULL, total REAL NOT NULL, sold INTEGER NOT NULL, date_time TEXT, FOREIGN KEY (uid) REFERENCES users (id))")

        # Check if user already has stock for that company purchased, if they do add to their existing shares
        users_portfolio = db.execute("SELECT portfolio.pid, users.id, portfolio.shares FROM portfolio JOIN users on portfolio.uid = users.id WHERE users.id = :uid and portfolio.symbol = :symbol"
                                        , uid=session["user_id"], symbol=clookup["symbol"])

        if len(users_portfolio) != 0:
            db.execute("UPDATE portfolio SET shares = :shares, date_time = :date_time WHERE pid = :pid",
                        shares=users_portfolio[0]["shares"] + int(request.form.get("shares")) , date_time=datetime.now(), pid=users_portfolio[0]["pid"])
            db.execute("INSERT INTO history (uid, symbol, stock_name, shares, price_per_share, total, sold, date_time) VALUES (:uid, :symbol, :stock_name, :shares, :price_per_share, :total, :sold, :date_time)",
                        uid=session["user_id"], symbol=clookup["symbol"],stock_name=clookup["name"], shares=request.form.get("shares"), price_per_share=float(clookup["price"]), total=total_price, sold=0, date_time=datetime.now())

        else:# Insert values if company not purchased before
            db.execute("INSERT INTO portfolio (uid, symbol, stock_name, shares, date_time) VALUES (:uid, :symbol, :stock_name, :shares, :date_time)",
                        uid=session["user_id"], symbol=clookup["symbol"],stock_name=clookup["name"], shares=request.form.get("shares"),date_time=datetime.now())
        # Add purchase to history table (PK, User ID #, Stock Symbol, Stock Name, purchased or sold, # of shares, Total Transaction Value, Price Per Share, Date Time of Action)
            db.execute("INSERT INTO history (uid, symbol, stock_name, shares, price_per_share, total, sold, date_time) VALUES (:uid, :symbol, :stock_name, :shares, :price_per_share, :total, :sold, :date_time)",
                        uid=session["user_id"], symbol=clookup["symbol"],stock_name=clookup["name"], shares=request.form.get("shares"), price_per_share=float(clookup["price"]), total=total_price, sold=0, date_time=datetime.now())
        # TODO Decrease cash in user table
        db.execute("UPDATE users SET cash = :cash WHERE id = :uid",
                    cash=new_cash, uid=session["user_id"])
        # TODO Redirect to account page (/)

        return redirect("/"), flash("Bought Stocks!")


    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    #TODO Query history for the user
    rows = db.execute("SELECT * FROM history WHERE uid = :uid", uid=session["user_id"])

    # Ensure there is a history
    if len(rows) == 0:
            return apology("No history of sales", 403)

    # Pass data to the template
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        # Ensure the user provided a symbol
        if not request.form.get("symbol"):
            return apology("Symbol must be provided", 403)

        # Pass symbol to lookup function, returns name, price, and symbol
        clookup = lookup(request.form.get("symbol"))

        # Confirm symbol match was found
        if not clookup:
            return apology("Stock Symbol Not Found", 403)

        # Load table with company info
        return render_template("quoted.html", name=clookup["name"],price=clookup["price"],symbol=clookup["symbol"] )


    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Enxure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # If username already exists, toss an error
        if len(rows) != 0:
            return apology("Username already exists", 403)

        #Insert the new user and hashed password
        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)",
                       request.form.get("username"), generate_password_hash(request.form.get("password"))  )

        # Redirect user to home page
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":
        # Make sure symbol is selected
        if not request.form.get("symbol"):
            return apology("must select symbol", 403)

        # Make sure whole positive numer is typed in
        if not request.form.get("shares") or int(request.form.get("shares")) <= 0 or not float(request.form.get("shares")).is_integer() :
            return apology("Shares must be enterd as whole non-negative numbers", 403)

        # Query data to pull all relevant information for that symbol
        rows = db.execute("SELECT portfolio.*, users.cash FROM portfolio JOIN users ON portfolio.uid = users.id WHERE users.id = :uid AND portfolio.symbol = :symbol"
                    , uid=session["user_id"], symbol=request.form.get("symbol"))

        # Ensure user has that much stock
        if rows[0]["shares"] < int(request.form.get("shares")):
             return apology("You don't have that many shares", 403)

        # Load local variable with stock information
        clookup = lookup(request.form.get("symbol"))

        # Multiply number of shares by cost per share
        total_value = float(clookup["price"]) * int(request.form.get("shares"))

        # Decrease shares by that number of shares
        db.execute("UPDATE portfolio SET shares = :shares WHERE pid = :pid"
                    , shares=int(rows[0]["shares"]) - int(request.form.get("shares")), pid=rows[0]["pid"])

        # Increase cash by total value
        db.execute("UPDATE users SET cash = (cash + :cash) WHERE id = :uid"
                    ,cash=total_value, uid=session["user_id"])

        # Add record to history table
        db.execute("INSERT INTO history (uid, symbol, stock_name, shares, price_per_share, total, sold, date_time) VALUES (:uid, :symbol, :stock_name, :shares, :price_per_share, :total, :sold, :date_time)",
                        uid=session["user_id"], symbol=clookup["symbol"],stock_name=clookup["name"], shares=request.form.get("shares"), price_per_share=float(clookup["price"]), total=total_value, sold=1, date_time=datetime.now())

        # Redirect user to home page
        return redirect("/"), flash("Sold Stocks!")

    #populate list of stocks owned by user to display in web form
    symbols = db.execute("SELECT symbol FROM portfolio WHERE uid =:uid", uid=session["user_id"])

    return render_template("sell.html", symbols = symbols )


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
